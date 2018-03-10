#ifndef __BLUEVM_OBJECT_H__
#define __BLUEVM_OBJECT_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_name_list.h>

typedef struct bv_variable bv_variable;

typedef struct bv_object {
	bv_object_info* type;	// points to information object

	int prop_count;
	bv_variable* prop;		// properties
} bv_object;


#endif