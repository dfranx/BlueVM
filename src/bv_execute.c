#include <BlueVM/bv_execute.h>
#include <BlueVM/bv_variable.h>
#include <BlueVM/bv_function.h>
#include <BlueVM/bv_array.h>
#include <BlueVM/bv_program.h>
#include <stdlib.h>

void bv_execute_unknown(bv_scope* scope) {
	bv_program_error(bv_scope_get_state(scope)->prog, 1, 47, "Unknown operator called");
}
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
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 48, "Not enough values on the stack for the operator +");
		return;
	}

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);
	
	bv_variable res = bv_variable_op_add(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_subtract(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 49, "Not enough values on the stack for the operator -");
		return;
	}

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	bv_variable res = bv_variable_op_subtract(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_multiply(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 50, "Not enough values on the stack for the operator *");
		return;
	}

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

	if (scope->stack.length < 2) {
		bv_program_error(state->prog, 0, 51, "Not enough values on the stack for the operator /");
		return;
	}

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
	bv_variable_op_increment(state->prog, &var);
}
void bv_execute_decrement(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_variable var = bv_stack_top(&scope->stack);
	bv_variable_op_decrement(state->prog, &var);
}
void bv_execute_negate(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_variable var = bv_stack_top(&scope->stack);
	bv_variable res = bv_variable_op_negate(state->prog, var);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_modulo(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 52, "Not enough values on the stack for the operator %");
		return;
	}

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	bv_variable res = bv_variable_op_modulo(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, res);
}
void bv_execute_bit_or(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 53, "Not enough values on the stack for the operator |");
		return;
	}

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u32 res = bv_variable_get_uint(var1) | bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, (void*)res));
}
void bv_execute_bit_and(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 54, "Not enough values on the stack for the operator &");
		return;
	}

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u32 res = bv_variable_get_uint(var1) & bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, (void*)res));
}
void bv_execute_bit_not(bv_scope* scope) {
	bv_variable var = bv_stack_top(&scope->stack);

	u32 res = ~bv_variable_get_uint(var);
	bv_type type = var.type;

	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, (void*)res));
}
void bv_execute_bit_xor(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 55, "Not enough values on the stack for the operator ^");
		return;
	}

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u32 res = bv_variable_get_uint(var1) ^ bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, (void*)res));
}
void bv_execute_bit_lshift(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 56, "Not enough values on the stack for the operator <<");
		return;
	}

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u32 res = bv_variable_get_uint(var1) << bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, (void*)res));
}
void bv_execute_bit_rshift(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 57, "Not enough values on the stack for the operator >>");
		return;
	}

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u32 res = bv_variable_get_uint(var1) >> bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create(type, (void*)res));
}
void bv_execute_bool_or(bv_scope * scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 58, "Not enough values on the stack for the operator ||");
		return;
	}

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_get_uchar(var1) || bv_variable_get_uchar(var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_bool_and(bv_scope * scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 59, "Not enough values on the stack for the operator &&");
		return;
	}

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_get_uchar(var1) && bv_variable_get_uchar(var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_equal(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 60, "Not enough values on the stack for the operator ==");
		return;
	}

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_op_equal(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_not_equal(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 61, "Not enough values on the stack for the operator !=");
		return;
	}

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
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 62, "Not enough values on the stack for the operator >");
		return;
	}

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_op_greater_than(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_less(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 63, "Not enough values on the stack for the operator <");
		return;
	}

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_op_less_than(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_greater_equal(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 64, "Not enough values on the stack for the operator >=");
		return;
	}

	bv_state* state = bv_scope_get_state(scope);

	bv_variable var1 = bv_stack_penultimate(&scope->stack);
	bv_variable var2 = bv_stack_top(&scope->stack);

	u8 res = bv_variable_op_greater_equal(state->prog, var1, var2);

	bv_stack_pop_free(&scope->stack);
	bv_stack_pop_free(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_uchar(res));
}
void bv_execute_less_equal(bv_scope* scope) {
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 65, "Not enough values on the stack for the operator <=");
		return;
	}

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

	bv_variable res = bv_variable_cast(new_type, var);

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

	if (index >= scope->locals.length) {
		bv_program_error(state->prog, 0, 66, "Trying to access non-existent local value");
		bv_stack_push(&scope->stack, bv_variable_create_int(0)); // push a 0 to the stack
	} else {
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

	if (index >= state->prog->globals.length) {
		bv_program_error(state->prog, 0, 67, "Trying to access non-existent global value");
		bv_stack_push(&scope->stack, bv_variable_create_int(0)); // push a 0 to the stack
	} else
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

	if (var.type == bv_type_string) {
		bv_string str = bv_variable_get_string(var);
		u16 ind = bv_variable_get_ushort(bv_stack_top(&scope->stack));
		bv_stack_pop(&scope->stack);

		bv_stack_push(&scope->stack, bv_variable_create_char(str[ind]));
	} else {
		bv_array* arr = bv_variable_get_array(var);

		u16* lens = (u16*)malloc(sizeof(u16) * arr->dim);
		u8 i = arr->dim;
		while (i != 0) {
			lens[arr->dim - i] = bv_variable_get_int(bv_stack_top(&scope->stack));
			bv_stack_pop_free(&scope->stack);
			i--;
		}

		bv_stack_push(&scope->stack, bv_variable_copy(bv_array_get(*arr, lens)));

		bv_variable_deinitialize(&var);
		free(lens);
	}
}
void bv_execute_set_array_el(bv_scope* scope) {
	bv_variable var = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);

	if (var.type == bv_type_string) {
		bv_string str = bv_variable_get_string(var);

		u16 ind = bv_variable_get_ushort(bv_stack_top(&scope->stack));
		bv_stack_pop(&scope->stack);

		bv_variable value = bv_stack_top(&scope->stack);
		bv_stack_pop(&scope->stack);

		if (value.type == bv_type_char || value.type == bv_type_uchar)
			str[ind] = bv_variable_get_char(value);
		else if (value.type == bv_type_string) {// some languages dont support char data type (stupid JS)
			bv_string val_str = bv_variable_get_string(value);
			str[ind] = val_str[0];
			bv_variable_deinitialize(&value);
		}

		bv_stack_push(&scope->stack, var);
	} else {
		bv_array* arr = bv_variable_get_array(var);

		u16* lens = (u16*)malloc(sizeof(u16) * arr->dim);
		u8 i = arr->dim;

		while (i != 0) {
			lens[arr->dim - i] = bv_variable_get_int(bv_stack_top(&scope->stack));
			bv_stack_pop_free(&scope->stack);
			i--;
		}

		bv_variable value = bv_stack_top(&scope->stack);
		bv_stack_pop(&scope->stack);

		bv_array_set(*arr, lens, value);
		bv_variable_set_array(&var, *arr);

		bv_stack_push(&scope->stack, var);

		bv_variable_deinitialize(&value);
		free(lens);
	}
}
void bv_execute_call(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_function* func = bv_program_get_function(state->prog, name);

	if (scope->stack.length < argc) {
		bv_program_error(state->prog, 0, 1, "Not enough arguments for a function call (opcode call)");
		return;
	}

	if (func != NULL)
		bv_scope_push(scope, bv_scope_type_function, func->code, state->prog, func, NULL, argc);
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

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_function* func = bv_program_get_function(state->prog, name);
	
	if (scope->stack.length < argc) {
		bv_program_error(state->prog, 0, 2, "Not enough arguments for a function call (opcode call_return)");
		return;
	}

	if (func != NULL) {
		bv_scope_push(scope, bv_scope_type_function, func->code, NULL, func, NULL, argc);
	} else {
		bv_stack func_args = bv_stack_create();

		for (u8 i = 0; i < argc; i++) {
			bv_stack_push(&func_args, bv_stack_top(&scope->stack));
			bv_stack_pop(&scope->stack);
		}

		bv_external_function ext_func = bv_program_get_ext_function(state->prog, name);
		bv_stack_push(&scope->stack, (*ext_func)(argc, func_args.data));

		bv_stack_delete(&func_args);
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
	bv_variable var = bv_stack_top(&scope->stack);

	if (bv_type_is_integer(var.type))
		if (bv_variable_get_int(var) == 0)
			state->code = state->func->code + addr;

	bv_stack_pop_free(&scope->stack);
}
void bv_execute_goto(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	u32 addr = u32_read(&state->code);
	state->code = state->func->code + addr;
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

	if (scope->stack.length < argc) {
		bv_program_error(state->prog, 0, 68, "Not enough arguments provided to the object constructor");
		return;
	}

	bv_function* constructor = bv_object_get_method(obj, info->name); // get constructor

	if (constructor != 0)
		bv_scope_push(scope, bv_scope_type_constructor, constructor->code, state->prog, constructor, obj, argc);
	else {
		bv_stack func_args = bv_stack_create();

		for (u8 i = 0; i < argc; i++) {
			bv_stack_push(&func_args, bv_stack_top(&scope->stack));
			bv_stack_pop(&scope->stack);
		}

		bv_external_method ext_func = bv_object_get_ext_method(obj, info->name);
		(*ext_func)(obj, argc, func_args.data);

		bv_stack_delete(&func_args);
	}

	bv_stack_push(&scope->stack, var);
}
void bv_execute_set_prop(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	bv_variable obj = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);

	if (obj.type == bv_type_pointer)
		obj = *((bv_variable*)obj.value);

	bv_variable val = bv_variable_copy(bv_stack_top(&scope->stack));

	bv_object_set_property(bv_variable_get_object(obj), name, val);
	bv_stack_pop_free(&scope->stack);
}
void bv_execute_set_my_prop(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	if (state->obj == 0) {
		bv_program_error(state->prog, 0, 69, "Parent(self,this,etc...) not passed to this state");
		return;
	}
	
	bv_object_set_property(state->obj, name, bv_variable_copy(bv_stack_top(&scope->stack)));
	bv_stack_pop(&scope->stack);
}
void bv_execute_get_prop(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	bv_variable var = bv_stack_top(&scope->stack);
	bv_object* top = NULL;
	if (var.type == bv_type_pointer) top = bv_variable_get_object(*(bv_variable*)var.value);
	else top = bv_variable_get_object(var);

	bv_stack_pop(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_copy(*bv_object_get_property(top, name)));

	//bv_variable_deinitialize(&var);
}
void bv_execute_get_my_prop(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	if (state->obj == 0) {
		bv_program_error(state->prog, 0, 70, "Parent(self,this,etc...) not passed to this state");
		return;
	}

	bv_stack_push(&scope->stack, bv_variable_copy(*bv_object_get_property(state->obj, name)));
}
void bv_execute_call_method(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_variable var = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);

	if (var.type == bv_type_pointer)
		var = *((bv_variable*)var.value);

	bv_object* obj = bv_variable_get_object(var);

	if (scope->stack.length < argc) {
		bv_program_error(state->prog, 0, 3, "Not enough arguments for a method call (opcode call_method)");
		return;
	}

	bv_object_call_method(obj, name, scope, argc);
}
void bv_execute_call_my_method(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_object* obj = state->obj;

	if (scope->stack.length < argc) {
		bv_program_error(state->prog, 0, 4, "Not enough arguments for a method call (opcode call_my_method)");
		return;
	}

	bv_object_call_method(obj, name, scope, argc);
}
void bv_execute_call_ret_method(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_variable var = bv_stack_top(&scope->stack);
	bv_stack_pop(&scope->stack);

	if (var.type == bv_type_pointer)
		var = *((bv_variable*)var.value);

	bv_object* obj = bv_variable_get_object(var);

	bv_function* func = bv_object_get_method(obj, name);

	if (scope->stack.length < argc) {
		bv_program_error(state->prog, 0, 5, "Not enough arguments for a method call (opcode call_ret_method)");
		return;
	}

	bv_variable ret = bv_object_call_method(obj, name, scope, argc);

	if (ret.type != bv_type_void)
		bv_stack_push(&scope->stack, ret);
}
void bv_execute_call_ret_my_method(bv_scope* scope) {
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);

	bv_object* obj = state->obj;

	bv_function* func = bv_object_get_method(obj, name);

	if (scope->stack.length < argc) {
		bv_program_error(state->prog, 0, 6, "Not enough arguments for a method call (opcode call_ret_my_method)");
		return;
	}

	bv_variable ret = bv_object_call_method(obj, name, scope, argc);

	if (ret.type != bv_type_void)
		bv_stack_push(&scope->stack, ret);
}
void bv_execute_scope_start(bv_scope * scope)
{
	bv_state* state = bv_scope_get_state(scope);
	bv_scope_push(scope, bv_scope_type_normal, state->code, state->prog, state->func, state->obj, 0);
}
void bv_execute_scope_end(bv_scope * scope)
{
	bv_scope_pop(scope);
}
void bv_execute_assign(bv_scope* scope)
{
	if (scope->stack.length < 2) {
		bv_program_error(bv_scope_get_state(scope)->prog, 0, 71, "Not enough values on the stack for the assign operator");
		return;
	}

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

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	bv_variable var = bv_stack_top(&scope->stack);
	bv_object* obj = NULL;
	if (var.type == bv_type_pointer) obj = bv_variable_get_object(*(bv_variable*)var.value);
	else obj = bv_variable_get_object(var);
	
	bv_stack_pop(&scope->stack);

	bv_stack_push(&scope->stack, bv_variable_create_pointer(bv_object_get_property(obj, name)));

	// TODO: push object to garbage collector
	//bv_variable_deinitialize(&var);
}
void bv_execute_get_my_prop_pointer(bv_scope * scope)
{
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	if (state->obj == 0) {
		bv_program_error(state->prog, 0, 72, "Parent(self,this,etc...) not passed to this state");
		return;
	}

	bv_stack_push(&scope->stack, bv_variable_create_pointer(bv_object_get_property(state->obj, name)));
}
void bv_execute_get_global_by_name(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	bv_variable* global = bv_program_get_global(state->prog, name);

	if (global != NULL)
		bv_stack_push(&scope->stack, bv_variable_copy(*global));
	else
		bv_program_error(state->prog, 0, 73, "Could not find global using it's name");
}
void bv_execute_get_global_by_name_ptr(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	bv_variable* global = bv_program_get_global(state->prog, name);

	if (global != NULL)
		bv_stack_push(&scope->stack, bv_variable_create_pointer(global));
	else
		bv_program_error(state->prog, 0, 74, "Could not find global (it's pointer) using it's name");
}
void bv_execute_set_global_by_name(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	
	bv_variable* global = bv_program_get_global(state->prog, name);

	if (global != NULL) {
		bv_variable var = bv_stack_top(&scope->stack);
		bv_stack_pop(&scope->stack);

		bv_variable_deinitialize(global);

		*global = var;
	}
	else
		bv_program_error(state->prog, 0, 75, "Could not find global using it's name");
}
void bv_execute_empty_stack(bv_scope* scope)
{
	bv_stack* stack = &scope->stack;
	u32 start = bv_scope_get_stack_start(scope);

	for (u32 i = start; i < stack->length; i++)
		bv_stack_pop_free(stack);
}
void bv_execute_debug_line_number(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);
	state->prog->current_line = u32_read(&state->code);
}
void bv_execute_debug_file(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);
	state->prog->current_file = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
}
void bv_execute_debug_breakpoint(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);
	if (state->prog->debugger != NULL)
		state->prog->debugger(scope);
}
void bv_execute_new_object_by_name(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);

	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));
	u8 argc = u8_read(&state->code);


	bv_object_info* info = bv_program_get_object_info(state->prog, name);
	bv_variable var = bv_variable_create_object(info);
	bv_object* obj = bv_variable_get_object(var);

	if (scope->stack.length < argc) {
		bv_program_error(state->prog, 0, 68, "Not enough arguments provided to the object constructor");
		return;
	}

	bv_function* constructor = bv_object_get_method(obj, info->name); // get constructor

	if (constructor != NULL)
		bv_scope_push(scope, bv_scope_type_constructor, constructor->code, state->prog, constructor, obj, argc);
	else {
		bv_external_method ext_func = bv_object_get_ext_method(obj, info->name);

		if (ext_func != NULL) {
			bv_stack func_args = bv_stack_create();

			for (u8 i = 0; i < argc; i++) {
				bv_stack_push(&func_args, bv_stack_top(&scope->stack));
				bv_stack_pop(&scope->stack);
			}

			(*ext_func)(obj, argc, func_args.data);

			bv_stack_delete(&func_args);
		}
	}

	bv_stack_push(&scope->stack, var);
}
void bv_execute_push_stack_function(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);
	bv_string name = bv_string_table_get_string(state->prog->string_table, u32_read(&state->code));

	bv_function* func = bv_program_get_function(state->prog, name);
	bv_stack_push(&scope->stack, bv_variable_create_function(func));
}
void bv_execute_call_stack(bv_scope* scope)
{
	bv_state* state = bv_scope_get_state(scope);

	u8 argc = u8_read(&state->code);

	bv_variable var2 = bv_stack_top(&scope->stack);
	bv_function* func = bv_variable_get_function(var2);
	bv_stack_pop_free(&scope->stack);

	bv_scope_push(scope, bv_scope_type_function, func->code, state->prog, func, NULL, argc);
}
