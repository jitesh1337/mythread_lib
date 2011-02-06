/* Single Author info:
 * 	jhshah 	 Jitesh H Shah
 * Group info:
 * 	jhshah	 Jitesh H Shah
 * 	sskanitk Salil S Kanitkar
 * 	ajalgao	 Aditya A Jalgaonkar
 */

#include <mythread.h>
#include <futex.h>
#include <mythread_q.h>
#include <string.h>
#include <sys/syscall.h>

/* Global futex. Please see the mythread_yield() function for more info */
struct futex gfutex;

/* This function is called from inside yield. It finds a next suitable thread
 * which is in the READY state and wakes it up for execution. 
 *
 * It starts looping from the next thread of the current one and hence, FIFO
 * is ensured.
 */
int __mythread_dispatcher(mythread_private_t * node)
{
	mythread_private_t *ptr = node->next;
	/* Loop till we find a thread in READY state. This loop is guanrateed
	 * to end since idle thread is ALWAYS READY.
	 */
	while (ptr->state != READY)
		ptr = ptr->next;

	/* No other thread is READY. Nothing to do */
	if (ptr == node)
		return -1;
	else {
		DEBUG_PRINTF("Dispatcher: Wake-up:%ld Sleep:%ld %d %d\n",
			     (unsigned long)ptr->tid, (unsigned long)node->tid,
			     ptr->sched_futex.count, ptr->state);

		/* Wake up the target thread. This won't cause any races because
		 * it is protected by a global futex. 
		 */
		futex_up(&ptr->sched_futex);

		DEBUG_PRINTF("Dispatcher: Woken up:%ld, to %d\n",
			     (unsigned long)ptr->tid, ptr->sched_futex.count);
		return 0;
	}
}

/* Yield: Yield the processor to another thread. Dispatcher selects the next
 * appropriate thread and wakes it up. Then current thread sleeps.
 */
int mythread_yield()
{
	mythread_private_t *self;
	int retval;

	self = __mythread_selfptr();

	/* Take the global futex to avoid races in yield. There was a condition 
	 * when after a wake-up the target thread races ahead and entered yield
	 * before the first thread finised. This caused deadlocks and ugly races
	 * when the original thread hadn't slept yet, but was tried to woken up.
	 * So, protect yield by a global futex and make sure the current thread
	 * atleast reduces its futex value to 0, before another one starts.
	 */
	futex_down(&gfutex);

	retval = __mythread_dispatcher(self);
	/* Only one thread. Nothing to do */
	if (retval == -1) {
		futex_up(&gfutex);
		return 0;
	}

	DEBUG_PRINTF("Yield: Might sleep on first down %ld %d\n",
		     (unsigned long)self->tid, self->sched_futex.count);
	/* The "if" condition was to fix a couple of race conditions. The purpose
	 * of two futex down's is to make the process sleep on the second. But
	 * sometimes the value of the futex is already 0, so do a conditional 
	 * down. This, alongwith the global futex, seems to alleviate maximum
	 * races in yield.
	 */
	if (self->sched_futex.count > 0)
		futex_down(&self->sched_futex);

	futex_up(&gfutex);

	DEBUG_PRINTF("Yield: Might sleep on second down %ld %d\n",
		     (unsigned long)self->tid, self->sched_futex.count);
	/* Sleep till another process wakes us up */
	futex_down(&self->sched_futex);

	return 0;
}
