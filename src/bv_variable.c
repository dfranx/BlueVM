#include <BlueVM/bv_variable.h>

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
bv_string bv_variable_get_string(bv_variable var)
{
	char* data = (char*)var.value;
	return bv_string_copy(data, strlen(data));
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
bv_variable bv_variable_create_string(const char * var)
{
	bv_variable ret;
	ret.type = bv_type_string;
	ret.value = var;
	return ret;
}

void bv_variable_set_int(bv_variable * var, s32 val)
{
	if (var->type != bv_type_int)
		return;
	var->value = val;
}
void bv_variable_set_uint(bv_variable * var, u32 val)
{
	if (var->type != bv_type_uint)
		return;
	var->value = val;
}
void bv_variable_set_short(bv_variable * var, s16 val)
{
	if (var->type != bv_type_short)
		return;
	var->value = val;
}
void bv_variable_set_ushort(bv_variable * var, u16 val)
{
	if (var->type != bv_type_ushort)
		return;
	var->value = val;
}
void bv_variable_set_char(bv_variable * var, s8 val)
{
	if (var->type != bv_type_char)
		return;
	var->value = val;
}
void bv_variable_set_uchar(bv_variable * var, u8 val)
{
	if (var->type != bv_type_uchar)
		return;
	var->value = val;
}
void bv_variable_set_float(bv_variable * var, float val)
{
	if (var->type != bv_type_float)
		return;
	*((float*)var->value) = val;
}
void bv_variable_set_string(bv_variable * var, const char * val)
{
	if (var->type != bv_type_short)
		return;
	var->value = val;
}

void bv_variable_deinitialize(bv_variable * var)
{
	// this is sort of deconstructor
	if (var->type == bv_type_float)
		free(var->value);
	// else if var == class
	//		bv_program_call(prog, "~ClassName");
}
bv_variable bv_variable_read(byte * mem, bv_type type)
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
	else if (type == bv_type_string)
		return bv_variable_create_string("NotSupported"); // [TODO] delete bv_string
}
