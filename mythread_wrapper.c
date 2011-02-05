#include <unistd.h>
#include <mythread.h>
#include <futex.h>

int mythread_wrapper(void *thread_tcb)
{
	mythread_private_t *new_tcb;
	new_tcb = (mythread_private_t *) thread_tcb;

	DEBUG_PRINTF("Wrapper: will sleep on futex: %ld %d\n",
		     (unsigned long)__mythread_gettid(),
		     new_tcb->sched_futex.count);
	futex_down(&new_tcb->sched_futex);
	DEBUG_PRINTF("Wrapper: futex value: %ld %d\n",
		     (unsigned long)new_tcb->tid, new_tcb->sched_futex.count);
	new_tcb->start_func(new_tcb->args);

	/* Ideally we shouldn't reach here */
	return 0;
}
