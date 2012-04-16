#ifndef _SIM_H_
#define _SIM_H_

#include <stdint.h>

#define KILOWORD * 1024
#define MEGAWORD * (1024 KILOWORD)

#define DATA_STACK_DEPTH 5
#define RET_STACK_DEPTH  5
#define INTR_STACK_DEPTH 5

#define MAIN_MEM_SIZE  (512 KILOWORD)

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

/* Data stack. Used by most operations as working space, and for
 * transferring arguments and returning results. */
extern muword data_stack[(DATA_STACK_DEPTH + 1)];

/* Return stack. Return addresses are pushed onto this as part of a
 * CALL. Can also be used, carefully, as extra working space. */
extern muword ret_stack[(RET_STACK_DEPTH + 1)];

/* Interrupt stack. Used to transfer arguments into and out of
 * interrupt handlers. */
extern muword int_stack[(INTR_STACK_DEPTH + 1)];

/* Main memory. Contains program code, stored data, memory-mapped I/O,
 * etc. */
extern muword main_mem[MAIN_MEM_SIZE];

/* ** Registers ** */
extern muword pc;   /* Program counter */
extern muword ds_p; /* Data-stack pointer. */
extern muword rs_p; /* Return-stack pointer. */
extern muword is_p; /* Interrupt-stack pointer. */
extern muword status; /* Control/status register */

void halt(void);
void reset(void);
void decode(muword opcode);

#endif /* _SIM_H_ */
