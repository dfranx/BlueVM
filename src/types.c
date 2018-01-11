#include <BlueVM/types.h>
#include <string.h>
#include <stdlib.h>

u8 u8_read(byte** mem)
{
	return (*(*mem)++);
}

s8 s8_read(byte** mem)
{
	return u8_read(mem);
}

u16 u16_read(byte** mem)
{
	return u8_read(mem) | (u8_read(mem) << 8);
}

s16 s16_read(byte** mem)
{
	return u16_read(mem);
}

u32 u32_read(byte** mem)
{
	return u8_read(mem) | u8_read(mem) << 8 | u8_read(mem) << 16 | u8_read(mem) << 24;
}

s32 s32_read(byte** mem)
{
	return u32_read(mem);
}

float float_read(byte** mem)
{
	float output;
	for (u8 i = 0; i < 4; i++)
		*((byte*)(&output) + i) = u8_read(mem);
	return output;
}

string string_read(byte** mem)
{
	u16 size = 0;
	while (1) {  // [TODO] add some limitation here
		if ((*mem)[size] == 0)
			break;
		size++;
	}

	size++;

	string ret = malloc(size * sizeof(char));
	memcpy(ret, *mem, size);

	*mem += size;

	return ret;
}