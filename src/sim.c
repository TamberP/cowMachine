/* Simulation code for cow16 */

#include "sim.h"
#include "ops.h"

#include <curses.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

/* Stacks */

word ds[DS_DEPTH];
word rs[RS_DEPTH];
word is[IS_DEPTH];

/* Registers */

word ds_p = 0;
word rs_p = 0;
word is_p = 0;
word pc   = 0;
word status = 0;

/* Main memory */
word main_mem[MAIN_MEM_SIZE];

/* I/O */
word io_a;
word io_b;

char *op_name = "NOP";

char *bios_name = "bios.bin";

void decode (word instruction){
     word target, opcode;

/*
 * F E D C B A 9 | 8 7 6 5 4 3 2 1 0 *
 *    Target     |       Opcode      *
 */

     target = (instruction >> 9);
     opcode = (instruction & ~OPCODE_MASK);

     if((TARGET_CORE == target) | (TARGET_ALL == target)){
	  /* This instruction is destined for us; decode and execute it. */
	  switch(opcode){
	  default:
	  case 0x0:
	       op_name = "HALT\0";
	       core_op_halt();
	       break;
	  case 0x1:
	       op_name = "STORE\0";
	       core_op_store();
	       break;
	  case 0x2:
	       op_name = "FETCH\0";
	       core_op_fetch();
	       break;
	  case 0x3:
	       op_name = "PUSH\0";
	       core_op_push();
	       break;
	  case 0x4:
	       op_name = "ADD\0";
	       core_op_add();
	       break;
	  case 0x5:
	       op_name = "SUB\0";
	       core_op_sub();
	       break;
	  case 0x6:
	       op_name = "MULT\0";
	       core_op_mult();
	       break;
	  case 0x7:
	       op_name = "DIVIDE\0";
	       core_op_div();
	       break;
	  case 0x8:
	       op_name = "DROP\0";
	       core_op_drop();
	       break;
	  case 0x9:
	       op_name = "DUP\0";
	       core_op_dup();
	       break;
	  case 0xA:
	       op_name = "OVER\0";
	       core_op_over();
	       break;
	  case 0xB:
	       op_name = "XOR\0";
	       core_op_xor();
	       break;
	  case 0xC:
	       op_name = "OR\0";
	       core_op_or();
	       break;
	  case 0xD:
	       op_name = "AND\0";
	       core_op_and();
	       break;
	  case 0xE:
	       op_name = "NOT\0";
	       core_op_not();
	       break;
	  case 0xF:
	       op_name = "LSH\0";
	       core_op_lsh();
	       break;
	  case 0x10:
	       op_name = "RSH\0";
	       core_op_rsh();
	       break;
	  case 0x11:
	       op_name = "IF\0";
	       core_op_if();
	       break;
	  case 0x12:
	       op_name = "CALL\0";
	       core_op_call();
	       break;
	  case 0x13:
	       op_name = "RETURN\0";
	       core_op_ret();
	       break;
	  case 0x14:
	       op_name = "JUMP\0";
	       core_op_jump();
	       break;
	  case 0x15:
	       op_name = "IRQ EN\0";
	       core_op_enable_irq();
	       break;
	  case 0x16:
	       op_name = "IRQ\0";
	       core_op_irq();
	       break;
	  case 0x17:
	       op_name = ">r\0";
	       core_op_rpush();
	       break;
	  case 0x18:
	       op_name = "r>\0";
	       core_op_rpop();
	       break;
	  case 0x19:
	       op_name = ">i\0";
	       core_op_ipush();
	       break;
	  case 0x1A:
	       op_name = "i>\0";
	       core_op_ipop();
	       break;
	  case 0x1B:
	       op_name = "CMPEQU\0";
	       core_cmp_eq();
	       break;
	  case 0x1C:
	       op_name = "CMPLT\0";
	       core_cmp_lt();
	       break;
	  case 0x1D:
	       op_name = "CMPGT\0";
	       core_cmp_gt();
	       break;
	  case 0x1E:
	       op_name = "CMPEQ0\0";
	       core_cmp_eq0();
	       break;
	  case 0x1F:
	       op_name = "CMPTRUE\0";
	       core_cmp_true();
	       break;
	  case 0x20:
	       op_name = "CMPFLSE\0";
	       core_cmp_false();
	       break;
	  case 0x21:
	       op_name = "BETWEEN\0";
	       core_cmp_between();
	       break;
	  case 0x22:
	       op_name = "WITHIN\0";
	       core_cmp_within();
	       break;
	  case 0x23:
	       op_name = "NOP";
	       core_nop();
	       break;
	  case 0x24:
	       op_name = "SWAP";
	       core_swap();
	       break;
	  }
     } else {
	  /* TODO: Send the instruction to the right processor. */
     }
}

