// 리눅스 시스템 프로그래밍_실습과제2_3번 문제_20193017_유지민

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define MAX_SIZE 1024

int main (int argc, char* argv[]) {
	int iter_num = 0; //정수(난수 생성 개수) 저장 변수
	int rand_fd; //정렬X 난수 저장 파일 디스크립터
	int rand_filesize = 0;
	int sort_fd; //정렬O 난수 저장 파일 디스크립터
	char filename[30] = "rand_test.txt"; //정렬X 난수 저장 파일
	char sorted_filename[30] = "sorted_test.txt"; //정렬O 난수 저장 파일
	
	int rand_int = 0; //생성되는 난수 저장 변수
	char rand_str[5]; //sprintf() 사용을 위한 배열
	char buf[MAX_SIZE]; //난수를 읽어와 저장하는 배열
	int read_arr[MAX_SIZE];

	char *token;
	int tok_cnt = 0;
	
	if (argc < 2) {
		fprintf(stderr, "usage : ./hw2_3_20193017 <iter_num>\n");
		exit(1);
	}

	iter_num = atoi(argv[1]);

	if ((rand_fd = creat(filename, 0666)) < 0) { //쓰기 모드로 파일이 생성되고 열림
		fprintf(stderr, "<filename> creat() error\n");
		exit(1);
	}

	srand(time(NULL)); //난수의 중복 방지
	for (int i = 0; i < iter_num; i++) {
		rand_int = rand() % iter_num;
		sprintf(rand_str, "%d\n", rand_int);
		printf("%s", rand_str);
		write(rand_fd, rand_str, sizeof(rand_str));
		memset(rand_str, 0, 5);
	}
	
	close(rand_fd);

	if ((rand_fd = open(filename, 0666)) < 0) { //읽기 전용으로 파일 오픈
		fprintf(stderr, "<filename> open() error\n");
		exit(1);
	}

	rand_filesize = lseek(rand_fd, (off_t)0, SEEK_END); //파일 크기 저장
	//printf("rand_filesize : %d\n", rand_filesize);
	
	memset(buf, 0, MAX_SIZE);
	lseek(rand_fd, (off_t)0, SEEK_SET);
	int res = read(rand_fd, buf, sizeof(buf)); //버퍼에 정렬되지 않은 난수들 저장
	printf("res : %d\n", res);
	printf("buf : %s\n", buf);
	close(rand_fd);

	if ((sort_fd = creat(sorted_filename, 0666)) < 0) {
		fprintf(stderr, "<sorted_filename> fopen() error\n");
		exit(1);
	}

	token = strtok(buf, "\n");
	//printf("2 - token : %s\n", token);
	//read_arr[tok_cnt] = atoi(token);
	//printf("3 - read_arr[tok_cnt] : %d\n", read_arr[tok_cnt]);
	while (token != NULL) {
		token = strtok(NULL, "\n");
		tok_cnt++;
		//printf("%d %s\n", tok_cnt, token);
		//read_arr[tok_cnt] = atoi(token);
	}

	//for (int i = 0; i < tok_cnt; i++) {
	//	printf("read_arr[%d] : %d\n", tok_cnt, read_arr[tok_cnt]);
	//}

	close(sort_fd);
	return 0;
}


