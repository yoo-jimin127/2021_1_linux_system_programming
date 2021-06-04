#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char **argv) {
	int mychild = 0;

	if (argv[1] == NULL) {
		fprintf(stderr, "mychild인자를 입력하세요.\n");
		exit(1);
	}

	mychild = atoi(argv[1]);

	return 0;
}
