#include <errno.h>
#include <string.h>

#include "maracas.h"
#include "types.h"
#include "macros.h"

u32 label_to_int(const char label[4]) TOGGLE_H_IMPL({
	return *(u32*)label;
})

bool cmp_labels(const char a[4], const char b[4]) TOGGLE_H_IMPL({
	return *(u32*)a == *(u32*)b;
})

#define CLASS NAME(Slice)	\
	FIELD(PTR(u8), ptr)	\
	FIELD(u32, len)
#include "define_class.h"
#include "derive_debug.h"
#undef CLASS

#define CLASS NAME(Reader)	\
	FIELD(Slice, content)	\
	FIELD(PTR(u8), cursor)
#include "define_class.h"
#include "derive_debug.h"
#undef CLASS

void read_into(Reader* rd, u8* buffer, u32 len) TOGGLE_H_IMPL({
	memcpy(buffer, rd->cursor, len);
	rd->cursor += len;
})

Slice read_str(Reader* rd, u32 len) TOGGLE_H_IMPL({
	rd->cursor += len;
	return (Slice) {
		.ptr = rd->cursor,
		.len = len,
	};
})

char read_char(Reader* rd) TOGGLE_H_IMPL({
	return *rd->cursor++;
})

#define X(_, T, N) T read_ ## T(Reader* rd)	\
TOGGLE_H_IMPL({					\
	u8* buf = rd->cursor;			\
	rd->cursor += N;			\
	T res = 0;				\
	for (int i=0; i<N; i++) {		\
		res |= buf[i] << 8*(N-i-1);	\
	}					\
	return res;				\
})
FOR_ALL_UINTS
FOR_ALL_INTS
#undef X

#define CLASS NAME(FontHeader)		\
	FIELD(u32, scaler)		\
	FIELD(u16, num_tables)		\
	FIELD(u16, search_range)	\
	FIELD(u16, entry_selector)	\
	FIELD(u16, range_shift)
#include "define_class.h"
#include "derive_debug.h"
#include "derive_serde.h"
#undef CLASS

#define CLASS NAME(DirectoryEntry)	\
	FIELD(ARR(char, 4), tag)	\
	FIELD(u32, checksum)		\
	FIELD(u32, offset)		\
	FIELD(u32, length)
#include "define_class.h"
#include "derive_debug.h"
#include "derive_serde.h"
#undef CLASS

#define CLASS NAME(ivec2)	\
	FIELD(i32, x)		\
	FIELD(i32, y)
#include "define_class.h"
#undef CLASS
void debug_ivec2(ivec2* this) TOGGLE_H_IMPL({
	printf("(%d, %d)", this->x, this->y);
})
DERIVE_FMT(ivec2)
SET_FMT_INLINE(ivec2, true);

#define CLASS NAME(Point)	\
	FIELD(i16, x)		\
	FIELD(i16, y)		\
	FIELD(bool, is_end)
#include "define_class.h"
#include "derive_debug.h"
#undef CLASS

#define CLASS NAME(GlyphHeader)	\
	FIELD(i16, n_contours)	\
	FIELD(i16, x_min)	\
	FIELD(i16, y_min)	\
	FIELD(i16, x_max)	\
	FIELD(i16, y_max)
#include "define_class.h"
#include "derive_debug.h"
#include "derive_serde.h"
#undef CLASS

#define CLASS NAME(Glyph)		\
	FIELD(GlyphHeader, header)	\
	FIELD(VEC(Point), points)	\
	FIELD(VEC(u16), contours)
#include "define_class.h"
#include "derive_debug.h"
#undef CLASS


Slice read_file(const char* path) TOGGLE_H_IMPL({
	Slice ret = (Slice) {
		.ptr = NULL,
		.len = 0,
	};
	u8* buffer = NULL;

	FILE* file = fopen(path, "rb");
	if (!file) {
		MRC_ERROR("Could not open '%s': %s", path, strerror(errno));
		goto defer;
	}

	if (fseek(file, 0, SEEK_END)) {
		MRC_ERROR("Could not seek end");
		goto defer;
	}

	int file_size = ftell(file);
	if (fseek(file, 0, SEEK_SET)) {
		MRC_ERROR("Could not seek start");
		goto defer;
	}

	buffer = (u8*)malloc(file_size+1);
	if (!buffer) {
		MRC_ERROR("Could not allocate buffer to read file");
		goto defer;
	}

	int read_amount = fread(buffer, 1, file_size, file);
	if (read_amount != file_size) {
		MRC_ERROR("File size and bytes read differs: %d != %d", file_size, read_amount);
		goto defer;
	}

	buffer[read_amount] = '\0';
	ret.ptr = buffer;
	ret.len = file_size+1;
	fclose(file);
	MRC_INFO("Read %d bytes from '%s'", file_size, path);
	return ret;

defer:
	if (file) fclose(file);
	if (buffer) free(buffer);
	return ret;
})


