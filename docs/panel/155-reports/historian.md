# Panel 155 — historian report

Written to disk by the coordinator, verbatim from the seat's final message, because
this seat has no write tool (panels 143 and 144 both recorded that it could not be
audited for exactly that reason). **This seat stalled on the watchdog at 600s on its
first run and produced nothing; this is the narrowed retry, and the stall is recorded
in the sitting.**

## verdict

**object (advisory)** — narrowly, and not against any of the three designs. I object
to two things the sitting can fix without choosing: (a) the record's claim that
Heroes is *the only* language making a generic float-map-key a run-time event is not
supportable as stated — Go 1.20 deliberately admits a run-time panic for exactly this
class; (b) if design (2) is adopted, C++'s own diagnosis of design (2)-without-constraints
should be recorded verbatim alongside it, because it is the sharpest evidence in this
file and it runs against (2).

All dates checked **2026-09-16**. Provenance note: I read the Stroustrup PDF pages
**first-hand**; every other quote was extracted by the fetch tool from the cited URL
and is marked `verified (fetch-extracted)` — one degree weaker than first-hand, and I
say so rather than smooth it.

## precedents

**1. Zig — closest living relative of (2). VERIFIED (metadata first-hand from the GitHub API).**

- `ziglang/zig#1268` "comptime interfaces": opened 2018-07-20, **closed `not_planned`
  2023-05-31**, milestone 0.11.0, 87 comments. Source:
  https://api.github.com/repos/ziglang/zig/issues/1268
- The thread kept accumulating comments **after** closure, latest seen 2025-03-26 —
  ~7 years of repeated requests, closed, still argued. Sample user complaints
  (fetch-extracted): "Anytype has very complicated autocompletion, no explicit
  constraints" (16hournaps, 2024-04-11); "Allocator in stdlib is a great example of
  why we should have this".
- `ziglang/zig#1669` "Proposal: User definable type constraints on polymorphic
  parameters": opened 2018-10-20, **closed 2020-12-06** (`state_reason: completed`),
  milestone 0.8.0, 19 comments. Source:
  https://api.github.com/repos/ziglang/zig/issues/1669. Closing rationale, SpexGuy
  2020-12-06 (fetch-extracted): *"everything that this proposal introduces is already
  possible by calling the validation functions in the first few lines of the
  function"*. A later comment, ThadThompson 2022-06-29: *"seeing an `anytype` gives me
  the similar feels as when I come to a C function that takes `void *`"*.
- **UNVERIFIED**: I found **no** statement by Andrew Kelley or another core maintainer
  giving the rejection rationale. SpexGuy's role (triager/contributor vs. core team) —
  unverified.
- **UNVERIFIED, flagged as a live lead**: a search result titled "#32099 - remove
  `@TypeOf` and `anytype`; introduce `|T|` syntax - ziglang/zig - Codeberg.org"
  (https://codeberg.org/ziglang/zig/issues/32099). I did not open it. If Zig is
  currently reformulating `anytype`, that is material to this panel and someone should
  read it.

**2. C++ Concepts — the diagnosis of (2). VERIFIED FIRST-HAND.**

Bjarne Stroustrup, **P0557r1**, *"Concepts: The Future of Generic Programming, or How
to design good concepts and use them well"*, dated **01/31/2017**,
https://www.stroustrup.com/good_concepts.pdf (pp. 1-3), read directly:

- p.1: *"In about 1987, I tried to design templates with proper interfaces [Str94]. I
  failed. I wanted three properties for templates: - Full generality/expressiveness -
  Zero overhead compared to hand coding - Well-specified interfaces. Then, nobody
  could figure out how to get all three, so we got - Turing completeness - Better than
  hand-coding performance - **Lousy interfaces (basically compile-time duck
  typing)**"*
- p.2: *"**The lack of well-specified interfaces led to the spectacularly bad error
  messages we saw over the years.** The other two properties made templates a run-away
  success."*
- p.3, the "We have problems" list: *"The requirements of `sort` on its argument type
  are implicit ("hidden") in its function body."* and *"**The error message for
  `sort(d)` will appear only when the template is instantiated, and that may be long
  after the point of call.**"*

This is design (2) without constraints, diagnosed by its own designer, 30 years after
shipping. (n3580 "Concepts Lite" was fetched but the PDF did not parse; not cited.)

**3. Go — required constraints from day one. VERIFIED (fetch-extracted).**

Type Parameters Proposal,
https://go.googlesource.com/proposal/+/refs/heads/master/design/43651-type-parameters.md :

- *"In C++ a generic function can call any method on a value of generic type... If the
  function is called with a type argument that does not have a String method, the
  error is reported when compiling the call... These errors can be lengthy, as there
  may be several layers of generic function calls before the error occurs."*
- *"The C++ approach would be a poor choice for Go. One reason is the style of the
  language. In Go we don't refer to names... and hope that they exist. Go resolves all
  names to their declarations when they are seen."*
- *"This is an important rule that we believe should apply to any attempt to define
  generic programming in Go: **generic code can only use operations that its type
  arguments are known to implement**."*
- Against inferring constraints from the body: *"We don't want to derive the
  constraints from whatever `Stringify` happens to do... a minor change to `Stringify`
  might change the constraints."*

**4. D — added constraints to instantiation-checked templates. VERIFIED
(fetch-extracted); authorship/date UNVERIFIED.**

https://dlang.org/articles/constraints.html lists three motivations, the third being
to *"provide better diagnostics when arguments don't match, rather than an obscure
error message based on the irrelevant (to the user) internal details of the template
implementation"*. The page carries no author or date that the fetch could find.

**5. The rare case — a language that chose the run-time abort for THIS class. VERIFIED
(fetch-extracted). This is the strongest precedent *for* (3), and it comes from Go.**

Go 1.20 release notes, https://go.dev/doc/go1.20 : *"Comparable types (such as
ordinary interfaces) may now satisfy `comparable` constraints, even if the type
arguments are not strictly comparable (**comparison may panic at runtime**). This
makes it possible to instantiate a type parameter constrained by `comparable` (e.g.,
**a type parameter for a user-defined generic map key**) with a non-strictly
comparable type argument such as an interface type."*

