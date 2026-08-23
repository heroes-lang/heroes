# ROADMAP — what is next

Why this file exists: autonomous work sessions need the goal chain **in the
repo**. It is the distillation of the approved bootstrap plan (revision 2,
reviewed by panel 000); the full rationale for the build order is design.md
Part 10, and the language-level acceptance criteria live in design.md Part 0.
Revision 2 had deleted the ROADMAP ("tags say where you are"); it returns
because tags say where you *are*, not what is *next* — and "what is next"
must not live outside version control.

**This file is about what is next.** A closed milestone's record — its status
paragraph, its inventory, what it carried forward — lives in its own journal,
indexed at `docs/journal/README.md`. That split exists because this file had
accumulated 512 lines about the past before its first line about the future, and
was growing about 66 lines per close; `/step`'s checklist now keeps § Status at
**≤15 lines** and sends the closing block to the journal.

## Status

**M-separate-compilation is open (2026-08-19), no step landed yet.** The baseline net,
run before touching anything, found `docs/defects/002` — a legal program killed on a
file that is not text — and panel 087 repaired it in five lines of runtime C for
**zero spec tokens**. Waiting, measured 2026-08-23: ratification of 085, 086, 087 ·
23 open questions in `DECIDE.md` · two `SCHEDULED.md` items due here, panel 087's
unpruned extern asserts (13 lines × 153 TUs) and panel 086 R4's four `ORDER:` walks.

    clang -I runtime seed/heroes.c runtime/runtime.c -o heroes    # 3.4 s
    ./heroes run tests/harness/main.hero -- ./heroes              # 845 checks, green 2026-08-23

Before it: **M-bootstrap-archive closed 2026-08-19**, tag `m-bootstrap-archive` —
`crates/` is `archive/bootstrap-rs/`, design.md:82 is a fact, there is no third
language. Record: `docs/journal/024-bootstrap-archive.md`.

---

## The order

| order | id | what | warrant |
|---|---|---|---|
| 1 | **M-selfhost-port** | The port | v1 |
| 2 | **M-selfhost-fixpoint** | Fixpoint and the seed — **v1** | v1 |
| 3 | **M-harness-port** | The net in Heroes — the golden harness, the corpus, the record checks — **done** | closure list (§1.0): nothing can ask whether the two compilers agree once the bootstrap is gone |
| 4 | **M-bootstrap-archive** | `crates/` → `archive/bootstrap-rs/` — the third language dies here — **done** | v1's last clause (design.md:82) |
| 5 | **M-separate-compilation** | Separate compilation — one `.c` per module, prototypes across TUs, the cache | closure list (§1.0) — the build architecture |
| 6 | **M-package-manager** | Packages — `heroes add`/`heroes fetch`, and bindings in place of a standard library | **scheduled, no warrant** |
| 7 | **M-isolated-threads** | Concurrency (design.md Part 7.13) | **scheduled, no warrant** |
| 8 | **M-qbe-backend** | QBE backend (Part 7.14) — the proof that the IR is not C in disguise | **scheduled, no warrant** |
| 9 | **M-lsp-server** | `heroes lsp` | **scheduled, no warrant** |
| 10 | **M-vscode-extension** | The VS Code extension, complete — LSP client, debugging, packaging | **scheduled, no warrant** |
| 11 | **M-documentation-site** | The site — the whole language documented, anchored to programs that run | **scheduled, no warrant** |
| 12 | **M-journey-book** | The journey — how this language came to be | **scheduled, no warrant** |
| 13 | **M-guide-book** | The guide — the language, as a book you would find in a shop | §1.1: comprehension is the objective |
| 14 | **M-publication-gate** | Publication readiness — the last gate before anything goes outward | CLAUDE.md §14 |

Both books are **plain language, Italian and English** — the one declared
exception to CLAUDE.md §11, recorded there.

`scheduled, no warrant` is not decoration. Part 7's preamble defers everything on
its list until the closure list compiles itself, and **a place in this table is not
a warrant** — measurement 003 rider 3 is the standing precedent, where this file
scheduled `outline` and `explain` and CLAUDE.md §10's stopping rule refused them.

