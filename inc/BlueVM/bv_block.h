#ifndef __BLUEVM_BLOCK_H__
#define __BLUEVM_BLOCK_H__

#include <BlueVM/bv_function_pool.h>
#include <BlueVM/bv_constant_pool.h>
#include <BlueVM/bv_object_pool.h>

typedef struct bv_block {
	bv_function_pool* functions;
	bv_constant_pool* constants;
	bv_object_pool* objects;
} bv_block;

bv_block* bv_block_create(byte** mem, byte* orig_mem);
void bv_block_delete(bv_block* block);

#endif