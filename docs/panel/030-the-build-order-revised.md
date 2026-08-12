# Panel 030 — the build order revised, and the half of M8a that was never a language feature

**Convened** 2026-08-11, after M6 close. **Trigger** architecture and design.md
Part 10 (CLAUDE.md §4). **Status** `RATIFIED 2026-08-12` (was `provisional — author ratification pending`).

## The proposal, verbatim

> design.md Part 10 steps 14–15 order the remaining work FFI → (I/O, args,
> modules) → port → fixpoint. This proposal reorders it:
>
> 1. **Modules first** (ROADMAP M8a), *before* the FFI: one `.c` per module, the
>    mangler's `h_<module>_<name>` namespace live, prototypes across translation
>    units, a per-module cache, panel 028 R3b's linkage rule implemented, and
>    panel 029 R5b's open question — where a monomorphised instance used by two
>    modules lives — decided.
> 2. **The FFI second** (ROADMAP M7), absorbing the closure list's last three
>    rows: file I/O, `args()`, `exit(code)` as plain `extern`s (measurement 003
>    rider 3), which CLAUDE.md §7 forbids from reaching a binary before the
>    header verifies them.
> 3. **A probe before the port** (new M8p): the Rust lexer (983 non-test lines)
>    ported to Heroes for real, to *measure* what self-hosting still lacks.
>    Missing forms go to a panel there, not during the port.
> 4. **Concurrency scheduled** (new M9, design.md Part 7.13) *after* the
>    fixpoint, so no language form is written twice; then QBE, then `heroes lsp`.
> 5. **No renumbering**: existing identifiers keep today's meaning in 132 lines
>    of record. The ROADMAP declares that ids are not chronological.

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| compiler-engineer | **accept-with-conditions** | clause 1 bundles a *language* requirement with a *build architecture*: the namespace slice is **≈+330** lines, one `.c` per module plus the cache is **+700–1000**, and §1.0 requires only the first |
| ffi-pragmatist | **veto**, narrow and pre-lifted | compiled it: a wrong `extern` is **5/5 rejected** in the TU holding the header and **0/5** in the calling TU — `sqlite3_errmsg` printed as `6714990092` at exit 0 under `-Weverything`; the per-module cache **converts a §4.19 compile error into a wrong answer** |
| spec-warden | **accept-with-conditions** | clause 2 is refuted by compilation: `extern function exit(code: int)` is `conflicting types for 'exit'` (`int64_t` vs `int`), and `fopen`/`fread` likewise — the three "free" rows are mortgaged at **≥60 tokens**, not 0 |
| llm-ergonomist | **B first** (I/O before modules), **veto** scoped to glob import | the absence of modules cost **scrolling**; the absence of I/O cost a **wrong program** — a Heroes calculator that fails prints an error and tells the shell it succeeded |
| historian (advisory) | **accept-with-conditions** | no project on record has a consequence attributable to the modules-vs-FFI *order* — but the **per-module cache** is Nim's unfinished item since 2018 and the reason Rust shipped 1.52.1 to disable incremental |

## The finding that reorganised the panel

Three judges arrived independently at the same cut, from three different inputs,
and none of them had been asked for it.

The compiler-engineer priced clause 1 and found two milestones inside it: `use`,
qualified names and per-declaration mangling (**≈+330** lines, and it *deletes*
the library's special case — `is_library`, `library_line_of`, `LIBRARY_MODULE`,
`writer::LIBRARY_FILE` and a 35-line reachability walk in `emit/builtins.rs` all
collapse when the library becomes an ordinary module), against one `.c` per
module with cross-TU prototypes and a cache (**+700–1000**, landing in the six
whole-program sweeps at `emit/mod.rs:149–156`).

