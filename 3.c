#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

int main(void){

	//fprintf(stderr, "Start!\n");
	int num;
	int fp1, fp2;
	char buffer[1025];
	int idx = 0;
	scanf("%d", &num);
	srand((int)time(NULL));

	int unsorted[num];
	fp1 = open("3_first.txt", O_RDWR|O_TRUNC|O_CREAT, 00755); //첫 파일 오픈
	for(int i = 0; i<num; i++){
		int r_data = rand(); //랜덤 값 생성
		//fprintf(stderr, "r_data : %d\n", r_data);
		char rr_data[128] = {0,};
		sprintf(rr_data, "%d", r_data); //값을 문자열로
		write(fp1, rr_data, strlen(rr_data)); //파일에 쓰기
		write(fp1, "\n", 1);
	}

	int len = lseek(fp1, 0, SEEK_END); //파일 길이 구하기
	//int len = ftell(fp1);
	lseek(fp1, 0, SEEK_SET);

	fp2 = open("3_second.txt", O_WRONLY|O_TRUNC|O_CREAT, 00755); //두번째 파일 오픈
	read(fp1, buffer, len); //내용 읽기. 첫번째 파일 내용
	char* token = strtok(buffer, "\n"); //공백을 단위로 끊어 읽기로
	while(token != NULL){
		unsorted[idx] = atoi(token); //다시 int로 변환
		//printf("%d : %d %s\n", idx, unsorted[idx], token);
		idx++;
		token = strtok(NULL, "\n");
	}

	idx--;
	for(int i = 0; i<idx; i++){ //정렬
		for(int j = i+1; j<=idx; j++){
			if(unsorted[j]<unsorted[i]){
				int tmp = unsorted[i];
				unsorted[i] = unsorted[j];
				unsorted[j] = tmp;
			}
			//fprintf(stderr,"j=%d) %d\n", j, unsorted[i]);
		}
	}

	/*
	for(int i = 0; i<=idx; i++){
		printf("%d\n", unsorted[i]);
	}*/

	for(int i = 0; i<=idx; i++){
		char new[128]={0,};
		sprintf(new, "%d", unsorted[i]); //정렬한 int형을 문자열로 변환
	//	fprintf(stderr, "new : %s\n", new);
		write(fp2, new, strlen(new)); //파일2에 쓰기
		write(fp2, "\n", 1);
	}

	return 0;

}

