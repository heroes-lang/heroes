# Panel 138 — variant constructors as values: the item named a parser that never existed, and the route nobody listed was already in the grammar

**Sat** 2026-09-13 · **milestone** M-deferral-ledger, step 4 · **status**
`provisional — author ratification pending`

**Lane: full five seats, plus a completeness critic**, which is new and is the
reason this synthesis says what it says. The gate CLAUDE.md § 4 asks once per
milestone was given at step 1; this sitting convenes without asking again.

## The item, as design.md Part 7 carries it (`:2868-2871`)

*"**Variant constructors as values** — the parser's `term` and `expression`
functions are identical except for two names, and would unify into
`sequence(@p, is_times, .product)` if a variant constructor could be passed as a
value. This is the first place the language is measurably poorer than needed.
Note it, don't fix it yet."*

**Every testable clause in that paragraph is false, and the sitting measured each
one.**

## The proposal, as it went out

A clause added to § 9 beside the function-values bullet:

```
-- Top-level functions are values: `xs.fold(0, add)`.
+- Top-level functions are values: `xs.fold(0, add)`, and so is a variant's case
+  written without its arguments: `.product` is the function that builds one,
+  `(function(children: [Expr]) -> Expr)`.
```

**+38** vendored (5655 → 5693). The real-tokeniser row for a draft is unrun, and
the FFI seat reports it comes back `STALE` on the judged path.

## What the coordinator measured before the briefs went out — and where the brief itself broke

| claim | verdict |
|---|---|
| `selfhost/` has no `term`/`expression` pair; its parser is precedence climbing since **2026-08-04 04:10**, the day the item was written | **holds** — three seats re-ran it |
| the pair lives in `examples/calculator/` | **holds**, but it is **one program written twice** (`parse.hero` and `whole.hero`), not three sites: the warden found `examples/spreadsheet/formula.hero` returns `f64?`, builds no variant, and differs by arithmetic, so it is not an instance |
| the pair differs by **three** names | **holds**, and the reader counts **four** — `term` appears on both sides, outer in one and inner in the other, so a mechanical rename cannot produce one from the other |
| **"two of the three are already values today"** | **FALSE**, and the coordinator was wrong. The engineer compiled it and the coordinator reproduced it: a function type **cannot carry `@` at all** — `error[expected_type]: expected a type, found `@`` — so the sub-parser, which takes `@p: Parse`, is not passable. `selfhost/ast.hero:319-330` confirms `function_type` has no inout flag |
| the unification already works with a two-line named wrapper | **holds**, compiled and run: `product(1 2)`, `sum(3 4)`, `5`, exit 0 |

**So the item asks for a feature that does not unlock the one thing it names.**
`sequence(@p, is_times, .product)` would still not compile with the feature
granted in full, because it needs a second feature — `@` in function types — that
Part 7 does not list and no sitting has proposed.

