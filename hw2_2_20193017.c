// 리눅스 시스템 프로그래밍_실습과제2_2번 문제_20193017 유지민

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_SIZE 999

int main (int argc, char * argv[]) {
	char *filename; //입력으로 주어지는 텍스트 파일명
	char *new_filename; //대소문자를 바꾸어 새롭게 생성하는 파일명
	FILE *fptr = NULL; //파일포인터
	int filesize = 0; //파일 사이즈 저장 변수
	char buf[MAX_SIZE]; //파일 내용 저장 버퍼

	if (argc <2) { //프로그램 실행 시 인자가 제대로 입력되지 않은 경우에 대한 예외처리
		fprintf(stderr, "usage : ./hw2_2_20193017 <filename>\n");
		exit(1);
	}

	filename = (char *)malloc(sizeof(char) * 100); //파일명 동적할당
	strcpy(filename, argv[1]); //argv[1]의 값을 filename에 저장

	memset(buf, 0, MAX_SIZE); //버퍼 초기화

	if ((fptr = fopen(filename, "r")) == NULL) { //입력으로 주어지는 텍스트파일 읽기 모드로 오픈
		fprintf(stderr, "<filename> fopen() error\n");
		exit(1);
	}

	fseek(fptr, 0, SEEK_END); //파일 포인터를 SEEK_END로 옮겨 파일의 사이즈를 알아냄
	filesize = (int)(ftell(fptr));

	fseek(fptr, 0, SEEK_SET); //파일 포인터 텍스트파일의 시작부분으로 옮김
	fread(buf, 1, filesize, fptr); //파일의 내용을 읽어 버퍼에 저장
	fclose(fptr); //입력으로 주어진 텍스트 파일 닫기

	for (int i = 0; i < strlen(filename); i++) { // 파일명의 대 -> 소, 소 -> 대
		if ((filename[i] >= 'a') && (filename[i] <= 'z')) { //소문자일 경우
			filename[i] = filename[i] - 'a' + 'A'; //대문자로 변경
		}
		else if ((filename[i] >= 'A') && (filename[i] <= 'Z')) { //대문자일 경우
			filename[i] = filename[i] - 'A' + 'a'; //소문자로 변경
		}
	}
	
	strcpy(new_filename, filename); //위에서 변경된 파일명을 new_filename에 저장

	for (int i = 0; i < strlen(buf); i++) { //텍스트 파일의 내용이 저장된 버퍼 대소문자 변경 
		if ((buf[i] >= 'a') && (buf[i] <= 'z')) { //소문자인 경우
			buf[i] = buf[i] - 'a' + 'A';
		}
		else if ((buf[i] >= 'A') && (buf[i] <= 'Z')) { //대문자인 경우
			buf[i] = buf[i] - 'A' + 'a';
		}
	}

	if ((fptr = fopen(new_filename, "w")) == NULL) { //대소문자가 바뀐 new_filename 쓰기모드로 오픈
		fprintf(stderr, "<new_filename> fopen() error\n");
		exit(1);
	}
	
	fwrite(buf, 1, filesize, fptr); //대소문자가 변경되어 저장된 버퍼의 내용을 파일에 씀
	fclose(fptr); //파일 닫기
	return 0;
}
