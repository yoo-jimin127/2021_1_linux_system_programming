// 리눅스 시스템 프로그래밍_실습과제2_3번 문제_20193017_유지민

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define MAX_SIZE 999

int main (int argc, char* argv[]) {
	int iter_num = 0;
	FILE *fptr = NULL;
	char filename[30] = "rand_test.txt";
	char sorted_filename[30] = "sorted_test.txt";
	int rand_num = 0;
	
	if (argc < 2) {
		fprintf(stderr, "usage : ./hw2_3_20193017 <iter_num>\n");
		exit(1);
	}

	iter_num = atoi(argv[1]);

	if ((fptr = fopen(filename, "wt")) == NULL) {
		fprintf(stderr, "<filename> fopen() error\n");
		exit(1);
	}

	srand(time(NULL)); //난수의 중복 방지
	for (int i = 0; i < iter_num; i++) {
		rand_num = rand() % iter_num;
		fwrite(&rand_num, sizeof(int), 1, fptr);
	}

	fclose(fptr);

	if ((fptr = fopen(sorted_filename, "wt")) == NULL) {
		fprintf(stderr, "<sorted_filename> fopen() error\n");
		exit(1);
	}

	fclose(fptr);
	return 0;
}