## Verdicts

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| `compiler-engineer` | **VETO** | §1.7 (`:415-419`), Part 5 (`:2474-2489`), §1.1 | **a case is not a call**: it lowers to a `.construct` shape, so **no C function exists whose address a case value could take**, and three of `Callee`'s four cases index a *declaration*. Either fabricate an `ast.Decl` per case — and the resolver, mangler, unused-function check, formatter and mutation sites all see functions nobody wrote — or add a fifth `Callee` case, and then `selfhost/emit/callback_guard.hero:77-82` **breaks structurally**: it writes `out[hf.decl] @ true`, indexed by declaration, so **the thread guard cannot be attached to a synthesised constructor**. Two files sit at **exactly** their DECIDED ceiling with zero slack, `selfhost/ir.hero` 310/310 and `selfhost/emit/inst.hero` 350/350. **Subtraction: zero** — scanned every `.hero` in the tree for a function whose whole body is `return .case(...)`: `selfhost/` 0, `examples/` 0. And `.plus` is a measured ambiguity, not a theoretical one: `t: Tok @ .plus` compiles today, and under the diff it means a `Tok` or a `(function() -> Tok)` depending on the expected type, so the checker must branch on the expected type's **shape** at seven sites — new bidirectional typing | re-running the wrapper scan returns **0**; if the item is ever implemented, `suite_layout.hero:342`'s DECIDED list gains raised ceilings for **both** files and net deletion is **0 lines** | falsified by one wrapper the feature would delete, or by landing it without touching either ceiling |
| `llm-ergonomist` | **VETO** | § 9 | counted by hand: **spec-B is 36 characters SHORTER** for the program the change exists for, 494 against 530, because the annotation the value needs is 36 characters and § 5 gives no rule that drops it. **A's § 9 contradicts itself**: `:238` already says `.plus` **is** the value, and the added sentence says a case written without its arguments is the function — *"two languages in one text"*. **Nothing A permits is impossible under B**: every A program is a B program plus one wrapper. And the class of mistake A creates that B cannot have: a bare `.case` meant as a value taken as a constructor, so § 7's *"a function value compares as an address"* **silently replaces structural `==`** — `.plus == .plus`, `xs.find(.plus)`, a `{case: i64}` map each compile and may answer differently from what the writer read | over 20 model-written programs whose variant has both a zero-field and a field-bearing case: under A **≥ 20%** carry a bare case in the wrong position; under B **0%** by construction | drops to adopt-with-condition on **180 characters** of two sentences closing `.plus` and the no-expectation case, plus a refusal of a case constructor as a map key, **and** a harness showing A's program shorter by more than 100 characters |
| `spec-warden` | **object — defer**, provisional (every draft is vendored-only) | §1.6 `:253-318`, §1.2 `:190-204`, §1.0 `:112-147` | **the +38 prices a sentence that contradicts § 9's existing `.plus` rule**; the payable figure is **+63** merged at construction's own home, **+115** appended with four silences closed. R1 measures **−54/−55** and is the tree's last free removal — *"it should not be spent here"*, since the honestly closed clause still lands **+61**, over `DELTA_GATE` 50. **§1.2 runs the wrong way**: the absence already fails with a `certain`-grade `type_mismatch` naming the exact function type, one bounded round trip, and the clause replaces it with two new error classes it does not close. **The grammar needs no new production** — `Primary = … \| "." ident [ Args ]` already admits `.product` — so the whole cost is prose that must first decide what `.plus` means | any landing answering silences 1, 3 and 4 reads **≥ +60** vendored, and the precise grep for wrappers passed as values in `selfhost/` still returns **0** | moves on **≥ 3** such wrappers in `selfhost/` (today 0), or a `type_mismatch` golden showing a wrapper's parameter list diverging from the case it wraps |
| `ffi-pragmatist` | **approve, no veto** — and its own recommendation is **defer**, *"because the boundary gets nothing"* | §4.19 `:2137`, §1.11; spec § 13 `:340` | **three compiled probes, three `ffi_type` refusals**: a constructor value's result is always a variant, and a variant is always refused in a callback's result — so **a constructor value can never reach C** and the ABI is safe. Measured: `sizeof(HeroFn)=8`, `hero_desc_func.size=8`, a fat `{fn,tag}` would be **16**. The nearest buildable thing — a named wrapper passed as a value — emits a plain address, and **the case tag is a compile-time constant baked into the function body**, so one emitted C function per (variant, case) is byte-identical to it and panel 119's closure finding does not apply. **What the boundary wants: nothing.** Of 21 files in `examples/` declaring an `extern`, exactly **one** also declares a `variant`, and all seven of its constructor sites **apply** rather than pass | at the next milestone touching `examples/ledger/db/sqlite.hero`: it loses **zero** lines and gains **zero** uses of the feature | **approve flips to veto the moment the representation is anything but a bare address** — a tag alongside the pointer takes `sizeof` from 8 to 16 and makes `descriptors.hero:62`'s `&hero_desc_func` a lie about the width |
| `historian` | **object** (advisory): defer | — | **OCaml refused for 25 years and the reason is this sitting's own question**: Leroy in 2001 — constructors *"are not functions"*, arity is not a tuple, and `fun x -> Succ x` reads better; and in 2023 an implementation *"got stalled … because we could not collectively agree on whether the function for a multi-argument constructor should be curried, tupled, or **labelled**"*. **Heroes' construction is the labelled case.** **Swift ships it and the labels do NOT survive** — SE-0155 gives `Expr.elet` the type `([(String, Expr)], Expr) -> Expr`, because SE-0111 forbids argument labels in a function type — while the draft here **keeps** the label. **Scala 3 un-shipped it**: the companion object no longer extends `Function{0-23}`, and the migration guide's recommended replacement is **the hand-written wrapper**. Rust gives tuple constructors functions and braced ones none, by explicit model (RFC 1506), and pays for the name living in two namespaces with a permanent grammar restriction. **No language designer has published a measurement of what first-class constructors saved** — searched-negative, so the item's word *measurably* is unearned | at the implementing milestone, corpus-wide saving is **under 10 deleted lines**, and the first multi-field case used as a value forces a ruling on whether function-type identity includes field labels, costing more diagnostic surface than it deletes | a shipped, unreversed language with **mandatory named-field construction** whose constructor value **keeps the labels**; or ≥ 3 wrapper sites in `selfhost/` itself; or a Rust rationale for its asymmetry |

