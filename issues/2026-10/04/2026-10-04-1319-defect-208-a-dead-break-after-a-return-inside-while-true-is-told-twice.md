---
kind: defect
area: check
milestone: none
filed: 2026-10-03
commit: 4d8259cc622f053083c69d8c33110bdc6f1f47fb
github: none
---

- [x] **208 — a dead `break` after a `return` inside `while true` is told twice, `unreachable_statement` and `missing_return`** | `while true` over `if m > 3`, `return m`, `break`, in a `function f(n: i64) -> i64`: `unreachable_statement` at the `break` and `missing_return` on `f`, both gone once the `break` is deleted | `selfhost/check/flow.hero` (panel 184's R4: a `while true` with a `break` of its own does not end a path, read by syntax) · **class: adjacent**

    **Origin:** lane flow4's report, 2026-10-03 (`scratchpad/lane-flow4/w/w16-dead-break-under-return.hero`, 2026-10-03); measured by the coordinator on the trunk at `e5893696` (`scratchpad/file-r5/`, 2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `4d8259cc`, 2026-10-04, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The cause is `selfhost/check/walk.hero`'s `block`, read by `check/path_end.hero`'s `forever`; `check/flow.hero`, named above, is route M's lattice and holds nothing about loops.

## The repair

Repaired at `4d8259cc`. A `break` after `return`, `break` or `continue` in its block, told `unreachable_statement`, is no longer counted as its loop's way out, so the function is not told `missing_return` beside it; a `break` that may run still counts. Its cases are `fixedbugs-208-a-dead-break-is-no-way-out-of-its-loop`, eleven shapes told once each where the trunk printed 22 messages, and `fixedbugs-208-a-break-that-may-run-still-lets-its-loop-finish`; the lane's census of `check` over 1,957 files moved nothing.

**Closed 2026-10-04** with batch 9 (lanes b9-notext, b9-emit, b9-harness, b9-recovery and b9-annot, merged into one round tree with the trunk at `f6a3122e`), its closing gate run on the round's head from `2c58b28e` to `662870e6`, no line of `selfhost/`, `runtime/` or the seed moving between, with the seed regenerated: 41,364,146 bytes, SHA-256 beginning `26ccaa9d96478a20`, its fixpoint by `cmp`; the compiler's own tests 1,190, all passed; the net's own tests 246, all passed; the full net, 27 suites, 5,268 passed and 0 failed, `fixes` read alone after `662870e6`, which stopped that suite copying the byte fixtures of defects 227 and 241 as text. The census, the trunk's compiler at `703af779` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 1,993, 34 moved, and `build --emit-c` over the 621 holding an `extern`, 3 files of C and 22 of messages moved, every one the batch's own. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 68 fewer messages in the normal arm and 71 in the control arm and none more; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked.
