#ifndef __BLUEVM_SCOPE_TYPE_H__
#define __BLUEVM_SCOPE_TYPE_H__

typedef u8 bv_scope_type;

typedef enum __bv_scope_type {
	bv_scope_type_function,		// function call
	bv_scope_type_constructor,	// acts like a normal function but we handle arguments differently
	bv_scope_type_normal,		// for example when we enter for loop we can create new bv_scope_type_normal scope
	bv_scope_type_COUNT
} __bv_scope_type;

#endif