## The route nobody listed, found by the critic

**`Expr::product` — the `::` operator, which already exists and already means
exactly this.** `Postfix = Primary { "." ident [ Args ] | "::" ident | … }` parses
it today with no new production, and `::` already means *a member named on its
declaring type, checked against the declaration, an undeclared name being a
compile error* (`Point::x`, panel 132, M-reflection-verdict). **No seat mentioned
it.**

It closes all four briefed silences **by construction**: `.plus` keeps the one
meaning § 9 `:238` gives it, the variant is named so there is no bidirectional
branch and no no-expectation case, storage needs no annotation, and
`xs.map(Expr::number)` is decidable where `xs.map(.number)` is not — which is the
fifth silence, **the feature's own headline use**, that no seat named either.

It is not free and the coordinator measured its floor: `Expr::sum` is refused
today, `error[variant_in_value_position]`, so the checker must learn to admit a
variant on the left of `::`; and panel 132's own landed figure for a new member
node is **+388/−95 across 28 files, four ceilings, 68 exhaustive matches**. It is
recorded as the form that returns, not adopted, because nothing in this sitting
measured it and the sitting will not adopt an unmeasured route to escape a
measured refusal.

**A second unlisted route, one line**: narrow the bare form to argument position
only — no binding, no array element, no `==`, no map key — which kills the
reader's address-equality class and the warden's storage silence for fewer tokens
than any draft on the table.

## Where they disagree, unsmoothed

**Two vetoes and no support.** The engineer vetoes on §1.7, the reader on
locality; the warden and the historian object; the FFI seat approves the ABI and
then recommends defer because the boundary gains nothing. No seat argues for it.

**The corpus count, settled by the critic.** The engineer found **0** wrapper
functions; the warden found **3**. The critic checked the warden's third,
`examples/gallery/11-trees.hero:80`, and it takes no parameters and has two
statements, so it is not a wrapper. **The engineer's 0 stands**, and the warden's
break-even arithmetic — 3 constructors, or 6 — rested on an over-count against a
corpus of zero.

**The coordinator's claim, settled against the coordinator.** The engineer called
claim 3 false; the FFI seat said it holds. The coordinator compiled the case and
**the engineer is right**: a function type cannot carry `@`. That is recorded
here rather than quietly corrected, because the brief is what every seat reasoned
from.

**A command that did not settle what it was cited for.** The warden wrote
`git log -S "fn term(" --all` — `fn` is not Heroes syntax. Its conclusion
survives, and the command that settles it is
`git log --all -S "function expression(" -- selfhost/`, which returns empty.

## The resolution adopted, provisionally

**Item 9 is REFUSED, to Part 6, with its falsifier — and its own text is struck
beneath the row with the date, which is the half a verdict on the feature alone
would have missed.** The critic's fifth point is the reason this is written down:
*"Every verdict addresses the feature; none addresses the row."*

1. **The row's two testable claims are struck.** *"The parser's `term` and
   `expression` functions"* names no parser in this repository: the compiler's has
   been precedence climbing since the hour it was born, and the pair is in one
   example program written twice. *"Identical except for two names"* is three by
   one count and four by another. *"Would unify into
   `sequence(@p, is_times, .product)`"* **would not compile even with the feature
   granted**, because a function type cannot carry `@`. And *"measurably poorer"*
   is unearned: the only measurement in existence is this sitting's, and it is
   two lines per constructor against a corpus of zero.
