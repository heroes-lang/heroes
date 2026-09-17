# Panel 160 — the reader that forgets the level

2026-09-17, at M-check-completeness. Five seats. **The completeness critic did
not sit** — it died on a session rate limit after its first tool call, and what
it was convened to settle the coordinator measured instead, below, marked as
such. **Resolution provisional — ratified the same day by the author's standing
delegation, recorded at the foot.**

Convened on defect 050. A value can be fallible twice — `m[k]` on a
`{str: i64?}`, `find` over a `[i64?]` — and `.is_err()` on it answers *was the
key there* while the line reads *did the stored value fail*. Panel 158's
completeness critic filed it while auditing that sitting's own framing; that
sitting had measured that of its four repairs only one closed this, and that one
protected zero corpus programs.

## The five as put, and the verdict table

**A** refuse all four readers on a nested fallible · **B** refuse `{K: V?}` at
the declaration · **C** a distinct spelling for the outer question · **D**
document only · **E** refuse `.is_err()` alone.

| seat | verdict | for | veto |
|---|---|---|---|
| **compiler-engineer** | approve | **E** — +58 lines, 0 bootstrap sites, 0 corpus files, flat `check` time; A refuses correct programs and closes no additional silent door | not cast; **would** veto closing the generic-body hole, a post-`mono` layer nobody has costed |
| **llm-ergonomist** | approve | **E** (its Version Q); object to D alone | not cast — a forced `match` is more local, not only more text |
| **spec-warden** | approve | **E3**, the refusal as a clause in § 6's `.is_err()` row: **+17 vendored / +21 real**, paid twice over | not cast; object to any wording that writes `i64??` |
| **ffi-pragmatist** | approve | **A**, conditioned: it would approve E *"only if a measurement shows P2d refused some other way at `check`"* | not cast on the ballot; **would** veto B widened to `[V?]` |
| **historian** | approve (advisory) | E or A **as a deliberate departure**: no shipped compiler or lint refuses a predicate on a nested optional | none |

## THE HINGE, and the condition dissolves on a measurement no seat ran

The ffi-pragmatist approved A over E on one program. Its P2d: a `[Db?]` pool of
real SQLite connections, `find` over it, `hit.must().default(nullptr)` handed to
`sqlite3_close`, **exit 0**, printing a close for a connection that never
existed. It read that as a silent wrong answer E leaves open, and conditioned
its approval of E on that shape being refused some other way.

**It is refused by E.** That seat built one prototype — A — and read E's reach
from the brief rather than from a run. P2d's fourth line is `if hit.is_err()` on
a `Db??`, which is exactly what E refuses. Measured by the coordinator on the
compiler-engineer's own `heroes-E`, all thirteen of that seat's probes:

| probe | base | E | A |
|---|---|---|---|
| `p0_match` — both levels named | 0 | 0 | 0 |
| `p1_outer` — `.is_err()` misread | 0 | **1** | **1** |
| `p2a_must_once` — one peel into a `consumes` | 1 | 1 | 1 |
| `p2b_must_must` — chained, strands the pool at 134 | 0 | **1** | **1** |
| `p2c_default_null` — closes nothing, leaks at 134 | 0 | **1** | **1** |
| **`p2d_default_null_cleanup`** — **the condition** | 0 | **1** | **1** |
| `p3_leak` — never closed | 0 | **1** | **1** |
| `p3b_double_close` | 0 | **0** | **1** |
| `p4_stmt_pool` — §4.19's ladder step 3 | 0 | 0 | 0 |
| `p5_named_pool` | 0 | 0 | 0 |
| `p5b_named_pool_is_err` | 0 | **1** | **1** |
| **`p6_try_nested` — a CORRECT program** | 0 | **0** | **1** |
| `p6a_match_instead` | 0 | 0 | 0 |

