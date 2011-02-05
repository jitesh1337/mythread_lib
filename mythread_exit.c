/* Single Author info:
 * 	jhshah 	Jitesh H Shah
 * Group info:
 * 	jhshah	Jitesh H Shah
 * 	salilk	Salil S Kanitkar
 * 	ajalgao	Aditya A Jalgaonkar
 */

#include <mythread.h>
#include <malloc.h>
#include <mythread_q.h>

#include <sys/syscall.h>

/* Calling the glibc's exit() exits the process. Directly call the syscall
 * instead
 */
static void __mythread_do_exit()
{
	syscall(SYS_exit, 0);
}

/* See whether anyone is blocking on us for a join. If yes, mark that thread as READY
 * and kill ourselves
 */
void mythread_exit(void *return_val)
{
	mythread_private_t *self_ptr;

	/* Get pointer to our TCB structure */
	self_ptr = __mythread_selfptr();

	/* Don't remove the node from the list yet. We still have to collect the return value */
	self_ptr->state = DEFUNCT;
	self_ptr->returnValue = return_val;

	/* Change the state of any thread waiting on us. FIFO dispatcher will do the 
	 * needful
	 */
	if (self_ptr->blockedForJoin != NULL)
		self_ptr->blockedForJoin->state = READY;

	__mythread_dispatcher(self_ptr);

	/* Suicide */
	__mythread_do_exit();

}
