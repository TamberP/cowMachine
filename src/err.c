/*
  Error handling and debugging routines for cowMachine
  Copyright (c) 2012 Tamber Penketh <tamber@furryhelix.co.uk>
*/

#include <stdio.h>

#include "sim.h"
#include "err.h"

static void _errmsg(errno){
  switch(errno){
  case EBADOP:
    fprintf(stderr, "Bad operation\n");
    break;
  case EUSTACK:
    fprintf(stderr, "Stack underflow\n");
    break;
  case EOSTACK:
    fprintf(stderr, "Stack overflow\n");
    break;
  default:
    fprintf(stderr, "!! TILT !!");
  }
}

void _stackprint(void){
     muword i;
     if(ds_p < 0)
	  return;

     for(i=0; i < ds_p; i++)
	  fprintf(stderr, "\t%X", i);
     
     fprintf(stderr, "\n");

     for(i=0; i < ds_p; i++)
	  fprintf(stderr, "\t%x", data_stack[i]);

     fprintf(stderr, "\n");
}

void crash(muword errno){
     _errmsg(errno);
     stop();
}
