#include <BlueVM.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

bv_variable my_print(u8 count, bv_variable* args)
{
	for (u8 i = 0; i < count; i++)
		if (args[i].type == bv_type_string) {
			string s = bv_variable_get_string(args[i]);
			if (strcmp(s, "\\n") == 0)
				printf("\n");
			else
				printf("%s", s);
		} else if (bv_type_is_integer(args[i].type)) {
			int n = bv_variable_get_int(args[i]);
			printf("%d", n);
		}

	return bv_variable_create_void();
}

bv_variable Animal_status(bv_object* obj, u8 count, bv_variable* args)
{
	string name = bv_variable_get_string(*bv_object_get_property(obj, "name"));
	string type = bv_variable_get_string(*bv_object_get_property(obj, "type"));
	u16 health = bv_variable_get_ushort(*bv_object_get_property(obj, "health"));

	printf("name=%s;type=%s;hp=%d;\n", name, type, health);

	return bv_variable_create_void();
}
bv_variable Vehicle_update(bv_object* obj, u8 count, bv_variable* args)
{
	bv_variable_set_string(bv_object_get_property(obj, "type"), "Jeep");
	return bv_variable_create_void();
}
bv_variable Vehicle_status(bv_object* obj, u8 count, bv_variable* args)
{
	string owner = bv_variable_get_string(*bv_object_get_property(obj, "owner"));
	string type = bv_variable_get_string(*bv_object_get_property(obj, "type"));

	printf("type=%s;owner=%s;\n", type, owner);

	return bv_variable_create_void();
}

int main()
{
#if defined(_WIN32)
	FILE *f = fopen("E:/aGen/test.bv", "rb");
#else
	FILE *f = fopen("/mnt/e/aGen/test.bv", "rb");
#endif

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  //same as rewind(f);

	char *mem = malloc((fsize + 1) * sizeof(char));
	fread(mem, fsize, 1, f);
	fclose(f);

	mem[fsize] = 0;

	if (mem == 0) {
		printf("Failed to load file!\n");
		return 0;
	}

	bv_program* prog = bv_program_create(mem);
	
	// external object
	bv_object_info* Animal = bv_object_info_create("Animal");
	bv_object_info_add_property(Animal, "health");
	bv_object_info_add_property(Animal, "name");
	bv_object_info_add_property(Animal, "type");
	bv_object_info_add_ext_method(Animal, "status", Animal_status);
	
	bv_program_add_object_info(prog, Animal);

	// 'internal' object with external method
	bv_object_info* Vehicle = bv_program_get_object_info(prog, "Vehicle");
	bv_object_info_add_ext_method(Vehicle, "update", Vehicle_update);
	bv_object_info_add_ext_method(Vehicle, "status", Vehicle_status);


	bv_program_add_function(prog, "print", my_print);
	bv_program_set_global(prog, "a", bv_variable_create_int(25));
	// bv_program_add_global(prog, "globalAnimal", bv_variable_create_object(Animal));

	bv_function* func_main = bv_program_get_function(prog, "main");
	if (func_main == NULL)
		printf("Program is missing function 'main'.\n");
	else {
		clock_t t = clock();
		bv_variable ret = bv_program_call(prog, func_main, NULL, NULL);
		printf("time: %.3f\n", ((float)clock() - t) / CLOCKS_PER_SEC);
		printf("main() returned: %u\n", bv_variable_get_uint(ret));
		bv_variable_deinitialize(&ret);
	}
	bv_program_delete(prog);

	free(mem);

	return 0;
}