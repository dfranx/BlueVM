#include <BlueVM/bv_name_list.h>
#include <stdlib.h>
#include <string.h>

bv_name_list bv_name_list_create(byte** mem)
{
	bv_name_list ret;
	ret.name_count = u16_read(mem);

	ret.names = (bv_string*)malloc(sizeof(bv_string)*ret.name_count);

	for (u16 i = 0; i < ret.name_count; i++)
		ret.names[i] = string_read(mem);

	return ret;
}
bv_name_list bv_name_list_create_empty()
{
	bv_name_list ret;
	ret.name_count = 0;
	ret.names = (bv_string*)malloc(ret.name_count * sizeof(bv_string));

	return ret;
}

u16 bv_name_list_get_id(bv_name_list nlist, const bv_string name)
{
	for (u16 i = 0; i < nlist.name_count; i++)
		if (strcmp(name, nlist.names[i]) == 0)
			return i;
	return nlist.name_count;
}

void bv_name_list_add(bv_name_list* nlist, const bv_string name)
{
	nlist->name_count++;
	nlist->names = (bv_string*)realloc(nlist->names, nlist->name_count * sizeof(bv_string));

	size_t name_len = strlen(name);
	bv_string name_ptr = nlist->names[nlist->name_count - 1];
	
	name_ptr = (bv_string)malloc((name_len+1) * sizeof(char));
	memcpy(name_ptr, name, name_len);

	name_ptr[name_len - 1] = 0;

	nlist->names[nlist->name_count - 1] = name_ptr;
}

void bv_name_list_delete(bv_name_list * nlist)
{
	for (u16 i = 0; i < nlist->name_count; i++)
		free(nlist->names[i]);
	free(nlist->names);
	nlist->name_count = 0;
}
