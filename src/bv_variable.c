#include <BlueVM/bv_variable.h>
#include <BlueVM/bv_array.h>
#include <BlueVM/bv_object.h>
#include <BlueVM/bv_object_info.h>
#include <BlueVM/bv_program.h>
#include <BlueVM/bv_function.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

s32 bv_variable_get_int(bv_variable var)
{
	return (intptr_t)var.value;
}
u32 bv_variable_get_uint(bv_variable var)
{
	return (intptr_t)var.value;
}
s16 bv_variable_get_short(bv_variable var)
{
	return (intptr_t)var.value;
}
u16 bv_variable_get_ushort(bv_variable var)
{
	return (intptr_t)var.value;
}
s8 bv_variable_get_char(bv_variable var)
{
	return (intptr_t)var.value;
}
u8 bv_variable_get_uchar(bv_variable var)
{
	return (intptr_t)var.value;
}
float bv_variable_get_float(bv_variable var)
{
	return *((float*)var.value);
}
bv_string bv_variable_get_string(bv_variable var)
{
	return (bv_string)var.value;
}
bv_array bv_variable_get_array(bv_variable var)
{
	return *((bv_array*)var.value);
}
bv_object* bv_variable_get_object(bv_variable var)
{
	return (bv_object*)var.value;
}
bv_variable* bv_variable_get_pointer(bv_variable var)
{
	return (bv_variable*)var.value;
}

bv_function* bv_variable_get_function(bv_variable var)
{
	return (bv_function*)var.value;
}


bv_variable bv_variable_create(bv_type type, void* value)
{
	bv_variable ret;
	ret.type = type;
	ret.value = value;
	return ret;
}

bv_variable bv_variable_create_void()
{
	bv_variable ret;
	ret.type = bv_type_void;
	ret.value = NULL;
	return ret;
}

bv_variable bv_variable_create_int(s32 var)
{
	bv_variable ret;
	ret.type = bv_type_int;
	ret.value = (void*)var;
	return ret;
}
bv_variable bv_variable_create_uint(u32 var)
{
	bv_variable ret;
	ret.type = bv_type_uint;
	ret.value = (void*)var;
	return ret;
}
bv_variable bv_variable_create_short(s16 var)
{
	bv_variable ret;
	ret.type = bv_type_short;
	ret.value = (void*)var;
	return ret;
}
bv_variable bv_variable_create_ushort(u16 var)
{
	bv_variable ret;
	ret.type = bv_type_ushort;
	ret.value = (void*)var;
	return ret;
}
bv_variable bv_variable_create_char(s8 var)
{
	bv_variable ret;
	ret.type = bv_type_char;
	ret.value = (void*)var;
	return ret;
}
bv_variable bv_variable_create_uchar(u8 var)
{
	bv_variable ret;
	ret.type = bv_type_uchar;
	ret.value = (void*)var;
	return ret;
}
bv_variable bv_variable_create_float(float var)
{
	bv_variable ret;
	ret.type = bv_type_float;
	ret.value = malloc(sizeof(float));
	*((float*)ret.value) = var;
	return ret;
}
bv_variable bv_variable_create_string(const bv_string var)
{
	bv_variable ret;
	ret.type = bv_type_string;

	s32 len = strlen(var);
	ret.value = malloc((len + 1) * sizeof(char));
	memcpy(ret.value, var, len);
	((bv_string)ret.value)[len] = 0;
	
	return ret;
}
bv_variable bv_variable_create_array(bv_array var)
{
	bv_variable ret;
	ret.type = bv_type_array;
	ret.value = malloc(sizeof(bv_array));
	*((bv_array*)ret.value) = var;
	return ret;
}
bv_variable bv_variable_create_object(bv_object_info* var)
{
	bv_variable ret;
	ret.type = bv_type_object;
	ret.value = bv_object_create(var);
	return ret;
}
bv_variable bv_variable_create_null_object()
{
	bv_variable ret;
	ret.type = bv_type_object;
	ret.value = 0;
	return ret;
}
bv_variable bv_variable_create_pointer(bv_variable* var)
{
	bv_variable ret;
	ret.type = bv_type_pointer;
	ret.value = var;
	return ret;
}

bv_variable bv_variable_create_function(bv_function* func)
{
	bv_variable ret;
	ret.type = bv_type_function;
	ret.value = func;
	return ret;
}

