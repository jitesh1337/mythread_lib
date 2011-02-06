/* Single Author info:
 * 	ajalgao	Aditya A Jalgaonkar
 * Group info:
 * 	jhshah	Jitesh H Shah
 * 	salilk	Salil S Kanitkar
 * 	ajalgao	Aditya A Jalgaonkar
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mythread.h>

/* Number of threads to start */
#define NTHREADS	8

/* This function will first increment count by 50, yield. When it gets the 
 * control back, it will increment count again and then exit
 */
void *thread_func(void *arg)
{
	int *count = (int *)arg;

	*count = *count + 50;
	LOG_PRINTF("Thread %ld: Incremented count by 50 and will now yield\n", (unsigned long)mythread_self().tid);
	mythread_yield();
	*count = *count + 50;
	LOG_PRINTF("Thread %ld: Incremented count by 50 and will now exit\n", (unsigned long)mythread_self().tid);
	mythread_exit(NULL);
	return NULL;
}

/* This is a simple demonstration of how to use the mythread library.
 * Start NTRHEADS number of threads, collect count value and exit
 */
int main()
{
	mythread_t threads[NTHREADS];
	int count[NTHREADS];
	int i;
	char *status;

	for (i = 0; i < NTHREADS; i++) {
		count[i] = i;
		mythread_create(&threads[i], NULL, thread_func, &count[i]);
	}
	for (i = 0; i < NTHREADS; i++) {
		LOG_PRINTF("Main: Will now wait for thread %ld. Yielding..\n", (unsigned long)threads[i].tid);
		mythread_join(threads[i], (void **)&status);
		LOG_PRINTF("Main: Thread %ld exited and increment count to %d\n", (unsigned long)threads[i].tid, count[i]);
	}
	LOG_PRINTF("Main: All threads completed execution. Will now exit..\n");
	mythread_exit(NULL);

	return 0;
}