**Two milestones were asked for on 2026-08-12 and neither was added; the table is
unchanged and this paragraph is why** (panels 033 and 034). *Visibility*: three
tiers, and two of them were never visibility questions — private record fields
are an opaque type (§4.9 makes construction impossible from outside, and §4.20
makes a shim read the field anyway) and private variant cases are
`#[non_exhaustive]`, which Rust deleted in 2014, re-added per type in 2019 and
documents as costing exhaustiveness. Both are now **Part 6, permanently**. The
third, `private` on a declaration, is **Part 7 item 14** at a pre-fixed +18;
M-selfhost-probe was assigned to decide it and **did, at its close 2026-08-15:
no blockage, so it stays Part 7** — eleven modules ported, every cross-module
read intended, and the rule's own words ("a blockage there puts it on the
closure list, a wish does not") made the close mechanical (DESIGN-LOG). *Errors*: the Rust shape
landed at M-optional-map — `T?` is `Result<T,E>`, `?` is `?`, `.must()` is `.unwrap()` — and
the part Rust has that Heroes does not, the typed error, stays **Part 8 wart 5**
rather than becoming a deferral, because it loses on §4.12's positive rule as well
as on simplicity. What is real underneath the question is measured:
`docs/measurements/004-error-codes.md`, **25 mutants, 0 caught**, and the answer is
a `constant`, not a feature.

## What each one is

