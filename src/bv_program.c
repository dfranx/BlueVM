#include <BlueVM/bv_program.h>
#include <BlueVM/bv_object.h>
#include <BlueVM/bv_array.h>
#include <string.h>
#include <stdlib.h>

bv_program* bv_program_create(byte* mem)
{
	bv_program* ret = malloc(sizeof(bv_program));

	byte* original_mem = mem;

	ret->header = bv_header_create(&mem);
	ret->global_names = bv_name_list_create(&mem);
	ret->globals = bv_stack_create();
	ret->block = bv_block_create(&mem);
	ret->functions = bv_function_create_array(ret->block->functions, original_mem);

	ret->external_functions = NULL;
	ret->external_function_names = NULL;
	ret->external_function_count = 0;

	ret->globals.length = ret->global_names.name_count;
	ret->globals.data = malloc(sizeof(bv_variable) * ret->globals.length);
	for (u16 i = 0; i < ret->globals.length; i++)
		ret->globals.data[i] = bv_variable_create_void();

	return ret;
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

bv_variable bv_program_call(bv_program* prog, bv_function* func, bv_stack* args)
{
	bv_stack stack = bv_stack_create();
	bv_stack locals = bv_stack_create(); // local variable container
	bv_variable rtrn;

	if (args != NULL && args->length == func->args) {
		// push arguments to local variables
		for (int i = 0; i < args->length; i++)
			bv_stack_push(&locals, args->data[i]);
	}

	bv_constant_pool* cpool = prog->block->constants;
	byte* code = func->code;

	while ((code - func->code) < func->code_length) {
		bv_opcode op = bv_opcode_read(&code);

		if (op == bv_opcode_return) break;
		else if (op == bv_opcode_push_stack) {
			bv_type type = bv_type_read(&code);
			bv_stack_push(&stack, bv_variable_read(&code, type));
		}
		else if (op == bv_opcode_pop_stack) bv_stack_pop(&stack);
		else if (op == bv_opcode_const_get) {
			bv_type type = bv_type_read(&code);
			u16 index = u16_read(&code);

			for (u8 i = 0; i < cpool->type_count; i++)
				if (cpool->val_type[i] == type) {
					if (index < cpool->val_count[i])
						bv_stack_push(&stack, bv_variable_copy(cpool->val[i][index]));
					break;
				}
		}
		else if (op == bv_opcode_add) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;
			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);

			if (var1.type == bv_type_float || var2.type == bv_type_float) {
				float sum = 0;

				if (var1.type == bv_type_float)
					sum += bv_variable_get_float(var1);
				else sum += bv_variable_get_int(var1);
				if (var2.type == bv_type_float)
					sum += bv_variable_get_float(var2);
				else sum += bv_variable_get_int(var2);

				bv_stack_pop(&stack);
				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create_float(sum));
			}
			else if (var1.type == bv_type_string && var2.type == bv_type_string) {
				string result = malloc(strlen(var1.value) + strlen(var2.value) + 1);
				strcpy(result, var1.value);
				strcpy(result + strlen(var1.value), var2.value);
				result[strlen(var1.value) + strlen(var2.value)] = 0;

				bv_stack_pop(&stack);
				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create_string(result));
				free(result);
			}
			else {
				u32 sum = bv_variable_get_int(var1) + bv_variable_get_int(var2);
				bv_type type = var1.type;

				bv_stack_pop(&stack);
				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create(type, sum));
			}
		}
		else if (op == bv_opcode_subtract) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);

			if (var1.type == bv_type_float || var2.type == bv_type_float) {
				float sum = 0;

				if (var1.type == bv_type_float)
					sum = bv_variable_get_float(var1);
				else sum = bv_variable_get_int(var1);
				if (var2.type == bv_type_float)
					sum -= bv_variable_get_float(var2);
				else sum -= bv_variable_get_int(var2);

				bv_stack_pop(&stack);
				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create_float(sum));
			}
			else {
				u32 sum = bv_variable_get_int(var1) - bv_variable_get_int(var2);
				bv_type type = var1.type;

				bv_stack_pop(&stack);
				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create(type, sum));
			}
		}
		else if (op == bv_opcode_multiply) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);

			if (var1.type == bv_type_float || var2.type == bv_type_float) {
				float sum = 0;

				if (var1.type == bv_type_float)
					sum = bv_variable_get_float(var1);
				else sum = bv_variable_get_int(var1);
				if (var2.type == bv_type_float)
					sum *= bv_variable_get_float(var2);
				else sum *= bv_variable_get_int(var2);

				bv_stack_pop(&stack);
				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create_float(sum));
			}
			else {
				u32 sum = bv_variable_get_int(var1) * bv_variable_get_int(var2);
				bv_type type = var1.type;

				bv_stack_pop(&stack);
				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create(type, sum));
			}
		}
		else if (op == bv_opcode_divide) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);

			if (var1.type == bv_type_float || var2.type == bv_type_float) {
				float sum = 0;

				if (var1.type == bv_type_float)
					sum = bv_variable_get_float(var1);
				else sum = bv_variable_get_int(var1);
				if (var2.type == bv_type_float)
					sum /= bv_variable_get_float(var2); // [TODO] check if == 0
				else sum /= bv_variable_get_int(var2);

				bv_stack_pop(&stack);
				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create_float(sum));
			}
			else {
				u32 sum = bv_variable_get_int(var1) / bv_variable_get_int(var2);
				bv_type type = var1.type;

				bv_stack_pop(&stack);
				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create(type, sum));
			}
		}
		else if (op == bv_opcode_increment) {
			bv_variable var = bv_stack_top(&stack);

			if (var.type == bv_type_float) {
				float val = bv_variable_get_float(var) + 1;

				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create_float(val));
			}
			else {
				u32 val = bv_variable_get_int(var) + 1;
				bv_type type = var.type;

				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create(type, val));
			}
		}
		else if (op == bv_opcode_decrement) {
			bv_variable var = bv_stack_top(&stack);

			if (var.type == bv_type_float) {
				float val = bv_variable_get_float(var) - 1;

				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create_float(val));
			}
			else {
				u32 val = bv_variable_get_int(var) - 1;
				bv_type type = var.type;

				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create(type, val));
			}
		}
		else if (op == bv_opcode_negate) {
			bv_variable var = bv_stack_top(&stack);

			if (var.type == bv_type_float) {
				float val = -bv_variable_get_float(var);

				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create_float(val));
			}
			else {
				u32 val = -bv_variable_get_int(var);
				bv_type type = var.type;

				bv_stack_pop(&stack);

				bv_stack_push(&stack, bv_variable_create(type, val));
			}
		}
		else if (op == bv_opcode_modulo) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);
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

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create_uint(val1 % val2));
		}
		else if (op == bv_opcode_bit_or) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);

			u32 res = bv_variable_get_uint(var1) | bv_variable_get_uint(var2);
			bv_type type = var1.type;

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create(type, res));
		}
		else if (op == bv_opcode_bit_and) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);

			u32 res = bv_variable_get_uint(var1) & bv_variable_get_uint(var2);
			bv_type type = var1.type;

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create(type, res));
		}
		else if (op == bv_opcode_bit_not) {
			bv_variable var = bv_stack_top(&stack);

			u32 res = ~bv_variable_get_uint(var);
			bv_type type = var.type;

			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create(type, res));
		}
		else if (op == bv_opcode_bit_xor) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);

			u32 res = bv_variable_get_uint(var1) ^ bv_variable_get_uint(var2);
			bv_type type = var1.type;

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create(type, res));
		}
		else if (op == bv_opcode_bit_lshift) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);

			u32 res = bv_variable_get_uint(var1) << bv_variable_get_uint(var2);
			bv_type type = var1.type;

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create(type, res));
		}
		else if (op == bv_opcode_bit_rshift) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);

			u32 res = bv_variable_get_uint(var1) >> bv_variable_get_uint(var2);
			bv_type type = var1.type;

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create(type, res));
		}
		else if (op == bv_opcode_equal) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);
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

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create_uchar(out));
		}
		else if (op == bv_opcode_not_equal) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);
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

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create_uchar(out));
		}
		else if (op == bv_opcode_not) {
			bv_variable var = bv_stack_top(&stack);
			u8 out = !bv_variable_get_uchar(var);

			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create_uchar(out));
		}
		else if (op == bv_opcode_greater) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);
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

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create_uchar(out));
		}
		else if (op == bv_opcode_less) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);
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

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create_uchar(out));
		}
		else if (op == bv_opcode_greater_equal) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);
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

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create_uchar(out));
		}
		else if (op == bv_opcode_less_equal) {
			if (stack.length < 2) // dont do anything if there is not enough arguments in stack
				continue;

			bv_variable var1 = bv_stack_penultimate(&stack);
			bv_variable var2 = bv_stack_top(&stack);
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

			bv_stack_pop(&stack);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_create_uchar(out));
		}
		else if (op == bv_opcode_nop) continue;
		else if (op == bv_opcode_convert) {
			bv_variable var = bv_stack_top(&stack);
			bv_type new_type = bv_type_read(&code);
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

			bv_stack_pop(&stack);
			bv_stack_push(&stack, var);
		}
		else if (op == bv_opcode_duplicate) {
			bv_variable var = bv_variable_copy(bv_stack_top(&stack));
			bv_stack_push(&stack, var);
		}
		else if (op == bv_opcode_swap) {
			bv_variable var1 = bv_variable_copy(bv_stack_penultimate(&stack));
			bv_variable var2 = bv_variable_copy(bv_stack_top(&stack));
			bv_stack_pop(&stack);
			bv_stack_pop(&stack);
			bv_stack_push(&stack, var2);
			bv_stack_push(&stack, var1);
		}
		else if (op == bv_opcode_get_local) {
			u16 index = u16_read(&code);

			if (index >= locals.length)
				bv_stack_push(&stack, bv_variable_create_int(0)); // push a 0 to the stack
			else {
				bv_variable* pLocal = &locals.data[index];
				if (pLocal->type == bv_type_array) {
					bv_array arr = bv_variable_get_array(*pLocal);

					int* lens = malloc(sizeof(int) * arr.dim);
					int i = arr.dim;
					while (i != 0) {
						lens[arr.dim - i] = bv_variable_get_int(bv_stack_top(&stack));
						bv_stack_pop(&stack);
						i--;
					}

					bv_stack_push(&stack, bv_array_get(arr, lens));

					free(lens);
				}
				else
					bv_stack_push(&stack, bv_variable_copy(*pLocal)); // make a copy (no pointers :()
			}
		}
		else if (op == bv_opcode_set_local) {
			u16 index = u16_read(&code);
			bv_variable var = bv_variable_copy(bv_stack_top(&stack));
			bv_stack_pop(&stack);

			if (index == locals.length) { // 'declare' a new variable
				bv_stack_push(&locals, var);
			}
			else {
				bv_variable* pLocal = &locals.data[index];

				bv_type my_type = pLocal->type;
				if (my_type == bv_type_array) {
					bv_array arr = bv_variable_get_array(*pLocal);

					int* lens = malloc(sizeof(int) * arr.dim);
					int i = arr.dim;
					while (i != 0) {
						lens[arr.dim - i] = bv_variable_get_int(bv_stack_top(&stack));
						bv_stack_pop(&stack);
						i--;
					}

					bv_variable_deinitialize(&arr.data[bv_array_get_index(arr, lens)]);
					bv_array_set(arr, lens, var);

					free(lens);
				}
				else {
					bv_type st_type = var.type;

					if (st_type == bv_type_string && my_type != bv_type_string)
						continue; // cant assign string to an int/float

					if (my_type == bv_type_string) {
						if (st_type != bv_type_string) // cant assign int/float to a string
							continue;

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
		else if (op == bv_opcode_get_global) {
			u16 index = u16_read(&code);

			if (index >= prog->globals.length)
				bv_stack_push(&stack, bv_variable_create_int(0)); // push a 0 to the stack
			else
				bv_stack_push(&stack, bv_variable_copy(prog->globals.data[index])); // make a copy ( no pointers :( )
		}
		else if (op == bv_opcode_set_global) {
			u16 index = u16_read(&code);

			bv_variable var = bv_stack_top(&stack);
			bv_stack_pop(&stack);

			bv_variable* pLocal = &prog->globals.data[index];

			bv_type my_type = pLocal->type;
			bv_type st_type = var.type;

			if (st_type == bv_type_string && my_type != bv_type_string)
				continue; // cant assign string to an int/float

			if (my_type == bv_type_string) {
				if (st_type != bv_type_string) // cant assign int/float to a string
					continue;

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
		else if (op == bv_opcode_new_array) {
			int dim = u8_read(&code);

			if (stack.length < dim)
				continue;

			int* lens = malloc(sizeof(int) * dim);
			int i = dim;
			while (i != 0) {
				lens[dim - i] = bv_variable_get_int(bv_stack_top(&stack));
				bv_stack_pop(&stack);
				i--;
			}

			bv_stack_push(&locals, bv_variable_create_array(bv_array_create(dim, lens)));
			free(lens);
		}
		else if (op == bv_opcode_call)
		{
			char* name = string_read(&code);
			u8 argc = u8_read(&code);

			bv_function* func = bv_program_get_function(prog, name);
			bv_stack func_args = bv_stack_create();

			if (stack.length < argc)
				continue; // [TODO] error, not enough arguments
			
			for (int i = 0; i < argc; i++) {
				bv_stack_push(&func_args, bv_variable_copy(bv_stack_top(&stack)));
				bv_stack_pop(&stack);
			}

			if (func != NULL) {
				bv_program_call(prog, func, &func_args);
			} else {
				bv_external_function ext_func = bv_program_get_ext_function(prog, name);
				(*ext_func)(argc, func_args.data);
			}

			// bv_stack_delete(&func_args); ?
		}
		else if (op == bv_opcode_call_return) {
			char* name = string_read(&code);
			u8 argc = u8_read(&code);

			bv_function* func = bv_program_get_function(prog, name);
			bv_stack func_args = bv_stack_create();

			if (stack.length < argc)
				continue; // [TODO] error, not enough arguments

			for (int i = 0; i < argc; i++) {
				bv_stack_push(&func_args, bv_stack_top(&stack));
				bv_stack_pop(&stack);
			}

			if (func != NULL)
				bv_stack_push(&stack, bv_program_call(prog, func, &func_args));
			else {
				bv_external_function ext_func = bv_program_get_ext_function(prog, name);
				bv_stack_push(&stack, (*ext_func)(argc, func_args.data));
			}

			// bv_stack_delete(&func_args); ?
		}
		else if (op == bv_opcode_is_type_of) {
			bv_type type = bv_type_read(&code);

			bv_variable var = bv_stack_top(&stack);
			bv_stack_pop(&stack);
		
			if (var.type == type)
				bv_stack_push(&stack, bv_variable_create_char(1));
			else
				bv_stack_push(&stack, bv_variable_create_char(0));
		} 
		else if (op == bv_opcode_if) {
			u32 addr = u32_read(&code);

			bv_variable var = bv_stack_top(&stack);
			bv_stack_pop(&stack);

			if (bv_type_is_integer(var.type))
				if (bv_variable_get_int(var) == 0)
					code = func->code + addr;
		}
		else if (op == bv_opcode_goto) {
			u32 addr = u32_read(&code);
			code = func->code + addr;
		}
		else if (op == bv_opcode_new_object) {
			u16 id = u16_read(&code);
			bv_stack_push(&stack, bv_variable_create_object(prog->block->objects->info[id]));
		}
		else if (op == bv_opcode_get_prop) {
			string name = string_read(&code);
			
			bv_variable var = bv_variable_copy(bv_stack_top(&stack));
			bv_object* top = bv_variable_get_object(var);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, bv_variable_copy(*bv_object_get_property(top, name)));
			bv_variable_deinitialize(&var);
		}
		else if (op == bv_opcode_set_prop) {
			string name = string_read(&code);

			bv_variable var = bv_variable_copy(bv_stack_top(&stack));
			bv_stack_pop(&stack);
			
			bv_variable* prop = bv_object_get_property(bv_variable_get_object(var), name);

			bv_variable newval = bv_variable_copy(bv_stack_top(&stack));
			*prop = bv_variable_copy(newval);
			bv_stack_pop(&stack);

			bv_stack_push(&stack, var);
		}
	}

	// get return value
	if (stack.length > 0)
		rtrn = bv_variable_copy(bv_stack_top(&stack)); // on return, make a copy of the return value
	else
		rtrn = bv_variable_create_int(0);

	bv_stack_delete(&stack);
	bv_stack_delete(&locals);

	return rtrn;
}
