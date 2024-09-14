#include "macros.h"

#define FMT_NAMES 0x10
#define FMT_NL 0x20
#define FMT_FNL 0x40
#define FMT_INLINE 0x80
#define FMT_CAP 0x100

#define FMT_NEXT(fmt) (((fmt)+1) & ~FMT_FNL)
#define FMT (FMT_NAMES | FMT_NL | FMT_FNL | FMT_INLINE | FMT_CAP)

#define FMT_TAB_WIDTH 4
#define FMT_CAP_INLINE 10
#define FMT_CAP_COMPOUND 3

#define debug_bool(x) printf(*x ? "true" : "false")
#define debug_fmt_bool(x, _) debug_bool(x)
#define debug_char(x) printf("%c", *x)
#define debug_fmt_char(x, _) debug_char(x)
#define debug_ptr(x) printf("%p", *x)
#define debug_fmt_ptr(x, _) debug_ptr(x)

#define debug_u8(x) printf("%u", *x)
#define debug_fmt_u8(x, _) debug_u8(x)
#define debug_u16(x) printf("%u", *x)
#define debug_fmt_u16(x, _) debug_u16(x)
#define debug_u32(x) printf("%u", *x)
#define debug_fmt_u32(x, _) debug_u32(x)
#define debug_u64(x) printf("%lu", *x)
#define debug_fmt_u64(x, _) debug_u64(x)

#define debug_i8(x) printf("%d", *x)
#define debug_fmt_i8(x, _) debug_i8(x)
#define debug_i16(x) printf("%d", *x)
#define debug_fmt_i16(x, _) debug_i16(x)
#define debug_i32(x) printf("%d", *x)
#define debug_fmt_i32(x, _) debug_i32(x)
#define debug_i64(x) printf("%ld", *x)
#define debug_fmt_i64(x, _) debug_i64(x)

#define INLINE_bool 1
#define INLINE_char 1
#define INLINE_ptr 1

#define INLINE_u8 1
#define INLINE_u16 1
#define INLINE_u32 1
#define INLINE_u64 1
#define INLINE_i8 1
#define INLINE_i16 1
#define INLINE_i32 1
#define INLINE_i64 1

#define debug_fmt_array(T, DIM, x, fmt)	STATEMENT(			\
	int has_nl = ((fmt) & FMT_NL) && (!INLINE_ ## T || !((fmt) & FMT_INLINE));\
	int has_fnl = (fmt) & FMT_FNL;					\
	int cap = (fmt) & FMT_CAP ? INLINE_ ## T ? FMT_CAP_INLINE : FMT_CAP_COMPOUND : 0;\
	int tabs = has_nl ? ((fmt)+1) & 0xF : 0;			\
	int ftabs = has_nl ? (fmt) & 0xF : 0;				\
	char sep = has_nl ? '\n' : ' ';					\
	printf("[%c", sep);						\
	for (int i=0; i<DIM; i++) {					\
		printf("%*s", FMT_TAB_WIDTH*tabs, "");			\
		if (cap && i >= cap)					\
			{ printf("...%c", sep); break; }		\
		debug_fmt_ ## T((x+i), FMT_NEXT(fmt));			\
		printf(",%c", sep);					\
	} printf("%*s]", FMT_TAB_WIDTH*ftabs, ""); if (has_fnl) printf("\n"); )
#define debug_array(T, DIM, x) debug_fmt_array(T, DIM, x, FMT)

#define debug_fmt_vector(T, x, fmt) STATEMENT( if (!x) printf("vec(0)\n"); else {\
	int length = ((VecHeader*)x)[-1].len;				\
	if ((fmt) & FMT_NAMES)						\
		printf("vec(%d, %d)", vector_len(x), vector_cap(x));	\
	debug_fmt_array(T, length, x, fmt);})
#define debug_vector(T, x) debug_fmt_vector(T, x, FMT)

#define NAME(N)	SET_FMT_INLINE(N, false)				\
	void debug_fmt_ ## N (N* this, int fmt);			\
	void debug_ ## N (N* this) TOGGLE_H_IMPL({ debug_fmt_ ## N (this, FMT); })\
	void debug_fmt_ ## N (N* this, int fmt) TOGGLE_H_IMPL({		\
		int has_nl = fmt & FMT_NL;				\
		int has_fnl = fmt & FMT_FNL;				\
		int has_names = fmt & FMT_NAMES;			\
		int tabs = has_nl ? (fmt+1) & 0xF : 0;			\
		int ftabs = has_nl ? fmt & 0xF : 0;			\
		char sep = has_nl ? '\n' : ' ';				\
		if (has_names)						\
			printf(#N " {%c", sep);				\
		else							\
			printf("{%c", sep);)

#define PTR(T) ptr
#define VEC(T) T, VECTOR, ?
#define ARR(T, X) T, ARRAY, X
#define FIELD_RAW(X)
#define FIELD(T, N) TOGGLE_H_IMPL(_FIELD(N, T, SCALAR, 1))
#define _FIELD(N, T, CONTAINER, DIM, ...)				\
	if (has_names)							\
		printf("%*s" #N ": ", FMT_TAB_WIDTH*tabs, "");		\
	else								\
		printf("%*s", FMT_TAB_WIDTH*tabs, "");			\
	IF ## CONTAINER ## IS ## SCALAR (				\
		debug_fmt_ ## T(&this->N, FMT_NEXT(fmt)));		\
	IF ## CONTAINER ## IS ## ARRAY (				\
		debug_fmt_array(T, DIM, this->N, FMT_NEXT(fmt)));	\
	IF ## CONTAINER ## IS ## VECTOR (				\
		debug_fmt_vector(T, this->N, FMT_NEXT(fmt)));		\
	printf(",%c", sep);

CLASS TOGGLE_H_IMPL(printf("%*s}", FMT_TAB_WIDTH*ftabs, ""); if (has_fnl) printf("\n"); })

#undef NAME
#undef PTR
#undef VEC
#undef ARR
#undef FIELD_RAW
#undef FIELD
#undef _FIELD
