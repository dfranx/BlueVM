#include <BlueVM/bv_program.h>

bv_program* bv_program_create(byte * mem)
{
	bv_program* ret = malloc(sizeof(bv_program));

	ret->header = bv_header_create(mem);
	ret->block = bv_block_create(mem + sizeof(ret->header));
	ret->functions = bv_function_create_array(ret->block->functions, mem);

	return ret;
}
u32 bv_program_length(bv_program * prog)
{
	// header
	u32 ret = sizeof(prog->header.major) + sizeof(prog->header.minor) + 3;
	
	// block
	ret += bv_function_pool_length(prog->block->functions);
	ret += bv_constant_pool_length(prog->block->constants);

	// functions
	u16 func_count = bv_program_get_function_count(prog);
	ret += (sizeof(bv_type) + sizeof(u8) + sizeof(u32)) * func_count;

	for (u16 i = 0; i < func_count; i++) {
		bv_function* func = prog->functions[i];
		ret += func->op_length*sizeof(bv_opcode) + func->args*sizeof(bv_type);
	}
	
	return ret;
}
void bv_program_delete(bv_program * prog)
{
	bv_function_delete_array(prog->functions, bv_program_get_function_count(prog));
	bv_block_delete(prog->block);

	free(prog);
}

u16 bv_program_get_function_count(bv_program * prog)
{
	return prog->block->functions->count;
}
bv_function* bv_program_get_function(bv_program* prog, const char* str)
{
	u16 func_count = bv_program_get_function_count(prog);

	for (u16 i = 0; i < func_count; i++)
		if (strcmp(prog->block->functions->names[i].data, str) == 0)
			return prog->functions[i];

	return 0;
}

bv_variable bv_program_call(bv_program * prog, bv_function * func)
{
	char* code = func->op;
	while ((code - func->op) < func->op_length) {
		bv_opcode op = bv_opcode_read(code);
		code += sizeof(bv_opcode);

		if (op == bv_opcode_return)
			return bv_variable_read(code, func->return_type);
	}
}
