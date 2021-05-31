#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <pthread.h>
#include <dirent.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>

#define MAX_SIZE 1024

/* ========================== << 함수 정의부 >> ========================*/
void sequential_processing(int** cell_arr, int input_gene); //순차처리
void process_parallel_processing(int** cell_arr, int input_gene, int child_process_input); //프로세스 병렬처리
void thread_parellel_processing(int** cell_arr, int input_gene, int thread_process_input); //스레드 병렬처리
int get_row_cnt(char* input_filename);
int get_col_cnt(char* input_filename);
int get_alive_cell(int **cell_arr, int row_cnt, int col_cnt);

long row = 0, col = 0; //매트릭스의 행, 열 저장 변수
/* =========================== << main func >> ===========================*/
int main (int argc, char* argv[]) {
	FILE* fp = NULL; //파일포인터
	char *input_filename = (char*) malloc(sizeof(char) * MAX_SIZE); //입력파일명 동적할당
	int user_choice = 0; //사용자 입력 메뉴
	int input_gene = 0; //세대 수 입력 값
	int child_process_input = 0; //(3)번 프로세스 병렬처리 메뉴 선택 시 입력될 child process 개수입력
	int thread_input = 0; //(4)번 스레드 병렬처리 메뉴 선택 시 입력될 thread 개수 입력
	//long long filesize = 0; //파일 크기 저장 변수
	int **cell_arr; //세포 매트릭스를 저장할 2차원 정수형 배열
	char *input_buf; //파일의 내용을 받아올 버퍼
	//char *tmpbuf; //2차원 배열에 저장하기 위한 임시 버퍼
	long row_cnt = 0, col_cnt = 0; //행, 열 카운트 변수

	strcpy(input_filename, argv[1]); //입력받은 argv[1](input.matrix)파일명을 복사 
	
	if ((fp = fopen(input_filename, "r+")) == NULL) { //파일 내용 읽기 위해 읽기모드로 열기
		fprintf(stderr, "fopen() error <mode : [r+]> in main() first call.\n");
		exit(1);
	}

	input_buf = (char*)malloc(sizeof(char) * MAX_SIZE); //임시버퍼 동적할당
	if (input_buf == NULL) {
		fprintf(stderr, "input_buf memory malloc error\n");
		exit(1);
	}
	memset(input_buf, 0, MAX_SIZE);
	
	row = get_row_cnt(input_filename);
	col = get_col_cnt(input_filename); //행과 열의 개수 구하기

	cell_arr = (int**)malloc(sizeof(int*) * (row + 2)); //가장자리 세포의 이웃 생성 (행)
	for (int i = 0; i < row + 2; i++) {
		cell_arr[i] = (int *)malloc(sizeof(int) * (col + 2)); //가장자리 세포의 이웃 생성 (열)
	} //행과 열의 개수만큼 2차원 배열 동적할당
	
	for (int i = 0; i < row + 2; i++) { //동적할당된 2차원 Int형 배열의 값 0으로 초기화
		for (int j = 0; j < col + 2; j++) {
			cell_arr[i][j] = 0;
		}
	}
	fseek(fp, 0, SEEK_SET); //파일포인터의 위치 파일의 시작으로 옮김
	row_cnt = 0;
	while(!feof(fp)) {
		col_cnt = 0;
		fgets(input_buf, MAX_SIZE, fp); //1줄씩 파일의 내용을 읽어와서
		char *token = strtok(input_buf, " "); //공백자를 기준으로 토큰 분리
		while(token != NULL) { //더이상 분리되는 토큰이 없을 때까지
			cell_arr[row_cnt + 1][col_cnt + 1] = atoi(token); //(row_cnt + 1, col_cnt + 1)에 정수로 형변환하여 넣기
			col_cnt++;
			token = strtok(NULL, " "); //다음 토큰 get
		}
		row_cnt++;
	}

	/*for (int i = 1; i < row + 1; i++) { //온전한 input.matrix의 내용
		for (int j = 1; j < col + 1; j++) {
			printf("%d#", cell_arr[i][j]);
		}
		printf("\n");
	}*/	

	//-----------------------메뉴 출력 후 해당 기능으로 인자를 넘기는 작업---------------------
	while(1) {
		printf("(1) 프로그램 종료\t (2) 순차처리\t (3) Process 병렬처리\t (4) Thread 병렬처리\n");
		printf("원하는 메뉴를 입력하세요 : ");
		scanf("%d", &user_choice);

		while(getchar() != '\n'); //버퍼에 있는 개행문자 제거

		switch(user_choice) {
			case 1 : //프로그램 종료
				system("clear");
				return 0;

			case 2 : // 순차처리
				system("clear");
				printf("======순차처리======\n");
				printf("진행할 세대 수를 입력하세요 (최소 1세대 이상) : ");
				scanf("%d", &input_gene);
				sequential_processing(cell_arr, input_gene);
				break;

			case 3 : // process 병렬처리
				system("clear");
				printf("======process 병렬처리======\n");
				printf("진행할 세대 수를 입력하세요 (최소 1세대 이상) : ");
				scanf("%d", &input_gene);
				printf("생성할 child process의 개수를 입력하세요 : ");
				scanf("%d", &child_process_input);
				process_parallel_processing(cell_arr, input_gene, child_process_input);
				break;
			
			case 4 : // thread 병렬처리
				system("clear");
				printf("======thread 병렬처리======\n");
				printf("진행할 세대 수를 입력하세요 (최소 1세대 이상) : ");
				scanf("%d", &input_gene);
				printf("생성할 thread의 개수를 입력하세요 : ");
				scanf("%d", &thread_input);
				thread_parellel_processing(cell_arr, input_gene, thread_input);
				break;

			default : // 그 밖의 메뉴 선택
				printf("(1) ~ (4) 메뉴를 선택하세요.\n");
		}
	}
	fclose(fp);
	return 0;
}

