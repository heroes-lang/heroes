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

    **Ruled 2026-10-10** by panel 207 (`docs/panel/207-a-constant-s-body-refuses-at-least-the-integer-steps-whose-operands-are-known-and-the-spec-says-exactly-that.md`, ratified at 12:33): C4b states the walk's class exactly, open above (R1), the diagnostic note agrees (R2), and what the class leaves (an index, a string index, an `assert`, a nan, a branch the walk cannot decide, a group's constant, a hang) aborts where it runs with a `run` witness per kind (R3); an exact evaluator with a bound is refused by design.md's *never a quota*. This item, whose title names one of at least six kinds, closes with R1 to R3's landing.
