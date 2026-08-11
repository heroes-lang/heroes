# Panel 031 — Modules: the namespace, and the sentence that taught the wrong call

**Convened** 2026-08-11, before M8a step 1. **Trigger** surface syntax and
semantics, `spec/**` (CLAUDE.md §4). **Status** `provisional — author
ratification pending`.

## The proposal, verbatim

> Replacing `spec/heroes-spec.md` line 6, which today reads
> `- One file is one program. Entry point: \`function main()\`.`
>
> **VARIANT A** (+64 measured, 2363 → 2427):
> ```
> - One file is one module; the file you compile holds `function main()`.
> - `use "geom"` makes another file's declarations available, always
>   qualified: `geom.dist2(a, b)`, `p: geom.Point`. The path is relative to
>   this file and `.hero` is implied. No aliases, no wildcard, no nesting.
> ```
> **VARIANT B** (+60 measured, 2363 → 2423): the same, with `use geom` and
> `geom.hero` beside this file.
>
> Unchanged: **one whole-program `.c`** (separate compilation is M9, panel
> 030 R1), no privacy, no aliases, no wildcard import, no package hierarchy,
> and the library becomes an ordinary module whose names stay reserved
> built-ins, unqualified and needing no `use`.

The two variants went to the llm-ergonomist label-stripped, as `variant-1.md`
(B) and `variant-2.md` (A), each a complete specification, neither presented as
the status quo or as anyone's preference.

## The verdict table

| judge | verdict | cost / delta | its own finding |
|---|---|---|---|
| compiler-engineer | **object** (approve-with-condition; no veto — the core is untouched) | **+475** non-test lines, −10, on a measured base of 23,293 | panel 030's ≈+330 is low by ~45%: it priced neither discovery, nor the type-position qualifier, nor the `Field` collision — and the deletion it promised does not survive the grep |
| llm-ergonomist | **approve-with-condition**, variant B | one new fact under B, **three** under A (two of them unstated) | the bullet's own example is **illegal as the thing it demonstrates**: `geom.dist2(a, b)` is legal only as UFCS |
| spec-warden | **approve-with-condition**, variant B only | **+60** B / +64 A alone; found +50 and +45 wordings; the adopted form is **+71** | both variants are silent on the one property the port exercises in every file: may a module `use` another, and are names re-exported |
| ffi-pragmatist | **approve-with-condition** | runtime +0, emitted C +0, resolver ~+20 | seven programs compiled: §4.19's guarantee is **intact** under one whole-program `.c`, and the two ways to break it are both optimisations this emitter already performs elsewhere |
| historian (advisory) | **support-with-caution**, slight B | — | nobody who shipped mandatory qualification ever removed it; everybody who relaxed it published a warning about their own relaxation |

## The finding that decided the wording

The ergonomist wrote eight `.hero` files twice, once per variant, and reported
that **exactly one character-level change separated them on the happy path**:
`use geom` → `use "geom"`. A verdict therefore had to be decided at the edges —
and while looking for the edges it found something neither variant was about.

**The proposal's only example is illegal as a qualified call.** Spec line 81:
when two parameters in a signature share a type, named arguments are mandatory
at the call site. `dist2(a: Point, b: Point)` has two `Point`s, so
`geom.dist2(a, b)` is not a legal qualified call at all — it is legal *only* as
UFCS, desugaring to `dist2(geom, a, b)`. The one line teaching qualification was
written in the exact shape that qualification forbids and that UFCS produces,
in both variants, and the judge wrote the *correct* form (`geom.dist2(a: p, b:
q)`) in its own program **against the spec's own example**, then registered the
prediction that ≥30% of readers would copy the example instead.

That is the second time in this project's record that a spec sentence taught a
wrong program (panel 023 was the first), and it was found the same way: by
writing from the document rather than reading it.

## What the other judges found while pricing it

**The concatenation is not a preference, it is 39 against 0.** The
compiler-engineer counted **39 `Span {` construction sites across 14 files** and
**271 `&Source` parameters**. Generalising `Source` from "one text plus a library
boundary" to "N files concatenated plus a per-file table" changes **0 of 39** and
**0 of 271**; a file id inside `Span` changes all 39, pushes `Span` past 8 bytes
and forces `Span::to` to assert file identity. The architecture M5b chose for one
special case turns out to be the architecture for N.

