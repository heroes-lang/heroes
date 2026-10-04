---
kind: defect
area: compiler
milestone: none
filed: 2026-10-02
commit: e0ea7c9dc747b2bcf19ea94227a9ae6fd424fa4e
github: none
---

- [x] **180 — a range written `1..2` is told twice as a field access** | `x = 1..2`: `expected_field_name` at 3:11, *found `.`*, and again at 3:12, *found a number (`2`)*; `x = 0x1..5` the same at 3:13 and 3:14 | `selfhost/grammar_expr.hero:343` · **class: adjacent**

    **Origin:** lane arm's first pass for defect 154, 2026-10-02 (`scratchpad/lane-arm/pass1/n154/e20.hero`, 2026-10-02), queued as *a range habit, recovery's file*; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/range-dots-twice/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The recovery instrument counts its `range-dots` operator at 12 EXTRA.

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-a-range-written-with-two-dots-is-told-twice.hero`, a known cost under the sitting's R1**, not repaired inline: its site is `selfhost/grammar_expr.hero`'s `after_dot`, at its `DECIDED` ceiling of 1085 with no line of room, and the range's right operand can be read only inside that knot, so no module of `parse/` can carry the repair; a `for i in 0..10` head costs the same two. The item stays open.

    **2026-10-03, lane b8-recovery: the range is told once, at its second dot, its code and words kept and words added naming `range(from: a, to: b)`, the rest of its operator passed and its right operand read** (`grammar_expr.past_a_range`, from `after_dot` and from `case_expr` for `..n`; `selfhost/dot_words.hero`, text-only, outside `parse/`; the knot's room made by moving `ends_the_expression` and `is_place` to `selfhost/expr_kinds.hero` at `24b3f85d`; sixteen shapes one message each): repaired at `e0ea7c9d`, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

## The repair

Repaired at `e0ea7c9d` (with faa4cc5c). A range written `..` (and `...`, `..=`, `..<`, `..n`) is told once, at its second dot, its code and words kept and words added naming `range(from: a, to: b)` (17 messages against the trunk's 30). Its pin, `panel-187-a-range-written-with-two-dots-is-told-twice`, moved from 6 messages to 3; R2 read 12 `range-dots` mutants go from two messages to one in each arm, and 32 singles' words gain the sentence.

**Closed 2026-10-04** with batch 8 (lanes b8-ffi, b8-source, b8-recovery, b8-emit and b8-defects, merged into one round tree), its closing gate run on `921dc61e` with the seed regenerated: 40,628,892 bytes, SHA-256 beginning `2d55c5ff8309b812`, its fixpoint by `cmp`; the compiler's own tests 1,158, all passed; the net's own tests 210, all passed; the full net, 26 suites, 5,177 passed and 0 failed. The census at the batch's first gate, the trunk's compiler at `7d9f2e8f` against the round's over the tree's tracked files: `check --brief` over 1,954, 30 moved, and `build --emit-c` over the 1,228 holding an `extern`, 44 moved, every one the batch's own (its new refusals, its words, its `#line` before a fixed array field's assertion). Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 12 fewer messages and none more; 15,842 pairs, one told second now carried in the first message's fixes (defect 271).
