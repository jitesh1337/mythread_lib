/* Single Author info:
 * 	jhshah 	Jitesh H Shah
 * Group info:
 * 	jhshah	Jitesh H Shah
 * 	salilk	Salil S Kanitkar
 * 	ajalgao	Aditya A Jalgaonkar
 */

#include <mythread.h>
#include <mythread_q.h>
#include <sys/syscall.h>

/* Wait on the thread specified by "target_thread". If the thread is DEFUNCT,
 * just collect the return status. Else, wait for the thread to die and then
 * collect the return status
 */
int mythread_join(mythread_t target_thread, void **status)
{
	mythread_private_t *target, *self_ptr;

	self_ptr = __mythread_selfptr();
	DEBUG_PRINTF("Join: Got tid: %ld\n", (unsigned long)self_ptr->tid);
	target = mythread_q_search(target_thread.tid);

	/* If the thread is already dead, no need to wait. Just collect the return
	 * value and exit
	 */
	if (target->state == DEFUNCT) {
		*status = target->returnValue;
		return 0;
	}

	DEBUG_PRINTF("Join: Checking for blocked for join\n");
	/* If the thread is not dead and someone else is already waiting on it
	 * return an error
	 */
	if (target->blockedForJoin != NULL)
		return -1;

	/* Add ourselves as waiting for join on this thread. Set our state as
	 * BLOCKED so that we won't be scheduled again.
	 */
	target->blockedForJoin = self_ptr;
	DEBUG_PRINTF("Join: Setting state of %ld to %d\n",
		     (unsigned long)self_ptr->tid, BLOCKED);
	self_ptr->state = BLOCKED;

	/* Schedule another thread */
	mythread_yield();

	/* Target thread died, collect return value and return */
	*status = target->returnValue;

	/* TODO: delete the node? */
	return 0;
}
