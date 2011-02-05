#include <unistd.h>
#include <mythread.h>
#include <futex.h>


void *mythread_idle(void *phony)
{
	mythread_private_t *traverse_tcb;
	pid_t idle_tcb_tid;

	while (1) {
		DEBUG_PRINTF("I am idle\n");
		traverse_tcb = __mythread_selfptr();
		idle_tcb_tid = traverse_tcb->tid;
		traverse_tcb = traverse_tcb->next;

		while (traverse_tcb->tid != idle_tcb_tid) {
			if (traverse_tcb->state != DEFUNCT) {
				break;
			}
			traverse_tcb = traverse_tcb->next;
		}

		if (traverse_tcb->tid == idle_tcb_tid)
			exit(0);

		mythread_yield();
	}
}
