- [ ] **204 — inside a list whose `[` the lexer paired with a closer of another kind further down, a binding is read as an element and told without the `[`, and the stray closer waits for the `]`** | `x = [1, 2` over `print(x)` over `y = 3 )`: one message, `expected_separator` at the `=` two lines below the `[`, *found `=`*, naming no `[`; with the `]` written, the `)` is told, `expected_end_of_line`, on a second run | `selfhost/parse/unclosed.hero` (panel 183's R1, a binding below a `[` ends its reach only where the lexer named the `[` never closed) · `selfhost/closers.hero` (the closer of another kind paired with the `[`) · **class: adjacent**

    **Origin:** lane rec187's first pass beside defect 203, 2026-10-03, on the head's compiler and on the lane's (`scratchpad/lane-rec187/pass1/r6/r16_closer_two_below.hero` and `r16b_bracket_written.hero`, 2026-10-03). Another cause than 203's: the found token is no closer, and the reach rule is not asked.

    **Why it is a defect.** The `]` left out is told nowhere and the stray `)` only on a second run (design.md §4.17's measure).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only once another is fixed, class (b).

    Repaired at `54d8e383` (2026-10-04, lane b9-recovery), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
