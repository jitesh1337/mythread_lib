#include <malloc.h>
#include <mythread.h>
#include <mythread_q.h>

mythread_t mythread_self()
{
	pid_t tid;
	tid = __mythread_gettid();

	mythread_private_t *self_priv_tcb;
	mythread_t self_tcb;
	self_priv_tcb = (mythread_q_search(tid));
	self_tcb.tid = self_priv_tcb->tid;
	return (self_tcb);
}

mythread_private_t *__mythread_selfptr()
{
	return mythread_q_search(__mythread_gettid());
}
