# Tuli

Tuli is a machine code language based on base -2 integers and a few instructions.

## Registers

Tuli has 4 32-bit base -2 registers: referred to as t, u, l, i.

- **t** - Transfer register: for storing data to be transferred to other registers.
 
- **u** - Useless register: register with no particular purpose.
 
- **l** - Location register: stores a location in memory to read/write.
 
- **i** - Instruction register: stores the address of the next instruction to be run. Increments (arithmetically) after each instruction.

All registers are initialized to 0.

## Terminating the program

The instruction to terminate a program is `t`.
If a Tuli program does not reach a termination instruction, it will keep running forever. ***If you let the program run forever then the virtual machine may keep allocating memory forever.***

## Register storage instructions

Table of instructions for register storage:

| Store | in t | in u | in l | in i |
|:-----:|:----:|:----:|:----:|:----:|
| **t** |      | U    | L    | I    |
| **u** | u    |      | M    | J    |
| **l** | l    | m    |      | K    |
| **i** | i    | j    | k    |      |

## Indirect register storage instructions

Memory at an address in l can be stored in t, and it can also be written to by data from another register. Since memory locations are in bytes, each instruction has a 4 byte version and a 1 byte (least significant byte) version.

### 4 bytes

`\` - store memory at l into t

`<` - write t to memory at l

`=` - write u to memory at l

`[` - write i to memory at l

### 1 byte

` ` ` - store memory at l into t

`@` - write t to memory at l

`A` - write u to memory at l

`\_` - write i to memory at l

Code is stored in memory starting at address 0.

## Single register data operations

Single register data operations are all performed on t.

`c` (in**c**rement) - Perform a bitwise (not arithmetic) increment on t.

e.g. t:1011010 (70) -> t:1011011 (71), however t:1011011 (71) -> t:1011100 (74), rather than t:1011110 (72)

`r` (**r**otate) - Rotate the bits of t right.

e.g. t:1011010 (70) -> t:101101 (-35) -> t:10000000000000000000000000010110 (-2147483630)

`v` (in**v**ert) - Ignoring leading zeros, invert the bits of t.

e.g. t:1011010 (70) -> t:100101 (-27)

## Double register data operations

`f` (**f**ire instruction) - Perform a bitwise NOR between t and the bits of l after the bits of l are reversed, and store the result in t: **F**lip bits of l, **I**nverted O**R** into t, **E**nd.

e.g. t:1011010 (70), l:11101101 (-99) -> t:00010010111111111111111110100101 (229288549)

`s` (**s**wap instruction) - Swap the data in t and l.

e.g. t:1011010 (70), l:11101101 (-99) -> t:11101101 (-99)

## I/O operations

`o` (**o**utput) - Write the last 9 bits of t as an ASCII character to standard output.

e.g. t:1011010 (70) -> (outputs "F", i.e. char 70)

## No-ops

The standard no-op is `.`. Every other instruction not listed in this manual is a no-op: invalid instructions are skipped rather than leading to a crash.

