#include "sim.h"

int initialise_io(char *file_io_a, char *file_io_b);
int shutdown_io(void);

int io_a_xmit(word data);
int io_b_xmit(word data);

word io_a_recv(void);
word io_b_recv(void);
