#include <BlueVM/bv_header.h>

bv_header bv_header_create(byte* mem)
{
	bv_header ret;
	memcpy(&ret, mem, sizeof(bv_header));
	return ret;
}