#include "sim.h"
#include "ops.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>

/* Internal, stack-manipulation, operations */

/*** Data stack ***/
word _pop(void){
     word val;

     if(ds_p == 0){
	  abort();
     } else {
	  ds_p = ds_p - 1;
	  val = ds[ds_p];
	  ds[ds_p] = 0; /* Clean up stack prints a little */
	  return val;
     }
}


void _push(word val){
     if(ds_p < DS_DEPTH){
	  ds[ds_p] = val;
	  ds_p = (ds_p + 1);
     } else {
	  /* Stack overflow */
	  abort();
     }
}

/*** Return stack ***/
word _rpop(void){
     word val;

     if(rs_p == 0){
	  abort();
     } else {
	  rs_p = rs_p - 1;

	  val = rs[rs_p];
	  rs[rs_p] = 0;
	  return val;
     }
}


void _rpush(word val){
     if(rs_p < RS_DEPTH){
	  rs[rs_p] = val;
	  rs_p = (rs_p + 1);
     } else {
	  /* Stack overflow */
	  abort();
     }
}

/*** Interrupt stack ***/
word _ipop(void){
     word val;

     if(is_p == 0){
	  /* Stack underflow */
	  abort();
     } else {
	  is_p = is_p - 1;
	  val = is[is_p];

	  is[is_p] = 0;
	  return val;
     }
}

void _ipush(word val){
     if(is_p < IS_DEPTH){
	  is[is_p] = val;
	  is_p = (is_p + 1);
     } else {
	  /* Stack overflow! */
	  abort();
     }
}


/* Programmer-visible opcodes */

/* ** Stop execution of instructions until next interrupt ** */
void core_op_halt(void){
     status &= ~status_cpu_run;
}

/* ** Store value in memory ** */
void core_op_store(void){
     word n, addr;

     addr = _pop();
     n    = _pop();

     if((addr == MMAP_PORT_A) || (addr == MMAP_PORT_B)){
	  if(addr == MMAP_PORT_A){
	       io_a = n;
	       io_a_xmit(n);
	  }
	  if(addr == MMAP_PORT_B){
	       io_b = n;
	       io_b_xmit(n);
	  }
     } else {
	  main_mem[addr] = n;
     }
}

/* ** Fetch value from memory ** */
void core_op_fetch(void){
     word addr = _pop();

     if((addr == MMAP_PORT_A) || (addr == MMAP_PORT_B)){
	  if(addr == MMAP_PORT_A)
	       io_a = io_a_recv();
	       _push(io_a);
	  if(addr == MMAP_PORT_B)
	       io_b = io_b_recv();
	       _push(io_b);
     } else {
	  _push(main_mem[addr]);
     }
}


/* ** Push value from next address in memory, onto the top of the stack ** */
void core_op_push(void){
     _push(main_mem[pc + 1]);
     pc = (pc + 1);
}

/* ** Discard the value currently on the top of the stack ** */
void core_op_drop(void){
     _pop();
}

/* ** Copy value at stack #2, to top of stack ** */
void core_op_over(void){
     word n1, n2;
     n2 = _pop();
     n1 = _pop();

     _push(n1);
     _push(n2);
     _push(n1);
}

/* Duplicate value at top of data stack */
void core_op_dup(void){
     word n1 = _pop();

     _push(n1);
     _push(n1);
}

void core_op_add(void){
     _push((_pop() + _pop()));
}

void core_op_sub(void){
     _push((_pop() - _pop()));
}

void core_op_mult(void){
     _push((_pop() * _pop()));
}

void core_op_div(void){
     _push((_pop() / _pop()));
}

void core_op_xor(void){
     _push((_pop() ^ _pop()));
}

void core_op_or(void){
     _push((_pop() | _pop()));
}

void core_op_and(void){
     _push((_pop() & _pop()));
}

void core_op_not(void){
     _push(~_pop());
}

/* Logical left shift */
void core_op_lsh(void){
     _push((_pop() << _pop()));
}

/* Logical right shift */
void core_op_rsh(void){
     _push((_pop() >> _pop()));
}

