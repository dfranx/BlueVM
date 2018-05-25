#ifndef __BLUEVM_PROGRAM_H__
#define __BLUEVM_PROGRAM_H__

#include <BlueVM/bv_header.h>
#include <BlueVM/bv_block.h>
#include <BlueVM/bv_function.h>
#include <BlueVM/bv_variable.h>
#include <BlueVM/bv_stack.h>
#include <BlueVM/bv_name_list.h>
#include <BlueVM/bv_string_table.h>
#include <BlueVM/bv_execute.h>
#include <BlueVM/bv_error.h>
#include <BlueVM/bv_debug.h>
#include <BlueVM/bv_library.h>

typedef struct bv_program {
	bv_header header;
	bv_string_table* string_table;
	bv_block* block;
	bv_function** functions;

	bv_execute* opcodes;

	u16 external_function_count;
	bv_external_function* external_functions;
	bv_string* external_function_names;

	bv_library** libraries;
	u8 lib_count;

	bv_stack globals;
	bv_name_list global_names;

	bv_string current_file;
	s32 current_line;
	bv_error_handler error;
	bv_breakpoint_handler debugger;
} bv_program;

bv_program* bv_program_create(byte* mem);
void bv_program_delete(bv_program* program);

void bv_program_set_error_handler(bv_program* prog, bv_error_handler errh);
void bv_program_set_breakpoint_handler(bv_program* prog, bv_breakpoint_handler dbgh);
void bv_program_error(bv_program* prog, u8 lvl, u16 id, const bv_string msg);

u16 bv_program_get_function_count(bv_program* prog);
bv_function* bv_program_get_function(bv_program* prog, const bv_string str);
bv_external_function bv_program_get_ext_function(bv_program* prog, const bv_string str);
void bv_program_add_function(bv_program* prog, const bv_string name, bv_external_function ext_func);
void bv_program_add_library(bv_program* prog, bv_library* lib);

void bv_program_add_object_info(bv_program* prog, bv_object_info* obj);
bv_object_info* bv_program_get_object_info(bv_program* prog, const bv_string name);

u16 bv_program_get_global_count(bv_program* prog);
bv_variable* bv_program_get_global(bv_program* prog, bv_string name);
void bv_program_add_global(bv_program* prog, bv_string name);
void bv_program_set_global(bv_program* prog, const bv_string name, bv_variable var);

bv_variable bv_program_call(bv_program* prog, bv_function* func, bv_stack* args, bv_object* parent);

#endif