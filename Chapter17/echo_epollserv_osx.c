#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#define BUF_SIZE 100
#define MAX_LENGTH_CHANGE 50
void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	int str_len, i;
	char buf[BUF_SIZE];

	struct kevent kq_events[MAX_LENGTH_CHANGE];
	struct kevent kq_eventlist[MAX_LENGTH_CHANGE];
	int kq, kq_cnt, iEventCnt, iEventListCnt;
    struct timespec tTimeout = {0,10000000};

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
    
    kq=kqueue(); // kqueue 인스턴스 생성
    if(kq == -1) {
        break;
    }
	//k_events=malloc(sizeof(struct kevent)*EPOLL_SIZE); // 구조체 초기화

	//event.events=EPOLLIN; // 수신할 데이터 존재하는 상황
	//event.data.fd=serv_sock;
	//epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event); //serv_sock에 수신할 데이터가 있는 상황을 감지 하도록 등록
    
    kevent(kq,kq_events,iEventCnt,NULL,0,NULL);
    iEventCnt = 0;
    
    iEventCnt = kevent(kq,NULL,0,kq_eventlist,MAX_LENGTH_CHANGE,&tTimeout)

	while(1)
	{
		if(event_cnt > 0)
		{
            for(int i=0;i<iEvent;i++){
                if(ep_events[i].data.fd==serv_sock)
                {
                    adr_sz=sizeof(clnt_adr);
                    clnt_sock=
                    accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    event.events=EPOLLIN;
                    event.data.fd=clnt_sock;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                    printf("connected client: %d \n", clnt_sock);
                }
                else
                {
                    str_len=read(ep_events[i].data.fd, buf, BUF_SIZE);
                    if(str_len==0)    // close request!
                    {
                        epoll_ctl(
                                  epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("closed client: %d \n", ep_events[i].data.fd);
                    }
                    else
                    {
                        write(ep_events[i].data.fd, buf, str_len);    // echo!
                    }
                    
                }
            }
		}
			
	}
	close(serv_sock);
	close(epfd);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
