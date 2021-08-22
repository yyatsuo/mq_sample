#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h>

int main()
{
  int i;
  char buff[10];
  struct mq_attr attr;
  mqd_t recv_que;
 
  recv_que = mq_open("/mq_epoll_sample", O_RDONLY|O_CREAT|O_NONBLOCK, 0644, NULL);
  mq_getattr(recv_que, &attr);
 

  if(-1 == recv_que)
  {
    perror("mq_open error");
  }
  else
  {
    // 1: Create epoll fd
    // epoll_create() の引数は後方互換のためのものであり最近のカーネルだと意味は特にない
    // 詳細は man epoll
    int epfd = epoll_create(1); 
    if(-1 == epfd)
    {
      perror("epoll_create1 error");
      return 1;
    }

    // 2: Add fd for epoll to watch
    // fd recv_que の POLLIN を監視する
    struct epoll_event recv_event, event;
    recv_event.data.fd = recv_que;
    recv_event.events = EPOLLIN;
    if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, recv_que, &recv_event))
    {
      perror("epoll_ctl");
      close(epfd);
      return 1;
    }

    while(1)
    {
      printf("Start epoll\n");
      int evt_count = epoll_wait(epfd, &event, 1, 3000);

      // queにメッセージが入っていれば抜けてくる
      if(evt_count < 0)
      {
        perror("epoll_wait error");
        break;
      }
      else if(evt_count == 0)
      {
        printf("Timeout!\n");
        continue;
      }

      // 入っているメッセージ全部抜く
      while(-1 != mq_receive(recv_que, buff, attr.mq_msgsize, NULL))
      {
        printf("received msg: %s \n", buff);
        if(0 == strcmp(buff, "close"))
        {
          goto end;
        }
      }
    }
    end:
    close(epfd);
  }

  printf("close and unlink que\n");
  mq_close(recv_que);
  mq_unlink("/mq_epoll_sample");

  return 0;
}