The ffi-pragmatist, who was asked only whether §4.19 survives separate
compilation, answered by compiling it and found that it does not, unaided: with
`#include <sqlite3.h>` present, three wrong SQLite declarations and two wrong
raylib declarations are all rejected; in a calling TU that sees only a generated
prototype, **all five are accepted**, link, run, and print a `const char *` as a
Heroes `int` at exit 0. The per-module cache is worse than silent — it is
*stale*: with a cache key of `VERSION + level + source name + source text +
runtime_text()` (`toolchain.rs:121-129`), upgrading a library's header while the
module text is unchanged is a cache hit, and the binary prints `1410065408`
where it should print `10000000000`.

The historian, who was asked only for precedent, produced the same boundary from
outside: Nim accepted the per-module-cache RFC in 2018 under a "Nim2020"
milestone and it is still the primary goal of Nim 3; Rust shipped a patch
release to turn incremental compilation *off* because verification caught latent
miscompilations; SQLite ships 238K lines as one translation unit on purpose.

**So the resolution writes itself, and it is more conservative than the proposal
in exactly one place**: modules land as a *namespace*, compiled to one
whole-program `.c`, and separate compilation — the part that carries the veto,
the precedent risk and two thirds of the cost — moves past the fixpoint.

## Resolution — provisional, author ratification pending

### R1 · M8a is the namespace, not the build architecture

M8a lands `use`, always-qualified cross-module names, one module string per
declaration through the mangler, N source files in one `Source`, and the library
as an ordinary module. It emits **one whole-program `.c`**, exactly as today.
Principle 0's closure list says *modules*; it does not say translation units,
and CLAUDE.md §13 forbids compile time as a justification.

Consequences taken deliberately: R5b (where a monomorphised instance lives) does
**not** need answering at M8a, because whole-program monomorphisation still sees
every instance — which matters more than it looks, since six of the library's
seven functions are generic and the first two-module program that calls `map`
would hit it. Panel 028 R3b's linkage rule stays decided and unimplemented until
there are two TUs to link.

### R2 · Separate compilation moves past the fixpoint, and carries four acceptance rows

One `.c` per module, prototypes across TUs and the per-module cache become
**M9**, after M8c. When they land, these are acceptance criteria, not advice —
they are what lifts the ffi-pragmatist's veto:

1. **The header travels with the extern**: every TU that *calls* an extern emits
   that extern's `#include` beside the prototype, not only the declaring TU.
   Acceptable alternative: externs are module-private and reachable only through
   a Heroes wrapper — but then the **type checker** must refuse the qualified
   call, not a convention.
2. **Headers and link flags enter the cache key**, together with each included
   header's contents and `runtime_text()` (already keyed; do not drop it).
3. **Every dependency's emitted interface enters the key** — measured, `-flto`
   does not catch cross-TU signature skew and changes the answer (`3.0` at
   `-O0`/`-O2`, `0.0` under `-flto`, no diagnostic anywhere).
4. **One two-module FFI-shaped golden** with a wrong `extern` signature: exit 1
   required in *both* TUs, `#~` annotated per CLAUDE.md §9.

Until then the fixpoint is cold-cache by construction, which is also the
historian's condition and removes its prediction's whole failure mode.

### R3 · The FFI is second, and the last three closure-list rows are a *route to decide*, not a settled `extern`

Measurement 003 rider 3 assigned file I/O, `args()` and `exit(code)` to plain
`extern`s and concluded "the spec gains **nothing** for them". Compiled against
the real headers with the emitter's own type mapping (`emit/ctype.rs:258–259`),
that is false:

```
error: conflicting types for 'exit'      void exit(int64_t) vs void exit(int) __dead2
error: conflicting types for 'fopen'     FILE * vs void *
error: conflicting types for 'fread'     size_t vs int64_t
```

This is panel 013's 9-of-9 callback finding reproduced on the closure list's own
last three rows: the boundary lacks `c_int`, `size_t` and `const`, which Part 7
item 10 defers. And `args() -> [str]` is a runtime function wearing an `extern`
hat, because §4.20:1815–1817 says no `extern` may return `str` without
`hero_str_from_*`.

