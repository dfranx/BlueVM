#include <BlueVM/bv_object_pool.h>
#include <stdlib.h>
#include <string.h>

bv_object_pool* bv_object_pool_create(byte** mem, byte* orig_mem)
{
	bv_object_pool* ret = (bv_object_pool*)malloc(sizeof(bv_object_pool));
	
	ret->count = u16_read(mem);
	ret->info = (bv_object_info**)malloc(sizeof(bv_object_info) * ret->count);

	for (u16 i = 0; i < ret->count; i++)
		ret->info[i] = bv_object_info_read(mem, orig_mem);

	return ret;
}

bv_object_info* bv_object_pool_get(bv_object_pool* pool, bv_string name)
{
	for (u16 i = 0; i < pool->count; i++)
		if (strcmp(name, pool->info[i]->name) == 0)
			return pool->info[i];
	return 0;
}

void bv_object_pool_delete(bv_object_pool* pool)
{
	for (u16 i = 0; i < pool->count; i++)
		bv_object_info_delete(pool->info[i]);

	free(pool->info);
	free(pool);
}