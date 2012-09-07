CFLAGS:=-std=c89 -Wall -Wextra -Werror -Wno-unused -pedantic -g -I./include
LINT:=splint
LINT_OPTS:= -I./include
LDFLAGS:=-lcurses -lpthread

cm:  sim.c ops.c ui.c irq.c main.c
	${CC} ${CFLAGS} -c $?
	${CC} ${CFLAGS} ${LDFLAGS} -o cm *.o

.PHONY: lint
lint: *.c
	${LINT} ${LINT_OPTS} $?

.PHONY: clean
clean:
	-rm *.o core cm *~ cow.pat
