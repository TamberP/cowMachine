#ifndef _IRQ_H_
#define _IRQ_H_

#include "sim.h"

enum {
     IRQ_NMI = 0x01,
     IRQ_INVALID_OP,
     IRQ_STACK_OVER,
     IRQ_STACK_UNDER,
     IRQ_MCE
};

void interrupt(muword irq);

void _is_push(muword val);
muword _is_pop(void);

#endif /* _IRQ_H_ */
