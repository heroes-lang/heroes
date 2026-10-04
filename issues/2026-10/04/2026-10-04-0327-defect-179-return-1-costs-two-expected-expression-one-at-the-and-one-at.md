---
kind: defect
area: parse
milestone: none
filed: 2026-10-02
commit: ecf53e4141add8a26863f7e8fe34ff72a5a0411e
github: none
---

- [x] **179 — `@return 1` costs two `expected_expression`, one at the `@` and one at `return`** | `function f() -> i64` over `@return 1`: `check` exit 1, *expected an expression, found `@`* at 2:5 and *expected an expression, found `return`* at 2:6, neither naming the sigil | `selfhost/parse/at_prefix.hero` (a sigil before a name is one message since `4441148b`; before a keyword it is not) · **class: adjacent**

    **Origin:** lane recovery-b4's report (*`@return 1`: two messages*), queued under recovery-b5, 2026-10-02; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/at-return/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-a-sigil-before-return-is-told-twice.hero`, a known cost under the sitting's R1**, not repaired inline: the statement's start is `selfhost/grammar_expr.hero`'s, at its `DECIDED` ceiling of 1085 with no line of room, so a repair first moves code out of that knot. The item stays open.

    **2026-10-03, lane b8-recovery: a sigil before a word that begins a statement is read as before a name, told once at the `@`, naming the word, its deletion certain, and the line read as that statement** (`parse/at_prefix`, `grammar_expr.prefixed`, which took no new line; twelve shapes one message each, `@return 1 +` now telling its operand; the sigil's look ahead on `cursor.trial`, linear): repaired at `ecf53e41`, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

## The repair

Repaired at `ecf53e41` (with c311e1ca). A sigil before a word that begins a statement is read as before a name, told once at the `@`, naming the word, its deletion certain, and the line read as that statement (13 messages on the item's shapes against the trunk's 22). Its pin, `panel-187-a-sigil-before-return-is-told-twice`, moved from 2 messages to 1.

**Closed 2026-10-04** with batch 8 (lanes b8-ffi, b8-source, b8-recovery, b8-emit and b8-defects, merged into one round tree), its closing gate run on `921dc61e` with the seed regenerated: 40,628,892 bytes, SHA-256 beginning `2d55c5ff8309b812`, its fixpoint by `cmp`; the compiler's own tests 1,158, all passed; the net's own tests 210, all passed; the full net, 26 suites, 5,177 passed and 0 failed. The census at the batch's first gate, the trunk's compiler at `7d9f2e8f` against the round's over the tree's tracked files: `check --brief` over 1,954, 30 moved, and `build --emit-c` over the 1,228 holding an `extern`, 44 moved, every one the batch's own (its new refusals, its words, its `#line` before a fixed array field's assertion). Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 12 fewer messages and none more; 15,842 pairs, one told second now carried in the first message's fixes (defect 271).