**E and A differ on exactly two rows, and both favour E.** `p6` is a correct use
of `?` on a nested value that A refuses and E allows. `p3b` is the one mistake E
lets compile — and it is not silent: it aborts at **134** with *given back that
were never taken*, the runtime ledger doing its job on both platforms. So A buys
nothing E does not already have, and costs one correct program.

**The ffi seat's condition is therefore met by E itself**, and with it the only
dissent on the ballot. The seat's finding stands whole; its ranking rested on a
scope it did not run.

## What was measured, and it settled the sitting's own premises

**R3 is zero, twice over and independently.** The compiler applies no reader to
a nested fallible: `check selfhost/main.hero` is exit 0 with 0 diagnostics under
prototypes of E and of A (compiler-engineer), and the ffi seat's own prototype
read the same over `selfhost/main.hero`, `tests/harness/main.hero`, 54
`examples/` and 289 goldens — **0 sites**. Across 368 corpus files the per-file
record under baseline, E and A is **identical**, `diff` empty. The brief's
inference about the 72 map-read sites is now run: all single-level. Principle 0
does not carry this sitting; the thesis does.

**The `find` door is empty in the corpus.** All six `find(` sites in `examples/`
are in `readings/main.hero` over a `[Reading]`. In `selfhost/` there are **zero**
library `find` calls — the brief's *3 sites* were `hero_map_find(` inside a C
string and two `rfind`s.

**`.is_err()` is the only reader whose RESULT forgets the level.** Of 33 arms in
`selfhost/check/` naming `.fallible`, every one that produces a value keeps the
payload's type: `.must()` and `?` hand back an `i64?`, `.default(v)` types `v`
against the payload, so a wrong depth fails at its next use. `is_err` at
`builtins.hero:378` returns `bool` and did not even bind the payload. That
asymmetry is the whole of defect 050.

**The `.default(0)` noise the defect called *the place to start* is an
accident.** It is loud with `0` — two diagnostics, not the one the brief said —
and silent with `ok(0)`. A type coincidence, never a rule about levels.

**`check` time is flat.** Medians 17.65 s baseline, 17.79 s E, 17.29 s A, all
inside the baseline's own 1.26 s spread, `real`/(`user`+`sys`) ≤ 1.04 in nine
runs.

**The llm-ergonomist wrote the wrong program first, and said so.** Its Task 1
reached for `.is_err()` believing it asked about the stored parse; the seat
records that it doubts it would have caught this unprimed. The defect
reproduced by the one seat whose brief forbids it a compiler.

## The prices, on both instruments

Base 6004 vendored / **7998 real**, digest `7a8fb4400c7ed444`. Every draft was
applied to a copy's `spec/heroes-spec.md` and refreshed on `claude-opus-5` —
the first sitting to do so under the rule written the night before.

| draft | vendored | real |
|---|---|---|
| **E3**, the refusal in § 6's `.is_err()` row | **+17** | **+21** |
| E as a free-standing sentence | +30 | +36 |
| A as a sentence | +31 | +36 |
| panel 158's ergonomist sentence verbatim (writes `i64??`) | +41 | +46 |
| the minimal peel sentence | +9 | +11 |
| B, a clause in § 3's map row | +11 | +11 |
| **Ra** — § 10's *and `for k in sort(keys(m))` walks them in order* | **−18** | **−21** |
| **Re** — § 8's *`break` and `continue` exist.* | **−10** | **−12** |

**E3 + Ra + Re lands at 5993 / 7986: the document shrinks by 11 vendored and 12
real while gaining a refusal.** Both removals verified by the coordinator as true
duplicates: `spec:279` is a compiled fence showing `for k in sort(keys(m))` ten
lines above the prose at `:289`, and § 11 now carries `sort`'s direction (panel
159); `break` and `continue` are in the `Statement` production at `:140` and the
`Inline` production at `:236` before § 8 says at `:229` that they exist.

## The resolution adopted

