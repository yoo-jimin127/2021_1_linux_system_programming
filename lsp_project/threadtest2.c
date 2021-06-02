#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int th_cnt = 0;

int main (int argc, char **argv) {
	th_cnt = atoi(argv[1]);
	
	return 0;
}
