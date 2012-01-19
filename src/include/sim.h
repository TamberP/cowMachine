#ifndef _SIM_H_
#define _SIM_H_

#include <stdint.h>

#define KILOWORD * 1024
#define MEGAWORD * (KILOWORD * 1024)

#define DATA_STACK_DEPTH 5
#define RET_STACK_DEPTH  5

#define PROG_MEM_SIZE  (512 KILOWORD)

enum {
     S_CPU_RUN = 0x01
};

#if defined(COW_16_BITS)
     typedef uint16_t muword;
     typedef int16_t smuword;
#elif defined(COW_32_BITS)
     typedef unint32_t muword;
     typedef int32_t smuword;
#elif defined(COW_64_BITS)
     typedef uint64_t muword;
     typedef int64_t smuword;
#else
     typedef uint8_t muword;
     typedef int8_t smuword;
#endif

extern muword data_stack[DATA_STACK_DEPTH];
extern muword ret_stack[RET_STACK_DEPTH];
extern muword prog_mem[PROG_MEM_SIZE];

/* Program counter, data-stack pointer, return-stack pointer, 
 * status register */
extern muword pc, ds_p, rs_p, status;
  
void stop(void);
void reset(void);
void decode(muword opcode);

#endif /* _SIM_H_ */