### M-selfhost-port — The port
Rust → Heroes into `selfhost/` (directory born here), file by file, **the goldens
and M-program-corpus's corpus as the net**, the `PORT-DEBT` count as the map. Every ordering-sensitive map walk
becomes an explicit `sort` (panel 006), or the fixpoint diff breaks.
**Carried in as a defect, not a feature: `-g`.** design.md §2 and §3.1 both state
that lldb breaks on and steps through `.hero` lines through the emitted `#line`
directives — and `-g` is passed to clang **only** under `--sanitize`
(`toolchain.rs:203`, the file's one occurrence), so an ordinary build carries no
DWARF and the claim has never been executed by anything. It is repaired here
because this is the milestone that needs it: debugging a Heroes compiler written
in Heroes is where the source mapping stops being a nicety. A golden runs lldb in
batch mode and asserts that a breakpoint on a `.hero` line is hit (CLAUDE.md §9:
every claim gets a test that makes it fire).

### M-selfhost-fixpoint — Fixpoint and the seed — **v1**
**Closed 2026-08-18.** A builds `B.c`, B builds `C.c`, `diff B.c C.c` empty
(generated C, not binaries; clang version pinned and recorded) — done, and the
seed with it: `seed/heroes.c`, one clang line, no flags, tested from `git archive
HEAD` because a test run in the working tree proves nothing about a *checkout*.

**The archive left this milestone** (panel 085 B4) and became the two rows above.
Three reasons, all measured: the port read its standard library from `crates/` at
run time, so the self-hosted compiler was already broken outside this repository
and blamed the author's line for it; `tests/differential.rs` — the instrument that
found that and two more — has the **bootstrap** as its expectation, so archiving it
removes the only thing that can ask whether the two compilers agree; and
`heroes measure` is not in the port, while design.md §1.6 cites
`crates/heroes/src/measure/gate.rs` as the spec budget's live enforcer.

### M-harness-port — The net, in Heroes
The 4,279 lines of Rust harness (`golden` 1,095 · `surface` 2,105 · `corpus` 544 ·
`milestones` 310 · `layout` 116 · `expectation` 109) become a Heroes program run by
the one command. **No compiler lines and no new surface**: a 79-line probe ran 65
of 65 `check/` cases green, and exit codes, separate streams, `getenv` and file
comparison were each measured reachable today. Predicted **2,500–3,760 lines**,
centre ~3,200, by three independent ratios.

Two things a straight port would miss. `layout.rs` is a **rewrite** — it walks
`crates/**/*.rs`, the compiler becomes `selfhost/*.hero`, and 29 of 143 selfhost
files already exceed §11's 300 lines, so the check must carry the knot rule or it
goes red on day one. And a directory walk has no sound cheap route: `ls > file`
plus `read_file` **splits a filename containing a newline into two entries at exit
0**, measured, which §1.12 forbids — so it is `popen`/`fgets`/`pclose`, measured
working on both compilers, with the wait status from `pclose`.

### M-bootstrap-archive — The third language dies
**Closed 2026-08-19.** `crates/` → `archive/bootstrap-rs/`, and the move was the
last commit rather than the first, because five things died with the bootstrap and
each needed a successor first — every one of them measured against what it
replaced: `tests/emission/` (142 programs, both compilers green on the same
bytes), `heroes measure` (3440 / 3512 / spread 72, identical), `suite_spec.hero`
(8 checks, replacing 435 lines in which every check was `cfg(test)`), `heroes
mutate` (538 mutants, byte-identical report), and CI's seed leg. The `#~`
invariant's runner needed nothing — M-harness-port had already carried it — and
neither did `doctor`, which has never asked about cargo. Panel 086 moved the
spec-budget ledger to `docs/measurements/010` and gave it the agreement check it
had never had; `records/citations` was built the same day and found nine dead
citations that predated the archive. The one number to carry forward: the
compiler's own 482 tests cost 13 s through the Rust and ~16 minutes through the
seed, which is the price of self-hosting and its own item in `DECIDE.md`.

### M-separate-compilation — Separate compilation
One `.c` per module, prototypes across translation units, the per-module cache:
the second half of the modules row, moved past the fixpoint because it costs
+700–1000 lines against the namespace's ~+330, because Nim has not finished its
own per-module cache since 2018 and Rust shipped 1.52.1 to disable incremental,
and because — measured — it is where §4.19's guarantee can quietly die. **Four
acceptance rows, and they are what lift the ffi-pragmatist's veto** (panel 030 R2):

1. the header travels with the extern into **every calling TU**, or externs are
   module-private and the *type checker* refuses the qualified call. **Panel 033
   found a third answer that costs nothing and is not a visibility rule**: *an
   `extern` declaration is never callable across a module boundary; a qualified
   call to one is a type error, and the route is a Heroes function in the
   declaring module.* Compiled, linked and run — and the shape this row exists to
   prevent reproduced exactly beside it (header only in the declaring TU: exit 1
   there, **exit 0 in the caller**, `rc=0 db=open`, with the wrong signature);
2. headers and link flags enter the cache key, with `runtime_text()` kept;
3. every dependency's emitted interface enters the key — `-flto` does not catch
   cross-TU signature skew and changes the answer;
4. one two-module FFI-shaped golden with a wrong `extern` signature: **exit 1 in
   both TUs**, `#~` annotated.

### M-package-manager — Packages, and what stands in for a standard library
**Scheduled, no warrant.** Not a decision to take later: design.md:637 already
fixes the shape — *"No package manager exists before modules do; when it arrives
it will be `heroes add`/`heroes fetch` — inside the same binary"* (never a second
binary, CLAUDE.md §6 and §10). Its real prerequisite is **M-separate-compilation**, not M-module-namespace: without
separate compilation, installing a package means recompiling the world on every
build.
**And this is where "a standard library that wraps C" goes.** §1.11 refuses a
standard library permanently, and that refusal is the founding constraint rather
than a shortage of effort — but what a standard library is *wanted* for arrives
here in a form the constraint permits: **distributable bindings**, ordinary
Heroes modules over real C headers, each with its link flag declared next to the
`extern` that needs it (§3.5). The difference is not cosmetic: a binding is
verified by clang against the header it names, and a standard library is verified
by whoever wrote it.

### M-isolated-threads — Concurrency
**Scheduled, no warrant.** design.md Part 7.13 — isolated per-thread heaps,
copying at the boundaries, OS threads, **no scheduler** — and its width (data
parallelism alone, or the mailbox too) is a panel question when it opens.
**Before M-selfhost-fixpoint the record must state whether the C11 backend can express the
intended model at all** (stack switching in the runtime, or a CPS/state-machine
transform): cfront, the direct ancestor of this architecture, was abandoned in
1993 after a failed attempt to add exception support, having frozen around forms
that could not carry non-local control flow. If the answer is "not yet known",
the deferral is a bet and is logged as one (panel 030 R6).

