#include <stdio.h>
#include <stdlib.h>

int main (void) {
	int row = 102;
	int child = 5;
	int *arr = (int*)malloc(sizeof(int) * child);

	if (row % child == 0) {
		printf("row/child : %d\n", row/child);
		for (int i = 0; i < child; i++){
			arr[i] = row/child;
		}
	}

	else {
		printf("row/child : %d, row mod child : %d\n", row/child, row%child);
		int mod = row%child;
		for (int i = 0; i < child; i++){
			arr[i] = row/child;
		}

		//나머지 값은 child process보다 클 수 없음.
		for (int i = 0; i < mod; i++){
			int cnt = 0;
			arr[i] += 1;
		}
	}

	for (int i = 0; i < child; i++) {
		printf("arr[%d] : %d\n", i, arr[i]);
	}

	return 0;
}
