#ifndef __BLUEVM_OPCODE_H__
#define __BLUEVM_OPCODE_H__

#include <BlueVM/types.h>

typedef u8 bv_opcode;

typedef enum __bv_opcode {
	bv_opcode_unknown,
	bv_opcode_func_start,
	bv_opcode_return,
	bv_opcode_COUNT, // this is not an actual opcode
} __bv_opcode;

bv_opcode bv_opcode_get(byte* mem);

#endif