#include <BlueVM/bv_constant_pool.h>
#include <stdlib.h>

bv_constant_pool * bv_constant_pool_create(byte** mem)
{
	bv_constant_pool* pool = (bv_constant_pool*)malloc(sizeof(bv_constant_pool));

	pool->type_count = u8_read(mem);

	pool->val_type = (bv_type*)malloc(sizeof(bv_type)*pool->type_count);
	pool->val_count = (u16*)malloc(sizeof(u16)*pool->type_count);
	pool->val = (bv_variable**)malloc(sizeof(bv_variable)*pool->type_count);

	for (u8 i = 0; i < pool->type_count; i++) {
		pool->val_type[i] = bv_type_read(mem);

		pool->val_count[i] = u16_read(mem);

		pool->val[i] = (bv_variable*)malloc(sizeof(bv_variable)*pool->val_count[i]);

		for (u16 j = 0; j < pool->val_count[i]; j++)
			pool->val[i][j] = bv_variable_read(mem, pool->val_type[i]);
	}

	return pool;
}

void bv_constant_pool_delete(bv_constant_pool * pool)
{
	for (u8 i = 0; i < pool->type_count; i++) {
		for (u16 j = 0; j < pool->val_count[i]; j++)
			 bv_variable_deinitialize(&pool->val[i][j]);
		free(pool->val[i]);
	}

	free(pool->val_type);
	free(pool->val_count);
	free(pool->val);

	free(pool);
}