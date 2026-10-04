---
kind: defect
area: harness
milestone: none
filed: 2026-10-03
commit: 80cb060ca850d08f4eecfb0d001c48ea4e8aff1a
github: none
---

- [x] **209 — a `layout` run filtered to a name that matches no module reads 1 passed** | `./heroes run tests/harness/main.hero -- ./heroes layout <a name no module has>`: `1 passed, 0 failed`, because the harness's guard against an empty selection counts cases and the suite's file-wide checks are always one case | `tests/harness/suite_layout.hero` · the harness's selection guard · **class: improvement**

    **Origin:** the coordinator's agent finishing defect 167 in lane cb4, 2026-10-03, which then checked by hand that each of its six filters matched one module (`scratchpad/lane-cb4/progress.md`, 2026-10-03).

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an instrument that can read green over nothing; no program moves.

    Repaired at `80cb060c`, 2026-10-04, gated by its cases and the net's own tests; the net is owed at the batch's close.

## The repair

Repaired at `80cb060c`. `layout` narrowed to a name no module has read 1 passed at exit 0, its ceiling one case for every module its walk took, where every other narrowed form exits 2. A narrowed walk that selects no module now records nothing, and the harness's own rule, a narrowed run that selected no case exits 2, reads it. Its case is in `suite_layout.hero`: a walk selecting nothing records no case, one selecting `selfhost/ast.hero` records its pass.

**Closed 2026-10-04** with batch 10 (lanes b10-ir, b10-cli and b10-harness, merged into one round tree with the trunk at `761525bb`), its closing gate run on the round's head: the seed regenerated at `a134aa74`, 35,206,983 bytes, SHA-256 beginning `c79ffd5ad005c301`, its fixpoint by `cmp`, and the compiler's own tests 1,213, all passed; panel 191's sitting merged at `1dad1ac9`, no line of `selfhost/`, `runtime/`, `tests/` or the seed moving between; then on `1dad1ac9` the net's own tests 260, all passed, and the full net, 27 suites and `cache`, 5,321 passed and 0 failed. The census, the trunk's compiler at `761525bb` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,014, 3 moved, defect 325's own cases; `build --emit-c` over the 621 holding an `extern`, every exit the same, 34 files of C moved by defect 231's one exit and 15 of messages by the build cache's key alone. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, none with more or fewer messages in either arm, 26,329 readings differing by the mutant's file name alone; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.64 s against the trunk's 5.63 s, and `build --emit-c` 69.4 and 70.2 s against 75.0 and 75.9 s.