**§4.19 survives one whole-program `.c`, and the reason is exact.** The
ffi-pragmatist compiled a wrong `extern` in a module the caller cannot see:
against the real `sqlite3.h`, three `conflicting types` errors plus one at the
*caller's* own line, because `#line` put `main.hero:9:8` on it. The single TU
contains every `use`d module's `#include` and every `use`d module's prototype,
and C applies compatible-type checking to all declarations of one identifier in
one TU. Modules are erased before clang sees them.

**Both ways to break that are optimisations this emitter already performs
elsewhere.** Deduplicating extern prototypes by C name: measured **exit 0**,
calling through a wrong signature and printing garbage bytes, with the only
`-Weverything` complaint being a C++-compatibility warning. Pruning an unused
module's `#include`: measured **exit 0** printing `6714990092` where the
unpruned form is two clang errors. `emit/decls.rs` prunes twice today.

**A legal Heroes program can still collide at the mangler**, and the code says
otherwise. `emit/mangle.rs`'s module doc asserts that "at M8a a module is a
declared name rather than a file stem, so the shape stops being a
stem-sanitising question at all". That is **false under both variants**:
`module_of` strips `_`, so modules `geo_m` and `geom` produce one component. The
ffi-pragmatist compiled it — `error: redefinition of 'h_geom_Point'` — and
CLAUDE.md §7 routes a clang failure to exit 2, *the compiler is wrong*, on a
program the user was entitled to write. The port carries five `_`-bearing file
names today, `ir/print_inst.rs` and `ir/print_names.rs` among them.

**The record on qualification is one-sided.** Wirth dropped Modula-2's
unqualified-import option in Oberon and recorded the reason as reading benefit;
Java added static import in J2SE 5.0 (2004) and Sun's own guide answers "when
should you use static import?" with "**Very sparingly!**"; PEP 8 warns off
wildcard imports; Ada kept accreting `use` forms through 2012. The historian
searched for a shipped language that made qualification mandatory and later
removed it, and **found none**. On the spelling, Go's quoted path is the only
form with a documented, frozen defect — golang/go#28428 (2018): you cannot tell
from a Go file which import provides which name; the 2024 proposal to re-spell
it, golang/go#68151, reached Final Comment Period and closed *not-planned*. Nim
is variant B with a string escape valve for paths that are not identifiers,
which makes B additive and A's reverse a breaking change.

## The disagreements, unsmoothed

**Panel 030's deletion claim does not survive the grep, and this panel
withdraws it.** 030 said the library becoming an ordinary module collapses
`is_library`, `library_line_of`, `LIBRARY_MODULE`, `writer::LIBRARY_FILE` and
`emit/builtins.rs`'s 35-line reachability walk. The compiler-engineer checked
every one. `library::misplaced` survives and is asserted by a test — a
diagnostic pointing into the library is still a compiler bug. `reachable`
survives **verbatim**, with `is_library` swapped for a module comparison,
because a program that never calls `range` must still not carry it into
`--emit-c`. `is_library` survives as a one-line wrapper over the file table,
which is exactly what keeps its 15 non-test call sites at zero edits. **Net
deletion ≈10 lines**, against a milestone cost of ~+475. The ROADMAP's M8a
entry is amended by this panel.

**The ergonomist and the historian want opposite rules for the same collision.**
A local binding named after a module: the historian recommends D's two-phase
rule — innermost scope wins, the module name only when nothing shadows it — on
the ground that no precedent supplies anything else. The ergonomist measured its
own confidence and found that reading is the *silent* one: under variant A it
rose from 50% to 70% that `geom = Point(...)` is legal and prints 1. The
resolution takes the ergonomist's side, and R3 below buys the loud rule for zero
tokens rather than arguing for it.

