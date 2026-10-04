---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: f065c0af90dec3652c19b43bf84c92c94dadfd4a
github: none
---

- [x] **324 — `leading_zero`'s advice for a number whose zero is followed by `_`, `0_7`, offers `_7` and `0o_7`, and the compiler refuses both** | `x = 0_7`: `leading_zero`'s note says *Write `0o_7` for the octal value, or `_7` for the decimal one*, and its two `guess` fixes write the same; `_7` is a name, `unknown_name`, and `0o_7` is `misplaced_separator` (batch 9's round compiler at `38d6c6b1`, run by the coordinator 2026-10-04, `<scratchpad>/p324/`); the brief golden `check/leading-zero.hero` hides it, `check` without `--brief` shows it | `selfhost/number.hero:120` to `:135` (`stripped`, the number with its zero taken off and its separator left leading) · **class: blocking**

    **Origin:** lane b10-harness, 2026-10-04, found beside 289 (the `full` form's first reading); reproduced by the coordinator; filed for lane b10-cli, no lane of batch 10 holding `number.hero`.

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a false message, advice that writes two programs the compiler refuses; both fixes are `guess`, so no `--apply` writes them.

    Repaired at `f065c0af`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The harness lane's `full` form, `tests/golden/full/`, is not on this branch, so a case there is the merge's.

## The repair

Repaired at `f065c0af`. For `0_7`, `leading_zero` offered `0o_7` and `_7`, a separator out of place and a name, both refused. The zeros now go with the separators among them, `0o7` and `7`; and under the coordinator's standard, every piece of advice a literal with its reading's value: an `8` or `9` has no octal reading, so only the decimal fix is offered; zeros before a base's marker, `00x7`, get `0x7`; and a letter after the digits, `0644u`, joins no fix, so none is offered and a note says why. The words move to `selfhost/leading_zeros.hero`. Its cases are five under `tests/golden/check/`, `fixedbugs-324-*`, and two compiler tests that scan each fix again and read its value.

**Closed 2026-10-04** with batch 10 (lanes b10-ir, b10-cli and b10-harness, merged into one round tree with the trunk at `761525bb`), its closing gate run on the round's head: the seed regenerated at `a134aa74`, 35,206,983 bytes, SHA-256 beginning `c79ffd5ad005c301`, its fixpoint by `cmp`, and the compiler's own tests 1,213, all passed; panel 191's sitting merged at `1dad1ac9`, no line of `selfhost/`, `runtime/`, `tests/` or the seed moving between; then on `1dad1ac9` the net's own tests 260, all passed, and the full net, 27 suites and `cache`, 5,321 passed and 0 failed. The census, the trunk's compiler at `761525bb` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,014, 3 moved, defect 325's own cases; `build --emit-c` over the 621 holding an `extern`, every exit the same, 34 files of C moved by defect 231's one exit and 15 of messages by the build cache's key alone. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, none with more or fewer messages in either arm, 26,329 readings differing by the mutant's file name alone; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.64 s against the trunk's 5.63 s, and `build --emit-c` 69.4 and 70.2 s against 75.0 and 75.9 s.
