// 커널 2.6.1 이상
// 이하는 dnotify.h 사용해야함
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/inotify.h>

int exitflag = 1;

int main(int argc, char * argv[]) {

  if (argc == 1) {
    printf("tail filename\n");
    return 0;
  }

  char fname[256];
  sprintf(fname, "%s", argv[1]);
  FILE * fp = fopen(fname, "r");
  if (!fp) {
    printf("file(%s)open failed (%d:%s)\n", fname, errno, strerror(errno));
    return 0;
  }
    
  fseek(fp, 0, SEEK_END);
  int eventfd = inotify_init();
  if (eventfd < 0) {
    printf("initial inotify failed (%d:%s)\n", errno, strerror(errno));
    return 0;
  }

  int wd = 0;
  wd = inotify_add_watch(eventfd, fname, IN_MODIFY | IN_DELETE_SELF | IN_MOVE_SELF);
  if (wd < 0) {
    printf("add inotify watch failed (%d:%s)\n", errno, strerror(errno));
    return 0;
  }

  while (exitflag) {
    fd_set fdset;
    FD_ZERO( &fdset);
    FD_SET(eventfd, &fdset);
    struct timeval tv;

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    int rc = select(eventfd + 1, &fdset, (fd_set *) NULL, (fd_set *) NULL, &tv);

    if (rc == 0) continue;
    if (rc < 0) {
      printf("select failed (%d:%s)\n", errno, strerror(errno));
      exitflag = 0;
      break;
    }

    if (!FD_ISSET(eventfd, &fdset)) continue;
    char buff[1024];
    int readlen = read(eventfd, buff, sizeof(buff));

    if (readlen < 0) {
      printf("event read failed (%d:%s)\n", errno, strerror(errno));
      exitflag = 0;
      break;
    }

    int eventpos = 0;
    while (eventpos < readlen) {
      struct inotify_event * event = (struct inotify_event *) &buff[eventpos];

      if (event->mask & IN_MODIFY) {
        char readbuff[1024];
        if (fgets(readbuff, sizeof(readbuff), fp) == NULL) continue;
        printf("%s", readbuff);

      } else if (event->mask & IN_DELETE_SELF || event-> mask & IN_MOVE_SELF) {
        printf("file delete or move event recved\n");
        exitflag = 0;
        break;
      }
      eventpos += (sizeof(struct inotify_event) + event->len);
    }
  }

  inotify_rm_watch(eventfd, wd);
  close(eventfd);
  fclose(fp);
  printf("normal terminate (exitflag = 0)\n");
  return 0;
}