Glyph read_Font(const char* path, int glyph_index) TOGGLE_H_IMPL({
	Slice content = read_file(path);
	if (!content.ptr) {
		MRC_ABORT("An error occured while reading '%s', aborting", path);
	}
	Reader rd = (Reader) {
		.content = content,
		.cursor = content.ptr,
	};

	FontHeader font = read_FontHeader(&rd);
	debug_FontHeader(&font);

	Reader glyf_rd = { 0 };
	DirectoryEntry entries[2];
	for (int i=0; i<font.num_tables; i++) {
		DirectoryEntry entry = read_DirectoryEntry(&rd);
		debug_fmt_DirectoryEntry(&entry, FMT_NAMES | FMT_FNL);
		if (cmp_labels(entry.tag, "glyf")) {
			MRC_INFO("Found glyf table");
			glyf_rd.content.ptr = glyf_rd.cursor = rd.content.ptr + entry.offset;
			glyf_rd.content.len = entry.length;
		}
		entries[0] = entries[1] = entry;
	}
	if (!glyf_rd.cursor) {
		MRC_ABORT("Could not find 'glyf' table in '%s', aborting", path);
	}

	Glyph glyph;
	for (int c=0; c<=glyph_index; c++) {
	GlyphHeader header = read_GlyphHeader(&glyf_rd);
	debug_GlyphHeader(&header);

	u16 n_contours = header.n_contours;
	u16* contours = vector_new(u16, n_contours);
	u16 n_points = 0;
	for (int i=0; i<n_contours; i++) {
		u16 end_point = read_u16(&glyf_rd);
		contours[i] = end_point;
		n_points = end_point+1;
	}
	vector_len(contours) = n_contours;
	debug_vector(u16, contours);

	u16 n_instructions = read_u16(&glyf_rd);
	MRC_DEBUG("n_instructions: %d", n_instructions);
	glyf_rd.cursor += n_instructions;

	u8* flags = vector_new(u8, n_points);
	for (int i=0; i<n_points; i++) {
		u8 flag = read_u8(&glyf_rd);
		u8 n_repeat = 0;
		if (flag & 1<<3) {
			n_repeat = read_u8(&glyf_rd);
		}
		for (int j=0; j<=n_repeat; j++) {
			flags[i+j] = flag;
			//points[i+j].on_curve = flag & 1;
		}
	}
	vector_len(flags) = n_points;
	debug_vector(u8, flags);

	Point* points = vector_new(Point, n_points+n_contours);
	i16 v = 0;
	for (int j=0; j<2; j++) {
		int contour = 0;
		for (int i=0; i<n_points; i++) {
			if (flags[i] & 1<<(1+j)) {
				u8 offset = read_u8(&glyf_rd);
				if (flags[i] & 1<<(4+j)) {
					v += offset;
				} else {
					v -= offset;
				}
			} else if (!(flags[i] & 1<<(4+j))) {
				i16 offset = read_i16(&glyf_rd);
				v += offset;
			}
			if (j == 0) {
				points[i+contour].x = v;
			} else {
				points[i+contour].y = v;
			}
			points[i+contour].is_end = 1;
			if (i >= contours[contour]) {
				contour++;
				if (j == 1) {
					points[i+contour] = contour-1 ? points[contours[contour-2]+contour] : points[0];
					points[i+contour].is_end = 0;
				}
			}
		}
	}
	vector_free(flags);
	vector_len(points) = n_points+n_contours;
	debug_vector(Point, points);

	glyph = (Glyph) {
		.header = header,
		.points = points,
		.contours = contours,
	};
	}
	return glyph;
})