/* Reset the processor to its initial state.
 *
 * # Main memory may or may not be cleared.
 *
 * # Stacks dropped.
 *
 * # Registers cleared, and, in the case of control register, set to
 * the default state.
 *
 * Data from pre-reset *may* still be present, but this should not be
 * relied on. Using this data may lead to undefined behaviour, should
 * memory locations be filled with random garbage from un-initialised
 * hardware.
 */

void reset(void){
     /* Clear registers */
     ds_p = rs_p = is_p = pc = status = 0;

     /* Reload BIOS. */
     load_bios();

     /* Initialise status register; starting CPU running again. */
     status = status_cpu_run;
}

/* Load the bios from bios.bin or, alternatively, from the given
 * file. */

void load_bios(void){
     int bios_fd, ret;
     int rows, cols;
     WINDOW *err;

     if((bios_fd = ret = open(bios_name, O_RDONLY)) < 0){
	  ret = errno;

	  getmaxyx(stdscr, rows, cols);
	  err = newwin(rows, cols, 0, 0);

	  wattron(err, COLOR_PAIR(1) | A_BOLD);
	  wprintw(err, "Error: Could not load bios file %s: %s\n", bios_name, strerror(ret));
	  wprintw(err, "Press any key to exit.");
	  wrefresh(err);
	  refresh();
	  nodelay(stdscr, 0);
	  getch();
	  endwin();
	  exit(ret);
     }

     if((ret = (int) read(bios_fd, main_mem, (BIOS_SIZE -1))) < 0){
	  ret = errno;
	  getmaxyx(stdscr, rows, cols);

	  err = newwin(rows, cols, 0, 0);

	  wattron(err, COLOR_PAIR(1) | A_BOLD);
	  wprintw(err, "Error: Could not read bios file %s into memory: %s\n", bios_name, strerror(ret));
	  wprintw(err, "Press any key to exit.");
	  wrefresh(err);
	  refresh();
	  nodelay(stdscr, 0);
	  getch();
	  endwin();
	  exit(ret);
     }

     /* Set jump to end of reserved memory */
     main_mem[0x100] = 0x14;
     main_mem[0x101] = RSRV_MEM_END;
}

void make_cow_pat(void){
     int pat_fd, ret;

     int rows, cols;
     WINDOW *err;

     if((pat_fd = ret = open("cow.pat", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) < 0){
	  ret = errno;
	  getmaxyx(stdscr, rows, cols);

	  err = newwin(rows, cols, 0, 0);
	  wattron(err, COLOR_PAIR(1) | A_BOLD);
	  wprintw(err, "Error: Could not write cow.pat: %s\n", strerror(ret));
	  wprintw(err, "Press any key to continue.");

	  wrefresh(err);
	  refresh();
	  nodelay(stdscr, 0);
	  getch();

	  /* Go back to simulation. */
	  nodelay(stdscr, 1);
     }

     if((ret = (int) write(pat_fd, main_mem, (MAIN_MEM_SIZE - 1))) < 0){
	  ret = errno;

	  getmaxyx(stdscr, rows, cols);

	  err  = newwin(rows, cols, 0, 0);
	  wattron(err, COLOR_PAIR(1) | A_BOLD);
	  wprintw(err, "Error: Could not write cow.pat: %s\n", strerror(ret));
	  wprintw(err, "Press any key to continue.");

	  wrefresh(err);
	  refresh();
	  nodelay(stdscr, 0);
	  getch();

	  nodelay(stdscr, 1);
     }

     close(pat_fd);
}
