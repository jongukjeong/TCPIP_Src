#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
    int fd[2];
    int i;
    int n;
    int state;
    char buf[255];
    struct timeval tv;
    fd_set readfds, writefds;

    if ((fd[0] = open("./testfile", O_RDONLY)) == -1)
    {
        perror("file open error : ");
        exit(0);
    }
    if ((fd[1] = open("./testfile2", O_RDONLY)) == -1)
    {
        perror("file open error : ");
        exit(0);
    }

    memset (buf, 0x00, 255);

    for(;;)
    {
        FD_ZERO(&readfds);
        FD_SET(fd[0], &readfds);
        FD_SET(fd[1], &readfds);
        
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        state = select(fd[1]+1, &readfds, NULL, NULL, &tv);
        switch(state)
        {
            case -1:
                perror("select error : ");
                exit(0);
                break;

            default :
                for (i = 0; i < 2; i++)
                {
                    if (FD_ISSET(fd[i], &readfds))
                    {
                        while ((n = read(fd[i], buf, 255)) > 0)
                            printf("(%d) [%d] %s", state, i, buf);
                    }
                }
                memset (buf, 0x00, 255);
                break;
        }
        //usleep(1000);
    }
}

/**

 "/tmp/testfile" 와 "/tmp/testfil2" 2개의 파일을 열어서 출력을 하고 파일 끝까지 가더라도 프로그램을 종료하지 않고, select 를 이용해서 파일에 새로운 내용이 입력되는지 기다리는지를 조사해서 새로운 내용이 입력되면 화면에 출력
 
 */
