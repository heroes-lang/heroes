# 126 — the document nobody had tidied

Convened 2026-09-10, closed 2026-09-11 · M-anchored-spec · **full lane, five
seats** · status: **provisional — author ratification pending**
(`docs/work/DECIDE.md`)

## The proposal

Verbatim as the seats received it, except that the numbers in point 4 are the
coordinator's measurement of the draft as it then stood; the landing figures are
in § What the sitting changed and in the ledger's row 63.

1. `spec/heroes-spec.md` is re-shaped into **13 numbered sections in reference
   order**: 1 Files and layout · 2 Types · 3 Literals · 4 Top-level declarations
   · 5 Bindings · 6 Failure: `T?` · 7 Operators · 8 Control flow · 9 Functions
   and calls · 10 Strings, arrays, maps · 11 Built-ins · 12 Tests and holes ·
   13 FFI. **No rule of the language changes.** Two sections are new homes
   (Literals, Built-ins), not new content.
2. **Twenty duplications are merged so each rule has one home**: `range`/`slice`
   excluding `to` said once (§11); mixed operands only in the operator table;
   the two `_` paragraphs one; `m[k]` read and write in one sentence (§10);
   `ok()` moves from the types table to Failure; ordering a `nan` aborts said
   once for `<` and `sort` (§7); container literals one sentence; `find`'s code
   inside the built-in list's parentheses; `validated` lives at the boundary
   (§13); `if`/`match` as expressions said once (§8); `fold`'s direction in the
   list; a lend and a lease name one rule (§13); the function type becomes a row
   of the types table. The definition of *abort* moves to Failure, before every
   site that aborts; each site stays with its operation.
3. **Six additions, each priced alone** (vendored delta): the abort definition
   completed (+6); the title in the Reports' form (+2); the loops sentence
   saying what `for` walks and where `range` went (+14); a Failure example
   (+78); a containers example (+56); one generic call (+19). All three examples
   compile and run on today's compiler.
4. **Measured on `claude-opus-5` through `count_tokens`**: 5378 → 5555 real
   (+177); the pure move +51, the merges −71, the additions +197. Vendored
   maximum 4210 → 4376. Ceiling 6144; net of the 60-token FFI floor, 529 free.
5. **The author decided a ceiling of +10% (5916 real) for this operation on
   2026-09-10**, told that Part 11's metric 2 has never run, so the effect of
   the form on a reader is **unmeasured**; the sitting is asked to say so plainly
   and to measure what it can.
