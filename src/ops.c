#include <stdio.h>
#include <stdint.h>
#include "sim.h"
#include "err.h"

/* Non-opcode functions for manipulating stack. */
static muword _pop(void){
     muword n1 = data_stack[ds_p];
     if(ds_p > 0){
	  ds_p = --ds_p;
     } else {
	  /* Attempt to pop from empty stack */
	  crash(EUSTACK);
     }
     return n1;
}

static void _push(muword val){
     if(ds_p < (DATA_STACK_DEPTH - 1)){
	  ds_p = ++ds_p;
	  data_stack[ds_p] = val;
     } else {
	  /* Attempt to push to full stack */
	  crash(EOSTACK);
     }
}

static muword _rpop(void){
     rs_p = --rs_p;
     return ret_stack[++rs_p];
}

static void _rpush(muword val){
     ret_stack[rs_p] = val;
     rs_p = ++rs_p;
}

/* Opcode functions. */
void op_push(void){
     _push(prog_mem[pc + 1]);
     pc = ++pc;
}

void op_halt(void){
     /* Stops execution on the processor. */
     _stop();
}

void op_store(void){
     /* (n1 addr -- )
	Stores value n1 at location addr in program memory.
     */
     prog_mem[_pop()] = _pop();
}

void op_fetch(void){
     /* (addr -- n1)
	Fetch value from location addr in program memory.
     */
     _push(prog_mem[_pop()]);
}

void op_add(void){
     /* (n1 n2 -- (n1+n2))
	Add n1 and n2. Push result.
     */
     _push(_pop() + _pop());
}

void op_sub(void){
     /*
       (n1 n2 -- (n1 - n2))
       Subtract n2 from n1. Push result.
     */
     _push((_pop() - _pop()));
}

void op_mult(void){
     /*
       (n1 n2 -- (n1 * n2))
       Multiply n1 by n2
     */
     _push((_pop() * _pop()));
}

void op_div(void){
     /*
       (n1 n2 -- (n1 / n2))
       Divide n1 by n2
     */
     _push((_pop() / _pop()));
}

void op_xor(void){
     /*
      (n1 n2 -- (n1 ^ n2))
      Bitwise XOR of n1 and n2
     */
     _push((_pop() ^ _pop()));
}

void op_or(void){
     /*
       (n1 n2 -- (n1 | n2))
       Bitwise OR of n1 and n2
     */
     _push((_pop() | _pop()));
}

void op_and(void){
     /*
       (n1 n2 -- (n1 & n2))
       Bitwise AND of n1 and n2
     */
     _push((_pop() & _pop()));
}

void op_not(void){
     /* BUG: This does not yet work, due to the NOT not working on
      * muwords.

	(n1  -- ~n1)
	Bitwise NOT of the value on the TOS
     */
     /* _push((~_pop)); */
}

void op_drop(void){
     /* (n1 n2 -- n1)
	Discard the value at the top of the stack.
     */
     _pop();
}

void op_dup(void){
     /*
       (n1 -- n1 n1)
       Duplicate value at TOS
     */
     muword t = _pop();
     _push(t);
     _push(t);
}

void op_over(void){
     /* (n1 n2 -- n1 n2 n1)
	Duplicate value at second position on stack.
     */
     muword n1, n2;
     n2 = _pop();
     n1 = _pop();

     _push(n1);
     _push(n2);
     _push(n1);
}

void op_shl(void){
     /*
       (n1 n2 -- (n1 << n2))
       Shift n1 left by n2 places.
     */
     _push((_pop() << _pop()));
}

void op_shr(void){
     /*
	(n1 n2 -- (n1 >> n2 ))
	Shift n1 right by n2 places.
     */
     _push((_pop() >> _pop()));
}