Worse, the route that *appears* to work is the one §4.19 forbids: drop the
`#include` and `void *fopen(const char *, const char *)` links by accident on
arm64. That is the silent-wrong-answer class, reached by following the audit's
own advice.

**So the ROADMAP line becomes**: *M7 decides the route for the last three rows —
`extern`, shim, or built-in — and pays the measured spec cost of whichever it
is.* The mortgage is **≥60 tokens** (point estimate 84), and it is the first
real entry in panel 024 R2's mortgage ledger.

### R4 · Modules are always-qualified; glob import is refused

The llm-ergonomist's scoped veto, adopted, and design.md Part 8 item 4 already
says it: cross-module references are qualified (`lex.tokenise(s)`). The reason
is UFCS: `x.f(y)` is `f(x, y)`, and today `f` is either in this file or in the
closed built-in list — both on the page. Under glob import, `f` is determined by
an import list in another file, and the one construct this language relies on
for locality stops being resolvable from the line.

### R5 · M8p reports blockages, not wishes

Adopted verbatim from the spec-warden, and it goes in the ROADMAP as written:

> **M8p reports blockages, not wishes.** Every form the probe proposes arrives
> with three things — the Heroes code that does the same job *without* it, that
> code's line count, and the form's own spec cost quoted from `heroes measure` —
> and a form whose workaround compiles is a Part 7 deferral by default,
> overturned only by §1.0 compiler-need (nothing in Heroes expresses the case at
> all) or a measured Part 11 effect. "The port would be easier with X" is
> evidence for nothing: the workaround compiling is the proof that the compiler
> did not need X.

Two riders on the slice itself. The lexer is the right first file for a reason
the proposal did not give: `lexer/scan.rs` reaches `as_bytes()` at seven sites
and compares bytes, `types/ops.rs:54` refuses `<` on `str`, and the closure list
has **no form** that replaces byte access — so the probe hits a missing-form wall
on file one, cheaply, which is what a probe is for. And the historian's
condition, adopted: the lexer has zero `BTreeMap` and zero closures, so **the
probe's findings are a lower bound, not a measurement**, and the ROADMAP says so.
If the byte wall falls early, a second file that exercises maps, recursive
variants and generics (`types/` or `ir/verify.rs`) is added before M8b opens.

### R6 · Post-fixpoint entries are `scheduled, no warrant`

M9 (separate compilation) is warranted by the closure list — it is the second
half of a row Principle 0 names. M10 (concurrency), M11 (QBE) and M12 (`heroes
lsp`) are **not**: Part 7's preamble defers everything on its list until the
closure list compiles itself, and a milestone number is not a warrant. Each
carries the label, and measurement 003 rider 3 is cited as the precedent —
the ROADMAP scheduled `outline` and `explain` and the stopping rule refused them.

The historian's cfront rider, adopted: **before M8c** the record states whether
the C11 backend can express the intended concurrency model at all (stack
switching in the runtime, or a CPS/state-machine transform). cfront — the direct
ancestor of this architecture — was abandoned in 1993 after a failed attempt to
add exception support, having frozen around forms that could not carry non-local
control flow. If the answer is "not yet known", the deferral is a bet and is
logged as one.

### R7 · No renumbering, and the ROADMAP carries an explicit order table

Confirmed by precedent rather than by preference: RFC numbers are never
reassigned and gaps simply remain; PEP numbers "once assigned are never
changed"; LLVM renumbered its issues in the Bugzilla→GitHub migration and pays
permanent rent in redirect infrastructure so old citations resolve. The
condition both the historian and the compiler-engineer attached: the chain is an
**explicit execution-order table** at the top of the section — never "file order
is chronological", which is a rule a reader has to be told twice.

### R8 · One rider filed, not scheduled here

§4.20's single allocation point is stated as an invariant that is "already true"
and is measured false today: `malloc` appears at `runtime/parts/str.c:53`,
`array.c:46`, `map.c:80` and `sort.c:100`, with no `hero_alloc` wrapper. Part
7.13 depends on it and says a second allocation site discovered later is a
redesign. Repair is ~15 lines plus 8 call sites. Filed to the queue against M7,
because this panel's subject is the order, not the runtime.

