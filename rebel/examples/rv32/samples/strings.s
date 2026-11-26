# strings.s: a line read from the standard input with the environment's
# `read`, then its length, the line reversed in place, the line in upper
# case, and how many of its bytes are vowels, each written with `write`.

        .equ    SIZE, 80

        .data
line:
        .space  SIZE
digits:
        .space  12
vowels:
        .asciz  "aeiouAEIOU"
length_label:
        .asciz  "length: "
reversed_label:
        .asciz  "reversed: "
upper_label:
        .asciz  "upper: "
vowels_label:
        .asciz  "vowels: "
newline:
        .byte   '\n'

        .text
        .globl  _start
_start:
        li      a0, 0                   # the standard input
        la      a1, line
        li      a2, SIZE
        li      a7, 63                  # read
        ecall
        mv      s0, a0                  # its length; below 0 on a failure
        bgez    s0, have_line
        li      s0, 0
have_line:
        beqz    s0, measured
        la      t0, line
        add     t1, t0, s0
        lbu     t2, -1(t1)
        li      t3, '\n'
        bne     t2, t3, measured
        addi    s0, s0, -1              # the line end is no part of it
measured:
        la      a0, length_label
        call    write_string
        mv      a0, s0
        call    write_number
        call    write_newline

        la      a0, line
        mv      a1, s0
        call    reverse
        la      a0, reversed_label
        call    write_string
        la      a0, line
        mv      a1, s0
        call    write_bytes
        call    write_newline

        la      a0, line
        mv      a1, s0
        call    reverse                 # back as it was
        la      a0, line
        mv      a1, s0
        call    to_upper
        la      a0, upper_label
        call    write_string
        la      a0, line
        mv      a1, s0
        call    write_bytes
        call    write_newline

        la      a0, line
        mv      a1, s0
        call    count_vowels
        mv      s1, a0
        la      a0, vowels_label
        call    write_string
        mv      a0, s1
        call    write_number
        call    write_newline
        li      a0, 0
        li      a7, 93                  # exit
        ecall

# reverse: reverses the a1 bytes from a0 in place, from both ends inward.
reverse:
        add     t1, a0, a1
        addi    t1, t1, -1              # the last byte
reverse_step:
        bgeu    a0, t1, reversed
        lbu     t2, 0(a0)
        lbu     t3, 0(t1)
        sb      t3, 0(a0)
        sb      t2, 0(t1)
        addi    a0, a0, 1
        addi    t1, t1, -1
        j       reverse_step
reversed:
        ret

# to_upper: the a1 bytes from a0, each from 'a' to 'z' made upper case.
to_upper:
        add     t1, a0, a1
upper_step:
        bgeu    a0, t1, uppered
        lbu     t2, 0(a0)
        addi    t3, t2, -'a'
        li      t4, 26
        bgeu    t3, t4, upper_next      # unsigned: below 'a' wraps past 25
        addi    t2, t2, 'A' - 'a'
        sb      t2, 0(a0)
upper_next:
        addi    a0, a0, 1
        j       upper_step
uppered:
        ret

# count_vowels: a0 = how many of the a1 bytes from a0 stand in `vowels`.
count_vowels:
        add     t1, a0, a1
        li      t5, 0
vowel_step:
        bgeu    a0, t1, counted
        lbu     t2, 0(a0)
        la      t3, vowels
vowel_try:
        lbu     t4, 0(t3)
        beqz    t4, vowel_next
        beq     t2, t4, vowel_found
        addi    t3, t3, 1
        j       vowel_try
vowel_found:
        addi    t5, t5, 1
vowel_next:
        addi    a0, a0, 1
        j       vowel_step
counted:
        mv      a0, t5
        ret

# write_string: writes the bytes from a0 up to its zero.
write_string:
        mv      t0, a0
string_end:
        lbu     t1, 0(t0)
        beqz    t1, string_ended
        addi    t0, t0, 1
        j       string_end
string_ended:
        sub     a1, t0, a0
        j       write_bytes

# write_bytes: writes the a1 bytes from a0.
write_bytes:
        mv      a2, a1
        mv      a1, a0
        li      a0, 1                   # the standard output
        li      a7, 64                  # write
        ecall
        ret

# write_newline: writes a line end.
write_newline:
        la      a0, newline
        li      a1, 1
        j       write_bytes

# write_number: writes a0, unsigned, in decimal.
write_number:
        la      t0, digits
        addi    t1, t0, 12
        li      t2, 10
number_digit:
        remu    t3, a0, t2
        addi    t3, t3, '0'
        addi    t1, t1, -1
        sb      t3, 0(t1)
        divu    a0, a0, t2
        bnez    a0, number_digit
        mv      a0, t1
        addi    a1, t0, 12
        sub     a1, a1, t1
        j       write_bytes
