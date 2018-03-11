#ifndef __BLUEVM_OBJECT_H__
#define __BLUEVM_OBJECT_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_name_list.h>
#include <BlueVM/bv_object_info.h>

typedef struct bv_variable bv_variable;

typedef struct bv_object {
	bv_object_info* type;	// points to information object
	bv_variable* prop;		// properties
} bv_object;

void bv_object_deinitialize(bv_object* val);

#endif