#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <errno.h>
#include <sched.h>

#include <mythread.h>
#include <futex.h>
#include <mythread_q.h>

#include <sys/syscall.h>
#include <sys/types.h>

#define CLONE_SIGNAL            (CLONE_SIGHAND | CLONE_THREAD)

int mythread_wrapper(void *);
void *mythread_idle(void *);

mythread_private_t *mythread_q_head;

mythread_private_t *main_tcb;

mythread_t idle_u_tcb;

extern struct futex gfutex;

static int __mythread_add_main_tcb()
{
	DEBUG_PRINTF("add_main_tcb: Creating node for Main thread \n");
	main_tcb = (mythread_private_t *) malloc(sizeof(mythread_private_t));
	if (main_tcb == NULL) {
		ERROR_PRINTF("add_main_tcb: Error allocating memory for main node\n");
		return -ENOMEM;
	}

	main_tcb->start_func = NULL;
	main_tcb->args = NULL;
	main_tcb->state = READY;
	main_tcb->returnValue = NULL;
	main_tcb->blockedForJoin = NULL;

	/* Get the main's tid and put it in its corresponding tcb. */
	main_tcb->tid = __mythread_gettid();

	/* Initialize futex to zero */
	futex_init(&main_tcb->sched_futex, 1);

	/* Put it in the Q of thread blocks */
	mythread_q_add(main_tcb);
	return 0;
}

int mythread_create(mythread_t * new_thread_ID,
		    mythread_attr_t * attr,
		    void *(*start_func) (void *), void *arg)
{

	/* pointer to the stack used by the child process to be created by clone later */
	char *child_stack;
	unsigned long stackSize;
	mythread_private_t *new_node;
	pid_t tid;
	int retval;

	/* Flags to be passed to clone system call. 
	   This flags variable is picked up from pthread source code. 
	 */
	int clone_flags = (CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGNAL
			   | CLONE_PARENT_SETTID
			   | CLONE_CHILD_CLEARTID | CLONE_SYSVSEM);

	if (mythread_q_head == NULL) {
		/* This is the very first mythread_create call. Set up the Q first with tcb nodes for main thread. */
		retval = __mythread_add_main_tcb();
		if (retval != 0)
			return retval;

		/* Initialise the global futex */
		futex_init(&gfutex, 1);

		/* Now create the node for Idle thread. */
		DEBUG_PRINTF("create: creating node for Idle thread \n");
		mythread_create(&idle_u_tcb, NULL, mythread_idle, NULL);
	}

	/* Crazy bit: in 2.6.35, all threads can access main thread's stack, but
	 * on the OS machine, this stack is somehow private to main thread only. 
	 * may be some clone flag? Work around that
	 */
	new_node = (mythread_private_t *) malloc(sizeof(mythread_private_t));
	if (new_node == NULL) {
		ERROR_PRINTF("Cannot allocate memory for node\n");
		return -ENOMEM;
	}

	/*Stack-size argument is not provided */
	if (attr == NULL)
		stackSize = SIGSTKSZ;
	else
		stackSize = attr->stackSize;

	/* posix_memalign aligns the allocated memory at a 64-bit boundry. */
	if (posix_memalign((void **)&child_stack, 8, stackSize)) {
		ERROR_PRINTF("posix_memalign failed! \n");
		return -ENOMEM;
	}

	/* We leave space for one invocation at the base of the stack */
	child_stack = child_stack + stackSize - sizeof(sigset_t);

	/* Save the thread_fun pointer and the pointer to arguments in the TCB. */
	new_node->start_func = start_func;
	new_node->args = arg;
	/* Set the state as READY - READY in Q, waiting to be scheduled. */
	new_node->state = READY;

	new_node->returnValue = NULL;
	new_node->blockedForJoin = NULL;
	/* Initialize the tcb's sched_futex to zero. */
	futex_init(&new_node->sched_futex, 0);

	/* Put it in the Q of thread blocks */
	mythread_q_add(new_node);

	/* Call clone with pointer to wrapper function. TCB will be passed as arg to wrapper function. */
	if ((tid =
	     clone(mythread_wrapper, (char *)child_stack, clone_flags,
		   new_node)) == -1) {
		printf("clone failed! \n");
		printf("ERROR: %s \n", strerror(errno));
		return (-errno);
	}
	/* Save the tid returned by clone system call in the tcb. */
	new_thread_ID->tid = tid;
	new_node->tid = tid;

	DEBUG_PRINTF("create: Finished initialising new thread: %ld\n",
		     (unsigned long)new_thread_ID->tid);
	return 0;
}
