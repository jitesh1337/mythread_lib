#include "myatomic.h"

/* Atomic dec: return new value. */
static __inline__ int __futex_down(int *counter)
{
  int oval, val = *counter;

  /* Don't decrement if already negative. */
  if (val < 0)
    return val;

  oval = compare_and_swap(counter, val-1, val);
  
  if (oval == val) return val-1;
  /* Otherwise, we have no way of knowing value.  Guess -1 (if
     we're wrong we'll spin). */
  return -1;
}

/* Atomic inc: return 1 if counter incremented from 0 to 1. */
static __inline__ int __futex_up(int *counter)
{
  int oval, val = *counter;

  oval = compare_and_swap(counter, val+1, val);

  return (oval == val && oval == 0);
}

/* Simple atomic increment. */
// static __inline__ void __atomic_inc(int *counter)
// {
//   int oval, val = *counter;

//   oval = compare_and_swap(counter, val+1, val);
// }

/* Atomic decrement, and return 1 if result is negative. */
static __inline__ int __furwock_dec_negative(int *counter)
{
  int oval, val = *counter;

  oval = compare_and_swap(counter, val-1, val);

  return (oval == val) && (val+1 < 0);
}

/* Commit the write, so it happens before we send the semaphore to
   anyone else */
static __inline__ void __futex_commit(void)
{
	/* Probably overkill, but some non-Intel clones support
           out-of-order stores, according to 2.5.5-pre1's
           linux/include/asm-i386/system.h */
#ifdef __i386__
	__asm__ __volatile__ ("lock; addl $0,0(%%esp)": : :"memory");
#endif
}
