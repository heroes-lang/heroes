# 130 — the name a generic callback could not say

Convened and closed 2026-09-11 · M-named-callbacks · **full lane**
· status: **provisional — author ratification pending**
(`docs/work/DECIDE.md`)

Panel 129 classified the `fold` role inversion as the price of design.md §4.9's
criterion rather than a defect against it, and offered the author three ways to
treat it. **The author took none of them and asked for the route the sitting had
recommended against.** This sitting judged what that produced, and it changed
three times while it sat: twice because a seat sent it back and once because the
author chose between two numbers.

## What the earlier pricing rested on, and why half of it was unnecessary

Panel 129 priced the route at four parts, and objected to two: a declared
function carrying its names ALWAYS rather than only at the confusable positions,
and a widening relation to keep the tree working under that. **Both rest on a
premise that does not hold, and it was measured before anything was written.** A
callback's roles can be read the other way round, and the compiler sees it only
when the two parameters end up the SAME type; with different types the swap
changes the type and `bind` already refuses it — `fold` at `B := str, A := i64`
with the two transposed is two `type_mismatch` on every compiler this project has
built. And when they collapse to one type, they share a type, so
M-labelled-types' `declared_labels` already names them.

So what was missing was only that `fold`'s own type could not SAY what it wanted,
and that the generic path never compared names even when both sides had them.

## What the seats found

**compiler-engineer — veto, and the measurement that changed the milestone.** It
attacked eleven shapes and **could not falsify the premise**. What it did falsify
was the conclusion. The rule as first built fired only where the callee reused
the type's OWN word for another position — panel 127's `elsewhere` test applied
to a value — and the seat measured it the way the class deserves: **transposing
each of the seven callbacks this repository hands to `fold` is caught 2 of 7.**
Panel 129's comparable number was 6 of 6. The five it missed call their
parameters `a` and `b`, `so_far` and `one`, `total` and `i`. It also found that
an earlier version had refused three CORRECT programs, including design.md:3085's
own *spuriously triggers* case; that `layout` was red on two files, one of them
the ceiling panel 129 had added nineteen hours earlier **as a stop on growth**;
that `check/table.hero`'s own comment claimed one relation where there were now
two, *a widening, the word panel 129 used for what this route was supposed to
avoid*; and that `grep -rl callback_name` found three source files and **no
golden and no annotation**. Its ground: design.md:1439 and design.md:1456 were
both false against the built compiler and neither was amended.

**spec-warden — veto, lifted, then object.** Three rounds, and it rebuilt its own
compiler before each because the tree moved under it twice; it caught its own
stale measurement with `stat` rather than by trusting its note. Its first finding
is the one that mattered most: **a written function type naming a position
nothing can be confused with was UNINHABITED.** A declaration carries a label run
only where two of its own parameters share a type, so `twice(n: i64)` has type
`(function(i64) -> i64)` with the name erased, and `fits` refused two runs of
different lengths outright — `(function(item: i64) -> i64)` could be satisfied by
no function in the language. It found it by building a compiler and running the
program rather than reading for it.

Its veto lifted on the repair. Its final **object** is a second false sentence,
this time in design.md and this time mine: *a function passed to `fold` names its
two parameters what `fold` names them* is false, because `tally(item: str, acc:
i64)` has its names fully inverted against the type, puts none in its own type,
and **prints `123` at exit 0**. It refused to write the unscoped spec sentence for
the same reason, and priced the true one at three words more.

**llm-ergonomist — approve, and the best argument for the rule was not available
when the author chose it.** Reading the specification alone, it wrote the three
`fold` tasks and typed `acc, item` three times **without making a naming
decision**: left to itself it would have written `total`/`n`, `so_far`/`word` and
`best`/`x` — *three vocabularies for one built-in*. And it reports the whole
signature-order class closed: a swapped `fold` callback cannot compile in either
direction, by name where the two share a type and by the ordinary type rule where
they do not.

Two findings it left. **The message was broader than the document**: `spec § 9`
scopes the rule three times to two parameters sharing a type, and a message
stating it for any named callback type would ask a reader to know every callback
type their function will ever be handed to. The message carries the scope now,
and that was its stated veto ground, left open rather than exercised. And the
silence it names is not this milestone's: the document gives the ORDER and the
NAMES and withholds the MEANING — *the word "accumulator" does not appear in the
document and `acc` is never expanded* — so the label is fixed and checked and the
meaning is still guessed. It is `sort`'s direction all over again, which panel 126
already scheduled.

## Resolution adopted, provisional

**The rule the author chose, and the sitting recommended against.** The names of
a function handed to a callback type that names its parameters must AGREE with
them. Measured: **7 of 7** transposed callbacks caught, against 2 of 7 under the
cheaper reading. The price is a permanent naming mandate on `fold`'s callers and
six programs renamed once each, and the record calls it a mandate rather than a
detector in three places — design.md, the diagnostic's own module doc, and the
golden.

Four repairs from the seats, all landed:

1. **`fits` blank-fills a missing run**, so a type that names nothing fits
   wherever the expected type's names get filled in, and the two paths are one
   rule. Without it every such annotation was uninhabited.
2. **The name check is guarded by agreeing TYPES.** A callback whose parameter
   types disagree is reported by the walk, and a name on top of that was an
   eighth diagnostic for one mistake.
3. **The golden is moved, not deleted** — `tests/golden/` is append-only — and
   corrected underneath with its date: it pinned a refusal that no longer exists,
   and it is a `run/` case now, pinning that such a type compiles.
4. **`callback_name` has a golden and an annotation**, and its module doc says
   why no fix is offered: the repair is a parameter of another declaration, which
   `--apply` does not reach, and a rename would leave the defect standing, which
   is panel 127's own test for a guess.

**The document**: § 9 loses *and no others*, false since the refusal behind it
went, and gains *A function that has such a pair names it as the type does* —
three words of scope that are the only reason it is true. § 11's `fold` entry
carries the names in mandate register, without which the warden refused the § 9
clause. § 3's two extra arities pay for both, **−20 real measured alone**, and
the landing is **−1 real**: 5662, digest `6e82771d0dbabc31`, ledger row 67, and
row 66's *paid by nothing* does not repeat.

**What conservative would have been**, recorded so the author can still choose
it: the 2-of-7 reading, which costs nothing and renames nobody.

## Author's verdict

**Pending**, and narrower than the sittings before it, because the author has
already decided the substance. Asked at panel 129's ratification what to do with
the `fold` case, they said build the route that closes it; asked mid-sitting
whether to keep the cheaper 2-of-7 reading or take agreement at 7 of 7 with a
naming mandate attached, they took agreement. **What is queued is the sitting's
verdict on what that produced**, which is not the same question: two seats sent
the work back after the decision, and what landed is not what they were handed.

**What a yes settles**: the mandate, the document at −1 real, and the tag.
**What it does not**: a callback whose own two parameters are different types,
and a user's own generic higher-order function. Both are below.

## What this sitting did not close

**A callback whose own two parameters are different types.** Its declaration puts
no names in its type, so the mandate does not bind it, and `tally(item: str, acc:
i64)` runs at exit 0 with its names inverted against `fold`'s. Closing it needs a
declared function to carry its names ALWAYS — panel 129's expensive route, whose
two objected-to parts this sitting showed were unnecessary for the case that was
closed and would be necessary for this one.

**A user's own generic higher-order function.** The library closes its own
callbacks by naming them; the language does not close everybody's.

**And the cost is not small.** `+87` net `code_lines` on top of M-labelled-types'
432, all frontend, and a naming mandate that every future caller of `fold` pays.
