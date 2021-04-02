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
	int rand_fd;
	int sort_fd;
	char filename[30] = "rand_test.txt";
	char sorted_filename[30] = "sorted_test.txt";
	int rand_num = 0;
	
	if (argc < 2) {
		fprintf(stderr, "usage : ./hw2_3_20193017 <iter_num>\n");
		exit(1);
	}

	iter_num = atoi(argv[1]);
	int *rand_arr = malloc(sizeof(int) * iter_num);
	//memset(rand_arr, 0, iter_num);

	if ((rand_fd = creat(filename, 0666)) < 0) { //읽기 모드로 파일이 생성되고 열림
		fprintf(stderr, "<filename> creat() error\n");
		exit(1);
	}

	srand(time(NULL)); //난수의 중복 방지
	for (int i = 0; i < iter_num; i++) {
		rand_num = rand() % iter_num;
		printf("rand_num : %d\n", rand_num);
		write(rand_fd, &rand_num, sizeof(int));
		//printf("success : %d\n", success);
	}

	//write(rand_arr, sizeof(int), iter_num, fptr);
	close(rand_fd);

	if ((sort_fd = creat(sorted_filename, 0666)) < 0) {
		fprintf(stderr, "<sorted_filename> fopen() error\n");
		exit(1);
	}

	close(fd2);
	return 0;
}
