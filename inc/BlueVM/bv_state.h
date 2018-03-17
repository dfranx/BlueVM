#ifndef __BLUEVM_STATE_H__
#define __BLUEVM_STATE_H__
#include <BlueVM/types.h>
#include <BlueVM/bv_stack.h>
#include <BlueVM/bv_object.h>

typedef struct bv_program bv_program;

typedef struct bv_state
{
	bv_program* prog;
	bv_function* this_func;
	byte** code;
	bv_stack* stack;
	bv_stack* locals;
	bv_object* obj;
	u8 should_exit;
} bv_state;

#endif // __BLUEVM_STATE_H__