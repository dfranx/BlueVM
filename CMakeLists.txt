cmake_minimum_required(VERSION 3.0)

project(BlueVM)

include_directories(inc)

add_library(BlueVM STATIC
	src/bv_array.c
	src/bv_block.c
	src/bv_constant_pool.c
	src/bv_function.c
	src/bv_function_pool.c
	src/bv_function_stepper.c
	src/bv_string_table.c
	src/bv_object_info.c
	src/bv_object_pool.c
	src/bv_library.c
	src/bv_object.c
	src/bv_opcode.c
	src/bv_header.c
	src/bv_name_list.c
	src/bv_execute.c
	src/bv_program.c
	src/bv_stack.c
	src/bv_scope.c
	src/bv_type.c
	src/bv_variable.c
	src/types.c
)

if (LINK_LIBM)
	target_link_libraries(BlueVM m)
endif()

if (BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()