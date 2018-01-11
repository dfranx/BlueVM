#include <BlueVM/bv_opcode.h>

bv_opcode bv_opcode_read(byte** mem)
{
	bv_opcode op = u8_read(mem);
	if (op >= bv_opcode_COUNT)
		return bv_opcode_unknown;
	return op;
}
