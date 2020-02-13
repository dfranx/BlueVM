#include <BlueVM/bv_object.h>
#include <BlueVM/bv_variable.h>
#include <BlueVM/bv_stack.h>
#include <BlueVM/bv_scope.h>
#include <BlueVM/bv_program.h>
#include <stdlib.h>
#include <string.h>

bv_object* bv_object_create(bv_object_info* info)
{
	bv_object* ret = (bv_object*)malloc(sizeof(bv_object));
	ret->prop = (bv_variable*)calloc(info->props.name_count , sizeof(bv_variable));
	ret->type = info;
	ret->user_data = NULL;

	return ret;
}

bv_variable* bv_object_get_property(bv_object* obj, const bv_string name)
{
	bv_name_list* props = &obj->type->props;

	for (u16 i = 0; i < props->name_count; i++)
		if (strcmp(props->names[i], name) == 0)
			return &obj->prop[i];

	return 0;
}
u8 bv_object_set_property(bv_object* obj, const char* name, bv_variable val)
{
	bv_name_list* props = &obj->type->props;

	for (u16 i = 0; i < props->name_count; i++)
		if (strcmp(props->names[i], name) == 0) {
			bv_variable_deinitialize(&obj->prop[i]);
			obj->prop[i] = val;
			return 1;
		}
	return 0;
}

bv_function* bv_object_get_method(bv_object* obj, const char* name)
{
	bv_function_pool* info = obj->type->method_info;
	for (u16 i = 0; i < info->count; i++)
		if (strcmp(info->names[i], name) == 0)
			return obj->type->methods[i];

	return 0;
}
bv_external_method bv_object_get_ext_method(bv_object* obj, const char* name)
{
	bv_object_info* info = obj->type;
	for (u16 i = 0; i < info->ext_method_count; i++)
		if (strcmp(info->ext_method_names[i], name) == 0)
			return obj->type->ext_methods[i];

	return 0;
}
bv_variable bv_object_call_method(bv_object* obj, const char* name, bv_scope* scope, u8 argc)
{
	bv_function* func = bv_object_get_method(obj, name);

	if (func != 0)
		bv_scope_push(scope, bv_scope_type_function, func->code, NULL, func, obj, argc);
	else {
		bv_stack func_args = bv_stack_create();

		for (u8 i = 0; i < argc; i++) {
			bv_stack_push(&func_args, bv_stack_top(&scope->stack));
			bv_stack_pop(&scope->stack);
		}

		bv_program* prog = NULL;
		if (scope->count != 0)
			prog = bv_scope_get_state(scope)->prog;

		bv_external_method ext_func = bv_object_get_ext_method(obj, name);
		bv_variable var = (*ext_func)(prog, obj, argc, func_args.data);

		bv_stack_delete(&func_args);

		return var;
	}

	return bv_variable_create_void();
}

void bv_object_deinitialize(bv_object* val)
{
	if (val == NULL) // dont delete null objects
		return;

	if (val->type->on_delete)
		val->type->on_delete(val);

	for (u16 i = 0; i < val->type->props.name_count; i++)
		bv_variable_deinitialize(&val->prop[i]);

	free(val->prop);
	val->prop = NULL;

	free(val);
	val = NULL;
}
