#include <BlueVM/bv_program.h>
#include <BlueVM/bv_object.h>
#include <BlueVM/bv_array.h>
#include <BlueVM/bv_state.h>
#include <BlueVM/bv_execute.h>
#include <BlueVM/bv_scope.h>
#include <BlueVM/bv_opcode.h>
#include <string.h>
#include <stdlib.h>

void bv_program_build_opcode_table(bv_program* prog)
{
	prog->opcodes = (bv_execute*)malloc(sizeof(bv_execute) * bv_opcode_COUNT);

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
	prog->opcodes[bv_opcode_bool_or] = bv_execute_bool_or;
	prog->opcodes[bv_opcode_bool_and] = bv_execute_bool_and;
	prog->opcodes[bv_opcode_equal] = bv_execute_equal;
	prog->opcodes[bv_opcode_not_equal] = bv_execute_not_equal;
	prog->opcodes[bv_opcode_not] = bv_execute_not;
	prog->opcodes[bv_opcode_greater] = bv_execute_greater;
	prog->opcodes[bv_opcode_less] = bv_execute_less;
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
	prog->opcodes[bv_opcode_push_array] = bv_execute_push_array;
	prog->opcodes[bv_opcode_set_array_el] = bv_execute_set_array_el;
	prog->opcodes[bv_opcode_get_array_el] = bv_execute_get_array_el;
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
	prog->opcodes[bv_opcode_scope_start] = bv_execute_scope_start;
	prog->opcodes[bv_opcode_scope_end] = bv_execute_scope_end;
	prog->opcodes[bv_opcode_assign] = bv_execute_assign;
	prog->opcodes[bv_opcode_get_local_pointer] = bv_execute_get_local_pointer;
	prog->opcodes[bv_opcode_get_global_pointer] = bv_execute_get_global_pointer;
	prog->opcodes[bv_opcode_get_prop_pointer] = bv_execute_get_prop_pointer;
	prog->opcodes[bv_opcode_get_my_prop_pointer] = bv_execute_get_my_prop_pointer;
	prog->opcodes[bv_opcode_get_global_by_name] = bv_execute_get_global_by_name;
	prog->opcodes[bv_opcode_get_global_by_name_ptr] = bv_execute_get_global_by_name_ptr;
	prog->opcodes[bv_opcode_set_global_by_name] = bv_execute_set_global_by_name;
	prog->opcodes[bv_opcode_empty_stack] = bv_execute_empty_stack;
	prog->opcodes[bv_opcode_debug_line_number] = bv_execute_debug_line_number;
	prog->opcodes[bv_opcode_debug_file] = bv_execute_debug_file;
	prog->opcodes[bv_opcode_debug_breakpoint] = bv_execute_debug_breakpoint;
	prog->opcodes[bv_opcode_new_object_by_name] = bv_execute_new_object_by_name;
	prog->opcodes[bv_opcode_push_stack_function] = bv_execute_push_stack_function;
	prog->opcodes[bv_opcode_call_stack] = bv_execute_call_stack;
}
bv_program* bv_program_create(byte* mem)
{
	bv_program* ret = (bv_program*)malloc(sizeof(bv_program));

	byte* original_mem = mem;

	ret->header = bv_header_create(&mem);
	ret->string_table = bv_string_table_create(&mem);
	ret->global_names = bv_name_list_create(&mem);
	ret->globals = bv_stack_create();
	ret->block = bv_block_create(&mem, original_mem);
	ret->functions = bv_function_create_array(ret->block->functions, original_mem);

	bv_program_build_opcode_table(ret);

	ret->lib_count = 0;
	ret->libraries = NULL;
	ret->external_functions = NULL;
	ret->external_function_names = NULL;
	ret->external_function_count = 0;
	ret->current_file = NULL;
	ret->current_line = -1;
	ret->error = NULL;
	ret->debugger = NULL;

	ret->globals.length = ret->global_names.name_count;
	ret->globals.data = (bv_variable*)malloc(sizeof(bv_variable) * ret->globals.length);
	for (u16 i = 0; i < ret->globals.length; i++)
		ret->globals.data[i] = bv_variable_create_void();

	return ret;
}
void bv_program_set_error_handler(bv_program * prog, bv_error_handler errh)
{
	prog->error = errh;
}
void bv_program_set_breakpoint_handler(bv_program * prog, bv_breakpoint_handler dbgh)
{
	prog->debugger = dbgh;
}
void bv_program_error(bv_program * prog, u8 lvl, u16 id, const bv_string msg)
{
	if (prog->error != NULL)
		prog->error(lvl, id, msg, prog->current_line, prog->current_file);
}
void bv_program_delete(bv_program * prog)
{
	bv_function_delete_array(prog->functions, bv_program_get_function_count(prog));
	bv_stack_delete(&prog->globals);
	bv_string_table_delete(prog->string_table);
	bv_name_list_delete(&prog->global_names);
	bv_block_delete(prog->block);

	free(prog->libraries);

	if (prog->external_functions != NULL)
		free(prog->external_functions);
	if (prog->external_function_names != NULL)
		free(prog->external_function_names);
	if (prog->opcodes != NULL)
		free(prog->opcodes);
	free(prog);
}

