#include <BlueVM/bv_name_list.h>
#include <stdlib.h>
#include <string.h>

bv_name_list bv_name_list_create(byte** mem)
{
	bv_name_list ret;
	ret.name_count = u16_read(mem);

	ret.names = malloc(sizeof(string)*ret.name_count);
	ret.name_ids = malloc(sizeof(u16)*ret.name_count);

	for (u16 i = 0; i < ret.name_count; i++) {
		ret.name_ids[i] = u16_read(mem);
		ret.names[i] = string_read(mem);
	}

	return ret;
}
bv_name_list bv_name_list_create_empty()
{
	bv_name_list ret;
	ret.name_count = 0;
	ret.names = malloc(ret.name_count * sizeof(string));
	ret.name_ids = 0;

	return ret;
}

u16 bv_name_list_get_id(bv_name_list nlist, string name)
{
	for (u16 i = 0; i < nlist.name_count; i++)
		if (strcmp(nlist.names[i], name) == 0)
			return nlist.name_ids[i];
	return 0;
}

void bv_name_list_add(bv_name_list* nlist, const string name)
{
	nlist->name_count++;
	nlist->names = realloc(nlist->names, nlist->name_count * sizeof(string));

	size_t name_len = strlen(name);
	string name_ptr = nlist->names[nlist->name_count - 1];
	
	name_ptr = malloc((name_len+1) * sizeof(char));
	memcpy(name_ptr, name, name_len);

	name_ptr[name_len - 1] = 0;

	nlist->names[nlist->name_count - 1] = name_ptr;
}

void bv_name_list_delete(bv_name_list * nlist)
{
	for (u16 i = 0; i < nlist->name_count; i++)
		free(nlist->names[i]);
	free(nlist->names);
	free(nlist->name_ids);
	nlist->name_count = 0;
}
