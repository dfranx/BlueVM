#include <BlueVM/bv_program.h>
#include <BlueVM/bv_object.h>
#include <BlueVM/bv_array.h>
#include <BlueVM/bv_state.h>
#include <BlueVM/bv_execute.h>
#include <string.h>
#include <stdlib.h>

bv_program* bv_program_create(byte* mem)
{
	bv_program* ret = malloc(sizeof(bv_program));

	byte* original_mem = mem;

	ret->header = bv_header_create(&mem);
	ret->global_names = bv_name_list_create(&mem);
	ret->globals = bv_stack_create();
	ret->block = bv_block_create(&mem, original_mem);
	ret->functions = bv_function_create_array(ret->block->functions, original_mem);

	bv_program_build_opcode_table(ret);

	ret->external_functions = NULL;
	ret->external_function_names = NULL;
	ret->external_function_count = 0;

	ret->globals.length = ret->global_names.name_count;
	ret->globals.data = malloc(sizeof(bv_variable) * ret->globals.length);
	for (u16 i = 0; i < ret->globals.length; i++)
		ret->globals.data[i] = bv_variable_create_void();

	return ret;
}
void bv_program_build_opcode_table(bv_program* prog)
{
	prog->opcodes = malloc(sizeof(bv_execute) * bv_opcode_COUNT);

	prog->opcodes[bv_opcode_unknown] = bv_execute_unknown;
	prog->opcodes[bv_opcode_func_start] = bv_execute_func_start;
	prog->opcodes[bv_opcode_return] = bv_execute_return;
	prog->opcodes[bv_opcode_push_stack] = bv_execute_push_stack;
	prog->opcodes[bv_opcode_pop_stack] = bv_execute_pop_stack;
	prog->opcodes[bv_opcode_const_get] = bv_execute_const_get;
	prog->opcodes[bv_opcode_add] = bv_execute_add;
	prog->opcodes[bv_opcode_subtract] = bv_execute_subtract;
	prog->opcodes[bv_opcode_multiply] = bv_execute_multiply;
	prog->opcodes[bv_opcode_divide] = bv_execute_divide;
	prog->opcodes[bv_opcode_increment] = bv_execute_increment;
	prog->opcodes[bv_opcode_decrement] = bv_execute_decrement;
	prog->opcodes[bv_opcode_negate] = bv_execute_negate;
	prog->opcodes[bv_opcode_modulo] = bv_execute_modulo;
	prog->opcodes[bv_opcode_bit_or] = bv_execute_bit_or;
	prog->opcodes[bv_opcode_bit_and] = bv_execute_bit_and;
	prog->opcodes[bv_opcode_bit_not] = bv_execute_bit_not;
	prog->opcodes[bv_opcode_bit_xor] = bv_execute_bit_xor;
	prog->opcodes[bv_opcode_bit_lshift] = bv_execute_bit_lshift;
	prog->opcodes[bv_opcode_bit_rshift] = bv_execute_bit_rshift;
	prog->opcodes[bv_opcode_equal] = bv_execute_equal;
	prog->opcodes[bv_opcode_not_equal] = bv_execute_not_equal;
	prog->opcodes[bv_opcode_not] = bv_execute_not;
	prog->opcodes[bv_opcode_greater] = bv_execute_greater;
	prog->opcodes[bv_opcode_greater_equal] = bv_execute_greater_equal;
	prog->opcodes[bv_opcode_less_equal] = bv_execute_less_equal;
	prog->opcodes[bv_opcode_nop] = bv_execute_nop;
	prog->opcodes[bv_opcode_convert] = bv_execute_convert;
	prog->opcodes[bv_opcode_duplicate] = bv_execute_duplicate;
	prog->opcodes[bv_opcode_swap] = bv_execute_swap;
	prog->opcodes[bv_opcode_get_local] = bv_execute_get_local;
	prog->opcodes[bv_opcode_set_local] = bv_execute_set_local;
	prog->opcodes[bv_opcode_get_global] = bv_execute_get_global;
	prog->opcodes[bv_opcode_set_global] = bv_execute_set_global;
	prog->opcodes[bv_opcode_new_array] = bv_execute_new_array;
	prog->opcodes[bv_opcode_call] = bv_execute_call;
	prog->opcodes[bv_opcode_call_return] = bv_execute_call_return;
	prog->opcodes[bv_opcode_is_type_of] = bv_execute_is_type_of;
	prog->opcodes[bv_opcode_if] = bv_execute_if;
	prog->opcodes[bv_opcode_goto] = bv_execute_goto;
	prog->opcodes[bv_opcode_new_object] = bv_execute_new_object;
	prog->opcodes[bv_opcode_set_prop] = bv_execute_set_prop;
	prog->opcodes[bv_opcode_set_my_prop] = bv_execute_set_my_prop;
	prog->opcodes[bv_opcode_get_prop] = bv_execute_get_prop;
	prog->opcodes[bv_opcode_get_my_prop] = bv_execute_get_my_prop;
	prog->opcodes[bv_opcode_call_method] = bv_execute_call_method;
	prog->opcodes[bv_opcode_call_my_method] = bv_execute_call_my_method;
	prog->opcodes[bv_opcode_call_ret_method] = bv_execute_call_ret_method;
	prog->opcodes[bv_opcode_call_ret_my_method] = bv_execute_call_ret_my_method;
}
void bv_program_delete(bv_program * prog)
{
	bv_function_delete_array(prog->functions, bv_program_get_function_count(prog));
	bv_block_delete(prog->block);
	bv_stack_delete(&prog->globals);
	bv_name_list_delete(&prog->global_names);

	free(prog);
}

