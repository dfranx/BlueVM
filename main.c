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

int main()
{
	// int i = 5;
	// i = i*i;
	// return i;
	char* mem = read_file("test.bv");

	bv_program* prog = bv_program_create(mem);
	
	bv_function* func_main = bv_program_get_function(prog, "main");
	if (func_main == NULL)
		printf("Program is missing function 'main'.\n");
	else {
		bv_variable ret = bv_program_call(prog, func_main);
		printf("main() returned: %u\n", bv_variable_get_uint(ret));
		printf("global variable 'pow': %u\n", bv_variable_get_uint(bv_program_get_global(prog, "pow")));
		bv_variable_deinitialize(&ret);
	}
	bv_program_delete(prog);

	free(mem);

	return 0;
}