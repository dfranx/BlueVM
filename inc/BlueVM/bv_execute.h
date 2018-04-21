#ifndef __BLUEVM_EXECUTE_H__
#define __BLUEVM_EXECUTE_H__
#include <BlueVM/bv_scope.h>

typedef void(*bv_execute)(bv_scope* scope);

void bv_execute_unknown(bv_scope* scope);
void bv_execute_func_start(bv_scope* scope);
void bv_execute_return(bv_scope* scope);
void bv_execute_push_stack(bv_scope* scope);
void bv_execute_pop_stack(bv_scope* scope);
void bv_execute_const_get(bv_scope* scope);
void bv_execute_add(bv_scope* scope);
void bv_execute_subtract(bv_scope* scope);
void bv_execute_multiply(bv_scope* scope);
void bv_execute_divide(bv_scope* scope);
void bv_execute_increment(bv_scope* scope);
void bv_execute_decrement(bv_scope* scope);
void bv_execute_negate(bv_scope* scope);
void bv_execute_modulo(bv_scope* scope);
void bv_execute_bit_or(bv_scope* scope);
void bv_execute_bit_and(bv_scope* scope);
void bv_execute_bit_not(bv_scope* scope);
void bv_execute_bit_xor(bv_scope* scope);
void bv_execute_bit_lshift(bv_scope* scope);
void bv_execute_bit_rshift(bv_scope* scope);
void bv_execute_bool_or(bv_scope* scope);
void bv_execute_bool_and(bv_scope* scope);
void bv_execute_equal(bv_scope* scope);
void bv_execute_not_equal(bv_scope* scope);
void bv_execute_not(bv_scope* scope);
void bv_execute_greater(bv_scope* scope);
void bv_execute_less(bv_scope* scope);
void bv_execute_greater_equal(bv_scope* scope);
void bv_execute_less_equal(bv_scope* scope);
void bv_execute_nop(bv_scope* scope);
void bv_execute_convert(bv_scope* scope);
void bv_execute_duplicate(bv_scope* scope);
void bv_execute_swap(bv_scope* scope);
void bv_execute_get_local(bv_scope* scope);
void bv_execute_set_local(bv_scope* scope);
void bv_execute_get_global(bv_scope* scope);
void bv_execute_set_global(bv_scope* scope);
void bv_execute_push_array(bv_scope* scope);
void bv_execute_set_array_el(bv_scope* scope);
void bv_execute_get_array_el(bv_scope* scope);
void bv_execute_call(bv_scope* scope);
void bv_execute_call_return(bv_scope* scope);
void bv_execute_is_type_of(bv_scope* scope);
void bv_execute_if(bv_scope* scope);
void bv_execute_goto(bv_scope* scope);
void bv_execute_new_object(bv_scope* scope);
void bv_execute_set_prop(bv_scope* scope);
void bv_execute_set_my_prop(bv_scope* scope);
void bv_execute_get_prop(bv_scope* scope);
void bv_execute_get_my_prop(bv_scope* scope);
void bv_execute_call_method(bv_scope* scope);
void bv_execute_call_my_method(bv_scope* scope);
void bv_execute_call_ret_method(bv_scope* scope);
void bv_execute_call_ret_my_method(bv_scope* scope);
void bv_execute_scope_start(bv_scope* scope);
void bv_execute_scope_end(bv_scope* scope);

#endif // __BLUEVM_EXECUTE_H__