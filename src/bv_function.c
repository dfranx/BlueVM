#include <BlueVM/bv_function.h>
#include <BlueVM/bv_opcode.h>
#include <stdlib.h>

bv_function* bv_function_create(bv_header header, byte* mem)
{
	if (bv_opcode_read(&mem) != bv_opcode_func_start)
		return 0;

	bv_function* func = (bv_function*)malloc(sizeof(bv_function));

	func->args = u8_read(&mem);
	func->arg_type = NULL;
	func->arg_obj_name = NULL;
	func->is_pointer = 0;

	if (!(header.major == 0 && header.minor == 1) && func->args != 0) {
		func->arg_type = (bv_type*)malloc(sizeof(bv_type) * func->args);
		func->arg_obj_name = (bv_string**)malloc(sizeof(bv_string*) * func->args);
		for (u8 i = 0; i < func->args; i++) {
			func->arg_type[i] = bv_type_read(&mem);
			func->arg_obj_name[i] = NULL;

			if (func->arg_type[i] == bv_type_object)
				func->arg_obj_name[i] = string_read(&mem);
		}
	}

	func->code_length = u32_read(&mem);
	func->code = mem;

	return func;
}

void bv_function_delete(bv_function * func)
{
	if (func->arg_type != NULL)
		free(func->arg_type);
	if (func->arg_obj_name != NULL) {
		for (u8 i = 0; i < func->args; i++)
			if (func->arg_obj_name[i] != NULL)
				free(func->arg_obj_name[i]);
		free(func->arg_obj_name);
	}

	free(func);
}

bv_function** bv_function_create_array(bv_header header, bv_function_pool* funcs, byte* mem)
{
	bv_function** arr = (bv_function**)malloc(sizeof(bv_function)*funcs->count);
	
	for (u16 i = 0; i < funcs->count; i++)
		arr[i] = bv_function_create(header, mem + funcs->address[i]);

	return arr;
}

void bv_function_delete_array(bv_function** data, u16 len)
{
	for (u16 i = 0; i < len; i++) {
		if (data[i]) {
			if (data[i]->is_pointer)
				free(data[i]);
			else
				bv_function_delete(data[i]);
		}
	}

	free(data);
}
