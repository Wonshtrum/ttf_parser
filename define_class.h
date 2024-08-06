#include "macros.h"

#define free_bool(x)
#define free_char(x)
#define free_ptr(x)

#define free_u8(x)
#define free_u16(x)
#define free_u32(x)
#define free_u64(x)
#define free_i8(x)
#define free_i16(x)
#define free_i32(x)
#define free_i64(x)

#define NAME(N) typedef struct N N; struct N {
#define PTR(T) T*
#define VEC(T) T*
#define ARR(T, X) T, ARRAY, X
#define FIELD_RAW(X) X;
#define FIELD(T, N) _FIELD(N, T, SCALAR, 1);
#define _FIELD(N, T, CONTAINER, DIM, ...) T N IF ## CONTAINER ## IS ## ARRAY ([DIM])
CLASS };

#undef NAME
#undef PTR
#undef VEC
#undef ARR
#undef FIELD_RAW
#undef FIELD

#define NAME(N) void free_ ## N (N* this) TOGGLE_H_IMPL ({ (void)this;)
#ifdef H_IMPL
#define PTR(T) ptr
#define VEC(T) T, VECTOR, ?
#define ARR(T, X) T, ARRAY, X
#define FIELD_RAW(X)
#define FIELD(T, N) _FIELD(N, T, SCALAR, 1)
#undef _FIELD
#define _FIELD(N, T, CONTAINER, DIM, ...)	\
	IF ## CONTAINER ## IS ## VECTOR (	\
		vector_free_rec(T, this->N))	\
	IF ## CONTAINER ## IS  ## ARRAY (	\
		free_array(T, DIM, this->N))	\
	IF ## CONTAINER ## IS ## SCALAR (	\
		free_ ## T(&this->N));
#else
#define PTR(T)
#define VEC(T)
#define ARR(T, X)
#define FIELD_RAW(X)
#define FIELD(T, N)
#endif
CLASS TOGGLE_H_IMPL(})

#undef NAME
#undef PTR
#undef VEC
#undef ARR
#undef FIELD_RAW
#undef FIELD
#undef _FIELD
