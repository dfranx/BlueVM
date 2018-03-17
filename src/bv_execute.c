#include <BlueVM/bv_execute.h>
#include <BlueVM/bv_variable.h>
#include <BlueVM/bv_function.h>
#include <BlueVM/bv_array.h>
#include <BlueVM/bv_program.h>
#include <stdlib.h>

void bv_execute_unknown(bv_state* state) { }
void bv_execute_func_start(bv_state* state) { }
void bv_execute_return(bv_state* state) {
	state->should_exit = 1;
}
void bv_execute_push_stack(bv_state* state) {
	bv_type type = bv_type_read(state->code);
	bv_stack_push(state->stack, bv_variable_read(state->code, type));
}
void bv_execute_pop_stack(bv_state* state) {
	bv_stack_pop(state->stack);
}
void bv_execute_const_get(bv_state* state) {
	bv_type type = bv_type_read(state->code);
	u16 index = u16_read(state->code);

	bv_constant_pool* cpool = state->prog->block->constants;
	for (u8 i = 0; i < cpool->type_count; i++)
		if (cpool->val_type[i] == type) {
			if (index < cpool->val_count[i])
				bv_stack_push(state->stack, bv_variable_copy(cpool->val[i][index]));
			break;
		}
}
void bv_execute_add(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments on stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);

	if (var1.type == bv_type_float || var2.type == bv_type_float) {
		float sum = 0;

		if (var1.type == bv_type_float)
			sum += bv_variable_get_float(var1);
		else sum += bv_variable_get_int(var1);
		if (var2.type == bv_type_float)
			sum += bv_variable_get_float(var2);
		else sum += bv_variable_get_int(var2);

		bv_stack_pop(state->stack);
		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create_float(sum));
	}
	else if (var1.type == bv_type_string && var2.type == bv_type_string) {
		string result = malloc(strlen(var1.value) + strlen(var2.value) + 1);
		strcpy(result, var1.value);
		strcpy(result + strlen(var1.value), var2.value);
		result[strlen(var1.value) + strlen(var2.value)] = 0;

		bv_stack_pop(state->stack);
		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create_string(result));
		free(result);
	}
	else {
		u32 sum = bv_variable_get_int(var1) + bv_variable_get_int(var2);
		bv_type type = var1.type;

		bv_stack_pop(state->stack);
		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create(type, sum));
	}
}
void bv_execute_subtract(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);

	if (var1.type == bv_type_float || var2.type == bv_type_float) {
		float sum = 0;

		if (var1.type == bv_type_float)
			sum = bv_variable_get_float(var1);
		else sum = bv_variable_get_int(var1);
		if (var2.type == bv_type_float)
			sum -= bv_variable_get_float(var2);
		else sum -= bv_variable_get_int(var2);

		bv_stack_pop(state->stack);
		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create_float(sum));
	}
	else {
		u32 sum = bv_variable_get_int(var1) - bv_variable_get_int(var2);
		bv_type type = var1.type;

		bv_stack_pop(state->stack);
		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create(type, sum));
	}
}
void bv_execute_multiply(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);

	if (var1.type == bv_type_float || var2.type == bv_type_float) {
		float sum = 0;

		if (var1.type == bv_type_float)
			sum = bv_variable_get_float(var1);
		else sum = bv_variable_get_int(var1);
		if (var2.type == bv_type_float)
			sum *= bv_variable_get_float(var2);
		else sum *= bv_variable_get_int(var2);

		bv_stack_pop(state->stack);
		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create_float(sum));
	}
	else {
		u32 sum = bv_variable_get_int(var1) * bv_variable_get_int(var2);
		bv_type type = var1.type;

		bv_stack_pop(state->stack);
		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create(type, sum));
	}
}
void bv_execute_divide(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);

	if (var1.type == bv_type_float || var2.type == bv_type_float) {
		float sum = 0;

		if (var1.type == bv_type_float)
			sum = bv_variable_get_float(var1);
		else sum = bv_variable_get_int(var1);
		if (var2.type == bv_type_float)
			sum /= bv_variable_get_float(var2); // [TODO] check if == 0
		else sum /= bv_variable_get_int(var2);

		bv_stack_pop(state->stack);
		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create_float(sum));
	}
	else {
		u32 sum = bv_variable_get_int(var1) / bv_variable_get_int(var2);
		bv_type type = var1.type;

		bv_stack_pop(state->stack);
		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create(type, sum));
	}
}
void bv_execute_increment(bv_state* state) {
	bv_variable var = bv_stack_top(state->stack);

	if (var.type == bv_type_float) {
		float val = bv_variable_get_float(var) + 1;

		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create_float(val));
	}
	else {
		u32 val = bv_variable_get_int(var) + 1;
		bv_type type = var.type;

		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create(type, val));
	}
}
void bv_execute_decrement(bv_state* state) {
	bv_variable var = bv_stack_top(state->stack);

	if (var.type == bv_type_float) {
		float val = bv_variable_get_float(var) - 1;

		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create_float(val));
	}
	else {
		u32 val = bv_variable_get_int(var) - 1;
		bv_type type = var.type;

		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create(type, val));
	}
}
void bv_execute_negate(bv_state* state) {
	bv_variable var = bv_stack_top(state->stack);

	if (var.type == bv_type_float) {
		float val = -bv_variable_get_float(var);

		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create_float(val));
	}
	else {
		u32 val = -bv_variable_get_int(var);
		bv_type type = var.type;

		bv_stack_pop(state->stack);

		bv_stack_push(state->stack, bv_variable_create(type, val));
	}
}
void bv_execute_modulo(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);
	bv_type type = var1.type;
	s32 val1 = 0, val2 = 1;

	if (type == bv_type_float)
		val1 = bv_variable_get_float(var1);
	else
		val1 = bv_variable_get_int(var1);

	if (var2.type == bv_type_float)
		val2 = bv_variable_get_float(var2);
	else
		val2 = bv_variable_get_int(var2);

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create_uint(val1 % val2));
}
void bv_execute_bit_or(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);

	u32 res = bv_variable_get_uint(var1) | bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create(type, res));
}
void bv_execute_bit_and(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);

	u32 res = bv_variable_get_uint(var1) & bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create(type, res));
}
void bv_execute_bit_not(bv_state* state) {
	bv_variable var = bv_stack_top(state->stack);

	u32 res = ~bv_variable_get_uint(var);
	bv_type type = var.type;

	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create(type, res));
}
void bv_execute_bit_xor(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);

	u32 res = bv_variable_get_uint(var1) ^ bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create(type, res));
}
void bv_execute_bit_lshift(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);

	u32 res = bv_variable_get_uint(var1) << bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create(type, res));
}
void bv_execute_bit_rshift(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);

	u32 res = bv_variable_get_uint(var1) >> bv_variable_get_uint(var2);
	bv_type type = var1.type;

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create(type, res));
}
void bv_execute_equal(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);
	u8 out = 0;

	if (var1.type == bv_type_float || var2.type == bv_type_float) {
		float x = 0, y = 0;

		if (var1.type == bv_type_float)
			x = bv_variable_get_float(var1);
		else
			x = bv_variable_get_int(var1);
		if (var2.type == bv_type_float)
			y = bv_variable_get_float(var2);
		else
			y = bv_variable_get_int(var2);

		out = (x == y);
	}
	else if (var1.type == bv_type_string && var2.type == bv_type_string)
		out = strcmp(var1.value, var2.value) == 0;
	else
		out = bv_variable_get_uint(var1) == bv_variable_get_uint(var2);

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create_uchar(out));
}
void bv_execute_not_equal(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);
	u8 out = 0;

	if (var1.type == bv_type_float || var2.type == bv_type_float) {
		float x = 0, y = 0;

		if (var1.type == bv_type_float)
			x = bv_variable_get_float(var1);
		else
			x = bv_variable_get_int(var1);
		if (var2.type == bv_type_float)
			y = bv_variable_get_float(var2);
		else
			y = bv_variable_get_int(var2);

		out = (x != y);
	}
	else if (var1.type == bv_type_string && var2.type == bv_type_string)
		out = strcmp(var1.value, var2.value) != 0;
	else
		out = bv_variable_get_uint(var1) != bv_variable_get_uint(var2);

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create_uchar(out));
}
void bv_execute_not(bv_state* state) {
	bv_variable var = bv_stack_top(state->stack);
	u8 out = !bv_variable_get_uchar(var);

	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create_uchar(out));
}
void bv_execute_greater(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);
	u8 out = 0;

	if (var1.type == bv_type_float || var2.type == bv_type_float) {
		float x = 0, y = 0;

		if (var1.type == bv_type_float)
			x = bv_variable_get_float(var1);
		else
			x = bv_variable_get_int(var1);
		if (var2.type == bv_type_float)
			y = bv_variable_get_float(var2);
		else
			y = bv_variable_get_int(var2);

		out = (x > y);
	}
	else
		out = bv_variable_get_uint(var1) > bv_variable_get_uint(var2);

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create_uchar(out));
}
void bv_execute_less(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);
	u8 out = 0;

	if (var1.type == bv_type_float || var2.type == bv_type_float) {
		float x = 0, y = 0;

		if (var1.type == bv_type_float)
			x = bv_variable_get_float(var1);
		else
			x = bv_variable_get_int(var1);
		if (var2.type == bv_type_float)
			y = bv_variable_get_float(var2);
		else
			y = bv_variable_get_int(var2);

		out = (x < y);
	}
	else
		out = bv_variable_get_uint(var1) < bv_variable_get_uint(var2);

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create_uchar(out));
}
void bv_execute_greater_equal(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);
	u8 out = 0;

	if (var1.type == bv_type_float || var2.type == bv_type_float) {
		float x = 0, y = 0;

		if (var1.type == bv_type_float)
			x = bv_variable_get_float(var1);
		else
			x = bv_variable_get_int(var1);
		if (var2.type == bv_type_float)
			y = bv_variable_get_float(var2);
		else
			y = bv_variable_get_int(var2);

		out = (x >= y);
	}
	else
		out = bv_variable_get_uint(var1) >= bv_variable_get_uint(var2);

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create_uchar(out));
}
void bv_execute_less_equal(bv_state* state) {
	if (state->stack->length < 2) // dont do anything if there is not enough arguments in stack
		return;

	bv_variable var1 = bv_stack_penultimate(state->stack);
	bv_variable var2 = bv_stack_top(state->stack);
	u8 out = 0;

	if (var1.type == bv_type_float || var2.type == bv_type_float) {
		float x = 0, y = 0;

		if (var1.type == bv_type_float)
			x = bv_variable_get_float(var1);
		else
			x = bv_variable_get_int(var1);
		if (var2.type == bv_type_float)
			y = bv_variable_get_float(var2);
		else
			y = bv_variable_get_int(var2);

		out = (x <= y);
	}
	else
		out = bv_variable_get_uint(var1) <= bv_variable_get_uint(var2);

	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_create_uchar(out));
}
void bv_execute_nop(bv_state* state){}
void bv_execute_convert(bv_state* state) {
	bv_variable var = bv_stack_top(state->stack);
	bv_type new_type = bv_type_read(state->code);
	bv_type old_type = var.type;

	var.type = new_type;

	if (old_type == bv_type_string || new_type == bv_type_string || old_type == new_type) {}// convert doesnt support strings
	else if (old_type == bv_type_float) {
		u32 value = (u32)bv_variable_get_float(var);
		bv_variable_deinitialize(&var);
		var = bv_variable_create(new_type, value);
	}
	else {
		u32 value = bv_variable_get_uint(var);

		if (new_type == bv_type_float)
			var = bv_variable_create_float(value);
		else
			var = bv_variable_create(new_type, value);
	}

	bv_stack_pop(state->stack);
	bv_stack_push(state->stack, var);
}
void bv_execute_duplicate(bv_state* state) {
	bv_variable var = bv_variable_copy(bv_stack_top(state->stack));
	bv_stack_push(state->stack, var);
}
void bv_execute_swap(bv_state* state) {
	bv_variable var1 = bv_variable_copy(bv_stack_penultimate(state->stack));
	bv_variable var2 = bv_variable_copy(bv_stack_top(state->stack));
	bv_stack_pop(state->stack);
	bv_stack_pop(state->stack);
	bv_stack_push(state->stack, var2);
	bv_stack_push(state->stack, var1);
}
void bv_execute_get_local(bv_state* state) {
	u16 index = u16_read(state->code);

	if (index >= state->locals->length)
		bv_stack_push(state->stack, bv_variable_create_int(0)); // push a 0 to the stack
	else {
		bv_variable* pLocal = &state->locals->data[index];
		if (pLocal->type == bv_type_array) {
			bv_array arr = bv_variable_get_array(*pLocal);

			int* lens = malloc(sizeof(int) * arr.dim);
			int i = arr.dim;
			while (i != 0) {
				lens[arr.dim - i] = bv_variable_get_int(bv_stack_top(state->stack));
				bv_stack_pop(state->stack);
				i--;
			}

			bv_stack_push(state->stack, bv_array_get(arr, lens));

			free(lens);
		}
		else
			bv_stack_push(state->stack, bv_variable_copy(*pLocal)); // make a copy (no pointers :()
	}
}
void bv_execute_set_local(bv_state* state) {
	u16 index = u16_read(state->code);
	bv_variable var = bv_variable_copy(bv_stack_top(state->stack));
	bv_stack_pop(state->stack);

	if (index == state->locals->length) { // 'declare' a new variable
		bv_stack_push(state->locals, var);
	}
	else {
		bv_variable* pLocal = &state->locals->data[index];

		bv_type my_type = pLocal->type;
		if (my_type == bv_type_array) {
			bv_array arr = bv_variable_get_array(*pLocal);

			int* lens = malloc(sizeof(int) * arr.dim);
			int i = arr.dim;
			while (i != 0) {
				lens[arr.dim - i] = bv_variable_get_int(bv_stack_top(state->stack));
				bv_stack_pop(state->stack);
				i--;
			}

			bv_variable_deinitialize(&arr.data[bv_array_get_index(arr, lens)]);
			bv_array_set(arr, lens, var);

			free(lens);
		}
		else {
			bv_type st_type = var.type;

			if (st_type == bv_type_string && my_type != bv_type_string)
				return; // cant assign string to an int/float

			if (my_type == bv_type_string) {
				if (st_type != bv_type_string) // cant assign int/float to a string
					return;

				*pLocal = bv_variable_copy(var);
			}
			else if (my_type == bv_type_float) {
				if (st_type == bv_type_float) {
					*pLocal = bv_variable_copy(var);
				}
				else {
					if (st_type == bv_type_uint || st_type == bv_type_ushort || st_type == bv_type_uchar)
						*pLocal = bv_variable_create_float(bv_variable_get_uint(var));
					else
						*pLocal = bv_variable_create_float(bv_variable_get_int(var));

					pLocal->type = my_type; // return the old type
				}
			}
			else {
				if (st_type == bv_type_float) {
					u32 newVal = bv_variable_get_float(var);
					*pLocal = bv_variable_create(my_type, newVal);
				}
				else pLocal->value = var.value;
			}
		}
	}
}
void bv_execute_get_global(bv_state* state) {
	u16 index = u16_read(state->code);

	if (index >= state->prog->globals.length)
		bv_stack_push(state->stack, bv_variable_create_int(0)); // push a 0 to the stack
	else
		bv_stack_push(state->stack, bv_variable_copy(state->prog->globals.data[index])); // make a copy ( no pointers :( )
}
void bv_execute_set_global(bv_state* state) {
	u16 index = u16_read(state->code);

	bv_variable var = bv_stack_top(state->stack);
	bv_stack_pop(state->stack);

	bv_variable* pLocal = &state->prog->globals.data[index];

	bv_type my_type = pLocal->type;
	bv_type st_type = var.type;

	if (st_type == bv_type_string && my_type != bv_type_string)
		return; // cant assign string to an int/float

	if (my_type == bv_type_string) {
		if (st_type != bv_type_string) // cant assign int/float to a string
			return;

		*pLocal = bv_variable_copy(var);
	}
	else if (my_type == bv_type_float) {
		if (st_type == bv_type_float) {
			*pLocal = bv_variable_copy(var);
		}
		else {
			if (st_type == bv_type_uint || st_type == bv_type_ushort || st_type == bv_type_uchar)
				*pLocal = bv_variable_create_float(bv_variable_get_uint(var));
			else
				*pLocal = bv_variable_create_float(bv_variable_get_int(var));

			pLocal->type = my_type; // return the old type
		}
	}
	else {
		if (st_type == bv_type_float) {
			u32 newVal = bv_variable_get_float(var);
			*pLocal = bv_variable_create(my_type, newVal);
		}
		else pLocal->value = var.value;
	}
}
void bv_execute_new_array(bv_state* state) {
	int dim = u8_read(state->code);

	if (state->stack->length < dim)
		return;

	int* lens = malloc(sizeof(int) * dim);
	int i = dim;
	while (i != 0) {
		lens[dim - i] = bv_variable_get_int(bv_stack_top(state->stack));
		bv_stack_pop(state->stack);
		i--;
	}

	bv_stack_push(state->locals, bv_variable_create_array(bv_array_create(dim, lens)));
	free(lens);
}
void bv_execute_call(bv_state* state) {
	char* name = string_read(state->code);
	u8 argc = u8_read(state->code);

	bv_function* func = bv_program_get_function(state->prog, name);
	bv_stack func_args = bv_stack_create();

	if (state->stack->length < argc)
		return; // [TODO] error, not enough arguments

	for (int i = 0; i < argc; i++) {
		bv_stack_push(&func_args, bv_variable_copy(bv_stack_top(state->stack)));
		bv_stack_pop(state->stack);
	}

	if (func != NULL) {
		bv_program_call(state->prog, func, &func_args, 0);
	}
	else {
		bv_external_function ext_func = bv_program_get_ext_function(state->prog, name);
		(*ext_func)(argc, func_args.data);
	}

	// bv_stack_delete(&func_args); ?
}
void bv_execute_call_return(bv_state* state) {
	char* name = string_read(state->code);
	u8 argc = u8_read(state->code);

	bv_function* func = bv_program_get_function(state->prog, name);
	bv_stack func_args = bv_stack_create();

	if (state->stack->length < argc)
		return; // [TODO] error, not enough arguments

	for (int i = 0; i < argc; i++) {
		bv_stack_push(&func_args, bv_stack_top(state->stack));
		bv_stack_pop(state->stack);
	}

	if (func != NULL)
		bv_stack_push(state->stack, bv_program_call(state->prog, func, &func_args, 0));
	else {
		bv_external_function ext_func = bv_program_get_ext_function(state->prog, name);
		bv_stack_push(state->stack, (*ext_func)(argc, func_args.data));
	}

	// bv_stack_delete(&func_args); ?
}
void bv_execute_is_type_of(bv_state* state) {
	bv_type type = bv_type_read(state->code);

	bv_variable var = bv_stack_top(state->stack);
	bv_stack_pop(state->stack);

	if (var.type == type)
		bv_stack_push(state->stack, bv_variable_create_char(1));
	else
		bv_stack_push(state->stack, bv_variable_create_char(0));
}
void bv_execute_if(bv_state* state) {
	u32 addr = u32_read(state->code);

	bv_variable var = bv_stack_top(state->stack);
	bv_stack_pop(state->stack);

	if (bv_type_is_integer(var.type))
		if (bv_variable_get_int(var) == 0)
			*state->code = state->this_func->code + addr; // WARNING! THIS MIGHT NOT WORK (SAME FOR GOTO)
}
void bv_execute_goto(bv_state* state) {
	u32 addr = u32_read(state->code);
	*state->code = state->this_func->code + addr;
}
void bv_execute_new_object(bv_state* state) {
	u16 id = u16_read(state->code);
	u8 argc = u8_read(state->code);

	bv_object_info* info = state->prog->block->objects->info[id];
	bv_variable var = bv_variable_create_object(info);
	bv_object* obj = bv_variable_get_object(var);

	bv_stack func_args = bv_stack_create();
	if (state->stack->length >= argc)
		for (int i = 0; i < argc; i++) {
			bv_stack_push(&func_args, bv_variable_copy(bv_stack_top(state->stack)));
			bv_stack_pop(state->stack);
		}


	bv_function* constructor = bv_object_get_method(obj, info->name); // get constructor
	bv_program_call(state->prog, constructor, &func_args, obj); // call constructor

	bv_stack_push(state->stack, var);
}
void bv_execute_set_prop(bv_state* state) {
	string name = string_read(state->code);

	bv_variable var = bv_variable_copy(bv_stack_top(state->stack));
	bv_stack_pop(state->stack);

	bv_object_set_property(bv_variable_get_object(var), name, bv_stack_top(state->stack));
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, var);
}
void bv_execute_set_my_prop(bv_state* state) {
	string name = string_read(state->code);

	if (state->obj == 0)
		return;

	bv_object_set_property(state->obj, name, bv_stack_top(state->stack));
	bv_stack_pop(state->stack);
}
void bv_execute_get_prop(bv_state* state) {
	string name = string_read(state->code);

	bv_variable var = bv_variable_copy(bv_stack_top(state->stack));
	bv_object* top = bv_variable_get_object(var);
	bv_stack_pop(state->stack);

	bv_stack_push(state->stack, bv_variable_copy(*bv_object_get_property(top, name)));
	bv_variable_deinitialize(&var);
}
void bv_execute_get_my_prop(bv_state* state) {
	string name = string_read(state->code);

	if (state->obj == 0)
		return;

	bv_stack_push(state->stack, bv_variable_copy(*bv_object_get_property(state->obj, name)));
}
void bv_execute_call_method(bv_state* state) {
	string name = string_read(state->code);
	u8 argc = u8_read(state->code);

	bv_variable var = bv_variable_copy(bv_stack_top(state->stack));
	bv_stack_pop(state->stack);

	bv_object* obj = bv_variable_get_object(var);

	bv_function* func = bv_object_get_method(obj, name);
	bv_stack func_args = bv_stack_create();

	if (state->stack->length < argc)
		return; // [TODO] error, not enough arguments

	for (int i = 0; i < argc; i++) {
		bv_stack_push(&func_args, bv_variable_copy(bv_stack_top(state->stack)));
		bv_stack_pop(state->stack);
	}

	if (func != NULL)
		bv_program_call(state->prog, func, &func_args, obj);
	else {
		// [TODO] external functions for objects
	}

	bv_stack_push(state->stack, var); // [TODO] pointers plzz :(
}
void bv_execute_call_my_method(bv_state* state) {
	string name = string_read(state->code);
	u8 argc = u8_read(state->code);

	bv_object* obj = state->obj;

	bv_function* func = bv_object_get_method(obj, name);
	bv_stack func_args = bv_stack_create();

	if (state->stack->length < argc)
		return; // [TODO] error, not enough arguments

	for (int i = 0; i < argc; i++) {
		bv_stack_push(&func_args, bv_variable_copy(bv_stack_top(state->stack)));
		bv_stack_pop(state->stack);
	}

	if (func != NULL) {
		bv_program_call(state->prog, func, &func_args, obj);
	}
	else {
		// [TODO] external functions for objects
	}
}
void bv_execute_call_ret_method(bv_state* state) {
	string name = string_read(state->code);
	u8 argc = u8_read(state->code);

	bv_variable var = bv_variable_copy(bv_stack_top(state->stack));
	bv_stack_pop(state->stack);

	bv_object* obj = bv_variable_get_object(var);

	bv_function* func = bv_object_get_method(obj, name);
	bv_stack func_args = bv_stack_create();

	if (state->stack->length < argc)
		return; // [TODO] error, not enough arguments

	for (u8 i = 0; i < argc; i++) {
		bv_stack_push(&func_args, bv_variable_copy(bv_stack_top(state->stack)));
		bv_stack_pop(state->stack);
	}

	if (func != NULL) {
		bv_stack_push(state->stack, bv_program_call(state->prog, func, &func_args, obj));
	}
	else {
		// [TODO] external functions for objects
	}
	
	bv_stack_push(state->stack, var); // [TODO] pointers plzz :(
}
void bv_execute_call_ret_my_method(bv_state* state) {
	string name = string_read(state->code);
	u8 argc = u8_read(state->code);

	bv_object* obj = state->obj;

	bv_function* func = bv_object_get_method(obj, name);
	bv_stack func_args = bv_stack_create();

	if (state->stack->length < argc)
		return; // [TODO] error, not enough arguments

	for (int i = 0; i < argc; i++) {
		bv_stack_push(&func_args, bv_variable_copy(bv_stack_top(state->stack)));
		bv_stack_pop(state->stack);
	}

	if (func != NULL) {
		bv_stack_push(state->stack, bv_program_call(state->prog, func, &func_args, obj));
	}
	else {
		// [TODO] external functions for objects
	}
}