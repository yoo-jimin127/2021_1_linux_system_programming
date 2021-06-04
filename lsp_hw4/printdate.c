#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main (int argc, char **argv) {
	int timelimit = 0;
	pid_t pid;
	int exec_return = 0;

	if (argv[1] != NULL) {
		timelimit = atoi(argv[1]);
	}

	if (timelimit != 0) {
		while(timelimit){
			if ((pid = fork()) < 0) {
				fprintf(stderr, "fork() error\n");
				exit(1);
			}

			else if ((pid = fork()) == 0) {
				exec_return = execl("/bin/date", "/bin/date", NULL, (char *)0);
				kill(pid, SIGINT);
				if (exec_return == -1) {
					fprintf(stderr, "failed the call of execl.\n");
					exit(1);
				}
				sleep(1);
				timelimit--;
			}

			if (timelimit <= 5) {
				printf("printdate 종료 카운트다운 %d초 전\n", timelimit);
			}
		}
	}

	else if (timelimit == 0) {
		while(1) {
			if ((pid = fork()) < 0) {
				fprintf(stderr, "fork() error at infinity roof\n");
				exit(1);
			}

			else if ((pid = fork()) == 0) {
				exec_return = execl("/bin/date", "/bin/date", NULL, (char *)0);
				kill(pid, SIGINT);
				if (exec_return == -1) {
					fprintf(stderr, "failde the call of execl at infinity roof.\n");
					exit(1);
				}
			}

			sleep(1);
		}
	}



	return 0;
}
