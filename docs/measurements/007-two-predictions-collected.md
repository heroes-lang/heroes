# 007 — the two collectable predictions, collected

Date: 2026-08-13 · at M-program-corpus close · **the first time this project has
scored a registered prediction that bought a spec clause.**

## Provenance

| what | value |
|---|---|
| compiler | `5ecbc98` plus this commit |
| spec | sha256 `8825dc10f94b678d…` — measured max **2974**, unchanged by this run |
| corpus | `examples/` — **45** programs, and for the first time it contains bitwise code |
| instruments | `heroes mutate --survivors`; one blind `llm-ergonomist` run against `spec/heroes-spec.md` alone |
| command | `heroes mutate --survivors`, and the writing experiment below |

## Why now, and what was owed

Panel 046 R2, ratified 2026-08-13: the six spec rows bought with a registered
prediction are **re-decided at M-program-corpus** — scored, or marked `lapsed`
with the clause re-argued under the removal branch, never renewed with a new
milestone name. R1's test is whether the prediction named an instrument that
existed on the day it was registered.

Four of the six named **metric 2** — first-try rates, first-try failures, the
first metric-2 harness run — and the author scheduled metric 2 at
M-selfhost-fixpoint (author decision 2026-08-12). They are **lapsed**: `2588`,
`2745`, `2768`, `2959`. Their clauses stay in the spec, because R1 governs what
may be *offered* as payment and is not retroactive, and each is back in
`DECIDE.md` under the removal branch.

Two named instruments that exist. They are scored below, and **both found
something the row itself did not predict**, which is the argument for collecting
a prediction rather than filing it.

---

## `2627` — panel 037's clause on what a string costs

Bought at **+39**: `join`'s shape at +23 on the llm-ergonomist's ask, plus 16 for
`push`'s copy, *"without which the sentence would send a reader to build the
`[str]` quadratically instead of the `str`"*.

The prediction, verbatim:

> In the next spec-only writing experiment, **no program builds a long string by
> repeated `+` in a loop**, and no reader records the hesitation this sitting
> measured. If a program still does it, the clause bought nothing and is the
> first candidate at the next budget squeeze.

### The experiment

One `llm-ergonomist`, given `spec/heroes-spec.md` and nothing else — no design
document, no examples, no compiler source. Three tasks chosen because each ends
in a built string and only one of them is a join by shape: a table renderer
(needs a captured separator), a banner (needs N copies of one character), and a
CSV line (a pure map-then-join). Then four questions, with the instruction that
a null answer is the answer and a flattering one makes the experiment worthless.

### Result: the first half held, the second half is falsified

**Held.** No program built a long string by repeated `+` in a loop. The judge
considered it exactly once — the banner's rule of `=`, the one place with no
separator to join on — and rejected it *before writing it*, naming the clause:

> My first instinct on "make a string of N identical characters" is `s @ s + "="`
> inside a loop over `range`. I rejected it before writing it, for one reason:
> *"`+` on `str` copies both sides and `push` copies the array, so accumulating
> either in a loop is quadratic. `join` is linear."* That sentence is … the only
> sentence in the document with a cost claim in it, so it is **loud**.

That is a clause doing the job it was bought for, reported by a reader who did
not know it was being tested.

**Falsified.** The second half asked that no reader record a hesitation. This
reader recorded one, unprompted, and it is worse than the one panel 037 measured:

> What I actually wrote in 1 and 2 is `push`-in-a-loop, which the same sentence
> calls quadratic too. The warning redirected me from one quadratic accumulation
> to the other and **I noticed only afterwards**. The sentence steered the
> *syntax* I used, not the complexity of the program.

The extra **16 tokens** were spent precisely to prevent this. Panel 037's own
words: *"a reader told to build the `str` with `join` and left to build the
`[str]` with `push` has moved the quadratic cost rather than removed it."* The
clause names `push`'s copy, the reader read it, and did it anyway.

### Why, and it is not the wording

The reader gave the reason, and it is a **language gap** rather than a spec gap:

> The shape I wanted was `map(rows, row => join(row, separator))` … I could not
> write it: the document gives function values and generic `map`, but shows **no
> lambda syntax** anywhere, and `separator` has to be captured. A top-level
> helper cannot see `separator`, and there is no partial application.

> With no lambda, `map` cannot capture `separator`, and with no `repeat`, there is
> no way to make N copies of `"="`. So in two of three programs the document told
> me the idiom is quadratic and then **left me no other way** to build the input
> `join` needs.

The third program — the only one whose transform captures nothing — is the one
line with no accumulator in it: `join(map(cells, quote), ",")`.

### The verdict on the row

