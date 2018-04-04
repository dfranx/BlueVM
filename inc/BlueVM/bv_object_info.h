#ifndef __BLUEVM_OBJECT_INFO_H__
#define __BLUEVM_OBJECT_INFO_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_name_list.h>
#include <BlueVM/bv_function.h>

typedef struct bv_function_pool bv_function_pool;
typedef struct bv_variable bv_variable;

typedef bv_variable(*bv_external_method)(bv_object*, u8, bv_variable*);

typedef struct bv_object_info {
	string name;	// class type/object name (eg "Vehicle", "Animal", etc...)

	bv_name_list props;			// property names

	bv_function_pool* method_info;
	bv_function** methods;	// method data

	u16 ext_method_count; // todo: change to u16
	bv_external_method* ext_methods;
	string* ext_method_names;
} bv_object_info;

bv_object_info* bv_object_info_create(const string name);
bv_object_info* bv_object_info_read(byte** mem, byte* orig_mem);
void bv_object_info_delete(bv_object_info* info);

void bv_object_info_add_property(bv_object_info* info, const string name);
void bv_object_info_add_ext_method(bv_object_info* info, const string name, bv_external_method method);

#endif