#ifndef __BLUEVM_STRING_TABLE_H__
#define __BLUEVM_STRING_TABLE_H__

#include <BlueVM/types.h>

typedef struct bv_string_table {
	u32 count;
	string* strings;
} bv_string_table;

bv_string_table* bv_string_table_create(byte** mem);
u32 bv_string_table_get_id(bv_string_table* tbl, const string name);
string bv_string_table_get_string(bv_string_table* tbl, u32 id);
void bv_string_table_delete(bv_string_table* tbl);

#endif