#include <BlueVM/bv_array.h>
#include <BlueVM/bv_variable.h>
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

u32 bv_array_get_size(bv_array arr)
{
	return sizeof(u8) + sizeof(u16) * arr.dim + bv_array_get_range(arr) * sizeof(bv_variable);
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

void bv_array_deinitialize(bv_array* arr)
{
	u32 size = bv_array_get_range(*arr);

	for (u32 i = 0; i < size; i++)
		bv_variable_deinitialize(&arr->data[i]);

	free(arr->data);
	free(arr->length);
	free(arr);
}