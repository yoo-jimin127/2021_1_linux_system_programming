#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define MAX_SIZE 1024

char type(mode_t);
char* perm(mode_t);

int main (int argc, char* argv[]) {	
	char *filename; //파일명 저장 변수
	int file_perm[4] = { 0, }; //파일 권한 저장 배열
	char *file_perm_str; //문자열로 지정된 파일 권한 저장 배열
	struct stat statbuf; //stat 구조체
	DIR *dirptr = NULL; //dirent 포인터
	struct dirent *entry; //dirent 구조체

	if (argc != 3) { //실행 인자를 잘못 입력하였을 경우
		fprintf(stderr, "Usage : %s <filename> <permission>\n", argv[0]);
		exit(1);
	}

	filename = (char *)malloc(sizeof(char) * 512);
	file_perm_str = (char *)malloc(sizeof (char) * 50);
	memset(filename, 0, 512);

	strcpy(filename, argv[1]); //filename 저장
	strcpy(file_perm_str, argv[2]); //file_perm_str 저장
	
	for (int i = 0; i < 4; i++) { //파일 권한을 int형 배열에 저장하기 위해 아스키코드 연산 수행
		file_perm[i] = file_perm_str[i] - '0';
	}

	mode_t mode; //mode 변수 선언
	mode = file_perm[3]; //mode_t의 비트마스크 연산 진행
	mode = mode | (file_perm[2] << 3);
	mode = mode | (file_perm[1] << 6);
	mode = mode | (file_perm[0] << 9);

	if (access(filename, F_OK) < 0) { // access() 함수 사용해 파일 존재 여부 확인
		fprintf(stderr, "<filename> doesn't exist.\n");
		exit(1);
	}
	
	if (lstat(filename, &statbuf) < 0) { // lstat()을 통해 파일의 정보 얻어옴
		fprintf(stderr, "lstat() error\n");
		exit(1);
	}

	if (chmod(filename, mode) < 0) { // chmod() 진행
		fprintf(stderr, "<filename> chmod error\n");
		exit(1);
	}

	else {
		printf("<%s> success change mode\n", filename);
	}	

	return 0;
}

//파일의 타입을 리턴해주는 함수
char type(mode_t mode) {
    if (S_ISREG(mode)) return '-';
    if (S_ISDIR(mode)) return 'd';
    if (S_ISCHR(mode)) return 'c';
    if (S_ISBLK(mode)) return 'b';
    if (S_ISLNK(mode)) return 'l';
    if (S_ISFIFO(mode)) return 'p';
    if (S_ISSOCK(mode)) return 's';
}

//파일 사용 권한을 리턴해주는 함수
char* perm(mode_t mode) {
    static char perms[10] = "---------";
    
    for (int i = 0; i < 3; i++) {
        if (mode & (S_IRUSR >> i * 3)) perms[i*3] = 'r';
        if (mode & (S_IWUSR >> i * 3)) perms[i*3+1] = 'w';
        if (mode & (S_IXUSR >> i * 3)) perms[i*3+2] = 'x';
    }   

    return(perms);
}

