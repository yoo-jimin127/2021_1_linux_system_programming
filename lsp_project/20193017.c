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
void sequential_processing(char* input_filename, int input_gene); //순차처리
void process_parallel_processing(char* input_filename, int input_gene, int child_process_input); //프로세스 병렬처리
void thread_parellel_processing(char* input_filename, int input_gene, int thread_process_input); //스레드 병렬처리
int get_row_cnt(char* input_filename);
int get_col_cnt(char* input_filename);


/* =========================== << main func >> ===========================*/
int main (int argc, char* argv[]) {
	char *input_filename = (char*) malloc(sizeof(char) * 500); //입력파일명 동적할당
	int user_choice = 0; //사용자 입력 메뉴
	int input_gene = 0; //세대 수 입력 값
	int child_process_input = 0; //(3)번 프로세스 병렬처리 메뉴 선택 시 입력될 child process 개수입력
	int thread_input = 0; //(4)번 스레드 병렬처리 메뉴 선택 시 입력될 thread 개수 입력
	
	strcpy(input_filename, argv[1]); //입력받은 argv[1](input.matrix)파일명을 복사 
	
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
				printf("순차처리 입장\n");
				printf("세대 수를 입력하세요 (최소 1세대 이상) : ");
				scanf("%d", &input_gene);
				sequential_processing(input_filename, input_gene);
				break;

			case 3 : // process 병렬처리
				system("clear");
				printf("process 병렬처리 입장\n");
				printf("세대 수를 입력하세요 (최소 1세대 이상) : ");
				scanf("%d", &input_gene);
				printf("생성할 child process의 개수를 입력하세요 : ");
				scanf("%d", &child_process_input);
				process_parallel_processing(input_filename, input_gene, child_process_input);
				break;
			
			case 4 : // thread 병렬처리
				system("clear");
				printf("thread 병렬처리 입장\n");
				printf("세대 수를 입력하세요 (최소 1세대 이상) : ");
				scanf("%d", &input_gene);
				printf("생성할 thread의 개수를 입력하세요 : ");
				scanf("%d", &thread_input);
				thread_parellel_processing(input_filename, input_gene, thread_input);
				break;
		}
	}

	return 0;
}

/*================================ << 순차처리 기능 구현부 >> ================================*/
void sequential_processing(char* input_filename, int input_gene) {
	FILE* fp = NULL; //파일포인터
	long long filesize = 0; //파일 크기 저장 변수
	int **cell_arr; //세포 매트릭스를 저장할 2차원 정수형 배열
	char *input_buf; //파일의 내용을 받아올 버퍼
	char *tmpbuf; //2차원 배열에 저장하기 위한 임시 버퍼
	long row = 0, col = 0; //매트릭스의 행, 열 저장 변수

	if ((fp = fopen(input_filename, "r+")) == NULL) { //파일 내용 읽기 위해 읽기모드로 열기
		fprintf(stderr, "fopen() error <mode : [r+]> in sequential_processing()\n");
		exit(1);
	}

	fseek(fp, 0, SEEK_END); 
	filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET); //파일크기 알아오고 다시 포인터 원위치로
	
	input_buf = (char*)malloc(sizeof(char) * filesize); //파일사이즈만큼 임시버퍼 동적할당
	if (input_buf == NULL) {
		fprintf(stderr, "input_buf memory malloc error\n");
		exit(1);
	}

	fread(input_buf, 1, filesize, fp); //input_buf에 파일의 내용 읽어옴
	printf("%s\n", input_buf);
	
	row = get_row_cnt(input_filename);
	col = get_col_cnt(input_filename); //행과 열의 개수 구하기

	cell_arr = (int**)malloc(sizeof(int*) * row);
	for (int i = 0; i < row; i++) {
		cell_arr[i] = (int *)malloc(sizeof(int) * col);
	} //행과 열의 개수만큼 2차원 배열 동적할당
	
	/*
	tmpbuf = (char *)malloc(sizeof(char) * filesize);
	char *ptr = strtok(input_buf, " ");
	//printf("ptr : %s", ptr);
	int tmpbuf_cnt = 0;
	while (ptr != NULL) {
		strcpy(tmpbuf,ptr);
		ptr = strtok(NULL, " ");
		//tmpbuf_cnt++;
		//strcpy(tmpbuf + tmpbuf_cnt,ptr);
	}

	printf("tmpbuf :%s\n", tmpbuf);
	*/
	int *arr = (int*)malloc(sizeof(int) * row * col);
	for (int i = 0; i < filesize; i++) {
		if (!strncmp(input_buf + i," ", 1) && !strncmp(input_buf + i, "\n", 1)) {
			continue;
		}
		else {
			arr[i] = atoi(input_buf + i);
		}
	}

	for (int i = 0; i < row * col; i++) {
		printf("%d", arr[i]);
	}

	//printf("순차처리 기능이 실행됩니다.\n");
	//printf("%s %d\n", input_filename, input_gene);
	
	return;
}

/*================================ << process 병렬처리 기능 구현부 >> ================================*/
void process_parallel_processing(char* input_filename, int input_gene, int child_process_input) {
	printf("프로세스 병렬처리 기능이 실행됩니다.\n");
	printf("%s %d %d\n", input_filename, input_gene, child_process_input);
	return;
}

/*================================ << thread 병렬처리 기능 구현부 >> ================================*/
void thread_parellel_processing(char* input_filename, int input_gene, int thread_process_input) {
	printf("스레드 병렬처리 기능이 실행됩니다.\n");
	printf("%s %d %d\n", input_filename, input_gene, thread_process_input);
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
