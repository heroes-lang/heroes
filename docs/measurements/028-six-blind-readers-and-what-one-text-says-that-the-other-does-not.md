# 028 — six blind readers, and what one text says that the other does not

Date: 2026-09-11 · M-anchored-spec, panel 126 · **the check the author asked for
before the new text may be called the specification** (author instruction
2026-09-10: *once the new specification is produced, compare it with blind
readers to see whether it says the same thing as the one before, that nothing
has been added and nothing invented*).

## Why a reader and not a diff

A diff of the two texts is 24 hunks and says nothing about meaning: the whole
operation moved every paragraph, so `git diff` reports the move and cannot
report a rule. What had to be checked is the one thing a diff cannot see —
whether the two documents state the same language — and the only instrument for
that is somebody who reads them. Six `llm-ergonomist` seats did, each told to
read nothing but the file or files named; that seat's own definition already
forbids design.md, the compiler and CLAUDE.md, so a blind reading is what it
does by construction rather than by promise.

**Neither text was labelled.** They were handed over as `one.md` and `two.md`,
`one` the proposed text, and two of the six were handed the pair in the opposite
order, so that a reader could not favour "the new one" without knowing which it
was.

## Provenance

| what | value |
|---|---|
| the current text | `spec/heroes-spec.md` at `834d804f`, 265 lines, **5378** real tokens on `claude-opus-5`, digest `527e1b762302f9ca` |
| the proposed text | 289 lines, **5575** real, digest `7c49cfcc73cb52d7` |
| readers | six seats: four inventories (two per text), two differential readings |
| the inventory instruction | one line per rule, `kind | the rule in your own words | the document's own words, quoted`; every sentence that states a fact yields at least one line; do not infer, quote |
| the differential instruction | three lists — in one and not two, in two and not one, stated in both with a different meaning — with a moved, reworded, split or merged fact explicitly NOT a difference; a null list is a good answer |
| the matcher | `inventories.py` in the session's scratchpad: every quoted rule looked for in the OTHER text, whole or by its longer half |
| criteria, written before the readers ran | zero rules of the current text absent from the proposed one; the additions exactly those the record names; zero differences of meaning |

## The differential readings

| reader | order handed | in one, absent from two | in two, absent from one | different meaning |
|---|---|---|---|---|
| C | one = proposed | **4** | **0** | 1 |
| D | one = current | **0** | **3** | 0 |

**Nothing of the current text is absent from the proposed one, and both readers
say so from opposite sides.** Reader D, given the current text as `one`, found
its first list empty. Reader C, given the proposed text as `one`, found its
second list empty. That is the criterion the author asked for, met twice.

**The additions both readers found are the ones the record names**: an abort is
carried by no `T?`; `for` walks an array or a `range`; a null `cstr` through
`validated()` fails `null_cstr`. Reader C found a fourth that the ledger row
does not price as an addition, and it is worth its own sentence: *`if` may stand
as a statement*. The current text says *there is no ternary; `if` is an
expression, and so is `match`* in one place and *a `match` may stand as a
statement* in another, and never joins them; the merge that put the two
sentences together made explicit what every example in the document already
showed. No program's meaning moves, and the rule was in the compiler all along.

**The one difference of meaning is a defect of the current text, not of the
proposed one.** Reader C read the gloss *(which does not)* after
`args_checked() -> [str?]` and found that in the current text `exit(code: i64)`
and `validated` stand between it and the `args()` sentence whose *aborts* it
negates, so it can be read as negating *ends the program* or as attaching to
nothing; in the proposed text it follows `args()` directly. The reader found the
current text ambiguous rather than the new text changed, which is the one place
in this record where the re-shaping repaired a sentence nobody had reported.

## The inventories

| reader | text | rules written | quotes not found in the other text |
|---|---|---|---|
| A | current | 280 | 17 |
| B | current | 303 | 21 |
| A | proposed | 296 | 16 |
| B | proposed | 287 | 19 |

**1166 rules were written in all, and every unmatched quote was read by hand.**
Of the 38 quotes from the current text's readers that do not appear verbatim in
the proposed one, every single one is either a code-block line the reader joined
into prose with a slash or a *then* (`record Point / x: i64 / y: i64`, the
`variant Token` cases: both blocks stand unchanged in the proposed text), or one
of the sentences a named merge rewrote:

| the quote | the merge |
|---|---|
| *`a + b` needs both the same type* | D2b, now the operator table's own row |
| *Empty container literals need an annotation* | D13, one sentence with the multi-line rule |
| *folding left with the accumulator first* | D17, inside `fold`'s parentheses |
| *`(function(A) -> B)`, `(function(A, B) -> C)`, `(function() -> C)`* | D18, a row of the type table |
| *A `match` may stand as a statement* · *`if` is an expression, and so is `match`* | D16, one sentence |
| *`find` fails `not_found`* · *`find`, the first it accepts, or an error* | D14, inside `find`'s parentheses |
| *a `nan` aborts*, twice | D11, said once for `< <= > >=` and for `sort` |
| *Multi-line literals separate elements by newline; single-line by comma* | D13 |
| *`to` excluded* | D1, one sentence for `slice` and `range` |
| *`validated(c: cstr) -> str?` (text from C)* | D15, stated where the boundary is |

Of the 35 quotes from the proposed text's readers that do not appear in the
current one, every one is either a priced addition, a merged sentence read from
its new side, or a line of the two new examples that illustrates a rule the
prose already states (`return ok(xs[0])`, `.ok n  => print(n)`, the `test` and
`extern` lines, all of which the current text's own examples show elsewhere).

## What this measures, and what it does not

It measures **content**: the two documents state the same language, and the
additions are the ones the record names. It does not measure **effect**: whether
the new shape makes a model write a correct program more often is Part 11's
metric 2, which has never run, and the ergonomist's blind A/B in panel 126 is
the nearest thing that exists — three tasks written by the author, each program
written twice, once under each text, and compiled by the coordinator. That
experiment is the panel's, this record is the author's check, and neither
pretends to be the other.
