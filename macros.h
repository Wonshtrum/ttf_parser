#ifndef __MACROS_H__
#define __MACROS_H__

#define IF0(THEN, ELSE) ELSE
#define IF1(THEN, ELSE) THEN

#define IGNORE_UNUSED(N) void __ ## N() { (void) N;  }

#define DERIVE_DEFAULT_FMT(T) void debug_ ## T (T* this) { debug_fmt_ ## T(this, FMT); }
#define DERIVE_FMT(T) void debug_fmt_ ## T (T* this, int fmt) { (void)fmt; debug_ ## T(this); }
#define SET_FMT_INLINE(T, V) static bool INLINE_ ## T = V; IGNORE_UNUSED(INLINE_ ## T);

#ifdef H_IMPL
#define TOGGLE_H_IMPL(H, I) H I
#else
#define TOGGLE_H_IMPL(H, I) H;
#endif

#endif