/* Conditional operations.
   If the value at the top of the data stack is greater than 0, then jump to the value encoded in the second half of the two-word 'if' instruction; otherwise, continue as normal.
*/
void core_op_if(void){
     if(_pop() > 0)
	  pc = main_mem[(pc + 1)]; /* Make the jump to the location given in the second word */
     else
	  pc = (pc + 1); /* Skip the jump target. */
}

/* call a function */
void core_op_call(void){
     /* Save the address we're jumping to. */
     word target = main_mem[(pc + 1)];

     /* Push the address of the next instruction onto the return
      * stack; so we return *after* the jump. */
     _rpush(pc + 2);

     /* Jump to the function's start. (Functions have a null/useless
      * opcode to mark their beginning, and the actual executable code
      * starts the instruction *after* this, since the PC will be
      * incremented after this operation ends. */
     pc = target;
}

/* Return from a function */
void core_op_ret(void){
     pc = _rpop();
}

/* Unconditional jump */
void core_op_jump(void){
     pc = main_mem[pc + 1];
}

/* Enable/disable interrupts */

void core_op_enable_irq(void){
     word enable = _pop();

     if(enable == 0){
	  status &= ~status_irq_enable;
     } else {
	  status &= status_irq_enable;
     }
}

void core_op_irq(void){
     /* Call an interrupt from software */
/*     word interrupt = _ipop();

     if((status & status_irq_enable) > 0){
	  
     }*/
}

/* Move value from ds to rs */
void core_op_rpush(void){
     _rpush(_pop());
}

/* Move value from rs to ds */
void core_op_rpop(void){
     _push(_rpop());
}

/* Move value from ds to is */
void core_op_ipush(void){
     _ipush(_pop());
}

/* Move value from is to ds */
void core_op_ipop(void){
     _push(_ipop());
}


/******************************************************************************
 ** Comparison Operators
 *****************************************************************************/

/* = ( n1 n2  -- flag )
   True if n1 == n2
 */

void core_cmp_eq(void){
     if((_pop() == _pop()))
	  _push(1);
     else
	  _push(0);
}

/* <> ( n1 n2 -- flag )
   True if n1 != n2
*/
void core_cmp_neq(void){
     if((_pop() != _pop()))
	  _push(1);
     else
	  _push(0);
}

/* < ( n1 n2 -- flag )
   True if n1 < n2 (unsigned)
*/

void core_cmp_lt(void){
     if(_pop() < _pop())
	  _push(1);
     else
	  _push(0);
}

/* > ( n1 n2 -- flag )
   True if n1 > n2 (unsigned)
*/

void core_cmp_gt(void){
     if(_pop() > _pop())
	  _push(1);
     else
	  _push(0);
}

/* 0= ( n -- flag )
   True if n == 0
*/

void core_cmp_eq0(void){
     if(0 == _pop())
	  _push(1);
     else
	  _push(0);
}

/* 0<> ( n -- flag )
   True if n != 0
*/

void core_cmp_neq0(void){
     if(0 != _pop())
	  _push(1);
     else
	  _push(0);
}

/* True ( -- flag )
   Pushes a true value onto the stack
*/

void core_cmp_true(void){
     _push(1);
}

/* False ( -- flag )
   Pushes a false value onto the stack
*/

void core_cmp_false(void){
     _push(0);
}

/* Between ( n low high -- flag )
   True if low <= n <= high
*/

void core_cmp_between(void){
     word u, low, high;
     u = _pop();
     low = _pop();
     high = _pop();

     if((low <= u) && (u <= high))
	  _push(1);
     else
	  _push(0);
}

/* Within  ( n low high -- flag )
   True if low <= u < high
*/

void core_cmp_within(void){
     word u, low, high;
     u = _pop();
     low = _pop();
     high = _pop();

     if((low <= u) && (u < high))
	  _push(1);
     else
	  _push(0);
}

/* No op */

void core_nop(void){
}

/* Swap top values on stack. */

void core_swap(void){
     word a, b;

     a = _pop();
     b = _pop();

     _push(a);
     _push(b);
}
