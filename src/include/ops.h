#ifndef _OPS_H_
#define _OPS_H_

#include "sim.h"

/*************************************************
 * Control operations
 ************************************************/

void op_halt(void);    /* Stop execution */

void op_reset(void);   /* Reset the processor */

/*************************************************
 * Data operations
 ************************************************/

void op_store(void);   /* ! (n addr -- ) Store n in program memory at
                            addr */

void op_fetch(void);   /* @ (addr -- n ) Fetch value from addr in
			  program memory */

void op_push(void);    /* Put the value of the next program memory
			  cell onto the top of the stack */

void op_drop(void);    /* ( n -- ) Discard the value at the top of the
			  stack. */

void op_over(void);    /* ( n1 n2 -- n1 n2 n1 )*/

void op_dup(void);     /* ( n -- n n ) */

/**************************************************
 * Mathematical Operators
 *************************************************/

void op_add(void);     /* + ( n1 n2 -- (n1+n2) ) */

void op_sub(void);     /* - ( n1 n2 -- (n1-n2) ) */

void op_mult(void);    /* * ( n1 n2 -- (n1*n2) ) */

void op_div(void);     /* / ( n1 n2 -- (n1/n2) ) */

/**************************************************
 * Logical Operators
 *************************************************/

void op_xor(void);

void op_or(void);

void op_and(void);

void op_not(void);

void op_rsh(void);

void op_lsh(void);

#endif /* _OPS_H_ */
