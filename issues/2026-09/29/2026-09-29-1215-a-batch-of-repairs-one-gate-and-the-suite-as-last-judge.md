---
kind: decision
area: none
milestone: none
filed: 2026-09-29
commit: eb3fb613ef27526d73317409ddb1de9e7279c383
github: none
---

# A batch of repairs, one gate, and the suite as the last judge

2026-09-29, 11:00 to 12:15, the author's instruction over a morning's conversation, with no milestone open and lanes x1, x3, x4 and y live in other sessions.

## The decision

| | |
|---|---|
| date | 2026-09-29 |
| decision | **a repair or a step is gated by the golden form that holds its cases and by the compiler's own tests; a batch of at most five repairs, one lane per cluster of defects sharing files and worked in sequence, is gated once by the seed's fixpoint, the compiler's own tests, the net's own tests and the full net, then Linux x86-64; Linux arm64 and Windows run once before the push; a red batch is bisected by commit; the seed is regenerated at the batch's close and not per repair; `git merge --ff-only` before a merge commit; and a suite is the last judge, never the first finder: the hooks catch a parse error, a name or type error, a line over the ceiling, a missing annotation and a compiler older than the seed at the moment of writing** |
| reason | the author, in these words: *Nel frattempo, vorrei che ragionassi su un modo molto più veloce di procedere con la risoluzione di bug o lo sviluppo degli step. Io sono convinto che si possa evitare di far girare tutta la suite tutte le volte. [...] ci deve essere un punto, ma solo uno, in cui si va a testare tutto in una rete. È vero che se tu testi due difetti o tre insieme c'è il rischio che si contaminino, però questo è un rischio che si può portare a casa.* And: *la suite di test deve essere veramente l'ultima cosa, cioè non deve individuare cose che gli altri non hanno individuato, ma che in modo molto più semplice si possa individuare.* And, choosing between the two shapes put to them: *Una lane per gruppo, in sequenza* and *windows e arm linux prima del push*. Meant as: it is unsustainable to go at this speed; a repair is tested alone and queued, a batch of four or five is tested together on one platform and then on the others, with one point and only one where everything runs; two repairs tested together may contaminate each other and that risk is accepted; the suite must be the last thing and never find what something simpler could; one sequential lane per cluster; Windows and arm Linux before the push. What was measured before writing it is CL-079 |
| design.md § | none: a process rule, CLAUDE.md § Verification and § 3 |
| panel | none, by CLAUDE.md § 4: the process is amended by author instruction |

## What it replaces, and where the old text stands

CL-063's first half (*the named suites gate a sub-step*) and CL-048's *before the commit*; `.claude/rules/records.md`'s *a lane per DEFECT*; `/step` § 2's gate bullet. Each old sentence stays where it was, with today's date beneath it, as `.claude/rules/records.md` § A record is never rewritten asks.

## What was measured to buy it

CL-079 carries the numbers: the night of 2026-09-28 to 29 (31 commits, 11 merges, 8 gates of 10 to 21 suites all at 0 failed, 0 defects closed), the two weeks 2026-09-15 to 29 (311 commits, 58 merges, 49% of the non-merge commits touching only records, 93 seed commits and 21 seed conflicts, 2 of 59 defects found by a suite, the sittings' hours lost to infrastructure, `pmset sleep 1`), and the day's own timings (`heroes fmt` 0.02 s, `heroes check` 0.18 to 0.48 s per module; the cache copy measured not to help).

## What it did not decide

The Windows leg of CI, red in all 8 red runs of the fortnight and the critical path at 39 minutes: the author's, and untouched. Whether a repair session reads the whole specification at its start (CLAUDE.md § 1): the author's, filed as a question and not changed.
