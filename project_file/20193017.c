#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>

#define MAX_SIZE 1024

/* ========================== << 함수 정의부 >> ========================*/
void sequential_processing(int** cell_arr, int input_gene); //순차처리
void process_parallel_processing(int** cell_arr, int input_gene, int child_process_input); //프로세스 병렬처리
void thread_parellel_processing(int** cell_arr, int input_gene, int thread_input); //스레드 병렬처리
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

	clock_t start, finish;
	double duration;

	if(argv[1] == NULL) {
		fprintf(stderr, "input.matrix 파일명을 입력하세요.\n");
		exit(1);
	}
	strcpy(input_filename, argv[1]); //입력받은 argv[1](input.matrix)파일명을 복사

	start = clock();
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
				finish = clock();
				duration = (double)(finish - start);
				printf("#순차처리 %d세대 진행 총 수행시간 : %lfms\n\n", input_gene, duration);
				break;

			case 3 : // process 병렬처리
				system("clear");
				printf("======process 병렬처리======\n");
				printf("생성할 child process의 개수를 입력하세요 : ");
				scanf("%d", &child_process_input);
				printf("진행할 세대 수를 입력하세요 (최소 1세대 이상) : ");
				scanf("%d", &input_gene);

				process_parallel_processing(cell_arr, input_gene, child_process_input);
				finish = clock();
				duration = (double)(finish - start);
				printf("#프로세스 병렬처리 %d세대 진행 총 수행시간 : %lfms\n\n", input_gene, duration);
				break;

			case 4 : // thread 병렬처리
				system("clear");
				printf("======thread 병렬처리======\n");
				printf("생성할 thread의 개수를 입력하세요 : ");
				scanf("%d", &thread_input);
				printf("진행할 세대 수를 입력하세요 (최소 1세대 이상) : ");
				scanf("%d", &input_gene);

				thread_parellel_processing(cell_arr, input_gene, thread_input);
				finish = clock();
				duration = (double)(finish - start);
				printf("#스레드 병렬처리 %d세대 진행 총 수행시간 : %lfms\n\n", input_gene, duration);
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
	char *new_filename = (char *)malloc(sizeof(char) * MAX_SIZE); //다음 세대 파일 이름 저장 배열
	FILE *new_fp = NULL; //다음 세대 매트릭스 파일 생성을 위한 파일포인터
	int **working_cell_arr; //인자로 전달받은 매트릭스를 복사해 작업하기 위한 목적의 배열
	int **new_cell_arr; //다음 단계에서 생성될 매트릭스
	int alive_cell_cnt = 0; //살아있는 이웃세포의 개수를 저장하기 위한 변수
	char tmp_cell[10] = ""; //다음 세대의 파일 내용을 저장할 때 필요한 세포 저장 임시 버퍼
	char *space_bar = " "; //공백문자
	char *enter = "\n"; //개행문자

	pid_t pids[child_process_input], pid;
	int row_cnt = 0;
	int remainder_row = row;
	int rows_per_child = 0; //프로세스 별로 담당해야하는 행의 개수 저장 변수
	int gene_cnt = 0; //세대 카운트
	int status;
	int file_pointer = 0;

	working_cell_arr = (int **)malloc(sizeof(int *) * (row + 2)); //가장자리의 이웃세포+2 만큼 동적할당
	for (int i = 0; i < row + 2; i++) {
		working_cell_arr[i] = (int *)malloc(sizeof(int) * (col + 2));
	}

	new_cell_arr = (int **)malloc(sizeof(int *) * row); //새롭게 생성되는 다음 세대의 파일 동적할당
	for (int i = 0; i < row; i++) {
		new_cell_arr[i] = (int *)malloc(sizeof(int) * col);
	}

	for (int i = 0; i < row; i++) { //새로 생성된 배열 값 초기화
		for (int j = 0; j < col; j++) {
			new_cell_arr[i][j] = 0;
		}
	}

	for (int i = 0; i < row + 2; i++) {
		for (int j = 0; j < col + 2; j++) {
			working_cell_arr[i][j] = cell_arr[i][j]; //인자로 전달받은 값 이 곳에 할당
		}
	}

	/*for (int i = 0; i < row + 2; i++) { //8 * 7 배열 기준으로 working_cell_arr는 [10][9], 실제 데이터는 [1][1] ~ [8][7]에 들어있음 
	  for (int j = 0; j < col + 2; j++) {
	  printf("%d ", working_cell_arr[i][j]);
	  }
	  printf("\n");
	  }*/


	//-----------------------------게임 진행-------------------------
	//자식 프로세스 별로 담당해야하는 줄 수 배분 작업
	for (gene_cnt = 0; gene_cnt < input_gene; gene_cnt++) {	
		if (gene_cnt < input_gene -1) {
			sprintf(new_filename, "gen_%d.matrix", gene_cnt + 1);
			printf("%s\n", new_filename);
			if ((new_fp = fopen(new_filename, "a+")) == NULL) {
				fprintf(stderr, "new_fp fopen() error <mode : [a+]> in process_parellel_processing.\n");
				exit(1);
			}
			printf("파일열림\n");
		//	fseek(new_fp, 0, SEEK_SET);
			printf("파일포인터\n");
		}
		
		else if (gene_cnt == input_gene -1) {
			sprintf(new_filename, "output.matrix");
			if ((new_fp = fopen(new_filename, "a+")) == NULL) {
				fprintf(stderr, "new_fp fopen() error <mode : [a+]> in process_parellel_processing.\n");
				exit(1);
			}
		}
		printf("------gene_cnt : %d------\n", gene_cnt + 1);
		for (int i = 0; i < child_process_input; i++) {
			if ((remainder_row % (child_process_input - i)) != 0) { //자식 프로세스의 개수가 input.matrix의 개수로 나누어떨어지지 않는 경우
				rows_per_child = (remainder_row / (child_process_input - i)) + 1; //1씩 더해 균등 배분
				printf("step %d case A - rows_per_child : %d\n", i, rows_per_child);
			}

			else if ((remainder_row % (child_process_input - i)) == 0) { //자식 프로세스의 개수가 input.matrix의 개수로 나누어떨어지는 경우
				rows_per_child = remainder_row / (child_process_input - i);
				printf("step %d case B - remainder_row : %d ,rows_per_child : %d\n", i, remainder_row, rows_per_child);
			}

			if ((pids[i] = fork()) < 0) { //자식 프로세스 생성
				fprintf(stderr, "child_process[%d] : fork() error\n", i);
				exit(1);
			}
			else if (pids[i] == 0) { //자식 프로세스
				printf("pids[%d] : %d\n", i, getpid());
				for (int j = row_cnt + 1; j <= row_cnt + rows_per_child; j++) { //실제 데이터가 저장된 1행부터 시작
					for (int k = 1; k <= col; k++) { //실제 데이터가 저장된 1열부터 시작
						alive_cell_cnt = get_alive_cell(working_cell_arr, j, k);
						if ((working_cell_arr[j][k] == 1) && (3 <= alive_cell_cnt) && (alive_cell_cnt <= 6)) {
							new_cell_arr[j-1][k-1] = 1;
						}
						else if ((working_cell_arr[j][k] == 0) && (alive_cell_cnt == 4)) {
							new_cell_arr[j-1][k-1] = 1; //각 child프로세스가 맡은 행의 내용을 new_cell_arr에 업데이트 완료
						}
					}
				}

				/*for (int w = row_cnt + 1; w <= row_cnt + rows_per_child; w++) {
					for (int z = 1; z <= col; z++) {
						printf("%d ", new_cell_arr[w-1][z-1]);
					}
					printf("\n");
				}*/
				//배열에 값 넣는 작업
				memset(tmp_cell, 0, sizeof(char) * 10);
				for (int c = row_cnt + 1; c <= row_cnt + rows_per_child; c++) {
					for (int d = 1; d <= col; d++) {
						sprintf(tmp_cell, "%d ", new_cell_arr[c-1][d-1]);
					//	printf("%s", tmp_cell);
						fseek(new_fp, 0, SEEK_CUR);
						fwrite(tmp_cell, 1, strlen(tmp_cell), new_fp);
					//	fwrite(space_bar, 1, sizeof(char), new_fp);
					//	if (col == d + 1) {
					//		fwrite(enter, 1, sizeof(char), new_fp);
					//	}
					}
					//printf("\n");
					fwrite(enter, 1, sizeof(char), new_fp);
				}

				exit(0);
			} //pids[i] == 0 분기문 작업 끝
			
			else if (pids[i] > 0) {
				if ((wait(&status)) != pids[i]) {
					fprintf(stderr, "pids[%d] wait() error.\n", i);
					exit(1);
				}
			}
			remainder_row -= rows_per_child;
			printf("remainder_row : %d at gene_cnt : %d\n", remainder_row, gene_cnt + 1);
			row_cnt += rows_per_child;
			printf("row_cnt : %d at gene_cnt : %d\n", row_cnt, gene_cnt + 1);

		}//child_process_input만큼의 반복문
		remainder_row = row;
		row_cnt = 0;
	//	fclose(new_fp);
		printf("작업 끝\n");
	}//input_gene만큼의 반복문

	return;
}

