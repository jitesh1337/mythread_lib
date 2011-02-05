/* Single Author info:
 * 	ajalgao	Aditya A Jalgaonkar
 * Group info:
 * 	jhshah	Jitesh H Shah
 * 	salilk	Salil S Kanitkar
 * 	ajalgao	Aditya A Jalgaonkar
 */

#include <unistd.h>
#include <mythread.h>
#include <futex.h>

/* A new thread is created with this wrapper pointer. The aim is to suspend the new
 * thread until it is scheduled by the dispatcher.
 */
int mythread_wrapper(void *thread_tcb)
{
	mythread_private_t *new_tcb;
	new_tcb = (mythread_private_t *) thread_tcb;

	DEBUG_PRINTF("Wrapper: will sleep on futex: %ld %d\n",
		     (unsigned long)__mythread_gettid(),
		     new_tcb->sched_futex.count);

	/* Suspend till explicitly woken up */
	futex_down(&new_tcb->sched_futex);

	DEBUG_PRINTF("Wrapper: futex value: %ld %d\n",
		     (unsigned long)new_tcb->tid, new_tcb->sched_futex.count);

	/* We have been woken up. Now, call the user-defined function */
	new_tcb->start_func(new_tcb->args);

	/* Ideally we shouldn't reach here */
	return 0;
}
