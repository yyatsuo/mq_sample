#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
 
int main()
{
  int i;
  char* buff;
  mqd_t send_que;
 
  send_que = mq_open("/mq_sample", O_CREAT|O_WRONLY, 0644, NULL);
 
  if(-1 == send_que)
  {
    perror("mq_open error");
  }
  else
  {
    for(i=0; i<10; i++)
    {
      buff = "hello";
      printf("send msg: %s \n", buff);
 
      if(-1 == mq_send(send_que, buff, sizeof(buff), 0))
      {
        perror("error");
      }
      sleep(1);
    }
 
    buff = "close";
    printf("send msg: %s \n", buff);
    if(-1 == mq_send(send_que, buff, sizeof(buff), 0))
    {
      perror("error");
    }
  }
  return 0;
}
