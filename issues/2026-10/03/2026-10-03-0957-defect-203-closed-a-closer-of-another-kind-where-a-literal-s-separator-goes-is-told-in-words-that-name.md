---
kind: defect
area: parse
milestone: none
filed: 2026-09-30
commit: 3d704b03777c2359dc9cf746a33b9e9d4561ed25
github: none
---

# Defect 203 closed: a closer of another kind where a literal's separator goes is told in words that name neither the opener nor its closer

- [x] **203 — a closer of another kind where a literal's separator goes is told in words that name neither the opener nor its closer** | `x = [1, 2` over `print(x) )` in a function: `expected_separator` at the `)`, *expected `,` or a new line between one element and the next, found `)`*, no `[` and no `]` in it; the `)` deleted, a second run tells `unclosed_bracket` at the `[`; the same on one line, `[1, 2)` and `{1: 2]`, and for a `}` in a list | `selfhost/parse/list_line.hero:223-228` (`separator`'s message) · panel 187's Q2 and R6 · **class: adjacent** · **closed 2026-10-03**

    **Origin:** the audit's row 130-34a (`scratchpad/audit-130-133/cases/130-34a/`, 2026-09-30), the one row of item 130 open at panel 187; its R6 rewords the message to name the `[` left open, at its line and column, and both edits, the sitting's pin `tests/golden/check/panel-187-a-closer-of-another-kind-inside-a-list-is-one-message.hero` holding it meanwhile. Filed by lane rec187 before its repair, the shapes beside it read on the head's compiler (2026-10-03, `scratchpad/lane-rec187/pass1/k-r6.txt`).

    **Why it is a defect.** Both readings of the program stand (the grammar's, the line a third element and the `)` its `]`; and the `]` left out above, the `)` a stray), and the message serves the first alone: under the second the repair costs a run more (design.md §4.17's measure).

    **2026-10-03, lane rec187, panel 187's R6: the message names the opener
    still open, where it stands as a distance from the caret's line with its
    column in characters, and both edits** (the row and six shapes beside it
    with its cause, a second module among them): repaired at `97008231`,
    gated by its own cases; the rest is owed at the round's gate.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    **Closed 2026-10-03** at the round of 2026-10-03's gate `b43d224d` (the seed's fixpoint by `cmp`, the compiler's own tests 1,079, the net's own 200, the full net 4,901 passed and 0 failed over 26 suites); the recovery instrument's reading of that round's compiler against the trunk's over the frozen plan showed no mutant worse for the recovery (`scratchpad/inst-187/r4-differential.txt`, 2026-10-03). Its own cases re-run on the trunk at `02e507bc` at 09:56 by `date`: `check`, `annotations` and `fixes` 1 of 1 each, all passed. Not a C-boundary defect, so it closes at the Mac's gate (`.claude/rules/verification.md` § The batch).
