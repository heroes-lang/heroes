---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **577 — literal arithmetic that cannot fit its position compiles and aborts at run time instead of being refused** | since defect 564's repair the operands of an operator take the type their position asks for, so `y: u8 = 200 + 100` and `repeat("-", 0 - 1)` build and abort 134 on the overflow at run time, as spec § 2 and § 7 say; refusing such an expression at compile time, every operand a literal, would be a new refusal (Rust's `arithmetic_overflow` is the precedent the lane names), so it is a sitting's | the checker's constant evaluation of literal operands, `selfhost/check/`; for a sitting · **class: improvement**

    **Origin:** found by lane b18-infer beside defect 564 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-infer/notes.txt`, ignored by git), filed by the coordinator at 04:34 on 2026-10-10.

    **Class: improvement**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a compile-time refusal nobody needs for robustness: the program aborts cleanly.
