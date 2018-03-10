#ifndef __BLUEVM_OPCODE_H__
#define __BLUEVM_OPCODE_H__

#include <BlueVM/types.h>

typedef u8 bv_opcode;

// {} represents top of the stack
// [] represents parameters

typedef enum __bv_opcode {
	bv_opcode_unknown,
	bv_opcode_func_start,		// func_start
	bv_opcode_return,			// return {value}
	bv_opcode_push_stack,			// push_stack [type] [value] -> {value}
	bv_opcode_pop_stack,			// duplicate {value} -> {value, value}
	bv_opcode_const_get,			// push_const_stack [type] [index] -> {const_value[index]}
	bv_opcode_add,					// add {value1, value2} -> {value1+value2}
	bv_opcode_subtract,				// sub {value1, value2} -> {value1-value2}
	bv_opcode_multiply,				// mul {value1, value2} -> {value1*value2}
	bv_opcode_divide,				// div {value1, value2} -> {value1/value2}
	bv_opcode_increment,			// inc {value} -> {value+1}
	bv_opcode_decrement,			// dec {value} -> {value-1}
	bv_opcode_negate,				// negate {value} -> {-value}
	bv_opcode_modulo,				// modulo {value1, value2} -> {value1 % value2}
	bv_opcode_bit_or,				// bitwise or {value1, value2} -> {value1 | value2}
	bv_opcode_bit_and,				// bitwise and {value1, value2} -> {value1 & value2}
	bv_opcode_bit_not,				// bitwise not {value1} -> {~value1}
	bv_opcode_bit_xor,				// bitwise xor {value1, value2} -> {value1 ^ value2}
	bv_opcode_bit_lshift,			// bitwise lshift {value1, value2} -> {value1 << value2}
	bv_opcode_bit_rshift,			// bitwise rshift {value1, value2} -> {value1 >> value2}
	bv_opcode_equal,				// equal {value1, value2} -> {value1 == value2}
	bv_opcode_not_equal,			// not_equal {value1, value2} -> {value1 != value2}
	bv_opcode_not,					// not {value} -> {!value1}
	bv_opcode_greater,				// greater_than {value1, value2} -> {value1 > value2}
	bv_opcode_less,					// less_than {value1, value2} -> {value1 < value2}
	bv_opcode_greater_equal,		// greater_equal {value1, value2} -> {value1 >= value2}
	bv_opcode_less_equal,			// less_equal {value1, value2} -> {value1 <= value2}
	bv_opcode_nop,					// does nothing
	bv_opcode_convert,				// convert [new_type] {value} -> {new_type_value}
	bv_opcode_duplicate,			// duplicate {value} -> {value, value}
	bv_opcode_swap,					// swap {value1, value2} -> {value2, value1}
	bv_opcode_get_local,			// get_local [index]
	bv_opcode_set_local,			// set_local [index] {value} -> {}			NOTE: stack if local at given index is array: { value, ind0, ..., indN } 
	bv_opcode_get_global,			// get_global [index]
	bv_opcode_set_global,			// set_global [index] {value} -> {}
	bv_opcode_new_array,			// new_array [dim] { size0, ..., sizeN } -> { }
	bv_opcode_call,					// call [name] {arg1, arg2 ... argN} -> {}
	bv_opcode_call_return,			// call_return [name] {arg1, arg2, ... argN} -> {return_value}
	bv_opcode_is_type_of,			// is_type [type] {value} -> {false/true}
	bv_opcode_if,					// if [address_if_false] {value} -> {}
	bv_opcode_goto,					// goto [address]
	bv_opcode_new_object,			// new_object [id] {} -> {object}
	bv_opcode_set_prop,				// set_prop [propid] {object, value} -> {object}
	bv_opcode_set_my_prop,			// set_my_prop [propid] {value} -> {}
	bv_opcode_get_prop,				// get_prop [propid] {object} -> {porperty_value}
	bv_opcode_get_my_prop,			// get_my_prop [propid] {} -> {property_value}
	bv_opcode_call_method,			// call_method [methodid] {object} -> {}
	bv_opcode_call_my_method,		// call_my_method [methodid] {} -> {}
	bv_opcode_call_ret_method,		// call_method [methodid] {object} -> {return_value}
	bv_opcode_call_ret_my_method,	// call_my_method [methodid] {} -> {return_value}
	bv_opcode_COUNT					// this is not an actual opcode
} __bv_opcode;

bv_opcode bv_opcode_read(byte** mem);

#endif