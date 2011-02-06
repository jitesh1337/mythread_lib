/* Single Author info:
 *      sskanitk   Salil S Kanitkar
 * Group info:
 *      jhshah     Jitesh H Shah
 *      sskanitk   Salil S Kanitkar
 *      ajalgao    Aditya A Jalgaonkar
 */

/* We use a Queue Data structure for managing the tcb corresponding to each thread created. 
   The queue is implemented using a double ended linked list.
 */
#include<mythread.h>

/* The global extern pointer defined in mythread.h which points to the head node in
   Queue of the Thread Control Blocks.
*/
mythread_private_t *mythread_q_head;

/* This function initializes the Queue with a single node.
*/
void mythread_q_init(mythread_private_t * node)
{

	node->prev = node;
	node->next = node;

	mythread_q_head = node;

}

/* This function adds a node to the Queue, at the end of the Queue. 
   This is equivalent to Enque operation.
 */
void mythread_q_add(mythread_private_t * node)
{

	if (mythread_q_head == NULL) {
		//Q is not initiazed yet. Create it.
		mythread_q_init(node);
		return;
	}
	//Insert the node at the end of Q
	node->next = mythread_q_head;
	node->prev = mythread_q_head->prev;
	mythread_q_head->prev->next = node;
	mythread_q_head->prev = node;

	return;

}

/* This function deleted a specified(passed as a parameter) node from the Queue.
 */
void mythread_q_delete(mythread_private_t * node)
{

	mythread_private_t *p;
	if (node == mythread_q_head && node->next == mythread_q_head) {
		//There is only a single node and it is being deleted
		printf("The Q is now Empty!\n");
		mythread_q_head = NULL;
	}

	if (node == mythread_q_head)
		mythread_q_head = node->next;

	p = node->prev;

	p->next = node->next;
	node->next->prev = p;

	return;

}

/* This function iterates over the ntire Queue and prints out the state(see mythread.h to refer to various states)
   of all the tcb members.
 */
void mythread_q_state_display()
{

	if (mythread_q_head != NULL) {

		//display the Q - for debug purposes
		printf("\n The Q contents are -> \n");
		mythread_private_t *p;
		p = mythread_q_head;
		do {		//traverse to the last node in Q
			printf(" %d\n", p->state);
			p = p->next;
		} while (p != mythread_q_head);

	}

}

/* This function iterates over the Queue and prints out the state of the specified thread.
 */
mythread_private_t *mythread_q_search(unsigned long new_tid)
{

	mythread_private_t *p;
	if (mythread_q_head != NULL) {

		p = mythread_q_head;
		do {		//traverse to the last node in Q
			if (p->tid == new_tid)
				return p;
			p = p->next;
		} while (p != mythread_q_head);

	}
	return NULL;

}
