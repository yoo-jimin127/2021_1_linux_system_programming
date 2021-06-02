#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>


int main (int argc, char **argv) { // ./printdate (timelimit)
	long long timelimit = 0;
	if (argv[1] != NULL) { //argv[1]의 값이 있을 경우
		timelimit = atoi(argv[1]);
	}

	if (timelimit == 0) { //인자로 전달받은 timelimit의 값이 존재하지 않는 경우 (무한루프)
		while(1) {
			
		}
	}

	else if (timelimit != 0) {
		for (int i = 0; i < timelimit; i++) {

		}
	}
	
	return 0;
}