void bv_variable_set_int(bv_variable * var, s32 val)
{
	if (var->type != bv_type_int)
		return;
	var->value = (void*)val;
}
void bv_variable_set_uint(bv_variable * var, u32 val)
{
	if (var->type != bv_type_uint)
		return;
	var->value = (void*)val;
}
void bv_variable_set_short(bv_variable * var, s16 val)
{
	if (var->type != bv_type_short)
		return;
	var->value = (void*)val;
}
void bv_variable_set_ushort(bv_variable * var, u16 val)
{
	if (var->type != bv_type_ushort)
		return;
	var->value = (void*)val;
}
void bv_variable_set_char(bv_variable * var, s8 val)
{
	if (var->type != bv_type_char)
		return;
	var->value = (void*)val;
}
void bv_variable_set_uchar(bv_variable * var, u8 val)
{
	if (var->type != bv_type_uchar)
		return;
	var->value = (void*)val;
}
void bv_variable_set_float(bv_variable * var, float val)
{
	if (var->type != bv_type_float)
		return;
	*((float*)var->value) = val;
}
void bv_variable_set_string(bv_variable * var, const bv_string val)
{
	if (var->type != bv_type_string)
		return;

	free(var->value);


	s32 len = strlen(val);

	var->value = malloc((len + 1) * sizeof(char));
	memcpy(var->value, val, len);

	((bv_string)var->value)[len] = 0;
}
void bv_variable_set_array(bv_variable * var, bv_array val)
{
	if (var->type != bv_type_array)
		return;
	*((bv_array*)var->value) = val;
}
void bv_variable_set_object(bv_variable* var, bv_object* val)
{
	if (var->type != bv_type_object)
		return;
	*((bv_object*)var->value) = *val;
}
void bv_variable_set_pointer(bv_variable* var, bv_variable* val)
{
	var->type = bv_type_pointer;
	var->value = val;
}

void bv_variable_set_function(bv_variable* var, bv_function* val)
{
	if (var->type != bv_type_function)
		return;
	var->type = bv_type_function;
	var->value = val;
}

void bv_variable_deinitialize(bv_variable * var)
{
	// this is sort of deconstructor
	if (var->type == bv_type_float || var->type == bv_type_string)
		free(var->value);
	else if (var->type == bv_type_array) {
		bv_array_deinitialize((bv_array*)var->value);
	} else if (var->type == bv_type_object)
		bv_object_deinitialize((bv_object*)var->value);
}
bv_variable bv_variable_copy(bv_variable var)
{
	bv_variable ret;
	ret.type = var.type;

	if (var.type == bv_type_float) {
		ret.value = malloc(sizeof(float));
		memcpy(ret.value, var.value, sizeof(float));
	} else if (var.type == bv_type_string) {
		s32 len = strlen((bv_string)var.value);
		ret.value = malloc((len + 1) * sizeof(char));
		memcpy(ret.value, var.value, len);
		((bv_string)ret.value)[len] = 0;
	} else if (var.type == bv_type_array) {
		bv_array from = bv_variable_get_array(var);
		bv_array cpy = bv_array_create(from.dim, from.length);

		u32 cnt = bv_array_get_range(from);
		for (u32 i = 0; i < cnt; i++)
			cpy.data[i] = bv_variable_copy(from.data[i]);

		ret = bv_variable_create_array(cpy);
	}
	else if (var.type == bv_type_object) {
		if (var.value == 0)
			ret.value = 0;
		else {
			bv_object* cobj = (bv_object*)var.value;
			bv_object* nobj = bv_object_create(cobj->type);
			for (u16 i = 0; i < nobj->type->props.name_count; i++)
				nobj->prop[i] = bv_variable_copy(cobj->prop[i]);
			ret.value = nobj;			
		}
	} else
		ret.value = var.value;

	return ret;
}
bv_variable bv_variable_read(byte** mem, bv_type type)
{
	switch (type) {
		case bv_type_uint: return bv_variable_create_uint(u32_read(mem));
		case bv_type_int: return bv_variable_create_int(s32_read(mem));
		case bv_type_short: return bv_variable_create_short(s16_read(mem));
		case bv_type_ushort: return bv_variable_create_ushort(u16_read(mem));
		case bv_type_char: return bv_variable_create_char(s8_read(mem));
		case bv_type_uchar: return bv_variable_create_uchar(u8_read(mem));
		case bv_type_float: return bv_variable_create_float(float_read(mem));
		case bv_type_string: {
			bv_variable var;
			var.type = bv_type_string;
			var.value = string_read(mem);
			return var;
		}
	}

	return bv_variable_create_void();
}


