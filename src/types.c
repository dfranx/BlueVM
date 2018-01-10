#include <BlueVM/types.h>

u8 u8_read(byte* mem)
{
	return mem[0];
}

s8 s8_read(byte* mem)
{
	return u8_read(mem);
}

u16 u16_read(byte* mem)
{
	return mem[0] | mem[1] << 8;
}

s16 s16_read(byte* mem)
{
	return u16_read(mem);
}

u32 u32_read(byte* mem)
{
	return mem[0] | mem[1] << 8 | mem[2] << 16 | mem[3] << 24;
}

s32 s32_read(byte* mem)
{
	return u32_read(mem);
}

float float_read(byte * mem)
{
	float output;
	for (u8 i = 0; i < 4; i++)
		*((byte*)(&output) + i) = mem[i];
	return output;
}
