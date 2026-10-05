---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: 98d01442aa2c00105d280af5a36fa3832109962b
github: none
---

- [x] **326 — an empty complaint from `pkg-config` is told as the note *pkg-config said:* with nothing after it** | a `pkg-config` that fails and writes nothing on its error stream: the package's `ffi_package` carries the note *pkg-config said:* and no words, a note that promises an answer and holds none | `selfhost/cli/package_answer.hero` (the notes written from pkg-config's complaint) · defect 285's reading of the answer's lines · **class: adjacent**

    **Origin:** lane b10-cli, 2026-10-04, reproduced on its compiler at `86b29733` (its final reply's *Found beside*).

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be: the note should say pkg-config gave no reason.

    Repaired at `98d01442`, 2026-10-04 (lane b11-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The shape beside with this cause was repaired with it: clang refusing a link, a unit or the runtime with no word, which read `linking failed:` over an empty line.

## The repair

Repaired at `98d01442`. A `pkg-config` that failed and wrote nothing was told the note *pkg-config said:* over nothing; an empty or whitespace complaint now reads *pkg-config gave no reason*, and an installed package's way out is `link` alone. The same cause at clang, `linking failed:` over an empty line, now reads *clang exited 1 and said nothing*. Measured by hand with stand-ins; its cases are two compiler tests, no golden being able to put a stand-in on the net's PATH.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.