The clause is **not** the first candidate at the next budget squeeze, because its
first half is the half that was in dispute and it held under a real reader. Its
second half is falsified, and what that buys is a question for a later sitting
rather than a deletion: **a reader who is told a loop is quadratic and has no
non-loop available does the loop.** Two candidate answers are now in `DECIDE.md`
— a way to build N copies of a `str`, and a way for `map` to see a local — and
both are Part 7 questions, not spec-wording questions.

### Four other things the same run reported, unasked

They are recorded because a blind reader is expensive and its by-products are
free:

1. **`push`'s signature is a guess — and it found a defect.** The reader wrote
   `push(@lines, x)` and said so: *"That is inference from the shape of a caveat,
   not from a statement, and I want it on the record as a guess."* Its tie-breaker
   was the spec's own sentence that UFCS does not apply to an `@` first parameter,
   and `xs.push(x)` is the case where that would bite.

   `push` returns a new array, so the guess is wrong. It failed to compile — but
   on **`discarded_value`**, one line later and about the returned array, saying
   nothing at all about the `@`. A *user* function has answered
   `marker_mismatch` since M-generics-library; a built-in answered nothing,
   because `types/apply.rs` checks the marker per argument and built-ins do not go
   through it. Fixed in this commit, one arm in `types/calls.rs`, and the case is
   `tests/golden/check/fixedbugs-at-marker-on-a-builtin.hero`. The `@` is a claim
   and not a decoration: `push(@lines, x)` says the argument may come back
   changed, and it may not.

   That is the sixth compiler defect this milestone has found, and the second one
   found by an instrument rather than by a program — the blind reader's careful,
   wrong inference is exactly what a marker check exists to answer.
2. **`s[i] == ','` was written first and caught on re-reading.** The reader
   indexed a byte and compared it to a character literal, then corrected itself
   against three separate spec sentences. It is glad the language refuses it.
3. **Whether a function's last expression is its value is undecided in the
   document.** The reader wrote `return` everywhere *"rather than find out"*.
4. **Multi-line container literal layout is stated and never shown.** The reader
   dodged it by keeping the literal on one line — in the one program whose data
   is a table.

---

## `2675` — panel 040's bitwise set

Bought at **+48**. The prediction, #2 of three, and the panel calls it *"the
falsifier for this whole change"*:

> The six operators produce **no new surviving-mutant class** in metric 3 — no
> `heroes mutate` operator's survivor list gains an entry whose mistake is a
> bitwise operator misread as its boolean twin (`&` for `&&`, `|` for `||`).
> Falsified by any such survivor.

Checkable at *"the first `mutate --survivors` run over a corpus containing
bitwise code"*, and until this milestone there was none: `examples/` had no
bitwise operator in it at all, only `|` as a pattern separator. `examples/logs/`
now carries a level mask — `|` to add a flag, `&` to test, `~` to take away, `^`
to toggle, `>>` to count, and `<<` to declare the bits — which is the flag union
the +48 was argued from in the first place.

### Result: held, and the way it holds is the finding

**Held.** 892 survivor lines over 5798 mutants, and not one of them is a bitwise
operator swapped for its boolean twin.

**And it could not have been otherwise.** The twelve operators are `swap-args`,
`drop-case`, `forget-at-decl`, `mutate-undeclared`, `typo-ident`, `typo-code`,
`wildcard-variant`, `positional-named`, `mix-int-float`, `shadow`,
`drop-question`, `typo-digit`. **None of them substitutes `&` for `&&`.** The
prediction named a live instrument — `heroes mutate` has existed since
M-generics-library — and the instrument has no arm for the question it asks.

That is a finding about panel 046's own rule, and it sharpens it: R1 asks that a
prediction name an instrument that **exists**, and this one did. What it did not
ask is that the instrument be able to **produce the observation**. A
`boolean-twin` operator is now in `DECIDE.md`; until it exists, this row is
scored `held` with the vacuity written beside it rather than quietly banked as
evidence.

### A by-product

`typo-digit` produced **0 mutants over the whole corpus** when the §11 sweep
looked at it, and was queued as an operator that measures nothing. With
`mask.hero` in the corpus it produces **12**, of which 0 are killed — `constant
BIT_TRACE: i64` with body `1` mutated to `2` survives, as it must, since nothing
in the language can know which integer was meant. The operator now measures
something, and what it measures is a hole.

---

## The corpus, before and after the mask

| | before | after |
|---|---|---|
| programs | 44 | 45 |
| mutants | 5591 | 5798 |
| killed (`check`) | 5254 (95%) | 5450 (95%) |
| killed (`--permissive`) | 4668 (84%) | 4858 (85%) |
| `typo-digit` | 11 mutants | 12 mutants |

Per panel 011 the headline is never pooled; the rows are the measurement.
