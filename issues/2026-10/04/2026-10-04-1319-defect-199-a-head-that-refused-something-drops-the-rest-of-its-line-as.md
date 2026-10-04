---
kind: defect
area: parse
milestone: none
filed: 2026-10-02
commit: b510f9025c75289d4bd91f8402be8713ed0e1267
github: none
---

- [x] **199 — a head that refused something drops the rest of its line as debris, a stray closer in it with it** | `if f(1 +) )` over its body: `expected_expression` at the call's `)`, and the stray `)` after it told only once the operand is written | `selfhost/parse/opening.hero:147` (`drop_rest_of_line` after a failed head) · pinned by `tests/golden/check/panel-187-a-failed-heads-rest-is-dropped-as-debris.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause B2 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on lane recovery-b8's shape `g4/ti`, one of the six beside item 130; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a mistake told only once another is fixed, class (b).

    Repaired at `b510f902` (2026-10-04, lane b9-recovery), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `b510f902`. A failed head's stray closer is told in the same run: a failed statement and a failed head share one drop, `brace_habit.drop_told`, which tells the first closer one too many on the line. Its cases are `fixedbugs-199-a-failed-heads-stray-closer-is-told`, eleven shapes. Its pin, `panel-187-a-failed-heads-rest-is-dropped-as-debris`, moved from one message to two, the stray `)` told in the same run, read at the gate.

**Closed 2026-10-04** with batch 9 (lanes b9-notext, b9-emit, b9-harness, b9-recovery and b9-annot, merged into one round tree with the trunk at `f6a3122e`), its closing gate run on the round's head from `2c58b28e` to `662870e6`, no line of `selfhost/`, `runtime/` or the seed moving between, with the seed regenerated: 41,364,146 bytes, SHA-256 beginning `26ccaa9d96478a20`, its fixpoint by `cmp`; the compiler's own tests 1,190, all passed; the net's own tests 246, all passed; the full net, 27 suites, 5,268 passed and 0 failed, `fixes` read alone after `662870e6`, which stopped that suite copying the byte fixtures of defects 227 and 241 as text. The census, the trunk's compiler at `703af779` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 1,993, 34 moved, and `build --emit-c` over the 621 holding an `extern`, 3 files of C and 22 of messages moved, every one the batch's own. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 68 fewer messages in the normal arm and 71 in the control arm and none more; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked.