**The warden's third condition is not fully met and it says so here.** It asked
that panel 024's delta gate land in this commit or the next, because +71 is the
first commit that would trip the ~+50 threshold the gate was designed around.
The gate is refused **as a command** by CLAUDE.md §10's stopping rule — it types
no fixpoint invocation, no golden harness, no Part 11 harness, which is the same
refusal that struck `outline` and `explain` — and adopted **as a test** (R13).
The warden's objection to the *shape* of that answer stands on the record.

**The compiler-engineer's spec condition is refused, and the alternative
removes the hazard instead.** It asked for a "`use` lines come first" rule so
discovery could be a prefix scan, warning that otherwise discovery lexes every
file twice — "two grammars that can disagree, the exact failure CLAUDE.md §7
names". R8 takes the second horn: discovery lexes each file with *the* lexer,
so there is no second grammar to disagree, and CLAUDE.md §13 forbids the
compile-time argument that would have bought the rule.

## Resolution — `provisional — author ratification pending`

**The spec text, +71 measured (2363 → 2434), headroom 1662:**

```
- One file is one module; the file you compile holds `function main()`.
- `use geom` binds `geom` to `geom.hero`'s declarations, beside this file,
  written qualified: `geom.dist2(a: p, b: q)`, `p: geom.Point`. Every
  module you name needs its own `use`. No aliases, no wildcard.
```

**R1 — Variant B, `use geom`.** Four judges of five, from four inputs. The
qualifier is read off the line rather than inferred from a basename the document
never derives; B makes the two-modules-one-basename collision (`use "geom"` and
`use "shapes/geom"`) *unwriteable* rather than unresolved; and Nim's precedent
makes strings an additive later change where the reverse is breaking.

**R2 — the example is fixed to be legal**: `geom.dist2(a: p, b: q)`,
`p: geom.Point`. Zero tokens, and it removes the strongest wrong-guess
attractor the ergonomist found in the section.

**R3 — `use` *binds*.** The verb is the spec's own (line 66: "`=` binds once,
forever"), and it makes two existing sentences answer the two questions both
judges raised, at **zero** tokens: "Shadowing is a compile error" (line 76)
covers a local named after a module, and "An unused binding or parameter is a
compile error" (line 74) covers an unused `use`. Module names enter the ordinary
namespace, not a third one — otherwise `p = geom.Point(…)` followed by
`geom = 3` is legal and `geom.f()` becomes ambiguous.

**R4 — "Every module you name needs its own `use`."** The transitivity rule,
+2 tokens over the wording without it. It is the property the port exercises in
every file: `lex` and `parse` both naming `token.Token`, with nothing in either
variant, in design.md, or in panel 030 R4 saying whether the second gets it for
free. There is no re-export.

**R5 — UFCS across a module boundary is a diagnostic, not a sentence.**
`p.dist2(o)` where `dist2` is imported: the resolver looks up `dist2`
unqualified, fails, and the *diagnostic* names the module and carries a
`certain` fix. Measured saving 11 tokens (2445 → 2434), and strictly more
informative than the sentence: CLAUDE.md §8 requires the error to carry what is
needed without opening another file. The ergonomist's prediction 3 is registered
against the diagnostic instead of against the spec.

**R6 — module cycles are refused, with no spec sentence.** The discovery walk
needs a `seen` stack to terminate anyway, so refusal is a back-edge check on it
(~15 lines). It is the reversible direction: relaxing later breaks nothing,
while tightening at M9 — where the per-module cache needs a topological order —
breaks the port. Go's own design paper records the same trade and the cost it
names (incremental builds) does not exist yet under one whole-program `.c`.

**R7 — `Source` becomes N files concatenated with a per-file table** (module
name, file name, start offset, line offset). 39 `Span` sites and 271 `&Source`
parameters unchanged; the Cyclone rule untouched; `Span` stays 8 bytes and
`Copy`. The library stops being a boundary and becomes the last row of the
table.

**R8 — discovery re-uses the real lexer.** Read the root file, lex it, collect
its `use` names, recurse, then concatenate and lex once for the pipeline. Same
grammar twice; no prefix scanner; no spec rule about where `use` may appear.

