#include <BlueVM.h>
#include <stdio.h>
#include <memory.h>

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
	char* mem = read_file("test.bv");

	bv_program* prog = bv_program_create(mem);

	bv_function* func_main = bv_program_get_function(prog, "main");
	if (func_main != 0)
		printf("main code length: %d\n", func_main->op_length);
	else
		printf("function main() not found.\n");

	bv_program_delete(prog);
	free(mem);

	return 0;
}