#include <BlueVM/types.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
	u16 ret = *(*mem + 0) | (*(*mem + 1) << 8);
	*mem += 2;
	return ret;
}

s16 s16_read(byte** mem)
{
	return u16_read(mem);
}

u32 u32_read(byte** mem)
{
	u32 ret = *(*mem + 0) | (*(*mem + 1) << 8) | (*(*mem + 2) << 16) | (*(*mem + 3) << 24);
	*mem += 4;
	return ret;
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

bv_string string_read(byte** mem)
{
	u16 size = 0;
	while (size != 0xFFFF) {  // limitation = u16 length
		if ((*mem)[size] == 0)
			break;
		size++;
	}

	size++;

	bv_string ret = (bv_string)malloc(size * sizeof(char));
	memcpy(ret, *mem, size);

	*mem += size;

	return ret;
}

u8 get_digit_count(s32 val) {
	u8 ret = 0;

	while (val != 0) {
		ret++;
		val = val / 10;
	}

	return ret;
}
bv_string bv_itoa(s32 val, s32 base) {
	static char buf[32] = { 0 };
	buf[0] = '-';

	byte isNegative = 1; // 1 = not negative, 0 = negative

	// numbers can only be negative with base 10, for every other base they are considered unsigned
	if (val < 0 && base == 10)
	{
		isNegative = 0;
		val = -val;
	}

	// get all digits
	u8 dig_count = get_digit_count(val);
	u32 i = 0;
	while (val != 0)
	{
		s32 r = val % base;
		buf[dig_count - i] = (r > 9) ? ((r - 10) + 'a') : (r + '0');
		
		val = val / base;
		i++;
	}

	buf[dig_count + 1] = '\0';

	return &buf[isNegative];
}

bv_string bv_ftoa(float val, u8 dec)
{
	static char buf[64] = { 0 };

	// split value in two parts
	s32 ip = (s32)val;
	s32 fp = round((val - ip) * pow(10, dec));

	// convert integer part to string
	bv_string ip_str = bv_itoa(ip, 10);
	u32 decPlace = strlen(ip_str);
	memcpy(&buf[0], ip_str, decPlace);

	// check for display option after point
	if (dec != 0) {
		buf[decPlace] = '.';
		
		bv_string fp_str = bv_itoa(fp, 10);
		memcpy(&buf[decPlace + 1], fp_str, strlen(fp_str));
	}

	return &buf[0];
}