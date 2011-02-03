#include <mythread.h>
#include <malloc.h>
#include <mythread_q.h>

#include <sys/syscall.h>

void __mythread_do_exit()
{
	syscall(SYS_exit, 0);
}

/* A very basic exit implementation which doesn't even store the return value. 
 * Partly because, mythread_self return a value instead of a pointer. 
 * TODO: correct the implementation
 */
void mythread_exit(void *return_val)
{
	mythread_t self, *self_ptr, *next;
	self = mythread_self();

	self_ptr = mythread_q_search(self.tid);
	next = self_ptr->next;

	/* Don't remove the node from the list yet. We still have to collect the return value */
	self_ptr->state = DEFUNCT;
	self_ptr->returnValue = return_val;

	/* Change the state of any thread waiting on us */
	self_ptr->blockedForJoin->state = READY;

	//futex_up(&next->sched_futex);
	mythread_dispatcher(self_ptr);
	
	/* Wake up next thread in queue and kill ourself */
	__mythread_do_exit();

}