/*================================ << 순차처리 기능 구현부 >> ================================*/
void sequential_processing(int** cell_arr, int input_gene) {
	//printf("순차처리 기능이 실행됩니다.\n");
	printf("input_gene : %d row : %ld col : %ld\n", input_gene, row, col);
	
	char *new_filename = (char *)malloc(sizeof(char) * MAX_SIZE);
	FILE* new_fp = NULL; //다음 세대 매트릭스 파일 생성을 위한 파일포인터
	FILE* final_fp = NULL; //output.matrix 파일 생성을 위한 파일포인터
	int **working_cell_arr; //인자로 전달받은 매트릭스 복사해 작업하기 위한 목적의 배열
	int **new_cell_arr; //다음 단계에서 생성될 매트릭스
	int alive_cell_cnt = 0; //살아있는 이웃세포의 개수를 저장하기 위한 변수
	char tmp_cell[10] = ""; //다음 세대의 파일 내용을 저장할 때 필요한 세포 저장 임시 버퍼
	char *space_bar = " "; //공백문자
	char *enter = "\n"; //개행문자
	
	working_cell_arr = (int**)malloc(sizeof(int*) * (row + 2)); //가장자리의 이웃세포까지 만들어주기위해 행, 열의 +2만큼 동적할당
	for (int i = 0; i < row + 2; i++) {
		working_cell_arr[i] = (int*)malloc(sizeof(int) * (col + 2));
	}

	new_cell_arr = (int**)malloc(sizeof(int*) * row); //새롭게 생성되는 다음 세대의 파일 내용 담을 정수형 배열 동적할당
	for (int i = 0; i < row; i++) {
		new_cell_arr[i] = (int*)malloc(sizeof(int) * col);
	}

	for (int i = 0; i < row; i++) { // 새로 생성된 배열 값 초기화
		for (int j = 0; j < col; j++) {
			new_cell_arr[i][j] = 0; //new_cell_arr memset
		}
	}

	for (int i = 0; i < row + 2; i++) {
		for (int j = 0; j < col + 2; j++) {
			working_cell_arr[i][j] = cell_arr[i][j]; //인자로 전달받은 배열의 값 이 곳에 할당
		}
	}
	
	/*for (int i = 0; i < row + 2; i++) {
		for (int j = 0; j < col + 2; j++) {
			printf("%d ", working_cell_arr[i][j]);
		}
		printf("\n");
	} */

	//-----------------------------게임 진행-------------------------
	for (int i = 0; i < input_gene - 1; i++) { //입력받은 세대만큼 반복 진행
		for (int j = 0; j < row; j++) { //세대를 반복할 때 new_cell_arr의 내용 초기화하고 시작
			for (int k = 0; k < col; k++) {
				new_cell_arr[j][k] = 0;
			}
		}

		for (int j = 1; j <= row; j++) { // row만큼 진행[1][1] ~ (실제 데이터가 들어있는 배열원소 시작 인덱스 값)
			for (int k = 1; k <= col; k++) {
				alive_cell_cnt = get_alive_cell(working_cell_arr, j, k); //살아있는 세포 개수를 얻어옴
				if ((working_cell_arr[j][k] == 1) && (3 <= alive_cell_cnt) && (alive_cell_cnt <= 6)) { //세포가 살아있는 경우(1) && 3 <= 산 이웃세포 <= 6
						new_cell_arr[j-1][k-1] = 1; //(j -1) (k -1)의 세포 값을 1로 변경
				}
				else if (working_cell_arr[j][k] == 0 && alive_cell_cnt == 4) { //세포가 죽은 경우(0) && 산 이웃세포 == 4
						new_cell_arr[j-1][k-1] = 1; // (j - 1) ( k - 1)의 세포 값을 1로 변경
				}
			}
		}

		sprintf(new_filename, "gen_%d.matrix", i+1); //gen_n.matrix이름의 파일명 생성
		if ((new_fp = fopen(new_filename, "w+")) == NULL) { //읽기, 쓰기 모드로 열어 파일 없으면 새로 생성하도록
			fprintf(stderr, "new_fp fopen() error <mode : [w+]> in sequential_processing.\n");
			exit(1); //예외처리
		}

		//배열에 값 넣는 작업
		memset(tmp_cell, 0, sizeof(char) * 10); //먼저 임시버퍼 초기화를 시켜준 뒤
		for (int j = 0; j < row; j++) {
			for (int k = 0; k < col; k++) {
				working_cell_arr[j + 1][k + 1] = new_cell_arr[j][k]; //작업 매트릭스의 값 업데이트
				sprintf(tmp_cell, "%d", new_cell_arr[j][k]); //새롭게 생성된 세포배열 문자열 변환
				fwrite(tmp_cell, 1, sizeof(char), new_fp); //배열에 해당 세포 원소값 쓰는 작업
				fwrite(space_bar, 1, sizeof(char), new_fp); //공백자로 구분하여 넣음
				if (col == k + 1) { //다음 행으로 넘어가기 위한 조건
					fwrite(enter, 1, sizeof(char), new_fp); //파일에 개행 fwrite
				}
			}
		}
		fclose(new_fp);
	}

	memset(tmp_cell, 0, sizeof(char) * 10); //tmp_cell 버퍼 사용 전 초기화 작업
	char *final_filename = "output.matrix";
	if ((final_fp = fopen(final_filename, "w+")) == NULL) { //output.matrix 파일 생성 및 열기
		fprintf(stderr, "final_fp fopen() error <mode : [w+]> in sequential_processing.\n");
		exit(1);
	}
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			sprintf(tmp_cell, "%d", working_cell_arr[i][j]); //세포 값 문자열로 변환하여 tmp_cell에 sprintf()
			fwrite(tmp_cell, 1, sizeof(char), final_fp); //세포 값 fwrite
			fwrite(space_bar, 1, sizeof(char), final_fp);
			if (j == col) {
				fwrite(enter, 1, sizeof(char), final_fp);
			}
		}
	}
	fclose(final_fp);
		
	return;
}

