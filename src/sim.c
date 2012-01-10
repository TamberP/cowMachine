/*
  Opcode decoding and various simulator-related storages for cowMachine
  Copyright (c) 2012  Tamber Penketh <tamber@furryhelix.co.uk>
*/

#include <stdio.h>
#include <stdint.h>

#include "ops.h"
#include "sim.h"
#include "err.h"

muword data_stack[DATA_STACK_DEPTH];
muword ds_p; /* Data-stack pointer. (Index into data stack) */

muword ret_stack[RET_STACK_DEPTH];
muword rs_p; /* Return-stack pointer. (Index into return stack) */

muword prog_mem[PROG_MEM_SIZE];

/* Program counter */
muword pc = 0;

muword status;

void decode(muword opcode){
     switch(opcode){
     case 0x00:
	  op_halt();
     case 0x01:
	  op_store();
	  break;
     case 0x02:
	  op_fetch();
	  break;
     case 0x03:
	  op_push();
	  break;
     case 0x04:
	  op_add();
	  break;
     case 0x05:
	  op_sub();
	  break;
     case 0x06:
	  op_drop();
	  break;
     case 0x07:
	  op_dup();
	  break;
     case 0x08:
	  op_over();
	  break;
     case 0x09:
	  op_mult();
	  break;
     case 0x0A:
	  op_div();
	  break;
     case 0x0B:
	  op_xor();
	  break;
     case 0x0C:
	  op_or();
	  break;
     case 0x0D:
	  op_and();
	  break;
     case 0x0E:
	  op_not();
	  break;
     case 0x0F:
	  op_lsh();
	  break;
     case 0x10:
	  op_rsh();
	  break;
     case 0x11:
	  op_if();
	  break;
     case 0x12:
	  op_call();
	  break;
     case 0x13:
	  op_ret();
	  break;
     case 0xFE:
	  /* Function 'opcode'. This is a non-op, used purely to mark
	   * the beginning of a function; and so that we can jump
	   * straight to the function and begin execution as soon as
	   * the PC ticks upward as normal */
	  break;
     default:
	  crash(EBADOP);
     }
}

void stop(void){
     status &= ~S_CPU_RUN;
     fprintf(stderr, "CPU halted\n");
}

void reset(void){
     /* Clean up a little:
	- Unset all flags.
	- Empty stacks
	- Return program-counter to 0
     */
     fprintf(stderr, "Initialising processor...\n");
     status = 0;
     pc     = 0;
     rs_p   = 0;
     ds_p   = 0;
}
