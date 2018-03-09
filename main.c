#include <BlueVM.h>
#include <stdio.h>
#include <stdlib.h>

char* read_file(char const* path)
{
	char* buffer = 0;
	long length = 0;
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
			buffer[length] = '\0';
		}
		fclose(f);
	}

	return buffer;
}

bv_variable my_print(int count, bv_variable* args)
{
	for (int i = 0; i < count; i++) {
		if (args[i].type == bv_type_char)
			printf("%d", (int)bv_variable_get_char(args[i]));
	}

	return bv_variable_create_void();
}

int main()
{
	char* mem = read_file("test.bv");

	bv_program* prog = bv_program_create(mem);

	bv_program_add_function(prog, "print", my_print);

	bv_function* func_main = bv_program_get_function(prog, "main");
	if (func_main == NULL)
		printf("Program is missing function 'main'.\n");
	else {
		bv_variable ret = bv_program_call(prog, func_main, NULL);
		printf("\nmain() returned: %u\n", bv_variable_get_uint(ret));
		bv_variable_deinitialize(&ret);
	}
	bv_program_delete(prog);

	free(mem);

	getchar();

	return 0;
}