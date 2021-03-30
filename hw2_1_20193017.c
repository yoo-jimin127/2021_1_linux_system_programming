// 리눅스 시스템 프로그래밍_실습과제2_1번 문제_20193017_유지민

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char * argv[]) {
	char *filename;
	FILE *fptr = NULL;
	int iter_num = 0;
	char symbol[1] = "";
	char enter[1] = "\n";

	if (argc < 4) {
		fprintf(stderr, "usage : ./hw2_1_20193017 <iter_num> <symbol> <filename>\n");
		exit(1);
	}

	for (int i = 0; i < argc; i++) {
		printf("argv[%d] : %s\n", i, argv[i]);
	}

	filename = (char *)malloc(sizeof(char) * 100);

	iter_num = atoi(argv[1]);
	strcpy(symbol, argv[2]);
	strcpy(filename, argv[3]);

	if ((fptr = fopen(filename, "w")) == NULL) {
		fprintf(stderr, "<filename> fopen() error\n");
		exit(1);
	}

	if (iter_num > 0) {
		for (int i = 1; i <= iter_num; i++) {
			for (int j = 1; j <= i; j++) {
				fwrite(symbol, 1, 1, fptr);
				//printf("*");
			}
			fwrite(enter, 1, 1, fptr);
			//printf("\n");
		}
	}

	else if (iter_num < 0) {
		for (int i = 1; i <= ( (-1) * iter_num); i++) {
			for (int j = ( (-1) * iter_num); j >= i; j--) {
				fwrite(symbol, 1, 1, fptr);
				//printf("#");
			}
			fwrite(enter, 1, 1, fptr);
			//printf("\n");
		}
	}

	fclose(fptr);

	return 0;
}
