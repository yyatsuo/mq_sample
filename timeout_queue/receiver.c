#include <mqueue.h>
#include <stdio.h>
#include <string.h>
 
int main()
{
  int i;
  char buff[10];
  struct mq_attr attr;
  mqd_t recv_que;
 
  recv_que = mq_open("/mq_sample", O_RDONLY|O_CREAT, 0644, NULL);
  mq_getattr(recv_que, &attr);
 
  if(-1 == recv_que)
  {
    perror("mq_open error");
  }
  else
  {
    while(1)
    {
      if(-1 == mq_receive(recv_que, buff, attr.mq_msgsize, NULL))
      {
        perror("mq_receive error");
        break;
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
  mq_unlink("/mq_sample");
 
  return 0;
}
