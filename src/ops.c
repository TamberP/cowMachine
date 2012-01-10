/* 
   Implementation of op-codes for cowMachine.
   Copyright (c) 2012 - Tamber Penketh <tamber@furryhelix.co.uk>
*/

#include "ops.h"
#include "sim.h"

/* Internal stack-management operations */

/* Pop a word from the stack and return it */
static muword _pop(void){
     muword val = data_stack[ds_p];
     ds_p = (ds_p - 1);

     return val;
}

/* Push a word onto the stack. */
static void _push(muword val){
     ds_p = (ds_p + 1);
     data_stack[ds_p] = val;
}

static muword _rpop(void){
     muword val = ret_stack[rs_p];
     rs_p = (rs_p - 1);

     return val;
}

static void _rpush(muword val){
     rs_p = (rs_p + 1);
     ret_stack[ds_p] = val;
}

/* Programmer-visible Opcodes */

/*************************************************
 * Control operations
 ************************************************/

/* Stop execution */
void op_halt(void){
     stop();
}

/* Reset the processor */
void op_reset(void){
     reset();
}

/* Jump to a function */
void op_call(void){
     /* Save the address we're jumping to */
     muword jmp = prog_mem[(pc + 1)];

     /* Push the address of the next instruction onto the return
      * stack; so we return where we left off. */
     _rpush((pc + 2));

     /* Jump to the function's opcode. The function's executable code
      * starts the instruction after this, since the PC will be
      * incremented after this opcode ends. */
     pc = jmp;
}

/* Return to whence we came. */
void op_ret(void){
     pc = _rpop();
}


/* Conditional operation.  If the current top of the stack is 0, then
   jump to the value encoded in the second half of the two-word
   instruction; otherwise, continue as normal.  */
void op_if(void){
}

/*************************************************
 * Data operations
 ************************************************/

/* Store  !
   (n1 addr -- )

   Stores value n1 at location addr in program memory
*/
void op_store(void){
     muword n1, addr;

     addr = _pop();
     n1 = _pop();

     prog_mem[addr] = n1;
}

/* Fetch  @
   (addr -- n1)

   Retrieve the value from location addr in program memory
*/
void op_fetch(void){
     _push(prog_mem[_pop()]);
}

/* Push
   ( -- n1)
   
   Push the value from the next program memory cell onto the top of the
   stack.
 */

void op_push(void){
     _push(prog_mem[pc + 1]);
     pc = (pc + 1);
}

/* Drop
   ( n1 n2 -- n1 )

   Discard the item on the top of the stack
*/
void op_drop(void){
     _pop();
}

/* Over
   ( n1 n2 -- n1 n2 n1 )

*/
void op_over(void){
     muword n1, n2;
     n2 = _pop();
     n1 = _pop();

     _push(n1);
     _push(n2);
     _push(n1);
}

/* Dup
   ( n1 -- n1 n1)
*/
void op_dup(void){
     muword n1 = _pop();

     _push(n1);
     _push(n1);
}

/*************************************************
 * Mathematical operations
 ************************************************/

/* Add  +
   ( n1 n2 -- (n1+n2) )

   Add n1 to n2. Push the result onto the stack in their place.
*/
void op_add(void){
     _push((_pop() + _pop()));
}

/* Subtract  -
   ( n1 n2 -- (n1-n2) )

   Subtract n2 from n1. Push the result.
*/
void op_sub(void){
     _push((_pop() - _pop()));
}

/* Multiply  *
   ( n1 n2 -- (n1*n2) )

   Multiply n1 by n2.
*/
void op_mult(void){
     _push((_pop() * _pop()));
}

/* Divide  /
   ( n1 n2 -- (n1 / n2) )

   Divide n1 by n2.
*/
void op_div(void){
     _push((_pop() / _pop()));
}

/*************************************************
 * Logical operations
 ************************************************/

/* XOR
   ( n1 n2 -- (n1 ^ n2) )

   Bitwise XOR of top two items on stack
*/
void op_xor(void){
     _push((_pop() ^ _pop()));
}

/* OR
   ( n1 n2 -- (n1 | n2) )

   Bitwise OR of top two items on stack
*/
void op_or(void){
     _push((_pop() | _pop()));
}

/* AND
   ( n1 n2 -- (n1 & n2) )

   Bitwise AND
*/
void op_and(void){
     _push((_pop() & _pop()));
}

/* NOT
   ( n1 -- ~n1 )

   Bitwise NOT
*/
void op_not(void){
     _push(~_pop());
}

/* Left shift
   ( n1 n2 -- (n1 << n2) )

   Left shift n1 by n2 places.
*/
void op_lsh(void){
     _push((_pop() << _pop()));
}

/* Right shift
   ( n1 n2 -- (n1 >> n2) )

   Right shift n1 by n2 places.
*/
void op_rsh(void){
     _push((_pop() >> _pop()));
}