2. **The Part 6 row's falsifier**, which is the engineer's and the historian's
   condition merged: *a corpus program where a variant constructor must be chosen
   at run time from data that is not itself a variant case already in scope, so no
   exhaustive `match` can dispatch it* — **or** three or more one-line constructor
   wrappers passed as values in `selfhost/` itself, which is Principle 0's own
   test and today reads zero.
3. **What returns is named and it is not this spelling**: `Expr::product`, on the
   `::` operator that already exists, with panel 132's measured price beside it
   and `variant_in_value_position` as the diagnostic that stands in its way. A
   later sitting takes it with a number; this one records it.
4. **The fourth route is adopted as the answer to the corpus**, and it is not a
   language change: dispatch the constructor with an exhaustive `match` on the
   value already in hand. The coordinator compiled it — `product(1 2)`,
   `sum(3 4)`, `5` — and then compiled the safety claim: adding a case to the
   variant makes the dispatch `error[non_exhaustive]`, **a compile error a
   function value would not have caught**. It is strictly safer than the feature
   it replaces.
5. **Item 16 is named as a neighbour this sitting must not decide.** Symmetric
   variant syntax promises one spelling for building and matching in v2; the bare
   `.case` already carries two productions, and the refused feature would have
   made it three before item 16 rules.

**What conservative would have been, so the author can choose it**: defer with the
`::` route as the return condition, rather than refusing. The sitting refuses
because two vetoes are engaged, no seat supports it, and a Part 6 row carries a
falsifier a reader can check where a fifth deferral carries a promise.

## Found beside the sitting

**Not a defect, and the sitting says why rather than inflating the list.** The FFI
seat found the emitter writing two identical typedefs for one function shape, as
panel 137's seat did. It is legal C11, deterministic, and is neither a crash, a
wrong answer, nor a silence where a message is owed — the three shapes
`docs/work/DEFECTS.md` holds. It stays recorded for the milestone that next
touches `selfhost/emit/synth.hero`.

## Process note — the critic earned its place on its first run

The completeness critic is new to this panel and it changed the resolution three
times over: it found the `::` route no seat had named, it settled the
engineer-against-warden corpus count by reading the disputed file, and it caught
that every seat had judged the feature while none had judged the row. It also
listed what each seat asserted without running, including the engineer's own
rank-3 premise and the warden's Rust-syntax grep. **A sixth seat would have added
a sixth opinion; a critic added the questions.**

## Author's verdict

Pending — queued in `docs/work/DECIDE.md` as `panel 138`.

## Predictions to score

| prediction | instrument | scored at |
|---|---|---|
| engineer: the wrapper scan returns 0; an implementation raises both zero-slack ceilings and deletes 0 lines | `grep`, `-- ./heroes layout` | M-deferral-ledger's close |
| reader: ≥ 20% of generated programs carry a bare case in the wrong position under the A text | metric 2's runner | M-thesis-harness |
| warden: a landing answering the silences reads ≥ +60 vendored; wrappers in `selfhost/` still 0 | `heroes measure`, `grep` | M-core-packages |
| FFI: `examples/ledger/db/sqlite.hero` loses zero lines and gains zero uses | `git diff --stat` | the next milestone touching it |
| historian: corpus-wide saving under 10 deleted lines; the first multi-field case forces a labels-in-function-type ruling | the `corpus` suite | the implementing milestone, if any |
| the falsifier's own clock: three wrapper sites in `selfhost/`, or a run-time constructor choice no `match` can dispatch | `grep`, a sitting that proposes one | every later sitting of this ledger |

## What the seats could not source or could not run

The engineer built no spike, so its cost table is projection from grep counts, and
its rank-3 premise — that a constructor handed to a C callback puts refcount
mutation on a C-made thread — **was never run**, while the FFI seat's three probes
refuse a variant at every callback slot; the critic named that contradiction and
it is unresolved. The reader hand-counted characters over one program that cannot
compile and one that was not compiled, in characters where §1.6 judges tokens. The
warden's no-veto is an inference from vendored deltas, and draft A's `real` row is
`STALE`. The FFI seat's *a constructor value can never reach C* is a class claim
from three probes that all carry variant payloads; a field-less variant and a
single-case variant are unprobed. The historian could not verify that no Rust RFC
proposes braced-variant constructor functions, could not extract Elm's stated
rationale, and reads Haskell and SML through secondary sources.
