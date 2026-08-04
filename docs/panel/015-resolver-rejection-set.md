# Panel 015 — the resolver's rejection set

Date: 2026-08-04. Trigger: M3a, the resolver (design.md Part 10 step 4). Five
decisions, each of them a **diagnostic class**, so the panel is mandatory before
they ship (CLAUDE.md §4). Five judges, all with standing: the ffi-pragmatist's is
narrow (A and C) and it produced the panel's most consequential correction.

## Proposal (as put to the judges)

`spec/heroes-spec.md` already legislates two of these, in six words: "An unused
variable is a compile error. Shadowing is a compile error." What it does not say
is **how far each reaches**. M3a cannot avoid choosing.

| # | question | default implemented while the panel ran |
|---|---|---|
| **A** | Does "unused" reach **parameters**? | Yes; `extern` exempt (no body). Top-level declarations not covered. |
| **B** | Does a **mutation** count as a use? | No — only a read does. |
| **C** | Are the **built-in names** reserved? | Two tiers: the compiler forms reserved, the six the spec says are "written in Heroes" declarable, and a file's own definition wins **silently**. |
| **D** | Do **UFCS method names** resolve at M3a? | Yes — `x.f(y)` is `f(x, y)`, so an unknown `f` is an error before types exist. |
| **E** | May **sibling scopes** reuse a name? | Yes; a local may not shadow what encloses it. `_` never binds. |

Every default except E is a rejection, on the standing rule that a rejection can
be relaxed later while an acceptance cannot be withdrawn.

## Verdicts

| Judge | A | B | C | D | E |
|---|---|---|---|---|---|
| compiler-engineer | approve (~9 lines) | approve (**1 line**) | **object** — the list is wrong | **VETO the rejection half** (21 lines) | approve (**0 lines**, and it *saves* state) |
| llm-ergonomist | approve α **only with an escape form** | approve α **with an `@`-parameter carve-out** | **VETO β** on locality; approve α | approve α | approve α; object to β |
| spec-warden | object (+2 terse / +14 honest) | approve **at 0 tokens** | **VETO the second tier** (Principle 0) | approve — 0 tokens, confirmed | object (+20) |
| ffi-pragmatist | object, narrow (two textual gaps) | — | approve, **`extern` must not be exempt** | — | — |
| historian (advisory) | approve, one required companion | approve, **and go further than Go** | object, narrowly | approve — precedent does *not* transfer | approve; best-precedented of the five |

Measured, by the judge who was given the instrument:

- **spec cost** (binding max = larger of two vendored tokenisers; baseline **2048**):
  A +2 terse / +14 honest · B +8 · C **+48 enumerated / +23 without the list** ·
  D **0** · E +20. All five stated: **2113** (no list) / **2138** (enumerated).
  Removals found and measured: **165 tokens** in six places, the largest being a
  **96-token stale HTML preamble** that still claims the budget is ~1500.
- **implementation cost**: a working prototype, **619 lines in 5 files**, built
  against the real tree; honest M3a with fixes, candidates, `heroes check` and
  goldens ≈ **700 lines**. A is a free rider on the walk; B is **one line**
  (`mark_written` instead of `mark_read`); E is the *cheaper* branch, because
  forbidding sibling reuse needs a per-function set that never pops.
