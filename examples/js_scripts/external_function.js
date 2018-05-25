function main() {
	var start = clock();
	var i = 0;
	for (; i < 10; i++)
		print(rand() % 100);
	
	print(clock() - start);
}
