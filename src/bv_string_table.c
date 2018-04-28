#include <BlueVM/bv_string_table.h>
#include <stdlib.h>

bv_string_table* bv_string_table_create(byte** mem)
{
	bv_string_table* ret = (bv_string_table*)malloc(sizeof(bv_string_table));

	ret->count = u32_read(mem);
	ret->strings = (string*)malloc(ret->count * sizeof(string));

	for (u32 i = 0; i < ret->count; i++)
		ret->strings[i] = string_read(mem);

	return ret;
}

u32 bv_string_table_get_id(bv_string_table* tbl, const string name)
{
	for (u32 i = 0; i < tbl->count; i++)
		if (strcmp(name, tbl->strings[i]) == 0)
			return i;

	return 0xFFFFFFFF;
}

string bv_string_table_get_string(bv_string_table* tbl, u32 id)
{
	return tbl->strings[id];
}

void bv_string_table_delete(bv_string_table * tbl)
{
	for (u32 i = 0; i < tbl->count; i++)
		free(tbl->strings[i]);

	free(tbl->strings);
	free(tbl);
}
