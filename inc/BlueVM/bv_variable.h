#ifndef __BLUEVM_VARIABLE_H__
#define __BLUEVM_VARIABLE_H__

#include <BlueVM/bv_type.h>

typedef struct bv_array_s bv_array;
typedef struct bv_object_s bv_object;
typedef struct bv_object_info_s bv_object_info;
typedef struct bv_function_s bv_function;
typedef struct bv_program_s bv_program;

typedef struct bv_variable_s {
	bv_type type;
	void* value;
} bv_variable;

s32 bv_variable_get_int(bv_variable var);
u32 bv_variable_get_uint(bv_variable var);
s16 bv_variable_get_short(bv_variable var);
u16 bv_variable_get_ushort(bv_variable var);
s8 bv_variable_get_char(bv_variable var);
u8 bv_variable_get_uchar(bv_variable var);
float bv_variable_get_float(bv_variable var);
bv_string bv_variable_get_string(bv_variable var);
bv_array* bv_variable_get_array(bv_variable var);
bv_object* bv_variable_get_object(bv_variable var);
bv_variable* bv_variable_get_pointer(bv_variable var);
bv_function* bv_variable_get_function(bv_variable var);

bv_variable bv_variable_create(bv_type type, void* value);
bv_variable bv_variable_create_void();
bv_variable bv_variable_create_int(s32 var);
bv_variable bv_variable_create_uint(u32 var);
bv_variable bv_variable_create_short(s16 var);
bv_variable bv_variable_create_ushort(u16 var);
bv_variable bv_variable_create_char(s8 var);
bv_variable bv_variable_create_uchar(u8 var);
bv_variable bv_variable_create_float(float var);
bv_variable bv_variable_create_string(const bv_string var);
bv_variable bv_variable_create_array(bv_array var);
bv_variable bv_variable_create_object(bv_object_info* var);
bv_variable bv_variable_create_null_object();
bv_variable bv_variable_create_pointer(bv_variable* var);
bv_variable bv_variable_create_function(bv_function* func);

void bv_variable_set_int(bv_variable* var, s32 val);
void bv_variable_set_uint(bv_variable* var, u32 val);
void bv_variable_set_short(bv_variable* var, s16 val);
void bv_variable_set_ushort(bv_variable* var, u16 val);
void bv_variable_set_char(bv_variable* var, s8 val);
void bv_variable_set_uchar(bv_variable* var, u8 val);
void bv_variable_set_float(bv_variable* var, float val);
void bv_variable_set_string(bv_variable* var, const bv_string val);
void bv_variable_set_array(bv_variable* var, bv_array val);
void bv_variable_set_object(bv_variable* var, bv_object* val);
void bv_variable_set_pointer(bv_variable* var, bv_variable* val);
void bv_variable_set_function(bv_variable* var, bv_function* val);


void bv_variable_deinitialize(bv_variable* var);
bv_variable bv_variable_copy(bv_variable var);
bv_variable bv_variable_read(byte** mem, bv_type type);

// operators, casting and setting value
u8 bv_variable_op_equal(bv_program* prog, bv_variable left, bv_variable right);
u8 bv_variable_op_not_equal(bv_program* prog, bv_variable left, bv_variable right);
u8 bv_variable_op_greater_than(bv_program* prog, bv_variable left, bv_variable right);
u8 bv_variable_op_greater_equal(bv_program* prog, bv_variable left, bv_variable right);
u8 bv_variable_op_less_than(bv_program* prog, bv_variable left, bv_variable right);
u8 bv_variable_op_less_equal(bv_program* prog, bv_variable left, bv_variable right);
void bv_variable_op_assign(bv_program* prog, bv_variable* left, bv_variable right);
bv_variable bv_variable_op_add(bv_program* prog, bv_variable left, bv_variable right);
bv_variable bv_variable_op_subtract(bv_program* prog, bv_variable left, bv_variable right);
bv_variable bv_variable_op_divide(bv_program* prog, bv_variable left, bv_variable right);
bv_variable bv_variable_op_multiply(bv_program* prog, bv_variable left, bv_variable right);
void bv_variable_op_increment(bv_program* prog, bv_variable* left);
void bv_variable_op_decrement(bv_program* prog, bv_variable* left);
bv_variable bv_variable_op_negate(bv_program* prog, bv_variable left);
bv_variable bv_variable_op_modulo(bv_program* prog, bv_variable left, bv_variable right);
u8 bv_variable_op_not(bv_program* prog, bv_variable left);
bv_variable bv_variable_cast(bv_type new_type, bv_variable right);

#endif