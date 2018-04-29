#ifndef __BLUEVM_SCOPE_H__
#define __BLUEVM_SCOPE_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_state.h>
#include <BlueVM/bv_object.h>
#include <BlueVM/bv_function.h>
#include <BlueVM/bv_scope_type.h>

#define SCOPE_RULE 8

typedef struct bv_scope {
	u32 capacity;
	u32 count;
	u32* start_local;	// scope's start index
	u32* start_stack;	// scope's start index
	bv_scope_type* type;
	bv_stack locals;	// complete list of all variables
	bv_stack stack;
	bv_state* state;	// state[current] contains all the information about this scope
} bv_scope;

bv_scope* bv_scope_create();
void bv_scope_push(bv_scope* scp, bv_scope_type type, byte* code, bv_program* prog, bv_function* func, bv_object* obj, u8 argc);
void bv_scope_pop(bv_scope* scp);
u32 bv_scope_get_locals_start(bv_scope* scp);
u32 bv_scope_get_stack_start(bv_scope* scp);
bv_state* bv_scope_get_state(bv_scope* scp);
void bv_scope_delete(bv_scope* scp);

#endif