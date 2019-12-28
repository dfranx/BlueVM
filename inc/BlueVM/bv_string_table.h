#ifndef __BLUEVM_STRING_TABLE_H__
#define __BLUEVM_STRING_TABLE_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_header.h>

typedef struct bv_string_table_s {
	u32 count;
	bv_string* strings;
} bv_string_table;

bv_string_table* bv_string_table_create(bv_header header, byte** mem);
u32 bv_string_table_get_id(bv_string_table* tbl, const bv_string name);
bv_string bv_string_table_get_string(bv_string_table* tbl, u32 id);
void bv_string_table_delete(bv_string_table* tbl);

#endif