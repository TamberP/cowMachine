#ifndef _UI_H_
#define _UI_H_

#include <curses.h>
#include "sim.h"

/* Displays status register.

   Currently implemented flags:
   RUN : CPU Running (Obviously)
   INT : Interrupts enabled.

   Arguments: Window to display status flags in.
 */
void ui_update_reg_status(WINDOW *r_status);

/* Displays status of I/O port.

   Prints the status of the give I/O port. 16 bit flags, set inverse
   for ON, and normal for OFF.

   Arguments:
   1. Window to display the port in.
   2. Character to lable the port. ('A', 'B', etc.)
   3. The value currently on the port.
 */
void ui_update_reg_port(WINDOW *r_port, char port_name, word port);

/* Displays various info about the processor.
   Currently implemented:
   * Value of program counter.
   * Name of current opcode.

   Arguments:
   1. Window to display info in.
   2. String naming opcode.
*/
void ui_update_info(WINDOW *info, char *opname);

/* Displays given stack, with an arrow pointing to current location of stack pointer.

   Arguments:
   1. Window to display the stack in.
   2. Stack
   3. Depth of stack.
   4. Stack pointer.
   5. Label for stack.
*/

void ui_update_stack(WINDOW *s_window, word *stack, short stack_depth, short stack_p, char *name);

#endif /* _UI_H_ */