**R1. Option E.** `.is_err()` is refused on a value whose payload is itself
fallible; `.must()`, `.default(v)`, `?` and `match` are untouched. It is the one
reader whose result type forgets the level, and the evidence is that refusing it
closes every silent program the five seats between them could write, while A
refuses a correct one.

**What conservative would have been: option D**, the peel sentence alone, at
+9/+11. Refused because it changes no behaviour: the wrong program still
compiles and still prints the opposite answer, and the ergonomist measured that
the sentence sits after the table a reader stops at.

**What ROBUST would have been and is not taken: option A.** Recorded because
CLAUDE.md § 4 asks for it: A is the complete form, refusing all four readers.
The measurement above is why it loses rather than a preference — it refuses `p6`
and the golden `a-fallible-type-is-never-written-fallible-twice.hero`, both
correct, and the one mistake it catches that E does not is already an abort at
134.

**R2. The diagnostic does not wait on panel 158 R1.** It names the PAYLOAD,
which is spellable: *"`.is_err()` reads one level, and this value is fallible
twice — its payload is itself a `i64?`, so the failure stored inside would go
unread"*, with a note giving `match`'s two arms. No `i64??` in the message, no
`(i64?)?` needed. **And the premise behind R2 was already false**: the shipping
compiler prints `i64??` today in `discarded_failure`.

**R3. No fix, one note.** Both repairs change what the program means and a
`match` skeleton is multi-line control flow, so a fix here would be a `guess` at
best; `missing_return` in the same file is the precedent for a note and no fix.
**The note must name BOTH routes** — `match` for the outer question, `.must()`
then `.is_err()` for the inner — because the ergonomist predicted that a message
naming only `match` sends at least one repair in five to `.must().is_err()`,
which asks the inner question and aborts on an absent key. A refusal that
relocates the silent error is not a repair.

**R4. The specification gets E3**, the clause in § 6's `.is_err()` row, **paid by
Ra and Re**. Not the peel sentence alone, and not a wording carrying `i64??`.

**R5. The generic-body hole is named and left open.** `is_bad<A>(x: A?) -> bool`
with `A := i64?` passes under both E and A: a generic body is checked once with
a `.generic` payload and instantiation happens after the checker, in
`selfhost/ir/mono.hero`. Live instances: **0** in `selfhost/`, **0** in the
corpus. Closing it is a post-`mono` check, a second layer under §1.7, and the
compiler-engineer would **veto** a resolution that required it without a costing.
Filed, with its measurement, rather than waved at.

**R6. Three defects are booked out of this sitting.**

- **054 — the specification states a balance the runtime stopped keeping.**
  § 13 says *"The owing is counted, so a handle consumed twice hides one never
  consumed."* That entered 2026-09-14; on 2026-09-15 commit `2e7d221c` made the
  counter a **set** (*"the counter became a set"*, its own subject), and
  `abort-handle-given-back-twice.expected` asserts the set's message —
  *the set of live handles did not hold that address*. The ffi seat measured
  P3b: exit 134, the stray reported BEFORE the leak, on both platforms. Verified
  by the coordinator. **It is the tenth correction to this coordinator's briefs
  and the first that is a sentence in the specification.** The seat priced the
  merge that replaces it at **−11 vendored**, which would pay for a future
  addition; the real number is UNRUN.
- **055 — `design.md`'s map section strikes `has(m, k)` on a spelling E removes.**
  `design.md:1388-1390` refuses a presence test because `!m[k].is_err()` is its
  spelling. Under E a `{K: V?}` map has no one-line presence test at all. That
  paragraph owes a correction, and the historian predicts the request for option
  C arrives next.
- **056 — a generic body reads the outer level of whatever it is instantiated
  with.** `is_bad<A>(x: A?) -> bool` with `A := i64?` passes under E and under A,
  measured by the compiler-engineer. Zero live instances in `selfhost/` and zero
  in the corpus, so it is filed rather than repaired; its repair is a post-`mono`
  check the compiler-engineer would veto without a costing. Booked so that R5's
  finding has a number, rather than living only in a synthesis.

