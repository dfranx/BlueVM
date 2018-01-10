#ifndef __BLUEVM_FUNCTION_H__
#define __BLUEVM_FUNCTION_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_type.h>
#include <BlueVM/bv_function_pool.h>

typedef struct bv_function
{
	bv_type return_type;
	u8 args;
	bv_type* arg_type;
	u32 op_length;
	u8* op;
} bv_function;

bv_function* bv_function_create(byte* mem);
void bv_function_delete(bv_function* func);
bv_function** bv_function_create_array(bv_function_pool* funcs, byte* mem);
void bv_function_delete_array(bv_function** data, u16 len);

#endif