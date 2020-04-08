#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	FD_ZERO(&reads);
	FD_SET(0, &reads); // 0 is standard input(console) 표준입력을 대상으로 변화를 검출 하겠다는뜻

	/* 이 위치에서 timeout 값을 설정하면 타임아웃이 발생하기까지 남았던 시간으로 바뀌기 때문에
	timeout.tv_sec=5;
	timeout.tv_usec=5000;
	*/

	while(1)
	{
		temps=reads; // select()호출 하면 값이 변하므로 임시 복사
		timeout.tv_sec=5;
		timeout.tv_usec=0;
		result=select(1, &temps, 0, 0, &timeout); // 콘솔로부터 입력된 데이터가 있다면 0보다 큰수 반환, 입려된 데이터가 없으면(타임아웃발생) 0반환
		if(result==-1)
		{
			puts("select() error!");
			break;
		}
		else if(result==0) // 타임아웃 발생
		{
			puts("Time-out!");
		}
		else 
		{
			if(FD_ISSET(0, &temps)) // select()호출결과 0보다 큰수 이면 해당 디스크립터를 읽어서 내용 확인
			{
				str_len=read(0, buf, BUF_SIZE);
				buf[str_len]=0;
				printf("message from console: %s", buf);
			}
		}
	}
	return 0;
}
