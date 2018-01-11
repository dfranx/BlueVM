#ifndef __BLUEVM_OPCODE_H__
#define __BLUEVM_OPCODE_H__

#include <BlueVM/types.h>

typedef u8 bv_opcode;

// {} represents top of the stack
// [] represents parameters

typedef enum __bv_opcode {
	bv_opcode_unknown,
	bv_opcode_func_start,	// func_start
	bv_opcode_return,		// return {value}
	bv_opcode_push_stack,	// push_stack [type] [value] -> {value}
	bv_opcode_add,			// add {value1, value2} -> {value1+value2}
	bv_opcode_const_get,	// push_const_stack [type] [index] -> {const_value[index]}
	bv_opcode_COUNT			// this is not an actual opcode
} __bv_opcode;

bv_opcode bv_opcode_read(byte** mem);

#endif