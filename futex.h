/*
 * Userspace Semaphores
 */
#ifndef	_FUTEX_H
#define	_FUTEX_H
#include <unistd.h> /* size_t */
#include <linux/futex.h>
#include <stdio.h>

/* You should never touch this structure directly! */
struct futex
{
	int count;
};

#include "futex_inline.h"
#define FUTEX_PASSED (-(1024 * 1024 * 1024))

/* Down and up operations */
struct timespec;
extern int sys_futex(int *, int, int, const struct timespec *);
extern int __futex_down_slow(struct futex *, int, struct timespec *);
extern int __futex_up_slow(struct futex *);

static inline int futex_down_timeout(struct futex *futx, struct timespec *rel)
{
	int val, woken = 0;

	/* Returns new value */
	while ((val = __futex_down(&futx->count)) != 0) {
		switch (__futex_down_slow(futx, val, rel)) {
		case -1: return -1; /* error */
		case 1: 
			return 0; /* passed */
		case 0: woken = 1; break; /* slept */
		}
	}
	/* If we were woken, someone else might be sleeping too: set to -1 */
	if (woken) {
		futx->count = -1;
	}
	return 0;
}

/* If __futex_down decrements from 1 to 0, we have it.  Otherwise sleep. */
static inline int futex_down(struct futex *futx)
{
	return futex_down_timeout(futx, NULL);
}

static inline int futex_trydown(struct futex *futx)
{
	return (__futex_down(&futx->count) == 0 ? 0: -1);
}

/* If __futex_up increments count from 0 -> 1, none was waiting.
   Otherwise, set to 1 and tell kernel to wake them up. */
static inline int futex_up(struct futex *futx)
{
	if (!__futex_up(&futx->count))
		return __futex_up_slow(futx);
	return 0;
}

static inline int futex_up_fair(struct futex *futx)
{
	/* Someone waiting? */
	if (!__futex_up(&futx->count)) {
		futx->count = FUTEX_PASSED;
		__futex_commit();
		/* If we wake one, they'll see it's a direct pass. */
		if (sys_futex(&futx->count, FUTEX_WAKE, 1, NULL) == 1)
			return 0;
		/* Otherwise do normal slow case */
		return __futex_up_slow(futx);
	}
	return 0;
}

/* If signal non-zero, set up that signal to come to us for FD ready.
   Returns fd. */
int futex_await(struct futex *futx, int signal);

void futex_init(struct futex *sem, int val);
#endif /* _FUTEX_H */
