#include <mythread.h>
#include <malloc.h>
#include <mythread_q.h>

void mythread_exit(mythread_t *p)
{
  mythread_q_delete(p);
  mythread_q_state_display();
  
  mythread_t *q;
  q = mythread_q_search(p->tid);
  if (q == NULL)
    printf("\n Node not present!");
  else
    printf("\n %d",p->state);
}