### M-qbe-backend — QBE: the proof that the IR is not C in disguise
**Scheduled, no warrant**, and its warrant is stated here more honestly than
"a second backend" ever did. Two things were bought when panel 001 replaced QBE
with C emission, and one of them was never paid for: **as long as exactly one
backend exists, "the IR is target-agnostic" is an assertion no artifact tests**,
and the IR could be a C pre-processor wearing an abstraction's name without
anything in this repo noticing. QBE from the same IR (~500 lines) is what turns
that sentence into a measurement — and it restores the register-allocation and
instruction-selection lesson, which is the half of a compiler this project
deliberately handed to clang (DESIGN-LOG 2026-08-03, panel 001).

### M-lsp-server — `heroes lsp`
**Scheduled, no warrant.** ~250 lines of JSON-RPC: diagnostics on save,
formatting, hover, documentSymbol. It blocks nothing and could land any time
after M-rich-diagnostics; it is here rather than earlier by the author's choice, and M-vscode-extension is what
consumes it.

### M-vscode-extension — The VS Code extension, complete
**Scheduled, no warrant.** `editors/vscode/` already ships the TextMate grammar,
the language configuration and the icon theme; this milestone makes it an
extension somebody could install and forget about.

- **The LSP client**, speaking to M-lsp-server's `heroes lsp`: diagnostics as you type,
  hover, go-to-definition, document symbols, formatting through `heroes fmt`.
- **Code actions from the fixes that already exist.** §4.17's `Fix`es are tagged
  `certain | guess` and `heroes check --apply` already applies the certain ones;
  the extension surfaces exactly those as quick fixes and never the guesses. This
  costs almost nothing and is the thesis made visible in the editor — the likeliest
  mistake arrives with its repair pre-written.
- **Debugging**, and the honest shape of it first: the emitted C carries `#line`
  back to `.hero` (with `-g` repaired at M-selfhost-port), so the debug info is ordinary DWARF
  pointing at Heroes source. `lldb-dap` therefore composes with the generated
  binary without this project writing a debug adapter — which is CLAUDE.md §10's
  *"nothing if two existing invocations already compose to it"*. If a launch
  configuration cannot be expressed that way, `heroes dap` enters under the
  stopping rule like any other verb, with the reason recorded.
  The known ceiling is design.md §2's: `p x` shows a mangled C temporary rather
  than a Heroes value. Typed inspection is not in v1 and this milestone does not
  smuggle it in.
- **Packaging**: a `.vsix` that installs, with `heroes doctor` as the extension's
  own health check.

### M-documentation-site — The site: the whole language, anchored to programs that run
**Scheduled, no warrant.** `site/` exists (`index.html`, its CNAME, and the
register rules in `site/README.md` § Style guide, which stay in force — song
titles as section nods, never lyrics; personality in the packaging, precision in
the substrate).

- **The language documented in full**, page by page, for someone who has not read
  `spec/heroes-spec.md` — the spec is the control instrument, not the teaching
  text, and it is budgeted precisely so that it can never become one.
- **Every code block on the site is a file in `examples/`**, not a snippet typed
  into HTML. M-program-corpus is what makes this possible, and it converts documentation drift
  into a test failure: a check asserts that each block matches a program in the
  repo that compiles and runs. Documentation that cannot rot is worth more than
  documentation that is merely current.
- **A history of the language**, distilled from `DESIGN-LOG.md`, `docs/panel/`
  and the journals: what was decided, what was refused, and the U-turns —
  including the ones that look bad in retrospect, which are the ones worth reading.
- **Publishing stays a hard stop** (CLAUDE.md §14): the site is built here and
  goes outward only when the author says so.

### Two books, and one rule that governs both

**They are written in simple, simple language** — the register of the `/where`
skill, which explains this project assuming zero compiler knowledge. The author's
instruction is the reason and it outranks elegance, brevity and completeness: he
will read these to *study* what was built, so a sentence that needs a compiler
course to parse is a sentence to rewrite.

