#include <BlueVM/bv_object_info.h>
#include <BlueVM/bv_function_pool.h>
#include <string.h>
#include <stdlib.h>

bv_object_info* bv_object_info_read(byte** mem, byte* orig_mem)
{
	bv_object_info* ret = malloc(sizeof(bv_object_info));
	ret->name = string_read(mem);
	ret->props = bv_name_list_create(mem);

	ret->ext_methods = 0;
	ret->ext_method_count = 0;
	ret->ext_method_names = 0;

	ret->method_info = bv_function_pool_create(mem);
	ret->methods = bv_function_create_array(ret->method_info, orig_mem);

	return ret;
}
void bv_object_info_delete(bv_object_info* info)
{
	free(info->name);
	bv_name_list_delete(&info->props);
	bv_function_delete_array(info->methods, info->method_info->count);
	bv_function_pool_delete(info->method_info);

	if (info->ext_methods != 0)
		free(info->ext_methods);
	if (info->ext_method_names != 0)
		free(info->ext_method_names);
	
	free(info);
}

bv_object_info* bv_object_info_create(const string name)
{
	bv_object_info* ret = malloc(sizeof(bv_object_info));
	ret->props = bv_name_list_create_empty();
	
	ret->method_info = malloc(sizeof(bv_function_pool));
	ret->method_info->address = 0;
	ret->method_info->count = 0;
	ret->method_info->names = 0;

	ret->methods = 0;

	ret->ext_methods = 0;
	ret->ext_method_count = 0;
	ret->ext_method_names = 0;

	size_t strl = strlen(name);
	ret->name = malloc((strl + 1) * sizeof(char));
	memcpy(ret->name, name, strl);
	ret->name[strl] = 0;

	return ret;
}
void bv_object_info_add_property(bv_object_info* info, const string name)
{
	bv_name_list_add(&info->props, name);
}
void bv_object_info_add_ext_method(bv_object_info* info, const string name, bv_external_method method)
{
	info->ext_methods = realloc(info->ext_methods, sizeof(bv_external_method) * (info->ext_method_count + 1));
	info->ext_method_names = realloc(info->ext_method_names, sizeof(char*) * (info->ext_method_count + 1));

	info->ext_methods[info->ext_method_count] = method;
	info->ext_method_names[info->ext_method_count] = name;

	info->ext_method_count++;
}