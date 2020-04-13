#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

static void *clnt_connection(void *arg);
void sendOk(FILE* fp);
void sendError(FILE* fp);

#define BUFSIZE 1024
#define LINESIZE 512
int main(int argc, char **argv)
{
    int serv_sock;
    int clnt_sock;
    pthread_t thread;
    struct sockaddr_in serv_addr, clnt_addr;
    unsigned int clnt_addr_size;

    if(argc!=2) {
        printf("usage: %s <port>\n", argv[0]);
        return -1;
    }
    serv_sock=socket(PF_INET, SOCK_STREAM,0);
    if(serv_sock==-1) {
        perror("socket() error");
        return -1;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = (argc!=2)?htons(8000):htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1) {
      perror("bind() error");
      return -1;
    }

    if(listen(serv_sock, 10) == -1) {
      perror("listen() error");
      return -1;
    }
  while(1) {
    clnt_addr_size=sizeof(clnt_addr);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    printf("Client IP : %s:%d\n", \
    inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    pthread_create(&thread,NULL,clnt_connection,&clnt_sock);
    pthread_join(thread, 0);
  };

  return 0;
}

void *clnt_connection(void *arg)
{
  int clnt_sock=*((int*)arg), clnt_fd;
  char reg_line[LINESIZE], reg_buf[LINESIZE];
  FILE *clnt_read, *clnt_write;
char method[10], ct[LINESIZE], type[LINESIZE];
char file_name[256], file_buf[256];
char* type_buf;
int i = 0, j = 0, len = 0;

clnt_read=fdopen(clnt_sock,"r");
clnt_write=fdopen(dup(clnt_sock),"w");
clnt_fd=clnt_sock;

fgets(reg_line, LINESIZE, clnt_read);
fputs(reg_line, stdout);

strcpy(method, strtok(reg_line,"/ "));
if(strcmp(method,"POST")==0) {
  sendOk(clnt_write);
  fclose(clnt_read);
  fclose(clnt_write);
  return (void*)NULL;
} else if(strcmp(method,"GET")!=0){
  sendError(clnt_write);
  fclose(clnt_read);
  fclose(clnt_write);
  return (void*)NULL;
}

strcpy(file_name,strtok(NULL," "));
if(file_name[0] == '/') {
  for(i = 0, j = 0; i < LINESIZE; i++) {
    if(file_name[0] == '/') j++;
    file_name[i] = file_name[j++];
    if(file_name[i+1] == '\0') break;
  };
}
  do {
    fgets(reg_line, LINESIZE, clnt_read);
    fputs(reg_line, stdout);
    strcpy(reg_buf, reg_line);
    type_buf = strchr(reg_buf, ':');
    if(!strncmp(reg_buf,"Content-Type", 12));
  } while(strncmp(reg_line, "\r\n",2));

  strcpy(file_buf, file_name);
  sendData(clnt_fd, clnt_write, ct, file_name);

  fclose(clnt_read);
  fclose(clnt_write);
  pthread_exit(0);

  return (void*)NULL;
}

int sendData(int fd, FILE* fp, char *ct, char *file_name)
{
  char protocol[]="HTTP/1.1 200 OK\r\n";
  char server[]="Server:Netscape-Enterprise/6.0\r\n";
  char cnt_type[]="Content-Type:text/html\r\n";
  char end[]="\r\n";
  char buf[BUFSIZE];
  int len;

  fputs(protocol,fp);
  fputs(server,fp);
  fputs(cnt_type,fp);
  fputs(end,fp);
  fflush(fp);

  fd = open(file_name, O_RDWR);

  do {
    len = read(fd, buf, BUFSIZE);
    fwrite(buf, len, sizeof(char), fp);
  } while(len == BUFSIZE);

  fflush(fp);
  close(fd);

  return 0;
}

void sendError(FILE* fp)
{
  char protocol[]="HTTP/1.1 400 Bad Request\r\n";
  char server[]="Server:Best HTTP Server\r\n";
  char cnt_len[]="Content-Length:1024\r\n";
  char cnt_type[]="Content-Type:text/html\r\n\r\n";
  char content1[]="<html><head><title>BAD Connection</tiitle></head>";
  char content2[]="<body><font size=+5>Bad Request</font></body></html>";

  printf("send_error\n");
  fputs(protocol,fp);
  fputs(server,fp);
  fputs(cnt_len,fp);
  fputs(cnt_type,fp);
  fputs(content1,fp);
  fputs(content2,fp);
  fflush(fp);

  return;
}

void sendOk(FILE* fp)
{
  char protocol[]="HTTP/1.1 200 OK\r\n";
  char server[]="Server:Best HTTP Server\r\n\r\n";

  fputs(protocol,fp);
  fputs(server,fp);
  fflush(fp);

  return;
}