u16 bv_program_get_function_count(bv_program * prog)
{
	return prog->block->functions->count;
}
bv_function* bv_program_get_function(bv_program* prog, const bv_string str)
{
	u16 func_count = bv_program_get_function_count(prog);

	for (u16 i = 0; i < func_count; i++)
		if (strcmp(prog->block->functions->names[i], str) == 0)
			return prog->functions[i];
	
	return 0;
}
bv_external_function bv_program_get_ext_function(bv_program * prog, const bv_string str)
{
	for (u16 i = 0; i < prog->external_function_count; i++)
		if (strcmp(prog->external_function_names[i], str) == 0)
			return prog->external_functions[i];

	for (u16 i = 0; i < prog->lib_count; i++) {
		bv_external_function extf = bv_library_get_function(prog->libraries[i], str);
		if (extf != NULL)
			return extf;
	}
	
	return NULL;
}
void bv_program_add_function(bv_program * prog, const bv_string name, bv_external_function ext_func)
{
	prog->external_functions = (bv_external_function*)realloc(prog->external_functions, sizeof(bv_external_function) * (prog->external_function_count + 1));
	prog->external_function_names = (bv_string*)realloc(prog->external_function_names, sizeof(bv_string) * (prog->external_function_count + 1));
	
	prog->external_functions[prog->external_function_count] = ext_func;
	prog->external_function_names[prog->external_function_count] = (bv_string)name;

	prog->external_function_count++;
}

void bv_program_add_library(bv_program * prog, bv_library * lib)
{
	prog->lib_count++;
	prog->libraries = (bv_library*)realloc(prog->libraries, sizeof(bv_library) * prog->lib_count);
	prog->libraries[prog->lib_count - 1] = lib;
}

void bv_program_add_object_info(bv_program* prog, bv_object_info* obj)
{
	bv_object_pool_add(prog->block->objects, obj);
}
bv_object_info* bv_program_get_object_info(bv_program* prog, const bv_string name)
{
	bv_object_pool* pool = prog->block->objects;

	for (u16 i = 0; i < pool->count; i++)
		if (strcmp(name, pool->info[i]->name) == 0)
			return pool->info[i];

	for (u8 i = 0; i < prog->lib_count; i++) {
		bv_library* lib = prog->libraries[i];
		bv_object_info* ret = bv_object_pool_get(lib->objects, name);
		if (ret != NULL)
			return ret;
	}

	return 0;
}

u16 bv_program_get_global_count(bv_program * prog)
{
	return prog->globals.length;
}
bv_variable* bv_program_get_global(bv_program* prog, bv_string name)
{
	u16 ind = bv_name_list_get_id(prog->global_names, name);

	if (ind == prog->global_names.name_count) {
		for (u8 i = 0; i < prog->lib_count; i++) {
			bv_library* lib = prog->libraries[i];
			u16 ext_ind = bv_name_list_get_id(lib->global_names, name);
			if (ext_ind < lib->global_names.name_count)
				return &lib->globals.data[ext_ind];
		}
	}
	else return &prog->globals.data[ind];

	return NULL;
}
void bv_program_add_global(bv_program * prog, bv_string name)
{
	bv_stack_push(&prog->globals, bv_variable_create_null_object());
	bv_name_list_add(&prog->global_names, name);
}
void bv_program_set_global(bv_program * prog, const bv_string name, bv_variable var)
{
	u16 ind = bv_name_list_get_id(prog->global_names, name);
	if (ind != prog->global_names.name_count)
		prog->globals.data[ind] = var;
	else {
		for (u8 i = 0; i < prog->lib_count; i++) {
			bv_library* lib = prog->libraries[i];
			u16 ext_ind = bv_name_list_get_id(lib->global_names, name);
			if (ext_ind < lib->global_names.name_count)
				lib->globals.data[ext_ind] = var;
		}
	}
}

bv_variable bv_program_call(bv_program* prog, bv_function* func, bv_stack* args, bv_object* parent)
{
	bv_variable rtrn;

	bv_scope* scope = bv_scope_create();
	bv_scope_push(scope, bv_scope_type_function, func->code, prog, func, parent, 0);

	if (args != NULL && args->length == func->args) {
		// push arguments to local variables
		for (u16 i = 0; i < args->length; i++)
			bv_stack_push(&scope->locals, args->data[i]);
	}

	// run while we still have some functions in our array
	u32 cnt = 1;
	u8 is_in = 1;
	bv_opcode op = 0;
	while (scope->count != 0) {
		bv_state* state = bv_scope_get_state(scope);
		cnt = scope->count;

		while (cnt == scope->count && (is_in = (u32)(state->code - state->func->code) < state->func->code_length)) {
			op = bv_opcode_read(&state->code);
			(*prog->opcodes[op])(scope);
		}

		if (!is_in && scope->count != 0 && cnt == scope->count)
			bv_scope_pop(scope);
	}

	// get return value
	if (scope->stack.length > 0)
		rtrn = bv_variable_copy(bv_stack_top(&scope->stack)); // on return, make a copy of the return value
	else
		rtrn = bv_variable_create_void();

	bv_scope_delete(scope);

	return rtrn;
}
