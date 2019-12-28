#ifndef __BLUEVM_CONSTANT_POOL_H__
#define __BLUEVM_CONSTANT_POOL_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_type.h>
#include <BlueVM/bv_header.h>
#include <BlueVM/bv_variable.h>

typedef struct bv_constant_pool_s {
	u8 type_count;
	bv_type* val_type;
	u16* val_count;
	bv_variable** val;
} bv_constant_pool;

bv_constant_pool* bv_constant_pool_create(bv_header header, byte** mem);
void bv_constant_pool_delete(bv_constant_pool* pool);

#endif