- **the witness, measured** (design.md's 317-line appendix): **53** UFCS calls,
  not the 23 the proposal claimed. 12 `@` cells, not eleven. One hole. With
  `fail` and `str` in the table and one top-level namespace: **0 diagnostics,
  293 resolved names** (240 bare + 53 through a dot).
- **the reserved list against reality**: **4215 real C headers** scanned (macOS
  SDK + Homebrew: SDL, cairo, FLAC, zstd, ICU, libxml…). **Zero** C functions or
  macros named after any reserved name. Every apparent hit was a C++ member
  reachable only through a shim whose exported name the shim author picks.

## What changed the questions

**1. C's second tier is dead, killed from two directions at once.** The
ergonomist vetoed it on locality: under a silent override, the meaning of
`xs.map(f)` on the line being written depends on whether some distant line
declares `map` — "a *meaning* change, not a legality change, and undetectable
from the line plus the enclosing signature". The warden vetoed it on Principle 0:
it is not an addition but an **exception** to a rule the spec already states, so
it costs 23–48 tokens to make two names in one sentence behave oppositely, and
the proposal's own wording ("wins silently") names §1.4's forbidden class. The
historian added that the *irreversibility runs the other way* from the proposal's
reasoning — Python needed a major-version break to reserve `True`/`False`, so
"declarable now, reserved later" is the expensive direction, and tier 2 was the
irreversible half. Resolution: **one tier. Every built-in name is reserved
everywhere — top level, parameter, local. Zero spec tokens, because the spec's
existing sentence already says it.**

**2. The reserved list itself was wrong, and the ffi-pragmatist found it in
design.md rather than in an opinion.** §1.11's Tier 2 and §4.20's inventory both
put **`range`** among the functions written in Heroes; the proposal had it as a
compiler form. §4.20's inventory also carries **`.str()`** and **`sort`**, which
the spec's built-in line omits entirely — and `.str()` is *called by the
acceptance program*, at appendix line 112, so without it the language cannot
render an integer into a message and the appendix could not resolve. The table
is now built from §1.11 and §4.20 verbatim. Two inventory names are deliberately
**absent**: `panic` (Part 5 desugars `assert` into it; a user-callable `panic` is
a language addition and waits for its own panel) and `Builder` (a runtime type
behind `join`, not surface). design.md's own contradiction on `join` — §1.11 and
§4.20 put it in the C runtime, `docs/ROADMAP.md` M6 listed it among the Heroes
library — is resolved **for the runtime**, and the ROADMAP line is corrected.

**3. D's rejection half was vetoed, and the veto was concrete.** design.md:1153
fixes §4.11's algorithm: "on seeing a dot, **first looks for a field**; failing
that, looks for a free function." A field can hold a function value (§4.13), so

```
Holder = record
    cb: (function(int) -> int)

run = function: (h: Holder, n: int) -> int
    return h.cb(n)
```

is legal, and the engineer's prototype **rejected it twice**. The historian,
reasoning from the spec alone (no methods, no overloading), had concluded the
opposite — that the dot needs no types at all — and could not see design.md's
field-first clause. Both are satisfied by a narrowing neither proposed: **the
unknown-function error fires only when no record or variant in the whole file
declares a field of that name.** It is type-free, it never rejects a legal
program, and it still catches every misspelling that is not also somebody's
field — including the ergonomist's own `counts.set("a", 1)`, which was 1 of the
4 programs it wrote from the spec.

**4. A's escape valve was missing, and it is what makes A better than doing
nothing.** Three judges converged. The ergonomist wrote the skeleton-first
program the rule is aimed at and reported that its *likeliest repair* is deleting
the parameter — "A-α without an exemption form does not remove the silent wrong
program, it relocates it into the signature". The historian found the rule has
exactly **one** shipped precedent (Zig) and that all six languages that enforce
or warn provide a signature-level `_` / `_x`. The ffi-pragmatist compiled the
`sqlite3_exec` trampoline and showed a fixed C signature forces **≥2 parameters
no Heroes code reads**, and that C23 legalised omitted parameter names for
exactly this reason. Resolution: **`_` is a legal parameter name, may repeat
within one parameter list, and never binds** (it already parses); **and §4.16's
suppression covers unused *parameters*, not only bindings** — otherwise the M6
witness's own `simplify` fails under A.

**5. B is extended past Go.** Go's rule is B ("assignment alone does not
constitute a use", tightened in Go 1.18, golang/go#49214), and Go left one hole
open for years (golang/go#20802): a write to a *field* of a variable counts as a
use. The historian pointed out Heroes can close it for free, because "no aliasing
exists anywhere" makes such a write provably unobservable. It is closed: `c.n @ 1`
writes `c` and does not read it. The one carve-out is the ergonomist's, and §4.8
requires it: **a write through an `@` parameter is a use**, because the copy-out
always happens and is the whole observable effect of `reset = function:
(@counts: {str: int})`.

**6. E is adopted with a departure stated rather than smoothed over.** Java (JLS
§6.4) and C# (CS0136) both state E's sibling rule almost verbatim and both
defended it when challenged — but both make the enclosing-shadow error
**order-insensitive** ("regardless of the textual order"). Heroes' check is
**sequential**: only a binding already in scope conflicts. The reason is §1.3 —
order-insensitivity means a line 20 lines below decides whether the line you are
writing compiles, and the ergonomist independently flagged that cost from the
spec alone ("legality-non-local"). Recorded as a deliberate departure with the
precedent against it.

**7. The warden's zero-token argument, and where it does not hold.** Four of the
five defaults end in a diagnostic the model reads, and §4.17 already obliges that
diagnostic to carry the rule — so they cost **zero** spec tokens and are pinned
by goldens whose `.expected` contains the wording. The exception was C's silent
tier, and it is gone. Net spec cost of this panel: **0**, with `_`'s widening
(+2) and the 165 tokens of removals recorded for the v1 package.

## Resolution — `provisional — author ratification pending`

1. **A adopted**: unused reaches parameters; `extern` exempt; top-level
   declarations not covered; `_` legal and repeatable in a parameter list;
   §4.16's hole suppression covers parameters.
2. **B adopted and extended**: only a read is a use; the initialiser is not a
   write; a field or element write writes the root; a write through an `@`
   parameter *is* a use.
3. **C: the two-tier proposal is withdrawn. One tier** — every name in §4.20's
   inventory is reserved everywhere, `extern` included. The tier field survives
   in the code as documentation of *where the implementation comes from*,
   rebuilt from §1.11/§4.20. `panic` and `Builder` stay out. The acceptance
   program's `map`/`fold` are renamed `apply`/`reduce` in design.md — the third
   time the appendix has caught a rule (after `.var` and `=> assert false`).
4. **D adopted, narrowed**: resolve at M3a; report an unknown function only when
   the name is not a field declared anywhere in the file; the combined
   field-or-function message belongs to M3c.
5. **E adopted, sequential**, against the Java/C# precedent, on §1.3.

What a veto would have compelled, on the record: had C's second tier shipped, the
ergonomist's veto stands on the meaning of `xs.map(f)` being non-local; had D's
rejection half shipped unnarrowed, the engineer's falsifier file would report
`unknown_function` from `m3a` and stay red through `m3b`.

## Predictions to score

| # | Judge | Prediction | Checkable at |
|---|---|---|---|
| 1 | compiler-engineer | the six-line `Holder`/`cb` file passes `heroes check` only once the unknown-name branch leaves the method lookup — 5 lines, moved to the checker | M3c |
| 2 | llm-ergonomist | A raises the compile-error rate by 15 ± 8 points and lowers "delivered function ignores a required parameter" by 8 ± 5; without a documented `_`, ≥25% (± 15) of A's repairs delete a parameter | first Part 11 run |
| 3 | llm-ergonomist | B without "the initialiser is not a use" fires **0 times in 20** — a measurable no-op; B without the `@` carve-out false-positives on ≥60% (± 20) of tasks whose signature has an `@` parameter | first Part 11 run |
| 4 | llm-ergonomist | E's permissive alternative would reject 40 ± 15% of one-shot programs; the adopted rule ≤10% | first Part 11 run |
| 5 | spec-warden | the warden resolution (removals + `_` widened) measures **2043 (−5)**; the five defaults stated in full measure **2131 (+83)** — 4% of the spec to say what four diagnostics say for free | M3a close / v1 package |
| 6 | ffi-pragmatist | SQLite's six non-callback entry points bind with zero shims and zero unused-parameter diagnostics; the first `sqlite3_exec` row handler needs a hand-written trampoline carrying ≥2 parameters no Heroes code reads | M7 |
| 7 | historian | the M8c self-hosted source contains at least one deliberately-unused parameter, forced by callbacks passed to `map`/`fold`/`find`; grep for `_` parameters or for the rename workarounds | M8c |

## Watch list

- **`+` on `str` is unwritable per the spec's operator table**, while §4.20 gives
  the runtime string concatenation and the appendix uses `"…" + name`. The
  ergonomist hit it from the spec alone and restructured a program to avoid it.
  Its own panel, at M3b where the operator table becomes code.
- **`print`'s reservation has a counter-precedent on the record**: PEP 3105 made
  `print` a function *so that* it could be replaced in one module, and Lua leaves
  it reassignable. The historian withdraws the objection if design.md gives
  `test` blocks an output-capture mechanism that does not need redefining
  `print`. It does not, today.
- **`_`-prefixed names** (`_opts`) as a wider exemption — Rust, Erlang, Elixir and
  TypeScript all use it. Bare `_` only for now: the narrower acceptance.
- **M7 needs an explicit C-name form for `extern`.** `raise` is C89
  `<signal.h>` and is *already* a lexer error in Heroes (foreign-word registry),
  so name-level reservation collides with real C symbols eventually. Nim's
  `importc` has the escape and §4.19 says Heroes took the surface from it.
- **`sort` has no line in the spec** and is in §4.20's inventory. Reserved, and
  resolvable, but unspecified surface.
- **`Ref::Top(u32)` indexes one `Vec<Decl>`.** At M6 the tier-2 functions become
  real Heroes source in a prelude, and the index needs a unit tag or a documented
  concatenation. No third tier is needed; a note in the type is.
- **Panel 010's repeated-`@` rule is now cheap**: the resolver computes the root
  binding of every place, which is the whole input that rule needs. Queued for
  M3c as decided, but the machinery is here.
- **The engineer's standing rule, written into `resolve/mod.rs`**: later passes
  consume `uses` and never re-resolve. Measured reason — in one function
  `len(xs)` can be a local while `xs.len()` is the built-in, legitimately, so a
  pass that re-resolved after Part 5 erases UFCS would silently swap the second
  meaning for the first.
