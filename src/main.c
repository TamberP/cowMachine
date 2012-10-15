/* cow16

   Draft 0.0 implementation.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

#include "sim.h"
#include "ui.h"
#include "io.h"

static void usage(char *us){
     fprintf(stderr, "usage: %s [-options ...]\n", us);
     fprintf(stderr, "Where options include:\n");
     fprintf(stderr, "\t-exec <filename>\t file containing code to execute\n");
     fprintf(stderr, "\t-bios <filename>\t File containing BIOS code.\n");
     fprintf(stderr, "\t-delay <num>\t\t Number of seconds between each cpu cycle. (Defaults to 0)\n");
     exit(EXIT_FAILURE);
}

int cycle_delay = 0;
bool single_step = false;

static void * cpu_cycles(void *arg){
     arg = NULL; /* To shut the compiler up about arg being un-used;
		  * since it's just there so the function has the
		  * correct prototype for pthread_create */

     while(1){
	  if(((status & status_cpu_run) > 0) | (single_step == true)){
	       decode(main_mem[pc]);
	       if(main_mem[pc] == 00)
		    op_name = "HALT";
	       else
		    pc = (pc + 1);
	  }

	  if(single_step == true)
	       single_step = false;

	  sleep(cycle_delay);
     }
     return NULL;
}


