#ifndef __BLUEVM_ARRAY_H__
#define __BLUEVM_ARRAY_H__
#include <BlueVM/types.h>

typedef struct bv_variable bv_variable;

typedef struct bv_array {
	u8 dim;			// number of dimensions
	u16* length;		// array that contains the length of each dimension
	bv_variable* data;	// values
} bv_array;

bv_array bv_array_create(u8 dim, u16* cnt);
u32 bv_array_get_range(bv_array arr);							// get amount of data that is being stored
u32 bv_array_get_index(bv_array arr, u16* atInd);				// get actual index of an array
bv_variable bv_array_get(bv_array arr, u16* atInd);				// get value at given index
void bv_array_set(bv_array arr, u16* atInd, bv_variable val);	// set value at given index 
bv_array bv_array_merge(bv_array a1, bv_array a2);				// merge 2 arrays into one
void bv_array_deinitialize(bv_array* arr);

#endif