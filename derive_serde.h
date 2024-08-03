#include "macros.h"

#define read_array(T, DIM, rd, x) for (int i=0; i<DIM; i++) x[i] = read_ ## T(rd);

#define NAME(N)				\
	N read_ ## N (Reader* rd) {	\
		N this;

#define PTR(T) ptr
#define ARR(T, X) T, 1, X
#define FIELD_RAW(X)
#define FIELD(T, N) _FIELD(N, T, 0, 1);
#define _FIELD(N, T, IS_ARR, DIM, ...)			\
	IF ## IS_ARR(					\
			read_array(T, DIM, rd, this.N),	\
			this.N = read_ ## T(rd));

CLASS return this; }

#undef NAME
#undef PTR
#undef ARR
#undef FIELD_RAW
#undef FIELD
#undef _FIELD
