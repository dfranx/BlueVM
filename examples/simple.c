#include <stdio.h>
#include <stdlib.h>
#include <BlueVM.h>
#include "common.h"

int main()
{
	// load bytecode
	byte* mem = load_file("simple.bv");

	// parse bytecode
	bv_program* prog = bv_program_create(mem);

	// get main() function from loaded file
	bv_function* func_main = bv_program_get_function(prog, "main");

	// check if it exists
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