**Both exist in Italian and English, and this is the one declared exception** to
CLAUDE.md §11's "everything written is English" (§11 now records it). Neither
version is a machine translation of the other; the Italian is the one the author
studies from, so where the two diverge, the Italian is fixed to be clearer rather
than the English to be more faithful.

**Both teach with M-program-corpus's programs** — code known to compile, run and pass its own
tests in three configurations, rather than snippets that were true once.

### M-journey-book — The journey: how this language came to be
**Scheduled, no warrant.** The narrative book: the itch, the design that met five
hostile experts, the U-turns, the deleted darlings, the days the machine found the
bug in the plan before we did — and what it was like to build a compiler with an
AI assistant. `docs/book/README.md` has been collecting the raw material since M-day-zero
and nothing extra needs maintaining: the journals are the spine, `DESIGN-LOG.md`
the decisions, `docs/panel/` the arguments, `docs/book/beats.md` the human
texture the technical records drop, the measurements the numbers, and
`git checkout m2` re-opens any chapter's code.
The rule from design.md's *The name* applies here and only here: **personality in
the packaging, precision in the substrate** — Bowie belongs in this book, never in
an error message.

### M-guide-book — The guide: the language, as a book you would find in a shop
**Scheduled, no warrant.** The classic language guide — the K&R shape: read it
front to back and you can write Heroes; open it in the middle and you find the
thing you were looking for.

- **Organised by subject, not by chronology**: values and types, bindings and
  `@`, control flow, records and variants, `T?` and failure, generics, the
  module, the FFI, the test blocks — each with the smallest program that shows it
  and one that gets it wrong on purpose.
- **It is not the spec, and it must never try to be.** `spec/heroes-spec.md` is
  the control instrument, budgeted at 4096 tokens precisely so that it can never
  become a teaching text; the guide is where the explanations, the worked
  examples and the "why it is like this" live, at whatever length clarity needs.
- **Its warrant is §1.1**, the only one after the fixpoint that has one:
  comprehension is this project's objective, measured rather than asserted, and a
  guide the author can read to rebuild the reasoning is that objective's final
  artifact.
- Where the guide and the spec disagree, **the spec wins and the guide has the
  bug** (CLAUDE.md §12) — and where the guide and the *compiler* disagree, that
  is a defect report on one of them, which is what the M-documentation-site check over `examples/`
  is for.

### M-publication-gate — Publication readiness: the last gate
The repo is **private** today and publishing is a hard stop that only the author
lifts (CLAUDE.md §14). This entry is the checklist that has to be true first, and
it exists because most of its items get worse the longer they wait.

**Already done, ahead of the milestone** (2026-08-11, because a repository
accumulates history and history cannot be relicensed retroactively): `LICENSE`
(Apache-2.0), `LICENSE-RUNTIME-EXCEPTION` — so a program compiled with Heroes
owes nothing for the runtime inside it — `NOTICE`, `README.md`, SPDX headers
across `runtime/`, and the attribution of the two vendored BPE tables.

**Still owed here:**
- **The thesis, measured.** Metric 2 has never run; §1.2's formula has two
  factors and only one is audited. The site and both books will state the claim,
  and stating it unmeasured publishes an opinion with a decimal point — the one
  thing §12 forbids, the author included. Not delegable: the held-out tasks must
  be author-written, or they measure the assistant's priors (panel 011).
- **A compatibility policy.** What v1 promises to somebody who writes code
  against it, in one honest paragraph. Silence reads as a promise.
- **The licence re-check on vendored material**, against the upstream
  repositories rather than against this project's recollection
  (`vendor/tokenizers/README.md` § Licensing).
- **The trademark question**, in the narrow form that actually applies: the name
  is a common word and does not worry anybody, but `site/`'s Aladdin Sane bolt is
  iconography attached to an actively managed estate. The style guide already
  keeps lyrics out; this is the other half, and it is cheaper to answer before
  publication than after.
- **Contribution policy in force** — the README's current answer ("issues yes,
  pull requests not yet") either stands or is replaced deliberately.
