#ifndef __BLUEVM_LIBRARY_H__
#define __BLUEVM_LIBRARY_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_function.h>
#include <BlueVM/bv_stack.h>
#include <BlueVM/bv_name_list.h>
#include <BlueVM/bv_object_pool.h>

typedef struct bv_library {
	u16 external_function_count;
	bv_external_function* external_functions;
	bv_string* external_function_names;

	bv_stack globals;
	bv_name_list global_names;

	bv_object_pool* objects;
} bv_library;

bv_library* bv_library_create();
void bv_library_add_function(bv_library* lib, const bv_string name, bv_external_function ext_func);
bv_external_function bv_library_get_function(bv_library* lib, const bv_string str);
void bv_library_add_global(bv_library* lib, const bv_string name, bv_variable value);
void bv_library_set_global(bv_library* lib, const bv_string name, bv_variable value);
bv_variable* bv_library_get_global(bv_library* lib, const bv_string name);
void bv_library_add_object_info(bv_library* lib, bv_object_info* info);
bv_object_info* bv_library_get_object_info(bv_library* lib, const bv_string name);
void bv_library_delete(bv_library* lib);

#endif