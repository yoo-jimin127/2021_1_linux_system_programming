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

char type(mode_t);
char* perm(mode_t);
void getMtime(struct stat*, char*);
void sortByMtime(char *);

int main (int argc, char* argv[]) {
	DIR *dirptr = NULL; //dirent 포인터
	struct dirent *entry; //dirent 구조체
	struct stat statbuf; //stat 구조체
	char *dir;
	char path[MAX_SIZE];

//=========================<< argc == 1 "myls" 명령어 구현부>>===========================

	if (argc == 1) { //옵션과 파일명 없이 myls 명령어만 입력했을 경우
		dir = "."; //현재 디렉토리로 설정
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

					printf("%5ld ", statbuf.st_blocks);
					printf("%c%s ", type(statbuf.st_mode), perm(statbuf.st_mode));
					printf("%3ld ", statbuf.st_nlink);
					printf("%s %s ", getpwuid(statbuf.st_uid) -> pw_name, getgrgid(statbuf.st_gid) -> gr_name);
					printf("%9ld ", statbuf.st_size);
					printf("%.12s ", ctime(&statbuf.st_mtime) + 4);
					printf("%s\n", entry -> d_name);
				}
			}
			
			closedir(dirptr);
		}

		else if (strcmp(argv[1], "-t") == 0) { //myls -t
			//char buf[MAX_SIZE] = "";
			//struct tm *mtime;

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
						fprintf(stderr, "<path> lstat{} error\n");
						exit(1);
					}
					
					//int cnt = 0;
					//buf[cnt] = sortByMtime(&statbuf, entry -> d_name);
				}
			}

			closedir(dirptr);
		}

//======================<< argc == 2 "myls <filename> 명령어 구현부 >>======================

		else if (strncmp(argv[1], "-", 1) != 0) { // <myls file_name> or <myls directory_name>
			dir = argv[1]; //인자로 입력받은 <filename> or <directory name>을 경로로 지정
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
	}

//==============<< argc == 3 "myls <filename or dirname> <option> 명령어 구현부 >>==============

	else if (argc == 3) { // myls <filename or directory_name> <option> 의 실행 인자 주어진 경우
		dir = argv[1];

		if (strcmp(argv[2], "-i") == 0) { //myls <filename or dirname> -i
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

		else if (strcmp(argv[2], "-l") == 0) { //myls <filename or dirname> -l
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
					if(lstat(path, &statbuf) < 0) {
						fprintf(stderr, "<path> lstat() error\n");
						exit(1);
					}

					printf("%5ld ", statbuf.st_blocks);
					printf("%c%s ", type(statbuf.st_mode), perm(statbuf.st_mode));
					printf("%3ld ", statbuf.st_nlink);
					printf("%s %s ", getpwuid(statbuf.st_uid) -> pw_name, getgrgid(statbuf.st_gid) -> gr_name);
					printf("%9ld ", statbuf.st_size);
					printf("%.12s ", ctime(&statbuf.st_mtime) + 4);
					printf("%s\n", entry -> d_name);
				}
			}

			closedir(dirptr);
		}

		else if (strcmp(argv[2], "-t") == 0) { //myls <filename or dirnme> -t
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

					//구현부
				}
			}
		}
	}

	return 0;
}

//파일의 타입을 리턴해주는 함수
char type(mode_t mode) {
	if (S_ISREG(mode)) return '-';
	if (S_ISDIR(mode)) return 'd';
	if (S_ISCHR(mode)) return 'c';
	if (S_ISBLK(mode)) return 'b';
	if (S_ISLNK(mode)) return 'l';
	if (S_ISFIFO(mode)) return 'p';
	if (S_ISSOCK(mode)) return 's';
}

//파일 사용 권한을 리턴해주는 함수
char* perm(mode_t mode) {
	static char perms[10] = "---------";
	
	for (int i = 0; i < 3; i++) {
		if (mode & (S_IRUSR >> i * 3)) perms[i*3] = 'r';
		if (mode & (S_IWUSR >> i * 3)) perms[i*3+1] = 'w';
		if (mode & (S_IXUSR >> i * 3)) perms[i*3+2] = 'x';
	}

	return(perms);
}

// -t 옵션에서 mtime 기준으로 정렬하는 함수
void getMtime(struct stat *statbuf, char *filename) {
	char str_mtime[MAX_SIZE] = "";
	char buf[MAX_SIZE] = "";
	struct tm *mtime;

	mtime = localtime(&statbuf -> st_mtime);
	sprintf(str_mtime, "%04d%02d%02d%02d%02d%02d %s", mtime -> tm_year + 1900, mtime -> tm_mon + 1, mtime -> tm_mday, mtime -> tm_hour, mtime -> tm_min, mtime -> tm_sec, filename);
	
	//printf("str_mtime : %s\n", str_mtime);

	//return str_mtime;
}

//토큰을 분리하여 저장하는 변수
void sortByMtime(char *str_mtime) {

}
