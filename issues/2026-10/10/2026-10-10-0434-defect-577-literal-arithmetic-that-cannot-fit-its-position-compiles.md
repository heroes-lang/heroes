---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: eb5fef1b1f055d1cb000ff8f82ba814b427f0ec3
github: none
---

- [ ] **577 — literal arithmetic that cannot fit its position compiles and aborts at run time instead of being refused** | since defect 564's repair the operands of an operator take the type their position asks for, so `y: u8 = 200 + 100` and `repeat("-", 0 - 1)` build and abort 134 on the overflow at run time, as spec § 2 and § 7 say; refusing such an expression at compile time, every operand a literal, would be a new refusal (Rust's `arithmetic_overflow` is the precedent the lane names), so it is a sitting's | the checker's constant evaluation of literal operands, `selfhost/check/`; for a sitting · **class: improvement**

    **Origin:** found by lane b18-infer beside defect 564 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-infer/notes.txt`, ignored by git), filed by the coordinator at 04:34 on 2026-10-10.

    **Class: improvement**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a compile-time refusal nobody needs for robustness: the program aborts cleanly.

    **Ruled 2026-10-10** by panel 206 (`docs/panel/206-a-constant-s-written-body-that-cannot-be-computed-is-a-compile-error-and-arithmetic-in-a-function-still-aborts-where-it-runs.md`, ratified by 10:22): a constant's written body whose step cannot be computed is a compile error, read or not (R1, C4b in spec § 4), which repairs this item's constant half; in a function body literal arithmetic aborts where it runs, as spec § 7 says (R2: a refusal there refuses code that never runs, and 0 of 6 blind readers wrote the mistake); `repeat`'s count computed negative aborts at its subtraction, panel 054's path (R3). This item closes with R1's landing.

    Repaired at `eb5fef1b`, 2026-10-10 (lane b18-close), with R3's dated paragraph at `1dd33aab`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. `check/const_steps.hero` walks every constant's written body, read or not, and computes each operator step at the width the checker recorded: one that overflows, divides or takes a remainder by zero, or shifts outside 0..63 is refused at its line, `int_out_of_range` or `division_by_zero`, the step quoted and its value said (`check/const_values.hero`). Every step is judged where it is written, array and map elements, holes, payloads, both branches, every arm, a loop's body and the right of `&&` included; a written constant is read through its own walk, a name bound by `=` holds its value and a cell its last on the straight path, forgotten by a branch or a loop that wrote it, and a group's constant gives none (R4, defect 579). Spec § 4 takes C4b, 7552 to 7565 cl100k, the merged document's one `--refresh` reading 9,944 real. Cases: `check/fixedbugs-577-…` three, one with its module in `check/fixedbugs577/`, `full/` one, `run/` three, every shape of the critic's; the census of 3,334 tracked files moves one, the sitting's own `docs/panel/206-briefs/blind/p3.hero`; `check selfhost/main.hero` 80.771e9 to 80.774e9 instructions. check 655, full 35, fixes 939, annotations 970, spec 23, grammar 9, unseen 3, special 10, records 28, run narrowed 5, emission 1148, the compiler's 1,564 tests and the net's 327, 0 failed.