6. **Refused by earlier rulings and not proposed**: an EBNF appendix (author
   2026-08-12), type signatures for the built-ins (panel 120), a budget preamble
   (removed in v1), a closed list of aborts (panel 087's veto).
7. **The section numbers become the citation anchors** (`spec § 7`), on CL-069's
   precedent, and two checks follow: `shape` and `anchors`. A number, once
   assigned, never changes.

Open question put to the historian: the Reports put the vocabulary first and
declarations before types; this draft puts Types before Literals. Which order is
the tradition, verified?

## What the sitting changed before adopting it

- **Literals before Types**, sections 2 and 3 swapped (the historian). Every
  Report read puts the vocabulary before the types: Algol 60 §2.5 Numbers and
  §2.6 Strings before §2.8 Values and Types, Pascal §4 before §6, Modula-2 and
  both Oberon revisions §3 before §6, R5RS and R7RS ch. 2 before ch. 3. The one
  exception, R6RS, is not repeated by R7RS. The draft had frozen the reverse
  under numbers meant never to change, and the swap cost nothing on the evening
  before they set. **The second half of the open question is not a tradition**:
  Algol puts declarations last, Scheme puts definitions after expressions, so
  the draft's fourth place for declarations is within the spread of precedent
  and the proposal's premise was half true.
- **The title is *Report on the Programming Language Heroes***, Algol's opening
  with Wirth's noun phrase, the form the historian names as truest to both title
  families; measured at 4377 against 4376 for the proposal's own wording.
- **The abort clause no longer says *nothing catches one*.** The compiler seat
  wrote a two-test program and ran it: `panic: array index out of range`, then
  `FAIL "aborts"`, then `ok "passes"`. Each test is its own process
  (`runtime/parts/failure.c`), so the one place a reader watches an abort happen
  shows it apparently caught, and § 12 says nothing about processes. The clause
  reads **no `T?` carries one**, which is the fact `docs/measurements/022`
  wanted stated and the one the checker enforces.
- **A null `cstr` through `validated()` is stated**, at +4: the ffi seat wrote a
  `getenv` binding from § 13 alone, hesitated there, and measured the runtime's
  answer, `fail("null_cstr", "a null cstr holds no text")`. Neither text said
  it. A reader who guesses *aborts* writes a needless guard; one who guesses
  `.must()` is safe ships a program that dies when the variable is unset.
- **A variant's case is stated**, at +17, and this one came out of the
  ergonomist's experiment: both of its task-2 programs failed at the same line,
  `Token.num(v: pending)`, because **neither text has ever shown a variant value
  being built**. `match` shows a reader how to take a `Token` apart and nothing
  showed how to make one. The clause is merged into the record-construction
  sentence rather than appended, which is panel 122's lesson, and four wordings
  were priced before the cheapest that says the fact was taken.
- **The `anchors` check was repaired on the compiler seat's six probes** before
  it landed: `spec` as a word, so `inspect` and `respect` are not it; a dotted
  number is design.md's; a bare `§ Title` with no document named is judged
  against the spec's sections and the other documents' own headings; a `## `
  inside a fence is not a heading. On its first run over the tree the repaired
  check found **three modules citing a `§ Values` the spec has never had**, one
  `§ Built-ins` that this milestone makes true, and one `§ Rule 3` of a
  harness file's own prose.
- **Every citation of the spec by line number in living code was converted**:
  55 `spec:N` and 21 `spec line N`, of which the compiler seat found **one**
  pointing at its own sentence, plus five heading citations the moves made
  stale. In `tests/golden/`, a record, the old line stays and a dated note at
  the end of the file gives the section.

## The verdict table

| seat | verdict | section | cost / delta, measured | prediction | condition |
|---|---|---|---|---|---|
| historian | **approve** (advisory) | design.md §1.6's Oberon calibration; the Report lineage | 0 for the order, +1 vendored for the title | the next form lands inside one of the thirteen sections, not as a fourteenth (Oberon 2016 folded §12 into §11.1; the Algol revision added nothing at top level) | a verified Report with types before its vocabulary that kept the order through a revision |
| spec-warden | **object** | §1.6's addition clause; §1.2; §1.0 | 5378 → 5555 real at the ballot; merges −71 real pay the move and the four small additions; the two examples, about 150 real, "nothing — and that is a problem" | the text minus both examples refreshes to 5412 ± 12 real | approve if both examples go, or if a seat gives a Part 1 argument per example the panel accepts AND row 63 states the reader effect is unmeasured and the +10% was an author decision, never a payment; **veto** if row 63 says the merges paid for the examples or names metric 2 as payment |
| compiler-engineer | **object** | §1.7, §1.1 | compiler 0 lines of code (one comment); harness +269 lines; 76 line citations and 5 heading citations to convert | at the tag, `git diff 834d804f..tag -- selfhost` outside comments is empty but for one diagnostic string | the six probes pass, fences skipped, the abort clause reworded, the three tables applied |
| ffi-pragmatist | **approve** | §1.11, §4.19 | 0 at the boundary; both bindings compile from § 13 alone | `examples/ledger/db/sqlite.hero` and `examples/gallery/13-lease.hero` compile unchanged; a reader writing `validated(s: x)` hits `wrong_label` with a certain fix | object if the lend's argument-only rule were dropped from the merged sentence; **veto** if a widths or `owned` line changed |
| llm-ergonomist | **approve** the shape; **keep** both examples | the objective, judged on the two texts alone | six programs written, three tasks × two texts; three lines diverge | one of the two task-2 programs fails on its `fail` line; both share the verdict on `Token.num`; on map tasks the new text beats the old by ≥15 points of first-try rate; silent-error rate within noise | cut an example if the compiler rejects a line it teaches; indifference if ten or more tasks show no first-try penalty on map and failure tasks; **veto** if variant construction, array element assignment or `sort`'s direction is ever specified away from the construct it governs |

## The experiment, and what the compiler said about it

The ergonomist wrote the author's three tasks twice, once under each text, and
the coordinator compiled all six with the compiler built from the seed. **Two
tasks of three compile and run identically under both texts**; the third fails
under both, at the same line, for the same reason.

| task | under the proposed text | under the current text |
|---|---|---|
| word count over a map | clean, `5 the / 3 fox / 3 quick …` | clean, byte-identical output |
| an arithmetic lexer | `error[variant_in_value_position]` at `Token.num(v: pending)` | the same error at the same line |
| the nearest points | clean, `(-1, 0) d2=1 / (1, 1) d2=2 / (2, -2) d2=8` | clean, byte-identical output |

**Its first prediction is falsified, and informatively.** It predicted exactly
one of the two lexers would fail on its `fail` line, the positional one under
the text with no example. Neither did: measured after the run,
`fail("a", "b")` and `fail(code: "a", msg: "b")` are **both** accepted, while a
user function with two `str` parameters refuses the positional form, and
`xs.slice(1, 3)` is accepted where `range(1, 4)` is refused. The seam is the
compiler's own built-ins skipping the check that reads a signature; the seven
built-ins the document says are written in Heroes obey it. That is a discrepancy
between § 9's rule and the compiler, filed as **defect 025** rather than
repaired here, because a diagnostic class is a panel path of its own. So the
Failure example did not settle a coin flip: both faces were already legal, and
what it teaches is one legal form.

**Its second prediction held and produced the sitting's cheapest clause.** Both
lexers failed on variant construction, a rule neither text has ever stated, and
the clause that states it is +17 vendored.

**Its third and fourth predictions are metric 2's** and go to the ledger as
registered observations, not as payment: no instrument scores a first-try rate
today.

**The hesitation points are the seat's real deliverable**, and three of them are
holes in both texts rather than in either: `sort`'s direction, which is the only
place in three tasks where a plausible mistake compiles and prints a silently
different answer; array element assignment, `xs[i] @ v`, given nowhere while
`m[k] @ v` is given; and whether `main` may be fallible, which makes `?`
unusable in the one function every program has. None is caused by the
re-shaping, none is closed by it, and each is queued rather than bought here.

## Disagreements, stated plainly

The warden objects to the two worked examples as additions nothing pays for, and
its arithmetic is right: they are 77% of what the additions cost and no
instrument scores them until metric 2 runs. The ergonomist, which is the one
seat whose verdict is an experiment, says keep them, and its evidence is the
search cost it measured: the map task needed one section of the proposed text
and three of the current one, whose own container section never mentions reading
a map. The two seats are not measuring the same thing, and the sitting does not
pretend they agree. What resolves it in the adopted resolution is neither
argument but the author's decision of 2026-09-10, recorded as a decision and not
as a payment, exactly as the warden's condition asks.

The compiler seat's objection was instrumental and was met inside the sitting.

## Resolution adopted, provisional

**The most robust and complete route, not the cheapest**: the thirteen numbered
sections with Literals before Types, the twenty merges, all six additions plus
the two the seats' own experiments produced, the numbers as the citation
anchors, the two checks that keep the form, the rule file that says it, and the
81 citations converted. The two worked examples **stay**, on the author's
decision, with the ledger row saying in its own words that their effect on a
reader is unmeasured and that the merges did not pay for them.

**What conservative would have been**, recorded so the author can choose it: the
reorder and the merges alone, no numbering, no additions, at 5358 real tokens,
twenty under today. It was refused because a form nobody can cite is the form
that rots, which is CL-069's finding about this project's own contract.

## Author's verdict

**Ratified as adopted, 2026-09-11** (the author's word, `ratifica 126`). The
thirteen numbered sections in reference order with Literals before Types stand;
each rule keeps one home; the numbers are the citation anchors and a number once
assigned never changes; the eight additions stand, **the two worked examples
included**. The conservative route recorded above, the reorder and the merges
alone at 5358 real tokens, was put as the alternative and declined.

**What the ratification settles about the payment**, since that is what the
sitting split over: the two examples stay on the author's decision of 2026-09-10
and on this word, and not on a measurement. Row 63 of the ledger says so in its
own sentences, names no instrument that does not exist, and does not claim the
merges paid for them. The spec-warden's condition is met in the form it asked
for, which was a record that tells the truth about what pays; its objection
stands on the record beside the ratification rather than being smoothed away.

**What it does not settle.** Defect 025 stays open, so the milestone stays
untagged (CLAUDE.md § Verification), and the three holes the ergonomist found in
both texts stay filed at M-check-completeness.

## Predictions to score

| seat | prediction | scored at | state |
|---|---|---|---|
| historian | the next form lands inside one of the thirteen sections | the first spec amendment that adds a construct | open, checkable by the `shape` check |
| spec-warden | the text minus both examples refreshes to 5412 ± 12 real | the day the examples are cut | **not renewed** (panel 046 R2): the ratification of 2026-09-11 keeps them, so the counterfactual has no milestone to be scored at and stands as an observation |
| compiler-engineer | the selfhost diff outside comments is empty but for one diagnostic string | the `m-anchored-spec` tag | open |
| ffi-pragmatist | the two example programs compile unchanged | the tag | open |
| ergonomist 1 | one of the two lexers fails on its `fail` line | this sitting | **FALSIFIED**, measured: neither does, and defect 025 is what the measurement found |
| ergonomist 2 | both lexers share the verdict on `Token.num` | this sitting | **HELD**, and it bought the variant clause |
| ergonomist 3 | on map tasks the new text beats the old by ≥15 points of first-try rate | M-thesis-harness | registered as an observation; metric 2 does not exist |
| ergonomist 4 | silent-error rate within noise between the texts | M-thesis-harness | registered as an observation |

## What this sitting did not do

It did not repair defect 025, and it did not close the three holes the ergonomist
named in both texts. It did not measure the effect of the new form on a reader,
because the instrument for that is Part 11's metric 2 and it has never run: the
six programs above are the nearest thing that exists, and three tasks are not a
rate.
