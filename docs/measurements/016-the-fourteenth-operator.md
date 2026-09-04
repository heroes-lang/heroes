# 016 — The fourteenth operator, and the corpus at forty-four programs

Date: 2026-09-04 · M-corpus-depth, its last instrument item ·
**the measurement panel 102 was told to wait for.**

## Why this file exists, and why it waited

Panel 102 ratified a refusal on 2026-09-02 — *a local may not take a name a
`use` bound* — and paid its 22 spec tokens with a prediction about corpus
cost, which `design.md` §1.6 calls the weaker of the two payments it admits.
The stronger one was available and deliberately deferred: an operator for
`heroes mutate` that plants exactly that mistake, so the refusal has a
measured kill rate instead of an argument.

It was deferred because **adding an operator moves the denominator of the
thesis's own score**. Metric 3 is the fraction of plausible mistakes the
compiler catches; a fourteenth operator changes what "all the mistakes" means,
so the discontinuity is spent once, against a corpus wide enough for the
number to be worth having. `docs/measurements/014` is the thirteen-operator
baseline it is scored against, and the corpus has grown from 35 programs to
**44** in between.

## Provenance

| what | value |
|---|---|
| compiler | the tree at this commit, rebuilt through the fixpoint |
| corpus | `examples/` — **107** `.hero` files under 44 program directories |
| command | `heroes mutate examples --survivors` |
| wall clock | **1,328.85 s** (22:09) on the author's Mac, arm64 |
| baseline | measurement 014: 13 operators, 78 files, 10,711 mutants |

## The operator

`local-takes-a-module` rewrites a binding's name to a name a `use` in the
same file bound. It imitates the reader who has `use table` at the top and
writes `table = rows.sort()` twenty lines down — a mistake no habit from
another language warns anybody about, because in most languages it is legal.

A file with no `use` produces no mutant, which is a fact about the file
rather than a gap: the mistake cannot be made where there is no module name
to take.

## The table

| operator | mutants | excluded | killed (check) | killed (--permissive) |
|---|---|---|---|---|
| swap-args | 1864 | 102 | 1412 (80%) | 1032 (59%) |
| drop-case | 132 | 0 | 132 (100%) | 132 (100%) |
| forget-at-decl | 659 | 0 | 649 (98%) | 649 (98%) |
| mutate-undeclared | 714 | 120 | 592 (100%) | 592 (100%) |
| typo-ident | 9849 | 68 | 9775 (100%) | 9775 (100%) |
| typo-code | 56 | 0 | 0 (0%) | 0 (0%) |
| wildcard-variant | 457 | 0 | 457 (100%) | 217 (47%) |
| positional-named | 1222 | 1 | 874 (72%) | 0 (0%) |
| mix-int-float | 1342 | 0 | 1342 (100%) | 1342 (100%) |
| shadow | 914 | 0 | 868 (95%) | 0 (0%) |
| drop-question | 236 | 0 | 225 (95%) | 193 (82%) |
| typo-digit | 48 | 0 | 2 (4%) | 2 (4%) |
| boolean-twin | 218 | 0 | 218 (100%) | 218 (100%) |
| **local-takes-a-module** | **1700** | **0** | **1700 (100%)** | **1636 (96%)** |
| **total** | 19411 | 291 | 18246 (95%) | 15788 (83%) |

Never pool these into one headline (panel 011): a per-operator rate is the
measurement, and `forget-at-decl` is excluded from any summary because the
mistakes it plants are caught by construction.

## What the new row says

**1,700 mutants, 1,700 killed in the strict arm.** Every program in this
corpus that names a module refuses a local that takes that name, which is
panel 102's ruling holding at every site it can be tested at.

**And 64 of them survive the permissive arm**, which is the number the
sitting was owed. `--permissive` drops the thesis-bearing checks; 1,636 of
the 1,700 die there anyway, because taking a module's name usually breaks
something else as well — a qualified call stops resolving, a type stops
matching. The remaining **64 are killed by panel 102's rule and by nothing
else**: 64 programs that would compile, run and be wrong without it. That is
what 22 spec tokens bought, stated as a count rather than as a conviction.

## What else moved since measurement 014

| | 014 (13 operators, 78 files) | here (14, 107 files) |
|---|---|---|
| mutants | 10,711 | **19,411** |
| killed, strict | 10,099 (96%) | **18,246 (95%)** |
| killed, permissive | 8,782 (83%) | **15,788 (83%)** |
| survivors listed | 479 | — |

**The rates did not move**, which is the result worth reading twice: the
corpus grew by nine programs and 29 files, including two whose whole subject
is arithmetic against published numbers and one that binds eighteen C
functions, and the fraction of plausible mistakes this compiler catches
stayed where it was. A corpus is supposed to do that to a rate. One that
moved would mean the earlier number had been measuring the corpus rather
than the language.

**`typo-digit` moved 0% → 4%**, its first non-zero score. It was 0 of 29 in
014 and is 2 of 48 here, and the two it kills are in the programs that
arrived since: a digit changed inside a published constant is caught where a
program checks itself against a document. That is `checksum/`, `nbody/` and
`spectral/` doing exactly what their family was written for — and it is a
better argument for oracle-checked programs than the plan's own was.

**`swap-args` remains the weakest live operator** at 80% strict, with 452
mutants surviving. It is the one to attack next if anybody attacks one:
every survivor is two arguments of one type that the language does not make
a caller name, and `spec:104` already names two of a kind. The gap is
between "two parameters of one type" and "two parameters whose types are
different but confusable".

## What is still zero, and why it is here

`typo-code` is 0 of 56, in both arms, as it has been since measurement 004.
An error code is a `str`, so a typo in one is a different `str` and not a
compile error. **The operator exists to measure a hole rather than to
confirm a defence** — `mutate/ops.hero`'s own module doc says so — and an
instrument that could only report success would not be one.
