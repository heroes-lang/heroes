---
kind: defect
area: cli
milestone: none
filed: 2026-10-05
commit: bf7fbe4b560ed412eee775f5481813d91476276b
github: none
---

- [x] **349 — `heroes build real.hero -o ""` says the link cannot be followed for *the operating system's own reason is 0*, where there is no link and no reason** | `heroes build real.hero -o ""`: *cannot write : the link cannot be followed: the operating system's own reason is 0*; `argv` accepts an empty `-o` value and the empty landing path is read as a failed link (lane b11-windows, this Mac, 2026-10-05, the lane's report) | `selfhost/cli/argv.hero` (an empty `-o` accepted) · the landing of a build's output (`selfhost/cli/publish.hero`) · defect 346, the runtime's reasons told as the system's, a different cause · **class: blocking**

    **Origin:** lane b11-windows, 2026-10-05, beside defect 346's repair (its final report, *Found beside*).

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a false message, a link and a reason named where there is neither; never deferred.

    **2026-10-05:** Repaired at `bf7fbe4b`: the parser refuses an empty value for every flag that takes one, naming it, gated by its two cases red first and the compiler's own tests (1,219) and the net's own (282), all passed on this Mac and on Linux arm64; the landing's own reading of an empty path, `cli/files.hero` and `cli/publish.hero`, is lane misc's and reported to the coordinator; the net is owed at the batch's close.

## The repair

Repaired at `bf7fbe4b`. `heroes build real.hero -o ""` told *the link cannot be followed: the operating system's own reason is 0*, a link and a reason where there was neither; a flag that takes a value now refuses an empty one as it refuses a missing one, *`-o` needs a path, and was given an empty one*, at exit 2, for every value-taking flag of the table, and an empty word after `run`'s `--` still goes to the program. Its cases are compiler tests over every such flag and the net's own test of the reproducer.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.
