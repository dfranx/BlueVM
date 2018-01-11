#include <BlueVM/bv_string.h>
#include <memory.h>

bv_string bv_string_create(const char * text)
{
	bv_string ret;
	ret.length = strlen(text);
	ret.data = malloc(strlen(text) + 1);
	memcpy(ret.data, text, ret.length);
	ret.data[ret.length] = '\0';

	return ret;
}

bv_string bv_string_copy(char * text, u32 len)
{
	bv_string ret;
	ret.length = len;
	ret.data = malloc(len + 1);
	memcpy(ret.data, text, len);
	ret.data[len] = '\0';

	return ret;
}

bv_string bv_string_read(byte* mem)
{
	bv_string ret;

	u32 len = u32_read(mem);
	mem += sizeof(u32);
	ret.length = len;

	ret.data = malloc(len + 1);
	for (u32 i = 0; i < len; i++)
		ret.data[i] = mem[i];
	ret.data[len] = '\0';

	return ret;
}

void bv_string_delete(bv_string str)
{
	free(str.data);
}

u32 bv_string_length(bv_string * str)
{
	return str->length;
}
