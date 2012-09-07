# cow16 Specification #
* Author:  Tamber Penketh <tamber@furryhelix.co.uk>
* Version: 0.4
* Date:    2012-07-18T01:46:28+01:00

# Introduction #

The cow16 is a toy stack-machine processor, based off the cowMachine
design; polished up a little bit (as much as you can a turd, anyway),
with new interface and a standard-ish word-width.

As the name implies, it has a 16-bit word-length; it is very
simplistic, does no signed math, no instruction packing, no
pipelining, etc.

Unlike the original cowMachine, I have not designed the cow16 to be
extendable to words longer than 16 bits. (It is possible to make it
so, but that is beyond the scope of my design. It'd also be a pain,
since there's no assembler; so you have to hand-assemble everything
you want to run.)

Thanks go to [Jesus Arias](http://www.ele.uva.es/~jesus) for f84,
which showed me the error of my ways when it came to the original UI.

## License ##

cow16 Copyright © 2012 Tamber Penketh <tamber@furryhelix.co.uk>

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHORS DISCLAIM ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.

## Why 'cowMachine'? ##

Because I had µ (mu) stuck in my brain whilst thinking about
microprocessors, so it got stuck with the name muPU.  If you read that
out loud, and you're almost as childish as I am, you'll see a small
issue with it. And so, it became the cowMachine.)

With me re-working it heavily for a new 16-bit incarnation after
realising how much of a pain the 8-bit limitation of the original
incarnation was, it became cow16 for presumably obvious reasons.

## Building the cow ##

Building cow16 is fairly easy; it requires a C compiler, a Make (Only
tested with GNU Make.), and a curses library. (So far, it requires the
use of a terminal with colours. xterm-color is what I use.)

It simply requires the typing of 'make', or 'gmake'.

The cow16 has been built and tested on, so far:

* amd64 Linux 3.4.4, Binutils 2.21.1, with GCC 4.5.3
* amd64 Linux 3.4.4, Binutils 2.21.1, with clang 3.1
* i386  SunOS 5.11,  Binutils 2.19,   with gcc 3.4.3 (OpenIndiana)

With the above, on my machine, it compiles without warnings. (The
CFLAGS are fairly strict, so this is probably a good sign.)

## Programming the cow16 ##

As yet, there is no support for loading hex-files other than the bios,
which is limited to 256 words of code. In theory, though, it would be
possible to use this to configure an I/O port to load the rest of the
code through.

