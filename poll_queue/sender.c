#include <mqueue.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <unistd.h>

int main()
{
  int i;
  char* buff;
  mqd_t send_que;
  send_que = mq_open("/mq_epoll_sample", O_CREAT|O_WRONLY|O_NONBLOCK, 0644, NULL);

  if(-1 == send_que)
  {
    perror("mq_open error");
  }
  else
  {
    int epfd = epoll_create(1);
    if(-1 == epfd)
    {
      perror("epoll_create error");
      return 1;
    }

    struct epoll_event send_event, event;
    send_event.data.fd = send_que;
    send_event.events = EPOLLOUT;
    if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, send_que, &send_event))
    {
      perror("epoll_ctl");
      close(epfd);
      return 1;
    }

    for(i=0; i<11; i++)
    {
      if( i < 10) { buff = "hello"; }
      else { buff = "close"; }
      printf("send msg: %s \n", buff);

      while(1)
      {
        int evt_count = epoll_wait(epfd, &event, 1, 1000);
        if(evt_count < 0)
        {
          perror("epoll_wait");
          return -1;
        }
        else if(evt_count == 0)
        {
          printf("Timeout!\n");
          continue;
        }
        else
        {
          if(-1 == mq_send(send_que, buff, sizeof(buff), 0))
          {
            perror("error");
            continue;
          }
          printf("Sent Message #%d \n", i);
          break;
        }
      }
      sleep(1);
    }
  }
  return 0;
}
