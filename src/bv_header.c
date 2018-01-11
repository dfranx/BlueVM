#include <BlueVM/bv_header.h>
#include <memory.h>

bv_header bv_header_create(byte* mem)
{
	bv_header ret;
	memcpy(&ret, mem, sizeof(bv_header));
	return ret;
}