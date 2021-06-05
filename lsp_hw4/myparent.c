#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main (int argc, char **argv) {
	int mychild = 0;
	pid_t pids[60], pid;
	int process_num = 0;
	int status = 0;
	int loop_cnt = 1;

	if (argv[1] == NULL) {
		fprintf(stderr, "mychild인자를 입력하세요.\n");
		exit(1);
	}

	mychild = atoi(argv[1]);

	while(process_num < mychild) {
		pids[process_num] = fork();
		if (pids[process_num] < 0) {
			fprintf(stderr, "pids[%d] fork() error.\n", process_num);
			exit(1);
		}

		else if (pids[process_num] == 0) {
			exit(0);
		}

		else {
			if (wait(&status) != pids[process_num]) {
				fprintf(stderr, "pids[%d] wait() error\n", process_num);
				exit(1);
			}
		}

		process_num++; // mychild의 개수와 process_num의 개수가 같아짐
	}
	
	printf("------%d 개의 child process 생성이 완료되었습니다.------\n", mychild);
	sleep(5); //mychild만큼의 자식 프로세스 생성 완료 후 5초 경과
	while(process_num) {
		printf("------process생성 완료로부터 %d초가 지났습니다.------\n", loop_cnt * 5);
		for (int i = 0; i < process_num; i++) { 
			printf("I'm child %d --- %d\n", i, pids[i]);
		}

		for (int i = process_num -1 ; i >= process_num - 5; i--) {
			kill(pids[i], SIGINT);
			sleep(1);
		}
			process_num -= 5;
			loop_cnt++;
	}

	return 0;
}
