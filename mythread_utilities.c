#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

pid_t __mythread_gettid()
{
	return (pid_t) syscall(SYS_gettid);
}
