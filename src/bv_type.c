#include <BlueVM/bv_type.h>

bv_type bv_type_read(byte** mem)
{
	return u8_read(mem);
}

char bv_type_is_integer(bv_type type)
{
	if (type >= bv_type_int && type <= bv_type_uchar)
		return 1;
	return 0;
}