## What the veto would have compelled

Had the ffi-pragmatist's veto stood against the proposal as written — one `.c`
per module in first position — M8a would have had to answer, before its first
commit, which TU carries each `#include`, what invalidates a cached object, and
which of the six whole-program sweeps at `emit/mod.rs:149–156` each artifact
belongs to. R1 removes the question instead of answering it: with one
translation unit there is no calling TU without the header and no cache to go
stale. **The veto is lifted by a change, not by an argument** — the fourth time
in this project's record, and the third time in the last three panels.

## The disagreement that is not resolved

The llm-ergonomist judged **I/O first**, and its evidence is the strongest single
datum in the session: writing three programs from the spec alone, the absence of
modules produced a long file, and the absence of I/O produced a **wrong
program** — `main` has no way to exit non-zero, so a calculator that fails prints
its error and reports success to the shell. It also found the hallucination
class the spec invites today: `extern function read_file(path: str) -> str` is
well-formed Heroes, and its near-misses (`puts`, `system`, `getenv`, `atoi`
taking a Heroes `str`) are real symbols that link, run, and are silently wrong.

The author's order stands — this panel does not overturn an author decision —
but the finding is recorded at full strength, and two things follow from it that
are cheap enough to do anyway: R4 (qualified names, so modules do not push the
FFI boundary off the line), and a queue item asking whether `main` should be
allowed to fail before M7 rather than after.

## Predictions to score

| # | judge | prediction | scored at |
|---|---|---|---|
| 1 | compiler-engineer | with the namespace-only slice, `emit/` grows **<150** non-test lines from 4,093 and none of `perfn.rs`, `descriptors.rs`, `types.rs`, `mono.rs` gains a per-TU-ownership parameter; with one `.c` per module it exceeds **4,700** and at least two of them do | M8a close |
| 2 | ffi-pragmatist | at M7 with the header present, **≥5 of 5** mis-declarations (3 SQLite + 2 raylib) are exit 1; in a calling TU without the header, **0 of 5** — and SQLite ladder step 3 passes green while `sqlite3_errmsg` returns a pointer as an `int` | M7, then M9 |
| 3 | ffi-pragmatist | with the current cache-key formula, `brew upgrade raylib` between two builds yields a binary from a stale `.o` with zero diagnostics, reproducible in under a minute | M9 |
| 4 | spec-warden | file I/O, `args()` and `exit(code)` will **not** reach a Heroes program as plain `extern`s: at M7 close ≥2 of 3 are built-ins or shim calls and the spec gains **≥60** binding tokens. Falsified by one M7 golden that reads a file, prints `args()` and calls `exit` through `extern function` only, with the real `#include`, no shim, spec byte-identical | M7 close |
| 5 | spec-warden | modules land under **240** spec tokens (panel 024 prediction 2 re-registered): measured **+150** in this order against +185 in the other | M8a close |
| 6 | llm-ergonomist | on any task naming a file, an argument or an exit status, the current spec yields **≥1 fabricated `extern`** per task and ≥40% of those name a real libc symbol taking a `str` — landing I/O as typed built-ins drives that to ≈0; landing modules first changes it by **0** | next harness run |
| 7 | historian | if the per-module cache lands before the port and is warm during it, the **first fixpoint attempt fails at least once** for cache staleness — `diff B.c C.c` non-empty, empty after deleting the cache with no compiler change | M8c (voided by R2, which moves the cache past it — record the void) |
| 8 | compiler-engineer | R5b's instance-placement work alone exceeds the whole `use`+resolver cost | M9 |

## Watch list

- **§4.19 never says which translation unit carries the `#include`**, because
  §4.1 says there are no modules in v1 and the question could not be asked. That
  sentence is owed to design.md before M9, and R2 row 1 is its content.
