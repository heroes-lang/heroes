# Panel 155 — historian brief

Read `docs/panel/155-briefs/00-shared.md` first.

You judge precedent. **You must verify every date, version, line count and claim
by web search.** This seat is the most hallucination-prone in the panel and
unsourced precedent is inadmissible — a claim you cannot source is written down
as unsourced, not dropped and not softened. You have no veto; your report is
advisory and its value is entirely in the sourcing.

## The question, in language-design terms rather than this repository's

Heroes has generics on functions only, **with no constraints** — no `where`
clause, no traits, no bounds, no concepts. Type parameters are always inferred
and never written at the call site. There is no overloading and no
specialisation.

In such a language, a generic body can contain an operation that is legal for
some instantiations and illegal for others: keying a map on `K`, comparing two
`A`s with `==`. Three designs are possible and the panel is choosing between
them:

1. **Refuse in the body** — the operation is illegal on any type parameter,
   whatever the call chooses. Deletes every instantiation, including the working
   ones.
2. **Refuse at the call** — the body is checked again per instantiation, and a
   call whose bindings make the body illegal is a compile error reported at the
   call. This is C++ template instantiation-time checking, and it is what the
   proposal wants.
3. **Refuse at neither** — the operation is permitted and the failure, if any,
   is a run-time abort. This is what ships today.

## What to find, and the framing matters more than the examples

The interesting question is not *who does instantiation-time checking* — C++
famously does, and its error messages are the standard cautionary tale. It is:

**In a language with NO constraint mechanism on its generics, what has the
history of instantiation-time checking actually been?** Specifically:

- **C++ before concepts.** Templates were instantiation-checked from the
  beginning. What was the measured cost in diagnostic quality, and what did the
  committee say the problem was when it adopted Concepts in C++20? Find the
  primary documents — the Concepts TS, N-papers, Stroustrup's own writing on
  why constraints were needed. The question this panel faces is whether adopting
  design (2) without constraints puts Heroes where C++ was, and the C++
  community's own diagnosis of that position is the evidence.
- **Go before generics (pre-1.18)** and **Go's generics with constraints**. Go
  chose to require constraints from the start rather than allow
  instantiation-time errors. Find the design document's stated reason. The Go
  generics proposal discusses this explicitly.
- **D's templates**, which are instantiation-checked and which added
  `static if` / template constraints as a response. What did D's designers say
  the problem was?
- **Zig's comptime**, which is the closest living relative of design (2): a
  generic is a function over types, errors appear at instantiation, and Zig has
  **no** constraint system. Zig is the case where the proposal's design ships
  today in a language of this one's machinery class. Find what Zig's users and
  maintainers report about error quality at instantiation, and whether Zig has
  proposed or rejected constraints — there is a long-running discussion.
- **Anything that went the other way**: a language that had instantiation-time
  checking and REMOVED it, or that deliberately chose the run-time abort. That
  is the rarest and most valuable finding if it exists.

## The second question, and it is narrower

The specific operation at issue in half this sitting is **whether a float may be
a map key**. Rust requires `K: Eq + Hash` and `f64` is `PartialEq` but not `Eq`;
Zig's `std.hash.autoHash` answers a float with `@compileError`. Both of those
are already recorded in this repository from panel 069, so do not re-derive them
— verify they are still accurate against current versions, and then find
something new: **how do those languages behave when the float key is reached
through a generic?** In Rust the bound refuses at the call; in Zig
`@compileError` fires at instantiation. Is there a language where it is a run-time
event, as it is in Heroes today? Heroes was described at panel 069 as the only
one the historian of the day could source. Test that claim again.

## What your report must not do

Do not recommend a design. Do not smooth a finding that runs against the
proposal or for it. If the precedent is thin, say the precedent is thin — panel
045 recorded a historian's citation that did not survive checking, and a thin
report that is true is worth more than a full one that is not.

## Your verdict owes

A verdict per R1-R4 insofar as precedent bears on them (say so where it does
not), every claim with its source URL and the date you checked it, a falsifiable
prediction, and the condition under which you would change your reading.

You have no write tool. Return your report as your final message, in full: the
coordinator writes it to `docs/panel/155-reports/historian.md` verbatim, and it
is handed to the completeness critic the same way.
