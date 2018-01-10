#ifndef __BLUEVM_STRING_H__
#define __BLUEVM_STRING_H__

#include <BlueVM/types.h>

typedef struct bv_string {
	u32 length;
	u8* data;
} bv_string;

bv_string bv_string_create(const char* text);
bv_string bv_string_copy(char* text, u32 len);
bv_string bv_string_read(char* mem);
void bv_string_delete(bv_string str);
u32 bv_string_length(bv_string* str);

#endif