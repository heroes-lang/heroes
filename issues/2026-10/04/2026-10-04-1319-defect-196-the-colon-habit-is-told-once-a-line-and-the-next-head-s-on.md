---
kind: defect
area: parse
milestone: none
filed: 2026-10-02
commit: e5a0db13bb354539b524eac15aac62abe4d442a2
github: none
---

- [x] **196 — the colon habit is told once a line, and the next head's `:` on the same line is named a missing body** | `if n > 0: if n > 1: print(1)`: `trailing_colon` at the first `:`, whose `certain` fix rewrites the whole line and checks clean, then `missing_body` at the second, *found `:`*; three heads on one line cost the same two | `selfhost/parse/colon_habit.hero:98` · `selfhost/parse/opening.hero:274` (`absent`) · pinned by `tests/golden/check/panel-187-the-colon-habit-is-told-once-a-line.hero` and its `.fixed` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause A3 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's rows 131-56a and 131-56b; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a second message for one mistake, class (a).

    Repaired at `e5a0db13` (2026-10-04, lane b9-recovery), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `e5a0db13`. A colon habit nested on one line is one report, one certain fix writing each inner suite one level deeper, and the inner body's own mistakes are told; `check --apply` writes the nested program, which checks clean. Its cases are `fixedbugs-196-a-suite-inside-a-suite-is-one-colon-habit`, nine shapes with its `.fixed`, and `fixedbugs-196-a-nested-suites-own-mistakes-are-told` with its `.applied`. Its pin, `panel-187-the-colon-habit-is-told-once-a-line`, moved from 4 messages to 2, the `missing_body` at each inner `:` gone, read at the gate.

**Closed 2026-10-04** with batch 9 (lanes b9-notext, b9-emit, b9-harness, b9-recovery and b9-annot, merged into one round tree with the trunk at `f6a3122e`), its closing gate run on the round's head from `2c58b28e` to `662870e6`, no line of `selfhost/`, `runtime/` or the seed moving between, with the seed regenerated: 41,364,146 bytes, SHA-256 beginning `26ccaa9d96478a20`, its fixpoint by `cmp`; the compiler's own tests 1,190, all passed; the net's own tests 246, all passed; the full net, 27 suites, 5,268 passed and 0 failed, `fixes` read alone after `662870e6`, which stopped that suite copying the byte fixtures of defects 227 and 241 as text. The census, the trunk's compiler at `703af779` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 1,993, 34 moved, and `build --emit-c` over the 621 holding an `extern`, 3 files of C and 22 of messages moved, every one the batch's own. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 68 fewer messages in the normal arm and 71 in the control arm and none more; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked.
