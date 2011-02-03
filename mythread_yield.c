#include <mythread.h>
#include <futex.h>

/* Basic yield. It will get more complex when we add join */
int mythread_yield()
{
	mythread_t self = mythread_self();
	if (self.next->tid == self.tid) /* Only one thread */
		return 0;

	printf("Wake-up:%ld Sleep:%ld %d\n", (unsigned long)self.next->tid, (unsigned long)self.tid, self.next->sched_futex.count);
	futex_up(&self.next->sched_futex);
	futex_down(&self.sched_futex);
	futex_down(&self.sched_futex);
	return 0;
}
