#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main (void) {
	char buf[1] = "#";
	char enter[1] = "\n";
	char *filename;
	FILE *fptr;
	
	filename = (char *)malloc(sizeof(char) * 100);

	strcpy(filename, "print_star.txt");

	if ((fptr = fopen(filename, "w")) == NULL) {
		printf("fopen()\n");
		exit(1);
	}

	for (int i = 1; i <= 5; i++) {
		for (int j = 1; j <= i; j++) {
			fwrite(buf, 1, 1, fptr);
		}
		fwrite(enter, 1, 1, fptr);
	}

	return 0;
}
