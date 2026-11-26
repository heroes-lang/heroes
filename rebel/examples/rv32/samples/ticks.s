# ticks.s: a trap handler and the timer. Five timer interrupts, due every
# 1000 cycles, the hart asleep in `wfi` between them; then
# an illegal instruction and an `ebreak`, which the same handler takes and
# steps over. The handler writes through the UART's registers: with a
# handler installed `ecall` is a trap too, so only once the handler is
# removed does the program's last `ecall` reach the environment.

        .equ    UART, 0x10000000
        .equ    LSR, 5
        .equ    CLINT, 0x02000000
        .equ    MTIMECMP, 0x4000
        .equ    MTIME, 0xbff8
        .equ    PERIOD, 1000

        .data
ticks:
        .word   0
saved:
        .space  32
tick_text:
        .asciz  "tick "
cycle_text:
        .asciz  " at cycle "
trap_text:
        .asciz  "trap: mcause "
skipped_text:
        .asciz  ", stepped over\n"

        .text
        .globl  _start
_start:
        la      t0, handler
        csrw    mtvec, t0
        call    arm
        li      t0, 0x80
        csrw    mie, t0                 # the timer's interrupt enabled
        csrsi   mstatus, 8              # and interrupts taken
wait:
        wfi
        la      t0, ticks
        lw      t1, 0(t0)
        li      t2, 5
        blt     t1, t2, wait
        csrci   mstatus, 8              # interrupts no longer taken
        .word   0                       # an illegal instruction
        ebreak
        csrw    mtvec, zero             # no handler: ecall is the environment's
        li      a0, 0
        li      a7, 93                  # exit
        ecall

# arm: the timer's interrupt PERIOD cycles from now. mtimecmp's high word
# is written first, so the compare never stands below mtime between the
# two stores.
arm:
        li      t0, CLINT
        li      t1, MTIME
        add     t1, t0, t1
        lw      t2, 0(t1)               # mtime's low word
        addi    t2, t2, PERIOD
        li      t1, MTIMECMP
        add     t1, t0, t1
        sw      zero, 4(t1)
        sw      t2, 0(t1)
        ret

# again: the next interrupt PERIOD cycles after the last one was due, so
# the ticks keep their pace whatever the handler costs.
again:
        li      t0, CLINT
        li      t1, MTIMECMP
        add     t1, t0, t1
        lw      t2, 0(t1)
        addi    t2, t2, PERIOD
        sw      t2, 0(t1)
        ret

# handler: every trap of the program. It keeps the registers it uses, so
# the code it interrupted goes on as if nothing ran. An interrupt counts a
# tick, says so with the cycle it came at, and asks for the next; an exception
# says its cause and moves mepc past the instruction that caused it.
        .balign 4
handler:
        csrw    mscratch, t6            # t6 kept where no register is needed
        la      t6, saved
        sw      ra, 0(t6)
        sw      t0, 4(t6)
        sw      t1, 8(t6)
        sw      t2, 12(t6)
        sw      t3, 16(t6)
        sw      t4, 20(t6)
        sw      t5, 24(t6)
        sw      a0, 28(t6)
        csrr    t0, mcause
        bltz    t0, interrupt           # the top bit: an interrupt
        la      a0, trap_text
        call    write_string
        csrr    a0, mcause
        call    write_number
        la      a0, skipped_text
        call    write_string
        csrr    t0, mepc
        addi    t0, t0, 4
        csrw    mepc, t0
        j       handled
interrupt:
        la      t0, ticks
        lw      t1, 0(t0)
        addi    t1, t1, 1
        sw      t1, 0(t0)
        la      a0, tick_text
        call    write_string
        la      t0, ticks
        lw      a0, 0(t0)
        call    write_number
        la      a0, cycle_text
        call    write_string
        rdcycle a0
        call    write_number
        li      a0, '\n'
        call    putc
        call    again
handled:
        la      t6, saved
        lw      ra, 0(t6)
        lw      t0, 4(t6)
        lw      t1, 8(t6)
        lw      t2, 12(t6)
        lw      t3, 16(t6)
        lw      t4, 20(t6)
        lw      t5, 24(t6)
        lw      a0, 28(t6)
        csrr    t6, mscratch
        mret

# putc: writes a0's low byte to the UART, waiting until it takes one.
putc:
        li      t5, UART
putc_wait:
        lbu     t4, LSR(t5)
        andi    t4, t4, 0x20
        beqz    t4, putc_wait
        sb      a0, 0(t5)
        ret

# write_string: writes the bytes from a0 up to its zero.
write_string:
        mv      t3, a0
        mv      t2, ra
string_next:
        lbu     a0, 0(t3)
        beqz    a0, string_done
        call    putc
        addi    t3, t3, 1
        j       string_next
string_done:
        mv      ra, t2
        ret

# write_number: writes a0, unsigned, in decimal, its digits found on the
# stack first.
write_number:
        mv      t2, ra
        li      t3, 0
        li      t1, 10
number_digit:
        remu    t0, a0, t1
        addi    t0, t0, '0'
        addi    sp, sp, -4
        sw      t0, 0(sp)
        addi    t3, t3, 1
        divu    a0, a0, t1
        bnez    a0, number_digit
number_write:
        lw      a0, 0(sp)
        addi    sp, sp, 4
        call    putc
        addi    t3, t3, -1
        bnez    t3, number_write
        mv      ra, t2
        ret
