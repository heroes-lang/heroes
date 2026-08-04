# 002 — metric 3, second run: a wider corpus, and the same shape

Date: 2026-08-04 · milestone M4 (after close) · **not comparable to 001 by design.**

## Provenance

| what | value |
|---|---|
| compiler | `e12ba53` (`git rev-parse`) |
| spec | sha256 `823ddab1b065…` — v2, measured max 2139 |
| corpus | `examples/` — **12** programs (the gallery's twelve, which now includes `00-first.hero`, moved in from `examples/`, plus `10-maps` and `11-trees`, written at M4) |
| operators | `harness/mutations/operators.md`, all ten — unchanged |
| arms | `check` and `check --permissive` — unchanged |
| command | `heroes mutate` |

**Why this file exists rather than an edit to 001.** The corpus changed, so the
numbers changed, and a published measurement whose input has moved underneath it is
the defect class this project keeps catching in its own documents. 001 records what
was measured on ten programs at M3d; this records what is measured on twelve at M4.
Panel 011's rule holds in both: a per-operator rate is the measurement, and the two
files are **not** a before/after of anything — nothing about the compiler's rules
changed between them.

## Result

| operator | mutants | excluded | killed (check) | killed (--permissive) |
|---|---|---|---|---|
| swap-args | 57 | 0 | 42 (74%) | 25 (44%) |
| drop-case | 8 | 0 | 8 (100%) | 8 (100%) |
| forget-at-decl | 24 | 0 | 22 (92%) | 22 (92%) |
| mutate-undeclared | 23 | 2 | 21 (100%) | 21 (100%) |
| typo-ident | 276 | 3 | 273 (100%) | 273 (100%) |
| wildcard-variant | 28 | 0 | 28 (100%) | 11 (39%) |
| positional-named | 42 | 1 | 37 (90%) | 0 (0%) |
| mix-int-float | 50 | 0 | 50 (100%) | 50 (100%) |
| shadow | 19 | 0 | 19 (100%) | 0 (0%) |
| drop-question | 2 | 0 | 2 (100%) | 2 (100%) |
| **total** | 529 | 6 | 502 (96%) | 412 (79%) |

## What moved, and what did not

| operator | 001 (10 programs) | 002 (12 programs) | mutants |
|---|---|---|---|
| swap-args | 67% / 30% | **74% / 44%** | 46 → 57 |
| drop-case | 100% / 100% | 100% / 100% | 4 → 8 |
| forget-at-decl | 90% / 90% | 92% / 92% | 20 → 24 |
| mutate-undeclared | 100% / 100% | 100% / 100% | 19 → 23 |
| typo-ident | 100% / 100% | 100% / 100% | 194 → 276 |
| wildcard-variant | 100% / 13% | 100% / **39%** | 15 → 28 |
| positional-named | 87% / 0% | 90% / 0% | 31 → 42 |
| mix-int-float | 100% / 100% | 100% / 100% | 35 → 50 |
| shadow | 100% / 0% | 100% / 0% | 19 → 19 |
| drop-question | — | 100% / 100% | 2 |

**What can honestly be said.** The two columns that were flat stayed flat, and they
are the ones worth reading twice: `typo-ident`, `mix-int-float` and
`mutate-undeclared` are at 100% in **both** arms, so the thesis rules add nothing to
them — those mistakes are caught by names and types, as they would be in any
language. The two that are zero in the permissive arm stayed zero: `positional-named`
and `shadow` are rules nothing else in the language looks for.

**What cannot.** Two permissive rates moved up — `swap-args` 30 → 44 and
`wildcard-variant` 13 → 39 — and this file does **not** explain why. Attributing a
per-operator delta to a property of the new programs would need the mutant list, and
`heroes mutate` prints rates rather than mutants. The corpus grew for reasons that had
nothing to do with the measurement (two examples written to cover lowering shapes),
so the honest reading is that these numbers describe a different corpus, not a
changed compiler: no rule changed between 001 and 002, and the compiler sha differs
only by M4.

The gap between the arms is **17 points** (96 vs 79) against 18 at 001 — the same
finding at a different corpus size.

**What this suggests for the harness.** A per-operator delta is uninterpretable
without the mutants behind it. `heroes mutate` should be able to print the surviving
mutants for one operator, so a moved rate can be read rather than guessed at. Queued.
