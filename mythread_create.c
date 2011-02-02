#include <malloc.h>
#include <string.h>

#include <signal.h>
#include <errno.h>
#include <sched.h>

#include <mythread.h>
#include <futex.h>
#include <mythread_q.h>

#define CLONE_SIGNAL            (CLONE_SIGHAND | CLONE_THREAD)

int mythread_wrapper(void *);

int mythread_create(mythread_t * new_thread_ID,
		    mythread_attr_t * attr,
		    void *(*start_func) (void *), void *arg)
{

	/* pointer to the stack used by the child process to be created by clone later */
	char *child_stack;
	unsigned long stackSize;
	unsigned long tid;

	/* Flags to be passed to clone system call. 
	   This flags variable is picked up from pthread source code. 
	 */
	int clone_flags = (CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGNAL
			   | CLONE_PARENT_SETTID
			   | CLONE_CHILD_CLEARTID | CLONE_SYSVSEM);

	/*Stack-size argument is not provided */
	if (attr == NULL)
		stackSize = SIGSTKSZ;
	else
		stackSize = attr->stackSize;

	/* posix_memalign aligns the allocated memory at a 64-bit boundry. */
	if (posix_memalign((void **)&child_stack, 8, stackSize)) {
		printf("posix_memalign failed! \n");
		printf("Exiting.....\n");
		return (-ENOMEM);
	}

	/* We leave space for one invocation at the base of the stack */
	child_stack = child_stack + stackSize - sizeof(sigset_t);

	/* Save the thread_fun pointer and the pointer to arguments in the TCB. */
	new_thread_ID->start_func = start_func;
	new_thread_ID->args = arg;
	/* Set the state as READY - READY in Q, waiting to be scheduled. */
	new_thread_ID->state = READY;

	new_thread_ID->returnValue = NULL;
	new_thread_ID->blockedForJoin = NULL;
	/* Initialize the tcb's sched_futex to zero. */
	futex_init(&new_thread_ID->sched_futex, 0);

	/* Put it in the Q of thread blocks */
	mythread_q_add(new_thread_ID);

	/* Call clone with pointer to wrapper function. TCB will be passed as arg to wrapper function. */
	if ((tid =
	     clone(mythread_wrapper, (char *)child_stack, clone_flags,
		   new_thread_ID)) == -1) {
		printf("clone failed! \n");
		printf("ERROR: %s \n", strerror(errno));
		return (-errno);
	}
	/* Save the tid returned by clone system call in the tcb. */
	new_thread_ID->tid = tid;

	printf("This should be printed first %ld\n",(unsigned long)new_thread_ID->tid);
	futex_up(&new_thread_ID->sched_futex);

	return 0;
}

int mythread_wrapper(void *thread_tcb)
{

	mythread_t *new_tcb;
	new_tcb = (mythread_t *) thread_tcb;

	futex_down(&new_tcb->sched_futex);
	new_tcb->start_func(new_tcb->args);

	/* Do a down on the futex corresponding to this tcb. */
	/* Call the thread_func. */
	/* Exit the current thread. */

	/* Ideally we shouldn't reach here, but if the thread doesn't call mythread_exit, we should */
	//mythread_exit(NULL);

	/* Ideally we shouldn't reach here */
	return 0;

}
