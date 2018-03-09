cmake_minimum_required(VERSION 3.9)

project(BlueVM)

include_directories(${PROJECT_SOURCE_DIR}/inc)

add_executable(BlueVM 
	main.c
	src/bv_array.c
	src/bv_block.c
	src/bv_constant_pool.c
	src/bv_function.c
	src/bv_function_pool.c
	src/bv_header.c
	src/bv_name_list.c
	src/bv_opcode.c
	src/bv_program.c
	src/bv_stack.c
	src/bv_type.c
	src/bv_variable.c
	src/types.c
)