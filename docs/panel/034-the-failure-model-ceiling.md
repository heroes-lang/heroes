# Panel 034 — The failure model's ceiling, and the cure the language already had

**Convened** 2026-08-12, after M8a, following measurement 004.
**Trigger** design.md Parts 1–11, and a proposed diagnostic *class* (CLAUDE.md §4).
**Status** `RATIFIED 2026-08-12` (was `provisional — author ratification pending`).

## What convened it

The author asked whether Heroes should get a Rust-style error system. It already
has one — `T?` ≈ `Result<T,E>`, `expr?` ≈ `?`, `.must()` ≈ `.unwrap()`,
`.default(v)` ≈ `.unwrap_or(v)`, `match .ok/.err`, and exceptions refused
permanently in Part 6. What it does not have is a *typed* error: the payload is
frozen at two `str`s, and §4.6:958 has called that *"acknowledged technical debt"*
since the beginning.

So the question became measurable, and measurement 004 answered it. A new
mutation operator, `typo-code`, slips one character in an error code — in the
`fail("unknown_char", …)` that builds it and in the `e.code == "unknown_char"`
that reads it back:

| operator | mutants | excluded | killed (check) | killed (--permissive) |
|---|---|---|---|---|
| **typo-code** | **25** | **0** | **0 (0%)** | **0 (0%)** |

The only zero in an eleven-row table where everything else scores 73–100%.

## The proposal, verbatim

> **Part 1** — record that the Rust shape already exists, operation by operation,
> and change nothing.
> **Part 2** — file typed errors as deferred, with their real price: not the +83
> spec sentence but **generics on types** (design.md:1425 refuses them), traits
> for any conversion (Part 7 item 8), and a runtime tax on **every `T?` in every
> program**.
> **Part 3** — a diagnostic at zero spec cost: *comparing `e.code` against a
> `str` literal that no `fail` in the program constructs is a compile error, with
> a did-you-mean over the codes that do exist.* Precedent for a rule living as a
> diagnostic rather than as spec text: panel 031 R5, −11 tokens.

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| compiler-engineer | **approve 1 · approve 2 · object 3** | the whole-program accept-set is **root-dependent, and it compiled the counter-example**: the same file is an error rooted at `lex` and clean rooted at `main`. And `fail(prefix() + "key", msg)` compiles today, so the escape hatch is a **silent global off-switch** |
| llm-ergonomist | **object 3** | wrote the program the rule *rejects*: `e.code == "missing_key"` is correct today and constructed by no `fail`. Worse, **`constant ERR_UNKNOWN_ITEM: str` already makes the typo an undefined-name error, locally, at zero cost — and silently defeats the diagnostic** |
| spec-warden | **approve 1 · object 2's filing · veto 3** | the rule **cannot fire in the self-hosted compiler at all**: the port's codes are a record field (`Diagnostic { code: String }`) threaded through `@diags`, not a `T?` error. All 25 protected sites are in `examples/` |
| ffi-pragmatist | **approve 1 · approve 2's deferral, object to its price · object 3** | compiled it: a typed error that carries what a C library actually reports is **32 bytes and refcounted**, and `ptr?SqlErr` is **40** — identical to today. The 16-byte win needs a payload-free error, which cannot carry `sqlite3_errmsg` |
| historian (advisory) | **approve 1 (re-labelled) · refuse typed errors for v1 · adopt 3, in Rust's shape** | **it was measured, in 2020**: 42% of 204 real string bugs are incorrect string literals; a widely used analyser found **1 of 204**; only **11%** ever surface as an error message |

**Part 3: one veto, three objections, one adopt.** It is not adopted. It is also
not refused — every objection arrived with a lifting condition, and the five
conditions converge.

## Part 1 is approved without argument, and the label is corrected

Nobody disputed that the Rust shape is present. What the historian changed is the
*name* the record gives it. §4.6:958 calls the string code "acknowledged
technical debt", which is a judgement; it is now a **measured defect class**, and
the citation exists:

> Eghbali & Pradel, *No Strings Attached: An Empirical Study of String-related
> Software Bugs*, ASE '20. 204 bugs across 13 repositories. *"The most prevalent
> root causes are incorrect string literals and incorrect regular expressions
> (42% and 37% respectively)"*; *"A widely used static code analyzer finds only
> one out of the 204 studied bugs"*; only *"11% of the bugs lead to an error
> message"*.

