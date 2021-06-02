#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

void *ssu_thread(void *arg);
int th_cnt = 0;

int main (int argc, char **argv) {
	pthread_t tid[255];
	pid_t pid;
	//int thr_id;
	int status;

	th_cnt = atoi(argv[1]);

	for (int i = 0; i < th_cnt; i++) {
		if(pthread_create(&tid[i], NULL, ssu_thread, NULL) != 0) {
			fprintf(stderr, "pthread_create error\n");
			exit(1);
		}
	}

	for (int i = 0; i < th_cnt; i++) {
		pthread_join(tid[i], (void *)&status);
	}

	printf("언제 종료?\n");
	
	pthread_exit(NULL);
	printf("지금 종료\n");
	return 0;
}

void *ssu_thread(void *arg) {
	pthread_t tid;
	pid_t pid;

	pid = getpid();
	tid = pthread_self();
	
	for (int i = 0; i < th_cnt; i++) {
		printf("pid : %d, tid : %u --- %d\n", (int)pid, (unsigned int)tid, i);
	}

	sleep(1);

	return NULL;
}
