#include<mythread.h>

mythread_t *mythread_q_head;

/*
*/
void mythread_q_init(mythread_t *node)
{

  node->prev = node;
  node->next = node;

  mythread_q_head = node;

}

void mythread_q_add(mythread_t *node)
{

  if ( mythread_q_head == NULL ) {
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

void mythread_q_delete(mythread_t *node)
{

  if ( node == mythread_q_head && node->next == mythread_q_head ) {
    //There is only a single node and it is being deleted
    printf("The Q is now Empty!\n");
    mythread_q_head = NULL;
  }
    
  if ( node == mythread_q_head ) 
    mythread_q_head = node->next;

  mythread_t *p;
  p = node->prev;

  p->next = node->next;
  node->next->prev = p;
  
  return;

}

void mythread_q_state_display()
{

  if (mythread_q_head != NULL) {

    //display the Q - for debug purposes
    printf("\n The Q contents are -> \n");
    mythread_t *p; 
    p = mythread_q_head;
    do { //traverse to the last node in Q
      printf(" %d\n",p->state);
      p = p->next; 
    }while ( p != mythread_q_head );

  }

}

mythread_t * mythread_q_search(unsigned long new_tid)
{

  if (mythread_q_head != NULL) {

    mythread_t *p; 
    p = mythread_q_head;
    do { //traverse to the last node in Q
      if ( p->tid == new_tid ) 
	return p;
      p = p->next; 
    }while ( p != mythread_q_head );

  }
  return NULL;

}
