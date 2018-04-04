#include <BlueVM/bv_function_pool.h>
#include <string.h>
#include <stdlib.h>

bv_function_pool* bv_function_pool_create(byte** mem)
{
	bv_function_pool* pool = (bv_function_pool*)malloc(sizeof(bv_function_pool));

	pool->count = u16_read(mem);

	pool->names = (string*)malloc(sizeof(string)*pool->count);
	pool->address = (u32*)malloc(sizeof(u32)*pool->count);

	for (u16 i = 0; i < pool->count; i++) {
		pool->names[i] = string_read(mem);
		pool->address[i] = u32_read(mem);
	}

	return pool;
}

u32 bv_function_pool_get_address(bv_function_pool* pool, string str)
{
	for (u16 i = 0; i < pool->count; i++) {
		string cur = pool->names[i];
		if (strcmp(str, cur) == 0)
			return pool->address[i];
	}

	return 0;
}

void bv_function_pool_delete(bv_function_pool* pool)
{
	for (u16 i = 0; i < pool->count; i++)
		free(pool->names[i]);

	pool->count = 0;
	free(pool->names);
	free(pool->address);

	free(pool);
}
