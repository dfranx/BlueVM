#include <BlueVM/bv_type.h>

char bv_type_is_integer(bv_type type)
{
	if (type >= bv_type_int && type <= bv_type_uchar)
		return 1;
	return 0;
}

bv_type bv_type_get(bv_type t1, bv_type t2)
{
	if (!bv_type_is_integer(t1) || !bv_type_is_integer(t2))
		return t1;

	if (t1 == bv_type_int || t2 == bv_type_int)
		return bv_type_int;
	if (t1 == bv_type_short || t2 == bv_type_short)
		return bv_type_short;
	if (t1 == bv_type_char || t2 == bv_type_char)
		return bv_type_char;
	if (t1 == bv_type_uint || t2 == bv_type_uint)
		return bv_type_uint;
	if (t1 == bv_type_ushort || t2 == bv_type_ushort)
		return bv_type_ushort;

	return bv_type_uchar;
}
