/*
  Opcode decoding and various simulator-related storages for cowMachine
  Copyright (c) 2012  Tamber Penketh <tamber@furryhelix.co.uk>
*/

#include "sim.h"
#include "ops.h"
#include "irq.h"

#include <string.h>

/* Stacks */
muword data_stack[(DATA_STACK_DEPTH + 1)];
muword ret_stack[(RET_STACK_DEPTH + 1)];
muword int_stack[(INTR_STACK_DEPTH + 1)];

/* Registers */
muword ds_p = 0;   /* Index into data stack. */
muword rs_p = 0;   /* Index into return stack */
muword is_p = 0;   /* Index into interrupt stack */
muword pc   = 0;   /* Program counter */
muword status = 0; /* Control/Status register */

/* Program memory */
muword main_mem[MAIN_MEM_SIZE];

/* 'decode' the current instruction and execute its operation */
void decode(muword opcode){
     switch(opcode){
     case 0x00:
	  op_halt();
	  break;
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
	  op_xor();
	  break;
     case 0x0A:
	  op_or();
	  break;
     case 0x0B:
	  op_and();
	  break;
     case 0x0C:
	  op_not();
	  break;
     case 0x0D:
	  op_lsh();
	  break;
     case 0x0E:
	  op_rsh();
	  break;
     case 0x0F:
	  op_if();
	  break;
     case 0x10:
	  op_call();
	  break;
     case 0x11:
	  op_ret();
	  break;
     default:
	  _is_push(pc);
	  _is_push(opcode);
	  interrupt(IRQ_INVALID_OP);
     }
}

void halt(void){
     status &= ~S_CPU_RUN;
}

void reset(void){
     /* Clean up a little... */

     /* Halt processor */
     status &= ~S_CPU_RUN;

     /* Unset all flags */
     {
	  status = 0;
	  pc     = 0;
	  rs_p   = 0;
	  ds_p   = 0;
	  is_p   = 0;
     }

     /* Clean stacks */
     {
	  /* Data stack */
	  memset(data_stack, 0, sizeof(data_stack));

	  /* Return stack */
	  memset(ret_stack, 0, sizeof(ret_stack));

	  /* Interrupt stack */
	  memset(int_stack, 0, sizeof(int_stack));
     }

     /* Start the processor again. */
     status &= S_CPU_RUN;
}