- **`main` cannot fail.** Found by the llm-ergonomist as a live consequence, not
  a hypothesis. Whether it waits for M7's `exit(code)` or gets a language answer
  earlier is undecided here.
- **Panel 024's predictions 1 and 3 are re-registered on this order** by their
  own author: prediction 1 (next five amendments ≤ +150) is consumed entirely by
  modules in one amendment, and prediction 3 (< 2600 through 2026-09-30) lands at
  **2597** if modules and the I/O route both ship in that window — three tokens
  of margin.
- **Spec line 6** (`One file is one program`) is falsified by modules, and its
  rewrite is **+12 longer**, not a deletion. Panel 024's "zero removals in seven
  amendments" survives its eighth.
- **The probe's lower bound.** If the byte-access wall is the only finding, the
  measurement is incomplete by construction and a second file is owed.

## DESIGN-LOG

`the build order revised — M8a is the namespace (one whole-program .c), the FFI
second and it decides the route for I/O/args/exit, a probe before the port,
separate compilation and everything in Part 7 after the fixpoint | Part 10, §4.19,
§1.0 | 030`

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

## Amendment to R7 — 2026-08-12, by author instruction

R7 was ratified earlier the same day, so this is an amendment to a **ratified**
resolution, not a verdict on a provisional one. It is taken by author instruction
under CLAUDE.md §4, which reserves the panel for the language — `spec/**`,
design.md Parts 1–11, surface syntax or semantics, a diagnostic class, architecture
— and amends the process by instruction. A milestone identifier is process: it
appears in no program, no diagnostic and no artifact.

**R7's prohibition stands, and its scope narrows: milestone identifiers stop being
numbers.** `M8a` becomes `M-module-namespace`, `M7` becomes `M-ffi-ladder`, `M8c`
becomes `M-selfhost-fixpoint`; the full map is `docs/ROADMAP.md` § The names, and
the algorithm that assigns the next one is CLAUDE.md §14.

**Why this is not the thing R7 refused.** R7's three precedents are exact against
*reassigning* a number: RFC numbers are never reassigned, PEP numbers "once
assigned are never changed", and LLVM pays permanent rent in redirects. What all
three share is that a reassigned identifier **still resolves, to the wrong thing** —
silently. A rename into a disjoint namespace fails in the opposite direction:
`M8a` in this file resolves to nothing in any living document and to exactly one
row in the alias table. It fails **loudly**, and unlike a number a name cannot be
handed to a different milestone later. The record is therefore not rewritten — this
file, `DESIGN-LOG.md`, `docs/journal/`, `docs/measurements/`, `docs/defects/`,
`docs/book/beats.md`, the golden fixtures, every commit subject and the twelve tags
`m0`–`m6` and `m8a` all keep the numbers they were written with.

**R7's condition survives and is strengthened.** The explicit execution-order table
stays where R7 put it. What goes is the paragraph above it: a name makes no claim
about position, so there is nothing left to apologise for, and R7's own objection —
that "file order is chronological" is *"a rule a reader has to be told twice"* —
is retired rather than restated. The ROADMAP said it three times.

**The alternative that was evaluated and refused: a numbered RFC/PEP-style series**
(`HEP-001`), which is the scheme R7 cited approvingly. Refused because the event
that broke this project's numbering was a **reorder**, not an insertion — this panel
moved M8a ahead of M7 — and no identifier that encodes order survives a reorder;
gapped numbering covers insertions only. Reorders remain likely: nine of the
sixteen rows carry `scheduled, no warrant`, which is this file saying they may be
reordered or dropped. And a number invites the order inference the apology existed
to deny, where a name does not. The one real advantage of numbers, tags that sort,
is `git tag --list --sort=creatordate`. The numbered scheme keeps the place where it
fits: `docs/panel/NNN-slug.md`, gaps included, because panel order *is* creation
order and is never revised.

**Appending to this file, or to any dated record, uses the vocabulary it was
written in** — with the new name in parentheses on first use. The prediction rows
below are scored as `M8a close (M-module-namespace)`.
