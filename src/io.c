/* CowMachine 16 simulator I/O thingies.

   Copyright © 2012 Tamber Penketh <tamber@furryhelix.co.uk>
*/

#include "sim.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * TODO: So we get notified when some I/O happens to the FD.
#include <sys/select.h>
*/

static int fd_io_a = 0, fd_io_b = 0;

int initialise_io(char *file_io_a, char *file_io_b){
     if(strlen(file_io_a) > 0){
	  /* IO A has a file to communicate via.

	     Create it if it doesn't exist, with rw------ (600)
	     permissions; and open it as non-blocking.)
	  */
	  fd_io_a = open(file_io_a, O_RDWR | O_CREAT | O_NDELAY, S_IRUSR | S_IWUSR);
	  if(fd_io_a < 0){
	       /* Well, crap. Throw the error back to something that
		* can display the message to the user. */
	       return -errno;
	  }
     }

     if(strlen(file_io_b) > 0){
	  /* Same as above, but for port B. TODO: De-duplicate this crap. */
	  fd_io_b = open(file_io_b, O_RDWR | O_CREAT | O_NDELAY, S_IRUSR | S_IWUSR);
	  if(fd_io_b < 0){
	       return -errno;
	  }
     }

     return 0;
}

int shutdown_io(void){
     if((fd_io_a != 0) && (close(fd_io_a) < 0)){
	  return -errno;
     }

     if((fd_io_b != 0) && (close(fd_io_b) < 0)){
	  return -errno;
     }
     return 0;
}

/* Send the given data to whatever's listening on the other end. */
int io_xmit(int fd, word data){
     uint8_t octet[2];
     int ret;

     /* XXX: This seems a little bit dodgy to me */

     octet[0] = (data>>8) & 255;
     octet[1] = (data & 255);

     if((ret = write(fd, octet, 2)) < 2){
	  /* Wat. */
	  if(ret < 0){
	       /* Oh, hooray, an error. */
	       return -errno;
	  } else {
	       /* Hell if I know what just happened, but we couldn't
		* write *two lousy octets*?! */
	  }
     }
     return 0;
}

word io_recv(int fd){
     uint8_t octet[2];
     word tmp;

     read(fd, octet, 2);

     tmp = ((word) octet[0] << 8);
     return (tmp & (word) octet[1]);
}

int io_a_xmit(word data){
     return io_xmit(fd_io_a, data);
}

int io_b_xmit(word data){
     return io_xmit(fd_io_b, data);
}

word io_a_recv(void){
     return (io_recv(fd_io_a));
}

word io_b_recv(void){
     return (io_recv(fd_io_b));
}
