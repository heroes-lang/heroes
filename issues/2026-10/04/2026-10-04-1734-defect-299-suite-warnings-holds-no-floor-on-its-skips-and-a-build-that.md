---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: dc03b34c56c0e3f9146cd46898c4491b9355adea
github: none
---

- [x] **299 — `suite_warnings` holds no floor on its skips, and a build that fails for another reason may read as passing** | lane b9-harness read in `tests/harness/suite_warnings.hero` that its skips have no floor (every other form's skip floor is a third) and that a build failing for a reason other than a missing library may read as passing; **read, not run**: a question until a planted failing build is run through it (2026-10-04) | `tests/harness/suite_warnings.hero` · defect 246's witness, which the suite now asks · **class: improvement**

    **Origin:** lane b9-harness, 2026-10-04 (its final reply's *found beside*), a reading.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument's hole, unmeasured; if a planted failing build reads green, it is `blocking` by 246's reason.

    Repaired at `dc03b34c`, 2026-10-04, gated by its cases and the net's own tests; the net is owed at the batch's close.

## The repair

Repaired at `dc03b34c`, its two sentences written without an em dash at `6c94ad45`. Planted on the base, a stand-in compiler refusing every program read 1 passed, 0 failed: a build that failed for a reason other than a library the machine lacks fell through to the warning scan, and `-O2` was asked through `run`, so a failed build and a program exiting 1 read alike. Each program is now built at `-O0` and at `-O2`, a failed build told with its exit and its stderr at its level, and the skips hold the floor every other skipping form holds. Its case is in `suite_warnings.hero`: a compiler refusing every build, one refusing `-O2` alone, a clean one, and the floor.

**Closed 2026-10-04** with batch 10 (lanes b10-ir, b10-cli and b10-harness, merged into one round tree with the trunk at `761525bb`), its closing gate run on the round's head: the seed regenerated at `a134aa74`, 35,206,983 bytes, SHA-256 beginning `c79ffd5ad005c301`, its fixpoint by `cmp`, and the compiler's own tests 1,213, all passed; panel 191's sitting merged at `1dad1ac9`, no line of `selfhost/`, `runtime/`, `tests/` or the seed moving between; then on `1dad1ac9` the net's own tests 260, all passed, and the full net, 27 suites and `cache`, 5,321 passed and 0 failed. The census, the trunk's compiler at `761525bb` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,014, 3 moved, defect 325's own cases; `build --emit-c` over the 621 holding an `extern`, every exit the same, 34 files of C moved by defect 231's one exit and 15 of messages by the build cache's key alone. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, none with more or fewer messages in either arm, 26,329 readings differing by the mutant's file name alone; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.64 s against the trunk's 5.63 s, and `build --emit-c` 69.4 and 70.2 s against 75.0 and 75.9 s.
