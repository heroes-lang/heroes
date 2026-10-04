---
kind: defect
area: compiler
milestone: none
filed: 2026-10-02
commit: bd43a92c4198954fc39e248d0891455d426096cb
github: none
---

- [x] **184 — `check` takes time quadratic in the `for` heads it refuses: 3,000 `for n > 0` lines cost 16 s** | a function of N loops `for n > 0` over `n @ n - 1`, `check --brief`: 0.44 s of user time at 500, 1.74 at 1,000, 3.87 at 1,500, 15.92 at 3,000, `real` within 0.16 s of `user`; 3,000 `while n > 0,` cost 0.15 s and 3,000 `n @ 0X1` 0.06 s | `selfhost/grammar_expr.hero:899` (`for_stmt`) · `selfhost/parse/loop_habit.hero` (`refuse`), the cause unrun · **class: adjacent**

    **Origin:** the coordinator's file-queue agent, 2026-10-02, measuring lane arm's item on field-place pushes, on `62d65e48` (2026-10-02, `scratchpad/file-queue/for-habit-quadratic/`), the machine at load 2 to 5; re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), the times above being that run's, on a busy machine and in the same order as the first. Two candidates were measured out over the same 3,000: `loop_habit.hero:62`'s append made in place, and `for_stmt`'s trial parse removed.

    **Why it is a defect.** Defect 146's class: a file of N mistakes costs N² work.

    **2026-10-03, lane b8-recovery: below a head that refused something only an opener is paired with a closer, and the `for` head's report is said in place, its condition read on a copy that holds none of the reports** (`parse/swallowed.spilled`, `cursor.trial`, `parse/loop_habit.refuse`; 3,000 heads 250.1 to 2.08 billion instructions retired): repaired at `bd43a92c`, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    **Corrected 2026-10-04, lane b8-recovery**: `bd43a92c`'s body says `while n >`, `if n >`, `for` alone, `for (n > 0)` and no body went from about 15x to 3.5x for 4 times the heads; measured, from 500 heads to 2,000 in instructions retired, the `for` shapes went to 3.48x to 3.62x and `while n >` and `if n >` from 15.04x to 5.47x and 5.46x, the rest of theirs `line_end.said_here`'s walk over every report said, at each refused condition, a cause apart, reported to the coordinator.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): real, found beside the work, no wrong value and no crash.

## The repair

Repaired at `bd43a92c` (with 72e5303c; corrected underneath at 3040cfb3). Below a head that refused something only an opener is paired with a closer, and the `for` head's report is said in place, its condition read on a copy that holds none of the reports: 3,000 refused heads cost 2.08 billion instructions where the trunk's cost 250.1 billion, and the growth from 500 to 3,000 heads is 5.27 times against 34.3. Timed on a still machine at the gate (load about 2, `real` equal to `user` plus `sys`): `check --brief` over the 3,000 heads, 15.24 s on the trunk's compiler, 0.16 s on the round's, the same seven messages. The `while` and `if` heads' and an unclosed `for`'s growth, beside it with other causes, are filed apart as defects 265 and 266.

**Closed 2026-10-04** with batch 8 (lanes b8-ffi, b8-source, b8-recovery, b8-emit and b8-defects, merged into one round tree), its closing gate run on `921dc61e` with the seed regenerated: 40,628,892 bytes, SHA-256 beginning `2d55c5ff8309b812`, its fixpoint by `cmp`; the compiler's own tests 1,158, all passed; the net's own tests 210, all passed; the full net, 26 suites, 5,177 passed and 0 failed. The census at the batch's first gate, the trunk's compiler at `7d9f2e8f` against the round's over the tree's tracked files: `check --brief` over 1,954, 30 moved, and `build --emit-c` over the 1,228 holding an `extern`, 44 moved, every one the batch's own (its new refusals, its words, its `#line` before a fixed array field's assertion). Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 12 fewer messages and none more; 15,842 pairs, one told second now carried in the first message's fixes (defect 271).
