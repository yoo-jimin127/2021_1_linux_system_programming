// 리눅스 시스템 프로그래밍_실습과제2_3번 문제_20193017_유지민

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define MAX_SIZE 999

int main (int argc, char* argv[]) {
	int iter_num = 0; //정수(난수 생성 개수) 저장 변수
	int rand_fd; //정렬X 난수 저장 파일 디스크립터
	int rand_filesize = 0;
	int sort_fd; //정렬O 난수 저장 파일 디스크립터
	char filename[30] = "rand_test.txt"; //정렬X 난수 저장 파일
	char sorted_filename[30] = "sorted_test.txt"; //정렬O 난수 저장 파일
	
	int rand_int = 0; //생성되는 난수 저장 변수
	char rand_str[10]; //sprintf() 사용을 위한 배열(난수)
	char sort_str[10]; //sprintf() 사용을 위한 배열(정렬된 난수)
	char buf[MAX_SIZE]; //난수를 읽어와 저장하는 배열
	int rand_arr[MAX_SIZE]; //정렬할 난수를 int형으로 저장할 배열
	int tok_cnt = 0; //토큰 개수 카운터
	
	if (argc < 2) { //실행의 방법이 잘못 된 경우
		fprintf(stderr, "usage : ./hw2_3_20193017 <iter_num>\n");
		exit(1); //에러처리 후 비정상 종료
	}

	iter_num = atoi(argv[1]); //정수 저장(문자를 정수로)

	if ((rand_fd = open(filename, O_RDWR|O_CREAT|O_TRUNC, 00666)) < 0) { //파일을 생성하여 읽기/쓰기가 가능하도록 open()
		fprintf(stderr, "<filename> open() error\n");
		exit(1); //open() 실패 시 에러출력 후 비정상 종료
	}

	srand(time(NULL)); //난수의 중복 방지를 위한 time.h 사용
	for (int i = 0; i < iter_num; i++) {
		rand_int = rand() % 1000; //난수 생성
		sprintf(rand_str, "%d", rand_int); //정수를 문자열로 변환하기 위해 사용되는 임시 버퍼
		write(rand_fd, rand_str, strlen(rand_str)); //rand_test.txt에 씀
		write(rand_fd, "\n", sizeof(char)); //난수 간의 구분자를 개행문자로 하여 출력
		memset(rand_str, 0, 10); //임시 버퍼인 문자열 초기화
	}
	
	rand_filesize = lseek(rand_fd, 0, SEEK_END); //파일 크기 저장
	lseek(rand_fd, 0, SEEK_SET); //read()를 위해 다시 파일의 시작부로 lseek
	read(rand_fd, buf, rand_filesize); //파일 크기만큼 버퍼에 파일의 내용을 저장
	close(rand_fd); //파일 닫기

	if ((sort_fd = open(sorted_filename, O_WRONLY|O_CREAT|O_TRUNC, 00666)) < 0) { //파일을 생성하여 읽기/쓰기 가능하도록 open()
		fprintf(stderr, "<sorted_filename> open() error\n");
		exit(1); //open() 실패 시 에러출력 후 비정상 종료
	}

	char *token = strtok(buf, "\n"); //개행문자를 구분자로 첫 토큰 분리하여 저장
	while (token != NULL) { //더이상의 토큰이 없을 때까지 반복
		rand_arr[tok_cnt] = atoi(token); //문자열을 정수로 변환해 int형 배열의 원소로 삽입
		tok_cnt++;
		token = strtok(NULL, "\n"); //다음 토큰 가져옴
	}

	for (int i = 0; i < tok_cnt; i++) {//버블정렬을 통해 배열의 원소를 오름차순으로 정렬
		for (int j = 0; j < tok_cnt - (i + 1); j++) {
			if (rand_arr[j] > rand_arr[j+1]) {
				int tmp = rand_arr[j+1];
				rand_arr[j+1] = rand_arr[j];
				rand_arr[j] = tmp;
			}
		}
	}

	for (int i = 0; i < tok_cnt; i++) { //정렬된 난수들을 토큰의 개수만큼 파일에 쓰는 과정
		sprintf(sort_str, "%d", rand_arr[i]);
		write(sort_fd, sort_str, strlen(sort_str));
		write(sort_fd, "\n", sizeof(char));
		memset(sort_str, 0, 10);
	}

	close(sort_fd); //파일 닫기
	return 0;
}


