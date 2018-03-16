#include <BlueVM/bv_object_info.h>
#include <BlueVM/bv_function_pool.h>

bv_object_info* bv_object_info_read(byte** mem, byte* orig_mem)
{
	bv_object_info* ret = malloc(sizeof(bv_object_info));
	ret->name = string_read(mem);
	ret->props = bv_name_list_create(mem);

	ret->method_info = bv_function_pool_create(mem);
	ret->methods = bv_function_create_array(ret->method_info, orig_mem);

	return ret;
}