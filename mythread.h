/*
 * mythread.h -- interface of user threads library
 */

/* Single Author info:
 * 	ajalgao	 Aditya A Jalgaonkar
 * Group info:
 * 	jhshah	 Jitesh H Shah
 * 	sskanitk Salil S Kanitkar
 * 	ajalgao	 Aditya A Jalgaonkar
 */

#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include<futex.h>
#include<string.h>

#ifndef MYTHREAD_H
#define MYTHREAD_H

#define FALSE 0
#define TRUE 1

#define RUNNING 0
#define READY 	1 /* Ready to be scheduled */
#define BLOCKED 2 /* Waiting on Join */
#define DEFUNCT 3 /* Dead */

typedef struct mythread_attr {
  unsigned long stackSize;     /* Stack size to be used by this thread. Default is SIGSTKSZ */
}mythread_attr_t;

/* Thread Handle exposed to the user */
typedef struct mythread {
  pid_t tid; /* The thread-id of the thread */
}mythread_t;

/* The Actual Thread Control Block structure */
typedef struct mythread_private {

  pid_t tid; 				/* The thread-id of the thread */
  int state; 				/* the state in which the corresponding thread will be. */
  void * (*start_func) (void *); 	/* The func pointer to the thread function to be executed. */
  void *args; 				/* The arguments to be passed to the thread function. */
  void *returnValue; 			/* The return value that thread returns. */
  struct mythread_private *blockedForJoin; 	/* Thread blocking on this thread */
  struct futex sched_futex;		/* Futex used by the dispatcher to schedule this thread */
  struct mythread_private *prev, *next; 

}mythread_private_t;

extern mythread_private_t *mythread_q_head; /* The pointer pointing to head node of the TCB queue */
/* add your code here */

/*
 * mythread_self - thread id of running thread
 */
mythread_t mythread_self(void);

/*
 * mythread_create - prepares context of new_thread_ID as start_func(arg),
 * attr is ignored right now.
 * Threads are activated (run) according to the number of available LWPs
 * or are marked as ready.
 */
int mythread_create(mythread_t *new_thread_ID,
					mythread_attr_t *attr,
					void * (*start_func)(void *),
					void *arg);

/*
 * mythread_yield - switch from running thread to the next ready one
 */
int mythread_yield(void);

/*
 * mythread_join - suspend calling thread if target_thread has not finished,
 * enqueue on the join Q of the target thread, then dispatch ready thread;
 * once target_thread finishes, it activates the calling thread / marks it
 * as ready.
 */
int mythread_join(mythread_t target_thread, void **status);

/*
 * mythread_exit - exit thread, awakes joiners on return
 * from thread_func and dequeue itself from run Q before dispatching run->next
 */
void mythread_exit(void *retval);

/* Private functions */

/* Get the tid of the current thread */
pid_t __mythread_gettid();

/* Get the pointer to the private structure of this thread */
mythread_private_t *__mythread_selfptr();

/* Dispatcher - The thread scheduler */
int __mythread_dispatcher(mythread_private_t *);
void __mythread_debug_futex_init();

/* Debugging stuff */
extern char debug_msg[1000];
extern struct futex debug_futex;

#ifdef DEBUG
#define DEBUG_PRINTF(...) __mythread_debug_futex_init(); \
			futex_down(&debug_futex); \
			sprintf(debug_msg, __VA_ARGS__); \
			write(1, debug_msg, strlen(debug_msg)); \
			futex_up(&debug_futex);
#else
#define DEBUG_PRINTF(...) do {} while(0);
#endif

#define ERROR_PRINTF(...) __mythread_debug_futex_init(); \
			futex_down(&debug_futex); \
			sprintf(debug_msg, __VA_ARGS__); \
			write(1, debug_msg, strlen(debug_msg)); \
			futex_up(&debug_futex);

#define LOG_PRINTF(...) __mythread_debug_futex_init(); \
			futex_down(&debug_futex); \
			sprintf(debug_msg, __VA_ARGS__); \
			write(1, debug_msg, strlen(debug_msg)); \
			futex_up(&debug_futex);

#endif /* MYTHREAD_H */
