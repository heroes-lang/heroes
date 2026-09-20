# The default flips, and robustness enters the Zen

2026-09-21. M-declared-extents step 24, commit B of panel 171's landing, and
the author's ruling of the same evening written where they asked for it.

## The decisions

| | |
|---|---|
| date | 2026-09-20 |
| decision | **a lend reaches only a `lent` parameter of an `extern` function**: `lend_kept` at an unmarked one, `lend_needs_a_header` into a function of the program, and the extent question stays first for a field so that one lend gets one message; § 13's lend sentence is REWRITTEN to the warden's M2, `CParam` gains `[ "lent" ]`, the document's own fence carries the word; **27 marks on 19 functions across three trees** and 8 sites in three shipped examples, five of them marked and three turned into a lease where no one word is true of the parameter |
| reason | the author's ruling, given in conversation after the cost was measured (33 ns per unmarked call at worst, zero when marked, `docs/measurements/038`): Heroes is young, nobody depends on its examples yet, and robustness is a principle of the language, so the assumption goes the robust way — a C parameter is assumed to keep what it is handed until its declaration says otherwise |
| design.md § | §1.12, §4.19, §4.17, §1.6 |
| panel | 171, ratification pending on the word and the rule; the direction is the author's |

| | |
|---|---|
| date | 2026-09-20 |
| decision | **the Zen gains a law and keeps twenty**: law 19, *Never crashing in the same car: what C might keep, Heroes assumes it keeps*, after the law that names C; laws 11 and 12, the two about the hole, become one — *Unless you mark the gap with ???: an honest hole beats a confident guess* — and the rest renumber |
| reason | the author's instruction, in their words: robustness is a principle and goes into `import this`; a new law rather than a widened one; twenty stays twenty by merging a redundant pair; and *the laws must all be of roughly one length, for readability*, which set both new lines at 74 characters against a longest existing law of 78 |
| design.md § | §1.12, Part 0 |
| panel | none: the Zen is prose and packaging, amended by author instruction (CLAUDE.md § 4) |

## What the author decided and what was delegated

The DIRECTION was the author's, put to them in plain words and ruled on with one
condition, the cost, which measurement 038 met before the ruling. The WORD, the
SENTENCE and the LANDING were panel 171's, adopted as the provisional default
and queued as `panel 171` in `docs/work/DECIDE.md`. The Zen's two lines were
put to the author with the lengths measured from the source string; the author
chose the new law over a widened one and set the length condition; the exact
words are the assistant's recommendation, and the author can overturn them at
the reading.

## What one lend now meets, and why one message

Three questions, in `check/lend_extent.hero`'s `landed_on_a_header`, in the
order a reader meets them and stopping at the first that fails: is the callee
a function of an `extern` group at all; for a field's address, does the
parameter declare its extent; does the parameter say `lent`. The sitting's
engineer fired the extent and the word together on a bare `ptr` parameter;
this commit asks the extent first and has its note name both words, because a
`ptr` parameter is either fully marked or not — `lent` with no extent beside it
is `lent_shape`, dead text, the sitting's gap 1.

## The three gaps the sitting named, closed here

`lent` on an uncounted `ptr` is `lent_shape`. `lend_kept`'s notes are shaped by
the lend: `.cstr()` is offered the lease, `.ptr()` is told a field has no lease
and what exists instead, and the spelling of the mark carries the sibling's
own name. Both notes carry *if C frees what it is handed, neither route works*,
because following the first note onto defect 070's fourth shape was `check` 0
and exit 134 with an empty stderr.

## What the shipped examples do

`examples/sqlite/` and the ledger mark `sqlite3_open`, `sqlite3_exec` and
`sqlite3_prepare_v2`, which the ffi seat measured copying. `sqlite3_bind_text`
keeps or copies by its fifth argument, so no one word is true of it: it stays
unmarked and `bind_text` hands the bytes over as a lease it releases the
moment the call is back, right under `SQLITE_TRANSIENT`. `curl_easy_setopt`
keeps or copies by its OPTION, so the same: unmarked, and the URL goes over as
a lease. A declaration that says `lent` about a function that keeps is the
mistake D's WASI binding made, and these two are the shape where nobody can
say it truthfully.

## What closes and what stays

Defects 066 and 068 close: both reproducers lend into an unmarked parameter
and that lend is refused at `check`. Defect 070 stays exactly where it was and
its fourth shape becomes `lend_kept`. The pointer C hands back is written down
as what the flip leaves standing, in the sitting and in the checker's own
tests.
