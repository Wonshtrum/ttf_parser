#define IF0(THEN, ELSE) ELSE
#define IF1(THEN, ELSE) THEN

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
