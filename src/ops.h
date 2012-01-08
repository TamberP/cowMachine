#ifndef _OPS_H_
#define _OPS_H_

/* Stop execution */
void op_halt(void);

/*
  ! (n1 addr -- )
  Store value n1 at location addr in program memory.
*/
void op_store(void);

/*
  @ (addr -- n1)
  Fetch value from location addr in program memory.
*/
void op_fetch(void);

/*
  (n1 n2 -- (n1+n2))
  Add n1 and n2, push result onto stack
*/
void op_add(void);

/*
  (n1 n2 -- (n1-n2))
  Subtract n2 from n1.
*/
void op_sub(void);

/*
  (n1 n2 -- (n1*n2))
  Multiply n1 by n2.
*/
void op_mult(void);

/*
  (n1 n2 -- (n1/n2))
  Divide n1 by n2.
*/
void op_div(void);

/*
  (n1 n2 -- (n1&n2))
  Bitwise AND of n1 and n2
*/
void op_and(void);

/*
  (n1 n2 -- (n1|n2))
  Bitwise OR of n1 and n2
*/
void op_or(void);

/*
  (n1 n2 -- (n1^n2))
  Bitwise XOR of n1 and n2
*/
void op_xor(void);

/*
  (n1 n2 -- (n1~n2))
  Bitwise NOT of n1 and n2
*/

void op_not(void);

/*
  (n1 -- )
  Discard n1 from the stack
*/
void op_drop(void);

/*
  (n1 -- n1 n1)
  Duplicate n1, returning a second copy of it to the top of the stack.
*/
void op_dup(void);

/*
  (n1 n2 -- n1 n2 n1)
  Push copy of second element of the stack (n1) to the top of the
  stack.
*/
void op_over(void);

/*
  (n1 n2 -- n2 n1)
  Swap the order of the top two stack elements.
*/
void op_swap(void);

/*
  (n1 -- )
  Push n1 onto the return stack
*/
void op_rpush(void);

/*
  ( -- n1)
  Pop top element of return stack, push it onto top of data stack.
*/
void op_rpop(void);

/*
  ( -- )
  Discard top element of return stack.
*/
void op_rdrop(void);

/*
  ( n1 -- )
  If n1 is 0, then jump to address in next program cell.
*/
void op_if(void);

/*
  ( -- )
  Perform a subroutine call to address in next program cell.
*/
void op_call(void);

/*
  ( -- )
  Perform subroutine return.
*/
void op_exit(void);

/*
  Pushes the value in the next program cell onto the data stack.
 */

void op_push(void);

#endif /* _OPS_H_ */
