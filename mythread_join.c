#include <mythread.h>
#include <mythread_q.h>
#include <sys/syscall.h>

pid_t gettid()
{
	return (pid_t)syscall(SYS_gettid);
}

int mythread_join(mythread_t target_thread, void **status)
{
	mythread_t *target, *self_ptr;
	
	self_ptr = mythread_q_search(gettid());
	target = mythread_q_search(target_thread.tid);

	if (target->state == DEFUNCT) {
		*status = target->returnValue;
		return 0;
	}
	
	/* someone else is already waiting for join */
	if (target->blockedForJoin == NULL)
		return -1;

	target->blockedForJoin = self_ptr;
	self_ptr->state = BLOCKED;
	mythread_yield();
	return 0;
}
