#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#include "sim.h"

/*
  Op: nop
  Desc: Do nothing.
  Stack effect: ( -- )
*/

void core_nop(void);

/*
  Op: halt
  Desc: Stops execution until next interrupt.
  Stack effect: ( -- )
*/
void core_op_halt(void);

/*
  Op: !
  Desc: Store (unsigned) value n at location addr in main memory
  Stack effect: ( n addr -- )
*/
void core_op_store(void);

/*
  Op: @
  Desc: Fetch value from location addr in main memory
  Stack effect: ( addr -- n )
*/
void core_op_fetch(void);

/*
   Op: push
   Desc: Load value from next address in memory onto the data stack.
   Stack effect: ( -- n )
*/
void core_op_push(void);

/*
   Op: +
   Desc: Add top two values from data stack.
   Stack effect: ( n1 n2 -- n )
*/
void core_op_add();

/*
   Op: -
   Desc: Subtract value at top of stack, from second value on stack
   Stack effect: ( n1 n2 -- n )
*/
void core_op_sub();

/*
   Op: *
   Desc: Multiply top two values on stack by eachother
   Stack effect: ( n1 n2 -- n )
*/
void core_op_mult();

/*
   Op: /
   Desc: Divide value at top of stack, by second value on stack.
   Stack effect: ( n1 n2 -- n )
*/
void core_op_div();

/*
   Op: drop
   Desc: Discard value at top of data stack.
   Stack effect: ( n -- )
*/
void core_op_drop();

/*
   Op: dup
   Desc: Duplicate value at top of stack
   Stack effect: ( n -- n n )
*/
void core_op_dup();

/*
   Op: over
   Desc: Duplicate value *second* on stack, to top of stack.
   Stack effect: ( n1 n2 -- n1 n2 n1 )
*/
void core_op_over();

/*
   Op: ^
   Desc: Bitwise XOR of top two values on stack
   Stack effect: ( n1 n2 -- n )
*/
void core_op_xor();

/*
   Op: |
   Desc: Bitwise OR of top two values on stack.
   Stack effect: ( n1 n2 -- n )
*/
void core_op_or();

/*
   Op: &
   Desc: Bitwise AND of top two values on stack.
   Stack effect: ( n1 n2 -- n )
*/
void core_op_and();

/*
   Op: ~
   Desc: Bitwise NOT of top two values on stack.
   Stack effect: ( n1 n2 -- n )
*/
void core_op_not();

/*
   Op: <<
   Desc: Logical left-shift of top value on stack, by number of places given by second value on stack
   Stack effect: ( n1 n2 -- n )
*/
void core_op_lsh();

/*
   Op: >>
   Desc: Logical right shift of top value on stack, by number of places given by second value on stack.
   Stack effect: ( n1 n2 -- n )
*/
void core_op_rsh();

/*
   Op: if
   Desc: Two-word instruction. Second word is used as the target of
     the jump made if the flag at the top of the stack is 1
   Stack effect: ( flag -- )
*/
void core_op_if();

/*
   Op: call
   Desc: Two-word instruction. Second word is target of the jump to
     the beginning of the required function. (Functions are loaded at
     a fixed address, and all calls are jumps that automatically
     perform the book-keeping required to return to the correct
     location.)
   Stack effect: ( -- R: n )
*/
void core_op_call();

/*
   Op: ret
   Desc: Return from the function.
   Stack effect: ( R: n -- )
*/
void core_op_ret();

/*
   Op: jump
   Desc: Two-word instruction. Unconditional jump to the address given
     in the scond word.
   Stack effect: ( -- )
*/
void core_op_jump();

/*
   Op: irq-enable
   Desc: Enable or disable interrupts, depending on flag at top of data stack.
         0 to disable, any other value to enable.
   Stack effect: ( flag -- )
*/
void core_op_enable_irq();

/*
   Op: irq
   Desc: Raise an interrupt from software. Interrupt to be raise, is
     given by the value at the top of the interrupt stack.
   Stack effect: ( I: n -- )
*/
void core_op_irq();

/*
   Op: >r
   Desc: Move value from top of data stack, to return stack.
   Stack effect: ( n -- R: n )
*/
void core_op_rpush();

/*
   Op: r>
   Desc: Move value from return stack to data stack.
   Stack effect: ( R: n -- n )
*/
void core_op_rpop();

/*
   Op: >i
   Desc: Move value from data stack, to interrupt stack.
   Stack effect: ( n -- I: n)
*/
void core_op_ipush();

/*
   Op: i>
   Desc: Move value from interrupt stack to data stack.
   Stack effect: ( I: n -- n )
*/
void core_op_ipop();


/* ******** Comparison operations ******** */
/* True if ... */

/* ...top two values on stack are equal */
void core_cmp_eq(void);

/* ...top two values on stack are *not* equal */
void core_cmp_neq(void);

/* ...value at top of stack is less than value second on stack */
void core_cmp_lt(void);

/* ...value at top of stack is greater than value second on stack */
void core_cmp_gt(void);

/* ...value at top of stack is 0 */
void core_cmp_eq0(void);

/* ...value at top of stack is *not* 0 */
void core_cmp_neq0(void);

/* unconditional true */
void core_cmp_true(void);

/* unconditional false */
void core_cmp_false(void);

/* n, low, high. True if low <= n <= high. */
void core_cmp_between(void);

/* n, low, high. True if low <= n < high */
void core_cmp_within(void);

#endif /* _OPERATIONS_H_ */
