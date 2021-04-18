#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define MAX_SIZE 1024

char get_type(mode_t);
char* check_permission(mode_t); 

struct File_info { //파일명과 시간 저장을 위한 구조체
	char filename[50]; //파일명
	time_t file_time; //시간
};

int main (int argc, char* argv[]) {
	DIR *dirptr = NULL; //dirent 포인터
	struct dirent *entry; //dirent 구조체
	struct stat statbuf; //stat 구조체
	char *dir;
	char path[MAX_SIZE];

//=========================<< argc == 1 "myls" 명령어 구현부>>===========================

	if (argc == 1) { //옵션과 파일명 없이 myls 명령어만 입력했을 경우
		dir = "."; //현재 디렉토리로 설정
		if ((dirptr = opendir(dir)) == NULL) { //opendir() 및 예외처리
			fprintf(stderr, "<dir> opendir() error\n");
			exit(1);
		}

		int print_cnt = 0; //ui대로 출력하기 위한 printf() 카운터 변수

		while ((entry = readdir(dirptr)) != NULL) { //readdir() 을 통하여 디렉토리 내의 파일 순회
			if (!strcmp(entry -> d_name, ".") || !strcmp(entry -> d_name, "..")) { // . 과 ..이 나올 경우 계속 진행
				continue;
			}

			else {
				printf("%-20s\t", entry -> d_name); //entry -> d_name출력
				print_cnt++;

				if (print_cnt % 6 == 5) {
					printf("\n");
				}
			}
		}

		printf("\n");
		closedir(dirptr); //디렉토리 포인터 close
	}

	//=========================<< argc == 2 "myls <option> 명령어 구현부 >>=======================

	else if (argc == 2) { //인자가 2개 입력된 경우
		if (strcmp(argv[1], "-i") == 0) { // myls -i
			dir = ".";
			if ((dirptr = opendir(dir)) == NULL) { // open directory
				fprintf(stderr, "<dir> opendir() error\n");
				exit(1);
			}   
			int print_cnt = 0;

			while ((entry = readdir(dirptr)) != NULL) { //파일의 모든 엔트리를 readdir()
				if (!strcmp(entry -> d_name, ".") || !strcmp(entry -> d_name, "..")) {
					continue;
				}

				else {
					printf("%ld %-20s\t", entry -> d_ino, entry -> d_name);
					print_cnt++;

					if (print_cnt % 6 == 5) {
						printf("\n");
					}
				}

				sprintf(path, "%s/%s", dir, entry -> d_name); //파일의 경로 문자열로 생성
				if (lstat(path, &statbuf) < 0) { // stat 구조체에 파일 내용 담아오기
					fprintf(stderr, "<path> lstat() error\n");
				}
			}

			printf("\n");
			closedir(dirptr);
		}

		else if (strcmp(argv[1], "-l") == 0) { //mysl -l
			dir = ".";
			if ((dirptr = opendir(dir)) == NULL) {
				fprintf(stderr, "<dir> opendir() error\n");
				exit(1);
			}

			while ((entry = readdir(dirptr)) != NULL) {
				if (!strcmp(entry -> d_name, ".") || !strcmp(entry -> d_name, "..")) {
					continue;
				}

				else {
					sprintf(path, "%s/%s", dir, entry -> d_name);
					if (lstat(path, &statbuf) < 0) {
						fprintf(stderr, "<path> lstat() error\n");
						exit(1);
					}

					printf("%c%s ", get_type(statbuf.st_mode), check_permission(statbuf.st_mode)); //해당 파일의 권한 출력
					printf("%ld ", statbuf.st_nlink); //해당 파일의 링크 출력
					printf("%s %s ", getpwuid(statbuf.st_uid) -> pw_name, getgrgid(statbuf.st_gid) -> gr_name); //uid와 gid출력
					printf("%6ld ", statbuf.st_size); //해당 파일의 사이즈 출력
					printf("%.12s ", ctime(&statbuf.st_mtime) + 4); //해당 파일의 최종 수정 시간 출력
					printf("%s\n", entry -> d_name); //해당 파일의 파일명 출력
				}
			}

			closedir(dirptr);
		}

		else if (strcmp(argv[1], "-t") == 0) { //myls -t
			int file_cnt = 0; //파일 구조체 개수
			struct File_info file_table[512]; //디렉토리를 순회하며 파일명과 st_mtime을 저장하기 위한 구조체 배열

			dir = "."; //인자가 없는 경우 현재 디렉토리를 dir로 설정

			if ((dirptr = opendir(dir)) == NULL) {
				fprintf(stderr, "<dir> opendir() error\n");
				exit(1);
			}

			while ((entry = readdir(dirptr)) != NULL) {
				if (!strcmp(entry -> d_name, ".") || !strcmp(entry -> d_name, "..")) {
					continue;
				}

				else {
					if (lstat(entry -> d_name, &statbuf) < 0) {
						fprintf(stderr, "lstat() error\n");
						exit(1);
					}

					strcpy(file_table[file_cnt].filename, entry -> d_name); //구조체 변수의 filename에 strcpy
					file_table[file_cnt].file_time = statbuf.st_mtime; //시간 저장

					file_cnt++;
				}

				struct File_info tmp; //구조체 변수를 임시로 저장하기 위한 구조체 변수
				for (int i = 0; i <= file_cnt; i++) {
					for (int j = 0; j < file_cnt - (i + 1); j++) { //버블정렬 진행
						if (file_table[j].file_time > file_table[j+1].file_time) {
							tmp = file_table[j+1];
							file_table[j+1] = file_table[j];
							file_table[j] = tmp;
						}
					}
				}
			}

			for (int i = file_cnt; i >= 0; i--) {
				printf("%s  ", file_table[i].filename);
			}
			printf("\n");

			closedir(dirptr);
		}

//=================<< argc == 2 "myls <filename> 명령어 구현부 >>=======================

		else if (strncmp(argv[1], "-", 1) != 0) { // <myls file_name> or <myls directory_name>
			dir = argv[1]; //인자로 입력받은 <filename> or <directory name>을 경로로 지정

			if(lstat(dir, &statbuf) < 0) {
				fprintf(stderr, "lstat() error\n");
				exit(1);
			}

			if (S_ISDIR(statbuf.st_mode)) { //디렉토리 파일일 경우
				if ((dirptr = opendir(dir)) == NULL) {
					fprintf(stderr, "<dir> opendir() error\n");
					exit(1);
				}

				int print_cnt = 0;
				while ((entry = readdir(dirptr)) != NULL) {
					if (!strcmp(entry -> d_name, ".") || !strcmp(entry -> d_name, "..")) {
						continue;
					}

					else {
						printf("%-20s\t", entry -> d_name);
						print_cnt++;

						if (print_cnt % 6 == 5) {
							printf("\n");
						}
					}
				}

				printf("\n");
				closedir(dirptr);
			}

			else if (S_ISREG(statbuf.st_mode)) { //일반 파일일 경우
				printf("%s\n", dir);
			}
		}
	}

//==============<< argc == 3 "myls <option> <filename or dirname> 명령어 구현부 >>==============

	else if (argc == 3) { // myls <option> <filename or directory_name>의 실행 인자 주어진 경우
		dir = argv[2];

		if (strcmp(argv[1], "-i") == 0) { //myls -i <filename or dirname>
			if (lstat(dir, &statbuf) < 0) {
				fprintf(stderr, "lstat() error\n");
				exit(1);
			}

			if (S_ISDIR(statbuf.st_mode)) {
				if ((dirptr = opendir(dir)) == NULL) {
					fprintf(stderr, "<dir> opendir() error\n");
					exit(1);
				}

				int print_cnt_i = 0;

				while ((entry = readdir(dirptr)) != NULL) {
					if (!strcmp(entry -> d_name, ".") || !strcmp(entry -> d_name, "..")) {
						continue;
					}

					else {
						printf("%ld %-20s\t", entry -> d_ino, entry -> d_name);
						print_cnt_i++;

						if (print_cnt_i % 5 == 4) {
							printf("\n");
						}
					}
				}

				printf("\n");
				closedir(dirptr);
			}

			else if (S_ISREG(statbuf.st_mode)) {
				printf("%ld %s\n", statbuf.st_ino, dir);	
			}
		}

		else if (strcmp(argv[1], "-l") == 0) { //myls -l <filename or dirname>
			if (lstat(dir, &statbuf) < 0) {
				fprintf(stderr, "lstat() error\n");
				exit(1);
			}

			if (S_ISDIR(statbuf.st_mode)) {
				if ((dirptr = opendir(dir)) == NULL) {
					fprintf(stderr, "<dir> opendir() error\n");
					exit(1);
				}

				while ((entry = readdir(dirptr)) != NULL) {
					sprintf(path, "%s/%s", dir, entry -> d_name);
					if (lstat(path, &statbuf) < 0) {
						fprintf(stderr, "lstat() error\n");
						exit(1);
					}

					if (!strcmp(entry -> d_name, ".") || !strcmp(entry -> d_name, "..")) {
						continue;
					}

					printf("%c%s ", get_type(statbuf.st_mode), check_permission(statbuf.st_mode));
					printf("%ld ", statbuf.st_nlink);
					printf("%s %s ", getpwuid(statbuf.st_uid) -> pw_name, getgrgid(statbuf.st_gid) -> gr_name);
					printf("%6ld ", statbuf.st_size);
					printf("%.12s ", ctime(&statbuf.st_mtime) + 4);
					printf("%s\n", entry -> d_name);
				}
			}

			else if (S_ISREG(statbuf.st_mode)) {
				printf("%c%s ", get_type(statbuf.st_mode), check_permission(statbuf.st_mode));
				printf("%ld ", statbuf.st_nlink);
				printf("%s %s ", getpwuid(statbuf.st_uid) -> pw_name, getgrgid(statbuf.st_gid) -> gr_name);
				printf("%6ld ", statbuf.st_size);
				printf("%.12s ", ctime(&statbuf.st_mtime) + 4); 
				printf("%s\n", dir);

			}

			closedir(dirptr);

		}

		else if (strcmp(argv[1], "-t") == 0) { //myls -t <filename or dirname>
			int file_cnt = 0; //파일 구조체 개수
			struct File_info file_table[512];

			if (lstat(dir, &statbuf) < 0) { //인자로 입력받은 파일이 디렉토리인지 파일인지 구별 위해
				fprintf(stderr, "lstat() error\n");
				exit(1);
			}
			
			if (S_ISDIR(statbuf.st_mode)) { //디렉토리 파일인 경우
				if ((dirptr = opendir(dir)) == NULL) {
					fprintf(stderr, "<dir> opendir() error\n");
					exit(1);
				}

				while ((entry = readdir(dirptr)) != NULL) {
					if (!strcmp(entry -> d_name, ".") || !strcmp(entry -> d_name, "..")) {
						continue;
					}
	
					else {
						sprintf(path, "%s/%s", dir, entry -> d_name);
						if (lstat(path, &statbuf) < 0) {
							fprintf(stderr, "<path> lstat() error\n");
							exit(1);
						}
	
						strcpy(file_table[file_cnt].filename, entry -> d_name);
						file_table[file_cnt].file_time = statbuf.st_mtime;

						file_cnt++;
					}

					struct File_info tmp;
					for (int i = 0; i <= file_cnt; i++) {
						for (int j = 0; j < file_cnt - (i + 1); j++) {
							if (file_table[j].file_time > file_table[j+1].file_time) {
								tmp = file_table[j+1];
								file_table[j+1] = file_table[j];
								file_table[j] = tmp;
							}
						}
					}
				}

				for (int i = file_cnt; i >= 0; i--) {
					printf("%s  ", file_table[i].filename);
				}
			}

			else if (S_ISREG(statbuf.st_mode)) { //파일의 경우
				printf("%s\t", dir); 	
			}

			printf("\n");
		}
	}

	return 0;
}

//파일의 타입을 리턴해주는 함수
char get_type(mode_t mode) {
	if (S_ISREG(mode)) return '-';
	if (S_ISDIR(mode)) return 'd';
	if (S_ISCHR(mode)) return 'c';
	if (S_ISBLK(mode)) return 'b';
	if (S_ISLNK(mode)) return 'l';
	if (S_ISFIFO(mode)) return 'p';
	if (S_ISSOCK(mode)) return 's';
}

//파일 사용 권한을 리턴해주는 함수
char* check_permission(mode_t mode) {
	static char perms[10] = "---------";

	for (int i = 0; i < 3; i++) {
		if (mode & (S_IRUSR >> i * 3)) perms[i*3] = 'r';
		if (mode & (S_IWUSR >> i * 3)) perms[i*3+1] = 'w';
		if (mode & (S_IXUSR >> i * 3)) perms[i*3+2] = 'x';
	}

	return(perms);
}
