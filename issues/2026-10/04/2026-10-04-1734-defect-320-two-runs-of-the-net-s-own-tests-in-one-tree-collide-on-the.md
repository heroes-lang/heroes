---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: c0f4f52136e5c5402a0944fffcb0198b6a3662b9
github: none
---

- [x] **320 — two runs of the net's own tests in one tree collide on the fixed scratch `build/harness-selftest`** | two `heroes test tests/harness/main.hero` at once in one tree: 10 and 8 failures, every one a collision on the same scratch folder, where either alone reads 240 passed and 0 failed (the lane's run, discarded) | `tests/harness/absence.hero` (`scratch = "build/harness-selftest"`, six tests), `tests/harness/probe.hero:287`, and the net's other tests that name the folder · **class: improvement**

    **Origin:** lane b9-annot, 2026-10-04, each reproduced on its worktree's harness (its final reply's *Found beside*; scratch `<scratchpad>/batch9/annot/`).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a run of the net's own tests beside another can fail without a defect in what it tests; hardening.

    Repaired at `c0f4f521`, 2026-10-04, gated by its cases and the net's own tests; the net is owed at the batch's close.

## The repair

Repaired at `c0f4f521`. Two runs of `heroes test tests/harness/main.hero` in one tree read 13 and 14 failures, either alone 0: every process named its captures from one serial under one fixed directory. A capture now carries the process's id, and each of the sixty tests that write files works in a directory of its own, made fresh and removed as the test ends. Measured: 252 and 252 passed with two runs at once. Its case is in `shell.hero`.

**Closed 2026-10-04** with batch 10 (lanes b10-ir, b10-cli and b10-harness, merged into one round tree with the trunk at `761525bb`), its closing gate run on the round's head: the seed regenerated at `a134aa74`, 35,206,983 bytes, SHA-256 beginning `c79ffd5ad005c301`, its fixpoint by `cmp`, and the compiler's own tests 1,213, all passed; panel 191's sitting merged at `1dad1ac9`, no line of `selfhost/`, `runtime/`, `tests/` or the seed moving between; then on `1dad1ac9` the net's own tests 260, all passed, and the full net, 27 suites and `cache`, 5,321 passed and 0 failed. The census, the trunk's compiler at `761525bb` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,014, 3 moved, defect 325's own cases; `build --emit-c` over the 621 holding an `extern`, every exit the same, 34 files of C moved by defect 231's one exit and 15 of messages by the build cache's key alone. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, none with more or fewer messages in either arm, 26,329 readings differing by the mutant's file name alone; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.64 s against the trunk's 5.63 s, and `build --emit-c` 69.4 and 70.2 s against 75.0 and 75.9 s.
