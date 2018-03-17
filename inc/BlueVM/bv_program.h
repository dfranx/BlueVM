#ifndef __BLUEVM_PROGRAM_H__
#define __BLUEVM_PROGRAM_H__

#include <BlueVM/bv_header.h>
#include <BlueVM/bv_block.h>
#include <BlueVM/bv_function.h>
#include <BlueVM/bv_variable.h>
#include <BlueVM/bv_stack.h>
#include <BlueVM/bv_name_list.h>
#include <BlueVM/bv_execute.h>

typedef struct bv_program {
	bv_header header;
	bv_block* block;
	bv_function** functions;

	bv_execute* opcodes;

	int external_function_count;
	bv_external_function* external_functions;
	const char** external_function_names;

	bv_stack globals;
	bv_name_list global_names;
} bv_program;

bv_program* bv_program_create(byte* mem);
void bv_program_build_opcode_table(bv_program* prog);
void bv_program_delete(bv_program* program);

u16 bv_program_get_function_count(bv_program* prog);
bv_function* bv_program_get_function(bv_program* prog, const char* str);
bv_external_function bv_program_get_ext_function(bv_program* prog, const char* str);
void bv_program_add_function(bv_program* prog, const char* name, bv_external_function* ext_func);

u16 bv_program_get_global_count(bv_program* prog);
bv_variable bv_program_get_global(bv_program* prog, string name);
void bv_program_set_global(bv_program* prog, string name, bv_variable var);

bv_variable bv_program_call(bv_program* prog, bv_function* func, bv_stack* args, bv_object* parent);

#endif