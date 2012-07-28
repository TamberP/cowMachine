CFLAGS:=-std=c89 -Wall -Wextra -Werror -pedantic -g -I./include
LINT:=splint
LINT_OPTS:= -I./include
LDFLAGS:=-lcurses

cm:  sim.c ops.c ui.c main.c
	${CC} ${CFLAGS} -c $?
	${CC} ${CFLAGS} ${LDFLAGS} -o cm *.o

.PHONY: lint
lint: *.c
	${LINT} ${LINT_OPTS} $?

.PHONY: clean
clean:
	-rm *.o core cm *~ cow.pat