- **One defect that shows up on the second page of any tour**: `main` cannot
  fail, so a program that goes wrong still tells the shell it succeeded
  (queued from panel 030). Whatever M-ffi-ladder decides for `exit(code)`, this must not be
  true on the day the examples go up.

## Done

One line each; the record is the journal, and `git checkout <tag>` re-opens the
code. `git tag --list --sort=creatordate` gives the same order from git itself.

| milestone | closed | tag | journal |
|---|---|---|---|
| M-day-zero — day zero | 2026-08-03 | `m0` | [000](journal/000-setup.md) |
| M-token-stream — the lexer | 2026-08-04 | `m1` | [001](journal/001-lexer.md) |
| M-syntax-tree — parser, tree, formatter | 2026-08-04 | `m2` | [002](journal/002-parser.md) |
| M-name-resolution — the resolver | 2026-08-04 | `m3a` | [003](journal/003-resolver.md) |
| M-typed-frontend, M-checker-core–M-rich-diagnostics — the frontend complete | 2026-08-04 | `m3` | [004](journal/004-checker.md) |
| M-ir-lowering — desugar and the IR | 2026-08-04 | `m4` | [005](journal/005-lowering.md) |
| M-native-backend, M-scalars-run — the first native binary | 2026-08-04 | `m5a` | [006](journal/006-scalars-run.md) |
| M-strings-ownership — `str`, and the ownership pass | 2026-08-05 | `m5b` | [007](journal/007-strings-and-ownership.md) |
| M-value-aggregates — records and variants by value | 2026-08-10 | `m5c` | [008](journal/008-aggregates.md) |
| M-optional-map — `T?` and `{K: V}` | 2026-08-10 | `m5d` | [009](journal/009-the-map-and-the-fallible.md) |
| M-generics-library — the language is finished | 2026-08-11 | `m6` | [010](journal/010-sugar-tests-generics-library.md) |
| M-module-namespace — a program is many files | 2026-08-12 | `m8a` | [011](journal/011-modules-the-namespace.md) |
| M-ffi-ladder — Heroes calls C | 2026-08-12 | `m-ffi-ladder` | [012](journal/012-the-ffi-ladder.md) |
| M-header-constants — the number leaves the file | 2026-08-12 | `m-header-constants` | [013](journal/013-header-constants.md) |
| M-literal-bases — `0x` `0o` `0b`, the `_` separator | 2026-08-12 | `m-literal-bases` | [014](journal/014-literal-bases.md) |
| M-sized-integers — eight widths, and `int` stops being a word | 2026-08-12 | `m-sized-integers` | [015](journal/015-sized-integers.md) |
| M-program-corpus — nine programs, six compiler defects, and a CI | 2026-08-13 | `m-program-corpus` | [016](journal/016-the-program-corpus.md) |
| M-binding-fidelity — what an `extern` accepts | 2026-08-14 | `m-binding-fidelity` | [017](journal/017-binding-fidelity.md) |
| M-struct-passing — the layout that is not ours | 2026-08-15 | `m-struct-passing` | [018](journal/018-struct-passing.md) |
| M-complete-structs — every field form a C header can write | 2026-08-15 | `m-complete-structs` | [019](journal/019-complete-structs.md) |
| M-selfhost-probe — the lexer ported, and the wall that was not there | 2026-08-15 | `m-selfhost-probe` | [020](journal/020-selfhost-probe.md) |
| M-selfhost-port — the compiler twice over, and one hash | 2026-08-17 | `m-selfhost-port` | [021](journal/021-selfhost-port.md) |
| M-selfhost-fixpoint — the seed, and four defects the fixpoint could not see | 2026-08-18 | `m-selfhost-fixpoint` | [022](journal/022-selfhost-fixpoint.md) |

## The names

Milestones were numbered until 2026-08-12 and are named now. The algorithm that
assigns the next one is **CLAUDE.md §14** — its only home; this section is only the
map, and it exists because **the record was not rewritten**. `docs/panel/`,
`DESIGN-LOG.md`, `docs/journal/`, `docs/measurements/`, `docs/defects/`,
`docs/book/beats.md`, `tests/golden/`, every commit subject and all twelve tags keep
the identifiers they were written with. Panel 030 R7, as amended, is the argument.

