---
kind: defect
area: cli
milestone: none
filed: 2026-10-05
commit: 122633b0c12c484b93ef56905b38db095e3b9e31
github: none
---

- [x] **340 — `selfhost/cli/artifact.hero` says `--emit-c` refuses exactly what `build` refuses, and `ffi-missing-link` is built by one and refused by the other** | `heroes build --emit-c` over `tests/golden/fixedbugs/ffi-missing-link.hero` exits 0 and writes C, where `heroes build` exits 1 at the link `--emit-c` does not run; the module's comment says the two refuse the same programs (lane b11-misc, 2026-10-05, measured, the lane's report) | `selfhost/cli/artifact.hero` (the comment) · defect 298, which corrected the same premise in `suite_emission.hero` · **class: adjacent**

    **Origin:** lane b11-misc, 2026-10-05, beside defect 298's repair (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a comment that is false of the compiler, no message or program moving.

    Repaired at `122633b0`, 2026-10-05 (lane b11-misc), gated by the compiler's own tests and `layout`; the net is owed at the batch's close.

## The repair

Repaired at `122633b0`. `selfhost/cli/artifact.hero` said `--emit-c` refuses exactly what `build` refuses; it now says `--emit-c` refuses what `build` refuses before the link, and that a program refused only at the link still has C. Re-measured: `ffi-missing-link` exits 0 under `--emit-c` and 1 under `build`. A comment; no program moves.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.
