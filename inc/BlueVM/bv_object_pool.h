#ifndef __BLUEVM_OBJECT_POOL_H__
#define __BLUEVM_OBJECT_POOL_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_object_info.h>

typedef struct bv_variable bv_variable;

typedef struct bv_object_pool {
	u16 count;
	bv_object_info** info;
} bv_object_pool;

bv_object_pool* bv_object_pool_create(byte** mem, byte* orig_mem);
bv_object_info* bv_object_pool_get(bv_object_pool* pool, bv_string name);
void bv_object_pool_delete(bv_object_pool* pool);

#endif