u16 bv_program_get_function_count(bv_program * prog)
{
	return prog->block->functions->count;
}
bv_function* bv_program_get_function(bv_program* prog, const char* str)
{
	u16 func_count = bv_program_get_function_count(prog);
	u16 ext_func_count = prog->external_function_count;

	for (u16 i = 0; i < func_count; i++)
		if (strcmp(prog->block->functions->names[i], str) == 0)
			return prog->functions[i];
	
	return 0;
}
bv_external_function bv_program_get_ext_function(bv_program * prog, const char * str)
{
	u16 ext_func_count = prog->external_function_count;
	for (u16 i = 0; i < ext_func_count; i++)
		if (strcmp(prog->external_function_names[i], str) == 0)
			return prog->external_functions[i];
	
	return NULL;
}
void bv_program_add_function(bv_program * prog, const char * name, bv_external_function* ext_func)
{
	prog->external_functions = realloc(prog->external_functions, sizeof(bv_external_function) * (prog->external_function_count + 1));
	prog->external_function_names = realloc(prog->external_function_names, sizeof(char*) * (prog->external_function_count + 1));
	
	prog->external_functions[prog->external_function_count] = ext_func;
	prog->external_function_names[prog->external_function_count] = name;

	prog->external_function_count++;
}

u16 bv_program_get_global_count(bv_program * prog)
{
	return prog->globals.length;
}
bv_variable bv_program_get_global(bv_program* prog, string name)
{
	return prog->globals.data[bv_name_list_get_id(prog->global_names, name)];
}
void bv_program_set_global(bv_program * prog, string name, bv_variable var)
{
	u16 ind = bv_name_list_get_id(prog->global_names, name);
	prog->globals.data[ind] = var;
}

bv_variable bv_program_call(bv_program* prog, bv_function* func, bv_stack* args, bv_object* parent)
{
	bv_stack stack = bv_stack_create();
	bv_stack locals = bv_stack_create(); // local variable container
	bv_variable rtrn;

	if (args != NULL && args->length == func->args) {
		// push arguments to local variables
		for (int i = 0; i < args->length; i++)
			bv_stack_push(&locals, args->data[i]);
	}

	byte* func_code = func->code;

	bv_state state;
	state.code = &func_code;
	state.locals = &locals;
	state.obj = parent;
	state.prog = prog;
	state.should_exit = 0;
	state.stack = &stack;
	state.this_func = func;

	while (((*state.code) - func->code) < func->code_length && !state.should_exit) {
		bv_opcode op = bv_opcode_read(state.code);
		(*prog->opcodes[op])(&state);
	}

	// get return value
	if (stack.length > 0)
		rtrn = bv_variable_copy(bv_stack_top(&stack)); // on return, make a copy of the return value
	else
		rtrn = bv_variable_create_void();

	bv_stack_delete(&stack);
	bv_stack_delete(&locals);

	return rtrn;
}
