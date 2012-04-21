#include "irq.h"
#include "sim.h"
#include "err.h"

#include <stdio.h>

void interrupt(muword irq){
     /* TODO: Disable interrupts. (Not required *yet*, since none will happen whilst we're executing this code. */
     switch(irq){
     case IRQ_NMI:
	  fprintf(stderr, "Non-Maskable Interrupt received");
	  break;
     case IRQ_INVALID_OP:
	  /* Clever things are meant to happen here about finding
	   * extensions to the instruction-set, as provided by
	   * addon-processors and other gadgetry. Until, then, though,
	   * just bitch and blow up. */
	  fprintf(stderr, "Opcode '%x' executed at address %x\n", _is_pop(), _is_pop());
	  crash(EBADOP);
	  break;
     case IRQ_STACK_OVER:
	  crash(EOSTACK);
	  break;
     case IRQ_STACK_UNDER:
	  crash(EUSTACK);
	  break;
     case IRQ_MCE:
	  fprintf(stderr, "MACHINE_CHECK: %x:%x", _is_pop(), _is_pop());
	  crash(E_MCE);
	  break;
     default:
	  fprintf(stderr, "Spurious interrupt %x received", irq);
	  crash(0);
	  break; /* Not that this is strictly required, since crash()
		  * aborts; but hey! */
     }
     /* TODO: Re-enable interrupts. */
}

void _is_push(muword val){
     is_p = (is_p + 1);
     int_stack[is_p] = val;
}

muword _is_pop(void){
     rs_p = (rs_p - 1);
     return ret_stack[(rs_p + 1)];
}
