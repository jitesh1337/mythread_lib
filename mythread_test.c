#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<mythread.h>

void *fun(void *arg)
{
	int *ptr = (int *)arg;
	mythread_t t = mythread_self();
	//while(1) {
		printf("Executed this: %d %ld\n", *ptr, (unsigned long)t.tid); fflush(stdout);
		mythread_yield();
	//}
		mythread_exit(NULL);
	return NULL;
}

int main()
{
  mythread_t p[20];
	int a = 10;
  //mythread_attr_t p_attr;
  
   mythread_create(&p[0], NULL , fun, &a);
   mythread_create(&p[1], NULL , fun, &a);
	while(1) {
		printf("I am main\n"); fflush(stdout);
		mythread_yield();
	}
	mythread_exit(NULL);
	while(1);
  /* int i;
  for(i=1;i<=10;i++) {
    p[i].state = i;
    p[i].tid = (unsigned long)i;
   mythread_create(&p[i], NULL , &fun, NULL);
  }

  mythread_exit(&p[5]);
  mythread_exit(&p[2]);
  mythread_exit(&p[6]);
  mythread_exit(&p[9]);
  mythread_exit(&p[10]);
  mythread_exit(&p[3]);
  mythread_exit(&p[8]);
  mythread_exit(&p[7]);
  mythread_exit(&p[1]);
  mythread_exit(&p[4]);

  for(i=12;i<=18;i++) {
    p[i].state = i;
    p[i].tid = i;
    mythread_create(&p[i],&p_attr,NULL,NULL);
  }

  mythread_exit(&p[15]); */

  return 0;
}
