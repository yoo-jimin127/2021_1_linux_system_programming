#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
	int num = atoi(argv[1]);
	char filename[30] = "test_test.txt";
	FILE *fptr = NULL;
	
	fptr = fopen(filename, "w");

	int tmp = 5;
	char arr[1];

	arr = 
	fwrite(&tmp, sizeof(int), 1, fptr);

	fclose(fptr);
	srand(time(NULL));

	for (int i = 0; i < num; i++) {
		int rand_num = rand() % num;
		printf("%d\n", rand_num);
	}

	return 0;
}
