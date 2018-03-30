#include <BlueVM/bv_object.h>
#include <BlueVM/bv_variable.h>

bv_object* bv_object_create(bv_object_info* info)
{
	bv_object* ret = malloc(sizeof(bv_object));
	ret->prop = malloc(sizeof(bv_variable) * info->props.name_count);
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

void bv_object_deinitialize(bv_object* val)
{
	for (u16 i = 0; i < val->type->props.name_count; i++)
		bv_variable_deinitialize(&val->prop[i]);

	free(val->prop);
	free(val);
}
