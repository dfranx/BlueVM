#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <BlueVM.h>
#include "common.h"

// super simple print function that only accepts one argument (int)
bv_variable my_print(u8 count, bv_variable* args)
{
	if (count == 1 && args[0].type == bv_type_int)
		printf("%d\n", bv_variable_get_int(args[0]));
	return bv_variable_create_void();
}

// clock() wrapper
bv_variable my_clock(u8 count, bv_variable* args)
{
	return bv_variable_create_int(clock());
}

// rand() wrapper
bv_variable my_rand(u8 count, bv_variable* args)
{
	return bv_variable_create_int(rand());
}

int main()
{
	// load bytecode

	byte* mem = load_file("external_function.bv");

	// parse bytecode
	bv_program* prog = bv_program_create(mem);

	// bind c function print()
	bv_program_add_function(prog, "print", my_print);
	bv_program_add_function(prog, "clock", my_clock);
	bv_program_add_function(prog, "rand", my_rand);

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