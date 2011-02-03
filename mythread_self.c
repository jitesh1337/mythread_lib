#include <malloc.h>
#include <string.h>

#include <signal.h>

#include <mythread.h>
#include <futex.h>
#include <mythread_q.h>

#include <sys/syscall.h>
#include <sys/types.h>
  
static pid_t gettid(void) {
  return (pid_t) syscall(SYS_gettid);
}

mythread_t mythread_self()
{
  pid_t tid;
  tid = gettid();

  return *(mythread_q_search(tid));
}
