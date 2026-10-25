# 070 — M-rechecked-issues: every issue run again, and the probe that could not end

## Goal

Row 71 of the chain, opened on 2026-10-25 (`9a38f1b26`) in the lane
`rechecked-issues`, by the author's instruction of 2026-10-14: a step that runs
every issue open at its start against the tree, gives each a dated comment, and
leaves it closed with the commit that settled it, closed with the reason it no
longer applies, repaired here when small, or re-homed on a row still to come.
Beside the issues, the milestone's page asked for design.md Part 7's return
conditions and Part 8's warts 8, 15 and 16 run again, the formatter's probe
M-inferred-cell's changes still owed, the false sentences the re-ordering of
2026-10-14 had found corrected underneath, `tools/spike/` out of the tree, and
one line in `/step`'s close checklist so the pile does not grow back. **The
acceptance**: `records/homes` and `records/tagged` green with GitHub asked,
every issue open at the start carrying its dated comment, and the counts after
measured beside the counts before, here.

## What surprised

**The probe that could not end was the formatter.** Defect 2139 said the
`bracket` family over `tests` could not end, 84,027 variants of one seed at more
than 2.6 s each, and the shared brief of panel 229 framed it as the judges' cost
on a seed nested 2,000 deep. The critic's first pass, before any seat sat, ran
`heroes fmt` on the variants: 31 s for one comment after the 1,000th `[` of a
chain 2,000 long written as one statement, 0.07 s for the file itself, the nest
cheap. Two squares in `selfhost/print/` did it: `written_pairs` walked every
written token again for each one's partner (repaired at `01c5efd75`, 29.8 s to
9.9 s, the same bytes), and `brackets.spread` asked `bare.needs_group` at every
part of a chain, each answer asking the part below and rendering its whole text
(panel 229's R1, `9a627663a`: 9.85 s to 0.09 s). With both, the owed run's work
fell from about 70 cool CPU hours to about 15, every one of the 4,126 tracked
`.hero` files formatting to the same bytes. Black met the same chain and the same
square this October, and pinned its repair with a test that counts calls; this
one is pinned the same way.

**The probe's stride had a period of its own.** `bracket` writes `T`, `O` and
`B` at every place, in that order, so every family's count is a multiple of 3
and a stride that is one judges one form of each place in every seed; the census
row had run with 9,999,999 since defect 155, harmless there, and defect 506
holds in its `O` form, so the blindness is not hypothetical. The probe refuses
such a stride since panel 229's R3.

**The rules said the probe two ways for sixteen days.** § The formatter's probe
said *before a push, push only over exit 0*; § The optimistic chain, the
author's instruction of 2026-10-09, put it after the push with nothing waiting,
and eight commits under `selfhost/print/` had been pushed by the second (panel
229's critic). Panel 229's R4 says which stands.

**Every refused unit, and then each at its own line.** Defect 2143's other half
was a round whose verdict was its first refused unit's, the others kept for the
tag round alone; reading every one (`cli/refused_units.hero`) showed, beside it,
a finding placed by spelling: one constant two modules bind told twice at the
first, a header two modules name told once (defect 2208, repaired here at the
author's answer).

**Part 7 reopened a door by its own letter.** `alias`'s return condition (b), a
commit changing one structural type spelling at 8 or more sites, was met twice,
by a record and by a qualified name, with no alias wanted (issue 2209 takes it to
M-core-packages' sitting); and wart 16's measured hazard, `write_file` to
`/dev/stdout` reordered beside `print`, no longer reproduces since panel 156's
line-buffered stdout, the route through `fdopen` still does.

**The site's log had missed thirteen milestones**, and its feed numbered entries
by their place, so a late postcard would have renumbered every one above it; the
feed reads each postcard's own id since `d861cbf72`.

## What broke and why

- **Step 4 gated a runtime comment by `emission` and not by `emit`**: the
  comment moved every program's sources key, `emission`'s 618 files were blessed
  that step, and `tests/golden/emit/`'s twelve were red at the batch's gate, each
  on its `hero_program_sources` line alone; rewritten by hand there (`3f52c6fea`).
  The map's runtime row names the form that holds a change's cases, and `emit`
  is one.
- **Steps 2204 and 2205 ran without `order`**: three map walks with a sort and no
  `# ORDER:` mark, found red beside defect 2143's gate and marked at step 5
  (`76ca5de65`); one mark's first reading, that two `use` lines may name one
  module, was false (`used_twice`), run before it was written.
- **A build run from inside `tests/golden/unsupported/`** left its `build/` cache
  there, 133 files, the session's own, removed the same minute.
- **The site's build refused step 8's Italian description**, 166 characters past
  the 160 a search result shows; shortened at `d861cbf72`. And `records/numbered`
  refused *MacBook Air M3* in a rule, a chip read as a numbered milestone.
- **Writes outside the repository's root**, each removed: early in the session a
  file of the coordinator's in `/tmp` and a note in the session's scratchpad; panel
  228's seats' files in that scratchpad, moved into the tree's evidence; the
  postcards' drafting agent's three working files there; panel 229's
  compiler-engineer's stray copy of `selfhost/` to `/private/tmp`; and the
  harness's own task files of the critic's background commands, which no agent
  wrote.
- **Panel 229's compiler-engineer stopped at the API's weekly limit** with its
  verdict unwritten, and finished from its own report on the author's new
  account.
- **Two messages to other sessions expired undelivered**: one to `sixtyalive-70`
  early in the session, and the push's notice to `heroes-lang-13`, held by a
  session in another permission mode; its commit `f5ce7d786` (measurement 053),
  made on the trunk's local `main` and not pushed, stood beside `origin/main`
  when the round was pushed, for its owner to merge.

## What landed, and what carried forward

**The re-check**: all 57 issues open at the start (5 defects, 24 features, 28
tasks, no decision; `open` at `76e98124e`) carry a dated comment of this
milestone; 4 closed with their reasons (653, 860, 864, 1105); 3 re-homed with the
author's yes (715 to M-latency-ceiling, 1001 to M-panic-location, 1234 to
M-generated-programs); 3 repaired here (1075 `--only`, 1077 `--help`, 936 the
Windows quoting round trip's test); the milestone's own three (2090, 2091,
2092) delivered.

**The repairs**, each its own commit, closed at the batch's gate: defects 2198
(`29f26f34e`), 2190 (`b2cfe69f3`), 2203 (`1d66ece88`), 2205 (`5b33f5559`), 2204
(`120147a9b`), 2158 with feature 2091 (panel 228's landing, `ca95f2b5d`), 2143
(`a126f7a60`), 2208 (`ccf1bedca`); 2139 by panel 229 (`01c5efd75`, `9a627663a`)
and the owed run below. **The sittings**: panel 228 (`ca1bcccd7`), ratified by
the author the same day (`9327f8836`, decision 2207); panel 229 (`32fcc8305`),
provisional, decision 2210. **Beside them**: `tools/spike/` out (`f6d7e3355`),
the re-ordering's false sentences and the ones found beside them (`f47421d04`),
Part 7 and Part 8 run again (`d7dd9bb48`), the site's log (`d861cbf72`,
`5387b5e21`).

**The counts, before and after**, `open` on GitHub: at the start 57, 5 defects,
24 features, 28 tasks, no decision; at the close 49: 3 defects (2206,
`adjacent`, this milestone's own finding; 2211 and 2212, `improvement`, another
session's), 20 features, 25 tasks and 1 decision (2210, panel 229's
ratification), 2090 closed at the close on the batch's gate, `3f52c6fea`. Of the
57, 15 closed and 42 stand open with their dated comments, three of them on the
rows that now owe them; of the eight issues this milestone filed, five closed
(2203, 2204, 2205, 2207, 2208) and three stand (2206, 2209, 2210); another
session filed four the same day (2211 to 2214).

**The owed run**: defect 2139 closed by the author's answer on its seed judged whole:
259's `bracket` family in eight parts at `--stride 8 --offset 0` to `7`, each at
exit 0, all 84,027 variants judged, 72,012 parsing and 72,012 held, none refused,
15,654.78 s of CPU under a load of 35 to 90 from two other sessions' gates
beside it. The rest of the owed run, 6,247 jobs over a copy of the lane at
`3f52c6fea` with the compiler at `-O2`, goes on as the post-push check §
The optimistic chain makes it; its result goes on defect 2139's issue when it
ends, and its wall is not a measurement.

**Carried**, each an issue with a home: defect 2206 (`adjacent`, a parser
message); issue 2209 (`alias` to M-core-packages' sitting); decision 2210
(panel 229's ratification); and panel 229's (D), (H), (L) and (G), recorded in
its file for a later sitting. Defects 2211 and 2212, and tasks 2213 (on
M-compatibility-promise) and 2214 (on M-generated-programs), were filed the same
day by another session comparing Heroes with Goose, the last two from a
paragraph it had offered this milestone's page.

**The gate**, on this Mac over the merged lane: stage 2 from the seed (`sources
00fc702a64dd9600`) and stage 3 from it emit the compiler's 27,000,911 bytes
alike, so the seed is not refreshed; the compiler's own tests 1,750 and the
net's own 425; the full net's 31 suites at 0 failed once `emit`'s twelve key
lines were written, `cache` 8 alone after the rest; on Linux arm64, in its
container with the same stage 2, `emission` 1,287 and 2 before and 1,289 and 0
after the two cases this Mac skips were blessed; the site's build, 190 pages
(`3f52c6fea`). Pushed to `origin/main` the same afternoon on the author's yes,
27 commits, before the close.

**Status at the close**, for `ROADMAP.md` § Where we are: the compiler 684
modules and 177,905 lines of Heroes (`find selfhost -name '*.hero'`); the seed
263,383 lines, 26,240,524 bytes, unchanged; the specification and the contract
untouched.

## Tagged over a clean list, 2026-10-25

**`m-rechecked-issues` is placed on the commit this section is written in**,
over a list asked of GitHub before it was placed: no `blocking` or `systemic`
defect open (2206, `adjacent`; 2211 and 2212, `improvement`), and one decision
open, panel 229's ratification, which a tag stands over (`records/tagged`).
Defects 2139 and 2143, carried past `m-thesis-harness` by the author's waiver
(decision 2200, *the next tag cannot stand over them*), are closed.

## The owed run's end, 2026-10-25

The rest of panel 229's R5 run ended at 19:39, after the push: 6,247 jobs,
none at exit 2; 4,142,682 variants judged, 3,495,006 parsing and all held, none
refused; 974 jobs at exit 1, 487 seeds that are programs with diagnostics of
their own, refused as seeds by the exit-code contract; the fixtures, 28,207
judged, at exit 0. No defect was filed. Its 4.20 hours of wall and 24.1 hours
of CPU are not a measurement, heroes-lang-13's full net and other projects'
work beside it at a load of 188. The sitting's file scores the two predictions
that waited on it, the compiler-engineer's 4 held in its measured half and its
6 held at this milestone's tag, 99.21% of the places kept by (B)'s rule.
