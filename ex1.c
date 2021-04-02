#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (void) {
	// ./실행파일 정수 기호 파일명
	int iter_num = 0;
	char *symbol;
	char *filename;
	int fd;
	char enter[10] = "\n";

	symbol = (char *)malloc(sizeof(char) * 10);
	filename = (char *)malloc(sizeof(char) * 100);

	memset(symbol, 0, 10);
	memset(filename, 0, 100);

	printf("정수를 입력하세요 : ");
	scanf("%d", &iter_num);
	printf("기호를 입력하세요 : ");
	scanf("%s", symbol);
	printf("출력할 파일 이름을 입력하세요 : ");
	scanf("%s", filename);

	if ((fd = creat(filename, 0666)) < 0) {
		fprintf(stderr, "filename open() error\n");
		exit(1);
	}

	if (iter_num > 0) {
		for (int i = 1; i <= iter_num; i++) {
			for (int j = 1; j <= i; j++) {
				write(fd, symbol, sizeof(symbol));
			}
			write(fd, enter, sizeof(enter));
		}
	}

	else if (iter_num < 0) {
		for (int i = 1; i <= ( (-1) * iter_num); i++) {
			for (int j = ( (-1) * iter_num); j >= i; j--) {
				write(fd, symbol, sizeof(symbol));
			}
			write(fd, enter, sizeof(enter));
		}
	}

	close(fd);

	return 0;
}
