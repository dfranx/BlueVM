#include <stdio.h>
#include <stdlib.h>
#include <BlueVM.h>
#include <math.h>
#include "common.h"

float parse_arg(bv_variable val) {
	float in = 0;

	if (val.type == bv_type_pointer)
		val = *bv_variable_get_pointer(val);

	if (val.type == bv_type_float)
		in = bv_variable_get_float(val);
	else
		in = bv_variable_get_int(val);

	return in;
}

// Math.sin, Math.cos and other implementations
bv_variable math_sin(bv_object* obj, u8 count, bv_variable* args)
{
	float in = parse_arg(args[0]);
	return bv_variable_create_float(sin(in));
}
bv_variable math_cos(bv_object* obj, u8 count, bv_variable* args)
{
	float in = parse_arg(args[0]);
	return bv_variable_create_float(cos(in));
}
bv_variable math_tan(bv_object* obj, u8 count, bv_variable* args)
{
	float in = parse_arg(args[0]);
	return bv_variable_create_float(tan(in));
}
bv_variable math_floor(bv_object* obj, u8 count, bv_variable* args)
{
	float in = parse_arg(args[0]);
	return bv_variable_create_float(floor(in));
}
bv_variable math_abs(bv_object* obj, u8 count, bv_variable* args)
{
	float in = parse_arg(args[0]);
	return bv_variable_create_float(abs(in));
}
bv_variable math_sqrt(bv_object* obj, u8 count, bv_variable* args)
{
	float in = parse_arg(args[0]);
	return bv_variable_create_float(sqrt(in));
}
bv_variable math_pow(bv_object* obj, u8 count, bv_variable* args)
{
	float in = parse_arg(args[0]);
	float pw = parse_arg(args[1]);
	return bv_variable_create_float(pow(in, pw));
}
bv_variable my_print(u8 count, bv_variable* args)
{
	for (int i = 0; i < count; i++) {
		if (args[i].type == bv_type_int)
			printf("%d", bv_variable_get_int(args[i]));
		else if (args[i].type == bv_type_float)
			printf("%.2f", bv_variable_get_float(args[i]));
		else if (args[i].type == bv_type_string)
			printf("%s", bv_variable_get_string(args[i]));
	}

	printf("\n");

	return bv_variable_create_void();
}

int main()
{
	// load bytecode
	byte* mem = load_file("library.bv");

	// parse bytecode
	bv_program* prog = bv_program_create(mem);

	// create Math class implementation
	bv_object_info* obji_math = bv_object_info_create("__ImplMath");
	bv_object_info_add_property(obji_math, "PI");
	bv_object_info_add_ext_method(obji_math, "sin", math_sin);
	bv_object_info_add_ext_method(obji_math, "cos", math_cos);
	bv_object_info_add_ext_method(obji_math, "tan", math_tan);
	bv_object_info_add_ext_method(obji_math, "floor", math_floor);
	bv_object_info_add_ext_method(obji_math, "abs", math_abs);
	bv_object_info_add_ext_method(obji_math, "sqrt", math_sqrt);
	bv_object_info_add_ext_method(obji_math, "pow", math_pow);

	// declare Math
	bv_variable var_math = bv_variable_create_object(obji_math);
	bv_object* obj_math = bv_variable_get_object(var_math);

	// set PI
	bv_object_set_property(obj_math, "PI", bv_variable_create_float(3.14f));

	// add Math to the library
	bv_library* libmath = bv_library_create();
	
	// add math declaration
	bv_library_add_global(libmath, "Math", var_math);
	bv_library_add_function(libmath, "print", my_print);

	// add library to the program
	bv_program_add_library(prog, libmath);
	// ***NOTE***: now you can simply add it to any bv_program you want to: bv_program_add_library(other_prog, libmath); without having to define the objects again


	// get functions from loaded file
	bv_function* func_main = bv_program_get_function(prog, "main");

	// check if the function exists
	if (func_main == NULL)
		printf("Program is missing function 'main'.\n");
	else {
		// call function and store its return value
		bv_variable ret = bv_program_call(prog, func_main, NULL, NULL);

		// print the value
		printf("main() returned: %u\n", bv_variable_get_uint(ret));

		// we have to deinitialize returned variables
		bv_variable_deinitialize(&ret);
	}

	// free memory
	bv_program_delete(prog);
	free(mem);

	return 0;
}