## Predictions to score

| seat | prediction | scored by |
|---|---|---|
| compiler-engineer | with E landed: `suite_layout` reads `builtins.hero 378`, `nested.hero` ≤ 60 lines, `check selfhost/main.hero` within 0.5 s of 17.65 s, `nested_read` fires on 0 corpus files; **falsified if landing touches `selfhost/ir/` or `selfhost/emit/`** | this milestone's close |
| llm-ergonomist | under E: 0 silently wrong on Task 1, ≥ 9 in 10 second tries right **if the message names both routes**, else ≥ 1 in 5 land `.must().is_err()`; ≥ 1 in 10 first tries misread *"names both levels"* as a nested pattern | M-thesis-harness |
| spec-warden | E3+Ra+Re lands at **7986 real / 5993 cl100k**; any other number means the landed text is not the text priced; E turns 0 existing programs red | the landing commit |
| spec-warden (panel 158) | *7974/5989 unchanged under a +0 resolution* — **LAPSED**, unscorable: panel 159 moved the base first, with three unrelated sentences | marked here |
| ffi-pragmatist | under the adopted rule `examples/sqlite`, `ledger` and `curl` check at 0 with no edit; `p3b` exits 134 with *given back that were never taken*, never 0 | this milestone's close |
| historian | before the next `m-*` tag, an item asking for a distinct outer-level spelling on a map read (option C) appears in `DECIDE.md` or `DEFECTS.md` | the next tag |

## Process, recorded against this sitting

**The completeness critic did not sit.** It was launched with the briefs and the
five reports, made one tool call and died on a session rate limit. What it was
asked to settle — whether P2d is a silent class E leaves open — the coordinator
measured instead, and the answer inverted the one seat that dissented. **A
sitting whose critic did not run is a sitting with one instrument missing**, and
the four sittings before this one each recorded the critic changing something no
seat had seen. What it was also asked for and nobody has: an eleventh brief
correction, the reachability of the generic hole through the library's own
`find`, and a second reading of the two removals. Filed as owed.

**No seat was killed by the watchdog**, the fifth sitting in a row under the
write-your-report-first rule.

**Every seat built, and two built the same rule independently.** The
compiler-engineer and the ffi-pragmatist each prototyped the refusal, each
measured R3 at zero, and their prototypes disagreed about nothing except the
scope each had been briefed on. The spec-warden refreshed every draft on the real
tokeniser inside its copy. The historian fetched thirteen sources and marked what
it could not verify — including that its own counts came from a summarising tool.

**Ten brief corrections across six sittings, four of them here**: `find(` in
`selfhost/` is 0 library calls and not 3; `.default(0)` fires two diagnostics and
not one; panel 158's *~26 tokens* is +41 vendored and +46 real as landed; and
spec § 13 carries a sentence the runtime falsified two days before the brief
handed it to five seats as fact.

## Author's verdict

**RATIFIED 2026-09-17, as adopted — BY DELEGATION AND NOT BY READING**, under the
author's standing instruction of 2026-09-16. The yes is the assistant's judgement
under an authority the author handed over, on a sitting the author has not read;
recording it otherwise would credit them with a reading that did not happen.

**What the yes settles.** That E lands and A does not, on a measurement that
inverted the dissenting seat's own condition. That the refusal names the payload
and needs no `T??` spelling. That the message must name both repairs or it moves
the silent error rather than removing it. That E3 lands in § 6's row, paid by two
true duplicates, and the document gets smaller.

**What it does not settle.** The generic-body hole, open under every option, with
zero live instances and no costing. Whether the outer question deserves a
spelling of its own — the historian predicts that request arrives next, and it is
a sitting of its own. And the critic's absence: this resolution has not been read
by the instrument that exists to find what five differentiated seats still share.
