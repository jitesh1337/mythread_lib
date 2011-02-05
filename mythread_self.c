/* Single Author info:
 * 	ajalgao	Aditya A Jalgaonkar
 * Group info:
 * 	jhshah	Jitesh H Shah
 * 	salilk	Salil S Kanitkar
 * 	ajalgao	Aditya A Jalgaonkar
 */

#include <malloc.h>
#include <mythread.h>
#include <mythread_q.h>

/* Return the user exposed handle to the current thread */
mythread_t mythread_self()
{
	pid_t tid;
	mythread_t self_tcb;

	/* Get our tid */
	tid = __mythread_gettid();
	self_tcb.tid = tid;

	/* Return by value */
	return (self_tcb);
}

/* Return pointed to the private TCB structure */
mythread_private_t *__mythread_selfptr()
{
	/* Search in the queue and return the pointer */
	return mythread_q_search(__mythread_gettid());
}
