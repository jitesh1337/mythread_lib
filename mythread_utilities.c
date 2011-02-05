#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <futex.h>

struct futex debug_futex;
static int debug_futex_init_done = 0;

pid_t __mythread_gettid()
{
	return (pid_t) syscall(SYS_gettid);
}

void __mythread_debug_futex_init()
{
	if (debug_futex_init_done != 1) {
		futex_init(&debug_futex, 1);
		debug_futex_init_done = 0;
	}
}
