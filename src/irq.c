#include "irq.h"

void irq(word irq){
     _rpush(pc);

     /* Jump to handler at: [IVT base address + irq#] */
     pc = main_mem[(IVT_PTR_ADDR + irq)];

     /* Begin execution */
     status |= status_cpu_run;

     /* Now, the rest is handled by normal execution. */
}
