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

Reader window_Reader(Reader* rd, u32 offset, u32 len) TOGGLE_H_IMPL({
	Reader new_rd;
	new_rd.content.ptr = new_rd.cursor = rd->content.ptr + offset;
	if (len) {
		new_rd.content.len = len;
	} else {
		new_rd.content.len = rd->content.len - offset;
	}
	return new_rd;
})

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

#define CLASS NAME(Fixed)	\
	FIELD(u16, a)		\
	FIELD(u16, b)
#include "define_class.h"
#include "derive_serde.h"
#undef CLASS
void debug_Fixed(Fixed* this) TOGGLE_H_IMPL({
	printf("(%d, %d)", this->a, this->b);
})
DERIVE_FMT(Fixed)
SET_FMT_INLINE(Fixed, true)

#define CLASS NAME(FontHeader)		\
	FIELD(u32, scaler)		\
	FIELD(u16, n_tables)		\
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

#define CLASS NAME(HeadTable)		\
	FIELD(Fixed, version)		\
	FIELD(Fixed, font_revision)	\
	FIELD(u32, check_sum)		\
	FIELD(u32, magic_number)	\
	FIELD(u16, flags)		\
	FIELD(u16, units_per_em)	\
	FIELD(i64, created)		\
	FIELD(i64, modified)		\
	FIELD(i16, x_min)		\
	FIELD(i16, y_min)		\
	FIELD(i16, x_max)		\
	FIELD(i16, y_max)		\
	FIELD(u16, mac_style)		\
	FIELD(u16, lowest_rec_ppem)	\
	FIELD(i16, direction_hint)	\
	FIELD(i16, loca_format)		\
	FIELD(i16, glyf_format)
#include "define_class.h"
#include "derive_debug.h"
#include "derive_serde.h"
#undef CLASS

#define CLASS NAME(MaxpTable)		\
	FIELD(Fixed, version)		\
	FIELD(u16, n_glyphs)		\
	FIELD(u16, max_points)		\
	FIELD(u16, max_contours)	\
	FIELD(u16, max_compound_contours)\
	FIELD(u16, max_zones)		\
	FIELD(u16, max_twilight_points)	\
	FIELD(u16, max_storage)		\
	FIELD(u16, max_function_defs)	\
	FIELD(u16, max_instruction_defs)\
	FIELD(u16, max_stack_elements)	\
	FIELD(u16, max_instruction_size)\
	FIELD(u16, max_compound_elements)\
	FIELD(u16, max_compound_depth)
#include "define_class.h"
#include "derive_debug.h"
#include "derive_serde.h"
#undef CLASS

#define CLASS NAME(CmapEntry)		\
	FIELD(u16, platform_id);	\
	FIELD(u16, specific_id);	\
	FIELD(u32, offset);
#include "define_class.h"
#include "derive_debug.h"
#include "derive_serde.h"
#undef CLASS

#define CLASS NAME(CmapGroup)	\
	FIELD(u32, start)	\
	FIELD(u32, end)		\
	FIELD(u32, code)
#include "define_class.h"
#include "derive_debug.h"
#include "derive_serde.h"
#undef CLASS

#define CLASS NAME(CmapTable)		\
	FIELD(u16, format)		\
	FIELD(VEC(CmapGroup), groups)	\
	FIELD(VEC(u16), glyph_index_array)
#include "define_class.h"
#include "derive_debug.h"
#undef CLASS

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

#define CLASS NAME(Font)		\
	FIELD(FontHeader, header)	\
	FIELD(HeadTable, head)		\
	FIELD(MaxpTable, maxp)		\
	FIELD(CmapTable, cmap)		\
	FIELD(VEC(u8), glyph_data)
#include "define_class.h"
#include "derive_debug.h"


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

u32* read_LocaTable(Reader* rd, int n_glyphs, bool long_format) TOGGLE_H_IMPL({
	u32* locations = vector_new(u32, n_glyphs);
	vector_len(locations) = n_glyphs;
	for (int i=0; i<n_glyphs; i++) {
		if (long_format) {
			locations[i] = read_u32(rd);
		} else {
			locations[i] = read_u16(rd);
		}
	}
	return locations;
})

