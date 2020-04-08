#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int cfd1, cfd2;
	char str1[]="Hi~ \n";
	char str2[]="It's nice day~ \n";

	cfd1=dup(1); // 파일디스트립터 1 복사
	cfd2=dup2(cfd1, 7); // 복사한 1을 재복사 하고 7 할당
	
	printf("fd1=%d, fd2=%d \n", cfd1, cfd2);
	write(cfd1, str1, sizeof(str1));
	write(cfd2, str2, sizeof(str2));
	
	close(cfd1); // 3 종료
	close(cfd2); // 7 종료
	write(1, str1, sizeof(str1));
	close(1); // 1 종료
	write(1, str2, sizeof(str2));
	return 0;
}
