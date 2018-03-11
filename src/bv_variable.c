#include <BlueVM/bv_variable.h>
#include <BlueVM/bv_array.h>
#include <string.h>
#include <stdlib.h>

s32 bv_variable_get_int(bv_variable var)
{
	return (s32)var.value;
}
u32 bv_variable_get_uint(bv_variable var)
{
	return (u32)var.value;
}
s16 bv_variable_get_short(bv_variable var)
{
	return (s16)var.value;
}
u16 bv_variable_get_ushort(bv_variable var)
{
	return (u16)var.value;
}
s8 bv_variable_get_char(bv_variable var)
{
	return (s8)var.value;
}
u8 bv_variable_get_uchar(bv_variable var)
{
	return (u8)var.value;
}
float bv_variable_get_float(bv_variable var)
{
	return *((float*)var.value);
}
string bv_variable_get_string(bv_variable var)
{
	return (string)var.value;
}
bv_array bv_variable_get_array(bv_variable var)
{
	return *((bv_array*)var.value);
}
bv_object* bv_variable_get_object(bv_variable var)
{
	return ((bv_object*)var.value);
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
	ret.value = var;
	return ret;
}
bv_variable bv_variable_create_uint(u32 var)
{
	bv_variable ret;
	ret.type = bv_type_uint;
	ret.value = var;
	return ret;
}
bv_variable bv_variable_create_short(s16 var)
{
	bv_variable ret;
	ret.type = bv_type_short;
	ret.value = var;
	return ret;
}
bv_variable bv_variable_create_ushort(u16 var)
{
	bv_variable ret;
	ret.type = bv_type_ushort;
	ret.value = var;
	return ret;
}
bv_variable bv_variable_create_char(s8 var)
{
	bv_variable ret;
	ret.type = bv_type_char;
	ret.value = var;
	return ret;
}
bv_variable bv_variable_create_uchar(u8 var)
{
	bv_variable ret;
	ret.type = bv_type_uchar;
	ret.value = var;
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
bv_variable bv_variable_create_string(string var)
{
	bv_variable ret;
	ret.type = bv_type_string;

	s32 len = strlen(var);
	ret.value = malloc((len + 1) * sizeof(char));
	memcpy(ret.value, var, len);
	((string)ret.value)[len] = 0;
	
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
bv_variable bv_variable_create_object(bv_object* var)
{
	bv_variable ret;
	ret.type = bv_type_object;
	ret.value = malloc(sizeof(bv_object));
	*((bv_object*)ret.value) = *var;
	return ret;
}
bv_variable bv_variable_create_null_object()
{
	bv_variable ret;
	ret.type = bv_type_object;
	ret.value = 0;
	return ret;
}

void bv_variable_set_int(bv_variable * var, s32 val)
{
	if (var->type != bv_type_int)
		return;
	var->value = (s32)val;
}
void bv_variable_set_uint(bv_variable * var, u32 val)
{
	if (var->type != bv_type_uint)
		return;
	var->value = (u32)val;
}
void bv_variable_set_short(bv_variable * var, s16 val)
{
	if (var->type != bv_type_short)
		return;
	var->value = (s16)val;
}
void bv_variable_set_ushort(bv_variable * var, u16 val)
{
	if (var->type != bv_type_ushort)
		return;
	var->value = (u16)val;
}
void bv_variable_set_char(bv_variable * var, s8 val)
{
	if (var->type != bv_type_char)
		return;
	var->value = (s8)val;
}
void bv_variable_set_uchar(bv_variable * var, u8 val)
{
	if (var->type != bv_type_uchar)
		return;
	var->value = (u8)val;
}
void bv_variable_set_float(bv_variable * var, float val)
{
	if (var->type != bv_type_float)
		return;
	*((float*)var->value) = val;
}
void bv_variable_set_string(bv_variable * var, string val)
{
	if (var->type != bv_type_string)
		return;

	free(var->value);


	s32 len = strlen(val);

	var->value = malloc((len + 1) * sizeof(char));
	memcpy(var->value, val, len);

	((string)var->value)[len] = 0;
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

void bv_variable_deinitialize(bv_variable * var)
{
	// this is sort of deconstructor
	if (var->type == bv_type_float || var->type == bv_type_string)
		free(var->value);
	else if (var->type == bv_type_array) {
		bv_array_deinitialize(var->value);
		free(var->value);
	} else if (var->type == bv_type_object) {
		bv_object_deinitialize(var->value);
		free(var->value);
	}
}
bv_variable bv_variable_copy(bv_variable var)
{
	bv_variable ret;
	ret.type = var.type;

	if (var.type == bv_type_float) {
		ret.value = malloc(sizeof(float));
		memcpy(ret.value, var.value, sizeof(float));
	} else if (var.type == bv_type_string) {
		s32 len = strlen(var.value);
		ret.value = malloc((len + 1) * sizeof(char));
		memcpy(ret.value, var.value, len);
		((string)ret.value)[len] = 0;
	} else if (var.type == bv_type_array) {
		ret.value = malloc(sizeof(bv_array) + bv_array_get_size(bv_variable_get_array(var)));
		memcpy(ret.value, var.value, sizeof(bv_array) + bv_array_get_size(bv_variable_get_array(var)));
	}
	else if (var.type == bv_type_object) {
		if (var.value == 0)
			ret.value = 0;
		else {
			ret.value = malloc(sizeof(bv_object));
			memcpy(ret.value, var.value, sizeof(bv_object));
		}
	} else
		ret.value = var.value;

	return ret;
}
bv_variable bv_variable_read(byte** mem, bv_type type)
{
	if (type == bv_type_uint)
		return bv_variable_create_uint(u32_read(mem));
	else if (type == bv_type_int)
		return bv_variable_create_int(s32_read(mem));
	else if (type == bv_type_short)
		return bv_variable_create_short(s16_read(mem));
	else if (type == bv_type_ushort)
		return bv_variable_create_ushort(u16_read(mem));
	else if (type == bv_type_char)
		return bv_variable_create_char(s8_read(mem));
	else if (type == bv_type_uchar)
		return bv_variable_create_uchar(u8_read(mem));
	else if (type == bv_type_float)
		return bv_variable_create_float(float_read(mem));
	else if (type == bv_type_string) {
		bv_variable var;
		var.type = bv_type_string;
		var.value = string_read(mem);
		return var;
	}

	return bv_variable_create_int(0);
}