Our 0/25 is not this project being pessimistic about itself. It is the 42% row,
reproduced in a language with 17 programs. And the paper's own future-work
sentence — *"Reasoning about the semantics of string literals, and how they
relate to their surrounding code, is a promising direction"* — is a description
of Part 3, filed as unsolved in 2020.

**One premise of the convening brief was refuted**, and it is recorded because
this project's own rule is that an uncitable claim is a guess. The brief assumed
Go abandoned string comparison for a documented reason. The historian read design
document 29934 (Amsterdam, Cox, van Lohuizen, Neil, January 2019) and Go 1.13's
notes: they replace **sentinel value equality**, and say nothing against string
comparison. `err.Error() == "…"` was folk practice the Go team never blessed and
therefore never formally abandoned. **Do not cite Go for this.**

## Part 2 — the deferral is right and both its filing and its price were wrong

**The filing.** The proposal wanted a new Part 7 item. The spec-warden refused,
and the ground is Part 7's own preamble: its items *"lose only on the simplicity
vertex"*. Typed errors lose on three — §1.6's budget, §4.12's positive rule
(*"Generics on functions only, not on types"*), and Part 7 item 8's traits. Filing
them on the non-negotiable ordered list would put there an item unreachable
without reversing a §4.12 **decision**, which turns a wart into a promise — the
exact thing Part 8's preamble forbids. **Wart 5 is the right home**, amended in
place to name the two prerequisites and quote the measured numbers.

**The price.** The ffi-pragmatist built it and measured, against real
`sqlite3.h`, real `-lsqlite3` and real `errno`:

```
HeroStr 16   HeroFailure 32
today:  int? 40   str? 40   Point? 40   Big? 48
typed:  SqlErr(payload-free) 4   int?SqlErr 16   str?SqlErr 24   Big?SqlErr 48
```

The 16-byte, register-returned `int?SqlErr` is real — and it requires a
**payload-free** error. A SQLite error that carries the only useful half,
`sqlite3_errmsg(db)`, must copy that borrowed pointer through
`hero_str_from_bytes`, so the variant is **32 bytes and refcounted** and
`ptr?SqlErr` is **40**: identical to today. Compiled, both ways, leak-clean:

```
today  tag=1 code=cantopen msg=unable to open database file
typed  tag=1 case=0        msg=unable to open database file
```

Three costs the record does not carry anywhere, each compiled:

- **`?` stops being a struct copy.** One function doing `sql.open(path)?` then
  `io.read_port(text)?` is `error: assigning to 'h_AppErr' from incompatible type
  'h_SqlErr'`. With no traits and no generics that is one hand-written converter
  per *(from, to)* pair — and the FFI is exactly where the pairs multiply.
- **`m[k]` has no error type to be.** `emit/aggregate.rs:497`'s own line under a
  typed error side is `error: initializing 'h_SqlErr_tag' with an expression of
  incompatible type 'HeroFailure'`. Typed errors must either re-split §4.6's
  deliberately collapsed `T?`/`T!` or invent a built-in error type for the map.
- **`record R { next: R? }` is unchanged.** design.md:1844-1846 credits `T?`'s
  by-value representation with making it have no size; clang gives the *same*
  `field has incomplete type 'R'` for both. Part 2 buys nothing there.

And the historian supplied the decisive precedent. **Swift shipped typed throws
(SE-0413, accepted 20 December 2023, implemented in Swift 6.0) and its own
proposal tells you not to use them**: *"Resist the temptation to use typed throws
because there is only a single kind of error that the implementation can throw"*
— it may *"hamper further evolution"*. A decade after Swift 1.0, with full
generics, restricted to module-internal code and constrained environments. Heroes
has neither generics on types nor traits nor macros; Rust's landing place
(`thiserror` over an enum) is unreachable for all three reasons.

**Two corrections to the queue, from measurement.** `QUEUE.md:150` and `:183` say
every `T?` is 40 bytes because the error side is two `str`s. The causal half is
confirmed; the quantified half is wrong — 40 holds while `T` fits in 32, and it
is 48 for a 40-byte record, which is panel 023's own correction still un-cashed.

## Part 3 — sound in aim, unsound as written, and the language already had a cure

