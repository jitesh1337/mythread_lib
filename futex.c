#include "futex.h"
#include <errno.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <sys/time.h>

int sys_futex(int *uaddr, int op, int val, const struct timespec *timeout)
{
  return syscall(SYS_futex, uaddr, op, val, timeout, NULL, 0);
}

/* Returns -1 on fail, 0 on wakeup, 1 on pass, 2 on didn't sleep */
int __futex_down_slow(struct futex *futx, int val, struct timespec *rel)
{
	if (sys_futex(&futx->count, FUTEX_WAIT, val, rel) == 0) {
		/* <= in case someone else decremented it */
		if (futx->count <= FUTEX_PASSED) {
			futx->count = -1;
			return 1;
		}
		return 0;
	}
	/* EWOULDBLOCK just means value changed before we slept: loop */
	if (errno == EWOULDBLOCK)
		return 2;
	return -1;
}

int __futex_up_slow(struct futex *futx)
{
	futx->count = 1;
	__futex_commit();
	return sys_futex(&futx->count, FUTEX_WAKE, 1, NULL);
}

int futex_await(struct futex *futx, int signal)
{
	return sys_futex(&futx->count, FUTEX_FD, signal, NULL);
}

void futex_init(struct futex *futx, int val)
{
	futx->count = val;
	__futex_commit();
}
