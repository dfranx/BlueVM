#include <BlueVM/bv_type.h>

bv_type bv_type_read(byte** mem)
{
	return u8_read(mem);
}
