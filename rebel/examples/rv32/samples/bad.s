# bad.s: a source with mistakes, which the assembler tells together, each
# with its line, and makes no image of.

        .text
_start:
        addd    a0, a1, a2              # no such instruction
        li      a0                      # li takes a register and a value
        lw      a0, 8(q9)               # no register is named q9
_start:                                 # a label defined twice
        .quad   5                       # no such directive here
        mv      a0, a1, a2              # mv takes two registers
        addi    a0, a0, 0x1_0000_0000   # a value past 32 bits
