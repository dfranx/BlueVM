#ifndef __BLUEVM_OBJECT_H__
#define __BLUEVM_OBJECT_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_name_list.h>
#include <BlueVM/bv_object_info.h>

typedef struct bv_variable bv_variable;
typedef struct bv_program bv_program;
typedef struct bv_stack bv_stack;
typedef struct bv_scope bv_scope;

typedef struct bv_object {
	bv_object_info* type;	// points to information object
	bv_variable* prop;		// properties
} bv_object;

bv_object* bv_object_create(bv_object_info* info);
bv_variable* bv_object_get_property(bv_object* obj, const bv_string name);
bv_function* bv_object_get_method(bv_object* obj, const char* name);
bv_external_method bv_object_get_ext_method(bv_object* obj, const char* name);
bv_variable bv_object_call_method(bv_object* obj, const char* name, bv_scope* scope, u8 argc);
void bv_object_set_property(bv_object* obj, const char* name, bv_variable val);
void bv_object_deinitialize(bv_object* val);

#endif