/*
   mythread_q.h : Function prototypes for thread q functions.
*/

extern void mythread_q_init(mythread_private_t *node);
extern void mythread_q_add(mythread_private_t *node);
extern void mythread_q_delete(mythread_private_t *node);
extern void mythread_q_state_display();
extern mythread_private_t * mythread_q_search(unsigned long );

