#include "ui.h"

/* Update the display of the status register */
void ui_update_reg_status(WINDOW *r_status) {
     wattron(r_status, COLOR_PAIR(1) | A_BOLD);
     mvwprintw(r_status, 0, 0, "STATUS: ");
     wattroff(r_status, A_BOLD);

     /* CPU Running? */
     if((status & status_cpu_run)){
	  wattron(r_status, A_REVERSE);
     }
     mvwprintw(r_status, 0, 8, " RUN ");
     if((status & status_cpu_run)){
	  wattroff(r_status, A_REVERSE);
     }

     if((status & status_irq_enable)){
	  wattron(r_status, A_REVERSE);
     }
     mvwprintw(r_status, 0, 13, " INT ");
     if((status & status_irq_enable)){
	  wattroff(r_status, A_REVERSE);
     }

     wnoutrefresh(r_status);
}


/* Update display of I/O ports. */
void ui_update_reg_port(WINDOW *r_port, char port_name, word port){
     /* Port A update */
     wattron(r_port, COLOR_PAIR(2) | A_BOLD);
     mvwprintw(r_port, 0, 0, "PORT %c: ", port_name);
     wattroff(r_port, A_BOLD);

     if((port & 32768)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 8, " F ");
     wattroff(r_port, A_REVERSE);

     if((port & 16384)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 11, " E ");
     wattroff(r_port, A_REVERSE);

     if((port & 8192)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 14, " D ");
     wattroff(r_port, A_REVERSE);

     if((port & 4096)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 17, " C ");
     wattroff(r_port, A_REVERSE);

     if((port & 2048)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 20, " B ");
     wattroff(r_port, A_REVERSE);

     if((port & 1024)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 23, " A ");
     wattroff(r_port, A_REVERSE);

     if((port & 512)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 26, " 9 ");
     wattroff(r_port, A_REVERSE);

     if((port & 256)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 29, " 8 ");
     wattroff(r_port, A_REVERSE);

     if((port & 128)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 32, " 7 ");
     wattroff(r_port, A_REVERSE);

     if((port & 64)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 35, " 6 ");
     wattroff(r_port, A_REVERSE);

     if((port & 32)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 38, " 5 ");
     wattroff(r_port, A_REVERSE);

     if((port & 16)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 41, " 4 ");
     wattroff(r_port, A_REVERSE);

     if((port & 8)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 44, " 3 ");
     wattroff(r_port, A_REVERSE);

     if((port & 4)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 47, " 2 ");
     wattroff(r_port, A_REVERSE);

     if((port & 2)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 50, " 1 ");
     wattroff(r_port, A_REVERSE);

     if((port & 1)){ wattron(r_port, A_REVERSE); }
     mvwprintw(r_port, 0, 53, " 0 ");
     wattroff(r_port, A_REVERSE);

     wnoutrefresh(r_port);
}

/* Display current value of program counter and name of current instruction. */
void ui_update_info(WINDOW *info, char *opname){
     wattron(info, A_BOLD);
     mvwprintw(info, 0, 0, "PC:");
     wattroff(info, A_BOLD);
     mvwprintw(info, 0, 4, "%x", pc);

     wattron(info, A_BOLD);
     mvwprintw(info, 0, 9, "Op:");
     wattroff(info, A_BOLD);
     mvwprintw(info, 0, 13, "%s", opname);

     wclrtoeol(info);
     wnoutrefresh(info);
}

/* Display stack. */
void ui_update_stack(WINDOW *s_window, word *stack, short stack_depth, short stack_p, char *name){
     short i;

     wattron(s_window, A_BOLD);
     mvwprintw(s_window, 0, 0, " %s ", name);
     wattroff(s_window, A_BOLD);

     for(i=0; i<stack_depth; i++){
	  mvwprintw(s_window, (i+2), 0, "%0X", stack[i]);
	  wclrtoeol(s_window);
	  if(i == stack_p)
	       mvwprintw(s_window, (i + 2), 5, "<");
     }

     wnoutrefresh(s_window);
}
