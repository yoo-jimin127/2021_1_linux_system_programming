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
	char *filename;
	int file_perm = 0;
	struct stat statbuf;
	DIR *dirptr = NULL;
	struct dirent *entry;

	if (argc != 3) {
		fprintf(stderr, "Usage : %s <filename> <permission>\n", argv[0]);
		exit(1);
	}

	filename = (char *)malloc(sizeof(char) * 512);
	memset(filename, 0, 512);

	strcpy(filename, argv[1]);
	file_perm = atoi(argv[2]);

	if (access(filename, F_OK) < 0) { // access() 함수 사용해 파일 존재 여부 확인
		fprintf(stderr, "<filename> doesn't exist.\n");
		exit(1);
	}

	if (lstat(filename, &statbuf) < 0) {
		fprintf(stderr, "<filename> stat error\n");
		exit(1);
	}

	if (chmod(filename, file_perm) < 0) {
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

