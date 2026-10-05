---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: 3f032bc7b4a35890406a2ac4a657f25a3ccee832
github: none
---

- [ ] **329 — a letter after a number's leading-zero digits, `00x7` or `0644u`, is told twice: `leading_zero` and `expected_end_of_line`** | `x = 00x7` and `x = 0644u`: each gets `leading_zero` at the number and `expected_end_of_line` at the letter, two messages for the one mistake; defect 324's goldens (`check/fixedbugs-324-*`) pin both as they stand | `selfhost/number.hero` and `selfhost/leading_zeros.hero` (where the leading zero's reading stops) · defect 324 · **class: adjacent**

    **Origin:** lane b10-cli, 2026-10-04, beside 324, on its compiler at `56cb4513` (its reply after 324).

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `3f032bc7`, 2026-10-05 (lane b11-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The shapes beside with this cause were repaired with it: other letters, a character beyond ASCII and a fraction after the digits, and the literal inside a call's or a list's brackets, where the second message was `expected_args_close` or `expected_separator`.
