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

	bv_header header = bv_header_create(mem);
	printf("%.3s version %d.%d\n\n", header.signature, header.major, header.minor);


	bv_block* block = bv_block_create(mem + sizeof(header));
	bv_function_pool* fpool = block->functions;
	bv_constant_pool* cpool = block->constants;

	printf("%d functions:\n", fpool->count);
	for (int i = 0; i < fpool->count; i++)
		printf("\t%s at 0x%08x\n", fpool->names[i].data, fpool->address[i]);
	printf("\n");


	printf("u32 count: %d\n", cpool->u_count);
	for (u16 i = 0; i < cpool->u_count; i++)
		printf("\tu32[%d] = %d\n", i, cpool->u[i]);

	printf("s32 count: %d\n", cpool->s_count);
	for (u16 i = 0; i < cpool->s_count; i++)
		printf("\ts32[%d] = %d\n", i, cpool->s[i]);

	printf("float count: %d\n", cpool->f_count);
	for (u16 i = 0; i < cpool->f_count; i++)
		printf("\tfloat[%d] = %.3f\n", i, cpool->f[i]);

	printf("string count: %d\n", cpool->str_count);
	for (u16 i = 0; i < cpool->str_count; i++)
		printf("\tstring[%d] = %s\n", i, cpool->str[i].data);
	printf("\n");

	bv_block_delete(block);
	free(mem);

	return 0;
}