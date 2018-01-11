#ifndef __BLUEVM_CONSTANT_POOL_H__
#define __BLUEVM_CONSTANT_POOL_H__

#include <BlueVM/types.h>

typedef struct bv_constant_pool {
	u16 u_count;
	u32* u;
	u16 s_count;
	s32* s;
	u16 f_count;
	float* f;
	u16 str_count;
	string* str;
} bv_constant_pool;

bv_constant_pool* bv_constant_pool_create(byte* mem);
u32 bv_constant_pool_length(bv_constant_pool* pool);
void bv_constant_pool_delete(bv_constant_pool* pool);

#endif