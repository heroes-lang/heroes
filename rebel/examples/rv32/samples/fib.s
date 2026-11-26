# fib.s: Fibonacci's numbers from 0 to 15, each by the doubly recursive
# definition, every call for 2 or more keeping a frame on the stack: the
# return address and the two registers it must give back.

        .data
buffer:
        .space  12

        .text
        .globl  _start
_start:
        li      s0, 0                   # n
loop:
        mv      a0, s0
        call    fib
        li      a1, ' '
        li      t0, 15
        bne     s0, t0, separated
        li      a1, '\n'
separated:
        call    print_number
        addi    s0, s0, 1
        li      t0, 16
        blt     s0, t0, loop
        li      a0, 0
        li      a7, 93                  # exit
        ecall

# fib: a0 = fib(a0), as fib(n - 1) + fib(n - 2); fib(0) = 0, fib(1) = 1.
fib:
        li      t0, 2
        blt     a0, t0, fib_done        # n < 2: fib(n) is n
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        sw      s1, 4(sp)
        mv      s0, a0                  # n
        addi    a0, a0, -1
        call    fib
        mv      s1, a0                  # fib(n - 1)
        addi    a0, s0, -2
        call    fib
        add     a0, a0, s1
        lw      s1, 4(sp)
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
fib_done:
        ret

# print_number: writes a0, unsigned, in decimal, then the byte a1, by one
# write; the digits fill `buffer` from its end.
print_number:
        la      t0, buffer
        addi    t1, t0, 11              # the last byte
        sb      a1, 0(t1)
        li      t2, 10
next_digit:
        remu    t3, a0, t2
        addi    t3, t3, '0'
        addi    t1, t1, -1
        sb      t3, 0(t1)
        divu    a0, a0, t2
        bnez    a0, next_digit
        li      a0, 1                   # the standard output
        mv      a1, t1
        addi    a2, t0, 12
        sub     a2, a2, t1
        li      a7, 64                  # write
        ecall
        ret
