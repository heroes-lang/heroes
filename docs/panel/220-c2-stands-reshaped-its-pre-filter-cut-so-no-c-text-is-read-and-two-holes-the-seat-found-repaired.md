# Panel 220: C2 stands reshaped, its pre-filter cut so no C text is read, and two holes the seat found repaired

**Convened 2026-10-17 from 11:50, synthesised the same day from 13:20**, by the
compiler-engineer's own condition of panel 218 (*I turn to veto if ... the cost crosses 450 code
lines without a sitting*), C2 having been built in lane `lane-land215` at about twice that.
**Seat**: compiler-engineer alone, the ceiling being the one question (CLAUDE.md § 4). No route
adopted adds surface, a diagnostic class or a spec token. **Resolution: provisional, author
ratification pending.** Brief: `docs/panel/220-briefs/00-shared.md`. Report:
`docs/panel/220-reports/compiler-engineer.md`.

## The proposal, verbatim (the shared brief's question)

> C2, panel 218's R1, is built and measures about 785 code lines where its seat predicted at most
> 450. Does it stand as built, and if not, what is cut or reshaped, with the cost of each cut
> measured?

## What was measured

- **The unit.** `layout`'s `code_lines` counts comment lines and the brief's `awk` did not: in the
  seat's unit, the one its 450 was in, C2 measured **1,001** lines (790 by the brief's `awk`),
  2.2 times the estimate. Where they go: the JSON reader about 217 (estimated 150 to 220), the
  conditions panel 218's synthesis added after the estimate about 135, message text 112
  (estimated 30 to 50), the pre-filter 103 (asked for, never priced), shared probe code moved 60.
- **Hole 1, in the pre-filter**, the one part of C2 that scanned C text: a `;` inside a literal cut
  its scan, so `';' + HS_EA` and `";"[0] + HS_EA` built at exit 0 and printed -2147483589, defect
  2109's own symptom; the same in brackets was refused. The seat's own veto line (*reads C text*),
  met in the part it had asked for.
- **Reshape A, built and measured by the seat**: the text scan cut whole, every unit with an asked
  accessor dumped. 103 lines fewer (C2 at 898 by `layout`); both hole cases refused, every other
  verdict unchanged, the `run` golden and the compiler's 1,705 tests green, the binary identical
  over two generations; the line-marker reader now `cli/bound_said.hero`'s, which reads a Windows
  path's `\\` as one. Cost: 4 dumps on the compiler's own cold build, about 2 s of `user` of about
  85 s at load 10 to 17; nothing warm.
- **Reshape B, the conservative**: keep the scan and send any value holding a literal to the
  dump; 8 lines saved, both shapes closed, 0 dumps; its completeness an argument about C's grammar,
  not a run.
- **Hole 2, in the walk**: a leaf converted to an enumeration with no name, `{ (unsigned)(INT_MIN /
  -1), 2 }` into `struct ae { enum { AE0, AE1 } k; int y; }`, was never asked (`arithmetic` drops a
  spelling holding `(`) and built at exit 0 printing 2147483648.
- **Found beside it**: the lane's 2109 `unsupported` golden pinned `build/tu-38b0a31177f9651b`, a
  key another checkout does not have (a header warning naming the build's unit file).

## Verdicts

| seat | verdict | ground | condition |
|---|---|---|---|
| compiler-engineer | **reshape A**; no veto (nothing touches the lexer, parser, checker, lowering, IR or runtime, §1.7, Part 5; every module under `layout`'s 300) | §1.1, §1.7, Part 5 | to B if the dumps cost more than 5% of the cold self-build on a still machine; to veto if a repair grows into reading C type or expression syntax, or C2 passes 1,000 lines after A without a sitting |

## Resolution (provisional: author ratification pending)

**R1, C2 stands, reshaped by A**: no text of C is scanned to decide what to dump; which accessor
reaches an initialiser is clang's dump's answer. Applied in lane `lane-land215` from the seat's
built modules (`cli/value_c2.hero`, `cli/value_text.hero`), which the coordinator checked were
built on the committed C2 (`e92f33a2f`).

**R2, the holes repaired before landing**: hole 2 by asking a leaf of an enumeration with no
spelling at its own type, `__typeof__((TEXT))`, so every operation in it is checked; and, found
beside the sitting by panel 221's ffi-pragmatist, the elements of an array filled in part, which
clang writes under `array_filler` and not `inner`, read as children (every object of an array
directly under a node is one). Each shape is a case of the 2109 `unsupported` golden, whose
header no longer warns, so no build key reaches it.

**R3, measured at the batch gate on a still machine**: the cold self-build's dumps, against the
seat's 5% line.

### The question to the author

None beyond the ratification: **C2 at about 900 code lines, twice its seat's estimate**, stands
by the seat's own reading of the ceiling. **Recommended: ratify.** The other answer is reshape
B, recorded above.

## Predictions to score

| seat | prediction | instrument | when |
|---|---|---|---|
| compiler-engineer | with A, C2 measures 880 to 920 lines by `layout`; a cold self-build writes exactly 4 dumps and runs no C2 probe; the dumps cost at most 3 s of `user` on a still machine; `K_QCH`, `K_STR` and the anonymous-enum case refused on all four CI legs | `layout`'s `code_lines`; `ls build/init-*/dump*`; `/usr/bin/time -p`; the CI | the batch gate and its CI |

## Process notes

- **The coordinator's count was in the wrong unit**: the brief's 785 dropped comment lines the
  seat's 450 counted. Corrected by the seat; the brief stands as written.
- **No historian sat**, so no `## Prior art` is owed.

## Author's verdict

Pending.

Asked for by https://github.com/heroes-lang/heroes/issues/2122, naming the sitting as `panel 220`.

**Corrected 13:03, by `date`**: the head's *synthesised ... from 13:20* was written without reading the clock; `date` printed 13:02 after this file was committed.

**RATIFIED, 2026-10-17**, R1 to R3 as written above, the author answering in conversation that evening, meant as *I ratify everything*, on the coordinator's list of the five open decisions, each with its recommendation: C2 at about 900 code lines stands,
reshape A, the other answer (reshape B) not taken. Recorded as a reading (CLAUDE.md § 4). The author
may overturn it.
