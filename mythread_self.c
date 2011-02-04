#include <malloc.h>
#include <string.h>

#include <signal.h>

#include <mythread.h>
#include <futex.h>
#include <mythread_q.h>

#include <sys/syscall.h>
#include <sys/types.h>
  
mythread_t mythread_self()
{
  pid_t tid;
  tid = __mythread_gettid();

  return *(mythread_q_search(tid));
}
