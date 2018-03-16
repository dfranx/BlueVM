#include <BlueVM/bv_block.h>
#include <stdlib.h>

bv_block* bv_block_create(byte** mem, byte* orig_mem)
{
	bv_block* block = malloc(sizeof(bv_block));

	block->functions = bv_function_pool_create(mem);
	block->constants = bv_constant_pool_create(mem);
	block->objects = bv_object_pool_create(mem, orig_mem);

	return block;
}

void bv_block_delete(bv_block * block)
{
	bv_function_pool_delete(block->functions);
	bv_constant_pool_delete(block->constants);
	bv_object_pool_delete(block->objects);
	free(block);
}
