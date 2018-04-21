#ifndef __BLUEVM_SCOPE_TYPE_H__
#define __BLUEVM_SCOPE_TYPE_H__

typedef u8 bv_scope_type;

typedef enum __bv_scope_type {
	bv_scope_type_function,
	bv_scope_type_normal,
	bv_scope_type_COUNT
} __bv_scope_type;

#endif