/*================================ << process 병렬처리 기능 구현부 >> ================================*/
void process_parallel_processing(int** cell_arr, int input_gene, int child_process_input) {
	printf("프로세스 병렬처리 기능이 실행됩니다.\n");
	//printf("%s %d %d\n", input_filename, input_gene, child_process_input);
	return;
}

/*================================ << thread 병렬처리 기능 구현부 >> ================================*/
void thread_parellel_processing(int** cell_arr, int input_gene, int thread_process_input) {
	printf("스레드 병렬처리 기능이 실행됩니다.\n");
	//printf("%s %d %d\n", input_filename, input_gene, thread_process_input);
	return;
}

/*================================ << 부수적 함수 구현부 >> ================================*/
//입력파일의 행의 개수를 리턴하는 함수
int get_row_cnt(char* input_filename) {
	FILE * fp;
	int row_cnt = 1;
	char c;

	if ((fp = fopen(input_filename, "r+")) == NULL) {
		fprintf(stderr, "fopen() error in get_row_cnt func.\n");
		exit(1);
	}

	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n') row_cnt++;
	}

	fclose(fp);

	return row_cnt;
}

//입력파일의 열의 개수를 리턴하는 함수
int get_col_cnt(char* input_filename) {
	FILE* fp;
	int col_cnt = 0;
	char c;
	
	if ((fp = fopen(input_filename, "r+")) == NULL) {
		fprintf(stderr, "fopen() error in get_row_cnt func.\n");
		exit(1);
	}

	while ((c = fgetc(fp)) != '\n') {
		col_cnt++;
	}

	col_cnt /= 2;
	
	fclose(fp);
	return col_cnt;
}

//인자로 주어진 매트릭스에서 살아있는 세포의 개수를 구해 리턴하는 함수
int get_alive_cell(int **cell_arr, int row_num, int col_num) {
	int alive_cell_cnt = 0;
	//printf("row_num : %d, col_num : %d\n", row_num, col_num);

	/*for (int i = 0; i < row + 2; i++) {
		for (int j = 0; j < col + 2; j++) {
			printf("%d ", cell_arr[i][j]);
		}
		printf("\n");
	} */

	for (int i = row_num - 1; i <= row_num + 1; i++) {
		for (int j = col_num - 1; j <= col_num + 1; j++) {
			if (cell_arr[i][j] == 1) {
				alive_cell_cnt++;
			}
		}
	}

	if (cell_arr[row_num][col_num] == 1) {
		alive_cell_cnt -= 1;
	}

	return alive_cell_cnt;
}

