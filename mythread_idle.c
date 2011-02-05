/* Single Author info:
 * 	jhshah 	Jitesh H Shah
 * Group info:
 * 	jhshah	Jitesh H Shah
 * 	salilk	Salil S Kanitkar
 * 	ajalgao	Aditya A Jalgaonkar
 */

#include <unistd.h>
#include <mythread.h>
#include <futex.h>


/* Idle thread implementation. 
 * The thread checks whether it is the only one alive, if yes, exit()
 * else keep scheduling someone.
 */
void *mythread_idle(void *phony)
{
	mythread_private_t *traverse_tcb;
	pid_t idle_tcb_tid;

	while (1) {
		DEBUG_PRINTF("I am idle\n");
		traverse_tcb = __mythread_selfptr();
		idle_tcb_tid = traverse_tcb->tid;
		traverse_tcb = traverse_tcb->next;

		/* See whether there is a NON-DEFUNCT process in the list.
		 * If there is, idle doesn't need to kill the process just yet */
		while (traverse_tcb->tid != idle_tcb_tid) {
			if (traverse_tcb->state != DEFUNCT) {
				break;
			}
			traverse_tcb = traverse_tcb->next;
		}

		/* Idle is the only one alive, kill the process */
		if (traverse_tcb->tid == idle_tcb_tid)
			exit(0);

		/* Some thread still awaits execution, yeild ourselves */
		mythread_yield();
	}
}
