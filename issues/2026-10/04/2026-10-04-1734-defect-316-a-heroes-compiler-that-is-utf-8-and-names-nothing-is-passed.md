---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: 31aaf512118d05df261306f4a828e285db082789
github: none
---

- [x] **316 — a `HEROES_COMPILER` that is UTF-8 and names nothing is passed over in silence for `./heroes`, so the net judges a compiler nobody named** | `HEROES_COMPILER=/nonexistent/heroes` and no compiler named after `--`: the harness's probe, built from lane b9-annot's committed code, printed `compiler ok ./heroes`; since defect 243's harness half (`c539f58d`) a value that is not UTF-8 is refused, and one that names nothing is not | `tests/harness/shell.hero` (`some_compiler`, its candidates after `HEROES_COMPILER`) · defect 276, the runtime's twin of this cause · **class: adjacent**

    **Origin:** lane b9-annot, 2026-10-04, each reproduced on its worktree's harness (its final reply's *Found beside*; scratch `<scratchpad>/batch9/annot/`).

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a hint that names nothing passed over in silence; the lane's reading, as for 276.

    Repaired at `31aaf512`, 2026-10-04, gated by its cases and the net's own tests; the net is owed at the batch's close.

## The repair

Repaired at `31aaf512`. A `HEROES_COMPILER` naming no file, or one with a stray space, had the net judge `./heroes` with no word said. Such a value is now a refusal naming the variable and its value, and the net stops at exit 2 with it, defect 243's precedent; an empty value stays no value. Its case is in `shell.hero`: a value naming no file and one with a stray space each told, an empty one reading the candidates.

**Closed 2026-10-04** with batch 10 (lanes b10-ir, b10-cli and b10-harness, merged into one round tree with the trunk at `761525bb`), its closing gate run on the round's head: the seed regenerated at `a134aa74`, 35,206,983 bytes, SHA-256 beginning `c79ffd5ad005c301`, its fixpoint by `cmp`, and the compiler's own tests 1,213, all passed; panel 191's sitting merged at `1dad1ac9`, no line of `selfhost/`, `runtime/`, `tests/` or the seed moving between; then on `1dad1ac9` the net's own tests 260, all passed, and the full net, 27 suites and `cache`, 5,321 passed and 0 failed. The census, the trunk's compiler at `761525bb` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,014, 3 moved, defect 325's own cases; `build --emit-c` over the 621 holding an `extern`, every exit the same, 34 files of C moved by defect 231's one exit and 15 of messages by the build cache's key alone. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, none with more or fewer messages in either arm, 26,329 readings differing by the mutant's file name alone; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.64 s against the trunk's 5.63 s, and `build --emit-c` 69.4 and 70.2 s against 75.0 and 75.9 s.