Four judges objected, from four inputs, and no two found the same defect.

**It is root-dependent, and the counter-example compiled.** `heroes check` on
`examples/calculator/lex.hero` loads `{lex, library}`; on `main.hero` it loads
`{main, eval, lex, parse, library}`. A comparison against `"unclosed_paren"` —
constructed only in `parse.hero`, which uses `lex` and not the reverse — is an
error rooted at `lex` and clean rooted at `main`. **Same bytes, two verdicts,
from two supported invocations.** Both are supported: `heroes test
examples/calculator/parse.hero` runs and passes today.

**It rejects correct programs, and the ergonomist wrote the shortest one.**
`return s.stock[item]` is the better-factored `lookup`; its failure carries
`missing_key`, constructed by the *language* and by no `fail`. The handler
`e.code == "missing_key"` is written straight from spec line 133 — *the only code
the spec names* — and the rule refuses it. Worse, the did-you-mean can only offer
the codes that do exist, so **a confident suggestion that is wrong**: a model
that applies it converts a compile error into a silent wrong program. That is the
worst cell in the matrix, and the spec-warden reached the same program
independently.

**"Zero spec tokens" is false.** `fail(prefix() + "key", msg)` compiles and runs
today, so the constructor set is open, and the proposal's own conservatism — do
not fire when any `fail` has a non-literal code — makes **one computed code
anywhere disable all 25 checks, silently**. That is §1.3 exactly: a construct
whose meaning lives elsewhere. Closing it needs a spec sentence restricting
`fail`'s first argument (~11 tokens); licensing the rule at all, if the
constructible set is not widened, needs one at **+17 measured**. Zero of 23
`fail(` sites in the repo use a computed code today, so per CLAUDE.md §9 the
escape hatch would ship with no test that makes it fire.

**It cannot protect the compiler this project is written to become.** The port's
error codes are a **record field** — `Diagnostic { code: String }`, accumulated
through `@diags: [Diagnostic]` (§1.0, Part 8 wart 14) — not a `T?` error side. The
one place the compiler compares codes against literals, `is_thesis_rule`
(`diagnostics/mod.rs:141`, 13 literals where a typo silently drops a rule from the
published control arm), is `d.code == "…"` on a record and **outside the rule**.
All 25 protected sites are in `examples/`.

**And after M7 the set is not knowable at all.** Compiled against the real header:

```
rc=5  -> code="database_is_locked"            (len 18)
rc=14 -> code="unable_to_open_database_file"  (len 28)   live=0
```

The whole vocabulary lives in libsqlite3, reached through `hero_str_from_bytes`,
with no literal anywhere in Heroes source.

**Nobody has shipped this rule in this shape.** The historian found three teams
that walked to the line and stopped, each for a reason:

| system | domain | verdict shipped |
|---|---|---|
| TypeScript TS2367 | **declared** union of string literal types | hard **error** |
| Rust `unexpected_cfgs` / `--check-cfg` (1.80, May 2024) | inferred closed world from `Cargo.toml` | **warning** — `build.rs` can widen it |
| Erlang Dialyzer | inferred whole-program | **outside the compiler**, opt-in |

Nobody derives the domain by collecting construction sites and makes the mismatch
a hard error. Heroes *could* — it compiles whole programs and has no build
scripts — but the closest ancestor chose `warn`, and its extension mechanism is a
**declaration**, not a suppression. And the design one would otherwise reach for,
OCaml's polymorphic variants, ships this exact bug and documents it: *"if code
that uses `is_positive_permissive` passes in `Float` misspelled as `Floot`, the
erroneous code will compile without complaint."*

**The cure the language already has.** The ergonomist's control arm is the
finding of the session. Write the codes as constants:

```
constant ERR_UNKNOWN_ITEM: str
    "unknown_item"
```

and compare `e.code == store.ERR_UNKNOWN_ITEM`. A typo is `error[unknown_name]`
**today**, locally, with a did-you-mean the resolver already builds, at **zero
spec tokens and zero compiler lines** — and it names the owning module, which is
the one thing the proposed diagnostic could not do without opening another file.
It also answers the question §4.6 leaves open (*which codes can this function
fail with?*), because a module's `ERR_*` constants are its published vocabulary.

The sting: **the convention silently defeats the diagnostic**, since there is no
string literal at the comparison for it to check. *"The rule is strongest where
the code is worst and silent where it is best."*

