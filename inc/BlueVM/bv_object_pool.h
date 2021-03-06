#ifndef __BLUEVM_OBJECT_POOL_H__
#define __BLUEVM_OBJECT_POOL_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_header.h>
#include <BlueVM/bv_object_info.h>

typedef struct bv_variable_s bv_variable;

typedef struct bv_object_pool_s {
	u16 count;
	bv_object_info** info;
} bv_object_pool;

bv_object_pool* bv_object_pool_create(bv_header header, byte** mem, byte* orig_mem);
bv_object_info* bv_object_pool_get(bv_object_pool* pool, bv_string name);
void bv_object_pool_add(bv_object_pool* pool, bv_object_info* info);
void bv_object_pool_delete(bv_object_pool* pool);

#endif