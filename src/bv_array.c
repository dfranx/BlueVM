#include <BlueVM/bv_array.h>
#include <BlueVM/bv_variable.h>

bv_array bv_array_create(int dim, int* cnt)
{
	bv_array ret;
	ret.dim = dim;
	ret.length = malloc(sizeof(int) * dim);
	memcpy(ret.length, cnt, sizeof(int)*dim);

	ret.data = malloc(sizeof(bv_variable) * bv_array_get_range(ret));

	return ret;
}

int bv_array_get_range(bv_array arr)
{
	int ret = 1;
	for (int i = 0; i < arr.dim; i++)
		ret *= arr.length[i];

	return ret;
}

int bv_array_get_index(bv_array arr, int * atInd)
{
	int index = 0, curCount = 0;
	for (int i = arr.dim - 1; i >= 0; i--) {
		if (curCount == 0)
			index += atInd[i];
		else
			index += atInd[i] * curCount;
		curCount += arr.length[i];
	}

	return index;
}

int bv_array_get_size(bv_array arr)
{
	return sizeof(int) + sizeof(int) * arr.dim + bv_array_get_range(arr) * sizeof(bv_variable);
}

bv_variable bv_array_get(bv_array arr, int* atInd)
{
	return arr.data[bv_array_get_index(arr, atInd)];
}

void bv_array_set(bv_array arr, int * atInd, bv_variable val)
{
	int index = bv_array_get_index(arr, atInd);
	bv_variable_deinitialize(&arr.data[index]);
	arr.data[index] = bv_variable_copy(val);
}

void bv_array_deinitialize(bv_array* arr)
{
	int size = bv_array_get_range(*arr);

	for (int i = 0; i < size; i++)
		bv_variable_deinitialize(&arr->data[i]);

	free(arr->data);
	free(arr->length);
	free(arr);
}