So a number met in the record resolves here, and only here.

| name | was | tag | what it delivered |
|---|---|---|---|
| `M-day-zero` | M0 | `m0` | git, the workspace, Cyclone tooling, four hand-written C targets |
| `M-token-stream` | M1 | `m1` | the lexer, complete, and `--dump-tokens` |
| `M-syntax-tree` | M2 | `m2` | parser, AST, the canonical formatter |
| `M-typed-frontend` | M3 | `m3` | the frontend complete: names, types, errors as a product |
| `M-name-resolution` | M3a | `m3a` | scopes, no shadowing, order-free top level |
| `M-checker-core` | M3b | — | bidirectional ⇐/⇒, one shared join |
| `M-data-declarations` | M3c | — | records, variants with payload |
| `M-rich-diagnostics` | M3d | — | §4.17's rich form |
| `M-ir-lowering` | M4 | `m4` | desugaring, and the three-address IR with basic blocks |
| `M-native-backend` | M5 | *(never tagged)* | the parent of the four backend milestones |
| `M-scalars-run` | M5a | `m5a` | the first native binary |
| `M-strings-ownership` | M5b | `m5b` | `str` in C, and the ownership pass |
| `M-value-aggregates` | M5c | `m5c` | records and variants by value, with copy-on-write |
| `M-optional-map` | M5d | `m5d` | `T?` and `{K: V}` — the last two types |
| `M-generics-library` | M6 | `m6` | sugar, `heroes test`, generics by monomorphisation, the library |
| `M-module-namespace` | M8a | `m8a` | `use`, qualified names, one whole-program `.c` |
| `M-ffi-ladder` | M7 | `m-ffi-ladder` | the FFI ladder, SQLite with no shim, and the last three closure-list rows |
| `M-header-constants` | — | `m-header-constants` | a `constant` whose value is the header's, so the number leaves the file |
| `M-literal-bases` | — | `m-literal-bases` | `0x` `0o` `0b`, the `_` separator, and a leading zero that is no longer decimal |
| `M-sized-integers` | — | `m-sized-integers` | the integer widths, signed and unsigned, and the conversions between them |
| `M-program-corpus` | M8e | `m-program-corpus` | many whole programs, all of them run |
| `M-binding-fidelity` | — | `m-binding-fidelity` | a binding says what the header says: the parameter side of §4.19's guarantee |
| `M-struct-passing` | — | `m-struct-passing` | a struct crosses the FFI boundary by value — §4.19's ladder rung 5, and the third of the boundary that was unreachable |
| `M-complete-structs` | — | `m-complete-structs` | every field form a C header can write — `partial`, `i32[4]`, and the completeness probe. **A new id rather than a reopening** (§14): the milestone before it says *a struct can cross*, this one says *every struct can be named*, and the work landed after `m-struct-passing` was tagged |
| `M-selfhost-probe` | M8p | `m-selfhost-probe` | the lexer ported, to measure what self-hosting lacks |
| `M-selfhost-port` | M8b | `m-selfhost-port` | the port, and one hash for three compilers |
| `M-selfhost-fixpoint` | M8c | `m-selfhost-fixpoint` | the fixpoint and the seed — **v1** |
| `M-harness-port` | — | — | the net in Heroes: the golden harness, the corpus, the record checks. **A new id and not part of M8c** (§14): panel 085 B4 split the archive off the fixpoint, and the archive's precondition is a milestone of its own — 4,279 lines of Rust harness, and the instrument that dies with the bootstrap is the one whose expectation *is* the bootstrap |
| `M-bootstrap-archive` | M8c, in part | — | `crates/` → `archive/bootstrap-rs/`. **The clause M8c was carrying and could not pay**: the port read its standard library from the directory being archived, so the compiler failed outside this repository, and `heroes measure` is still not in the port |
| `M-separate-compilation` | M9 | — | one `.c` per module, prototypes across TUs, the cache |
| `M-package-manager` | M10 | — | `heroes add`/`heroes fetch`, bindings in place of a standard library |
| `M-isolated-threads` | M11 | — | Part 7.13: per-thread heaps, copying at the boundaries, no scheduler |
| `M-qbe-backend` | M12 | — | Part 7.14: the proof that the IR is not C in disguise |
| `M-lsp-server` | M13 | — | `heroes lsp` |
| `M-vscode-extension` | M14 | — | the extension, complete |
| `M-documentation-site` | M15 | — | the site |
| `M-journey-book` | M16 | — | the journey |
| `M-guide-book` | M17 | — | the guide |
| `M-publication-gate` | M18 | — | the last gate before anything goes outward |

