#ifndef __BLUEVM_EXAMPLES_COMMON_H__
#define __BLUEVM_EXAMPLES_COMMON_H__

#include <stdio.h>

char* load_file(const char* fname) {
	FILE *f = fopen(fname, "rb");
	if (f == 0) {
		printf("Failed to load file %s\n", fname);
		return 0;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  //same as rewind(f);

	char *mem = (char*)malloc((fsize + 1) * sizeof(char));
	fread(mem, fsize, 1, f);
	fclose(f);

	mem[fsize] = 0;

	return mem;
}

#endif // __BLUEVM_EXAMPLES_COMMON_H__