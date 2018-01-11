#ifndef __BLUEVM_TYPE_H__
#define __BLUEVM_TYPE_H__

#include <BlueVM/types.h>

typedef u8 bv_type;

typedef enum __bv_type {
	bv_type_void,	// void
	bv_type_int,	// s32
	bv_type_short,	// s16
	bv_type_char,	// s8
	bv_type_uint,	// u32
	bv_type_ushort,	// u16
	bv_type_uchar,	// u8
	bv_type_float,	// float
	bv_type_string	// bv_string
} __bv_type;

bv_type bv_type_create_array(enum __bv_type type);
u8 bv_type_is_array(bv_type var);
bv_type bv_type_get(bv_type var);
bv_type bv_type_read(byte* mem);

#endif