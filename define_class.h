#include "macros.h"

#define NAME(N) typedef struct N N; struct N {
#define PTR(T) T*
#define ARR(T, X) T, 1, X
#define FIELD_RAW(X) X;
#define FIELD(T, N) _FIELD(N, T, 0, 1);
#define _FIELD(N, T, IS_ARR, ARR_X, ...) T N IF ## IS_ARR([ARR_X],)
CLASS };

#undef NAME
#undef PTR
#undef ARR
#undef FIELD_RAW
#undef FIELD
#undef _FIELD
