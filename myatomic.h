/*
 * myatomic.h -- support for atomic action (compare and swap)
 */

/* Atomically store NEWVAL in *MEM if *MEM is equal to OLDVAL and set eqz;
   o/w clear eqz. Always return the old *MEM value.
   The conditional atomic swap is performed if the return value == OLDVAL. */

#ifdef i386
# define __arch_compare_and_exchange_val_32_acq(mem, newval, oldval)	\
  ({ __typeof (*mem) ret; \
    __asm __volatile ("lock\n" "cmpxchgl %2, %1\n"		 \
		      : "=a" (ret), "=m" (*mem)			 \
		      : "r" (newval), "m" (*mem), "0" (oldval)); \
  ret; })
# define compare_and_swap(mem, newval, oldval)			 \
  __arch_compare_and_exchange_val_32_acq(mem, newval, oldval)
#else
# define __arch_compare_and_exchange_val_64_acq(mem, newval, oldval)	\
  ({ __typeof (*mem) ret; \
    __asm __volatile ("lock\n" "cmpxchgq %q2, %1\n"		 \
		       : "=a" (ret), "=m" (*mem)		 \
		       : "r" ((long int) (newval)), "m" (*mem),	 \
			 "0" ((long int) (oldval)));		 \
     ret; })
# define compare_and_swap(mem, newval, oldval)			 \
  __arch_compare_and_exchange_val_64_acq(mem, newval, oldval)
#endif
