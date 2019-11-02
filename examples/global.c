#include <stdio.h>
#include <stdlib.h>
#include <BlueVM.h>
#include "common.h"

int main()
{
	// load bytecode
	byte* mem = load_file("global.bv");

	// parse bytecode
	bv_program* prog = bv_program_create(mem);

	// set global variable's value
	bv_program_set_global(prog, "days", bv_variable_create_int(365));

	// get main() function from loaded file
	bv_function* func_main = bv_program_get_function(prog, "main");

	// check if it exists
	if (func_main == NULL)
		printf("Program is missing function 'main'.\n");
	else {
		// main() returns glob_var_name*24
		bv_variable ret = bv_program_call(prog, func_main, NULL, NULL);

		// print the value
		printf("main() returned: %u\n", bv_variable_get_uint(ret));

		// we have to deinitialize returned variables
		bv_variable_deinitialize(&ret);
	}
	
	// we can also get their values
	bv_variable* minutes = bv_program_get_global(prog, "minutes");
	printf("minutes = %d\n", bv_variable_get_int(*minutes));

	// free memory
	bv_program_delete(prog);
	free(mem);

	return 0;
}