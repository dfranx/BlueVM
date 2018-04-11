#include <BlueVM/bv_object.h>
#include <BlueVM/bv_variable.h>
#include <BlueVM/bv_stack.h>
#include <BlueVM/bv_program.h>
#include <stdlib.h>
#include <string.h>

bv_object* bv_object_create(bv_object_info* info)
{
	bv_object* ret = (bv_object*)malloc(sizeof(bv_object));
	ret->prop = (bv_variable*)malloc(sizeof(bv_variable) * info->props.name_count);
	ret->type = info;

	for (u16 i = 0; i < info->props.name_count; i++)
		ret->prop[i] = bv_variable_create_void();

	return ret;
}

bv_variable* bv_object_get_property(bv_object* obj, const string name)
{
	bv_name_list* props = &obj->type->props;

	for (u16 i = 0; i < props->name_count; i++)
		if (strcmp(props->names[i], name) == 0)
			return &obj->prop[i];

	return 0;
}
void bv_object_set_property(bv_object* obj, const string name, bv_variable val)
{
	bv_name_list* props = &obj->type->props;

	for (u16 i = 0; i < props->name_count; i++)
		if (strcmp(props->names[i], name) == 0) {
			obj->prop[i] = val;
			return;
		}
}

bv_function* bv_object_get_method(bv_object* obj, const string name)
{
	bv_function_pool* info = obj->type->method_info;
	for (u16 i = 0; i < info->count; i++)
		if (strcmp(info->names[i], name) == 0)
			return obj->type->methods[i];

	return 0;
}
bv_external_method bv_object_get_ext_method(bv_object* obj, const string name)
{
	bv_object_info* info = obj->type;
	for (u16 i = 0; i < info->ext_method_count; i++)
		if (strcmp(info->ext_method_names[i], name) == 0)
			return obj->type->ext_methods[i];

	return 0;
}
bv_variable bv_object_call_method(bv_object* obj, const string name, bv_program* prog, bv_stack* args)
{
	bv_function* func = bv_object_get_method(obj, name);

	if (func != 0)
		return bv_program_call(prog, func, args, obj);

	bv_external_method ext_func = bv_object_get_ext_method(obj, name);
	return (*ext_func)(obj, args->length, args->data);
}

void bv_object_deinitialize(bv_object* val)
{
	if (val == NULL) // dont delete null objects
		return;

	for (u16 i = 0; i < val->type->props.name_count; i++)
		bv_variable_deinitialize(&val->prop[i]);

	free(val->prop);
	free(val);
}
