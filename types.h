#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
void* realloc(void*, size_t);

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

#define free_array(T, DIM, x) for (typeof(DIM) i=0; i<DIM; i++) free_ ## T (x+i);

typedef struct VecHeader {
	u32 len;
	u32 cap;
	u8 data[];
} VecHeader;

#define vector_ptr(V) (((VecHeader*)V)-1)
#define vector_len(V) ((VecHeader*)V)[-1].len
#define vector_cap(V) ((VecHeader*)V)[-1].cap
#define vector_size(V) (vector_len(V)*sizeof(*V))
static inline void* __vector_alloc(void* _vector, u32 new_capacity, u32 size_of_item) {
	VecHeader* vector = (VecHeader*)_vector;
	u32 len = ((vector) ? vector_len(vector) : 0);
	vector = realloc(vector ? vector-1 : NULL, size_of_item * new_capacity + sizeof(VecHeader));
	vector[0].len = len;
	vector[0].cap = new_capacity;
	return vector+1;
}
#define vector_new(T, N) __vector_alloc(NULL, N, sizeof(T))
#define vector_free(V) if (V) free(vector_ptr(V));
#define vector_free_rec(T, V) { if (V) free_array(T, vector_len(V), V); vector_free(V); }

#endif
