---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **580 — a constant's body whose step aborts only on a loop's later turn compiles and aborts at every read** | after panel 206's R1 landed in lane b18-close (`eb5fef1b`, on batch 18's round, 2026-10-10), a constant body `v: u8 @ 0` with `while i < 3` doing `v @ v + 100` compiles and aborts at every read, while spec § 4's C4b, landed with it, reads *a step of it that aborts is a compile error*; the walk forgets an `@` cell's value after a loop that writes it, so the step's operands are unknown; deciding it needs an exact evaluator of a constant's body with a step bound, the row panel 039's C1 refused as an optimisation with no measured need | `selfhost/check/const_steps.hero` (in batch 18's round, 2026-10-10) and spec § 4; for a sitting: an evaluator of a constant's body with a bound, or C4b's sentence narrowed · **class: blocking**

    **Origin:** found by lane b18-close in its landing of panel 206 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-close/notes.txt`, ignored by git), filed by the coordinator at 11:25 on 2026-10-10, the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): the spec false on a shape it names (CLAUDE.md § 12).