So the language that refused instantiation-time errors on principle nevertheless
**loosened a compile-time refusal into a run-time panic**, in the generic-map-key
case, four versions after shipping generics.

**NOT FOUND**: a language that had instantiation-time checking and *removed* it. Per
CL-018 this is a statement about my vocabulary, not the world: I searched
Zig/C++/D/Go only, and did not search Nim, Ada, Eiffel, Scala or Julia. Treat as an
open question, not a negative result.

**6. Float-as-map-key across languages.**

- **Rust — bound VERIFIED, the `f64` half UNVERIFIED.**
  https://doc.rust-lang.org/std/collections/struct.HashMap.html : impl bound
  `where K: Eq + Hash, ...`, and *"It is required that the keys implement the `Eq` and
  `Hash` traits."* The page does **not** state that floats are excluded; I did not
  fetch `primitive.f64.html`, so "f64 is `PartialEq` but not `Eq`" is **unsourced this
  session**.
- **Zig — VERIFIED (fetch-extracted from source).** `lib/std/hash/auto_hash.zig` on
  master contains `@compileError("unable to hash type " ++ @typeName(Key))` reached for
  the `.float` case, plus separate `@compileError`s for many-item/C pointers and
  untagged unions. Source:
  https://raw.githubusercontent.com/ziglang/zig/master/lib/std/hash/auto_hash.zig
- **Standard ML — the finding the panel's three-way framing omits. VERIFIED
  (fetch-extracted).** SML Basis Library, `REAL`: *"Note that, as discussed below,
  `real` is not an equality type."* https://smlfamily.github.io/Basis/real.html . With
  equality type variables (`''a`, inferred by the checker, type of `=` is
  `''a * ''a -> bool`), SML gets a **compile-time** refusal of float equality
  **without any constraint the programmer writes and without re-checking the body per
  instantiation** — the constraint is inferred into the signature
  (https://mlton.org/EqualityTypeVariable ; that page itself failed TLS validation for
  me, so it is cited as a lead, not evidence). This is a **fourth design**, ~1990
  vintage, not among (1)/(2)/(3): *infer the constraint, publish it in the signature,
  refuse at the call against the signature*. Go's proposal explicitly rejects this
  route (quote in item 3) on stability grounds — which means both sides of it are
  sourced.
- **Verdict on the record's claim**: "Heroes is the only one making it a run-time
  event" is **not supported as stated** — Go 1.20 makes a generic map-key comparison a
  run-time panic. For *floats specifically* I found no other run-time-event language,
  so the narrower claim survives this session unfalsified.

**Route, not evidence**: *Heroes of code* is what told me which rooms to walk
(Stroustrup's 1987 attempt, Zurich's separate-compilation discipline). Every citation
above is the primary source I then went and read; the book upgrades nothing here.

## argument

Precedent is unusually unanimous on one point and silent on another. Four language
communities — C++, Go, D, and Zig's users for seven years — independently name the
same failure of design (2)-without-constraints: the requirement is hidden in the body,
and the error arrives at instantiation, far from the call. C++'s designer calls the
result "spectacularly bad error messages." That is evidence about (2), not about (3).
On (3) the precedent is thin but real and points the other way: Go deliberately traded
a compile-time refusal for a run-time panic on generic map keys in 1.20. Meanwhile SML
has, for decades, had a fourth option the framing omits: inferred equality
constraints, no constraint syntax, no per-instantiation re-check.

## condition

I would change my reading on finding any of: (a) an explicit Zig core-team rationale
for closing #1268 `not_planned` that is *not* "already expressible by calling a
validation function" — that would tell us whether Zig judged instantiation errors
acceptable or merely unsolved; (b) Codeberg #32099's current content, if Zig is now
reformulating `anytype` toward declared constraints; (c) any language that had
instantiation-time checking and removed it; (d) a shipped language with function-only
generics, no constraint syntax, and instantiation-time checking where a constraint
mechanism is *not* among the most-requested features after five years. And on the SML
route specifically: if inferred equality-type variables were found to have been
*removed* or deprecated in a later ML, that would weaken the fourth option
considerably.

## prediction

**Falsifiable, checkable inside this repository.** If Heroes adopts design (2), then
within the first milestone after it lands, at least one golden diagnostic will have
its primary source span in a file other than the program being compiled — i.e. inside
`selfhost/` or a generic definition — with the call site only as a note. Count is
measurable by running the golden suite and comparing each diagnostic's primary-span
file against the input file. Precedent (C++, D, Go's own warning about "several layers
of generic function calls") predicts > 0; if it stays at 0 across a milestone, the
precedent does not transfer to a language this small and I am wrong.
