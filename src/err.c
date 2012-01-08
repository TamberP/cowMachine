#include "sim.h"
#include "err.h"
#include <stdio.h>

/* Bring down the processing unit with the given error. */
void crash(muword errno){
     _stop();
     /* Hammertime */
     _debugmsg(errno);
}

void _debugmsg(muword errno){
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
	  fprintf(stderr, "Out of cheese\n");
     }
}

void _stop(void){
     /* Halt execution */
     status &= ~S_CPU_RUN;
     fprintf(stderr, "CPU halted\n");
}
