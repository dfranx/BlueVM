#include <BlueVM/bv_function_stepper.h>
#include <BlueVM/bv_state.h>
#include <BlueVM/bv_program.h>
#include <BlueVM/bv_opcode.h>
#include <stdlib.h>

bv_function_stepper* bv_function_stepper_create(bv_program* prog, bv_function* func, bv_object* parent, bv_stack* args)
{
	bv_function_stepper* ret = (bv_function_stepper*)malloc(sizeof(bv_function_stepper));

	ret->func = func;
	ret->result = bv_variable_create_void();
	

	ret->scope = bv_scope_create();
	bv_scope_push(ret->scope, bv_scope_type_function, func->code, prog, func, parent, 0);

	if (args != NULL && args->length == func->args) {
		// push arguments to local variables
		for (u16 i = 0; i < args->length; i++)
			bv_stack_push(&ret->scope->locals, args->data[i]);
	}

	return ret;
}

void bv_function_step(bv_function_stepper * func)
{
	// run while we still have some functions in our array
	u32 cnt = 1;
	u8 is_in = 1;
	if (func->scope->count != 0) {
		bv_state* state = bv_scope_get_state(func->scope);
		cnt = func->scope->count;

		if (cnt == func->scope->count && (is_in = ((u32)(state->code - state->func->code) < state->func->code_length))) {
			bv_opcode op = bv_opcode_read(&state->code);
			(*state->prog->opcodes[op])(func->scope);
		}

		if (!is_in && func->scope->count != 0 && cnt == func->scope->count)
			bv_scope_pop(func->scope);
	}

	if (func->scope->count == 0) {
		// get return value
		if (func->scope->stack.length > 0)
			func->result = bv_variable_copy(bv_stack_top(&func->scope->stack)); // on return, make a copy of the return value
		else
			func->result = bv_variable_create_void();
	}
}

byte bv_function_stepper_is_done(bv_function_stepper * func)
{
	return (func->scope->count == 0);
}

void bv_function_stepper_delete(bv_function_stepper * func)
{
	bv_variable_deinitialize(&func->result);
	bv_scope_delete(func->scope);
	free(func);
}

void bv_function_stepper_abort(bv_function_stepper* func)
{
	while (func->scope->count != 0)
		bv_scope_pop(func->scope);
}
