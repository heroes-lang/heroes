# Panel 033 — Visibility, and the three tiers of which only one was ever a language question

**Convened** 2026-08-12, after M8a, by author instruction ("senso aggiungere una
milestone per gestire la visibilità all'interno sia dei record, sia dei variant,
sia dei moduli, degli elementi… un po' alla Rust").
**Trigger** surface syntax and semantics, `spec/**`, design.md Parts 1–11
(CLAUDE.md §4).
**Status** `RATIFIED 2026-08-12` (was `provisional — author ratification pending`).

## The proposal, verbatim

> Heroes has no visibility of any kind: every top-level declaration of a module
> is visible to every module that `use`s it (`resolve/top.rs:81` inserts
> unconditionally), and record fields and variant cases are unrestricted. Three
> tiers, judged **separately**:
>
> **F** — private record fields: a field marked private is unreadable outside
> its declaring module.
> **C** — private variant cases: a case marked private is unmatchable outside
> its declaring module.
> **D** — private declarations, in two directions whose spec cost measures
> **identical**, +30 (2434 → 2464):
> **D-P** (opt-in) — `A declaration may start with `private`, which hides it
> from every other module; `private` is not written on fields or variant cases.`
> **D-E** (default-private) — `A declaration is visible only inside its own file
> unless it starts with `export`; `export` is not written on fields or variant
> cases.`
> **S** — stay as today. Zero.

The ergonomist received D-P and D-E as `v-1` and `v-2`, label-stripped, with
`v-0` (today's language) among them and unmarked. Every judge was told the tiers
were separable and that any could be adopted, refused or deferred on its own.

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| compiler-engineer | **veto F · veto C · defer D · approve S** | tier D is **~80 non-test lines and zero in `ir/`, `own.rs`, `emit/` and the mangler** — visibility has no linkage consequence in a one-TU compiler, so it is a filter on a lookup, not a construct. And it found **two live defects** in the machinery the tier would extend |
| llm-ergonomist | **object · keep v-0** (no veto) | wrote the tokenizer three times: **six guesses were the spec's, not the proposal's, and every one of them fails loudly**. v-1's own guess is the one that cannot fail at all — omitting `private` produces no signal, ever |
| spec-warden | **veto D-E · veto F · veto C · defer D-P** | the argued effect and the priced text are **different texts**: the +30 sentence does not buy the dead-code rule, which is a further sentence (+27 for both, measured). And the +30 itself is a **formatting artefact** — +28 appended, +31 wrapped |
| ffi-pragmatist | **veto D-E · veto F · veto C · defer D-P · adopt S** | compiled it: **`private` cannot mean `static`** — clang refuses it against the real header for `sqrt` and `sqlite3_open` — and the other reading links a **wrong** signature to the real symbol and runs at exit 0. A C shim read the "private" field at offset 16 and built the "private" case |
| historian (advisory) | **refuse F · refuse C · refuse D-P · defer D-E · approve S** | **no shipped language makes an unused top-level declaration a hard error**, and every one that merely detects them shipped an escape valve. Rust *had* private variant cases and deleted them in 2014 |

Five seats. **F and C drew three vetoes and a refusal each, from four
independent inputs. D-E drew two vetoes.** Nobody voted to adopt anything.

## F and C are not visibility features, and four judges said so from four directions

**Tier F was already decided, by name, in the document this panel is amending.**
design.md:1142 — *"No methods (UFCS covers it), no inheritance, **no private
fields**, no default values."* A spec sentence contradicting design.md is a
defect and not an addition (CLAUDE.md §12), so F could not have landed without an
amendment the proposal never wrote.

What the judges added is *why* the sentence is right, and none of it is taste.

The **compiler-engineer** priced the cascade at ~90 lines and named the shape:
`check_named_fields` (`types/construct.rs:159-197`) demands **every** field, so a
hidden field makes the type unconstructible from outside — a private field is not
a private field, it is an *opaque type*, which needs constructor functions the
language does not have. It also found that the `known` lists at
`types/access.rs:42,66` would **print the private field's name in the message
that refuses it**, and that CLAUDE.md §7's generated `eq`/`hash` walk fields
rather than bytes, so a hidden field still decides equality for every module — a
spec sentence the proposal owed and did not write.

The **ffi-pragmatist** stopped arguing and compiled the thing:

```
shim reads the PRIVATE field: secret = 4242 at offset 16 of 24 bytes
shim WROTE the private field: secret = -1
C built the PRIVATE case; Heroes describe() answered 9999
```

with `nm -gU` showing the "private" case's `_h_vis_Reply_c_internal_eq` and
`_hash` as global `T` symbols. §4.20 fixes a record as a C struct by value, so
**privacy stops at the ABI**: in a language whose founding constraint is that
everything comes from C, a hidden field is a fiction the linker contradicts on
the first shim anybody writes.

The **historian** found that the precedented form is not the private field at
all. Haskell 2010 §5.2 — *"The form `T` names the type but not the constructors
or field names. The ability to export a type without its constructors allows the
construction of abstract datatypes"* — and Ada 83's private part ran for twelve
years with no methods, operations being ordinary subprograms, which is
structurally Heroes' UFCS. Both hide the **type**, never the field. And Zig, the
closest living relative of this architecture, has `pub` on declarations and
**refused field privacy outright** (issue 9909, closed not planned).

**Tier C is the one every judge refused hardest, and Rust ran the experiment.**
Rust shipped per-variant privacy (`pub enum Foo { Bar, priv Baz }`) and **deleted
it** in RFC 0026 (31 March 2014) as *"a rarely used feature… generally not
regarded as a strong enough feature to justify the `priv` keyword"*. It
re-introduced the capability five years later at **whole-type** granularity —
`#[non_exhaustive]`, RFC 2008, stabilised in 1.40.0 on 19 December 2019 — and
Rust's own documentation records the price: *"The `#[non_exhaustive]` annotation
forces matches to use wildcards, so exhaustiveness checking cannot be used to
ensure that all fields/variants are matched explicitly."* The compensating
`non_exhaustive_omitted_patterns` lint has been unstable since October 2021.

Heroes cannot even buy that escape hatch, because `_` is forbidden on variants
(spec:106-107, design.md:993). The engineer stated the consequence in the
project's own currency: under C, **whether a line compiles depends on a
declaration the reader is forbidden to read**, and adding a private case silently
reroutes every outside `match` into its `_`. That is §1.3's non-locality and it
is precisely what `types/patterns.rs:1-15` says the `_` ban exists to prevent.
The IR cost is genuinely zero — `ir/matches.rs:126-132` already fills unclaimed
edges — and the engineer refused to pretend otherwise: **the cost is not lines,
it is the invariant.**

## D is a real question, and it lost on Principle 0 rather than on cost

Tier D is cheap. The engineer priced D-P at **~80 non-test lines**, all frontend,
**zero** in `ir/` (4,487 lines), `own.rs` (309), `emit/` (4,072) and
`emit/mangle.rs` (176) — because `emit/mod.rs` emits one whole-program
translation unit, so visibility never reaches linkage and the mangler never
learns the word. In §1.7's terms it is a filter on a lookup. The budget does not
bite either: 2434 of 4096, and +30 is 1.8% of the headroom. **No judge objected
on cost.** They objected on the burden.

**The thesis argument the proposal offered is wrong, and the warden showed why
with a grep rather than an opinion.** The argument was that `private` lets
§4.4's unused rule reach the top level, since a private declaration nobody calls
is provably dead where a public one is not. But Heroes compiles **one whole
program** from `main` (`modules/mod.rs:6`), so a declaration nobody calls is
*already* computable today — `unused_use` (`resolve/top.rs:119`) proves exactly
that shape at zero spec tokens. Privacy only makes the check **module-local**,
which starts mattering when separate compilation exists: M9, post-v1.

Three judges then closed the remaining escape routes independently.

- **The priced text is not the justifying text.** The warden measured the
  licence alone at **+18**, and licence *plus* the unused rule at **+27** — so
  the proposal's +30 wording buys a prohibition clause ("not written on fields or
  variant cases") worth 40% of its price, and does **not** buy the effect it was
  argued from. Panel 023's rule ("the spec pays for the licence, never the
  prohibition") and panel 031 R5 (a rule the compiler states loudly at the call
  site is not worth a sentence) both point the same way.
- **The rule would not be a simplification.** The engineer priced the dead-code
  sweep at ~40 further lines and answered §1.7's own criterion: it **adds** an
  arm rather than removing a special case, and under D-E it adds two exemptions.
- **Nobody has ever shipped it.** The historian checked Go, Rust, OCaml, Nim and
  Zig: rustc's `dead_code` lint is literally defined on visibility (*"detects
  unused, **unexported** items"*) but is **warn-by-default**; Go's spec stops the
  unused rule at the function body deliberately; OCaml's warning 32 is off by
  default; Nim's is a hint whose `--hintAsError` form crashed the compiler until
  1.6.14 (June 2023); Zig's request was closed. **Every one of them shipped an
  escape valve.** Heroes already has three — `???` (§4.16), `_`, and the
  `use`-binding rule — and the historian predicts a fourth would be owed.

## The direction split, stated plainly rather than smoothed over

This is the panel's one real disagreement and it does not resolve, because the
two sides are answering different questions.

**The historian is for D-E, on ancestry, and the argument is strong.** Wirth's
own HOPL III paper records the arc: Modula-2 PIM2 (1983) required
`EXPORT QUALIFIED`; PIM3 (1985) **deleted** it as redundant given a definition
file; Oberon (1988) deleted the definition file and reinstated a per-declaration
`*` mark. On the same page Wirth writes that dropping qualified import *"actually
turned out to be of great benefit when reading programs"* — and Heroes has
already independently re-derived that half at panel 030 R4. It is standing at
exactly the point in the arc where the answer was `*`, and Oberon, Nim, Go, Zig
and Erlang all landed on that side. Rust reversed **towards** private-by-default
(RFC 0001, 2014: 2036 public fields → 1602 `pub` annotations in one repository);
Swift reversed the same way (SE-0117); **no language in the list reversed away
from it.**

**The engineer, the warden and the ffi-pragmatist are against D-E, and they
counted.** Over `crates/heroes/src/`: 246 file-private `fn` plus 10 file-private
types against 257 cross-visible `fn` and 98 cross-visible types — so under panel
032's flat layout **D-P writes `private` 256 times and D-E writes `export` 355
times, 39% more**, on the tree the port produces. D-E also needs two exemptions
the compiler does not have: the library's seven functions are reached by
`top_visible`'s fallback rather than the qualified path, and `main` is found by
name at `emit/mod.rs:94` and called by nothing. D-P is backwards compatible; D-E
invalidates every existing multi-module program and, as the warden put it, is
**the unreversible direction** — the exact ground on which Part 7 item 5 defers
`alias`.

And the ffi-pragmatist supplied the argument nobody else could: a **binding
module is public by construction**. Measured on real headers, D-E costs one
`export` per line on 20 of 20 libm declarations and 287 of 287 SQLite ones — *"the
per-call glue §1.11 refused, moved to declaration sites"* — for **zero bytes of
object code**.

The ergonomist, who saw the two blind, split the difference in the most useful
way available: **v-2 is mechanically the better sentence and v-1 is the safer
one.** Under v-2 the permission signal is a *presence* (`^export` greps
positively) and the compiler answers the visibility question for you; under v-1
it is an *absence*, and the keyword's omission produces no diagnostic ever — *"a
feature that never fires"*. But v-2's three under-specified cases become
**mandatory in every multi-file program**, where v-1's are opt-in. Its predicted
failure mode is `v2/lex_forgot_export.hero`, which it wrote for real before
catching it: **export the verb, forget the noun** — you export `tokenize` because
it is what you call, and forget `Token` because the other file only *names* it.

## Two live defects, compiled rather than argued

Both were found by the compiler-engineer inside the machinery tier D would
extend, and both are **shipped today**. Reproduced independently before this file
was written.

**D1 — §4.16's hole exemption is program-wide, not file-wide, since M8a.**
`Resolved::has_hole` is a flat scan of `ast.exprs` (`resolve/mod.rs:226`), and one
`Ast` has covered every module since `Source` became N files. So a `???` in
`geom.hero` silences `unused_binding` in `main.hero`:

```
$ heroes check main.hero          # geom.hero contains a ???
hole at main.hero:8:12
exit=0
$ heroes check main.hero          # the same main.hero, geom.hero completed
error[unused_binding]: `unused_here` is bound and never read
exit=1
```

The doc comment at `resolve/mod.rs:169-172` says *"while a `???` is still in the
file"* and is now false. It matters beyond itself: **any top-level dead-code rule
inherits it**, so one unfinished module would silence dead-code reporting across
a whole program — which is a second, independent reason the tier's own thesis
argument does not stand up.

**D2 — the hole report names the wrong file and a line that does not exist.**
Same run: the hole is at `geom.hero:2:12` and it is reported at
`main.hero:8:12`, in a file five lines long. `types/holes.rs:34-35` assembles the
triple itself —

```rust
let (line, col) = src.line_col(hole.span.start);
out.push_str(&format!("hole at {}:{line}:{col}\n", src.name));
```

— where `Source::locate` exists for exactly this and `source/mod.rs:219-232`
documents it as mandatory: *"there is one function and no caller assembles the
triple itself."* This is M8a's own defect, surviving because **the hole report is
not a `Diagnostic`** and the sweep went through the diagnostics.

**D3 — a defect tier D would have shipped, recorded so it is not rediscovered.**
`Resolved::module_declaring` (`resolve/mod.rs:198-203`) searches the whole table
with no filter, and `resolve/qualified.rs:73-83` turns it into `needs_qualifying`
whose fix is `Certainty::Certain` (`errors_modules.rs:88-93`). Under any tier D,
a private `helper` would produce a **machine-applicable fix that writes a program
that does not compile** — against CLAUDE.md §8 and against the invariant
`errors_modules.rs:96-98` states in its own words.

## The finding that dissolves tier D's only warrant

Before this panel, the one place the record needed visibility was M9's first
acceptance row (`ROADMAP.md:658-659`, panel 030 R2): *"the header travels with
the extern into every calling TU, **or** externs are module-private and the type
checker refuses the qualified call."*

The ffi-pragmatist compiled all three shapes and found that **neither tier gives
M9 what that row asks for**: D-P leaves public externs as the default, so row 1's
primary obligation stands in full; D-E makes privacy the default but keeps
`export`, and `export` is the common case for a binding module, so the failure
returns. Measured:

| shape | `db` TU | `main` TU |
|---|---|---|
| A — header in **both** TUs (row 1 honoured) | exit 1 on a wrong signature | **exit 1** |
| B — header only in the declaring TU | exit 1 | **exit 0**, `rc=0 db=open` — with the wrong signature |
| C — extern module-private, `main` calls a Heroes wrapper | exit 1 | nothing to get wrong |

The rule M9 actually needs is **smaller than any tier and spends zero visibility
tokens**:

> An `extern` declaration is never callable across a module boundary. A
> qualified call to one is a type error; the route is a Heroes function in the
> declaring module.

That is shape C, which compiled, linked and ran. It needs no `private`, no
`export` and no storage class — and it removes the last reason to reach for a
visibility keyword before the fixpoint.

**Two further ffi findings, both corrections to the record.** `private` cannot
mean C `static`: clang answers `static declaration of 'sqrt' follows non-static
declaration` against the real header, and the fallback reading — hide the extern,
drop the `#include` — produces `warning: function 'sqlite3_open' has internal
linkage but is not defined`, links to the real global symbol anyway and **runs at
exit 0 with a wrong signature**, where the same declaration *with* its header is
exit 1. And on `examples/calculator`, where 29 of 36 declarations (80%) are
module-private, emitting them `static` takes the build from **0 warnings to 4**,
reproducing panel 028 R3b's projection at 1 warning per ~100 lines.
**One objection withdrawn**: panel 029's finding that `static` breaks
function-value pointer identity does **not** transfer to tier D — measured at
`-O0`, `-O2`, `-O2 -flto` and `-Wl,-dead_strip`, `a.helper == a.helper` is true
and `a.helper == b.helper` is false every time. A private function has one
definition; a monomorphised instance has N.

## Resolution — `provisional — author ratification pending`

**R1 — Tier F is struck, permanently, and moves onto Part 6.** design.md:1142
already refuses private fields inside §4.9's prose; this panel puts the refusal
on the list that means *never*, with its reason. Three vetoes on three
independent grounds: it is an opaque type wearing a field annotation's name
(construction and the `known` lists), the precedented form hides the type and
never the field (Haskell 2010 §5.2, Ada 83, Zig 9909), and **it does not exist in
the artifact** — a shim read the field at offset 16 and wrote it.

**R2 — Tier C is struck, permanently, and moves onto Part 6.** Rust shipped it,
deleted it in 2014, re-added it per-type in 2019, and documents the cost as the
loss of exhaustiveness with the repair lint unstable four years later. Heroes
cannot pay that cost: `_` is forbidden on variants, and the coupling in
`types/patterns.rs:1-15` is what makes exhaustiveness mean anything.

**R3 — Tier D is neither adopted nor refused: it becomes a Part 7 item, and the
instrument that decides it is M8p.** Principle 0's burden is unmet — not on
§1.0's closure list, no Part 11 measurement, and the §1-derived argument offered
is refuted by `unused_use` existing at zero tokens. ROADMAP:602-609's rule
applies literally: *a form whose workaround compiles is a Part 7 deferral by
default*, and the workaround compiles — `examples/calculator/` is four modules
with zero visibility.

**R4 — if the author overrules, the form is fixed here and it is not the tabled
one.** The landing form is **D-P at +18 measured** (the bare licence), never +30:
the "not written on fields or variant cases" clause is a prohibition, and R1/R2
put both prohibitions on Part 6 where they cost the spec nothing. If the dead-code
rule is wanted with it, the pair is **+27 measured** — and it must be written as
one amendment, because the +18 sentence does not buy it. **Never D-E before the
port**: it is the unreversible direction, it invalidates every existing
multi-module program, and it taxes exactly the module §1.11 and M10 mandate.

**R5 — M9's extern row gets its own rule, independent of any tier.** *An `extern`
declaration is never callable across a module boundary; a qualified call to one
is a type error.* Zero visibility tokens, compiled and run as shape C, and it
satisfies panel 030 R2 row 1 whether or not visibility ever ships. Queued for
M9 rather than landed here — this panel was convened about the language, and
this is a rule about `extern` that M9's own acceptance rows should carry.

**R6 — D1 and D2 are repaired now, with a case named after each** (CLAUDE.md §9's
`fixedbugs` rule). They are live defects in shipped code, found by a panel
convened about something else, which is the panel working rather than the panel
slipping. D3 is recorded against R4: if tier D ever lands, `module_declaring` is
the call site that must learn the filter, or `--apply` writes a broken program.

**R7 — the spec is not amended, and `SPEC_TOKENS` does not move.** It stays
**2434**. The measurements in this file are prices, spent by nobody.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | the ported lexer compiles and passes the Rust lexer's own tests with **zero visibility of any kind**, so visibility does not appear on M8p's blockage list. If any tier lands first, `resolve/` exceeds **2,050** non-test lines (from 1,973) and a new `tests/golden/check/` case is required for the `module_declaring` Certain-fix path | M8p |
| llm-ergonomist | v-1 vs v-0 first-try compile rate is **within ±2 points** (it adds no obligation, so it can neither cost nor buy); v-2 vs v-0 is **−8 to −15**, with ≥75% of new failures "name not exported" and **≥40% of those the type, not the function** | harness run |
| llm-ergonomist | under v-1, **≥60%** of generated `lex.hero` files contain **zero** `private` markers — i.e. the sentence buys under 40% of its stated benefit at full spec price | harness run |
| llm-ergonomist | both variants: **0%** silent behavioural divergence. No missing or extra marker changes what a program that compiles *does* — qualified names cannot misresolve | harness run |
| spec-warden | `docs/measurements/004-selfhost-readiness.md` records **zero** visibility blockages, and of the 703 module-private Rust declarations **0** produce a collision or a wrong call when ported public | M8p |
| spec-warden | the spec measures **≤2494** at M8p close (2434 + M7's ≥60 mortgage). Falsified above it, or by any removal-free amendment | M8p |
| ffi-pragmatist | if tier D lands in either direction *and* is implemented as C `static`, M9's two-module FFI golden does **not** produce exit 1 in both TUs: under D-E it is **exit 2** (`static declaration of 'sqlite3_open' follows non-static declaration`) — the compiler blaming itself for a language rule | M9 |
| ffi-pragmatist | under R5's extern rule, SQLite ladder step 3 needs **one Heroes wrapper per called function and still no C shim** | M7 |
| historian | fewer than **40%** of the port's top-level declarations are referenced from outside their own module — so D-E annotates a minority and D-P a majority, settling the direction by count. Above 60%, the reading flips to D-P | M8p / M8b |
| historian | tier D plus a dead-code error fires on a declaration used only by a `test` block or one build path, and requires a **fourth** escape valve beyond `???`, `_` and the `use` rule. Refuted if the port completes with no such site | M8e / M8b |

## What a veto would compel

F's and C's vetoes are not liftable by wording and the resolution strikes both.
F lifts only on an author amendment striking design.md:1142 **plus** a written
answer to what `==` means when it compares a field the caller cannot name, and a
mechanism by which the field is absent from the struct a C shim sees — which
§4.20 appears to forbid. C lifts only on naming which shape is taken (`_`
relaxed, or unmatchable-outside) **and** a Part 11 measurement in which a private
case raises first-attempt compile rate.

D-E's two vetoes lift only if `export`/`private` is proven to have no effect on
emitted linkage or on which `#include`s an extern's TU carries, **and** `extern`
is exempt from the default — at which point D-E and D-P differ only in spec text
and the 355-vs-256 count decides. D-P's defer becomes adopt on an M8p finding
that the port cannot proceed without it, under ROADMAP:602's rule (the Heroes
code that does the same job, its line count, and its `heroes measure` quote), or
on a Part 11 delta. The ffi-pragmatist attached three conditions to that, all
testable: privacy is a **type-checker** fact and `emit/decls.rs::signature` emits
no storage class ever; `private` on an `extern` is a **parse error** with its own
code and a `#~`-annotated golden; and R5's extern rule is written independently.

## The author's question, and what this panel says back to it

The question was whether visibility deserves a milestone. The panel answers that
it is **three questions, and two of them were never visibility questions**: a
private field is an opaque type, a private case is `#[non_exhaustive]`, and both
are refused here for reasons this project had already written down and one it had
not — that in a language whose values are C structs by value, privacy is a source
fiction the first shim contradicts.

The third is a real question with a cheap answer, and it is deferred for the
reason panel 032 gave two days earlier about subdirectories: **the instrument
that should decide it has not run.** M8p exists to report what self-hosting
lacks, under a rule that says *blockages, not wishes*, and the historian's count
— fewer than 40% of the port's declarations referenced from outside their module
— is the number that would settle even the direction. Spending a keyword now
pre-empts the measurement and, as the warden observed, would convert panel 031's
registered prediction into one this panel fulfilled by voting for it.

**One thing changed today that is worth more than the tier.** Panel 033 was
convened about a language feature and it found two shipped defects, one of which
— a hole in one module silencing diagnostics in another — is exactly the failure
mode the deferred feature's own thesis argument depended on not existing.

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