CmapTable read_CmapTable(Reader* rd) TOGGLE_H_IMPL({
	u16 version = read_u16(rd);
	u16 n_tables = read_u16(rd);
	MRC_DEBUG("version: %d", version);
	MRC_DEBUG("n_tables: %d", n_tables);
	bool found = false;
	CmapEntry current = { 0 };
	for (int i=0; i<n_tables; i++) {
		CmapEntry entry = read_CmapEntry(rd);
		debug_fmt_CmapEntry(&entry, FMT_NAMES | FMT_FNL);
		u16 id = entry.specific_id;
		if (entry.platform_id == 0 && (id == 0 || id == 1 || id == 3 || id == 4) && (current.platform_id != 0 || id >= current.specific_id)) {
			current = entry;
			found = true;
		} else if (entry.platform_id == 3 && (id == 1 || id == 10) && (!found || current.platform_id == 3) && id > current.specific_id) {
			current = entry;
			found = true;
		}
	}
	MRC_ASSERT(found, "Unsupported cmap format");
	MRC_INFO("found: %d", found);
	debug_CmapEntry(&current);
	*rd = window_Reader(rd, current.offset, 0);
	u16 format = read_u16(rd);
	MRC_INFO("format: %d", format);
	switch (format) {
	case 4: {
		u16 length = read_u16(rd);
		u16 language = read_u16(rd);
		u16 seg_count_2 = read_u16(rd);
		u16 search_range = read_u16(rd);
		u16 entry_selector = read_u16(rd);
		u16 range_shift = read_u16(rd);
		MRC_DEBUG("length: %d", length);
		MRC_DEBUG("language: %d", language);
		MRC_DEBUG("seg_count_2: %d", seg_count_2);
		MRC_DEBUG("search_range: %d", search_range);
		MRC_DEBUG("entry_selector: %d", entry_selector);
		MRC_DEBUG("range_shift: %d", range_shift);
		MRC_ASSERT(language == 0);

		int n_groups = seg_count_2/2;
		CmapGroup* groups = vector_new(CmapGroup, n_groups);
		vector_len(groups) = n_groups;
		for (int i=0; i<n_groups; i++) {
			groups[i].end = read_u16(rd);
		}

		u16 reserved = read_u16(rd);
		MRC_ASSERT(reserved == 0);

		for (int i=0; i<n_groups; i++) {
			groups[i].start = read_u16(rd);
		}
		for (int i=0; i<n_groups; i++) {
			AS(Fixed, groups[i].code).a = read_u16(rd);
		}
		u16 max_offset = 0;
		for (int i=0; i<n_groups; i++) {
			u16 id_range_offset = read_u16(rd);
			if (id_range_offset) {
				MRC_ERROR("%d %d", id_range_offset/2, i-n_groups+id_range_offset/2);
				u16 offset = i - n_groups + id_range_offset/2;
				AS(Fixed, groups[i].code).b = offset;
				if (offset+groups[i].end-groups[i].start > max_offset) {
					max_offset = offset+groups[i].end-groups[i].start;
				}
			} else {
				AS(Fixed, groups[i].code).b = 0;
			}
		}
		//debug_fmt_vector(CmapGroup, groups, FMT & ~FMT_CAP);
		MRC_ERROR("max_offset: %d", max_offset);
		u16* glyph_index_array = vector_new(u16, max_offset);
		vector_len(glyph_index_array) = max_offset;
		for (int i=0; i<max_offset; i++) {
			glyph_index_array[i] = read_u16(rd);
		}

		return (CmapTable) {
			.format = 4,
			.groups = groups,
			.glyph_index_array = glyph_index_array,
		};
	}
	case 12: {
		u16 reserved = read_u16(rd);
		u32 length = read_u32(rd);
		u32 language = read_u32(rd);
		int n_groups = read_u32(rd);
		MRC_DEBUG("length: %d", length);
		MRC_DEBUG("language: %d", language);
		MRC_DEBUG("n_groups: %d", n_groups);
		MRC_ASSERT(reserved == 0);
		MRC_ASSERT(language == 0);

		CmapGroup* groups = vector_new(CmapGroup, n_groups);
		vector_len(groups) = n_groups;
		for (int i=0; i<n_groups; i++) {
			groups[i] = read_CmapGroup(rd);
		}
		return (CmapTable) {
			.format = 12,
			.groups = groups,
			.glyph_index_array = NULL,
		};
	}
	default: {
		MRC_ABORT("Unsupported cmap format: %d", format);
	}}
})

u32 get_glyph_location(u32 c, CmapTable* cmap, u32* locations) {
	MRC_WARN("Searching for %c: %d", c, c);
	CmapGroup* groups = cmap->groups;
	int length = vector_len(groups);
	for (int i=0; i<length; i++) {
		if (groups[i].start <= c && c <= groups[i].end) {
			int offset = c - groups[i].start;
			MRC_DEBUG("Found fitting group for %d (offset: %d):", c, offset);
			debug_CmapGroup(groups+i);
			switch (cmap->format) {
			case 4:
				u16 id_delta = AS(Fixed, groups[i].code).a;
				u16 id_range_offset = AS(Fixed, groups[i].code).b;
				MRC_DEBUG("%d %d %d", id_range_offset, id_delta, vector_len(locations));
				if (id_range_offset) {
					MRC_WARN("%d %d", id_range_offset+offset, cmap->glyph_index_array[id_range_offset+offset]);
					return locations[(cmap->glyph_index_array[id_range_offset + offset] + id_delta) & 0xFFFF];
				} else {
					return locations[(id_delta + c) & 0xFFFF];
				}
			case 12:
				return locations[groups[i].code + offset];
			default:
				MRC_ABORT("Wrong cmap format: %d, this should not happen, aborting", cmap->format);
			}
		}
	}
	MRC_DEBUG("No groups found for %d", c);
	return 0;
}