// operators, casting and setting value
u8 bv_variable_op_equal(bv_program* prog, bv_variable left, bv_variable right)
{
	u8 out = 0;

	if (left.type == bv_type_function || right.type == bv_type_function) {
		if (left.type == bv_type_pointer && right.type == bv_type_pointer)
			return ((bv_function*)left.value)->code == ((bv_function*)right.value);
		else
			return 0;
	}
	else if (left.type == bv_type_pointer || right.type == bv_type_pointer) {
		if (left.type == bv_type_pointer && right.type == bv_type_pointer)
			return bv_variable_op_equal(prog, *((bv_variable*)left.value), *((bv_variable*)right.value));
		else if (left.type == bv_type_pointer)
			return bv_variable_op_equal(prog, *((bv_variable*)left.value), right);
		else
			return bv_variable_op_equal(prog, left, *((bv_variable*)right.value));
	}
	else if (left.type == bv_type_object || right.type == bv_type_object) {
		if (left.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(left);

			if (obj == 0)
				return left.value == right.value; // are we check if the object is null?

			bv_function* func = bv_object_get_method(obj, "==");
			if (func == 0)
				return left.value == right.value; // just check pointers if operator== does not exist
			else {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, right);

				out = bv_variable_get_uchar(bv_program_call(prog, func, &args, obj));

				bv_stack_delete(&args);
			}

			return out;
		}
		else if (right.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(right);

			if (obj == 0)
				return left.value == right.value; // are we check if the object is null?

			bv_function* func = bv_object_get_method(obj, "==");

			if (func == 0)
				return left.value == right.value; // just check pointers if operator== does not exist
			else {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, left);

				out = bv_variable_get_uchar(bv_program_call(prog, func, &args, obj));

				bv_stack_delete(&args);
			}

			return out;
		}
	}
	else if (left.type == bv_type_array || right.type == bv_type_array) {
		if (left.type == bv_type_array && right.type == bv_type_array) {
			bv_array a1 = bv_variable_get_array(left);
			bv_array a2 = bv_variable_get_array(right);

			if (a2.dim != a1.dim)
				return 0;
			else {
				for (u32 i = 0; i < a2.dim; i++) {
					if (a1.length[i] != a2.length[i])
						return 0;
				}
				
				u32 rng = bv_array_get_range(a2);

				for (u32 i = 0; i < rng; i++)
					if (bv_variable_op_equal(prog, a1.data[rng], a2.data[rng]) == 0)
						return 0;
			}
			out = 1;
		}
	}
	else if (left.type == bv_type_float || right.type == bv_type_float) {
		float x = 0, y = 0;

		if (left.type == bv_type_float)
			x = bv_variable_get_float(left);
		else if (left.type == bv_type_string)
			return 0;
		else x = bv_variable_get_int(left);

		if (right.type == bv_type_float)
			y = bv_variable_get_float(right);
		else if (right.type == bv_type_string)
			return 0;
		else y = bv_variable_get_int(right);

		out = (x == y);
	}
	else if (left.type == bv_type_string || right.type == bv_type_string) {
		if (left.type == bv_type_string && right.type == bv_type_string)
			out = (strcmp(bv_variable_get_string(left), bv_variable_get_string(right)) == 0);
	}
	else out = (bv_variable_get_uint(left) == bv_variable_get_uint(right));

	return out;
}
u8 bv_variable_op_not_equal(bv_program* prog, bv_variable left, bv_variable right)
{
	return !bv_variable_op_equal(prog, left, right);
}
u8 bv_variable_op_greater_than(bv_program* prog, bv_variable left, bv_variable right)
{
	u8 out = 0;

	if (left.type == bv_type_function || right.type == bv_type_function) {
		if (left.type == bv_type_pointer && right.type == bv_type_pointer)
			return ((bv_function*)left.value)->code_length > ((bv_function*)right.value)->code_length;
		else
			return 0;
	}
	else if (left.type == bv_type_pointer || right.type == bv_type_pointer) {
		if (left.type == bv_type_pointer && right.type == bv_type_pointer)
			return bv_variable_op_greater_than(prog, *((bv_variable*)left.value), *((bv_variable*)right.value));
		else if (left.type == bv_type_pointer)
			return bv_variable_op_greater_than(prog, *((bv_variable*)left.value), right);
		else
			return bv_variable_op_greater_than(prog, left, *((bv_variable*)right.value));
	}
	else if (left.type == bv_type_object || right.type == bv_type_object) {
		if (left.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(left);
			bv_function* func = bv_object_get_method(obj, ">");
			if (func == 0)
				return 0;
			else {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, right);

				out = bv_variable_get_uchar(bv_program_call(prog, func, &args, obj));

				bv_stack_delete(&args);
			}

			return out;
		}
		else if (right.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(right);
			bv_function* func = bv_object_get_method(obj, ">");
			if (func == 0)
				return 0;
			else {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, left);

				out = !bv_variable_get_uchar(bv_program_call(prog, func, &args, obj));

				bv_stack_delete(&args);
			}

			return out;
		}

		bv_program_error(prog, 0, 44, "Object does not have an operator > overload");
	}
	else if (left.type == bv_type_array || right.type == bv_type_array) {
		if (left.type == bv_type_array && right.type == bv_type_array) {
			bv_array a1 = bv_variable_get_array(left);
			bv_array a2 = bv_variable_get_array(right);

			if (a2.dim != a1.dim)
				return a1.dim > a2.dim;
			else {
				for (u8 i = 0; i < a2.dim; i++) {
					if (a1.length[i] != a2.length[i])
						return a1.dim > a2.dim;
				}
			}
		}
	}
	else if (left.type == bv_type_float || right.type == bv_type_float) {
		float x = 0, y = 0;

		if (left.type == bv_type_float)
			x = bv_variable_get_float(left);
		else if (left.type == bv_type_string)
			return 0;
		else x = bv_variable_get_int(left);

		if (right.type == bv_type_float)
			y = bv_variable_get_float(right);
		else if (right.type == bv_type_string)
			return 0;
		else y = bv_variable_get_int(right);

		out = (x > y);
	}
	else if (left.type == bv_type_string || right.type == bv_type_string) {
		if (left.type == bv_type_string && right.type == bv_type_string)
			out = strlen(bv_variable_get_string(left)) > strlen(bv_variable_get_string(right));
	}
	else out = (bv_variable_get_int(left) > bv_variable_get_int(right));

	return out;
}
u8 bv_variable_op_greater_equal(bv_program* prog, bv_variable left, bv_variable right)
{
	return bv_variable_op_greater_than(prog, left, right) || bv_variable_op_equal(prog, left, right);
}
u8 bv_variable_op_less_than(bv_program* prog, bv_variable left, bv_variable right)
{
	return !bv_variable_op_greater_equal(prog, left, right);
}
u8 bv_variable_op_less_equal(bv_program* prog, bv_variable left, bv_variable right)
{
	return !bv_variable_op_greater_than(prog, left, right);
}
void bv_variable_op_assign(bv_program* prog, bv_variable* left, bv_variable right)
{
	if (left->type == bv_type_function) {
		// do nothing
	} else if (left->type == bv_type_pointer)
		bv_variable_op_assign(prog, ((bv_variable*)left->value), right);
	else if (left->type == bv_type_object) {
		bv_object* obj = bv_variable_get_object(*left);
		bv_function* func = bv_object_get_method(obj, "=");
		if (func != 0) {
			bv_stack args = bv_stack_create();
			bv_stack_push(&args, right);
			
			bv_program_call(prog, func, &args, obj);

			bv_stack_delete(&args);
		}
		else
			bv_program_error(prog, 0, 43, "Object does not have an operator = overload");
	}
	else *left = bv_variable_cast(left->type, right);
}
bv_variable bv_variable_op_add(bv_program* prog, bv_variable left, bv_variable right)
{
	bv_variable out = bv_variable_create_null_object();


	if (left.type == bv_type_function || right.type == bv_type_function) {
		return bv_variable_create_null_object(); // do nothing
	}
	else if (left.type == bv_type_pointer || right.type == bv_type_pointer) {
		if (left.type == bv_type_pointer && right.type == bv_type_pointer)
			return bv_variable_op_add(prog, *((bv_variable*)left.value), *((bv_variable*)right.value));
		else if (left.type == bv_type_pointer)
			return bv_variable_op_add(prog, *((bv_variable*)left.value), right);
		else
			return bv_variable_op_add(prog, left, *((bv_variable*)right.value));
	}
	else if (left.type == bv_type_object || right.type == bv_type_object) {
		if (left.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(left);
			bv_function* func = bv_object_get_method(obj, "+");
			if (func != 0) {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, right);

				out = bv_program_call(prog, func, &args, obj);

				bv_stack_delete(&args);
			}

			return out;
		}
		else if (right.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(right);
			bv_function* func = bv_object_get_method(obj, "+");
			if (func != 0) {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, left);

				out = bv_program_call(prog, func, &args, obj);

				bv_stack_delete(&args);
			}

			return out;
		}

		bv_program_error(prog, 0, 42, "Object does not have an operator + overload");
	}
	else if (left.type == bv_type_array || right.type == bv_type_array) {
		if (left.type == bv_type_array && right.type == bv_type_array) {
			bv_array a1 = bv_variable_get_array(left);
			bv_array a2 = bv_variable_get_array(right);

			out = bv_variable_create_array(bv_array_merge(prog, a1, a2));
		}
		else {
			bv_program_error(prog, 0, 7, "Cannot use operator + between one array and one non-array value");
			out = bv_variable_create_void();
		}
	}
	else if (left.type == bv_type_float || right.type == bv_type_float) {
		float x = 0, y = 0;

		if (left.type == bv_type_float)
			x = bv_variable_get_float(left);
		else if (left.type == bv_type_string)
			return out;
		else x = bv_variable_get_int(left);

		if (right.type == bv_type_float)
			y = bv_variable_get_float(right);
		else if (right.type == bv_type_string)
			return out;
		else y = bv_variable_get_int(right);

		out = bv_variable_create_float(x + y);
	}
	else if (left.type == bv_type_string || right.type == bv_type_string) {
		if (left.type == bv_type_string && right.type == bv_type_string) {
			bv_string str1 = bv_variable_get_string(left);
			u32 strl1 = strlen(str1);

			bv_string str2 = bv_variable_get_string(right);
			u32 strl2 = strlen(str2);

			bv_string res = (bv_string)calloc(strl1+strl2+1, strl1 + strl2 + 1);
			strcat(res, str1);
			strcat(res, str2);
			res[strl1 + strl2] = '\0';

			out = bv_variable_create_string(res);

			free(res);
		}
	}
	else out = bv_variable_create(bv_type_get(left.type, right.type), (void*)(bv_variable_get_uint(left) + bv_variable_get_uint(right)));

	return out;
}
bv_variable bv_variable_op_subtract(bv_program* prog, bv_variable left, bv_variable right)
{
	bv_variable out = bv_variable_create_null_object();


	if (left.type == bv_type_function || right.type == bv_type_function) {
		return bv_variable_create_null_object(); // do nothing
	}
	else if (left.type == bv_type_pointer || right.type == bv_type_pointer) {
		if (left.type == bv_type_pointer && right.type == bv_type_pointer)
			return bv_variable_op_subtract(prog, *((bv_variable*)left.value), *((bv_variable*)right.value));
		else if (left.type == bv_type_pointer)
			return bv_variable_op_subtract(prog, *((bv_variable*)left.value), right);
		else
			return bv_variable_op_subtract(prog, left, *((bv_variable*)right.value));
	}
	else if (left.type == bv_type_object || right.type == bv_type_object) {
		if (left.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(left);
			bv_function* func = bv_object_get_method(obj, "-");
			if (func != 0) {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, right);

				out = bv_program_call(prog, func, &args, obj);

				bv_stack_delete(&args);
			}

			return out;
		}
		else if (right.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(right);
			bv_function* func = bv_object_get_method(obj, "-");
			if (func != 0) {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, left);

				out = bv_program_call(prog, func, &args, obj);

				bv_stack_delete(&args);
			}

			return out;
		}
		bv_program_error(prog, 0, 40, "Object does not have an operator - overload");
	}
	else if (left.type == bv_type_array || right.type == bv_type_array)
		bv_program_error(prog, 0, 39, "Cannot use subtraction operator on the array");
	else if (left.type == bv_type_float || right.type == bv_type_float) {
		float x = 0, y = 0;

		if (left.type == bv_type_float)
			x = bv_variable_get_float(left);
		else if (left.type == bv_type_string)
			return out;
		else x = bv_variable_get_int(left);

		if (right.type == bv_type_float)
			y = bv_variable_get_float(right);
		else if (right.type == bv_type_string)
			return out;
		else y = bv_variable_get_int(right);

		out = bv_variable_create_float(x - y);
	}
	else if (left.type == bv_type_string || right.type == bv_type_string)
		bv_program_error(prog, 0, 38, "Cannot use subtraction opreator on the string");
	else out = bv_variable_create(bv_type_get(left.type, right.type), (void*)(bv_variable_get_uint(left) - bv_variable_get_uint(right)));

	return out;
}
bv_variable bv_variable_op_divide(bv_program* prog, bv_variable left, bv_variable right)
{
	bv_variable out = bv_variable_create_null_object();

	if (left.type == bv_type_function || right.type == bv_type_function) {
		return bv_variable_create_null_object(); // do nothing
	}
	else if (left.type == bv_type_pointer || right.type == bv_type_pointer) {
		if (left.type == bv_type_pointer && right.type == bv_type_pointer)
			return bv_variable_op_divide(prog, *((bv_variable*)left.value), *((bv_variable*)right.value));
		else if (left.type == bv_type_pointer)
			return bv_variable_op_divide(prog, *((bv_variable*)left.value), right);
		else
			return bv_variable_op_divide(prog, left, *((bv_variable*)right.value));
	}
	else if (left.type == bv_type_object || right.type == bv_type_object) {
		if (left.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(left);
			bv_function* func = bv_object_get_method(obj, "/");
			if (func != 0) {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, right);

				out = bv_program_call(prog, func, &args, obj);

				bv_stack_delete(&args);
			}

			return out;
		}
		else if (right.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(right);
			bv_function* func = bv_object_get_method(obj, "/");
			if (func != 0) {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, left);

				out = bv_program_call(prog, func, &args, obj);

				bv_stack_delete(&args);
			}

			return out;
		}

		bv_program_error(prog, 0, 37, "Object does not have an operator / overload");
	}
	else if (left.type == bv_type_array || right.type == bv_type_array)
		bv_program_error(prog, 0, 36, "Cannot use division operator on the array");
	else if (left.type == bv_type_float || right.type == bv_type_float) {
		float x = 0, y = 0;

		if (left.type == bv_type_float)
			x = bv_variable_get_float(left);
		else if (left.type == bv_type_string)
			return out;
		else x = bv_variable_get_int(left);

		if (right.type == bv_type_float)
			y = bv_variable_get_float(right);
		else if (right.type == bv_type_string)
			return out;
		else y = bv_variable_get_int(right);

		out = bv_variable_create_float(x / y);
	}
	else if (left.type == bv_type_string || right.type == bv_type_string)
		bv_program_error(prog, 0, 35, "Cannot use division operator on the string");
	else out = bv_variable_create(bv_type_get(left.type, right.type), (void*)(bv_variable_get_uint(left) / bv_variable_get_uint(right)));

	return out;
}
bv_variable bv_variable_op_multiply(bv_program* prog, bv_variable left, bv_variable right)
{
	bv_variable out = bv_variable_create_null_object();


	if (left.type == bv_type_function || right.type == bv_type_function) {
		return bv_variable_create_null_object(); // do nothing
	}
	else if (left.type == bv_type_pointer || right.type == bv_type_pointer) {
		if (left.type == bv_type_pointer && right.type == bv_type_pointer)
			return bv_variable_op_multiply(prog, *((bv_variable*)left.value), *((bv_variable*)right.value));
		else if (left.type == bv_type_pointer)
			return bv_variable_op_multiply(prog, *((bv_variable*)left.value), right);
		else
			return bv_variable_op_multiply(prog, left, *((bv_variable*)right.value));
	}
	else if (left.type == bv_type_object || right.type == bv_type_object) {
		if (left.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(left);
			bv_function* func = bv_object_get_method(obj, "*");
			if (func != 0) {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, right);

				out = bv_program_call(prog, func, &args, obj);

				bv_stack_delete(&args);
			}

			return out;
		}
		else if (right.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(right);
			bv_function* func = bv_object_get_method(obj, "*");
			if (func != 0) {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, left);

				out = bv_program_call(prog, func, &args, obj);

				bv_stack_delete(&args);
			}

			return out;
		}
		bv_program_error(prog, 0, 32, "Object does not have an operator * overload");
	}
	else if (left.type == bv_type_array || right.type == bv_type_array)
		bv_program_error(prog, 0, 33, "Cannot use multiplication operator on the array");
	else if (left.type == bv_type_float || right.type == bv_type_float) {
		float x = 0, y = 0;

		if (left.type == bv_type_float)
			x = bv_variable_get_float(left);
		else if (left.type == bv_type_string)
			return out;
		else x = bv_variable_get_int(left);

		if (right.type == bv_type_float)
			y = bv_variable_get_float(right);
		else if (right.type == bv_type_string)
			return out;
		else y = bv_variable_get_int(right);

		out = bv_variable_create_float(x * y);
	}
	else if (left.type == bv_type_string || right.type == bv_type_string)
		bv_program_error(prog, 0, 34, "Cannot use multiplication operator on the string");
	else out = bv_variable_create(bv_type_get(left.type, right.type), (void*)(bv_variable_get_uint(left) * bv_variable_get_uint(right)));

	return out;
}
void bv_variable_op_increment(bv_program* prog, bv_variable* left)
{
	bv_variable out = bv_variable_create_null_object();

	if (left->type == bv_type_function)
		return bv_variable_create_null_object(); // do nothing
	else if (left->type == bv_type_pointer)
		return bv_variable_op_increment(prog, ((bv_variable*)left->value));
	else if (left->type == bv_type_object) {
		bv_object* obj = bv_variable_get_object(*left);
		bv_function* func = bv_object_get_method(obj, "++");
		if (func != 0)
			*left = bv_program_call(prog, func, NULL, obj);
		else
			bv_program_error(prog, 0, 31, "Object does not have an operator ++ overload");
	}
	else if (left->type == bv_type_array)
		bv_program_error(prog, 0, 30, "Cannot use increment operator on the array");
	else if (left->type == bv_type_float)
		*left = bv_variable_create_float(bv_variable_get_float(*left)+1);
	else if (left->type == bv_type_string)
		bv_program_error(prog, 0, 29, "Cannot use increment operator on the string");
	else left->value = (void*)((u32)left->value+1);
}
void bv_variable_op_decrement(bv_program* prog, bv_variable* left)
{
	bv_variable out = bv_variable_create_null_object();

	if (left->type == bv_type_function)
		return bv_variable_create_null_object(); // do nothing
	else if (left->type == bv_type_pointer)
		return bv_variable_op_decrement(prog, ((bv_variable*)left->value));
	else if (left->type == bv_type_object) {
		bv_object* obj = bv_variable_get_object(*left);
		bv_function* func = bv_object_get_method(obj, "--");
		if (func != 0)
			*left = bv_program_call(prog, func, NULL, obj);
		else
			bv_program_error(prog, 0, 28, "Object does not have an operator -- overload");
	}
	else if (left->type == bv_type_array)
		bv_program_error(prog, 0, 27, "Cannot use decrement operator on the array");
	else if (left->type == bv_type_float)
		*left = bv_variable_create_float(bv_variable_get_float(*left) - 1);
	else if (left->type == bv_type_string)
		bv_program_error(prog, 0, 26, "Cannot use decrement operator on the string");
	else left->value = (void*)((u32)left->value - 1);
}
bv_variable bv_variable_op_negate(bv_program* prog, bv_variable left)
{
	bv_variable out = bv_variable_create_null_object();

	if (left.type == bv_type_function)
		return bv_variable_create_null_object(); // do nothing
	else if (left.type == bv_type_pointer)
		return bv_variable_op_negate(prog, *((bv_variable*)left.value));
	else if (left.type == bv_type_object) {
		bv_object* obj = bv_variable_get_object(left);
		bv_function* func = bv_object_get_method(obj, "-");
		if (func != 0)
			out = bv_program_call(prog, func, NULL, obj);
		else
			bv_program_error(prog, 0, 24, "Object does not have an operator - overload");
	}
	else if (left.type == bv_type_array)
		bv_program_error(prog, 0, 22, "Cannot negate the array");
	else if (left.type == bv_type_float)
		out = bv_variable_create_float(-bv_variable_get_float(left));
	else if (left.type == bv_type_string)
		bv_program_error(prog, 0, 23, "Cannot negate the string");

	else out = bv_variable_create(left.type, (void*)(-bv_variable_get_int(left)));

	return out;
}
bv_variable bv_variable_op_modulo(bv_program* prog, bv_variable left, bv_variable right)
{
	bv_variable out = bv_variable_create_null_object();

	if (left.type == bv_type_function || right.type == bv_type_function)
		return bv_variable_create_null_object(); // do nothing
	else if (left.type == bv_type_pointer || right.type == bv_type_pointer) {
		if (left.type == bv_type_pointer && right.type == bv_type_pointer)
			return bv_variable_op_modulo(prog, *((bv_variable*)left.value), *((bv_variable*)right.value));
		else if (left.type == bv_type_pointer)
			return bv_variable_op_modulo(prog, *((bv_variable*)left.value), right);
		else
			return bv_variable_op_modulo(prog, left, *((bv_variable*)right.value));
	}
	else if (left.type == bv_type_object || right.type == bv_type_object) {
		if (left.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(left);
			bv_function* func = bv_object_get_method(obj, "%");
			if (func != 0) {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, right);

				out = bv_program_call(prog, func, &args, obj);

				bv_stack_delete(&args);
			}

			return out;
		}
		else if (right.type == bv_type_object) {
			bv_object* obj = bv_variable_get_object(right);
			bv_function* func = bv_object_get_method(obj, "%");
			if (func != 0) {
				bv_stack args = bv_stack_create();
				bv_stack_push(&args, left);

				out = bv_program_call(prog, func, &args, obj);

				bv_stack_delete(&args);
			}

			return out;
		}
		bv_program_error(prog, 0, 19, "Object does not have an operator % overload");
	}
	else if (left.type == bv_type_array || right.type == bv_type_array) {
		bv_program_error(prog, 0, 20, "Cannot use operator % on the array");
	}
	else if (left.type == bv_type_float || right.type == bv_type_float) {
		float x = 0, y = 0;

		if (left.type == bv_type_float)
			x = bv_variable_get_float(left);
		else if (left.type == bv_type_string)
			return out;
		else x = bv_variable_get_int(left);

		if (right.type == bv_type_float)
			y = bv_variable_get_float(right);
		else if (right.type == bv_type_string)
			return out;
		else y = bv_variable_get_int(right);

		out = bv_variable_create_float((u32)x % (u32)y);
	}
	else if (left.type == bv_type_string || right.type == bv_type_string)
		bv_program_error(prog, 0, 21, "Cannot use operator % on the string");
	else out = bv_variable_create(bv_type_get(left.type, right.type), (void*)(bv_variable_get_uint(left) % bv_variable_get_uint(right)));

	return out;
}
u8 bv_variable_op_not(bv_program* prog, bv_variable left)
{
	u8 out = 0;

	if (left.type == bv_type_function)
		return 0; // do nothing
	else if (left.type == bv_type_pointer)
		return bv_variable_op_not(prog, *((bv_variable*)left.value));
	else if (left.type == bv_type_object) {
		bv_object* obj = bv_variable_get_object(left);
		bv_function* func = bv_object_get_method(obj, "!");
		if (func != 0)
			out = bv_variable_get_uchar(bv_program_call(prog, func, NULL, obj));
		else
			bv_program_error(prog, 0, 18, "Object does not have an operator ! overload");
	}
	else if (left.type == bv_type_array) {
		bv_program_error(prog, 0, 16, "Cannot use operator ! on the array");
	}
	else if (left.type == bv_type_float) {
		out = !bv_variable_get_float(left);
	}
	else if (left.type == bv_type_string) {
		bv_program_error(prog, 0, 17, "Cannot use operator ! on the string");
	}
	else out = !bv_variable_get_uint(left);

	return out;
}
bv_variable bv_variable_cast(bv_type new_type, bv_variable right)
{
	bv_type old_type = right.type;

	bv_variable ret; 
	ret.type = new_type;

	if (new_type == old_type)
		return bv_variable_copy(right);
	else if (new_type == bv_type_function)
		return bv_variable_create_null_object();
	else if (old_type == bv_type_pointer)
		return bv_variable_cast(new_type, *((bv_variable*)right.value));
	else if (new_type == bv_type_pointer)
		return bv_variable_create_pointer(&right);
	else if (new_type == bv_type_string) {
		if (old_type == bv_type_int || old_type == bv_type_short)
			ret = bv_variable_create_string(bv_itoa(bv_variable_get_int(right), 10));
		else if (old_type == bv_type_uint || old_type == bv_type_ushort)
			ret = bv_variable_create_string(bv_itoa(bv_variable_get_uint(right), 10));
		else if (old_type == bv_type_char || old_type == bv_type_uchar) {
			bv_string m = (bv_string)malloc(sizeof(char) * 2);
			m[0] = bv_variable_get_char(right);
			m[1] = 0;

			ret = bv_variable_create_string(m);

			free(m);
		} else if (old_type == bv_type_float) {
			float value = bv_variable_get_float(right);
			ret = bv_variable_create_string(bv_ftoa(value, 6));
		} else if (old_type == bv_type_object)
			ret = bv_variable_create_string("[Object]");
		else if (old_type == bv_type_array)
			ret = bv_variable_create_string("[Array]");
	}
	else if (new_type == bv_type_float) {
		if (old_type == bv_type_int || old_type == bv_type_short || old_type == bv_type_char)
			ret = bv_variable_create_float(bv_variable_get_int(right));
		else if (old_type == bv_type_uint || old_type == bv_type_ushort || old_type == bv_type_uchar)
			ret = bv_variable_create_float(bv_variable_get_uint(right));
		else if (old_type == bv_type_object)
			ret = bv_variable_create_float(0);
		else if (old_type == bv_type_array)
			ret = bv_variable_create_float(0);
		else
			ret = bv_variable_create_float(0);
	}
	else if (bv_type_is_integer(new_type)) {
		if (old_type == bv_type_float) {
			float value = bv_variable_get_float(right);

			ret = bv_variable_create(new_type, (void*)((int)value));
		} else if (old_type == bv_type_object)
			ret = bv_variable_create(new_type, 0);
		else if (old_type == bv_type_array)
			ret = bv_variable_create(new_type, 0);
	}
	else if (new_type == bv_type_object)
		ret = bv_variable_create_null_object();
	else if (new_type == bv_type_array)
		ret = bv_variable_create_array(bv_array_create(0, 0));
	/* else bv_program_error(0, 12, "Undefined cast"); */


	return ret;
}