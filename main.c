#include <BlueVM.h>
#include <stdio.h>
#include <stdlib.h>

char* read_file(char const* path)
{
	char* buffer = 0;
	long length;
	FILE * f = fopen(path, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char*)malloc((length + 1) * sizeof(char));

		if (buffer)
		{
			fread(buffer, sizeof(char), length, f);
		}
		fclose(f);
	}
	buffer[length] = '\0';

	return buffer;
}

void decode(bv_program* prog)
{
	bv_constant_pool* cpool = prog->block->constants;

	for (u8 i = 0; i < cpool->type_count; i++) {
		for (u16 j = 0; j < cpool->val_count[i]; j++) {
			printf("const_add 0x%02x ", cpool->val_type[i]);
			if (cpool->val_type[i] == bv_type_string)
				printf("%s", bv_variable_get_string(cpool->val[i][j]));
			else if (cpool->val_type[i] == bv_type_int)
				printf("%d", bv_variable_get_uint(cpool->val[i][j]));
			else if (cpool->val_type[i] == bv_type_float)
				printf("%f", bv_variable_get_float(cpool->val[i][j]));
			// [TODO] etc...
			printf("\n");
		}
	}


	bv_function_pool* fpool = prog->block->functions;
	for (u16 i = 0; i < fpool->count; i++) {
		printf("func %s\n", fpool->names[i]);
		
		bv_function* func = bv_program_get_function(prog, fpool->names[i]);
		printf("\tset_return 0x%02x\n", func->return_type);
		for (u8 j = 0; j < func->args; j++)
			printf("\targ_add 0x%02x\n", func->arg_type[j]);
		printf("\t[CODE]\n");
		for (u32 j = 0; j < func->code_length; j++)
			printf("\t\t0x%02x\n", func->code[j]);
		printf("\t[/CODE]\n");
	}
}

int main()
{
	char* mem = read_file("test.bv");

	bv_program* prog = bv_program_create(mem);
	
	printf("BlueVM %d.%d\n", prog->header.major, prog->header.minor);
	printf("DECODE\n----------------------------------------------\n");
	decode(prog);
	printf("----------------------------------------------\n");

	bv_function* func_main = bv_program_get_function(prog, "main");
	if (func_main == NULL)
		printf("Program is missing function 'main'.\n");
	else {
		bv_variable ret = bv_program_call(prog, func_main);
		printf("main() returned: %d\n", bv_variable_get_int(ret));
		bv_variable_deinitialize(&ret);
	}
	bv_program_delete(prog);

	free(mem);

	return 0;
}