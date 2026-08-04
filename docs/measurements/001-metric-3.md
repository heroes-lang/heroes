# 001 — metric 3, first run: the silent-error rate

Date: 2026-08-04 · milestone M3d · **the first measurement of the thesis itself.**

## Provenance

| what | value |
|---|---|
| compiler | `b8da932` (`git rev-parse`) |
| spec | sha256 `f67917c91db2…` — v0, still frozen |
| corpus | `examples/` — 10 programs that check clean today (the gallery's nine plus `first.hero`) |
| operators | `harness/mutations/operators.md`, all ten |
| arms | `check` and `check --permissive` (the same compiler, the twelve thesis rules dropped) |
| command | `heroes mutate` |

Never diff this against a run with a different compiler sha unless the difference
is the point (`harness/README.md`).

## Result

| operator | mutants | excluded | killed (check) | killed (--permissive) |
|---|---|---|---|---|
| swap-args | 46 | 0 | 31 (67%) | 14 (30%) |
| drop-case | 4 | 0 | 4 (100%) | 4 (100%) |
| forget-at-decl | 20 | 0 | 18 (90%) | 18 (90%) |
| mutate-undeclared | 19 | 2 | 17 (100%) | 17 (100%) |
| typo-ident | 194 | 3 | 191 (100%) | 191 (100%) |
| wildcard-variant | 15 | 0 | 15 (100%) | 2 (13%) |
| positional-named | 31 | 1 | 26 (87%) | 0 (0%) |
| mix-int-float | 35 | 0 | 35 (100%) | 35 (100%) |
| shadow | 13 | 0 | 13 (100%) | 0 (0%) |
| drop-question | 2 | 0 | 2 (100%) | 2 (100%) |
| **total** | 379 | 6 | 352 (94%) | 283 (76%) |

Never pool these into one headline (panel 011): a per-operator rate is
the measurement, and `forget-at-decl` is excluded from any summary because
its catch rate is 100% by construction.

## What the numbers say, and what they do not

**The measured effect of the design is the gap between the two columns**, not the
first column. A compiler that rejected every program would score 100% and prove
nothing, which is why panel 011 made the control arm mandatory before any number
was allowed to be quoted.

Read per operator, as panel 011 requires — never pooled:

- **`wildcard-variant`: 100% → 13%.** The largest single effect in the run. §4.7's
  ban on `_` over a variant is doing exactly what it was argued to do: without it,
  a lazy catch-all is accepted 87% of the time.
- **`shadow`: 100% → 0%** and **`positional-named`: 87% → 0%.** Entirely thesis
  effects: nothing else in the language catches either mistake.
- **`swap-args`: 67% → 30%.** §4.9's same-typed-argument rule accounts for 37
  points. The third that survives strict checking is the honest part of the
  measurement: swapping two arguments of *different* types is caught by the type
  system (so it is in the 67%), and swapping two that are already labelled is
  caught by the label rule — what survives is a swap the program cannot tell from
  correct, which is a real limit and not a bug.
- **`typo-ident`: 100% in both arms.** Typos are caught by *names and types*, not
  by the thesis rules. Worth stating plainly: the resolver's unknown-name error
  would exist in any compiler, and this operator measures it rather than §1.
- **`forget-at-decl`: 90%, and the two survivors are correct.** Turning
  `v: int @ 0` into `v = 0` is only a mistake if something later writes `v`. Where
  nothing does, the mutation is **meaning-preserving** — which is precisely the
  counter-arm panel 011 asked for, appearing on its own inside a mutation
  operator. The operator table should say so; the number is right.
- **`drop-question`: 2 mutants.** The corpus barely uses `?`. That is a corpus
  gap, not a result: the acceptance program is where `?` lives, and it is not in
  `examples/` until M6 restores it as a file.

## Caveats on the record

1. **The corpus is assistant-written** (the gallery), so it measures the
   assistant's idea of ordinary Heroes. `harness/tasks/README.md` requires
   author-written tasks for metric 2 for exactly this reason; metric 3's corpus
   inherits the same weakness and no headline should be quoted without it.
2. **One mutant per site, first two arguments only** for `swap-args`: a call with
   four arguments has three more inversions this run does not try.
3. **Metric 2 has not run.** No model has written Heroes from the spec in this
   project's history, so "a model makes fewer mistakes" remains unmeasured — the
   thesis keeps its *measured mechanism* and does not yet have its measured claim
   (panel 011's own words).
