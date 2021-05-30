#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max 100000

int n,m;

void seq(int** matrix,int m,int n);
void prc(int** matrix);
void thr(int** matrix);

int main(int argc, char* argv[]) {
	FILE* check = fopen(argv[1],"r+");
	int row = 0;
	char line[max];
	while(!feof(check)) { // file 읽으면서 값 넣어주기.
		int col = 0;
		fgets(line,max,check);
		char* ptr = strtok(line," ");
		while(ptr != NULL){
			col++;
			ptr = strtok(NULL," ");
		}
		row++;
		n = col;
	}
	m = row;
	printf("1 m %d n %d \n",m,n);
	fclose(check);



	FILE* fp = fopen(argv[1],"r+");
	
	int** matrix = (int**)malloc(sizeof(int*) * (m + 2)); // +2는 벽을 만들어줌.
	for(int i = 0; i < m + 2; i++){
		matrix[i] = (int*)malloc(sizeof(int)* (n + 2)); // 2차원배열로 만들어줌.
	}

	
	for(int i = 0; i < m + 2; i++){
		for(int j = 0; j < n + 2; j++){
			matrix[i][j] = 0; // matrix 초기화.
		}
	}

	row = 0;
	while(!feof(fp)) { // file 읽으면서 값 넣어주기.
		int col = 0;
		
		fgets(line,max,fp);
		char* ptr = strtok(line," ");
		while(ptr != NULL){
			matrix[row + 1][col + 1] = atoi(ptr);
			col++;
			ptr = strtok(NULL," ");
		}
		row++;
		n = col;
	}
	m = row;
	printf("2 m %d n %d \n",m,n);
	fclose(fp);
/*
matrix 준비완료.
*/
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}
	while (1) {		
		printf("동작 방식을 입력하세요\n");
		printf(" (1) 프로그램종료\n");
		printf(" (2) 순차처리\n");
		printf(" (3) process 병렬처리\n");
		printf(" (4) thread 병렬처리\n");
		int play = 0;
		scanf("%d",&play);
		switch (play){
			case 1:
				printf("Quit\n");
				return 0;
			case 2:
				printf("순차처리입니다.\n");
				// 시간측정 추가하기.
				seq(matrix,m,n);
				// 시간측정 코드 추가하기
				break;
			case 3:
				printf("프로세스 처리입니다.\n");
				prc(matrix);
				break;
			case 4:
				printf("쓰레드 처리입니다.\n");
				thr(matrix);
				break;
			default : 
				printf("1~4 사이의 숫자만 입력해주세요.\n");
		}
	}

}

int count(int** matrix, int y, int x) {
	int count = 0;
	for(int i = y-1; i <= y+1; i++){
		for(int j = x-1; j<= x+1; j++){
			count += matrix[i][j];
		}
	}
	return count - matrix[y][x];;
}

void seq(int** cp_matrix,int m,int n) {
	int** matrix = (int**)malloc(sizeof(int*) * (m + 2));
	for(int i = 0; i < m + 2; i++){
		matrix[i] = (int*)malloc(sizeof(int)* (n + 2));
	}
	for(int i = 0; i < m + 2; i++){
		for(int j = 0; j < n + 2; j++){
			matrix[i][j] = cp_matrix[i][j];
		}
	}

	int step = 0;
	int cnt = 0;
	printf("단계를 입력하세요\n");
	char num[5];
	char* filename = "seq_";
	scanf("%d",&step);

	int next_matrix[m][n];
	for(int i = 0; i < step; i++) {	
		for(int j = 0; j < m; j++){
			for(int k = 0; k < n; k++){
				next_matrix[j][k] = 0;
			}
		}

		for(int j = 1; j <= m; j++) {
			for(int k = 1; k <= n; k++) {
				cnt = count(matrix,j,k);
				if(matrix[j][k] && 3 <= cnt && cnt <= 6) { // 살아있는경우
						next_matrix[j - 1][k - 1] = 1;	
				}
				else if(!matrix[j][k] && cnt == 4){ // 죽어있는경우
					next_matrix[j -1][k - 1] = 1;
				}
			}
		}
		char temp_filename[50] = "";
		//temp_filename = "";
		strcat(temp_filename,filename);
		strcat(temp_filename,"gen_");
		sprintf(num,"%d",i + 1);
		strcat(temp_filename,num);
		strcat(temp_filename,".matirx");

		//filemame = seq_gen_num.maitrx
		FILE* new_fp = fopen(temp_filename,"w+");
		
		char cell[5];
		for(int j = 0; j < m; j++) {
			for(int k = 0; k < n; k++) {
				sprintf(cell,"%d",next_matrix[j][k]);
				matrix[j + 1][k + 1] = next_matrix[j][k];
				fputs(cell,new_fp);
				
				if(k == n - 1){
					fputc('\n',new_fp);
				}
				else {
					fputc(' ',new_fp);
				}
			}
		}
		fclose(new_fp);
	}
	char cell[5];
	char output_filename[50] = "";
	strcat(output_filename,filename);
	strcat(output_filename,"output.matrix");
	FILE* fp1 = fopen(output_filename,"w+");
	for(int i = 1; i <= m; i++) {
		for(int j = 1; j <= n; j++) {
			sprintf(cell,"%d",matrix[i][j]);
			fputs(cell,fp1);
			if(j == n){
				fputc('\n',fp1);
			}
			else {
				fputc(' ',fp1);
			}
		}
	}
	fclose(fp1);

}

void prc(int** matrix) {
	int step = 0;
	printf("단계를 입력하세요\n");
	scanf("%d",&step);
	for(int i = 0; i < step; i++) {

	}

}

void thr(int** matrix) {
	int step = 0;
	printf("단계를 입력하세요\n");
	scanf("%d",&step);
	for(int i = 0; i < step; i++) {

	}

}