**`M8` has no row, because it meant three different things.** It was an umbrella that
predates the a/b/c/e/p split and no heading has carried it since. In the record it
reads as the **fixpoint** (panels 006, 027 ×2, 028, 029), as the **port**
(panel 022 — "instruction lists at M8"), and as the **whole span**
(panel 005 — "all of M0–M8"). Read the sentence, not the number. Its two living
sites were resolved by hand to `M-selfhost-fixpoint`.

**`M10`–`M13` resolve two ways, and the date decides which.** Packages were inserted
ahead of concurrency after the chain was first written down, so everything from
`DESIGN-LOG.md`'s *"the post-fixpoint order, by author instruction"* row onward uses
the table above. Two earlier sites use the superseded numbering, where **M10 was
concurrency, M11 QBE and M12 `lsp`**: `DESIGN-LOG.md`'s build-order row and
panel 030's own body. This ambiguity predates the rename and is exactly the failure
mode R7's precedent warns about — a number that still resolves, to the wrong thing.
A name cannot do it, which is the whole reason for this section.

**Two names are not the deliverable's obvious one, and the reason is on the record.**
`M-generics-library` covers four concerns (sugar, tests, generics, library); the
runner-up `M-language` was refused because four of §1.0's rows were still open, so it
overclaimed. `M-strings-ownership` names both halves rather than just `str`, because
the ownership pass is the architecturally load-bearing one and the milestone's journal
slug names both too.

## End-to-end verification (per milestone)

```sh
heroes doctor                                  # M-day-zero
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes         # M-bootstrap-archive: the way in
heroes test selfhost/main.hero                 # the compiler's own tests
heroes run tests/harness/main.hero -- ./heroes # the net (`cargo build && cargo test` until 2026-08-19)
heroes lex ex.hero --dump-tokens [--json]      # M-token-stream
heroes parse ex.hero --dump-ast                # M-syntax-tree
heroes fmt ex.hero [--in-place]                # M-syntax-tree (the flag was --write until panel 016)
heroes check ex.hero --json [--permissive]     # M-typed-frontend
heroes build ex.hero                           # M-ir-lowering: lowers, verifies, says so
heroes build ex.hero --dump-ir                 # M-ir-lowering (M-strings-ownership: increfs visible)
heroes build ex.hero --emit-c                  # M-scalars-run — and the determinism diff:
heroes build ex.hero --emit-c -o a.c && heroes build ex.hero --emit-c -o b.c && diff a.c b.c
heroes run examples/gallery/00-first.hero                 # M-scalars-run: first native binary (-O2)
heroes test examples/calculator/whole.hero     # M-generics-library: acceptance ✅
heroes test examples/calculator/main.hero      # M-module-namespace: the same tests, across modules
heroes run examples/sqlite/main.hero           # M-ffi-ladder: acceptance — open, query, close
heroes test examples/maze/main.hero            # M-program-corpus: one program (the harness runs them all,
                                               #      in three configurations — a directory
                                               #      argument is §10's question, not a given)
heroes mutate                                  # M-program-corpus: the rate over the enlarged corpus
heroes test selfhost/lexer.hero                # M-selfhost-probe: the ported lexer's own tests
# M-selfhost-fixpoint — the fixpoint, on generated C (the first line was
# `cargo run --` until the archive; `seed/README.md` is the live ritual):
heroes build selfhost/main.hero -o A
./A build selfhost/heroes.hero --emit-c -o B.c && clang … B.c -o B
./B build selfhost/heroes.hero --emit-c -o C.c && diff B.c C.c
```
