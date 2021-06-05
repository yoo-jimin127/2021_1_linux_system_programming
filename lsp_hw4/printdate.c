#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void sigint_handler(int signo);
void (*handler_return_val)(int);

int main (int argc, char **argv) {
	int timelimit = 0; // timelimit
	pid_t pid; //프로세스 아이디
	int status = 0; //wait를 위한 변수


	if (argv[1] == NULL) {
		timelimit = 0;
	} //timelimit 인자 없으면 0으로 설정

	if (argv[1] != NULL) {
		handler_return_val = signal(SIGINT, sigint_handler); //^C누르면 해당 함수 진행하도록 시그널 변경
		timelimit = atoi(argv[1]); //argv[1] 값의 인자를 정수화하여 timelimit변수에 저장
	}

	if (timelimit != 0) { //timelimit 인자가 주어진 경우
		for (int i = 0; i < timelimit; i++) {
			if ((pid = fork()) == 0) { //자식 프로세스는 execl으로 date명령어 실행 
				execl("/bin/date", "date", NULL, NULL);
			}

			else if (pid > 0) { //부모 프로세스는 자식들의 종료를 기다림
				if (wait(&status) != pid) {
					fprintf(stderr, "wait() error\n");
					exit(1);
				}
			}

			sleep(1); //1초씩 기다림
		}
	}

	else if (timelimit == 0) { //timelimit 인자가 주어지지 않은 경우
		while(1) {
			if ((pid = fork()) == 0) { //자식 프로세스는 execl로 date명령어 진행
				execl("/bin/date", "date", NULL, NULL);
			}

			else if (pid > 0) {
				if (wait(&status) != pid) { //부모 프로세스는 자식 프로세스의 종료를 기다림
					fprintf(stderr, "wait() error\n");
					exit(1);
				}
			}

			sleep(1); //1초 지연
		}
	}


	return 0;
}

void sigint_handler (int signo)  {
	int count_down = 5; //5초간의 카운트다운 진행
	printf("\n");
	for (int i = count_down; i > 0; i--) { 
		printf("-----printdate 프로그램 종료 %d초 전-----\n", i);
		sleep(1); //1초 주기로 5초간의 카운트다운 진행
	}
	signal(SIGINT, handler_return_val); //다시 SIGINT의 핸들러 원상복구
	exit(0); //프로그램 종료
	return;
}