However, see the [TODO](#TODO).

## Bugs ##

* Lots.
* Probably some relating to endianess.
* Crashes when stacks are over/underflowed need proper
  handling. (Probably interrupts.)

# Features #

* Stack-based
* 0x10000 words of RAM
* 5 registers
    + PC: Program counter
    + DSp: Data-stack pointer
    + RSp: Return-stack pointer
    + ISp: Interrupt-stack pointer
    + ctrl: Status and control
* 3 stacks
    + Data stack
    + Return stack
    + Interrupt stack
* Memory-mapped I/O [Which doesn't I, and only some O, at present.]



## TODO ##

* Interrupts
* An assembler/compiler, because hand-assembling is a pain in the proverbial.
* Less asploding on stack under/overflows. (Probably waiting on interrupts.)

### Done ###

* Multithreading.  commit e32958a
* Configurable delay between CPU cycles. (Blocks on multithreading.)  Done: commit e32958a
* Make single-step work again after multithreading splittage. Done: commit 342a5f9


### I/O ###

There is no real I/O yet. The only thing that comes close is the blinkenlights
of the UI, and that's not terribly useful. Probably blocks on multithreading; or
at least would make more sense to do after multithreading.

I have no plans for how the read/writes from the ports will do 'useful' things,
yet. Possibly something along the lines of sockets to swappable interface
programs.

### Accessory Processors ###

Same as I/O, but moreso. I have no idea for design, nor
implementation; it's just waiting on a sufficiently ${X} ${Y}.

# Instructions #

Instructions are fetched from main-memory[PC], decoded, executed and
then PC is incremented.

Instructions are 1 or 2 words long; with instruction (and target)
fully defined within the first word. The second word carries extra
data for certain opcodes; this second word is not decoded, but is a
raw data value, to be used depending on the instruction. (e.g. target
address of jumps.)

The instruction format is as follows:

    [ F E D C B A 9 ]|[ 8 7 6 5 4 3 2 1 0 ]
     Processor type  |              Opcode

The `processor type` value in the instruction allows for extension
processors to use the full range of opcodes (0 - 511) without fear of
conflicts with other processors (both extension and core).

This allows multiple different types of co-processor to be added to a
system, each using the same opcodes for vastly different purposes;
and, as long as the correct processor-type is given, and no
incompatible processors are given differing processor-type codes, no
misinterpretation of instructions meant for other processors can
occur.

There is no provision for handling multiple processors of the same
type in the system.

## Assigned Type Values

*  0h: Core processor
* 7Fh: All processors

## Core Opcodes

Unless otherwise specified, all instructions only modify the data
stack.

	+--------------+------------+----------------------+--------------------------------------+
	| Value        | Mnemonic   | Stack effect         | Description                          |
	+==============+============+======================+======================================+
	| 00h          | halt       | ( -- )               | Stops execution until next interrupt.|
	+--------------+------------+----------------------+--------------------------------------+
	| 01h          | !          | ( n addr -- )        | Store value n at location addr in    |
	|              |            |                      |main memory.                          |
	+--------------+------------+----------------------+--------------------------------------+
	| 02h          | @          | ( addr -- n )        | Fetch value from location addr in    |
	|              |            |                      |main memory.                          |
	+--------------+------------+----------------------+--------------------------------------+
	| 03h          | push       | ( -- n )             | Two-word instruction, used to load   |
	|              |            |                      |values onto the stack.                |
	+--------------+------------+----------------------+--------------------------------------+
	| 04h          | +          | ( n2 n1 -- n )       | Add the top two values on stack      |
	|              |            |                      |together.                             |
	+--------------+------------+----------------------+--------------------------------------+
	| 05h          | -          | ( n2 n1 -- n )       | Subtract value at top of stack from  |
	|              |            |                      |value second on stack.                |
	+--------------+------------+----------------------+--------------------------------------+
	| 06h          | *          | ( n2 n1 -- n )       | Multiply top two values on stack.    |
	+--------------+------------+----------------------+--------------------------------------+
	| 07h          | /          | ( n2 n1 -- n )       | Divide value at top of stack by value|
	|              |            |                      |second on stack.                      |
	+--------------+------------+----------------------+--------------------------------------+
	| 08h          | drop       | ( n -- )             | Discard value currently at top of    |
	|              |            |                      |stack.                                |
	+--------------+------------+----------------------+--------------------------------------+
	| 09h          | dup        | (n -- n n )          | Duplicate value currently at top of  |
	|              |            |                      |stack.                                |
	+--------------+------------+----------------------+--------------------------------------+
	| 0Ah          | over       | ( n2 n1 -- n2 n1 n2 )| Duplicate value *second* on stack, to|
	|              |            |                      |top of stack.                         |
	+--------------+------------+----------------------+--------------------------------------+
	| 0Bh          | ^          | ( n2 n1 -- n )       | Bitwise XOR of top two values on     |
	|              |            |                      |stack.                                |
	+--------------+------------+----------------------+--------------------------------------+
	| 0Ch          | |          | ( n2 n1 -- n )       | Bitwise OR of top two values on      |
	|              |            |                      |stack.                                |
	+--------------+------------+----------------------+--------------------------------------+
	| 0Dh          | &          | ( n2 n1 -- n )       | Bitwise AND of top two values on     |
	|              |            |                      |stack.                                |
	+--------------+------------+----------------------+--------------------------------------+
	| 0Eh          | ~          | ( n2 n1 -- n )       | Bitwise NOT of top two values on     |
	|              |            |                      |stack.                                |
	+--------------+------------+----------------------+--------------------------------------+
	| 0Fh          | <<         | ( n2 n1 -- n )       | Bitwise logical left-shift of top    |
	|              |            |                      |value on stack, by number of places   |
	|              |            |                      |given by value second on stack.       |
	+--------------+------------+----------------------+--------------------------------------+
	| 10h          | >>         | ( n2 n1 -- n )       | Bitwise logical right-shift of top   |
	|              |            |                      |value on stack, by number of places   |
	|              |            |                      |given by value second on stack.       |
	+--------------+------------+----------------------+--------------------------------------+
	| 11h          | if         | (flag -- )           | Two-word instruction. Second word is |
	|              |            |                      |used as target of the jump made if the|
	|              |            |                      |flag, at top of stack, is 1.          |
	+--------------+------------+----------------------+--------------------------------------+
	| 12h          | call       | ( -- )               | Two-word instruction. Second word is |
	|              |            |                      |used as the target of the jump to the |
	|              |            |                      |beginning of the function. (Functions |
	|              |            |                      |are loaded into a fixed address, and  |
	|              |            |                      |all CALLs are merely jumps that add a |
	|              |            |                      |return address to the return stack.)  |
	+--------------+------------+----------------------+--------------------------------------+
	| 13h          | ret        | ( -- )               | Return from last-called function, to |
	|              |            |                      |instruction immediately after the CALL|
	|              |            |                      |and its pointer word.                 |
	+--------------+------------+----------------------+--------------------------------------+
	| 14h          | jump       | ( -- )               | Non-conditional jump. Two word       |
	|              |            |                      |instruction. Second word is used as   |
	|              |            |                      |the address in main-memory to resume  |
	|              |            |                      |execution at. Does *not* add an       |
	|              |            |                      |address to the return stack.          |
	+--------------+------------+----------------------+--------------------------------------+
	| 15h          | irq-enable | ( flag -- )          | Enable/disable interrupts, depending |
	|              |            |                      |on flag at the top of stack. 0 to     |
	|              |            |                      |disable, any other value to enable.   |
	+--------------+------------+----------------------+--------------------------------------+
	| 16h          | irq        | ( I: n -- )          | Raise interrupt from                 |
	|              |            |                      |software. Interrupt to be raised is   |
	|              |            |                      |given as top value of interrupt stack.|
	+--------------+------------+----------------------+--------------------------------------+
	| 17h          | >r         | ( n -- R: n )        | Move value from data stack, to return|
	|              |            |                      |stack.                                |
	+--------------+------------+----------------------+--------------------------------------+
	| 18h          | r>         | ( R: n -- n )        | Move value from return stack to data |
	|              |            |                      |stack.                                |
	+--------------+------------+----------------------+--------------------------------------+
	| 19h          | >i         | ( n -- I: n )        | Move value from data stack, to       |
	|              |            |                      |interrupt stack.                      |
	+--------------+------------+----------------------+--------------------------------------+
	| 1Ah          | i>         | ( I: n -- n )        | Move value from interrupt stack to   |
	|              |            |                      |data stack.                           |
	+--------------+------------+----------------------+--------------------------------------+
	| 1Bh          | =          | ( n2 n1 -- flag )    | Check for equality. If top two values|
	|              |            |                      |on stack are equal, they're replaced  |
	|              |            |                      |with a `true` flag.                   |
	+--------------+------------+----------------------+--------------------------------------+
	| 1Ch          | <          | ( n2 n1 -- flag )    | If value second on stack is less than|
	|              |            |                      |value at top of stack, they're        |
	|              |            |                      |replaced with a `true` flag.          |
	+--------------+------------+----------------------+--------------------------------------+
	| 1Dh          | >          | ( n2 n1 -- flag )    | If value second on stack is greater  |
	|              |            |                      |than value at top of stack, they're   |
	|              |            |                      |replaced with a `true` flag.          |
	+--------------+------------+----------------------+--------------------------------------+
	| 1Eh          | =0         | ( n -- flag )        | If value at top of stack is equal to |
	|              |            |                      |0, set `true` flag.                   |
	+--------------+------------+----------------------+--------------------------------------+
	| 1Fh          | true       | ( -- flag )          | Unconditional `true` set at top of   |
	|              |            |                      |stack.                                |
	+--------------+------------+----------------------+--------------------------------------+
	| 20h          | false      | ( -- flag )          | Unconditional `false`set at top      |
	|              |            |                      | of stack.                            |
	+--------------+------------+----------------------+--------------------------------------+
	| 21h          | between    | ( n3 n2 n1 -- flag ) | true if n2 <= n1 <= n3               |
	|              |            |                      |                                      |
	+--------------+------------+----------------------+--------------------------------------+
	| 22h          | within     | ( n3 n2 n1 -- flag ) | true if n2 <= n1 < n3                |
	|              |            |                      |                                      |
	+--------------+------------+----------------------+--------------------------------------+
	| 23h          | nop        | ( -- )               | No effect.                           |
	+--------------+------------+----------------------+--------------------------------------+
	| 24h          | swap       | ( n2 n1 -- n1 n2 )   | Swap the top two values on stack.    |
	+--------------+------------+----------------------+--------------------------------------+

# Interrupts #

Interrupts wake the processor from ''halt'', or temporarily suspend execution of user-code, to handle an interrupt. When
user-code is running on the processor when an interrupt is received, the currently executing instruction is allwoed to
complete; then the interrupt handler is called (12h) to deal with the interrupt's contents. Whilst an interrupt handler
is executing, interrupts are automatically disabled for the duration of an interrupt handler's execution, and re-enabled
immediately before the user's code regains control.

Interrupt handlers should follow the following guidelines:

* They **must not** have any net effect on the data stack after execution of the handler is complete. Code politely; do
  not leave values on the data stack that weren't there when you were called; likewise do not remove values from the
  data stack that you didn't place there.

* Any data to be passed into the handler from user-space code will be via the interrupt stack; likewise any data passed
  back to user-space code from the handler should be via the same. It is permissible for this data to be a pointer to
  data, if both handler and user-space code expect this.

* Keep interrupt-service routines short; as interrupts are disabled, you may miss events if your handler runs long.

## List of Interrupts ##

* 0h: RESET -- Cannot be handled; returns device to state resembling how it was initially 'powered on'. Main memory
  contents may or may not be cleared; contents of stacks discarded, and registers cleared.

* 1h: NOIMPL -- Effectively an exception for a not implemented instruction; to allow for software emulation of any
  instructions not built into the hardware.

* 2h: CLOCK -- Raised on expiry of a timer.

* 3h: I/O -- Raised when devices have data waiting, or have completed sending data. This may include co-processors
  returning resulting of processing requested by instructions.


# Memory #

In total, the processor has 2^16 words of memory 'attached'; some of this is reserved for various purposes, the
remainder is taken up by user-code and data.

## Memory Layout ##

    +-----------------+
    | BIOS map start  | 00
           ...
    | BIOS map end    | FF
    +-----------------+
    | JMP             | 100
    | over RSVMEM     | 101
    +-----------------+
    | Pointer to IVT  | 102
    +-----------------+
    | MMAP PORT A     | 103
    +-----------------+
    | CTRL PORT A     | 104
    +-----------------+
    | MMAP PORT B     | 105
    +-----------------+
    | CTRL PORT B     | 106
    +-----------------+
    | Code, user data | 107
            ...
    |                 | FFFF
    +-----------------+

This leaves 65,272 words of memory for user code, data, etc.

## Mapped BIOS ##

The BIOS is 256 words, mapped into memory starting from address 0, and containing enough code to perform any initial
setup and so forth. (e.g. Code to bootstrap enough that the remainder of the code can be loaded via an I/O port.)

# I/O #

I/O is via a pair of memory-mapped 16-bit (word-width) ports, which can be configured to buffer reads and writes, and
block if writes cannot be made. I/O is performed by loads and stores to the address of the port in memory; stores write
the word out to any simulated attached hardware. If the port is configured to be buffered and set to block, any writes
when the buffer is full will block; if the port is configured to be buffered, and non-blocking, any writes when the
buffer is full will cause an I/O interrupt that can be handled to store the data until the buffer empties enough to
accept new data. This will be signified by another I/O interrupt.

(TODO: Clean this up)

## Port Configuration ##

**TODO**
