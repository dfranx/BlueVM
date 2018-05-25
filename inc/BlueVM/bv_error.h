#ifndef __BLUEVM_ERROR_H__
#define __BLUEVM_ERROR_H__

#include <BlueVM/types.h>

/*
arguments:
	1. arg -> LEVEL (0 = error, 1 = warning, 2 = message)
	2. arg -> ID
	3. arg -> MESSAGE
	4. arg -> LINE NUMBER (-1 = unknown line number)
	5. arg -> FILE NAME (NULL = unknown file)
*/
typedef void(*bv_error_handler)(u8, u16, const bv_string, s32, const bv_string);

#endif