int main(int argc, char **argv){
     int i, escape_loop = 0;

     int row, col;

     WINDOW *r_status, *r_port_a, *r_port_b;
     WINDOW *s_data, *s_return, *s_intr;
     WINDOW *key_bind, *statuswin;

     char *prog_name, *io_a_file = "", *io_b_file = "";
     pthread_t *cpu_sim = NULL;

     for(i = 1; i < argc; i++){
	  char *arg = argv[i];

	  if(arg[0] == '-'){
	       switch(arg[1]){
	       case 'b':
		    /* -bios <filename> */
		    if(++i >= argc) usage(argv[0]);

		    bios_name = argv[i];
		    continue;
	       case 'e':
		    /* -exec <filename> */
		    if(++i >= argc) usage(argv[0]);

		    prog_name = argv[i];
		    continue;
	       case 'd':
		    /* -delay <num> */
		    if(++i >= argc) usage(argv[0]);
		    cycle_delay = atoi(argv[i]);
		    continue;
	       case 'c':
		    /* -com? <file> */
		    switch(arg[4]){
		    case '1':
			 /* -com1 <file> */
			 if(++i >= argc) usage(argv[0]);
			 io_a_file = argv[i];
			 continue;
		    case '2':
			 /* -com2 <file> */
			 if(++i >= argc) usage(argv[0]);
			 io_b_file = argv[i];
			 continue;
		    default:
			 usage(argv[0]);
			 break;
		    }

	       default:
		    usage(argv[0]);
		    break;
	       }
	  }
     } /* End of option-munching */

     if((i = initialise_io(io_a_file, io_b_file)) < 0){
	  fprintf(stderr, "Couldn't initialise I/O: %s", strerror(i));
     }

     /* Begin to set up the display */
     initscr();
     clear();
     noecho();
     cbreak();
     nodelay(stdscr, 1);

     start_color();

     init_pair(1, COLOR_RED, COLOR_BLACK);
     init_pair(2, COLOR_YELLOW, COLOR_BLACK);
     init_pair(3, COLOR_BLUE, COLOR_BLACK);
     init_pair(4, COLOR_WHITE, COLOR_BLUE);

     attron(COLOR_PAIR(1) | A_BOLD);
     mvprintw(1, 5, "cow");
     attron(COLOR_PAIR(4));
     mvprintw(1, 8, "16");
     attroff(COLOR_PAIR(4) | A_BOLD);

     attron(A_REVERSE);
     mvprintw(1, 16, " Have you hugged your cow today? ");
     attroff(A_REVERSE);

     refresh();
     getmaxyx(stdscr, row, col);

     r_status = newwin(1, (5 * NUM_STATUS_FLAGS + 8), 3, 1);
     statuswin = newwin(2, 20, 4, 1);
     r_port_a = newwin(1, (3*16) + 8, 7, 1);
     r_port_b = newwin(1, (3*16) + 8, 8, 1);
     s_data = newwin((DS_DEPTH + 2), 6, 3, 58);
     s_return = newwin((RS_DEPTH + 2), 6, 3, 65);
     s_intr = newwin((IS_DEPTH + 2), 6, 3, 72);

     ui_update_reg_status(r_status);
     ui_update_reg_port(r_port_a, 'A', io_a);
     ui_update_reg_port(r_port_b, 'B', io_b);
     ui_update_stack(s_data, ds, DS_DEPTH, ds_p, "DATA");
     ui_update_stack(s_return, rs, RS_DEPTH, rs_p, "RETN");
     ui_update_stack(s_intr, is, IS_DEPTH, is_p, "INTR");
     if(op_name == NULL){ op_name = "NOP"; }
     ui_update_info(statuswin, op_name);

     key_bind = newwin(1, col, (row - 1), 0);

     {
	  /* Show key bindings */
	  wattron(key_bind, A_BOLD);
	  mvwprintw(key_bind, 0, 0, "Q");
	  wattroff(key_bind, A_BOLD);
	  mvwprintw(key_bind, 0, 1, ": Quit");

	  wattron(key_bind, A_BOLD);
	  mvwprintw(key_bind, 0, 8, "S");
	  wattroff(key_bind, A_BOLD);
	  mvwprintw(key_bind, 0, 9, ": Step");

	  wattron(key_bind, A_BOLD);
	  mvwprintw(key_bind, 0, 16, "R");
	  wattroff(key_bind, A_BOLD);
	  mvwprintw(key_bind, 0, 17, ": Run");

	  wattron(key_bind, A_BOLD);
	  mvwprintw(key_bind, 0, 23, "E");
	  wattroff(key_bind, A_BOLD);
	  mvwprintw(key_bind, 0, 24, ": Reset");

	  wattron(key_bind, A_BOLD);
	  mvwprintw(key_bind, 0, 32, "D");
	  wattroff(key_bind, A_BOLD);
	  mvwprintw(key_bind, 0, 33, ": Dump Core");

	  wnoutrefresh(key_bind);
     }

     doupdate();
     refresh();

     /* Load the BIOS file */
     load_bios();

     cpu_sim = malloc(sizeof(pthread_t));
     pthread_create(cpu_sim, NULL, (&cpu_cycles), NULL);

     while(!escape_loop){
	  char c = getch();

	  switch(c){
	  case 'q':
	  case 'Q':
	       escape_loop = 1;
	       break;
	  case 'r':
	  case 'R':
	       /* Toggle run status. */
	       if(!(status & status_cpu_run)){
		    /* Now running */
		    status |= status_cpu_run;
	       } else {
		    status &= ~status_cpu_run;
	       }

	       break;
	  case 's':
	  case 'S':
	       /* Run 1 cycle. TODO: Make work with multithreading.*/
	       single_step = true;
	       break;
	  case 'e':
	  case 'E':
	       /* We don't call reset() here, because we may not want to start the CPU running again afterwards. */

	       /* Clear registers */
	       ds_p = rs_p = is_p = pc = status = 0;

	       /* Reload BIOS. */
	       load_bios();
	  case 'D':
	  case 'd':
	       /* Dump our 'core' memory for examination with an
		* external application. */
	       make_cow_pat();
	  case ERR:
	  default:
	       break;
	  }

	  ui_update_reg_status(r_status);
	  ui_update_reg_port(r_port_a, 'A', io_a);
	  ui_update_reg_port(r_port_b, 'B', io_b);

	  ui_update_stack(s_data, ds, DS_DEPTH, ds_p, "DATA");
	  ui_update_stack(s_return, rs, RS_DEPTH, rs_p, "RETN");
	  ui_update_stack(s_intr, is, IS_DEPTH, is_p, "INTR");
	  ui_update_info(statuswin, op_name);
	  doupdate();
     }

     endwin();
     return EXIT_SUCCESS;
}
