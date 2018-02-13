#ifndef __BLUEVM_ARRAY_H__
#define __BLUEVM_ARRAY_H__

typedef struct bv_variable bv_variable;

typedef struct bv_array {
	int dim;			// number of dimensions
	int* length;		// array that contains the length of each dimension
	bv_variable* data;	// values
} bv_array;

bv_array bv_array_create(int dim, int* cnt);
int bv_array_get_range(bv_array arr);							// get amount of data that is being stored
int bv_array_get_index(bv_array arr, int* atInd);				// get actual index of an array
int bv_array_get_size(bv_array arr);							// get number of bytes this array takes
bv_variable bv_array_get(bv_array arr, int* atInd);				// get value at given index
void bv_array_set(bv_array arr, int* atInd, bv_variable val);	// set value at given index 
void bv_array_deinitialize(bv_array* arr);

#endif