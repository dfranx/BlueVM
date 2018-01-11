#include <BlueVM/bv_constant_pool.h>

bv_constant_pool * bv_constant_pool_create(byte * mem)
{
	bv_constant_pool* pool = malloc(sizeof(bv_constant_pool));

	// UInt32
	pool->u_count = u16_read(mem);
	mem += sizeof(u16);
	pool->u = malloc(sizeof(u32)*pool->u_count);
	for (u16 i = 0; i < pool->u_count; i++) {
		pool->u[i] = u32_read(mem);
		mem += sizeof(u32);
	}

	// SInt32
	pool->s_count = u16_read(mem);
	mem += sizeof(u16);
	pool->s = malloc(sizeof(s32)*pool->s_count);
	for (u16 i = 0; i < pool->s_count; i++) {
		pool->s[i] = s32_read(mem);
		mem += sizeof(s32);
	}

	// Float
	pool->f_count = u16_read(mem);
	mem += sizeof(u16);
	pool->f = malloc(sizeof(float)*pool->f_count);
	for (u16 i = 0; i < pool->f_count; i++) {
		pool->f[i] = float_read(mem);
		mem += sizeof(float);
	}

	// String
	pool->str_count = u16_read(mem);
	mem += sizeof(u16);
	pool->str = malloc(sizeof(string)*pool->str_count);
	for (u16 i = 0; i < pool->str_count; i++) {
		pool->str[i] = string_read(mem);
		mem += strlen(pool->str[i])+1;
	}

	return pool;
}

u32 bv_constant_pool_length(bv_constant_pool * pool)
{
	u32 strLen = 0;
	for (u16 i = 0; i < pool->str_count; i++)
		strLen += strlen(pool->str[i]) + 1;

	return sizeof(u16) * 4 + pool->u_count * sizeof(u32) + pool->s_count * sizeof(s32) + pool->f_count * sizeof(float) + strLen;
}

void bv_constant_pool_delete(bv_constant_pool * pool)
{
	// UInt32
	pool->u_count = 0;
	free(pool->u);

	// SInt32
	pool->s_count = 0;
	free(pool->s);

	// Float
	pool->f_count = 0;
	free(pool->f);

	// String
	for (u16 i = 0; i < pool->str_count; i++)
		free(pool->str[i]);
	free(pool->str);
	pool->str_count = 0;

	free(pool);
}
