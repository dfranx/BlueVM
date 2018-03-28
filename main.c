#include <BlueVM.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <BlueVM/bv_object.h>

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
	for (int i = 0; i < count; i++)
		if (args[i].type == bv_type_string) {
			string s = bv_variable_get_string(args[i]);
			printf("%s", s);
		} else if (args[i].type == bv_type_int) {
			int n = bv_variable_get_int(args[i]);
			printf("%d", n);
		}

	return bv_variable_create_void();
}

int main()
{
#ifdef WIN32
	char* mem = read_file("E:/aGen/test.bv");
#else
	char* mem = read_file("/mnt/e/aGen/test.bv");
#endif

	if (mem == 0) {
		printf("Failed to load file!\n");
		return 0;
	}

	bv_program* prog = bv_program_create(mem);

	bv_program_add_function(prog, "print", my_print);
	bv_program_set_global(prog, "a", bv_variable_create_int(14));

	bv_function* func_main = bv_program_get_function(prog, "main");
	if (func_main == NULL)
		printf("Program is missing function 'main'.\n");
	else {
		clock_t t = clock();
		bv_variable ret = bv_program_call(prog, func_main, NULL, NULL);
		printf("time: %.2f\n", ((float)clock() - t) / CLOCKS_PER_SEC);
		printf("main() returned: %u\n", bv_variable_get_uint(ret));
		bv_variable_deinitialize(&ret);
	}
	bv_program_delete(prog);

	free(mem);

#ifdef WIN32
	getchar();
#endif

	return 0;
}