#include <BlueVM/bv_function.h>
#include <BlueVM/bv_opcode.h>

bv_function * bv_function_create(byte * mem)
{
	if (bv_opcode_get(mem) != bv_opcode_func_start)
		return 0;
	mem += sizeof(bv_opcode);

	bv_function* func = malloc(sizeof(bv_function));
	
	func->return_type = bv_type_read(mem);
	mem += sizeof(bv_type);

	func->args = u8_read(mem);
	mem += sizeof(u8);

	func->arg_type = malloc(sizeof(bv_type) * func->args);
	for (u8 i = 0; i < func->args; i++) {
		func->arg_type[i] = bv_type_read(mem);
		mem += sizeof(bv_type);
	}

	func->op_length = u32_read(mem);
	mem += sizeof(u32);

	func->op = mem;

	return func;
}

void bv_function_delete(bv_function * func)
{
	free(func->arg_type);
	free(func);
}

bv_function** bv_function_create_array(bv_function_pool* funcs, byte * mem)
{
	bv_function** arr = malloc(sizeof(bv_function)*funcs->count);
	
	for (u16 i = 0; i < funcs->count; i++)
		arr[i] = bv_function_create(mem + funcs->address[i]);

	return arr;
}

void bv_function_delete_array(bv_function** data, u16 len)
{
	for (u16 i = 0; i < len; i++)
		bv_function_delete(data[i]);

	free(data);
}