/*================================ << thread 병렬처리 기능 구현부 >> ================================*/
void thread_parellel_processing(int** cell_arr, int input_gene, int thread_input) {
	printf("스레드 병렬처리 기능이 실행됩니다.\n");
	//printf("%s %d %d\n", input_filename, input_gene, thread_num_input);
	char *new_filename = (char *)malloc(sizeof(char) * MAX_SIZE);
	FILE *new_fp = NULL; //다음 세대 매트릭스 파일 생성을 위한 파일포인터
	FILE *final_fp = NULL; //output.matrix 파일 생성을 위한 파일포인터
	int **working_cell_arr; //인자로 전달받은 매트릭스 복사해 작업하기 위한 목적의 배열
	int **new_cell_arr; //다음 단계에서 생성될 매트릭스
	int alive_cell_cnt = 0; //살아있는 이웃세포의 개수를 저장하기 위한 변수
	char tmp_cell[10] = ""; //다음 세대의 파일 내용을 저장할 때 필요한 세포 저장 임시 버퍼
	char *space_bar = " "; //공백문자
	char *enter = "\n"; //개행문자

	pthread_t tids[thread_input]; //스레드 아이디
	pid_t pids[thread_input];
	int row_cnt = 0;
	int remainder_row = row;
	int rows_per_thread = 0;
	int gene_cnt = 0;

	working_cell_arr = (int **)malloc(sizeof(int *) * (row + 2)); //가장자리의 이웃세포까지 만들기
	for (int i = 0; i < row + 2; i++) {
		working_cell_arr[i] = (int *)malloc(sizeof(int) * (col + 2));
	}

	new_cell_arr = (int **)malloc(sizeof(int *) * row); //새롭게 생성되는 다음 세대의 파일 내용
	for (int i = 0; i < row; i++) {
		new_cell_arr[i] = (int *)malloc(sizeof(int) * col);
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			new_cell_arr[i][j] = 0; //인자로 전달받은 배열의 값 이 곳에 할당
		}
	}

	for (int i = 0; i < row + 2; i++) {
		for (int j = 0; j < col + 2; j++) {
			working_cell_arr[i][j] = cell_arr[i][j];
		}
	}

	/*for (int i = 0; i < row + 2; i++) {
	  for (int j = 0; j < col + 2; j++) {
	  printf("%d ", working_cell_arr[i][j]);
	  }
	  printf("\n");
	  }*/

	for (int i = 0; i < thread_input; i++) {
		if ((remainder_row % (thread_input - i)) != 0) {
			rows_per_thread = (remainder_row / (thread_input - i)) + 1;
			printf("step %d case A - rows_per_thread : %d\n", i, rows_per_thread);
		}

		else if ((remainder_row % (thread_input - i)) == 0) {
			rows_per_thread = remainder_row / (thread_input - i);
			printf("step %d case B - rows_per_thread : %d\n", i, rows_per_thread);
		}

	}


	return;
}

void *tid_print(void *arg) {

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

