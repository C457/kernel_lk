
extern uint32_t __div64_32(uint64_t *dividend, uint32_t divisor);

/* The unnecessary pointer compare is there
 * to check for type safety (n must be 64bit)
 */
#define do_div(n,base) ({				\
	uint32_t __base = (base);			\
	uint32_t __rem;					\
	__rem = __div64_32(&(n), __base);		\
	__rem;						\
 })
 
