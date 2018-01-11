#include <BlueVM/bv_program.h>
#include <BlueVM/bv_stack.h>
#include <string.h>
#include <stdlib.h>

bv_program* bv_program_create(byte * mem)
{
	bv_program* ret = malloc(sizeof(bv_program));

	ret->header = bv_header_create(mem);
	ret->block = bv_block_create(mem + sizeof(ret->header));
	ret->functions = bv_function_create_array(ret->block->functions, mem);

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
		if (strcmp(prog->block->functions->names[i], str) == 0)
			return prog->functions[i];

	return 0;
}

bv_variable bv_program_call(bv_program * prog, bv_function * func)
{
	bv_stack stack = bv_stack_create();
	bv_variable rtrn;
	byte* code = func->code;

	while ((code - func->code) < func->code_length) {
		bv_opcode op = bv_opcode_read(&code);

		if (op == bv_opcode_return)
			break;
		else if (op == bv_opcode_push_stack) {
			bv_type type = bv_type_read(&code);
			bv_stack_push(&stack, bv_variable_read(&code, type));
		} else if (op == bv_opcode_add) { // [TODO] add_i, add_f, etc...
			if (stack.length >= 2) { // dont do anything if there is not enough arguments in stack
				int sum = bv_variable_get_int(bv_stack_top(&stack));
				bv_stack_pop(&stack);

				sum += bv_variable_get_int(bv_stack_top(&stack));
				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create_int(sum));
			}
		}
	}

	// get return value
	if (stack.length > 0)
		rtrn = bv_variable_copy(bv_stack_top(&stack)); // on return, make a copy of the return value
	else
		rtrn = bv_variable_create_int(0);

	bv_stack_delete(&stack);

	return rtrn;
}
