#ifndef __BLUEVM_STATE_H__
#define __BLUEVM_STATE_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_stack.h>
#include <BlueVM/bv_object.h>
#include <BlueVM/bv_scope_type.h>

typedef struct bv_program_s bv_program;

typedef struct bv_state_s
{
	bv_program* prog;
	bv_function* func;
	byte* code;
	bv_object* obj;
} bv_state;

#endif // __BLUEVM_STATE_H__