/*
  Error handling and debugging routines for cowMachine
  Copyright (c) 2012 Tamber Penketh <tamber@furryhelix.co.uk>
*/

#include <stdio.h>
#include <stdlib.h>

#include "sim.h"
#include "err.h"

static void _errmsg(muword errid){
  switch(errid){
  case EBADOP:
       fprintf(stderr, "Bad operation\n");
       break;
  case EUSTACK:
       fprintf(stderr, "Stack underflow\n");
       break;
  case EOSTACK:
       fprintf(stderr, "Stack overflow\n");
       break;
  case E_MCE:
       fprintf(stderr, "Machine-check exception.\n");
       break;
  default:
    fprintf(stderr, "!! TILT !!\n");
  }
}

void _stackprint(void){
     muword i;

     fprintf(stderr, "Data stack:\t\t");
     for(i = 0; i < DATA_STACK_DEPTH; i++){
	  fprintf(stderr, "%x\t", data_stack[i]);
     }
     fprintf(stderr, "\n");

     fprintf(stderr, "Return stack:\t\t");
     for(i = 0; i < RET_STACK_DEPTH; i++){
	  fprintf(stderr, "%x\t", ret_stack[i]);
     }

     fprintf(stderr, "\n");

     fprintf(stderr, "Interrupt stack:\t");
     for(i = 0; i < INTR_STACK_DEPTH; i++){
	  fprintf(stderr, "%x\t", int_stack[i]);
     }

     fprintf(stderr, "\n");
}

void crash(muword errid){
     _errmsg(errid);
     halt();
}
