# crash.s: a store to the word at the stack pointer before any push, with no
# trap handler. The stack pointer starts one past the RAM's last byte, so
# the store reaches no memory: the machine stops with a store access fault
# and says where, and the program's `exit` is never reached.

        .equ    LENGTH, 34

        .data
message:
        .ascii  "storing past the top of the stack\n"

        .text
        .globl  _start
_start:
        li      a0, 1                   # the standard output
        la      a1, message
        li      a2, LENGTH
        li      a7, 64                  # write
        ecall
        sw      zero, 0(sp)             # sp is one past the RAM's end
        li      a0, 0
        li      a7, 93                  # exit, never reached
        ecall
