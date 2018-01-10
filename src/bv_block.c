#include <BlueVM/bv_block.h>

bv_block* bv_block_create(byte* mem)
{
	bv_block* block = malloc(sizeof(bv_block));

	block->functions = bv_function_pool_create(mem);
	block->constants = bv_constant_pool_create(mem + bv_function_pool_length(block->functions));

	return block;
}

void bv_block_delete(bv_block * block)
{
	bv_constant_pool_delete(block->constants);
	bv_function_pool_delete(block->functions);
	free(block);
}
