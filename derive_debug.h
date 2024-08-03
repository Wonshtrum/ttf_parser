#include "macros.h"

#define TAB_WIDTH 4
#define FMT_NAMES 0x10
#define FMT_NL 0x20
#define FMT_FNL 0x40
#define FMT_INLINE 0x80
#define FMT_NEXT(fmt) (((fmt)+1) & ~FMT_FNL)
#define FMT FMT_NAMES | FMT_NL | FMT_FNL | FMT_INLINE

#define debug_bool(x) printf(x ? "true" : "false")
#define debug_fmt_bool(x, _) debug_bool(x)
#define debug_char(x) printf("%c", *x)
#define debug_fmt_char(x, _) debug_char(x)

#define debug_u8(x) printf("%d", *x)
#define debug_fmt_u8(x, _) debug_u8(x)
#define debug_u16(x) printf("%d", *x)
#define debug_fmt_u16(x, _) debug_u16(x)
#define debug_u32(x) printf("%d", *x)
#define debug_fmt_u32(x, _) debug_u32(x)
#define debug_u64(x) printf("%d", *x)
#define debug_fmt_u64(x, _) debug_u64(x)

#define debug_i8(x) printf("%d", *x)
#define debug_fmt_i8(x, _) debug_i8(x)
#define debug_i16(x) printf("%d", *x)
#define debug_fmt_i16(x, _) debug_i16(x)
#define debug_i32(x) printf("%d", *x)
#define debug_fmt_i32(x, _) debug_i32(x)
#define debug_i64(x) printf("%d", *x)
#define debug_fmt_i64(x, _) debug_i64(x)

#define INLINE_char 1
#define INLINE_u8 1
#define INLINE_u16 1
#define INLINE_u32 1
#define INLINE_u64 1
#define INLINE_i8 1
#define INLINE_i16 1
#define INLINE_i32 1
#define INLINE_i64 1

#define debug_ptr(x) printf("%p", *x)
#define debug_fmt_ptr(x, _) debug_ptr(x)
#define debug_fmt_array(T, DIM, x, fmt)	{				\
	int has_nl = ((fmt) & FMT_NL) && (!INLINE_ ## T || !((fmt) & FMT_INLINE));\
	int has_fnl = (fmt) & FMT_FNL;					\
	int tabs = has_nl ? ((fmt)+1) & 0xF : 0;				\
	int ftabs = has_nl ? (fmt) & 0xF : 0;				\
	char sep = has_nl ? '\n' : ' ';					\
	printf("[%c", sep);						\
	for (int i=0; i<DIM; i++) {					\
		printf("%*s", TAB_WIDTH*tabs, "");			\
		debug_fmt_ ## T((x+i), FMT_NEXT(fmt));			\
		printf(",%c", sep);					\
	} printf("%*s]", TAB_WIDTH*ftabs, ""); if (has_fnl) printf("\n"); }
#define debug_array(T, DIM, x) debug_fmt_array(T, DIM, x, FMT)

#define NAME(N)	SET_FMT_INLINE(N, false)				\
	void debug_fmt_ ## N (N* this, int fmt);			\
	TOGGLE_H_IMPL(void debug_ ## N (N* this), { debug_fmt_ ## N (this, FMT); })\
	TOGGLE_H_IMPL(void debug_fmt_ ## N (N* this, int fmt), {	\
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

#ifdef H_IMPL
#define PTR(T) ptr
#define ARR(T, X) T, 1, X
#define FIELD_RAW(X)
#define FIELD(T, N) _FIELD(N, T, 0, 1)
#define _FIELD(N, T, IS_ARR, DIM, ...)					\
	if (has_names)							\
		printf("%*s" #N ": ", TAB_WIDTH*tabs, "");		\
	else								\
		printf("%*s", TAB_WIDTH*tabs, "");			\
	IF ## IS_ARR(							\
			debug_fmt_array(T, DIM, this->N, FMT_NEXT(fmt)),\
			debug_fmt_ ## T(&this->N, FMT_NEXT(fmt)));	\
	printf(",%c", sep);

CLASS printf("%*s}", TAB_WIDTH*ftabs, ""); if (has_fnl) printf("\n"); }
#else
#define PTR(T)
#define ARR(T, X)
#define FIELD_RAW(X)
#define FIELD(T, N)
CLASS
#endif

#undef NAME
#undef PTR
#undef ARR
#undef FIELD_RAW
#undef FIELD
#undef _FIELD
