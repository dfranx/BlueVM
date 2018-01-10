#include <BlueVM/bv_type.h>

bv_type bv_type_create_array(__bv_type type)
{
	return 0x80 | type;
}

u8 bv_type_is_array(bv_type var)
{
	return (var & 0x80) == 0x80; // get highest bit
}

bv_type bv_type_get(bv_type var)
{
	return var & 0x7F; // get the part of bv_type that tells us the type of variable
}

bv_type bv_type_read(byte* mem)
{
	u8 ret = u8_read(mem);
	return ret;
}
