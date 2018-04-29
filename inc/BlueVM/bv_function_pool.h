#ifndef __BLUEVM_FUNCTION_POOL_H__
#define __BLUEVM_FUNCTION_POOL_H__

#include <BlueVM/types.h>

typedef struct bv_function_pool {
	u16 count;
	bv_string* names;
	u32* address;
} bv_function_pool;

bv_function_pool* bv_function_pool_create(byte** mem);
u32 bv_function_pool_get_address(bv_function_pool* pool, bv_string str);
void bv_function_pool_delete(bv_function_pool* pool);

#endif