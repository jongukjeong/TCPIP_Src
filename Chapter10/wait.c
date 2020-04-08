#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int status;
	pid_t pid=fork();
	
	if(pid==0)
	{
		return 3; // 첫번째 자식프로세스 종료
	}
	else
	{
		printf("Child PID: %d \n", pid);
		pid=fork();
		if(pid==0)
		{
			exit(7); // 두번째 자식프로세스 종료
		}
		else
		{
			printf("Child PID: %d \n", pid);
			wait(&status);
			if(WIFEXITED(status)) // 첫번째 자식프로세스가 정상종료 했다면
				printf("Child send one: %d \n", WEXITSTATUS(status)); // 종료상태값 확인

			wait(&status);
			if(WIFEXITED(status)) // 두번째 자식프로세스가 정상정료 했다면
				printf("Child send two: %d \n", WEXITSTATUS(status)); // 종료상태값 확인
			sleep(30);     // Sleep 30 sec.
		}
	}
	return 0;
}
