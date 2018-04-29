#include <BlueVM/bv_scope.h>
#include <stdlib.h>

bv_scope* bv_scope_create()
{
	bv_scope* ret = (bv_scope*)malloc(sizeof(bv_scope));

	ret->capacity = 0;
	ret->count = 0;
	ret->locals = bv_stack_create();
	ret->stack = bv_stack_create();

	// NULLs
	ret->start_local = 0;
	ret->start_stack = 0;
	ret->state = 0;
	ret->type = 0;

	return ret;
}
void bv_scope_push(bv_scope* scp, bv_scope_type type, byte* code, bv_program* prog, bv_function* func, bv_object* obj, u8 argc)
{
	scp->count++;
	u32 current = scp->count - 1;

	// reallocate only if needed
	if (scp->count >= scp->capacity) {
		scp->capacity += SCOPE_RULE;

		scp->start_local = (u32*)realloc(scp->start_local, sizeof(u32) * scp->capacity);
		scp->start_stack = (u32*)realloc(scp->start_stack, sizeof(u32) * scp->capacity);
		scp->state = (bv_state*)realloc(scp->state, sizeof(bv_state) * scp->capacity);
		scp->type = (u8*)realloc(scp->type, sizeof(u8) * scp->capacity);
	}

	scp->start_local[current] = scp->locals.length;
	for (u8 i = 0; i < argc; i++) {
		bv_stack_push(&scp->locals, bv_stack_top(&scp->stack));
		bv_stack_pop(&scp->stack);
	}
	scp->start_stack[current] = scp->stack.length;

	scp->type[current] = type;

	bv_state* state = &scp->state[current];

	if (type == bv_scope_type_function) {
		// set current bv_program
		if (prog != 0)
			state->prog = prog;
		else
			state->prog = scp->state[current - 1].prog;	// fetch the previous program

		// set current function
		state->this_func = func;
		state->code = code;

		// set current object
		state->obj = obj;
	}
	else if (type == bv_scope_type_normal) {
		bv_state* parent_state = &scp->state[current - 1];
		state->prog = parent_state->prog;
		state->this_func = parent_state->this_func;
		state->code = code;
		state->obj = parent_state->obj;
	}
}
void bv_scope_pop(bv_scope* scp)
{
	u32 current = scp->count - 1;
	scp->count--;

	// free everything
	while (scp->stack.length - scp->start_stack[current] != 0)
		bv_stack_pop_free(&scp->stack);
	while (scp->locals.length - scp->start_local[current] != 0)
		bv_stack_pop_free(&scp->locals);

	// lower down the memory allocated if needed
	if (scp->capacity - scp->count > SCOPE_RULE) {
		scp->capacity = (((scp->count / SCOPE_RULE) + 1)*SCOPE_RULE);

		scp->start_local = (u32*)realloc(scp->start_local, sizeof(u32) * scp->capacity);
		scp->start_stack = (u32*)realloc(scp->start_stack, sizeof(u32) * scp->capacity);
		scp->state = (bv_state*)realloc(scp->state, sizeof(bv_state) * scp->capacity);
		scp->type = (u8*)realloc(scp->type, sizeof(u8) * scp->capacity);
	}
}
bv_state* bv_scope_get_state(bv_scope* scp)
{
	return &scp->state[scp->count-1];
}
u32 bv_scope_get_locals_start(bv_scope* scp)
{
	u32 func_loc = scp->count-1;
	if (scp->type[func_loc] == bv_scope_type_normal) {
		do {
			func_loc--;
			if (scp->type[func_loc] == bv_scope_type_function)
				break;
		} while (func_loc != 0);
	}

	return scp->start_local[func_loc];
}
u32 bv_scope_get_stack_start(bv_scope* scp)
{
	u32 func_loc = scp->count - 1;
	if (scp->type[func_loc] == bv_scope_type_normal) {
		do {
			func_loc--;
			if (scp->type[func_loc] == bv_scope_type_function)
				break;
		} while (func_loc != 0);
	}

	return scp->start_local[func_loc];
}
void bv_scope_delete(bv_scope* scp)
{
	bv_stack_delete(&scp->stack);
	bv_stack_delete(&scp->locals);
	free(scp->start_local);
	free(scp->start_stack);
	free(scp->state);
	free(scp->type);
	free(scp);
}