## Resolution — `provisional — author ratification pending`

**R1 — Part 1 is adopted, and the label changes.** The Rust shape is recorded as
present and complete for v1; nothing in the language moves. §4.6's "acknowledged
technical debt" is amended to cite ASE 2020 and measurement 004 — it is a
measured defect class, not a confession.

**R2 — typed errors stay a Part 8 wart, not a Part 7 item**, amended in place to
carry their real prerequisites (generics on types, refused at design.md:1425;
traits, Part 7 item 8) and their measured price (`?` becomes a conversion; `m[k]`
has no error type; the byte win vanishes for any error a C library can report).
Part 7 is an ordered promise and this is not one.

**R3 — the diagnostic is not adopted.** One veto and three objections, none of
them about the aim. If it is ever built, the five conditions are one design and
they are recorded here as a package:

1. the accept-set is **this module ∪ its transitive `use` closure ∪ the library ∪
   the codes the compiler itself produces** — never per-program, which is
   root-dependent, and never strict per-module, which rejects the four-module
   calculator M8a shipped;
2. the runtime's vocabulary seeds it — today exactly `missing_key`
   (`runtime/parts/failure.c:59`) plus the library's `"not_found"`
   (`library/source.hero:70`), neither of which is in any `fail` the frontend can
   see;
3. the escape hatch is **non-silent and scoped** — to the codes a non-literal
   `fail` could produce, not the whole program — or the +17 licence sentence
   lands and carries panel 012's price;
4. it fires only on a **near miss** (edit distance ≤ 2), so a code arriving from
   `extern` is never accused, and the message **names each candidate's
   construction site**, without which it is a non-local error carrying no
   non-local information;
5. the code joins `is_thesis_rule` (`diagnostics/mod.rs:141`) in the same commit,
   or both mutation arms move together and the control arm measures nothing.

**R4 — what is adopted instead costs nothing: error codes become `constant`s, as
a corpus convention.** No spec text, no compiler change, no new diagnostic — the
resolver's existing `unknown_name` does the work, locally, with the module named.
Queued for **M8e**, whose deliverable is programs, and it arrives with its own
measurement: convert `examples/` and re-run `heroes mutate`. If the ergonomist's
control prediction holds, this closes the class the diagnostic was for, and R3
stays unbuilt on evidence rather than on doubt.

