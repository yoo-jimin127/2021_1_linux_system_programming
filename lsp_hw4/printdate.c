#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigint_control(int signo);

int main (int argc, char **argv) {
	int timelimit = 0;
	pid_t pid;
	int status = 0;

	if (argv[1] == NULL) {
		timelimit = 0;
	}

	if (argv[1] != NULL) {
		timelimit = atoi(argv[1]);
	}

	if (timelimit != 0) {
		for (int i = 0; i < timelimit; i++) {
			if ((pid = fork()) == 0) {
				execl("/bin/date", "date", NULL, NULL);
			}

			else if (pid > 0) { 
				if (wait(&status) != pid) {
					fprintf(stderr, "wait() error\n");
					exit(1);
				}
			}

			sleep(1);
		}
	}

	else if (timelimit == 0) {
		while(1) {
			if ((pid = fork()) == 0) {
				execl("/bin/date", "date", NULL, NULL);
			}

			else if (pid > 0) {
				if (wait(&status) != pid) {
					fprintf(stderr, "wait() error\n");
					exit(1);
				}
			}

			sleep(1);
		}
	}


	return 0;
}
