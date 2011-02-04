#include <malloc.h>
#include <mythread.h>
#include <mythread_q.h>

  
mythread_t mythread_self()
{
  pid_t tid;
  tid = __mythread_gettid();

  return *(mythread_q_search(tid));
}

mythread_t *__mythread_selfptr()
{
	return mythread_q_search(__mythread_gettid());
}
