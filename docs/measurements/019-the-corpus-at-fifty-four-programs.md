# 019 — The corpus at fifty-four programs, and the table the site now prints

Date: 2026-09-07 · the site rebuild, its errors page ·
**the run behind a table a stranger can read.**

## Why this file exists

`docs/measurements/016` is the last mutation record, and its numbers are the
ones `site/src/html/errors.html` carried until today: thirteen operators over
44 program directories, 19,411 mutants. The corpus has grown since, the
fourteenth operator has landed, and the site's table had drifted from both.

It was rewritten from a completed run, and this file is that run's provenance.
The reason to write it down is narrower than the reason to take the
measurement: **the site's whole mechanism is that a reader can check it**, and
a skeptic who greps this directory for the table's numbers would have found a
record that contradicts the page and no newer one. That was raised by the
marketing seat of the site panel, which went looking for exactly that.

## Provenance

| what | value |
|---|---|
| compiler | the tree at `7dab98ce`, the seed-built binary in the working tree |
| corpus | `examples/` — **118** `.hero` files under **54** program directories plus the gallery's 12 |
| command | `heroes mutate examples` |
| wall clock | **32 minutes** on the author's Mac, arm64, while the site's own files were being edited (nothing timed was measured against it) |
| baseline | `docs/measurements/016`, thirteen operators over 44 directories |

## What it printed

```
corpus: 118 programs under examples

| operator | mutants | excluded | killed (check) | killed (--permissive) |
|---|---|---|---|---|
| swap-args | 1907 | 102 | 1438 (80%) | 1050 (58%) |
| drop-case | 133 | 0 | 133 (100%) | 133 (100%) |
| forget-at-decl | 758 | 0 | 748 (99%) | 748 (99%) |
| mutate-undeclared | 822 | 149 | 671 (100%) | 671 (100%) |
| typo-ident | 10443 | 71 | 10366 (100%) | 10366 (100%) |
| typo-code | 59 | 0 | 0 (0%) | 0 (0%) |
| wildcard-variant | 457 | 0 | 457 (100%) | 217 (47%) |
| positional-named | 1331 | 1 | 883 (66%) | 0 (0%) |
| mix-int-float | 1492 | 0 | 1492 (100%) | 1492 (100%) |
| shadow | 945 | 0 | 898 (95%) | 0 (0%) |
| drop-question | 236 | 0 | 225 (95%) | 193 (82%) |
| typo-digit | 68 | 0 | 2 (3%) | 2 (3%) |
| boolean-twin | 227 | 0 | 227 (100%) | 227 (100%) |
| local-takes-a-module | 1700 | 0 | 1700 (100%) | 1636 (96%) |
| **total** | 20578 | 323 | 19240 (95%) | 16735 (83%) |
```

The tool prints panel 011's prohibition under that table every time it runs,
and the site obeys it: per-operator or nothing, and `forget-at-decl` is
excluded from any summary because the mistakes it plants are caught by
construction.

## What moved since 016, and what the site had wrong

- **20,578 mutants against 19,411**, over 54 directories against 44.
- **`local-takes-a-module` was on no page.** The site's table had thirteen rows
  and the tool has fourteen operators, so the operator panel 102 asked for was
  measured and then invisible to every reader.
- **Every count on the page was stale**, because the denominator grew:
  `swap-args` 671 to 1,907, `shadow` 285 to 945, `typo-ident` 3,611 to 10,443.
- **`typo-digit` moved off zero**, to 2 of 68. The two rows the design fails at
  are still the two it failed at, and the page still says so.
- **One row was mislabelled rather than stale.** It read *two same-typed
  arguments swapped*, and `selfhost/mutate/edits.hero::swap_args` swaps the
  first two arguments of every call with two or more, whatever their types. The
  page's own explanation contradicted its label, and both now say what the
  operator does. Found by the communication seat of the site panel.

## What the page does about going stale again

The table now prints `$ heroes mutate examples` above itself, so a reader who
doubts it runs the same command. That is the same answer the fixpoint block
gives: the command is the evidence, and the number is what it happened to print
on the day somebody ran it.
