#include <stdio.h>
#include <stdlib.h>
#include <BlueVM.h>
#include "common.h"

// simple console.log implementation
bv_variable console_log(bv_object* obj, u8 count, bv_variable* args)
{
	for (u8 i = 0; i < count; i++) {
		bv_variable val = args[i];

		if (val.type == bv_type_pointer)
			val = *(bv_variable*)val.value;

		if (val.type == bv_type_string) {
			bv_string s = bv_variable_get_string(val);
			printf("%s", s);
		}
		else if (bv_type_is_integer(val.type)) {
			int n = bv_variable_get_int(val);
			printf("%d", n);
		}
	}

	printf("\n");

	return bv_variable_create_void();
}

int main()
{
	// load bytecode

	byte* mem = load_file("external_object.bv");

	// parse bytecode
	bv_program* prog = bv_program_create(mem);

	// create new object that contains information about __ImplConsole
	bv_object_info* impl_console = bv_object_info_create("__ImplConsole");
	bv_object_info_add_ext_method(impl_console, "log", console_log); // add method log
	bv_program_add_object_info(prog, impl_console); // NOTE: remove this line if you want to prevent users from defining their own __ImplConsole objects

	// define __ImplConsole class named 'console'
	bv_program_add_global(prog, "console");
	bv_program_set_global(prog, "console", bv_variable_create_object(impl_console));

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