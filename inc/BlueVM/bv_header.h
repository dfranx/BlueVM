#ifndef __BLUEVM_HEADER_H__
#define __BLUEVM_HEADER_H__

#include <BlueVM/types.h>

typedef struct bv_header {
	u8 signature[3];
	u8 major;
	u16 minor;
	// func_pool functions;
} bv_header;

bv_header bv_header_create(byte* mem);

#endif