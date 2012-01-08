CFLAGS:=-std=c89 -Wall -pedantic -g
LINT:=splint
LINT_OPTS:=

8: *.c
	gcc ${CFLAGS} -c $?
	gcc ${CFLAGS} ${LDFLAGS} -o cm *.o

.PHONY: 16
16: *.c
	gcc -DCOW_16_BITS ${CFLAGS} -c $?
	gcc ${CFLAGS} ${LDFLAGS} -o cm.16 *.o

.PHONY: 32
32: *.c
	gcc -DCOW_32_BITS ${CFLAGS} -c $?
	gcc ${CFLAGS} ${LDFLAGS} -o cm.32 *.o

.PHONY: 64
64: *.c
	gcc -DCOW_64_BITS ${CFLAGS} -c $?
	gcc ${CFLAGS} ${LDFLAGS} -o cm.64 *.o

.PHONY: clean
clean:
	-rm *.o core cm*

.PHONY: lint
lint: *.c
	${LINT} ${LINT_OPTS} $?
