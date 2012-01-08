#include <stdio.h>
#include <stdint.h>
#include "err.h"
#include "sim.h"
#include "ops.h"

muword data_stack[DATA_STACK_DEPTH];
muword ds_p; /* Data-stack pointer. (Index into data stack) */

muword ret_stack[RET_STACK_DEPTH];
muword rs_p; /* Return-stack pointer. (Index into return stack. ) */

muword prog_mem[PROG_MEMORY_SIZE];

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
	  op_shl();
	  break;
     case 0x10:
	  op_shr();
	  break;
     case 0xFF:
	  op_stackprint();
	  break;
     default:
	  crash(EBADOP);
     }
}
