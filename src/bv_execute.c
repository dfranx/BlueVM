#include <BlueVM/bv_execute.h>
#include <BlueVM/bv_variable.h>
#include <BlueVM/bv_function.h>
#include <BlueVM/bv_array.h>
#include <BlueVM/bv_program.h>
#include <stdlib.h>

void bv_execute_unknown(bv_scope* scope) { }
void bv_execute_func_start(bv_scope* scope) { }
void bv_execute_return(bv_scope* scope) {
	bv_variable ret_value = bv_variable_create_void();
	u8 has_value = scope->stack.length > 0;

	if (has_value)
		ret_value = bv_variable_copy(bv_stack_top(&scope->stack));

	bv_scope_pop(scope);

	if (has_value)
		bv_stack_push(&scope->stack, ret_value);
}
void bv_execute_push_stack(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_type type = bv_type_read(&state->code);
	if (type == bv_type_string)
		bv_stack_push(&scope->stack, bv_variable_create_string(bv_string_table_get_string(state->prog->string_table, u32_read(&state->code))));
	else
		bv_stack_push(&scope->stack, bv_variable_read(&state->code, type));
}
void bv_execute_pop_stack(bv_scope* scope) {
	bv_stack_pop_free(&scope->stack);
}
void bv_execute_const_get(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_type type = bv_type_read(&state->code);
	u16 index = u16_read(&state->code);

	bv_constant_pool* cpool = state->prog->block->constants;
	for (u8 i = 0; i < cpool->type_count; i++)
		if (cpool->val_type[i] == type) {
			if (index < cpool->val_count[i])
				bv_stack_push(&scope->stack, bv_variable_copy(cpool->val[i][index]));
			break;
		}
}
void bv_execute_add(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments on stack
		return;

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);
	
	bv_variable res = bv_variable_op_add(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_subtract(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	bv_variable res = bv_variable_op_subtract(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_multiply(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	bv_variable res = bv_variable_op_multiply(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_divide(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	bv_variable res = bv_variable_op_divide(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_increment(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_variable var = bv_stack_top(&scope->stack);
	bv_variable res = bv_variable_op_increment(state->prog, var);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_decrement(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_variable var = bv_stack_top(&scope->stack);
	bv_variable res = bv_variable_op_decrement(state->prog, var);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_negate(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_variable var = bv_stack_top(&scope->stack);
	bv_variable res = bv_variable_op_negate(state->prog, var);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_modulo(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	bv_variable res = bv_variable_op_modulo(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_bit_or(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u32 res = bv_variable_get_uint(var1) | bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, res));
}
void bv_execute_bit_and(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u32 res = bv_variable_get_uint(var1) & bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, res));
}
void bv_execute_bit_not(bv_scope* scope) {
	bv_variable var = bv_stack_top(&scope->stack);

	u32 res = ~bv_variable_get_uint(var);
	bv_type type = var.type;

	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, res));
}
void bv_execute_bit_xor(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u32 res = bv_variable_get_uint(var1) ^ bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, res));
}
void bv_execute_bit_lshift(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u32 res = bv_variable_get_uint(var1) << bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, res));
}
void bv_execute_bit_rshift(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u32 res = bv_variable_get_uint(var1) >> bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, res));
}
void bv_execute_bool_or(bv_scope * scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_get_uchar(var1) || bv_variable_get_uchar(var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_bool_and(bv_scope * scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_get_uchar(var1) && bv_variable_get_uchar(var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_equal(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_op_equal(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_not_equal(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_op_not_equal(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_not(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_variable var = bv_stack_top(&scope->stack);
	u8 res = bv_variable_op_not(state->prog, var);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_greater(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_op_greater_than(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_less(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;
		
	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);
	
	u8 res = bv_variable_op_less_than(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_greater_equal(bv_scope* scope) {
	
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_op_greater_equal(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_less_equal(bv_scope* scope) {
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_op_less_equal(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_nop(bv_scope* scope){}
void bv_execute_convert(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_variable var = bv_stack_top(&scope->stack);
	bv_type new_type = bv_type_read(&state->code);

	bv_variable res = bv_variable_cast(state->prog, new_type, var);

	bv_stack_pop_free(&scope->stack);
	bv_stack_push(&scope->stack, res);
}
void bv_execute_duplicate(bv_scope* scope) {
	bv_variable var = bv_variable_copy(bv_stack_top(&scope->stack));
	bv_stack_push(&scope->stack, var);
}
void bv_execute_swap(bv_scope* scope) {
	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);
	scope->stack.data[scope->stack.length - 2] = var2;
	scope->stack.data[scope->stack.length - 1] = var1;
}
void bv_execute_get_local(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	u32 index = bv_scope_get_locals_start(scope)+u16_read(&state->code);

	if (index >= scope->locals.length)
		bv_stack_push(&scope->stack, bv_variable_create_int(0)); // push a 0 to the stack
	else {
		bv_variable* pLocal = &scope->locals.data[index];
		bv_stack_push(&scope->stack, bv_variable_copy(*pLocal));
	}
}
void bv_execute_set_local(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	u16 index = bv_scope_get_locals_start(scope) + u16_read(&state->code);
	bv_variable var = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);
	
	if (index == scope->locals.length) { // 'declare' a new variable
		bv_stack_push(&scope->locals, var);
	} else {
		bv_variable* pLocal = &scope->locals.data[index];
		
		bv_variable_deinitialize(pLocal); // first deinitialize old value

		*pLocal = var;
	}
}
void bv_execute_get_global(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	u16 index = u16_read(&state->code);

	if (index >= state->prog->globals.length)
		bv_stack_push(&scope->stack, bv_variable_create_int(0)); // push a 0 to the stack
	else
		bv_stack_push(&scope->stack, bv_variable_copy(state->prog->globals.data[index]));
}
void bv_execute_set_global(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	u16 index = u16_read(&state->code);

	bv_variable var = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);

	bv_variable* pLocal = &state->prog->globals.data[index];

	bv_variable_deinitialize(pLocal);

	*pLocal = var;
}
void bv_execute_push_array(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	u8 dim = u8_read(&state->code);

	if (scope->stack.length < dim)
		return;

	u16* lens = (u16*)malloc(sizeof(u16) * dim);
	u8 i = dim;
	while (i != 0) {
		lens[dim - i] = bv_variable_get_int(bv_stack_top(&scope->stack));
		bv_stack_pop_free(&scope->stack);
		i--;
	}

	bv_array ret_arr = bv_array_create(dim, lens);
	u32 cnt = bv_array_get_range(ret_arr);
	for (u32 j = 0; j < cnt; j++)
		ret_arr.data[j] = bv_variable_create_void();
	
	bv_stack_push(&scope->stack, bv_variable_create_array(ret_arr));
	free(lens);
}
void bv_execute_get_array_el(bv_scope* scope) {
	bv_variable var = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);

	bv_array arr = bv_variable_get_array(var);

	u16* lens = (u16*)malloc(sizeof(u16) * arr.dim);
	u8 i = arr.dim;
	while (i != 0) {
		lens[arr.dim - i] = bv_variable_get_int(bv_stack_top(&scope->stack));
		bv_stack_pop_free(&scope->stack);
		i--;
	}

	bv_stack_push(&scope->stack, bv_array_get(arr, lens));

	bv_variable_deinitialize(&var);
	free(lens);
}
void bv_execute_set_array_el(bv_scope* scope) {
	bv_variable arr_holder = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);

	bv_variable value = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);

	bv_array arr = bv_variable_get_array(arr_holder);

	u16* lens = (u16*)malloc(sizeof(u16) * arr.dim);
	u8 i = arr.dim;

	while (i != 0) {
		lens[arr.dim - i] = bv_variable_get_int(bv_stack_top(&scope->stack));
		bv_stack_pop_free(&scope->stack);
		i--;
	}

	bv_array_set(arr, lens, value);
	bv_variable_set_array(&arr_holder, arr);

	bv_stack_push(&scope->stack, arr_holder);

	free(lens);
}
void bv_execute_call(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_function* func = bv_program_get_function(state->prog, name);

	if (scope->stack.length < argc)
		return; // [TODO] error, not enough arguments

	if (func != NULL)
		bv_scope_push(scope, bv_scope_type_function, func->code, bv_scope_get_state(scope)->prog, func, NULL, argc);
	else {
		bv_stack func_args = bv_stack_create();

		for (u8 i = 0; i < argc; i++) {
			bv_stack_push(&func_args, bv_stack_top(&scope->stack));
			bv_stack_pop(&scope->stack);
		}

		bv_external_function ext_func = bv_program_get_ext_function(state->prog, name);
		(*ext_func)(argc, func_args.data);

		bv_stack_delete(&func_args);
	}
}
void bv_execute_call_return(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_function* func = bv_program_get_function(state->prog, name);
	
	if (scope->stack.length < argc)
		return; // [TODO] error, not enough arguments

	if (func != NULL) {
		bv_scope_push(scope, bv_scope_type_function, func->code, NULL, func, NULL, argc); // TODO: no need for the call_return anymore
	} else {
		bv_stack func_args = bv_stack_create();

		for (u8 i = 0; i < argc; i++) {
			bv_stack_push(&func_args, bv_stack_top(&scope->stack));
			bv_stack_pop(&scope->stack);
		}

		bv_external_function ext_func = bv_program_get_ext_function(state->prog, name);
		bv_stack_push(&scope->stack, (*ext_func)(argc, func_args.data));
	}
}
void bv_execute_is_type_of(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_type type = bv_type_read(&state->code);

	bv_type stack_type = bv_stack_top(&scope->stack).type;
	bv_stack_pop_free(&scope->stack);

	if (stack_type == type)
		bv_stack_push(&scope->stack, bv_variable_create_char(1));
	else
		bv_stack_push(&scope->stack, bv_variable_create_char(0));
}
void bv_execute_if(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	u32 addr = u32_read(&state->code);
	/*
	Invalid read of size 8
		==283==    at 0x405E8D: bv_stack_top (in /mnt/e/BlueVM/BlueVM)
		==283==    by 0x40457A: bv_execute_if (in /mnt/e/BlueVM/BlueVM)
		==283==    by 0x405CB1: bv_program_call (in /mnt/e/BlueVM/BlueVM)
		==283==    by 0x400E29: main (in /mnt/e/BlueVM/BlueVM)
		==283==  Address 0xfffffffffffffff8 is not stack'd, malloc'd or (recently) free'd
	*/
	bv_variable var = bv_stack_top(&scope->stack);

	if (bv_type_is_integer(var.type))
		if (bv_variable_get_int(var) == 0)
			state->code = state->this_func->code + addr;

	bv_stack_pop_free(&scope->stack);
}
void bv_execute_goto(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	u32 addr = u32_read(&state->code);
	state->code = state->this_func->code + addr;
}
void bv_execute_new_object(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	u16 id = u16_read(&state->code);
	u8 argc = u8_read(&state->code);

	if (id == 0) {
		bv_stack_push(&scope->stack, bv_variable_create_null_object());
		return;
	}
	id--;

	bv_object_info* info = state->prog->block->objects->info[id];
	bv_variable var = bv_variable_create_object(info);
	bv_object* obj = bv_variable_get_object(var);

	bv_stack func_args = bv_stack_create();
	if (scope->stack.length >= argc)
		for (u8 i = 0; i < argc; i++) {
			bv_stack_push(&func_args, bv_stack_top(&scope->stack));
			bv_stack_pop(&scope->stack);
		}

	bv_function* constructor = bv_object_get_method(obj, info->name); // get constructor

	if (constructor != 0) 
		bv_program_call(state->prog, constructor, &func_args, obj); // call constructor

	bv_stack_push(&scope->stack, var);

	bv_stack_delete(&func_args);
}
void bv_execute_set_prop(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	bv_variable obj = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);

	if (obj.type == bv_type_pointer)
		obj = *((bv_variable*)obj.value);

	bv_object_set_property(bv_variable_get_object(obj), name, bv_stack_top(&scope->stack));
	bv_stack_pop(&scope->stack);
}
void bv_execute_set_my_prop(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	if (state->obj == 0)
		return;

	bv_object_set_property(state->obj, name, bv_stack_top(&scope->stack));
	bv_stack_pop(&scope->stack);
}
void bv_execute_get_prop(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	bv_variable obj = bv_stack_top(&scope->stack);
	bv_object* top = bv_variable_get_object(obj);
	bv_stack_pop(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_copy(*bv_object_get_property(top, name)));

	bv_variable_deinitialize(&obj);
}
void bv_execute_get_my_prop(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	if (state->obj == 0)
		return;

	bv_stack_push(&scope->stack, bv_variable_copy(*bv_object_get_property(state->obj, name)));
}
void bv_execute_call_method(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_variable var = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);

	if (var.type == bv_type_pointer)
		var = *((bv_variable*)var.value);

	bv_object* obj = bv_variable_get_object(var);

	if (scope->stack.length < argc)
		return; // [TODO] error, not enough arguments

	bv_object_call_method(obj, name, scope, argc);
}
void bv_execute_call_my_method(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_object* obj = state->obj;

	if (scope->stack.length < argc)
		return; // [TODO] error, not enough arguments

	bv_object_call_method(obj, name, scope, argc);
}
void bv_execute_call_ret_method(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_variable var = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);

	if (var.type == bv_type_pointer)
		var = *((bv_variable*)var.value);

	bv_object* obj = bv_variable_get_object(var);

	bv_function* func = bv_object_get_method(obj, name);

	if (scope->stack.length < argc)
		return; // [TODO] error, not enough arguments

	bv_object_call_method(obj, name, scope, argc);
}
void bv_execute_call_ret_my_method(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_object* obj = state->obj;

	bv_function* func = bv_object_get_method(obj, name);

	if (scope->stack.length < argc)
		return; // [TODO] error, not enough arguments

	bv_object_call_method(obj, name, scope, argc);
}
void bv_execute_scope_start(bv_scope * scope)
{
	bv_state* state = bv_scope_get_state(scope);
	bv_scope_push(scope, bv_scope_type_normal, state->code, state->prog, state->this_func, state->obj, 0);
}
void bv_execute_scope_end(bv_scope * scope)
{
	bv_scope_pop(scope);
}
void bv_execute_assign(bv_scope* scope)
{
	if (scope->stack.length < 2) // dont do anything if there is not enough arguments on stack
		return;

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var = bv_stack_penultimate(&scope->stack);
	bv_variable val = bv_stack_top(&scope->stack);

	bv_variable_op_assign(state->prog, &var, val);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);
}
void bv_execute_get_local_pointer(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);

	u32 index = bv_scope_get_locals_start(scope) + u16_read(&state->code);

	if (index >= scope->locals.length)
		bv_stack_push(&scope->stack, bv_variable_create_int(0)); // push a 0 to the stack (variable not found)
	else
		bv_stack_push(&scope->stack, bv_variable_create_pointer(&scope->locals.data[index]));
}
void bv_execute_get_global_pointer(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);

	u16 index = u16_read(&state->code);

	if (index >= state->prog->globals.length)
		bv_stack_push(&scope->stack, bv_variable_create_int(0)); // push a 0 to the stack (variable not found)
	else
		bv_stack_push(&scope->stack, bv_variable_create_pointer(&state->prog->globals.data[index]));
}
void bv_execute_get_prop_pointer(bv_scope * scope)
{
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	bv_variable var = bv_stack_top(&scope->stack);
	bv_object* obj = bv_variable_get_object(var);
	bv_stack_pop(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_pointer(bv_object_get_property(obj, name)));

	bv_variable_deinitialize(&var);
}
void bv_execute_get_my_prop_pointer(bv_scope * scope)
{
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	if (state->obj == 0)
		return;

	bv_stack_push(&scope->stack, bv_variable_create_pointer(bv_object_get_property(state->obj, name)));
}
void bv_execute_get_global_by_name(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	u16 ind = bv_name_list_get_id(state->prog->global_names, name);

	if (ind < state->prog->global_names.name_count)
		bv_stack_push(&scope->stack, bv_variable_copy(state->prog->globals.data[ind]));
}
void bv_execute_get_global_by_name_ptr(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	u16 ind = bv_name_list_get_id(state->prog->global_names, name);

	if (ind < state->prog->global_names.name_count)
		bv_stack_push(&scope->stack, bv_variable_create_pointer(&state->prog->globals.data[ind]));
}
void bv_execute_set_global_by_name(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);

	string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u16 index = bv_name_list_get_id(state->prog->global_names, name);

	if (index < state->prog->global_names.name_count) {
		bv_variable var = bv_stack_top(&scope->stack);
		bv_stack_pop(&scope->stack);

		bv_variable* pLocal = &state->prog->globals.data[index];

		bv_variable_deinitialize(pLocal);

		*pLocal = var;
	}
}
void bv_execute_empty_stack(bv_scope* scope)
{
	bv_stack* stack = &scope->stack;
	u32 start = bv_scope_get_stack_start(scope);

	for (u32 i = start; i < stack->length; i++)
		bv_stack_pop_free(stack);
}