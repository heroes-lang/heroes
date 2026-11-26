# primes.s: the primes below 100, by trial division, written on one line
# to the UART through its registers: each byte waits until the line status
# register says the transmitter is empty.

        .equ    UART, 0x10000000        # the 16550's registers
        .equ    LSR, 5                  # its line status register
        .equ    LIMIT, 100

        .text
        .globl  _start
_start:
        li      s0, 2                   # n, the number tried
next:
        li      t0, 2                   # d, the divisor tried
try:
        mul     t1, t0, t0
        bgt     t1, s0, prime           # d * d > n: no divisor is left
        rem     t2, s0, t0
        beqz    t2, composite
        addi    t0, t0, 1
        j       try
prime:
        li      t3, 2
        beq     s0, t3, first           # no space before the first
        li      a0, ' '
        call    putc
first:
        mv      a0, s0
        call    print_number
composite:
        addi    s0, s0, 1
        li      t3, LIMIT
        blt     s0, t3, next
        li      a0, '\n'
        call    putc
        li      a0, 0
        li      a7, 93                  # exit
        ecall

# putc: writes a0's low byte to the UART.
putc:
        li      t5, UART
putc_wait:
        lbu     t6, LSR(t5)
        andi    t6, t6, 0x20
        beqz    t6, putc_wait
        sb      a0, 0(t5)
        ret

# print_number: writes a0, unsigned, in decimal. Its digits wait on the
# stack, the last one found first.
print_number:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        sw      s1, 4(sp)
        mv      s0, a0
        li      s1, 0                   # how many digits are on the stack
digit:
        li      t0, 10
        remu    t1, s0, t0
        addi    t1, t1, '0'
        addi    sp, sp, -4
        sw      t1, 0(sp)
        addi    s1, s1, 1
        divu    s0, s0, t0
        bnez    s0, digit
write_digit:
        lw      a0, 0(sp)
        addi    sp, sp, 4
        call    putc
        addi    s1, s1, -1
        bnez    s1, write_digit
        lw      s1, 4(sp)
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret
