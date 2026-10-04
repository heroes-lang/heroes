---
kind: defect
area: compiler
milestone: none
filed: 2026-10-02
commit: 901acaa4e8a9031d4be08a48e05df12d97acea7f
github: none
---

- [x] **193 — an arm whose pattern failed hides the mistakes in its body: on one line in both arms, and, joined below the failed line, under `--permissive` since panel 187's V1** | `k = match n` over `x | 2 => f(1 +)`: `expected_pattern` at the `x` and the `1 +` untold, in both arms, on the head's compiler and on `29425af6`; `x |` over `2 => f(1 +)`: the normal arm tells the `1 +` from the lines apart, and `check --permissive` told it, `expected_expression`, until `29425af6` and not since; the same over `1 | +`, `x ==`, `1 -> 2 |` and an arm one level deeper | `selfhost/grammar_expr.hero` (`arms_of`'s `.err` branch: the failed arm's line goes with `cursor.drop_rest_of_line`, its body with it) · panel 187's R4 · **class: adjacent**

    **Origin:** panel 187's compiler engineer, 2026-10-02, on its V1 probes `h01`, `h05`, `h14`, `h16`, `h17` and their one-line twins `h02`, `h06` (`scratchpad/187-compiler-engineer-work/probes2/`, 2026-10-02); filed by lane rec187 at the sitting's R4, reproduced on the head's compiler and on `29425af6` (2026-10-03, `scratchpad/lane-rec187/pass1/k-166-control.txt` and `v1-vs-k.txt`). No golden form runs `--permissive` (the sitting's R3), so the control arm's half cannot be pinned; the one-line half can.

    **Why it is a defect.** A mistake told only once another is fixed: design.md §4.17's measure counts an exchange more.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only once another is fixed; no false message, no wrong certain fix.

    Repaired at `901acaa4` (2026-10-04, lane b9-recovery), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `901acaa4`. An arm whose pattern was refused, anywhere a pattern stands, or whose `=>` was owed at junk, goes on at a `=>` its line holds outside brackets: its value is read for its own mistakes and the arm is kept out of the tree, as one opened by an operator already was. Its cases are `fixedbugs-193-a-failed-arms-body-is-read`, eleven shapes; no golden moved. The control arm's half, which no golden form could pin when the item was filed, is pinned since the gate by `permissive/fixedbugs-193-the-control-arm-reads-a-failed-arms-body` (`74863163`): the control arm reads the normal arm's messages less one, the thesis rule `continuation_outside_brackets`.

**Closed 2026-10-04** with batch 9 (lanes b9-notext, b9-emit, b9-harness, b9-recovery and b9-annot, merged into one round tree with the trunk at `f6a3122e`), its closing gate run on the round's head from `2c58b28e` to `662870e6`, no line of `selfhost/`, `runtime/` or the seed moving between, with the seed regenerated: 41,364,146 bytes, SHA-256 beginning `26ccaa9d96478a20`, its fixpoint by `cmp`; the compiler's own tests 1,190, all passed; the net's own tests 246, all passed; the full net, 27 suites, 5,268 passed and 0 failed, `fixes` read alone after `662870e6`, which stopped that suite copying the byte fixtures of defects 227 and 241 as text. The census, the trunk's compiler at `703af779` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 1,993, 34 moved, and `build --emit-c` over the 621 holding an `extern`, 3 files of C and 22 of messages moved, every one the batch's own. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 68 fewer messages in the normal arm and 71 in the control arm and none more; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked.
