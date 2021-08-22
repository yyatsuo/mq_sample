#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

int main()
{
  int i;
  char buff[10];
  struct mq_attr attr;
  mqd_t recv_que;
 
  recv_que = mq_open("/mq_timed_sample", O_RDONLY|O_CREAT, 0644, NULL);
  mq_getattr(recv_que, &attr);
 
  struct timespec timeout;

  if(-1 == recv_que)
  {
    perror("mq_open error");
  }
  else
  {
    struct timespec timeout;
    while(1)
    {
      if(-1 == clock_gettime(CLOCK_REALTIME, &timeout))
      {
        perror("error");
      }
      timeout.tv_sec += 2;

      if(-1 == mq_timedreceive(recv_que, buff, attr.mq_msgsize, NULL, &timeout))
      {
        if(errno == ETIMEDOUT)
        {
          printf("Timeout!\n");
          continue;
        }
        else
        {
          perror("mq_receive error");
          break;
        }
      }
      printf("received msg: %s \n", buff);
 
      if(0 == strcmp(buff, "close"))
      {
        break;
      }
    }
  }
 
  printf("close and unlink que\n");
  mq_close(recv_que);
  mq_unlink("/mq_timed_sample");
 
  return 0;
}
