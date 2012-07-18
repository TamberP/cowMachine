#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

#include <stdint.h>

#define KILOWORD * 1024
#define MEGAWORD * (1024 KILOWORD)

#define DS_DEPTH 10
#define RS_DEPTH 10
#define IS_DEPTH 10

#ifndef MAIN_MEM_SIZE
#define MAIN_MEM_SIZE (64 KILOWORD)
#endif /* MAIN_MEM_SIZE */

#define NUM_STATUS_FLAGS 2

enum {
     status_cpu_run = 0x01,
     status_irq_enable = 0x02
};

typedef uint16_t word;

/* Data stack. Used by most operations as working space, and for
 * transferring arguments and results. */
extern word ds[DS_DEPTH];

/* Return stack. Return addresses are pushed onto this as part of a
 * call. Can also be used, with care, as extra working space. */
extern word rs[RS_DEPTH];

/* Interrupt stack. Used to transfer arguments into and out of
 * interrupt handlers. Should not be used as temporary storage unless
 * interrupts are disabled. */
extern word is[IS_DEPTH];

/* Contains program code, stored data, and other such miscellany. */
extern word main_mem[MAIN_MEM_SIZE];

/* Registers */
extern word pc;   /* Program counter */
extern word ds_p; /* Data stack pointer. */
extern word rs_p; /* Return stack pointer */
extern word is_p; /* Interrupt stack pointer */
extern word status; /* Control/status register */

/* Used to mask off parts of the instruction to get at its component
 * parts. */

#define TARGET_MASK 511
#define OPCODE_MASK 65024

/* Processor instruction targets */
enum {
     TARGET_CORE = 0x0,
     TARGET_ALL  = 0x7f
};

/* Size of BIOS in bytes. */
#define BIOS_SIZE 256

/* Load the bios into its mapped memory (First 254 words) from
 * bios.bin or, alternatively, from the named file. */
void load_bios(void);

/* Decode and execute an instruction. */
void decode(word instruction);

extern char *bios_name;

/* I/O ports */
extern word io_a;
extern word io_b;

/* Addresses for ports and port controls. */
#define MMAP_PORT_A 0x103
#define MMAP_PORT_B 0x105

#define MMAP_CTRL_A 104
#define MMAP_CTRL_B 106

/* Used to allow pretty-printing of the current operation */
extern char *op_name;

#endif /* _SIMULATOR_H_ */
