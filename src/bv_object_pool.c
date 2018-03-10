#include <BlueVM/bv_object_pool.h>

bv_object_pool* bv_object_pool_create(byte** mem)
{
	bv_object_pool* ret = malloc(sizeof(bv_object_pool));
	
	ret->count = u16_read(mem);
	ret->info = malloc(sizeof(bv_object_info) * ret->count);

	for (u16 i = 0; i < ret->count; i++)
		ret->info[i] = bv_object_info_read(mem);

	return ret;
}

bv_object_info* bv_object_pool_get(bv_object_pool* pool, string name)
{
	for (u16 i = 0; i < pool->count; i++)
		if (strcmp(name, pool->info[i]->name) == 0)
			return pool->info[i];
	return 0;
}

void bv_object_pool_delete(bv_object_pool* pool)
{
	// TODO!!!!
}