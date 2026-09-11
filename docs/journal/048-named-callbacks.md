# 048 — M-named-callbacks

Closed 2026-09-11. One sitting, panel 130, on the route panel 129 recommended
against and the author asked for anyway.

## Goal

Close the role inversion through a GENERIC callback, which M-labelled-types'
rule could not reach. `fold`'s callback is `(function(B, A) -> B)`, two distinct
letters, so §4.9's criterion — the declared type, never the monomorphised one —
leaves it outside the same-typed rule however a call instantiates it. A program
that read its two parameters the other way round compiled and printed `cba` where
it meant `abc`, at exit 0, on every compiler this project had built.

## What surprised

**Half of what the earlier sitting priced was unnecessary, and one measurement
said so before a line was written.** Panel 129 costed this at four parts and
objected to two of them, a declared function naming its parameters always and a
widening relation to carry that. Both rest on the premise that a role inversion
is invisible at any pair of parameters. It is not: with different types the swap
changes the type, and `fold` at `B := str, A := i64` with the two transposed is
two `type_mismatch` on the old compiler. **What is silent is the same-typed case,
and there the names already existed.**

**The library had been warning about this exact failure since panel 029, in
writing, inside its own source.** `library_source.hero`'s comment above `fold`
says that with an accumulator and an element of the same type — *`sum`, `concat`,
`max`, every string builder* — either order type-checks and the program prints a
different answer. It stopped being a warning and became a check.

**Five comment lines cost six rows of every emitted C file.** The strings in
`library_source.hero` ARE the library's source, so a comment among them shifts
every `#line "<heroes library>"` in every program. The explanation moved out to
the module's own doc and the C stopped moving.

**A rule can be measured the way a metric is, and it should be.** The first
version fired only where the callee reused the type's own word for another
position. It reads well. Against the seven callbacks this repository hands to
`fold`, each transposed, it catches **2 of 7** — the five it misses use their
authors' vocabulary. The author was given both numbers and took agreement, which
catches 7 of 7 and costs a permanent naming mandate.

**And the honest half: the mandate does not bind where the callee's own two
parameters differ in type.** `tally(item: str, acc: i64)` has its names fully
inverted against `fold`'s and prints `123` at exit 0. The spec-warden found it
against the built compiler and refused to write the sentence that would have
denied it.

## What broke and why

- **A written function type naming a position nothing can be confused with was
  UNINHABITED.** A declaration names only a shared pair, so `twice(n: i64)` has
  the name erased, and `fits` refused two label runs of different lengths. The
  warden found it by building its own compiler and running the program; nothing
  in the coordinator's own tests could have.
- **`cp` over a running binary gets it killed.** `./heroes` answered SIGKILL on
  every program until it was `rm`ed first — macOS invalidates the signature of a
  binary overwritten in place. Twenty minutes were lost reading it as a compiler
  bug.
- **The name check stacked an eighth diagnostic onto three type mismatches**,
  because it ran after the walk regardless of what the walk had said. It is
  guarded by agreeing types now.
- **A golden was deleted and had to be restored.** `tests/golden/` is
  append-only: a case whose rule is gone is MOVED and corrected underneath with
  its date, not removed.
- **design.md carried two false sentences at once**, both written in this
  session, and both found by seats rather than by their author.

## What landed, and what carried forward

**The rule.** A function type may name any parameter; where two share a type it
must. The names of a function handed to a callback type that names its
parameters must agree with them, checked in `check/generics.hero`'s `bind` —
the one path that never compares whole interned ids — as `error[callback_name]`,
with no fix and the reason stated.

**The number.**

| what | narrow reading | what landed |
|---|---|---|
| the seven `fold` callbacks, transposed | 1 killed... **2 of 7** | **7 of 7** |
| correct programs refused | 0 | 0, after six renames |

**The document.** 5662 real, **−1**, digest `6e82771d0dbabc31`, ledger row 67,
paid by § 3's two extra arities at −20 measured alone.

**What carried forward.** A callback whose own two parameters are different types
is unbound by the mandate and can have its names inverted in silence. A user's
own generic higher-order function whose callback type names nothing is unchecked.
Both are named in the sitting and in design.md rather than left for a reader.
