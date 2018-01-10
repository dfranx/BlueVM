#include <BlueVM/bv_function_pool.h>
#include <string.h>

bv_function_pool* bv_function_pool_create(byte* mem)
{
	bv_function_pool* pool = malloc(sizeof(bv_function_pool));

	pool->count = u16_read(mem);
	mem += sizeof(u16);

	pool->names = malloc(sizeof(bv_string)*pool->count);
	pool->address = malloc(sizeof(u32)*pool->count);

	for (u16 i = 0; i < pool->count; i++) {
		pool->names[i] = bv_string_create(mem);
		mem += pool->names[i].length+1;

		pool->address[i] = u32_read(mem);
		mem += sizeof(u32);
	}

	return pool;
}

u32 bv_function_pool_get_address(bv_function_pool* pool, bv_string str)
{
	for (u16 i = 0; i < pool->count; i++) {
		bv_string* cur = &pool->names[i];
		if (strcmp(str.data, cur->data) == 0)
			return pool->address[i];
	}

	return 0;
}

u32 bv_function_pool_length(bv_function_pool * pool)
{
	u32 ret = sizeof(u16) + sizeof(u32)*pool->count;
	for (u16 i = 0; i < pool->count; i++)
		ret += pool->names[i].length + 1;
	return ret;
}

void bv_function_pool_delete(bv_function_pool* pool)
{
	for (u16 i = 0; i < pool->count; i++) {
		bv_string_delete(pool->names[i]);
	}

	pool->count = 0;
	free(pool->names);
	free(pool->address);

	free(pool);
}
