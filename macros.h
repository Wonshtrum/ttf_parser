#include "types.h"

#define IFSCALARISSCALAR(THEN, ...) THEN
#define IFSCALARISARRAY(THEN, ...) __VA_ARGS__
#define IFSCALARISVECTOR(THEN, ...) __VA_ARGS__

#define IFARRAYISSCALAR(THEN, ...) __VA_ARGS__
#define IFARRAYISARRAY(THEN, ...) THEN
#define IFARRAYISVECTOR(THEN, ...) __VA_ARGS__

#define IFVECTORISSCALAR(THEN, ...) __VA_ARGS__
#define IFVECTORISARRAY(THEN, ...) __VA_ARGS__
#define IFVECTORISVECTOR(THEN, ...) THEN

#define IGNORE_UNUSED(N) void __ ## N() { (void) N;  }

#ifdef TOGGLE_H_IMPL
#undef TOGGLE_H_IMPL
#endif

#ifdef H_IMPL
#define TOGGLE_H_IMPL(...) __VA_ARGS__
#else
#define TOGGLE_H_IMPL(...) ;
#endif

#define DERIVE_DEFAULT_FMT(T) void debug_ ## T (T* this) TOGGLE_H_IMPL({ debug_fmt_ ## T(this, FMT); })
#define DERIVE_FMT(T) void debug_fmt_ ## T (T* this, int fmt) TOGGLE_H_IMPL({ (void)fmt; debug_ ## T(this); })
#define SET_FMT_INLINE(T, V) TOGGLE_H_IMPL(static bool INLINE_ ## T = V; IGNORE_UNUSED(INLINE_ ## T);)
