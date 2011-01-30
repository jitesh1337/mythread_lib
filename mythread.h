/*
 * mythread.h -- interface of user threads library
 */

#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>

#ifndef MYTHREAD_H
#define MYTHREAD_H

#define RUNNING 0
#define READY 1
#define BLOCKED 2 //Waiting on Join

typedef struct mythread_attr {
  unsigned long stackSize;     //spwcify the stack size to be used by each thread
}mythread_attr_t;

/* Thread Control Block structure */
typedef struct mythread {

  unsigned long tid; //the thread-id of the thread
  int state; //the state in which the corresponding thread will be.
  void * (*start_fun) (void *); //the func pointer to the thread function to be executed.
  void *args; //the arguments to be passed to the thread function.
  void *returnValue; //the return value that thread returns.

  struct mythread *prev, *next; //pointers to traverse the tcb DLL

}mythread_t;

extern mythread_t *mythread_q_head; //the pointer pointing to head node of the tcb q

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
void mythread_exit(mythread_t *);

#endif /* MYTHREAD_H */
