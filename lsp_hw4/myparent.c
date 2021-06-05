#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main (int argc, char **argv) {
	int mychild = 0; //mychild인자 저장 변수
	pid_t pids[60], pid; //pid저장
	int process_num = 0; //잔여 프로세스 개수 카운트 변수
	int status = 0;
	int loop_cnt = 1;
	int kill_return_val = 0; //kill()시 리턴값을 저장하기 위한 변수

	if (argv[1] == NULL) { //인자가 입력되지 않았을 때의 예외처리
		fprintf(stderr, "numofchild인자를 입력하세요.\n");
		exit(1);
	}

	mychild = atoi(argv[1]); //argv[1]의 인자 값을 정수로 변환

	while(process_num < mychild) { //process_num < mychild까지 반복
		pids[process_num] = fork(); //자식 프로세스 생성
		if (pids[process_num] < 0) { //자식 프로세스 생성이 제대로 이루어지지 않을 경우 에러 처리
			fprintf(stderr, "pids[%d] fork() error.\n", process_num);
			exit(1);
		}

		else if (pids[process_num] == 0) { //자식 프로세스가 성공적으로 수행되면 다음 루프로
			pause();
			signal(SIGTERM, 0);
			exit(0);
		}

		process_num++; // mychild의 개수와 process_num의 개수가 같아짐
	}

	printf("------%d 개의 child process 생성이 완료되었습니다.------\n\n", mychild);
	sleep(5); //mychild만큼의 자식 프로세스 생성 완료 후 5초 경과
	while(process_num) { //process_num이 양수일 때까지 
		printf("------process생성 완료로부터 %d초가 지났습니다.------\n", loop_cnt * 5);
		for (int i = 0; i < process_num; i++) { 
			printf("I'm child %d --- pid[%d] : %d\n", i + 1, i, pids[i]); //자식 프로세스의 생성 
		}

		for (int i = process_num -1 ; i >= process_num - 5; i--) {
			if ((kill_return_val = kill(pids[i], SIGTERM)) == -1) { //해당 자식 프로세스에 SIGTERM을 주어 종료
				fprintf(stderr, "pids[%d] kill() error\n", i);
				exit(1);
			}
			waitpid(pids[i], &status, 0);
			sleep(1); //1초에 하나의 프로세스가 종료되도록 하기 위해서 1초 시간 지연
		}

		if (process_num < 5) {
			for (int i = process_num -1; i >= 0; i--) {
				if ((kill_return_val = kill(pids[i], SIGTERM)) == -1) {
					fprintf(stderr, "pids[%d] kill() error\n", i);
					exit(1);
				}
				waitpid(pids[i], &status, 0);
				sleep(1);
			}
		}
		
		process_num -= 5; //5개의 프로세스가 종료되었으므로 process_num = process_num - 5
		loop_cnt++;
	}
	return 0;
}
