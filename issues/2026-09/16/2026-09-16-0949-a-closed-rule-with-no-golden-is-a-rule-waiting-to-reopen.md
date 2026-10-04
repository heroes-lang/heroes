# A closed rule with no golden is a rule waiting to reopen

2026-09-16. M-check-completeness opens, and the first thing the milestone's own
opening measurement found was that one of its four faces had been closed for a
month by a sitting held the same day as the one that named it. No sitting here:
this is a measurement and the case that should have followed 084's ruling.

## What the measurement said

The milestone's opening sentence — *"`heroes check` accepts `first([P(x: 1)])`
at exit 0 and `heroes build` refuses it"* — is **false**. Measured on a compiler
built from the seed: `check` **1**, `build` **1**. Panel 084 R1 took `generic`
off `is_refusable`'s exempt list on 2026-08-16, the same day panel 082 R3 wrote
the sentence, and nothing in a living document was told.

The other three faces were re-run rather than carried, and each holds exactly as
written. The map through `tally<K>` keyed at `f64` by the call is `check` 0,
`build` 0, run 0 **printing 1**, against `check` 1 for `m: {f64: i64}` written
down. The `partial` group record through `same<A>(x: A, y: A)` is `check` 0,
`build` 0, **run 134** — `panic: a partial record has no structural equality` —
against `check` 1 for `a == b` written down. The nested fallible is `check` 0,
`build` 0, run 0 printing 1.

## The decision, and it is not about `sort`

The closed rule had no golden case. Its only witness was
`selfhost/check/ordering.hero:186-188`, a unit test that interns a `.generic`
type id and asserts `is_refusable` on it. **That watches a table entry where the
rule is about a program**, so the refusal could stop firing with every suite
green and the check/build gap would reopen in silence — which is the failure
this milestone exists to prevent, one level up, aimed at itself.

So: **a rule that closes a check/build gap is pinned by a program, and the unit
test stays as the second witness rather than as the only one.**
`tests/golden/check/sort-through-a-type-parameter.hero` pins `sort(xs)` and
`xs.sort()` both, because UFCS makes them one rule; carries a control generic
that does not sort, so a widening arrives as a new diagnostic rather than as
silence; and instantiates the refused generic at `i64`, which sorts and is
refused all the same, so *the body decides, never the call* is a program and not
a sentence. `check` 120 to 121, `annotations` 157 to 158, everything else green.

The reason is CLAUDE.md § RUN IT applied to a test rather than to a claim: an
assertion about a table row is an inference about the programs that reach it, and
the connective is *so*. design.md §1.12 is what makes it worth the case — a gap
where `check` says yes and the program aborts at run time is the class this
milestone is named for.

## What the entry deliberately does not claim

That the sortable question is finished. The llm-ergonomist's veto at panel 082
stands: the line `sort(xs)` is still undecidable from the line plus its
signature, and lifting that needs constraints on generics, which is the author's
trade. What closed is the **gap between the two commands**, not the ergonomics of
the body.
