#include <BlueVM/bv_library.h>

bv_library* bv_library_create()
{
	bv_library* ret = (bv_library*)malloc(sizeof(bv_library));

	ret->external_functions = 0;
	ret->external_function_names = 0;
	ret->external_function_count = 0;

	ret->globals = bv_stack_create();
	ret->global_names = bv_name_list_create_empty();

	ret->objects = (bv_object_pool*)malloc(sizeof(bv_object_pool));
	ret->objects->count = 0;
	ret->objects->info = 0;

	return ret;
}
void bv_library_add_function(bv_library* lib, const bv_string name, bv_external_function ext_func)
{
	lib->external_functions = (bv_external_function*)realloc(lib->external_functions, sizeof(bv_external_function) * (lib->external_function_count + 1));
	lib->external_function_names = (bv_string*)realloc(lib->external_function_names, sizeof(bv_string) * (lib->external_function_count + 1));

	lib->external_functions[lib->external_function_count] = ext_func;
	lib->external_function_names[lib->external_function_count] = (bv_string)name;

	lib->external_function_count++;
}
bv_external_function bv_library_get_function(bv_library* lib, const bv_string str)
{
	for (u16 i = 0; i < lib->external_function_count; i++)
		if (strcmp(lib->external_function_names[i], str) == 0)
			return lib->external_functions[i];

	return 0;
}
void bv_library_add_global(bv_library* lib, const bv_string name, bv_variable value)
{
	bv_stack_push(&lib->globals, value);
	bv_name_list_add(&lib->global_names, name);
}
void bv_library_set_global(bv_library * lib, const bv_string name, bv_variable value)
{
	u16 ind = bv_name_list_get_id(lib->global_names, name);
	if (ind != lib->global_names.name_count)
		lib->globals.data[ind] = value;
}
bv_variable* bv_library_get_global(bv_library* lib, const bv_string name)
{
	u16 ind = bv_name_list_get_id(lib->global_names, name);

	if (ind == lib->global_names.name_count)
		return 0;
	else return &lib->globals.data[ind];
}
void bv_library_add_object_info(bv_library* lib, bv_object_info* info)
{
	bv_object_pool_add(lib->objects, info);
}
bv_object_info* bv_library_get_object_info(bv_library* lib, const bv_string name)
{
	for (u16 i = 0; i < lib->objects->count; i++)
		if (strcmp(name, lib->objects->info[i]->name) == 0)
			return lib->objects->info[i];

	return 0;
}
void bv_library_delete(bv_library* lib)
{
	free(lib->external_functions);
	free(lib->external_function_names);
	lib->external_function_count = 0;

	bv_stack_delete(&lib->globals);
	bv_name_list_delete(&lib->global_names);

	bv_object_pool_delete(lib->objects);

	free(lib);
	lib = 0;
}
