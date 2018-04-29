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
	bv_type_string,	// bv_string
	bv_type_array,	// bv_array
	bv_type_object,	// bv_object
	bv_type_pointer,// C99 pointer to bv_variable
} __bv_type;

#define bv_type_read u8_read

byte bv_type_is_integer(bv_type type);
bv_type bv_type_get(bv_type t1, bv_type t2); // 1.2f + 2 => returns float

#endif