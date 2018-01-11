#ifndef __BLUEVM_VARIABLE_H__
#define __BLUEVM_VARIABLE_H__

#include <BlueVM/bv_type.h>

typedef struct bv_variable {
	bv_type type;
	void* value;
} bv_variable;

s32 bv_variable_get_int(bv_variable var);
u32 bv_variable_get_uint(bv_variable var);
s16 bv_variable_get_short(bv_variable var);
u16 bv_variable_get_ushort(bv_variable var);
s8 bv_variable_get_char(bv_variable var);
u8 bv_variable_get_uchar(bv_variable var);
float bv_variable_get_float(bv_variable var);
string bv_variable_get_string(bv_variable var);

bv_variable bv_variable_create_int(s32 var);
bv_variable bv_variable_create_uint(u32 var);
bv_variable bv_variable_create_short(s16 var);
bv_variable bv_variable_create_ushort(u16 var);
bv_variable bv_variable_create_char(s8 var);
bv_variable bv_variable_create_uchar(u8 var);
bv_variable bv_variable_create_float(float var);
bv_variable bv_variable_create_string(string var);

void bv_variable_set_int(bv_variable* var, s32 val);
void bv_variable_set_uint(bv_variable* var, u32 val);
void bv_variable_set_short(bv_variable* var, s16 val);
void bv_variable_set_ushort(bv_variable* var, u16 val);
void bv_variable_set_char(bv_variable* var, s8 val);
void bv_variable_set_uchar(bv_variable* var, u8 val);
void bv_variable_set_float(bv_variable* var, float val);
void bv_variable_set_string(bv_variable* var, string val);

void bv_variable_deinitialize(bv_variable* var);
bv_variable bv_variable_read(byte** mem, bv_type type);

#endif