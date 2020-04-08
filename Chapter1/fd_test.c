#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	int fd1, fd2, fd3, fd4;

	fd1 = socket(PF_INET, SOCK_STREAM, 0);
	fd2 = open("test.dat", O_CREAT|O_WRONLY|O_TRUNC);
	fd3 = socket(PF_INET, SOCK_DGRAM, 0);

	printf("파일 디스크립터1 : %d\n", fd1);
	printf("파일 디스크립터2 : %d\n", fd2);
	printf("파일 디스크립터3 : %d\n", fd3);

	close(fd1);
	close(fd2);
	close(fd3);

	fd4 = socket(PF_INET, SOCK_DGRAM, 0);

	printf("파일 디스크립터4 : %d\n", fd4);

	close(fd4);
	return 0;
}