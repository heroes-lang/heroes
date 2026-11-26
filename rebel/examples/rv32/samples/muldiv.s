# muldiv.s: the edges of RV32IM's arithmetic, each written as its name and
# the word it gives in hexadecimal: the high halves of products, division
# by zero and the one signed overflow, which the M extension defines rather
# than traps, wrap-around, shift counts past 31, and the signed and
# unsigned compares of one word.

        .data
hex:
        .ascii  "0x00000000\n"
mul_name:       .asciz  "mul    -1 * -1           "
mulh_name:      .asciz  "mulh   -1 * -1           "
mulhu_name:     .asciz  "mulhu  ffffffff^2        "
mulhsu_name:    .asciz  "mulhsu -1 * ffffffff     "
mulh2_name:     .asciz  "mulh   80000000^2        "
div0_name:      .asciz  "div    7 / 0             "
divu0_name:     .asciz  "divu   7 / 0             "
rem0_name:      .asciz  "rem    7 % 0             "
divo_name:      .asciz  "div    80000000 / -1     "
remo_name:      .asciz  "rem    80000000 % -1     "
divn_name:      .asciz  "div    -7 / 2            "
remn_name:      .asciz  "rem    -7 % 2            "
add_name:       .asciz  "add    7fffffff + 1      "
sll_name:       .asciz  "sll    1 << 33           "
sra_name:       .asciz  "sra    80000000 >> 31    "
srl_name:       .asciz  "srl    80000000 >> 31    "
slt_name:       .asciz  "slt    -1 < 0            "
sltu_name:      .asciz  "sltu   ffffffff < 0      "

        .text
        .globl  _start
_start:
        li      s0, -1
        li      s1, 0x80000000
        li      s2, 7
        li      s3, -7
        li      s4, 2

        mul     a1, s0, s0
        la      a0, mul_name
        call    show
        mulh    a1, s0, s0
        la      a0, mulh_name
        call    show
        mulhu   a1, s0, s0
        la      a0, mulhu_name
        call    show
        mulhsu  a1, s0, s0
        la      a0, mulhsu_name
        call    show
        mulh    a1, s1, s1
        la      a0, mulh2_name
        call    show

        div     a1, s2, zero
        la      a0, div0_name
        call    show
        divu    a1, s2, zero
        la      a0, divu0_name
        call    show
        rem     a1, s2, zero
        la      a0, rem0_name
        call    show
        div     a1, s1, s0
        la      a0, divo_name
        call    show
        rem     a1, s1, s0
        la      a0, remo_name
        call    show
        div     a1, s3, s4
        la      a0, divn_name
        call    show
        rem     a1, s3, s4
        la      a0, remn_name
        call    show

        li      t0, 0x7fffffff
        addi    a1, t0, 1
        la      a0, add_name
        call    show
        li      t0, 1
        li      t1, 33
        sll     a1, t0, t1
        la      a0, sll_name
        call    show
        srai    a1, s1, 31
        la      a0, sra_name
        call    show
        srli    a1, s1, 31
        la      a0, srl_name
        call    show
        slt     a1, s0, zero
        la      a0, slt_name
        call    show
        sltu    a1, s0, zero
        la      a0, sltu_name
        call    show

        li      a0, 0
        li      a7, 93                  # exit
        ecall

# show: writes the name a0 points at, then a1 as 0x and eight digits.
show:
        mv      t5, a0
show_end:
        lbu     t1, 0(t5)
        beqz    t1, show_name
        addi    t5, t5, 1
        j       show_end
show_name:
        mv      t6, a1
        mv      a1, a0
        sub     a2, t5, a0
        li      a0, 1                   # the standard output
        li      a7, 64                  # write
        ecall
        la      t0, hex
        addi    t2, t0, 9               # the last digit
        li      t1, 8
show_digit:
        andi    t3, t6, 15
        li      t4, 10
        blt     t3, t4, decimal
        addi    t3, t3, 'a' - 10
        j       stored
decimal:
        addi    t3, t3, '0'
stored:
        sb      t3, 0(t2)
        srli    t6, t6, 4
        addi    t2, t2, -1
        addi    t1, t1, -1
        bnez    t1, show_digit
        li      a0, 1
        mv      a1, t0
        li      a2, 11
        li      a7, 64                  # write
        ecall
        ret
