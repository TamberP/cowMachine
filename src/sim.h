#ifndef _SIM_H_
#define _SIM_H_

#include <stdint.h>

#define KILOWORD * 1024


#define DATA_STACK_DEPTH 5
#define RET_STACK_DEPTH 5

#define PROG_MEMORY_SIZE (512 KILOWORD)


/* Status flags */
enum {
  S_CPU_RUN = 0x01
};

#ifdef COW_16_BITS
typedef uint16_t muword;
#elif defined COW_32_BITS
typedef uint32_t muword;
#elif defined COW_64_BITS
typedef uint64_t muword;
#else
typedef uint8_t muword;
#endif

typedef struct mucode {
  muword opcode;
  void * funct;
} mucode_t;

extern muword data_stack[DATA_STACK_DEPTH];
extern muword ret_stack[RET_STACK_DEPTH];
extern muword prog_mem[PROG_MEMORY_SIZE];
extern muword pc, ds_p, rs_p, status;

void decode(muword opcode);

#endif /* _SIM_H_ */