**R5 — five corrections to the record, each found by a judge compiling or reading
rather than arguing.** `examples/gallery/10-maps.hero:51` says the map-miss code
is "not specified" and spec line 133 has since specified it. `mutate/mod.rs:18`
says "the twelve thesis rules" and the list at `diagnostics/mod.rs:141` has
thirteen. CLAUDE.md §7 credits the `_Static_assert` on `HERO_RUNTIME_ABI` with
catching a decoy runtime; measured, a header with `HeroFailure`'s two fields
transposed passes both that assert and `sizeof`, and what actually protects the
build is `Toolchain::runtime_text()`'s cache key. `QUEUE.md:150` and `:183` need
panel 023's byte correction. And `heroes check` returns **exit 0** on `extern
function risky(x: int) -> int?` and `extern function name_of(p: ptr) -> str`,
which only the emit gate refuses — at M7 the first must become a *checker* error,
because measured, `int?` has no stable C spelling (`h_n1_opt0` is `{int64_t ok;}`
and `h_n2_opt0` is `{HeroStr ok;}` — the name is positional).

**R6 — the defect this panel found is fixed and named.** A variant case payload
was never released when no `match` in the same compilation bound it, so the same
`lex.hero` leaked rooted at itself and did not rooted at `parse.hero`. Fixed;
`tests/golden/run/fixedbugs-case-payload-leaked.hero`.

**R7 — the spec is not amended and `SPEC_TOKENS` stays 2434.** Every number here
is a price, spent by nobody.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | with the use-closure formulation and `is_thesis_rule` updated, `typo-code` reads **15/25 strict, 0/25 permissive**, corpus 1087 → 1102 (93% → 95%). The 15 are enumerated site by site in the panel record; materially above 15 means the accept-set is too small and the rule unsound, materially below means the closure is not transitive | M8e |
| compiler-engineer | the implementing diff is **< 200 non-test lines**, **zero** under `lexer/`, `syntax/`, `ir/`, `emit/` or `own.rs`. Any line there means the proposal was misclassified — and the objection becomes a veto | the implementing commit |
| llm-ergonomist | over 20 fresh attempts at a two-failure, two-file task: **≥5/20** contain an `e.code == "<literal>"` that no `fail` constructs (modal mismatch: `missing_key`); **≥2/20** of those are *correct* programs the rule would reject, and **≥50%** of those rejections are "fixed" by applying the wrong suggestion — the diagnostic *creating* a silent wrong program | harness run |
| llm-ergonomist | under R4's constant convention: codes-mismatch **≤1/20**, false positives **0/20**, first-try compile rate unchanged or better | M8e / harness run |
| spec-warden | the rule, if built, reads **14/25** strict — eleven survivors: 7 print-only codes plus 4 whose code has a second constructor in the same file (`calculator.hero:198,200`, `calculator/parse.hero:61,63`), which mutating one does not orphan | M8e |
| ffi-pragmatist | the first `examples/` SQLite binding to reach §4.19 ladder step 3 contains at least one `fail` whose code is not a literal; the diagnostic then goes silent program-wide and `typo-code` returns to **0/25** while that binding is in the corpus | M7 |
| ffi-pragmatist | if typed errors are ever prototyped, `sizeof` of the option type for any binding error carrying a library message is **≥ 40** — never the 16 the queue will be tempted to quote | post-fixpoint |
| historian | when a closed-world check lands, the operator reads exactly **25 − D**, where D is the number of mutants whose code has two or more `fail(` sites in the same program — countable before implementing. A different number means codes cross the `extern` boundary or are assembled | M8e |
| historian | these kills are **compile errors, not test kills**, and Part 11 must report them in a separate column or the row reads "100%" meaning "the mutant never ran" beside a 73% meaning "a test caught it" | M8e |

## What a veto would compel

The spec-warden's veto on Part 3 lifts on its four conditions together, of which
the load-bearing two are R3.1 (the accept-set includes compiler-produced codes,
so spec line 133's own program stays legal) and R3.3 (the entry-point question is
answered by a stated rule, not by which file you point the compiler at). The
other three judges' objections lift on the same package plus the near-miss and
construction-site requirements. The historian would additionally have the rule
ship as a **warning** unless the record states why Heroes' closed world is
actually closed where Cargo's is not — which after M7 it is not, so the warning
level may be the honest one.

The ffi-pragmatist attached a standing veto trigger that belongs to M7 rather
than here: **any wording that lets an `extern` or a shim construct or name a
`T?`**. There is no stable C name for one, so such a signature would be a C ABI
the compiler cannot honour.

## The author's question, and what this panel says back to it

The question was whether to add a milestone for Rust-style error handling. The
answer is that the Rust shape landed at M5d and nobody had said so out loud, and
that the part Rust has and Heroes does not — the typed error — costs three
things this language has decided against, one of which (generics on types) is a
positive rule in §4.12 rather than an omission. Swift shipped it after a decade
and tells you not to use it.

What is real is the hole underneath the question, and it is now measured rather
than suspected: **25 mutants, 0 caught**, against a published 42% of real string
bugs and a static analyser that found 1 in 204. The panel's answer to *that* is
not a language feature and not a diagnostic. It is a `constant`, which this
language has had since M2, which makes the mistake an `unknown_name` error with
the module named, and which costs nothing at all.

**The session's own footnote:** convened about a language question, it found a
miscompilation — the same module leaking rooted at itself and clean rooted at its
caller. That is the second panel in a row to find a live defect in code it was
not looking at.

## Ratification — 2026-08-12, by author instruction

**RATIFIED.** The author's instruction was a blanket one — *"ratifica anche tutto
quello che c'è da ratificare"* — given after reading the session summary, not a
clause-by-clause review of this file. It is recorded that way on purpose: this
project's own rule is that a record must not say more than what happened.

What it settles: the provisional resolution above **stands as the decision**, and
work no longer proceeds on it as a default. Every resolution here had been
load-bearing since the day it landed, so this changes the record's status rather
than the compiler's behaviour.

What it does **not** settle: anything this file keys to a measurement that has not
been taken. Those stay open on their own terms, listed in `docs/debrief/QUEUE.md`,
and a blanket yes cannot make a number arrive.