Glyph read_Font(const char* path, int glyph_index) TOGGLE_H_IMPL({
	Slice content = read_file(path);
	if (!content.ptr) {
		MRC_ABORT("An error occured while opening '%s', aborting", path);
	}
	Reader rd = (Reader) {
		.content = content,
		.cursor = content.ptr,
	};

	FontHeader font = read_FontHeader(&rd);
	debug_FontHeader(&font);

	Reader glyf_rd = { 0 };
	Reader head_rd = { 0 };
	Reader cmap_rd = { 0 };
	Reader maxp_rd = { 0 };
	Reader loca_rd = { 0 };
	for (int i=0; i<font.n_tables; i++) {
		DirectoryEntry entry = read_DirectoryEntry(&rd);
		debug_fmt_DirectoryEntry(&entry, FMT_NAMES | FMT_FNL);
		Reader entry_rd = window_Reader(&rd, entry.offset, entry.length);
		if (cmp_labels(entry.tag, "glyf")) {
			MRC_INFO("Found glyf table");
			glyf_rd = entry_rd;
		}
		if (cmp_labels(entry.tag, "cmap")) {
			MRC_INFO("Found cmap table");
			cmap_rd = entry_rd;
		}
		if (cmp_labels(entry.tag, "head")) {
			MRC_INFO("Found head table");
			head_rd = entry_rd;
		}
		if (cmp_labels(entry.tag, "maxp")) {
			MRC_INFO("Found maxp table");
			maxp_rd = entry_rd;
		}
		if (cmp_labels(entry.tag, "loca")) {
			MRC_INFO("Found loca table");
			loca_rd = entry_rd;
		}
	}
	MRC_ASSERT(glyf_rd.cursor, "Missing glyf table");
	MRC_ASSERT(head_rd.cursor, "Missing head table");
	MRC_ASSERT(cmap_rd.cursor, "Missing cmap table");
	MRC_ASSERT(maxp_rd.cursor, "Missing maxp table");
	MRC_ASSERT(loca_rd.cursor, "Missing loca table");

	HeadTable head = read_HeadTable(&head_rd);
	debug_HeadTable(&head);

	MaxpTable maxp = read_MaxpTable(&maxp_rd);
	debug_MaxpTable(&maxp);

	CmapTable cmap = read_CmapTable(&cmap_rd);
	if (!cmap.groups) {
		MRC_ABORT("An error occured while parsing the character map, aborting");
	}
	debug_CmapTable(&cmap);

	u32* locations = read_LocaTable(&loca_rd, maxp.n_glyphs, head.loca_format);
	if (!locations) {
		MRC_ABORT("An error occured while parsing the glyph locations, aborting");
	}
	debug_vector(u32, locations);

	u32 loc = get_glyph_location('y', &cmap, locations);
	MRC_DEBUG("Glyph location: %d", loc);

	free_CmapTable(&cmap);
	vector_free(locations);

	glyf_rd.cursor += loc;

	Glyph glyph;
	GlyphHeader header = read_GlyphHeader(&glyf_rd);
	debug_GlyphHeader(&header);

	u16 n_contours = header.n_contours;
	u16* contours = vector_new(u16, n_contours);
	vector_len(contours) = n_contours;
	u16 n_points = 0;
	for (int i=0; i<n_contours; i++) {
		u16 end_point = read_u16(&glyf_rd);
		contours[i] = end_point;
		n_points = end_point+1;
	}
	debug_vector(u16, contours);

	u16 n_instructions = read_u16(&glyf_rd);
	MRC_DEBUG("n_instructions: %d", n_instructions);
	glyf_rd.cursor += n_instructions;

	u8* flags = vector_new(u8, n_points);
	vector_len(flags) = n_points;
	for (int i=0; i<n_points;) {
		u8 flag = read_u8(&glyf_rd);
		u8 n_repeat = 1;
		if (flag & 1<<3) {
			n_repeat = 1+read_u8(&glyf_rd);
		}
		for (int j=0; j<n_repeat; j++) {
			flags[i+j] = flag;
			//points[i+j].on_curve = flag & 1;
		}
		i += n_repeat;
	}
	debug_vector(u8, flags);

	Point* points = vector_new(Point, n_points+n_contours);
	vector_len(points) = n_points+n_contours;
	for (int j=0; j<2; j++) {
		i16 v = 0;
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
			points[i+contour].is_end = 0;
			if (i >= contours[contour]) {
				contour++;
				if (j == 1) {
					points[i+contour] = contour-1 ? points[contours[contour-2]+contour] : points[0];
					points[i+contour].is_end = 1;
				}
			}
		}
	}
	vector_free(flags);
	//debug_fmt_vector(Point, points, FMT & ~FMT_CAP);
	debug_vector(Point, points);

	glyph = (Glyph) {
		.header = header,
		.points = points,
		.contours = contours,
	};
	return glyph;
})
