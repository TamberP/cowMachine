#ifndef _ERR_H_
#define _ERR_H_

#include "sim.h"


/* Error conditions */
enum {
     EBADOP = 0x01, /* Bad op-code */
     EUSTACK,  /* Stack underflow (Attempt to pop from empty stack) */
     EOSTACK   /* Stack overflow  (Attempt to push to full stack) */
};

void crash(muword errno);

void _debugmsg(muword errno);

void _stop();

#endif /* _ERR_H_ */
