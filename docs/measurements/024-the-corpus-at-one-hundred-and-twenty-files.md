# 024 — The corpus at 120 files, and the row where the compiler had got better

Measurement 019 recorded the mutation run the site prints, over **118** `.hero`
files. The corpus is **120** now, and the site panel's devex seat measured the
drift on 2026-09-10: the table's counts were 36 mutants short. The percentages
held, so nothing on the page was false, and the page was scoped that same hour
to say the corpus of the run it quotes. This is the fresh run, so the page can
quote today instead.

**The number that matters is not the drift.** `drop-question` moved from
**225 killed (95%)** to **236 (100%)**: the compiler now catches every dropped
`?` this tool plants. The site was quoting the project as weaker than it is, on
the page whose subject is what the checks catch, which is the opposite of the
failure the site's own mechanism is built against.

## Provenance

| what | value |
|---|---|
| compiler | the tree at `eededccc`, the seed-built binary in the working tree |
| corpus | `examples/` — **120** `.hero` files under **54** program directories plus the gallery's 14 |
| command | `heroes mutate examples` |
| wall clock | **NOT MEASURED**: five other processes and a subagent's own run were on this machine, so CL-025 forbids the clock. 019's 32 minutes is the figure that stands |
| baseline | `docs/measurements/019`, the same fourteen operators over 118 files |

## What it printed

```
corpus: 120 programs under examples

| operator | mutants | excluded | killed (check) | killed (--permissive) |
|---|---|---|---|---|
| swap-args | 1910 | 102 | 1441 (80%) | 1052 (58%) |
| drop-case | 133 | 0 | 133 (100%) | 133 (100%) |
| forget-at-decl | 762 | 0 | 751 (99%) | 751 (99%) |
| mutate-undeclared | 824 | 149 | 673 (100%) | 673 (100%) |
| typo-ident | 10463 | 71 | 10386 (100%) | 10386 (100%) |
| typo-code | 59 | 0 | 0 (0%) | 0 (0%) |
| wildcard-variant | 457 | 0 | 457 (100%) | 217 (47%) |
| positional-named | 1335 | 1 | 886 (66%) | 0 (0%) |
| mix-int-float | 1495 | 0 | 1495 (100%) | 1495 (100%) |
| shadow | 945 | 0 | 899 (95%) | 0 (0%) |
| drop-question | 236 | 0 | 236 (100%) | 193 (82%) |
| typo-digit | 68 | 0 | 2 (3%) | 2 (3%) |
| boolean-twin | 227 | 0 | 227 (100%) | 227 (100%) |
| local-takes-a-module | 1700 | 0 | 1700 (100%) | 1636 (96%) |
| **total** | 20614 | 323 | 19286 (95%) | 16765 (83%) |
```

## Against 019, row by row

Seven rows moved and seven did not. `swap-args` 1907 to 1910,
`positional-named` 1331 to 1335, `forget-at-decl` 758 to 762,
`mutate-undeclared` 822 to 824, `typo-ident` 10443 to 10463, `mix-int-float`
1492 to 1495, and `drop-question`'s kill count, which is the one that is not
arithmetic. The total is 20578 to 20614, exactly the 36 the two new gallery
programs contribute.

**And a lesson about sub-runs, which the devex seat found by contradicting
itself.** Its two-program run read `shadow` +1 and `typo-ident` +19; over the
whole corpus `shadow` does not move at all and `typo-ident` moves +20. The
headline held and two of the seven rows behind it did not, because mutation
sites are not additive across a corpus. A per-row table built by adding a
sub-run to 019 would have shipped two wrong numbers. Only a whole run is a
measurement of a whole corpus, which is why this file exists rather than a
patch to 019's table.

## What the site does with it

`site/src/html/docs/errors.html` and its Italian twin print this transcript,
with the page's own English row labels, and their prose says the corpus it was
taken over. `site/src/lib/claims.ts` binds that sentence to this file's
`corpus:` line, so the page and the record cannot drift apart in silence: moving
the number in one place turns the build red naming the other. That instrument
arrived with 019's drift and now points here.
