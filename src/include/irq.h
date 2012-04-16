#ifndef _IRQ_H_
#define _IRQ_H_

#include "sim.h"

enum {
     IRQ_NMI = 0x01,
     IRQ_INVALID_OP,
     IRQ_STACK_OVER,
     IRQ_STACK_UNDER,
     IRQ_MCE
     /* Interrupts 6 to 16 are reserved for internal use. 17 to
      * (2^word-width)-1, however, are available for software and
      * hardware interrupts.*/
};

void interrupt(muword irq);

void _is_push(muword val);
muword _is_pop(void);

#endif /* _IRQ_H_ */