**R9 — three extern rules, all three compiled, and all three M7's to
implement** (the emitter's `FnKind::Extern` gate is still closed):
prototypes **per declaration, in source order, never deduped by C name**;
every `use`d module's `#include`s and prototypes emitted **unpruned**,
independent of the call graph; and two differing extern declarations of one C
name reported as a **Heroes diagnostic with spans in both files**, because
clang's version is exit 2 and would accuse the compiler of a bug on a program
the user wrote.

**R10 — the module-name collision check lands at M8a**, ~20 lines: two modules
in one program whose names sanitise to the same mangler component are a Heroes
diagnostic naming both files. `mangle.rs`'s module doc is amended in the same
commit, because it currently asserts the opposite.

**R11 — the library becomes an ordinary module and ~10 lines are deleted, not a
special case.** Recorded here so the milestone is not judged against panel 030's
number.

**R12 — nominal typing is not written into the spec at M8a.** The ergonomist's
only silent counter-test — a local `record Point` beside a `use`d module with
one — is silent *only if* the language is structural, and it is nominal
(`TypeRef::Top(u32)` is a decl index), so the mistake is a loud type error and
the spec's silence costs a guess the compiler corrects. Cost if the author wants
it stated: ~+15 tokens. Queued, with the dissent recorded.

**R13 — panel 024's delta gate lands as a test, not a command.** A single test
asserts the spec's measured size against a recorded constant, so any spec change
must update that number in the same commit — which is precisely the moment
panel 012's rule asks for a named removal or a registered prediction. ~15 lines,
no surface change, and CLAUDE.md §10's stopping rule is satisfied because the
golden harness types it. Nothing pins that number today (grepped).

**What a veto would have compelled**, had one been available: the
compiler-engineer's objection is to panel 030's *estimate*, not to the work, and
it lifts by recording +475; the warden's veto was unavailable on arithmetic it
ran itself (2363 + 71 + M7's ≥60 = ≥2494 against 4096); the ergonomist's would
have compelled R2, R3 and R5, all three of which are adopted.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | at M8a close, non-test lines in `crates/*/src` exceed **23,700** (from 23,293), and **both** `resolve/exprs.rs` (279) and `resolve/errors.rs` (286) cross 300, forcing a split under CLAUDE.md §11 | M8a close |
| llm-ergonomist | (1) under A, ≥20% of first-try module programs contain a `use` with a `/` or a `.hero` suffix; under B the rate is structurally 0 | harness run |
| llm-ergonomist | (2) across both variants, ≥30% of programs calling an imported function with two same-typed parameters call it **positionally**, copied from the bullet — the prediction R2 exists to falsify | harness run |
| llm-ergonomist | (3) ≥25% of programs contain a `value.f(…)` whose `f` is declared in an imported module (R5's diagnostic is what it will meet) | harness run |
| llm-ergonomist | (4) variant *construction* syntax will be the largest source of first-try failures in any multi-file task using a `variant`, exceeding all module-bullet failures combined — it is absent from the spec entirely | harness run |
| spec-warden | modules cost **≥+62, not +60**: if the wording lands unamended, the spec exceeds **2425** before M8b closes via a module-visibility amendment of ≥+25 | M8p / M8b |
| ffi-pragmatist | at M7, `sqlite.sqlite3_libversion_number()` called from another module compiles and runs with no shim; and a wrong `sqlite3_column_bytes` in that module **that nobody calls** is exit 1 — the second half fails if M7 emits extern prototypes by reachability | M7 |
| historian | the first collision report will be a local or parameter binding whose name equals a module name; falsified if the M8b port compiles with no such collision and no rule needed | M8a / M8b |

The warden's prediction is registered as panel 012's price for a +71 addition
with no removal available: the tier phrase's −6 is parked on a harness that has
not run since M5b, and panel 028 R6 declined to spend it.

## Mortgage arithmetic, so a reader can check it

```
2363  spec today
 +71  this proposal (measured, replacement not addition)
 +60  M7's floor for file I/O · args() · exit(code)   (panel 030 R3)
----
2494  against a 4096 ceiling; 4010 after the 2% spread; ≥1516 free
```

design.md line 2011 recorded cutting modules from v1 as returning ~60 tokens.
+71 overdraws that by 11, and the overdraw is R2's legal example plus R4's
transitivity rule — both bought on a judge's evidence rather than on taste.
