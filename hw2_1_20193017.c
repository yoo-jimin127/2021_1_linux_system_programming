// 리눅스 시스템 프로그래밍_실습과제2_1번 문제_20193017_유지민

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (void) {
	char *filename; //파일명
	int fd; //파일 디스크립터
	int iter_num = 0; //정수 값
	char *symbol; //기호
	char enter[10] = "\n"; //개행문자

	filename = (char *)malloc(sizeof(char) * 100); //파일명 저장 배열 동적할당
	symbol = (char *)malloc(sizeof(char) * 10); //기호 저장 배열 동적할당

	memset(symbol, 0, 10); //symbol 메모리 초기화
	memset(filename, 0, 100); //filename 메모리 초기화

	printf("정수를 입력하세요 : "); //정수 입력
	scanf("%d", &iter_num);
	printf("기호를 입력하세요 : "); //기호 입력
	scanf("%s", symbol);
	printf("출력할 파일 이름을 입력하세요 :"); //파일명 입력
	scanf("%s", filename);
	
	if ((fd = creat(filename, 0666)) < 0)  { //모두에게 읽기, 쓰기 권한을 부여하여 파일 생성 후 읽기 모드로 open
		fprintf(stderr, "<filename> creat() error\n");
		exit(1); //파일 생성에 실패할 경우 표준 에러 출력 후 비정상 종료
	}

	if (iter_num > 0) { //입력 받은 정수 값이 양수일 경우
		for (int i = 1; i <= iter_num; i++) {
			for (int j = 1; j <= i; j++) {
				write(fd, symbol, sizeof(symbol)); //입력받은 기호를 파일에 씀
			}
			write(fd, enter, sizeof(enter)); //개행문자를 파일에 씀
		}
	}

	else if (iter_num < 0) { //입력 받은 정수 값이 음수일 경우
		for (int i = 1; i <= ( (-1) * iter_num); i++) {
			for (int j = ( (-1) * iter_num); j >= i; j--) {
				write(fd, symbol, sizeof(symbol));
			}
			write(fd, enter, sizeof(enter));
		}
	}

	else { //입력 받은 정수 값이 0인 경우
		printf("0을 제외한 양의 정수 또는 음의 정수를 입력하세요.\n");
		exit(1); //메시지 출력 후 종료
	}

	close(fd); //파일디스크립터를 넘겨주어 filename close

	return 0;
}
