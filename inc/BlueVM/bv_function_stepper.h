#ifndef __BLUEVM_FUNCTION_STEPPER_H__
#define __BLUEVM_FUNCTION_STEPPER_H__

#include <BlueVM/types.h>
#include <BlueVM/bv_variable.h>
#include <BlueVM/bv_scope.h>

typedef struct bv_function_stepper_s {
	bv_variable result;
	bv_scope* scope;
	bv_function* func;
} bv_function_stepper;

bv_function_stepper* bv_function_stepper_create(bv_program* prog, bv_function* func, bv_object* parent, bv_stack* args);
void bv_function_step(bv_function_stepper* func);
byte bv_function_stepper_is_done(bv_function_stepper* func);
void bv_function_stepper_delete(bv_function_stepper* func);
void bv_function_stepper_abort(bv_function_stepper* func);

#endif