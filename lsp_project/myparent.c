#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char **argv) {
	int numofchild = 0;
	pid_t pid[60];

	if (argv[1] == NULL) {
		printf("numofchild를 입력하세요\n");
		exit(1);
	}

	numofchild = atoi(argv[1]);

	for (int i = 0; i < numofchild; i++) {
		if((pid[i] = fork()) < 0){ //프로세스를 생성하고 해당 프로세스의 pid를 저장
			fprintf(stderr, "pid[%d] fork() error\n", i);
			exit(1);
		}
		else {
			printf("I'm child <%d
		}
	}

	for 
	return 0;
}
