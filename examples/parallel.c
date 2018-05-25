#include <stdio.h>
#include <stdlib.h>
#include <BlueVM.h>
#include "common.h"

// super simple print function that only accepts one argument (string)
bv_variable print(u8 count, bv_variable* args)
{
	if (count == 1 && args[0].type == bv_type_string)
			printf("%s\n", bv_variable_get_string(args[0]));
	return bv_variable_create_void();
}

int main()
{
	// load bytecode
	byte* mem = load_file("parallel.bv");

	// parse bytecode
	bv_program* prog = bv_program_create(mem);

	// bind print()
	bv_program_add_function(prog, "print", print);

	// get functions from loaded file
	bv_function* func1 = bv_program_get_function(prog, "func1");
	bv_function* func2 = bv_program_get_function(prog, "func2");

	// create function stepper (object that executes one opcode each bv_function_step call)
	bv_function_stepper* stepper1 = bv_function_stepper_create(prog, func1, NULL, NULL);
	bv_function_stepper* stepper2 = bv_function_stepper_create(prog, func2, NULL, NULL);

	u8 done1 = 0, done2 = 0;
	do {
		// check if we are done executing function
		done1 = bv_function_stepper_is_done(stepper1);
		done2 = bv_function_stepper_is_done(stepper2);

		// execute one opcode from each function
		if (!done1) bv_function_step(stepper1);
		if (!done2) bv_function_step(stepper2);
	} while (!done1 || !done2); // check if we are done

	// free memory
	bv_program_delete(prog);
	free(mem);

	return 0;
}