#ifndef __BLUEVM_OBJECT_INFO_H__
#define __BLUEVM_OBJECT_INFO_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_name_list.h>
#include <BlueVM/bv_function_pool.h>

typedef struct bv_variable bv_variable;

typedef struct bv_object_info {
	string name;	// class type/object name (eg "Vehicle", "Animal", etc...)

	bv_name_list props;			// property names
	bv_function_pool* methods;	// method data
} bv_object_info;

bv_object_info* bv_object_info_read(byte** mem);

#endif