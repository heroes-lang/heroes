---
kind: defect
area: compiler
milestone: none
filed: 2026-10-02
commit: 883a3fb9c67919de906f33b4ad298570da20e5e6
github: none
---

- [x] **182 — an operator alone on a line deeper than a `match`'s arms costs `continuation_outside_brackets` and `unexpected_block`** | `k = match n` over `1 => "one"`, then a line holding only `+` (or `-`) one level deeper, then `_ => "many"`: two messages on line 5; the `certain` deletion of the operator, applied, checks clean | `selfhost/open_line.hero` · `selfhost/sign_above.hero` (the deletion, defect 165's `8cb4ba6c`) · the orphan block's `unexpected_block` · **class: adjacent**

    **Origin:** lane h158's pass for defects 165 and 166, 2026-10-02 (`scratchpad/lane-h158/d166/q1_plus_deeper.hero`, `q2_minus_deeper_wild.hero`, 2026-10-02); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/operator-deeper-line/`), where the one fix was a guess, and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), where it is the `certain` deletion 165's repair brought; the second message stands.

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-an-operator-alone-deeper-than-the-arms-costs-two.hero`, a known cost under the sitting's R1**, not repaired inline: the deeper margin is laid out by the lexer before `selfhost/sign_above.hero` leaves the operator out of the stream, in `selfhost/open_line.hero`, two lines under its ceiling, and only the lexer's indent stack can take the block back. The item stays open.

    **2026-10-03, lane b8-recovery: where the deletion is the one repair, the operator's line goes with the margin it laid out, the indent or dedents and the reports that margin said, and the arm's line is laid out on its own margin, as the deletion writes the text; that refusal records no break** (`selfhost/sign_above.hero`, `selfhost/open_line.hero`; one message for each of eight shapes off the arms' margin, an over-indented arm below told at its own line; the pin moved with a dated line): repaired at `883a3fb9`, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

## The repair

Repaired at `883a3fb9` (with 7f87dac6). Where the deletion is the one repair, the stray operator's line goes with the margin it laid out and the arm's line is laid out on its own (`selfhost/sign_above.hero`): eight shapes one message each, against the trunk's 16. Its pin, `panel-187-an-operator-alone-deeper-than-the-arms-costs-two`, moved from 4 messages to 2. A `-` alone deeper than an integer arm, beside it and not its cause, is filed apart as defect 268.

**Closed 2026-10-04** with batch 8 (lanes b8-ffi, b8-source, b8-recovery, b8-emit and b8-defects, merged into one round tree), its closing gate run on `921dc61e` with the seed regenerated: 40,628,892 bytes, SHA-256 beginning `2d55c5ff8309b812`, its fixpoint by `cmp`; the compiler's own tests 1,158, all passed; the net's own tests 210, all passed; the full net, 26 suites, 5,177 passed and 0 failed. The census at the batch's first gate, the trunk's compiler at `7d9f2e8f` against the round's over the tree's tracked files: `check --brief` over 1,954, 30 moved, and `build --emit-c` over the 1,228 holding an `extern`, 44 moved, every one the batch's own (its new refusals, its words, its `#line` before a fixed array field's assertion). Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 12 fewer messages and none more; 15,842 pairs, one told second now carried in the first message's fixes (defect 271).
