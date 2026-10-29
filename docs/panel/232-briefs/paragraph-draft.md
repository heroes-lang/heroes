# The paragraph `1.0.0` rests on: the coordinator's draft, for the sitting

Drafted 2026-10-29 at M-compatibility-promise, not adopted: the sitting on
the specification prices it, decides its home and may refuse any sentence.
**Repaired the same day after the critic's first pass**
(`docs/panel/232-reports/critic-briefs.md`): the first draft said *nothing
can catch that stop*, false since a program's own C can (F20); grounded *it
runs one way* in a refusal panel 056 made conditional (F14); promised that a
newer compiler *never refuses an older program* beside a clause letting a
`1.x` release repair a defect, and *does what it did* where the
specification leaves the answer open (F22); and excluded no specification
error (F27).

> **What 1.0.0 promises.** A program that compiles under Heroes 1.0.0
> compiles under every later 1.x compiler, and does what the specification
> says it does. Three things are outside it: a program that depends on a
> defect of the compiler, which a 1.x release may repair; what the
> specification leaves open, such as the order of a map's keys; and an error
> in the specification itself, which a 1.x release may correct. The promise
> covers the language the specification describes, the functions it names,
> and those of `hero_os.h`.
>
> **It runs one way.** A program cannot say which version of the language it
> was written for, so a newer compiler keeps an older program, and an older
> compiler is never asked to read a newer one.
>
> **A Heroes program stops rather than guesses.** There are no exceptions:
> an error a program does not handle ends it at once, saying why and where,
> and no Heroes code can catch that stop. A program meant to run for a long
> time runs under something that starts it again.
>
> **2.0.0 may break this promise**, and says which programs it breaks.

## What each sentence rests on, read 2026-10-29

- *compiles under every later 1.x*: the author's ruling of 2026-10-29, the
  promise starting at `v1.0.0`; `tests/harness/suite_compat.hero` judges
  that a program still checks.
- *does what the specification says it does*: unmeasured by any suite. The
  compatibility suite checks; it does not run the programs. The corpus's
  `main.expected` files exist for 67 of the 69 examples (F23), and whether
  the suite should run them is the sitting's question.
- *what the specification leaves open*: `keys(m)` prints the same order
  every run today, though § 10 says *in no order* (F22); a program that
  relies on that order is outside the promise by this clause.
- *an error in the specification itself*: Go 1's document excludes
  specification errors among its eight exclusions (F27, the historian to
  verify); items 2 to 4 of this sitting are such errors, found before the
  promise.
- *those of `hero_os.h`*: design.md Part 7 item 2, *File I/O*, as decision
  2228 amended it: *its functions' stability is promised with the rest at
  M-compatibility-promise*; the header declares 57 functions (F15).
- *It runs one way*: CLAUDE.md § 10 admits no per-project file while panel
  056's refusal stands, and that refusal is conditional on its three return
  conditions (`.claude/rules/cli-surface.md`), not permanent (F14); the
  sitting decides whether the paragraph promises one way as a rule of
  `1.x` or states it as today's fact.
- *no Heroes code can catch that stop*: a `SIGABRT` handler in C the
  program binds turned an abort into exit 0, run by the critic (F20); the
  promise is about the language, and C is outside it.
- *stops rather than guesses*: design.md Part 6's permanent refusal of
  exceptions; spec § 6, *an abort ends the program at once, saying why*.
- *and where*: no sentence of the specification; the report's place is
  M-panic-location's (panel 230's R1), and a call through a function value
  of the library names the library's line, `<heroes library>:232:1` for
  `exit` (F25), so the sitting decides whether the paragraph promises it.
