# sort.s: twelve signed words sorted in place by insertion, written with
# the environment's `write` before and after. The smallest and the largest
# words are among them, so every compare must read the sign.

        .data
numbers:
        .word   42, -7, 1000, 0, 3, -2147483648, 2147483647, 17, -1, 256, 3, 99
count:
        .word   12
buffer:
        .space  16

        .text
        .globl  _start
_start:
        call    print_all
        la      a0, numbers
        la      t0, count
        lw      a1, 0(t0)
        call    insertion_sort
        call    print_all
        li      a0, 0
        li      a7, 93                  # exit
        ecall

# insertion_sort: sorts the a1 words from a0, ascending, as signed numbers.
insertion_sort:
        li      t0, 1                   # i
outer:
        bge     t0, a1, sorted
        slli    t1, t0, 2
        add     t1, a0, t1              # &a[i]
        lw      t2, 0(t1)               # the key, a[i]
        mv      t3, t0                  # j
inner:
        beqz    t3, place
        lw      t4, -4(t1)              # a[j - 1]
        ble     t4, t2, place
        sw      t4, 0(t1)               # a[j] = a[j - 1]
        addi    t1, t1, -4
        addi    t3, t3, -1
        j       inner
place:
        sw      t2, 0(t1)
        addi    t0, t0, 1
        j       outer
sorted:
        ret

# print_all: writes the words on one line, a space between two.
print_all:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        sw      s1, 4(sp)
        la      s0, numbers
        la      t0, count
        lw      s1, 0(t0)
print_next:
        lw      a0, 0(s0)
        call    print_int
        addi    s0, s0, 4
        addi    s1, s1, -1
        beqz    s1, printed
        li      a0, ' '
        call    putc
        j       print_next
printed:
        li      a0, '\n'
        call    putc
        lw      s1, 4(sp)
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret

# print_int: writes a0, signed, in decimal, by one write. The digits fill
# `buffer` from its end; -2147483648 has no positive twin, so each digit is
# taken from the negative number itself.
print_int:
        la      t0, buffer
        addi    t1, t0, 16              # one past the last byte
        mv      t3, a0
        li      t4, 10
print_digit:
        rem     t5, t3, t4              # negative for a negative number
        bgez    t5, positive_digit
        neg     t5, t5
positive_digit:
        addi    t5, t5, '0'
        addi    t1, t1, -1
        sb      t5, 0(t1)
        div     t3, t3, t4
        bnez    t3, print_digit
        bgez    a0, print_out
        li      t2, '-'
        addi    t1, t1, -1
        sb      t2, 0(t1)
print_out:
        li      a0, 1                   # the standard output
        mv      a1, t1
        addi    a2, t0, 16
        sub     a2, a2, t1
        li      a7, 64                  # write
        ecall
        ret

# putc: writes a0's low byte.
putc:
        la      t0, buffer
        sb      a0, 0(t0)
        li      a0, 1
        mv      a1, t0
        li      a2, 1
        li      a7, 64                  # write
        ecall
        ret
