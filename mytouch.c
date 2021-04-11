#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/type.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define MAX_SIZE 1024

int main (int argc, char* argv[]) {
	DIR *dirptr = NULL;
	struct dirent *entry;
	struct stat statbuf;
	char *dir;
	char path[MAX_SIZE];

	return 0;
}
