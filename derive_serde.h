#include "macros.h"

#define read_array(T, DIM, rd, x) for (int i=0; i<DIM; i++) x[i] = read_ ## T(rd);

#define NAME(N)	N read_ ## N (Reader* rd) TOGGLE_H_IMPL({ N this;)

#define PTR(T) ptr
#define VEC(T) vec
#define ARR(T, X) T, ARRAY, X
#define FIELD_RAW(X)
#define FIELD(T, N) TOGGLE_H_IMPL(_FIELD(N, T, SCALAR, 1);)
#define _FIELD(N, T, CONTAINER, DIM, ...)		\
	IF ## CONTAINER ## IS ## ARRAY (		\
			read_array(T, DIM, rd, this.N),	\
			this.N = read_ ## T(rd));

CLASS TOGGLE_H_IMPL(return this; })

#undef NAME
#undef PTR
#undef VEC
#undef ARR
#undef FIELD_RAW
#undef FIELD
#undef _FIELD
