#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>

#define FOR_ALL_UINTS		\
	X(uint8_t, u8, 1)	\
	X(uint16_t, u16, 2)	\
	X(uint32_t, u32, 4)	\
	X(uint64_t, u64, 8)
#define FOR_ALL_INTS	\
	X(int8_t, i8, 1)	\
	X(int16_t, i16, 2)	\
	X(int32_t, i32, 4)	\
	X(int64_t, i64, 8)

#define X(T, N, _) typedef T N;
FOR_ALL_UINTS
FOR_ALL_INTS
#undef X

typedef u8 bool;
#define true 1
#define false 0

#endif
