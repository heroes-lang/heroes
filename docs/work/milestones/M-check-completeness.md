# M-check-completeness — what `heroes check` accepts, `heroes build` compiles


**Scheduled, no warrant**, and the row says so on purpose. Principle 0 holds this
one: panel 082 R3 ruled the direction in 2026-08-16 and three seats measured that
nothing on the closure list needs it. What put a row under it on 2026-09-04 is
that its work had been parked on a `grep` for nineteen days with no milestone a
reader could open.

**The promise that is false today.** `heroes check` accepts `first([P(x: 1)])` at
exit 0 and `heroes build` refuses it, and the same gap has three more faces, all
measured: a map built inside `function tally<K>(k: K)` and keyed at `f64` by the
call is `check` 0 and **runs**, where `m: {f64: i64}` written down is exit 1;
`spec:221` promises a compile error for comparing a `partial` group record *"for
it and for any value holding it"*, and through `function same<A>(x: A, y: A)` it
is `check` 0, `build` 0, **run 134**; and a module that is nothing but an
`extern` group is `heroes check` **exit 0 with zero output**, because `check`
never runs clang — that fourth one is M-package-manager's, where a distributable
binding makes it load-bearing.

**Corrected 2026-09-16, at this milestone's opening: the first face is CLOSED,
and the sentence above is false as it stands.** Measured on a compiler built
from the seed, `first([Point(x: 1, y: 2)])` through a `function first<A>(xs:
[A])` whose body sorts is `check` **1** and `build` **1**, not `check` 0 —
panel 084 R1's body refusal closed it, by the very route the next paragraph
names as the one that worked. The other three faces were re-run the same day and
each holds exactly as written: the map through `tally<K>` is `check` 0, `build` 0,
**run 0 printing 1** where `m: {f64: i64}` written down is `check` 1; and
`same<A>(x: a, y: b)` over a `partial` group record is `check` 0, `build` 0,
**run 134** — `panic: a partial record has no structural equality` — where `a ==
b` written down is `check` 1. **What was found in the closing rather than in the
gap**: the refusal that closed the first face had no golden case, only a unit
test asserting `is_refusable` on a `.generic` type id, which is a fact about a
table entry and not about a program.
`tests/golden/check/sort-through-a-type-parameter.hero` is that case, and it
pins both spellings — `sort(xs)` and `xs.sort()`.

**Why none of them can be fixed in the body.** Panel 084 R1's shape worked
because `sort`'s domain is restricted, so refusing it on `[A]` deleted nothing. A
map keyed on `K` is legal at `str` and every integer, and `==` is legal on almost
everything, so a refusal inside the generic's body would delete working programs.
The concrete type arrives at **the call**, and that is the only line the author
can edit — which is also why the IR route provably cannot carry the diagnostic:
monomorphisation copies the template's span and discards the call's, while the
checker already keys instantiations **by the call-site span**.

**Refused, with its measurement** (082 R3): running `mono` inside `check` — 57
lines, the file past §11's ceiling, **1.3–2.2× on every keystroke**, a
`--permissive` flag contaminating Part 11's control arm, and a §4.16 hole turning
every `???` file into exit 1. Running clang inside `check` for the fourth face is
the same trade at a larger price.

**The trigger stays a `grep`, not a date**: `grep -rnE '^function [a-z_]+<'
selfhost/` returned zero generics whose body calls `sort(` when it was last
measured, and the day it returns one this is §1.0 compiler-need at any price.
**What the milestone does not lift** is the llm-ergonomist's veto: the generic
body's line stays undecidable from the line plus its signature, and lifting that
needs constraints on generics, which is the author's trade.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
