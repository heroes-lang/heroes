# The document was not the only thing that was behind

2026-09-16. Panel 159, on three rules a reader of `spec/heroes-spec.md` had to
guess at: is `sort` ascending, is `xs[i] @ v` accepted, may `main` be `-> ()?`.
The shared brief answered all three from the compiler and concluded, in its own
words, that **none of the three is a compiler defect**. Two of them were.

## What the sitting was for, and what it produced instead

It was convened to price three sentences. It priced them — +11, +9 and +9
vendored, with a −14 removal paying most of it, and a real delta of **+24**
against the vendored **+15**. That 60% gap is the author's instruction of the
same day, *always measure with the real*, arriving with its own number.

But the three findings that mattered were not prices.

## Two seats independently drafted a sentence that is false

R3 was put as *"`function main()`, that signature and no other"* by the
llm-ergonomist and as *", that signature exactly"*, at +4, by the spec-warden.
`function main() -> ()` — explicit unit — **compiles and runs at exit 0**,
because `selfhost/check/decls.hero`'s guard asks `result != unit`. Landing
either would have put a claim in the one document a reader is told to trust that
the compiler contradicts: **manufacturing a defect that does not exist**, which
is the inverse of every failure this project has catalogued.

Only the compiler-engineer found it. The true wording is the diagnostic's own —
`main` **produces nothing** — and it costs **+9**, more than double the false
one. The sitting has no mechanism that would have caught this; the first place
those drafts met a compiler was the completeness critic.

## A seat could not have known, and that is the sitting's design, not its fault

The llm-ergonomist's R2 sentence merged into `spec:283-285`, whose remainder
says a push *"reached through a field or an index copies the whole array"* —
true for `push` and **false for `xs[i] @ v`**, measured on the emitted C. Its
brief forbids it the repository, which is the point of the seat and the reason
its verdicts carry information. **So nothing routes a drafting seat's exact
wording back through a seat that can compile it**, and they meet for the first
time in the synthesis, by which point a resolution has been adopted.

## The fact R1 was pricing was already written, to the wrong reader

`runtime/heroes_runtime.h:420` has said `sort(xs)` — a NEW array, STABLE,
**ascending** since before the sitting. The compiler-engineer reported the rot
on the line below it — a type list naming three element kinds where
`hero_cmp_for` dispatches twelve — and walked past the line above. So the
direction was a published promise **to a C author including the header, and not
to the Heroes author reading the one document we call the whole language.**

## And the general rule is cheaper than its instance

R2 was put as *does the document state `xs[i] @ v`*. That partial sentence
prices at **+10**; the general rule — *`@` declares a mutable cell and re-binds
it, or a field or element inside one* — prices at **+9**, covers `f(@xs[i])`
which the document mentioned zero times, and carries no *expressio unius*
damage. Naming a third of a rule costs more than naming the rule.

## What landed

The three sentences, the −14 removal, defects 052 and 053 repaired first
because without them two of the sentences would teach what the compiler
punishes, defect 051 closed with the enumeration it owed and a second witness,
and three rules by author instruction: a vendored delta is not a price; a live
list is a preamble, a count and its items; and `grammar` joins the two suites
that judge a change to the specification.
