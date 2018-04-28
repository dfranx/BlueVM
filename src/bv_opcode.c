#include <BlueVM/bv_opcode.h>

bv_opcode bv_opcode_read(byte** mem)
{
	return u8_read(mem);
}
