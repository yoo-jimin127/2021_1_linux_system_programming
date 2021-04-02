// 리눅스 시스템 프로그래밍_실습과제2_1번 문제_20193017_유지민

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char * argv[]) {
	char *filename; //파일명
	int fd; //파일 디스크립터
	int iter_num = 0; //정수 값
	char symbol[1] = ""; //기호 값
	char enter[1] = "\n"; //

	if (argc < 4) {
		fprintf(stderr, "usage : ./hw2_1_20193017 <iter_num> <symbol> <filename>\n");
		exit(1);
	}

	//for (int i = 0; i < argc; i++) {
	//	printf("argv[%d] : %s\n", i, argv[i]);
	//}

	filename = (char *)malloc(sizeof(char) * 100);

	iter_num = atoi(argv[1]);
	strcpy(symbol, argv[2]);
	strcpy(filename, argv[3]);

	if ((fd = creat(filename, 0666)) < 0)  {
		fprintf(stderr, "<filename> creat() error\n");
		exit(1);
	}

	if (iter_num > 0) {
		for (int i = 1; i <= iter_num; i++) {
			for (int j = 1; j <= i; j++) {
				write(fd, symbol, sizeof(symbol));
				//printf("*");
			}
			write(fd, enter, sizeof(enter));
			//printf("\n");
		}
	}

	else if (iter_num < 0) {
		for (int i = 1; i <= ( (-1) * iter_num); i++) {
			for (int j = ( (-1) * iter_num); j >= i; j--) {
				write(fd, symbol, sizeof(symbol));
				//printf("#");
			}
			write(fd, enter, sizeof(enter));
			//printf("\n");
		}
	}

	close(fd);

	return 0;
}
