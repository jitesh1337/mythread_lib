#include <mythread.h>
#include <mythread_q.h>
#include <sys/syscall.h>

int mythread_join(mythread_t target_thread, void **status)
{
	mythread_private_t *target, *self_ptr;

	self_ptr = __mythread_selfptr();
	DEBUG_PRINTF("Join: Got tid: %ld\n", (unsigned long)self_ptr->tid);
	target = mythread_q_search(target_thread.tid);

	if (target->state == DEFUNCT) {
		*status = target->returnValue;
		return 0;
	}

	DEBUG_PRINTF("Join: Checking for blocked for join\n");
	/* someone else is already waiting for join */
	if (target->blockedForJoin != NULL)
		return -1;

	target->blockedForJoin = self_ptr;
	DEBUG_PRINTF("Join: Setting state of %ld to %d\n",
		     (unsigned long)self_ptr->tid, BLOCKED);
	self_ptr->state = BLOCKED;
	mythread_yield();
	*status = target->returnValue;
	return 0;
}
