#ifndef _ERR_H_
#define _ERR_H_

#include "sim.h"

enum {
     EBADOP = 0x01, /* Bad op-code */
     EUSTACK,
     EOSTACK
};

void crash(muword errno);
void _errormsg(muword errno);
void _stackprint(void);

#endif /* _ERR_H_ */
