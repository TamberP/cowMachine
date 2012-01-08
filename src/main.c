/* cowMachine simulator/testbed
   Copyright (c) 2012 by Tamber Penketh <tamber@furryhelix.co.uk>

   Permission to use, copy, modify, and distribute this software for
 any purpose with or without fee is hereby granted, provided that the
 above copyright notice and this permission notice appear in all
 copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUHTORS DISCLAIM ALL
 WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 AUTHORS BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#include "sim.h"
#include "ops.h"
#include "err.h"

static void usage(char *us){
     fprintf(stderr, "usage: %s [-options ...]\n", us);

     fprintf(stderr, "Where options include:\n");
     fprintf(stderr, "\t-exec <filename>\t file to execute\n");
     exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
     int i, ret, exe_fd;
     char *prog_name = NULL;
     uint8_t exe_header[5];

     for(i = 1; i < argc; i++){
	  char *arg = argv[i];

	  if(arg[0] == '-'){
	       switch(arg[1]){
	       case 'e':
		    /* -exec <filename> */
		    if(++i >= argc) usage(argv[0]);
		    prog_name = argv[i];
		    continue;
	       default:
		    usage(argv[0]);
	       }
	  }
     }

     /* Load exec file */
     if(prog_name != NULL){
	  /*
	     cowMachine executables have a 4 byte header;
	     2 bytes: signature ('MU')
	     1 byte : word-length of target machine
	     1 byte of padding.
	  */
	  if((exe_fd = ret = open(prog_name, O_RDONLY)) < 0){
	       perror("Could not open program file");
	       exit(EXIT_FAILURE);
	  }
	  if((ret = read(exe_fd, (char *) exe_header, 4)) < 3){
	       perror("Could not read header from program file");
	       exit(EXIT_FAILURE);
	  }
	  if((exe_header[0] != 'M') && (exe_header[1] != 'U')){
	       /* Not a mu-file */
	       fprintf(stderr, "%s is not a cowMachine executable\n", prog_name);
	       exit(EXIT_FAILURE);
	  }
	  if(exe_header[2] != (sizeof(muword)*8)){
	       fprintf(stderr, "Word length mismatch; %s created for a %d-bit cowMachine, this machine has %li bits.\n",
		       prog_name, exe_header[2], (sizeof(muword)*8));
	       exit(EXIT_FAILURE);
	  }
     } else {
	  /* TODO: Use a small internal test program */
	  abort();
     }

     /* Clean up a little:
	- Unset all flags.
	- Empty stacks
	- Return program-counter to 0
      */
     fprintf(stderr, "Initialising processor...\n");
     status = 0;
     pc     = 0;
     rs_p   = 0;
     ds_p   = 0;

     /* Load program into memory */
     if((ret = (int) read(exe_fd, prog_mem, (PROG_MEMORY_SIZE - 1))) < 0){
	  perror("Could not read program into program memory");
	  exit(EXIT_FAILURE);
     }

     /* We don't need to hold the file open now that the executable is
      * entirely in the program memory. */
     close(exe_fd);

     /* Begin execution */
     status |= S_CPU_RUN;

     while((status & S_CPU_RUN) > 0){
	  fprintf(stderr, "PC: %x Opcode: %x TOS: %x\n", pc, prog_mem[pc], data_stack[(ds_p)]);
	  decode(prog_mem[pc]);
	  pc = ++pc;
     }

     exit(EXIT_SUCCESS);
}
