#include <BlueVM/bv_name_list.h>

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

u16 bv_name_list_get_id(bv_name_list nlist, string name)
{
	for (u16 i = 0; i < nlist.name_count; i++)
		if (strcmp(nlist.names[i], name) == 0)
			return nlist.name_ids[i];
	return 0;
}

void bv_name_list_delete(bv_name_list * nlist)
{
	for (u16 i = 0; i < nlist->name_count; i++)
		free(nlist->names[i]);
	free(nlist->names);
	free(nlist->name_ids);
	nlist->name_count = 0;
}
