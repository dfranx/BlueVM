#include <BlueVM/bv_array.h>
#include <BlueVM/bv_variable.h>
#include <BlueVM/bv_program.h>
#include <stdlib.h>
#include <string.h>

bv_array bv_array_create(u8 dim, u16* cnt)
{
	bv_array ret;
	ret.dim = dim;
	ret.length = (u16*)malloc(sizeof(u16) * dim);
	memcpy(ret.length, cnt, sizeof(u16)*dim);

	ret.data = (bv_variable*)malloc(sizeof(bv_variable) * bv_array_get_range(ret));

	return ret;
}

u32 bv_array_get_range(bv_array arr)
{
	u32 ret = 1;
	for (u8 i = 0; i < arr.dim; i++)
		ret *= arr.length[i];

	return ret;
}

u32 bv_array_get_index(bv_array arr, u16* atInd)
{
	u32 index = 0, curCount = 0;
	for (s16 i = arr.dim - 1; i >= 0; i--) {
		if (curCount == 0)
			index += atInd[i];
		else
			index += atInd[i] * curCount;
		curCount += arr.length[i];
	}

	return index;
}

bv_variable bv_array_get(bv_array arr, u16* atInd)
{
	return arr.data[bv_array_get_index(arr, atInd)];
}

void bv_array_set(bv_array arr, u16* atInd, bv_variable val)
{
	u32 index = bv_array_get_index(arr, atInd);
	bv_variable_deinitialize(&arr.data[index]);
	arr.data[index] = bv_variable_copy(val);
}

bv_array bv_array_merge(bv_program* prog, bv_array a1, bv_array a2)
{
	if (a1.dim != a2.dim) {
		bv_program_error(prog, 0, 0, "Cannot merge arrays with different dimensions");
		return bv_array_create(0, 0);
	}

	u8 new_dim = a1.dim;
	u16* lens = (u16*)malloc(new_dim * sizeof(u16));

	for (u8 i = 0; i < new_dim; i++)
		lens[i] = a1.length[i] + a2.length[i];

	bv_array ret = bv_array_create(new_dim, lens);

	u32 range1 = bv_array_get_range(a1);
	u32 range2 = bv_array_get_range(a2);
	
	// copy first array
	for (u32 i = 0; i < range1; i++)
		ret.data[i] = bv_variable_copy(a1.data[i]);

	// copy second array
	for (u32 i = 0; i < range2; i++)
		ret.data[range1 + i] = bv_variable_copy(a2.data[i]);

	free(lens);
	return ret;
}

void bv_array_deinitialize(bv_array* arr)
{
	u32 size = bv_array_get_range(*arr);

	for (u32 i = 0; i < size; i++)
		bv_variable_deinitialize(&arr->data[i]);

	free(arr->data);
	free(arr->length);

	arr->dim = 0;
}