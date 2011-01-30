#include <mythread.h>
#include <malloc.h>
#include <mythread_q.h>

int mythread_create(mythread_t *new_thread_ID,
		    mythread_attr_t *attr,
		    void * (*start_func)(void *),
                                        void *arg)
{

  mythread_q_add(new_thread_ID);

  return 1;
}
