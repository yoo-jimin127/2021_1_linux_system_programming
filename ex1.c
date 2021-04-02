#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char* argv[]) {
	// ./실행파일 정수 기호 파일명
	int iter_num;
	char *symbol;
	char *filename;
	int fd;
	char enter[10] = "\n";

	symbol = (char *)malloc(sizeof(char) * 10);
	filename = (char *)malloc(sizeof(char) * 100);

	memset(symbol, 0, 10);
	memset(filename, 0, 100);
	
	iter_num = atoi(argv[1]);
	strcpy(symbol, argv[2]);
	strcpy(filename, argv[3]);

	if ((fd = open(filename, 0666)) < 0) {
		fprintf(stderr, "filename open() error\n");
		exit(1);
	}

	for (int i = 1; i <= iter_num; i++) {
		for (int j = 1; j <= i; j++) {
			write(fd, symbol, sizeof(symbol));
		}
		write(fd, enter, sizeof(enter));
	}

	close(fd);

	return 0;
}
