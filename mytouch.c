#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <dirent.h>
#include <time.h>
#include <utime.h>

#define MAX_SIZE 1024

int main (int argc, char* argv[]) {
	DIR *dirptr = NULL;
	struct dirent *entry;
	struct stat statbuf; //stat 구조체
	char *filename;
	struct utimbuf timebuf; //utimebuf 구조체
	int fd;

	if (argc != 2) {
		fprintf(stderr, "Usage : %s <filename or directory_name>\n", argv[0]);
		exit(1);
	}

	filename = (char *)malloc(sizeof(char) * 512);
	memset(filename, 0, 512);

	strcpy(filename, argv[1]);

	if (access(filename, F_OK) < 0) { //파일이 존재하지 않는 경우
		if ((fd = creat(filename, 0666)) < 0) { //creat로 파일을 생성
			fprintf(stderr, "create error for <%s>\n", filename);
			exit(1);
		}

		else {
			printf("<%s> is created\n", filename);
		}
	}

	if (stat(filename, &statbuf) < 0) {
		fprintf(stderr, "<%s> stat() error\n", filename);
		exit(1);
	}

	timebuf.actime = statbuf.st_atime; //timebuf.actime에 값 저장
	timebuf.modtime = statbuf.st_mtime; //timebuf.modtime에 값 저장

	if (utime(filename, NULL) < 0) {
		fprintf(stderr, "<%s> utime() error\n", filename);
		exit(1);
	}

	else {
		printf("success <%s> mytouch\n", filename);
	}

	return 0;
}
