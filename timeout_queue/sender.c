#include <mqueue.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main()
{
  int i;
  char* buff;
  mqd_t send_que;
  send_que = mq_open("/mq_timed_sample", O_CREAT|O_WRONLY, 0644, NULL);

  if(-1 == send_que)
  {
    perror("mq_open error");
  }
  else
  {
    struct timespec timeout;
    for(i=0; i<11; i++)
    {
      if(-1 == clock_gettime(CLOCK_REALTIME, &timeout))
      {
        perror("clock_gettime error");
      }
      timeout.tv_sec += 10;

      if( i < 10) { buff = "hello"; }
      else { buff = "close"; }
      printf("send msg: %s \n", buff);

      if(-1 == mq_timedsend(send_que, buff, sizeof(buff), 0, &timeout))
      {
        perror("mq_timedsend error");
      }
      sleep(1);
    }
  }
  return 0;
}
