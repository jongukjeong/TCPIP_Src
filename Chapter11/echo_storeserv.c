#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int fds[2];
	
	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state=sigaction(SIGCHLD, &act, 0);

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	pipe(fds);
	pid=fork();
	if(pid==0)
	{
		FILE * fp=fopen("echomsg.txt", "wt"); //파일 생성 텍스트모드
		char msgbuf[BUF_SIZE];
		int i, len;

		for(i=0; i<10; i++)
		{
			len=read(fds[0], msgbuf, BUF_SIZE); // 부모프로세스로부터 전달받은 메세지 읽기
			fwrite((void*)msgbuf, 1, len, fp); // 파일로 기록
		}
		fclose(fp);
		return 0;
	}

	while(1)
	{
		adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock==-1)
			continue;
		else
			puts("new client connected...");

		pid=fork();
		if(pid==0)
		{
            close(serv_sock); // 자식 : 불필요 소캣제거
			while((str_len=read(clnt_sock, buf, BUF_SIZE))!=0)
			{
				write(clnt_sock, buf, str_len); //클라이언트소캣으로 응답
				write(fds[1], buf, str_len); // 파일로 기록하기위해 자식프로세스에게 메세지 전달(출력)
			}
			
            close(clnt_sock); // 클라이언트 접속 종료(소캣제거)
			puts("client disconnected...");
			return 0;
		}
		else
			close(clnt_sock); // 부모 : 불필요 소캣제거
	}
	close(serv_sock); // 서버 프로그램 종료직전 소캣제거
	return 0;
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG);
	printf("removed proc id: %d \n", pid);
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
