# ROADMAP — where the project is, and what is next

Autonomous work sessions need the goal chain **in the repository**. This file is
the distillation of the approved bootstrap plan (revision 2, reviewed by panel
000); the build order's full rationale is design.md Part 10, and the
language-level acceptance criteria are design.md Part 0. Revision 2 had deleted
this file — *"tags say where you are"* — and it came back because tags say where
you **are**, not what is **next**, and what is next must not live outside version
control.

**A closed milestone's record lives in its own journal**, indexed at
`docs/journal/README.md`. That split exists because this file had accumulated 512
lines about the past before its first line about the future, and was growing
about 66 lines per close. `/step`'s checklist keeps § Where we are short and
sends the closing block to the journal.

**Reorganised 2026-08-25 by author instruction** — *"riscrivilo completamente
senza perdere nessuna informazione, in modo più ordinato"*. Nothing was dropped.
What changed is that the same milestones used to be listed in **three** separate
tables — the order, the done list, and the name map — and are now in one, with
the name map kept separately because CLAUDE.md §14 cites it and a cited record is
not merged away.

**Reorganised again 2026-08-26 by author instruction** — *"metti in alto la
tabella di sintesi e poi sotto tutti gli step in ordine"*. That pass merged the
tables and left the **prose** in four sections that did not share an order: a
milestone's row was in § The chain, the open one's story sat between the summary
table and that chain — **154 lines of the past before the first line of the
future**, the exact shape the journal split exists to prevent — the scheduled
ones were under *what is next*, and four closed ones under *what the closed
milestones settled*. Now every milestone that still has something to say has
**one** section, and the sections run in the chain's order. The two verification
blocks, which sat 700 lines apart, are one section under the summary table: two
copies of the same duty at opposite ends of a file is how one of them rots.
Nothing was dropped, and this time that is a **measurement** — the word frequency
table of the file before and after differs only in the headings that changed and
in the numbers that were re-measured.

**Reordered a third time 2026-09-03 by author instruction** — *"riordina roadmap
con tutto quello che è stato fatto prima di quello che deve essere fatto, in più
tutte le note spostale dalla tabella … la tabella la voglio pulita"*. Two
changes, and neither drops a word. **The chain now runs closed-then-scheduled**:
the one closed milestone that sat below five open ones — M-documentation-site,
taken out of order the day it landed — moves up to the end of the closed run, so
a reader meets the whole past before the first line of the future. That is the
same rule the journal split already applies to this file's prose, applied to its
table. And **the table carries the order and nothing else**: every scheduling
note, panel ratification and author decision that lived inside a cell is now a
line under it, keyed by name rather than by row number, because a reorder moves a
number and never a name (CLAUDE.md §14). Two sections whose milestone had closed
while they went on describing future work — M-package-layout and
M-documentation-site — now say so in their headings, the way
M-separate-compilation already did.

---

## Where we are

| | |
|---|---|
| **Current milestone** | **Two open, in two sessions** (author instruction 2026-09-03). **M-corpus-depth** — **OPEN 2026-09-03**, row 33: `heroes mutate` reads its corpus again (it refused `examples/` for a day), the gate on every CI leg, then nine programs for the rung between a program and the compiler. **M-isolated-threads** — **OPEN 2026-09-03**, row 34: Part 7.13 concurrency, per-thread heaps, copying at the boundaries, no scheduler — and the one hole panel 104 left, a C library's own thread overflowing its stack |
| **Last closed** | **M-robustness-guards**, 2026-09-03, tag `m-robustness-guards` ([031](journal/031-robustness-guards.md)) · v1 **reached** at M-selfhost-fixpoint, 2026-08-18 |
| Milestones closed | 32 of 44 · 32 tags |
| The compiler | **51,788 lines** of Heroes in **178** modules across **10 directories** and 37 flat files · the seed **840,806** lines of generated C |
| The spec | **3718** tokens of a hard 4096 · headroom **378** · runtime ABI **19** |
| Records | sittings **103** · journals 32 · measurements **14** · examples **35** · defects 5 · the site **46** pages (built 2026-09-03), 13 doc chapters per edition |
| Waiting on the author | **1 decision** (a discarded fallible value swallows its error: `_ = f()?` → `_ = f()` compiles, nine times in the corpus — measurement 014; `heroes mutate` in CI was asked and answered the same evening) · **18** in `SCHEDULED.md` · 297 in `LEARN.md` (never a gate) · an **outstanding veto** of the rule that stands (`docs/panel/101` R3) · the push of this close, which publishes the site's one-hole paragraph |

Every number re-measured 2026-09-03 at the close. **This section held three
stale tables until that day** — 64 lines against a ceiling of 15, caught here.

---

## Verify it yourself

### From a cold checkout, right now

```sh
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes   # the compiler, from C alone (~3.5 s)
./heroes run tests/harness/main.hero -- ./heroes             # the net
./heroes test tests/harness/main.hero                        # the net's own tests (13.5 s)
./heroes test selfhost/main.hero                             # the compiler's own tests
./heroes doctor                                              # the toolchain
```

**Rebuild `./heroes` before trusting it.** On 2026-08-25 the binary in the
repository root was nine hours stale and did not know a built-in that had landed
the evening before; a panel seat nearly filed that as a language defect. The
first line above is 3.5 seconds and removes the whole class.

### End to end, per milestone

In the chain's order. Every one of these is a command somebody can run today.

```sh
heroes doctor                                  # M-day-zero
heroes lex ex.hero --dump-tokens [--json]      # M-token-stream
heroes parse ex.hero --dump-ast                # M-syntax-tree
heroes fmt ex.hero [--in-place]                # M-syntax-tree (the flag was --write until panel 016)
heroes check ex.hero --json [--permissive]     # M-typed-frontend
heroes build ex.hero                           # M-ir-lowering: lowers, verifies, says so
heroes build ex.hero --dump-ir                 # M-ir-lowering (M-strings-ownership: increfs visible)
heroes build ex.hero --emit-c                  # M-scalars-run — and the determinism diff:
heroes build ex.hero --emit-c -o a.c && heroes build ex.hero --emit-c -o b.c && diff a.c b.c
heroes run examples/gallery/00-first.hero      # M-scalars-run: first native binary (-O2)
heroes test examples/calculator/whole.hero     # M-generics-library: acceptance
heroes test examples/calculator/main.hero      # M-module-namespace: the same tests, across modules
heroes run examples/sqlite/main.hero           # M-ffi-ladder: acceptance — open, query, close
heroes test examples/maze/main.hero            # M-program-corpus: one program (the harness runs them
                                               #   all, in three configurations — a directory
                                               #   argument is §10's question, not a given)
heroes mutate                                  # M-program-corpus: the rate over the enlarged corpus
heroes test selfhost/lexer.hero                # M-selfhost-probe: the ported lexer's own tests
# M-selfhost-fixpoint — the fixpoint, on generated C (the first line was
# `cargo run --` until the archive; `seed/README.md` is the live ritual):
heroes build selfhost/main.hero -o A
./A build selfhost/main.hero --emit-c -o B.c && clang … B.c -o B
./B build selfhost/main.hero --emit-c -o C.c && diff B.c C.c
heroes run tests/harness/main.hero -- ./heroes # the net (`cargo build && cargo test` until 2026-08-19)
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes    # M-bootstrap-archive: the way in
heroes test selfhost/main.hero                 # the compiler's own tests
```

---

## The chain

One table, one row per milestone, **closed first and scheduled after**: rows 1–32
are done, in the order they closed, and rows 33–44 are what is next, in the order
they will be taken. `warrant` is why a milestone exists: **v1** (the self-hosting
finish line), **closure list** (design.md §1.0 — the compiler needs it), **§1.1**
(comprehension is the objective), or **scheduled, no warrant**.

**The cells hold no prose.** Who scheduled a milestone, which panel ratified it,
and why one overtook another are under the table, in § Who scheduled what.

| # | milestone | state | tag | journal | what it delivers · warrant |
|---|---|---|---|---|---|
| 1 | **M-day-zero** | done 2026-08-03 | `m0` | [000](journal/000-setup.md) | git, workspace, four hand-written C targets |
| 2 | **M-token-stream** | done 2026-08-04 | `m1` | [001](journal/001-lexer.md) | the lexer, and `--dump-tokens` |
| 3 | **M-syntax-tree** | done 2026-08-04 | `m2` | [002](journal/002-parser.md) | parser, AST, the canonical formatter |
| 4 | **M-name-resolution** | done 2026-08-04 | `m3a` | [003](journal/003-resolver.md) | scopes, no shadowing, order-free top level |
| 5 | **M-typed-frontend** | done 2026-08-04 | `m3` | [004](journal/004-checker.md) | the frontend complete: names, types, errors as a product |
| 6 | **M-ir-lowering** | done 2026-08-04 | `m4` | [005](journal/005-lowering.md) | desugaring, the three-address IR with basic blocks |
| 7 | **M-scalars-run** | done 2026-08-04 | `m5a` | [006](journal/006-scalars-run.md) | the first native binary |
| 8 | **M-strings-ownership** | done 2026-08-05 | `m5b` | [007](journal/007-strings-and-ownership.md) | `str` in C, and the ownership pass |
| 9 | **M-value-aggregates** | done 2026-08-10 | `m5c` | [008](journal/008-aggregates.md) | records and variants by value, copy-on-write |
| 10 | **M-optional-map** | done 2026-08-10 | `m5d` | [009](journal/009-the-map-and-the-fallible.md) | `T?` and `{K: V}` — the last two types |
| 11 | **M-generics-library** | done 2026-08-11 | `m6` | [010](journal/010-sugar-tests-generics-library.md) | sugar, `heroes test`, generics, the library |
| 12 | **M-module-namespace** | done 2026-08-12 | `m8a` | [011](journal/011-modules-the-namespace.md) | a program is many files |
| 13 | **M-ffi-ladder** | done 2026-08-12 | `m-ffi-ladder` | [012](journal/012-the-ffi-ladder.md) | Heroes calls C; SQLite with no shim |
| 14 | **M-header-constants** | done 2026-08-12 | `m-header-constants` | [013](journal/013-header-constants.md) | the number leaves the file |
| 15 | **M-literal-bases** | done 2026-08-12 | `m-literal-bases` | [014](journal/014-literal-bases.md) | `0x` `0o` `0b`, the `_` separator |
| 16 | **M-sized-integers** | done 2026-08-12 | `m-sized-integers` | [015](journal/015-sized-integers.md) | eight widths; `int` stops being a word |
| 17 | **M-program-corpus** | done 2026-08-13 | `m-program-corpus` | [016](journal/016-the-program-corpus.md) | nine programs, six compiler defects, a CI |
| 18 | **M-binding-fidelity** | done 2026-08-14 | `m-binding-fidelity` | [017](journal/017-binding-fidelity.md) | what an `extern` accepts |
| 19 | **M-struct-passing** | done 2026-08-15 | `m-struct-passing` | [018](journal/018-struct-passing.md) | a struct crosses by value |
| 20 | **M-complete-structs** | done 2026-08-15 | `m-complete-structs` | [019](journal/019-complete-structs.md) | every field form a C header can write |
| 21 | **M-selfhost-probe** | done 2026-08-15 | `m-selfhost-probe` | [020](journal/020-selfhost-probe.md) | the lexer ported, to measure what was missing |
| 22 | **M-selfhost-port** | done 2026-08-17 | `m-selfhost-port` | [021](journal/021-selfhost-port.md) | the compiler twice over, and one hash · **v1** |
| 23 | **M-selfhost-fixpoint** | done 2026-08-18 | `m-selfhost-fixpoint` | [022](journal/022-selfhost-fixpoint.md) | the fixpoint and the seed · **v1** |
| 24 | **M-harness-port** | done 2026-08-18 | `m-harness-port` | [023](journal/023-harness-port.md) | the net in Heroes · closure list |
| 25 | **M-bootstrap-archive** | done 2026-08-19 | `m-bootstrap-archive` | [024](journal/024-bootstrap-archive.md) | the third language dies · v1's last clause (design.md:82) |
| 26 | **M-separate-compilation** | done 2026-08-26 | `m-separate-compilation` | [025](journal/025-separate-compilation.md) | one `.c` per module, prototypes across TUs, the cache · closure list |
| 27 | **M-argv-execution** | done 2026-08-31 | `m-argv-execution` | [026](journal/026-argv-execution.md) | the compiler runs programs by argument list, and the shell stops being the boundary |
| 28 | **M-package-layout** | done 2026-09-02 | `m-package-layout` | [027](journal/027-package-layout.md) | `use` paths, the qualifier, `as`, and where a program's files live |
| 29 | **M-selfhost-nesting** | done 2026-09-02 | `m-selfhost-nesting` | [028](journal/028-selfhost-nesting.md) | the compiler's own modules move into ten directories |
| 30 | **M-corpus-coverage** | done 2026-09-02 | `m-corpus-coverage` | [029](journal/029-corpus-coverage.md) | every language form has a program that runs it |
| 31 | **M-documentation-site** | done 2026-09-02 | `m-documentation-site` | [030](journal/030-documentation-site.md) | the site, anchored to programs that run |
| 32 | **M-robustness-guards** | done 2026-09-03 | `m-robustness-guards` | [031](journal/031-robustness-guards.md) | the guards that shut the holes §1.12 named: `@` on an immutable, the stack, the C pointer verdict, the harness scratch · §1.12 |
| 33 | **M-corpus-depth** | **OPEN** 2026-09-03 | — | — | the rung between a program and the compiler: `heroes mutate` reads its corpus again, then nine programs — oracle-checked, deep, FFI at program scale · **§1.1** |
| 34 | **M-isolated-threads** | **OPEN** 2026-09-03 | — | — | Part 7.13 concurrency: per-thread heaps, copying at the boundaries, no scheduler |
| 35 | **M-package-manager** | scheduled | — | — | `heroes add`/`heroes fetch`; bindings instead of a standard library |
| 36 | **M-core-packages** | scheduled | — | — | small packages that compose, organised as Go's tree, in Heroes or over C |
| 37 | **M-web-framework** | scheduled | — | — | composes the core packages, Go/Echo style: explicit routes, records, no magic |
| 38 | **M-qbe-backend** | scheduled | — | — | Part 7.14 — the proof that the IR is not C in disguise |
| 39 | **M-lsp-server** | scheduled | — | — | `heroes lsp` |
| 40 | **M-vscode-extension** | scheduled | — | — | the extension, complete |
| 41 | **M-interpolation-verdict** | scheduled | — | — | the ruling on design.md Part 7 item 7, string interpolation — a decision, not a feature |
| 42 | **M-journey-book** | scheduled | — | — | the journey — how this language came to be |
| 43 | **M-guide-book** | scheduled | — | — | the guide, as a book you would find in a shop · **§1.1** |
| 44 | **M-publication-gate** | scheduled | — | — | the last gate before anything goes outward · CLAUDE.md §14 |

Three closed milestones have no tag of their own because they were parents or
sub-steps: **M-checker-core**, **M-data-declarations** and **M-rich-diagnostics**
landed inside `m3`, and **M-native-backend** was the parent of the four backend
milestones and was never tagged. § The names carries them.

`git tag --list --sort=creatordate` gives the same chronology from git itself.

### Who scheduled what, and what ratified it

Every sentence here stood **inside a cell** until 2026-09-03, where it hid the
order the table exists to carry: the longest cell measured **383 characters**
against the 91 the table's widest now runs to. They are keyed by **name**, not by
row number, because a reorder moves a number and never a name (CLAUDE.md §14).

- **M-argv-execution** — **panels 097 and 098, ratified 2026-08-30.** `sq()` is
  deleted with it, and the Windows leg of CI goes green.
- **M-package-layout** — **panels 099, 100 and 101, all three ratified the same
  day**, 2026-09-02.
- **M-selfhost-nesting** — **panel 102, ratified the same day.** What the
  milestone found was not in its own plan: the flat prefixes had been holding
  module names out of the namespace where values live, so the collisions that
  mattered were local-against-module — 0 flat and 23 nested.
- **M-corpus-coverage** — **scheduled by author instruction 2026-09-02**, and
  ordered after the nesting so that the new programs are written against the
  final layout.
- **M-documentation-site** — **taken out of chain order by author instruction
  2026-09-02**, in the same breath as the nesting and the corpus, because the
  feature it documents had just landed and the site was silent on it. It was
  planned at position 36, after M-vscode-extension. It stands at 31 because the
  table runs closed-then-scheduled since 2026-09-03; the five milestones it
  overtook keep their turn and their order among themselves. **What the table no
  longer records is the plan** — that a milestone moved is here, in a sentence,
  rather than in a row's position, which is what the author's *"la tabella la
  voglio pulita"* decided.
- **M-isolated-threads** — **moved ahead of packages by author instruction,
  2026-08-25.**
- **M-interpolation-verdict** — **scheduled by author instruction 2026-09-02.**
- **M-robustness-guards** — **opened by author instruction 2026-09-03**, ahead of
  M-isolated-threads, out of a `/decide` sitting that closed ten items at once:
  *"ratifica tutto … scegli le soluzioni più robuste e complete rispetto a quelle
  più economiche … privilegia il consolidamento e le soluzioni migliori non le
  scorciatoie … più storia e meno attenzione al token … più robustezza su tutte le
  piattaforme, non silenziare errori"*. Its six steps are the six `SCHEDULED.md`
  items that name it; two of them are sittings (the FFI pointer verdict, full five
  seats; the stack guard, soundness lane). The `records/names` check found the id in
  a list before it had a row here, which is the order CLAUDE.md §14 wants.
- **M-corpus-depth** — **scheduled by author instruction 2026-09-03**, *"ragiona
  sulla possibilità di aggiungere ulteriori esempi anche alcuni più complessi per
  avere una rete più ampia, guarda anche cosa hanno fatto altri linguaggi"*, from a
  plan measured and approved the same day. The author placed it after
  M-robustness-guards, which closed that afternoon, so it first stood at 34 behind
  the open M-isolated-threads. Its step 0 — the first `heroes mutate` score over
  the 35-program corpus since 2026-08-13, and the corpus leg timed alone — landed
  with the scheduling, so the "before" exists before the first program does.
  **Then step 0 found `heroes mutate` unable to read `examples/` at all**, refused
  since 2026-09-02 (measurement 014), and the author moved the milestone to 33
  and opened it the same evening — *"anticipare quei due passi subito, prima di
  M-isolated-threads"*, with *"cancello su ogni ramo + punteggio pieno sui tag"*
  for the CI question — so that the metric the thesis rests on is repaired
  before anything else is measured against it. **Two milestones are open at
  once**, in two sessions, and the table says so rather than hiding one.
- **M-core-packages** and **M-web-framework** — **scheduled by author instruction
  2026-09-03**, the same evening, out of a reasoning session
  (`docs/reasoning/005-packages-in-place-of-a-standard-library.md`): *"vorrei
  avere tutti gli strumenti necessari a costruire un web framework alla Rails o
  Django o anche più sottile, tipo Go, Echo o FastAPI"*, and then, when a single
  toolkit was proposed, *"mi immagino più pacchetti che si combinano e poi quello
  web che li usa tutti; il modello è Go come organizzazione"*. Placed after
  M-package-manager because `heroes fetch` is what makes a package a thing you
  distribute. **Two rows because they are two deliverables** (CLAUDE.md §14): the
  packages, and the framework that composes them. The level is Go's and Echo's —
  everything explicit — because the Rails and Django shape rests on Part 6's own
  rows, metaprogramming, dynamic dispatch and inheritance (`design.md:2382-2404`).
  **The sitting sits at the opening, not at the scheduling**, on
  M-interpolation-verdict's precedent, and the author added its sixth question the
  same night: conditional compilation — *"la compilazione condizionale con la macro
  if che però non è una macro, o un'altra parola chiave"*.

---

## The milestones, one by one

One section per milestone that still has something to say, **in the order of the
table above** — so the **eight closed** ones come first and the **twelve** open or
scheduled ones after (CLAUDE.md §14: the table carries the numbers, the
sections carry the order, and neither repeats the other).

**Rows 1–21 have no section here**, and that is the rule rather than an omission:
a closed milestone's record is its journal, indexed at `docs/journal/README.md`.
The seven closed ones that do appear say **(closed …)** in their heading and are
here for one reason — their record is still the journal; what is kept here is the
reasoning a future milestone has to honour. **Two of them said nothing of the
kind until 2026-09-03** and went on describing their own work in the future
tense a day after it had shipped: M-package-layout and M-documentation-site,
both closed 2026-09-02. A closed milestone that reads as a plan is the one shape
this section's own split exists to prevent.

### M-selfhost-port — the port *(closed 2026-08-17)*

Rust → Heroes into `selfhost/` (the directory was born here), file by file, **the
goldens and M-program-corpus's corpus as the net**, the `PORT-DEBT` count as the
map. Every ordering-sensitive map walk becomes an explicit `sort` (panel 006), or
the fixpoint diff breaks.

**Carried in as a defect, not a feature: `-g`.** design.md §2 and §3.1 both state
that lldb breaks on and steps through `.hero` lines through the emitted `#line`
directives — and `-g` was passed to clang **only** under `--sanitize`, so an
ordinary build carried no DWARF and the claim had never been executed by
anything. It was repaired here because this is the milestone that needed it:
debugging a Heroes compiler written in Heroes is where the source mapping stops
being a nicety. A golden runs lldb in batch mode and asserts that a breakpoint on
a `.hero` line is hit (CLAUDE.md §9: every claim gets a test that makes it fire).

### M-selfhost-fixpoint — the fixpoint and the seed · **v1** *(closed 2026-08-18)*

A builds `B.c`, B builds `C.c`, `diff B.c C.c` empty — generated C, not binaries,
with the clang version pinned and recorded. The seed came with it:
`seed/heroes.c`, one clang line, no flags, tested from `git archive HEAD` because
a test run in the working tree proves nothing about a *checkout*.

**The archive left this milestone** (panel 085 B4) and became the two rows after
it. Three reasons, all measured: the port read its standard library from
`crates/` at run time, so the self-hosted compiler was already broken outside
this repository and blamed the author's line for it; `tests/differential.rs` —
the instrument that found that and two more — has the **bootstrap** as its
expectation, so archiving it removes the only thing that can ask whether the two
compilers agree; and `heroes measure` was not in the port, while design.md §1.6
cited the bootstrap's `measure/gate.rs` as the spec budget's live enforcer.

### M-harness-port — the net, in Heroes *(closed 2026-08-18)*

The 4,279 lines of Rust harness (`golden` 1,095 · `surface` 2,105 · `corpus` 544
· `milestones` 310 · `layout` 116 · `expectation` 109) became a Heroes program
run by the one command. **No compiler lines and no new surface**: a 79-line probe
ran 65 of 65 `check/` cases green, and exit codes, separate streams, `getenv` and
file comparison were each measured reachable. Predicted **2,500–3,760 lines**,
centre ~3,200, by three independent ratios; landed at 3,630.

Two things a straight port would have missed. `layout.rs` was a **rewrite** — it
walked `crates/**/*.rs`, the compiler became `selfhost/*.hero`, and 29 of 143
selfhost files already exceeded §11's 300 lines, so the check had to carry the
knot rule or go red on day one. And a directory walk had no sound cheap route:
`ls > file` plus `read_file` **splits a filename containing a newline into two
entries at exit 0**, measured, which §1.12 forbids — so it is
`popen`/`fgets`/`pclose`, measured working on both compilers, with the wait
status from `pclose`.

### M-bootstrap-archive — the third language dies *(closed 2026-08-19)*

`crates/` → `archive/bootstrap-rs/`, and the move was the last commit rather than
the first, because five things died with the bootstrap and each needed a
successor first — every one measured against what it replaced: `tests/emission/`
(142 programs, both compilers green on the same bytes), `heroes measure`
(3440 / 3512 / spread 72, identical), `suite_spec.hero` (8 checks, replacing 435
lines in which every check was `cfg(test)`), `heroes mutate` (538 mutants,
byte-identical report), and CI's seed leg. The `#~` invariant's runner needed
nothing — M-harness-port had already carried it — and neither did `doctor`, which
has never asked about cargo.

Panel 086 moved the spec-budget ledger to `docs/measurements/010` and gave it the
agreement check it had never had; `records/citations` was built the same day and
found nine dead citations that predated the archive.

**The one number carried forward**: the compiler's own tests cost 13 s through
the Rust and ~16 minutes through the seed. That is the price of self-hosting, and
it became its own decision — CI runs them at tags, the net on every push.

### M-separate-compilation — one `.c` per module *(closed 2026-08-26)*

**The record is [journal 025](journal/025-separate-compilation.md).** What stays
here is only what a later milestone has to honour.

**The four acceptance rows, and what closed each** (panel 030 R2 — they are what
lifted the ffi-pragmatist's veto):

| row | what it demands | closed by |
|---|---|---|
| 1 | the header travels with the `extern` into every calling TU, or an `extern` is never callable across a module boundary | step 5, via panel 033 R5's third answer: a qualified mention is a type error and the route is a Heroes function in the declaring module |
| 2 | headers and link flags enter the cache key, `runtime_text()` kept | step 7, by clang's own dependency listing rather than by naming headers |
| 3 | every dependency's emitted interface enters the key | by construction — a TU's text carries the prototypes and tag enums of everything it reaches, so a dependency's signature change moves the caller's key |
| 4 | a two-module FFI case with a wrong `extern` signature | step 10, in panel 093 R5's re-reading: the row as tabled ("exit 1 in both TUs") is unsatisfiable, because the caller is correct |

**Three things a later sitting must not re-derive.**

**The `extern` block is not pruned, and the architecture is why.** Panel 091
vetoed every pruning option with compiled evidence: pruning the `#include`
deletes a group record's C type, an uncalled group holds up a used one, the prune
takes the `-l` with the declaration and a constructor library stops running at
exit 0, and the probe is a **memory-safety instrument** — the only thing that
sees a `size_t` out-parameter declared `i32`, which when executed wrote four bytes
into an adjacent object with ASan and UBSan silent. One `.c` per module plus row 1
puts every group in exactly one TU, which is what makes the duplication go away
for free.

**A cache key can be blind to its own input by construction**, and this is the
shape to check first in anything that caches: an `extern constant` emits as
`return CONF_LIMIT;` whatever the header holds, so the emitted C is
byte-identical across a header edit. What stands between that and a wrong answer
at exit 0 is asking clang which files it actually opened. `tests/harness/suite_cache.hero`
is the instrument, and `suite_units.hero` holds what one translation unit must
contain.

#### What it does not deliver, and what the next sitting argues from

**The per-module build still does not beat the fused one.** Re-measured at the
close, one machine, one tree: per-module **cold 57.9 s**, **warm 45.2 s**; fused
**emit 40.4 s + one clang line 3.7 s = 44.1 s**. The gap is about a second, where
it was 8.7 before step 9's frontend repair and where this file once carried 127.

**And the reason is architectural rather than a defect**: panel 093 R4 puts the
emitted text **in** the cache key, so a warm build must emit all 157 translation
units to learn that it may reuse their objects. **A cache cannot skip the work
that computes its own key.** The frontend is now ~18% of a build where the open
item measured 83%, so an incremental frontend is the smaller half; the emission
is the larger one. Both numbers are in `docs/work/SCHEDULED.md` with their dates.

### M-package-layout — `use` paths, the qualifier, and where a program's files live *(closed 2026-09-02)*

**The record is [journal 027](journal/027-package-layout.md), and the rulings are
panels 099, 100 and 101.** Everything below this line is the section **as it read
the day the milestone opened**, kept rather than deleted for two reasons: a later
milestone on modules inherits the same constraints, and a brief that outlives its
own milestone is the only way to see what a sitting was not allowed to re-derive.
**So read the tenses as 2026-08-25's.** The paragraph that begins *"Today a module
name is one word"* describes the state this milestone **ended**: `use syntax/decl`
compiles today. Its diagnostic did not die with it and is the one thing in that
paragraph still live — measured 2026-09-03, `module_path_has_no_parts` now refuses
the **dotted** spelling, `use geom.shapes` (`tests/golden/check/use-has-a-path.hero`),
which is the form the ruling did not take.

**Scheduled by author decision 2026-08-25** (`/decide`), taken mid-M-separate-compilation
and recorded because the expectation came first: the author expected this
milestone's work to arrive with separate compilation, and separate compilation
delivers the build architecture instead — one `.c` per module, the per-module
cache. Organising `.hero` files into directories is a **different deliverable**,
so under CLAUDE.md §14 it is a different milestone rather than an area annexed by
one that already exists.

**What it delivers.** Today a module name is one word and nesting is refused by
name: `selfhost/parse/use_line.hero:47`, `error[module_path_has_no_parts]` — *"there
is no nesting: every `.hero` a program reads sits beside the file that names
it"*. This milestone decides what replaces that refusal — the spelling of a
`use` that names a path, **the qualifier it binds** (from `use shapes/geom`, is
the module `geom`, `shapes.geom`, or `shapes/geom`?), and how a program's files
are laid out on disk.

**The flat layout is a RULING, not an oversight — `docs/panel/032`, ratified
2026-08-12**, and this milestone reopens it rather than filling a silence. That
sitting took the same question with five seats: `C` (a directory is a module) was
**vetoed three times on three independent grounds**; `A` (Nim's quoted form) was
struck as dominated; `S` (stay flat) was adopted. So the sitting this milestone
convenes is bound by what 032 already fixed, and inherits four things rather than
re-deriving them:

- **the landing form is decided in advance.** 032 R4: if the author overrules,
  what lands is the sentence the warden and the ergonomist beat into shape
  together — **+26 spec tokens measured**, never the +38 that was tabled:

  ```
  - A `use` may be a path: `use syntax/decl` binds `decl`. Last parts are unique.
  ```

  So the qualifier's headline answer is already on the record: the module is
  `decl`, not `syntax.decl` and not `syntax/decl`.
- **what R4 leaves genuinely open** is root-relative against file-relative, with
  the ergonomist's prediction attached (≥50% of `use` lines from a subdirectory
  to a peer take the root-relative spelling; a file-relative compiler
  first-try-compiles a 6-file 3-directory program ~0% of the time). That, and
  not the separator, is the sitting's real subject.
- **R5 and R6 hold whether or not paths ever land**: the C name component is the
  **whole path**, not the last part (measured — last parts collided 22-way on
  `mod` over 169 paths, whole paths zero times), and `use` is scoped to *the
  program*, which must not inherit a global scope by silence when packages
  arrive.
- **panel 031's ergonomist declined to use a subdirectory at all**, from the
  spec alone, because it could not predict the qualifier — the measured cost of
  the silence, and the reason the question is worth a milestone rather than a
  footnote.

**Two of 032's own numbers have moved, measured 2026-08-25, and the sitting
should open with them rather than with the 2026-08-12 ones.** The spec-warden's
killer argument was renames — *"D renames ≥40 of 119 files to satisfy last-part
uniqueness"* — and on today's tree **zero of `selfhost/`'s 158 module names
collide**, so last-part uniqueness costs nothing there (repo-wide, over 451
`.hero` files, 9 stems collide). And the compiler-engineer predicted
`find selfhost -name '*.hero'` **≤ 45** at M-selfhost-port's close: measured at
the `m-selfhost-port` tag it was **143**, and it is **158** today — the
prediction is falsified by more than 3×, and the *"a flat tree stays small"*
premise under `S` went with it. Neither fact decides the question; both change
which side owes the argument.

**And what ordering it before M-package-manager costs is written down rather than
discovered later.** The deferral this replaces (ratified 2026-08-12) rested on
packages creating the pressure that decides the qualifier — naming a module of
another library. Sitting first means the panel decides from precedent, from the
compiler's own source, and from the corpus, **without** a distributed package in
hand. That is the trade the author took; if the sitting finds it cannot rule
without that case, the honest outcome is a conservative default and a return
condition, not an invented one.

### M-documentation-site — the whole language, anchored to programs that run *(closed 2026-09-02)*

**The record is [journal 030](journal/030-documentation-site.md).** The four
bullets below were the milestone's **brief**, written while it was scheduled;
what landed is the journal's to say, and the site's own register rules live in
`site/README.md` § Style guide. Two of the four bind **any later work on the
site** and are why this section is kept rather than folded into the journal: a
code block comes from `examples/`, and publishing is a hard stop.

- **The language documented in full**, page by page, for someone who has not read
  `spec/heroes-spec.md` — the spec is the control instrument, not the teaching
  text, and it is budgeted precisely so that it can never become one.
- **Every code block on the site is a file in `examples/`**, not a snippet typed
  into HTML. M-program-corpus is what makes this possible, and it converts
  documentation drift into a test failure: a check asserts that each block
  matches a program in the repository that compiles and runs. Documentation that
  cannot rot is worth more than documentation that is merely current.
- **A history of the language**, distilled from `DESIGN-LOG.md`, `docs/panel/`
  and the journals: what was decided, what was refused, and the U-turns —
  including the ones that look bad in retrospect, which are the ones worth
  reading.
- **Publishing stays a hard stop** (CLAUDE.md §14): the site is built here and
  goes outward only when the author says so.

### M-robustness-guards — the guards that shut the holes *(closed 2026-09-03)*

**Done 2026-09-03**, tag `m-robustness-guards`; the record is
[journal 031](journal/031-robustness-guards.md). What a later milestone has to
honour:

- **Robust over cheap, on every platform, and never a `-Wno-` flag** (author
  instruction 2026-09-03, four sentences, § Who scheduled what). A landing is
  measured on the Mac, the Linux image and the Windows box BEFORE its commit;
  a flag that hides what clang saw is not one of the options a sitting may choose.
- **The clang floor is 18** (`selfhost/cli/clang_floor.hero`; panel 103, author
  leave): the CI's Ubuntu leg, and the oldest clang whose `-ast-dump=json` shape
  the pointee check was measured on. Raising it owes a measurement of what the
  next-oldest CI leg builds with; lowering it promises a dump shape nobody read.
- **`HERO_RUNTIME_ABI` is 19** and the bump is a two-phase edit — emitter, then
  header, then the seed regenerated in the same commit (`seed/README.md`).
- **The stack guard is process-wide and names the function on POSIX only**
  (`runtime/parts/stack.c`): a C library's own thread that overflows is the hole
  M-isolated-threads owns, and Windows names the failure and not the function
  until dbghelp and a PDB are measured on the box.
- **Compiling a leaf below a nested program's root re-bases its `use` paths** —
  a stated rule with a fixture (`tests/golden/surface-fixtures/nested/`), not a
  defect; the historian's standing prediction on it (Zig #13970's shape) is
  scored the day a defect of that shape is filed.

### M-isolated-threads — concurrency

**Scheduled, no warrant.** design.md Part 7.13 — isolated per-thread heaps,
copying at the boundaries, OS threads, **no scheduler** — and its width (data
parallelism alone, or the mailbox too) is a panel question when it opens.
**The record must state whether the C11 backend can express the intended model at
all** (stack switching in the runtime, or a CPS/state-machine transform): cfront,
the direct ancestor of this architecture, was abandoned in 1993 after a failed
attempt to add exception support, having frozen around forms that could not carry
non-local control flow. If the answer is "not yet known", the deferral is a bet
and is logged as one (panel 030 R6).

### M-corpus-depth — the rung between a program and the compiler

**OPEN 2026-09-03**, scheduled and then moved ahead of M-isolated-threads the same
day by author instruction (§ Who scheduled what), from a plan measured that day.
**Its first two steps are not programs**: `heroes mutate` learns to check a module
below a nested program's root from that root (it refused the whole corpus for a
day), and the net gains the gate every leg runs plus the full score on the tag
run — because a corpus milestone measured by an instrument that cannot read the
corpus would be measuring nothing. Form coverage of `examples/` reached zero unexercised at
M-corpus-coverage (journal 029); what the corpus is thin in is **shape**, and it
was measured over the 78 files: the six `extern` programs are 65–192 lines and
single-module, none with a `variant`, a nested container, a generic or an
`@`-parameter structure — FFI and "real program" are disjoint sets; generics are
declared in three programs and never used across a `use`; direct self-recursion
lives in three programs at depths bounded by tiny inputs; one program checks an
answer somebody else wrote down (`sieve/`); one writes a file (`todo/`); and
between `json/` at 671 lines and the compiler at 50,452 there is nothing, where
Nim keeps `tests/manyloc/`, Zig `test/standalone/` and Rust `rustc-perf`'s pinned
crates (nine languages surveyed that day, twelve recurring patterns, ten of them
already here).

**Nine programs, three families.** Five with a public oracle — `nbody/`,
`spectral/`, `fannkuch/`, `binarytrees/` against the benchmarks game's published
outputs at a fixed n, `checksum/` against CRC-32's and Adler-32's catalogue check
values and RFC 4648's Base64 vectors — every number fetched from its source the day
it is written and quoted with its URL. Two large — `interpreter/`, ≥ 1,500 lines in
three directories with generics instantiated across modules and a 500-deep
expression as the stack guard's positive witness, and `query/`, a CSV query engine
over 5,000 rows it generates itself, copy-on-write measured at volume for the
first time. One FFI at program scale — `ledger/`, sqlite with ≥ 15 functions
bound, where sqlite's aggregate and Heroes' over the same rows must agree.

**Step 0 landed with the scheduling**: `heroes mutate` scored over the 35-program
corpus for the first time since 2026-08-13, the corpus leg timed alone, both in
`docs/measurements/014-mutate-over-thirty-five.md`; `examples/README.md`
rewritten to list every program; and two record faults repaired (§ Who scheduled
what names the third). The brief is the `SCHEDULED.md` item naming this
milestone; the record is journal 032 at close. Cost is projected at **+1:15** on
the corpus leg and checked at close against step 0's number; every FFI program is
measured on the Mac, the Linux image and the Windows box before its commit.

### M-package-manager — packages, and what stands in for a standard library

**Scheduled, no warrant.** Not a decision to take later: design.md:772 already
fixes the shape — *"No package manager exists before modules do; when it arrives
it will be `heroes add`/`heroes fetch` — inside the same binary"* (never a second
binary, CLAUDE.md §6 and §10). Its real prerequisite is
**M-separate-compilation**, not M-module-namespace: without separate compilation,
installing a package means recompiling the world on every build. **Since
2026-08-25 there is a second one in front of it** — M-package-layout, which rules
on how a fetched package's modules are named and reached; `heroes add` cannot
place files it has no spelling for.

**And this is where "a standard library that wraps C" goes.** §1.11 refuses a
standard library permanently, and that refusal is the founding constraint rather
than a shortage of effort — but what a standard library is *wanted* for arrives
here in a form the constraint permits: **distributable bindings**, ordinary
Heroes modules over real C headers, each with its link flag declared next to the
`extern` that needs it (§3.5). The difference is not cosmetic: a binding is
verified by clang against the header it names, and a standard library is verified
by whoever wrote it. **What this milestone does not deliver is the packages**:
those are M-core-packages, the row after it — and the sentence just above,
*verified by clang, or by whoever wrote it*, is the first question that
milestone's opening sitting has to answer for a package written in Heroes alone.

**Panel 056's return conditions live here, as that sitting ruled** (`docs/panel/056`
§ Resolution, deliverable D, ratified 2026-08-15 — and written into this entry only
on 2026-09-03, by a reasoning session that read the panel forward and found the
deliverable undischarged). A per-project file is refused (CLAUDE.md §10: there is
no fourth input class), and **three conditions, all met, return the question to a
panel**: *one key, not two* — a `prefix <dir>` from which `-I` and `-L` are both
derived, so the skew is unrepresentable; *no string in the file may also appear in
a `.hero` file*, so deleting the file never changes which symbols are linked, only
whether the build succeeds; and *one named binding that `package "<name>"` and one
command line cannot build* — a set that was empty on 2026-08-15. Meeting fewer
does not.

### M-core-packages — small packages that compose, and what a web server needs

**Scheduled, no warrant** (author instruction 2026-09-03; § Who scheduled what
has the words). It adds **no language form and no built-in**: panel 097
condition 5 keeps `selfhost/library_source.hero` closed, and panel 057 refused
`path_join` there on Principle 0. So Principle 0 binds nothing in this milestone;
what binds it is §1.11, and where its line falls for a package written in Heroes
alone is the first thing the opening sitting rules.

**What it delivers**: ordinary Heroes modules, distributable, organised as Go's
tree and reached by the `use` path form M-package-layout landed — `use net/http`
binds `http`, `use encoding/json` binds `json`, `as` renames (`spec:10-13`). Each
package is a directory with `test` blocks; a package over C is two modules, the
raw `extern` group and the idiomatic wrapper (panel 033 R5: an extern is never
callable across a module boundary). The list, with each package's source, was
measured on 2026-09-03 with probes on the Mac and in the Linux image; Windows was
not measured:

| package | source | today |
|---|---|---|
| `strings`, `path`, `net/url`, `log`, `sort` (generic — the built-in refuses a type parameter), `rand` (deterministic) | Heroes alone | writable |
| `encoding/json` | Heroes alone — `examples/json/`, 671 lines, already parses and renders | writable |
| `html/template` | Heroes alone — `examples/template/main.hero`, 235 lines, plus escaping | writable |
| `os` | `hero_os.h` and `getenv` — `selfhost/cli/process.hero:37-56` lifted out of the compiler | writable |
| `io`, `bufio` | `stdio.h` (`FILE*`); bulk text through `hero_str_from_bytes(p: ptr, n: i64)`, exported by panel 035 | writable, POSIX |
| `bytes` | Heroes plus `fmemopen`/`fgetc`, one C call per byte, until the sitting picks a route | writable, slow, POSIX |
| `time` | `time.h`: `time`, `timespec_get`, `strftime`, `gmtime_r`, `nanosleep` | writable, POSIX; `tv_nsec: i64` unmeasured on Windows |
| `math` | `math.h` `link "m"` | writable |
| `database/sql` | `sqlite3.h`, prepare/step; blobs through `bytes` | writable |
| `crypto/sha256`, `crypto/hmac` | `package "libcrypto"` | writable, Mac and Linux |
| `net/http` client | libcurl, two modules for `curl_easy_setopt`'s two value types (panel 094 R4) | writable |
| `net` | a runtime part, or one file per platform — the sitting's questions (iv) and (vi) | per platform only |
| `net/http` server | Heroes alone over `net`, `io`, `strings` | writable, one thread |

**Measured before it was scheduled, which is why it could be**: a single-threaded
HTTP server written in Heroes answered `curl` on 2026-09-03 with no language
change — `socket`, `bind`, `listen` and `accept` as `extern`s, the address as
`record SockAddr tag sockaddr` with the port's two bytes computed in Heroes (no
`htons`), the request read through `fdopen` and `fgets`, the response through
`write`; about 100 lines, and the same program on Linux with three lines of the
record changed (`sa_len` does not exist there, `sa_family` is `u16`). None of the
three gaps the brief expected was needed: not a byte buffer, not `htons`, not
`sockaddr_in`. And the callback boundary — a function value cannot cross the FFI,
`selfhost/check/ffi.hero:45` — is needed by **nothing on the list**: `sqlite3_exec`
is avoided by prepare/step, libcurl writes into `CURLOPT_WRITEDATA` with no
`CURLOPT_WRITEFUNCTION` (measured: a Heroes client fetched from the Heroes server),
`qsort` is the built-in `sort`. It is needed by threads and by event-driven server
libraries, so it is M-isolated-threads' question and is filed there.

**What the opening sitting rules, full five seats, before any step**: (i) the
§1.11 boundary for a package written in Heroes alone — the sentence in the entry
above, and the falsifier the answer owes (CLAUDE.md §12); (ii) where packages live
and how `use` reaches them — measured: a module that `use`s a sibling package
compiles only from the program's root, because `use` cannot climb (panel 099 R1),
so `heroes fetch` places a tree under the root and each tree wants a root-level
test driver; (iii) the byte buffer, three routes with their cost — per-byte
runtime entries (soundness lane, ABI +1), a `[u8]` result admitted for a group
over `heroes_runtime.h` (`ffi.hero:45` refuses `.array` today; a diagnostic and a
§4.19 sentence change), or a built-in (the route panel 036 refused for
`read_file`); (iv) `net` as a runtime part, because `sockaddr` differs between
Darwin and glibc — `error[ffi_field_type]` on the other platform, measured both
ways — and Windows is winsock: panel 097's `struct stat` shape, touching §1.11's
own row *Sockets: libc*; (v) Part 7 item 10 widened from `long` and `size_t` to a
typedef whose width **or sign** differs by platform, on the measurement that
`clockid_t` is `u32` on Darwin and `i32` on glibc, so `clock_gettime` has no
single spelling and `timespec_get` is the portable clock; (vi) **conditional
compilation** — the five shapes other languages use: C's textual `#if`; a
compile-time keyword in the body (Nim's `when`, D's `version`, Odin's `when`,
Swift's `#if`), whose inactive branch is not type-checked on this machine; Rust's
`cfg` attributes, likewise; one file per platform (Go's `net_linux.go`, Odin's
`_linux.odin`, Hare's `+linux`), every file a whole module checked on its platform
and no word in the body; and nothing in the language (Ada, Oberon), which is
where Heroes stands with the arm in `runtime/parts/`. The record the seats are
handed: panel 049 refused the platform axis with a veto, ratified 2026-08-14;
panel 097 put the arm in the runtime; panel 039 left comptime unplaced; §4.15
makes every textual difference semantic; and CI compares `seed/heroes.c` byte for
byte against what the compiler emits on every leg, so emission that depends on
the host breaks an instrument. The limit the sitting must name: the runtime is
the only C a package can add to, since panel 036 P2 vetoed `compile "shim.c"` —
so the shape Heroes has today serves the project's packages and nobody else's. A
refusal of any shape lands as a Part 6 row with its falsifier.

**Step order** — one package or one gap per step, each step a corpus program on
the three platforms or skipped by the missing-header rule: **0** three repairs
the probes found, each a `fixedbugs` case — `xs @ xs.push(c.to_u8().must())`
copies the whole array per push (100,000 pushes in 14.78 s against 0.00 s with
the value bound first, this Mac, 2026-09-03; `selfhost/ir/place_store.hero:100`
chooses `.push_owned` for a bound value and not for an inline `.must()`),
`@value: i32` against a `const void *` pointee is `internal error` at exit 2
(`setsockopt`; panel 103's pointee assertion writes `sizeof(const void)`), and
`ffi_unknown_name` for the macro-only `htons` on Darwin says the header declares
no such name when it does · **1** `strings` and `path`, and the copies leave
`examples/` — `split_lines` in 8 files, `is_space` in 8, `trimmed` in 7,
`index_of` in 4, and `tests/harness/strings.hero` carrying a fourth `trimmed` ·
**2** `encoding/json`, `net/url`, `html/template`, `log`, `sort`, `rand` · **3**
`os` · **4** `io` and `bufio` (M-corpus-depth's `wc` over stdin is the witness if
it has landed) · **5** `time` · **6** `math` · **7** the byte buffer, in the
sitting's route (M-corpus-depth's `checksum/` is the witness) · **8** `crypto` ·
**9** `database/sql` (M-corpus-depth's `ledger/`) · **10** `net`, in the sitting's
shape (the loopback program as `examples/echo/`) · **11** `net/http` client,
against a `file://` URL · **12** `net/http` server, two requests on one
connection · **13** `heroes fetch` under the root, the root-level driver, and the
`heroes check` item panel 091 filed under M-package-manager. Until `heroes fetch`
exists the packages live as directories under `examples/`, reached by `use` from a
program in the same root.

**Acceptance**: a small HTTP server built from the packages and nothing else, in
`examples/`, in the corpus's three configurations, in the loopback shape — server
and client in one process, deterministic output — because a listening server does
not fit `main.expected`.

### M-web-framework — the framework that composes the packages

**Scheduled, no warrant** (author instruction 2026-09-03, the same session). It
composes M-core-packages' packages in Go's and Echo's shape, everything explicit:
routes as a table of function values, `(function(Request) -> Response)` keyed by
method and path — top-level functions are values (`spec:112-117`); records for
request and response; middleware as a chain of functions, because v1 has no
closures (`selfhost/emit/ctype.hero:375-380`); templates from `html/template`,
bodies from `encoding/json`, rows from `database/sql`. No metaprogramming and no
dynamic dispatch: the Rails and Django shape rests on Part 6's own rows
(`design.md:2382-2404`), and `design.md:2395` says why — *"this is why LLMs err
more on Rails/RSpec than on plain Ruby"*. What it inherits from
M-isolated-threads is concurrency alone: `serve` takes one connection at a time
until a thread can take an accepted descriptor, and a `Request` record is exactly
the message the separate-heap model wants, small and copied once
(`design.md:2512-2524`). **Acceptance**: a small application — the corpus's `todo`
over a database — served over HTTP, on the three platforms. Its full brief is
written at M-core-packages' close, when the packages exist and what a framework
must add is measured rather than guessed.

### M-qbe-backend — the proof that the IR is not C in disguise

**Scheduled, no warrant**, and its warrant is stated here more honestly than
"a second backend" ever did. Two things were bought when panel 001 replaced QBE
with C emission, and one of them was never paid for: **as long as exactly one
backend exists, "the IR is target-agnostic" is an assertion no artifact tests**,
and the IR could be a C pre-processor wearing an abstraction's name without
anything in this repository noticing. QBE from the same IR (~500 lines) is what
turns that sentence into a measurement — and it restores the register-allocation
and instruction-selection lesson, which is the half of a compiler this project
deliberately handed to clang (DESIGN-LOG 2026-08-03, panel 001).

### M-lsp-server — `heroes lsp`

**Scheduled, no warrant.** ~250 lines of JSON-RPC: diagnostics on save,
formatting, hover, documentSymbol. It blocks nothing and could land any time
after M-rich-diagnostics; it is here rather than earlier by the author's choice,
and M-vscode-extension is what consumes it.

### M-vscode-extension — the extension, complete

**Scheduled, no warrant.** `editors/vscode/` already ships the TextMate grammar,
the language configuration and the icon theme; this milestone makes it an
extension somebody could install and forget about.

- **The LSP client**, speaking to M-lsp-server's `heroes lsp`: diagnostics as you
  type, hover, go-to-definition, document symbols, formatting through
  `heroes fmt`.
- **Code actions from the fixes that already exist.** §4.17's `Fix`es are tagged
  `certain | guess` and `heroes check --apply` already applies the certain ones;
  the extension surfaces exactly those as quick fixes and never the guesses. This
  costs almost nothing and is the thesis made visible in the editor — the
  likeliest mistake arrives with its repair pre-written.
- **Debugging**, and the honest shape of it first: the emitted C carries `#line`
  back to `.hero` (with `-g` repaired at M-selfhost-port), so the debug info is
  ordinary DWARF pointing at Heroes source. `lldb-dap` therefore composes with
  the generated binary without this project writing a debug adapter — which is
  CLAUDE.md §10's *"nothing if two existing invocations already compose to it"*.
  If a launch configuration cannot be expressed that way, `heroes dap` enters
  under the stopping rule like any other verb, with the reason recorded. The
  known ceiling is design.md §2's: `p x` shows a mangled C temporary rather than
  a Heroes value. Typed inspection is not in v1 and this milestone does not
  smuggle it in.
- **Packaging**: a `.vsix` that installs, with `heroes doctor` as the extension's
  own health check.

### M-interpolation-verdict — the ruling on string interpolation

**Scheduled by author instruction 2026-09-02**, and what it delivers is a
**decision**, not a feature: design.md Part 7 item 7 — *"String interpolation —
deferred; `print` takes multiple arguments"* — evaluated for the first time, against
the shape every other language spells `f"{name}"`, `$"{name}"`, `\(name)` or
`${name}`.

**Why it is admissible now, and was not before.** Part 7's preamble is categorical:
*"Nothing on this list is considered until the Principle 0 closure list (§1.0)
compiles itself"*, and CLAUDE.md §13 repeats it as a place not to go. That condition
has been met since M-selfhost-fixpoint, 2026-08-18. So the bar this item has never
been held to is Principle 0's own, and **it is not on the closure list** — the
compiler self-hosts today with none of it — which leaves the whole warrant to a
**measured Part 11 effect**. A sitting that cannot produce one closes with a
refusal, and that is a legitimate close for this milestone rather than a failure of
it.

**A refusal costs the same as a feature** (CLAUDE.md §12, author decision
2026-08-12): if the answer is no, what lands is a design.md Part 6 row naming the
program or the compiler fact that would make it wrong — not a second deferral. Part
7 item 7 has been one line with no argument since the day it was written, and this
milestone exists to end that either way.

**What the sitting must not re-derive: the trade was already made, in the other
direction.** `design.md:1403` — `print`'s variadic-looking form is *"compiler-known,
not a function value"* and *"was bought by trading away string interpolation (Part 7
item 7)"*, panel 006 (`docs/panel/006-map-order-print.md`), with Pascal's `WriteLn`
as the fifty-year precedent. The question is therefore not *may we add sugar*, it is
**may we buy back something already sold**, and whoever proposes it owes the other
side of that trade in spec tokens and says what `print` becomes afterwards.

**The whole repository says two things about interpolation and both are in
design.md** — measured 2026-09-02 with `grep -rn -i interpolat` over `docs/panel/`,
`docs/journal/` and `docs/measurements/`: **zero hits**. There is no ruling to read
forward from (CLAUDE.md §1), which is why this is a milestone and not a footnote.

**What its absence costs today, every number measured 2026-09-02.**

- **945** lines of `selfhost/` hold the sequence `" + ` — a string literal
  concatenated to something — and **146** hold a `to_str()` call. That is this
  compiler's own diagnostics being assembled by hand, and it is the largest single
  body of evidence in the repository.
- **118** `print(` calls in `examples/` carry a comma: the multi-argument form
  panel 006 bought instead.
- `examples/template/main.hero` (**235** lines) **already interpolates at run
  time**, and it is the witness the sitting must hold rather than imagine. It picks
  `{key}` with `{{` as the escape for a literal brace, refuses an unknown key
  instead of leaving the hole, and its module doc defends both choices in writing.
  A corpus program found a defect in exactly that escape rule
  ([029](journal/029-corpus-coverage.md): `{{name}}` was refused because the rule
  protected `{{` and not `}}`), which is the cheapest available demonstration that
  the rule is not free.
- The spec stands at **3685** tokens of a hard 4096, headroom **411**, spread 79
  (`heroes measure`, this session). Headroom exists; §1.2 still prices the addition
  against a named removal or a pre-registered prediction.

**The three questions, in the order they bite — the spelling is the last of them.**

1. **What may stand inside a hole**: a name, an expression, a call? A bare name is
   the cheapest rule to write and to lex, and it is the one that reads worst on the
   day somebody wants `{count + 1}`.
2. **How a value renders.** This half is already normative and costs nothing: every
   type has a canonical `to_str`, and design.md fixes `f64`'s as round-trip-exact
   rather than shortest. An interpolation that rendered differently would introduce
   a **second** rendering rule, which is the expensive answer.
3. **The spelling, and the escape it forces.** `spec:74` fixes the escape set, and
   panel 008 (`docs/panel/008-escape-sequences.md`, ratified 2026-08-04) reserved
   the backslash — *"any other character after `\` is a compile error"* — with R3
   stating that a new escape **reconvenes that panel**. So Swift's `\(name)` is not
   a free spelling: it reopens 008. And `{` already means a map in this language
   (`{K: V}`), so the brace spelling owes the doubling rule
   `examples/template/main.hero` already implements, or an argument against it.

**Full five seats, not the soundness lane** (`/panel`): the form has surface, a spec
token cost and at least one diagnostic class. The llm-ergonomist's seat is the one
that decides it, because the thesis is the only warrant available — and it receives
`spec/heroes-spec.md` and sample programs only, never this section.

**The cost of ordering it here is declared rather than discovered.** It sits after
the tools and before the two books because a book is the expensive consumer: a
surface form that lands after M-guide-book rewrites chapters in two languages, while
one that lands after M-vscode-extension adds a rule to a grammar file. CLAUDE.md §9
is the bill a new form arrives with — the formatter, every `--dump-<stage>` printer,
`heroes mutate`, the diagnostics that quote a program back, and `heroes measure`
where the form has spec text — and it was written the day `as` reached six consumers
one at a time, with the formatter silently deleting it from a working program.

### M-journey-book — the journey

**Scheduled, no warrant.** The narrative book: the itch, the design that met five
hostile experts, the U-turns, the deleted darlings, the days the machine found
the bug in the plan before we did — and what it was like to build a compiler with
an AI assistant. `docs/book/README.md` has been collecting the raw material since
M-day-zero and nothing extra needs maintaining: the journals are the spine,
`DESIGN-LOG.md` the decisions, `docs/panel/` the arguments, `docs/book/beats.md`
the human texture the technical records drop, the measurements the numbers, and
`git checkout m2` re-opens any chapter's code.

The rule from design.md's *The name* applies here and only here: **personality in
the packaging, precision in the substrate** — Bowie belongs in this book, never
in an error message.

### M-guide-book — the guide

**Scheduled**, and it is the one milestone after the fixpoint that has a warrant.
The classic language guide, the K&R shape: read it front to back and you can
write Heroes; open it in the middle and you find the thing you were looking for.

- **Organised by subject, not by chronology**: values and types, bindings and
  `@`, control flow, records and variants, `T?` and failure, generics, the
  module, the FFI, the test blocks — each with the smallest program that shows it
  and one that gets it wrong on purpose.
- **It is not the spec, and it must never try to be.** `spec/heroes-spec.md` is
  the control instrument, budgeted at 4096 tokens precisely so that it can never
  become a teaching text; the guide is where the explanations, the worked
  examples and the "why it is like this" live, at whatever length clarity needs.
- **Its warrant is §1.1**: comprehension is this project's objective, measured
  rather than asserted, and a guide the author can read to rebuild the reasoning
  is that objective's final artifact.
- Where the guide and the spec disagree, **the spec wins and the guide has the
  bug** (CLAUDE.md §12) — and where the guide and the *compiler* disagree, that
  is a defect report on one of them, which is what the M-documentation-site check
  over `examples/` is for.
- **Metric 2's held-out tasks land here** (author decision 2026-08-24): they must
  be author-written or they measure the assistant's priors, and this is the one
  milestone whose own work *is* the author writing programs.

### M-publication-gate — the last gate

The repository is **private** today and publishing is a hard stop that only the
author lifts (CLAUDE.md §14). This entry is the checklist that has to be true
first, and it exists because most of its items get worse the longer they wait.

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
  fail, so a program that goes wrong still tells the shell it succeeded (queued
  from panel 030). Whatever M-ffi-ladder decides for `exit(code)`, this must not
  be true on the day the examples go up.

---

## Decisions this file records

### `scheduled, no warrant` is not decoration

Part 7's preamble defers everything on its list until the closure list compiles
itself, and **a place in the table is not a warrant**. Measurement 003 rider 3 is
the standing precedent: this file scheduled `outline` and `explain`, and
CLAUDE.md §10's stopping rule refused them.

### Two milestones were asked for and neither was added

Asked 2026-08-12; the table is unchanged and this is why (panels 033 and 034).

**Visibility**: three tiers, and two of them were never visibility questions —
private record fields are an opaque type (§4.9 makes construction impossible from
outside, and §4.20 makes a shim read the field anyway) and private variant cases
are `#[non_exhaustive]`, which Rust deleted in 2014, re-added per type in 2019
and documents as costing exhaustiveness. Both are now **Part 6, permanently**.
The third, `private` on a declaration, is **Part 7 item 14** at a pre-fixed +18;
M-selfhost-probe was assigned to decide it and **did, at its close 2026-08-15: no
blockage, so it stays Part 7** — eleven modules ported, every cross-module read
intended, and the rule's own words (*"a blockage there puts it on the closure
list, a wish does not"*) made the close mechanical.

**Errors**: the Rust shape landed at M-optional-map — `T?` is `Result<T,E>`, `?`
is `?`, `.must()` is `.unwrap()` — and the part Rust has that Heroes does not,
the typed error, stays **Part 8 wart 5** rather than becoming a deferral, because
it loses on §4.12's positive rule as well as on simplicity. What is real
underneath the question is measured: `docs/measurements/004-error-codes.md`,
**25 mutants, 0 caught**, and the answer is a `constant`, not a feature.

### The two books, and one rule that governs both

**They are written in simple, simple language** — the register of the `/where`
skill, which explains this project assuming zero compiler knowledge. The author's
instruction is the reason and it outranks elegance, brevity and completeness: he
will read these to *study* what was built, so a sentence that needs a compiler
course to parse is a sentence to rewrite.

**Both exist in Italian and English, and this is the one declared exception** to
CLAUDE.md §11's "everything written is English" (§11 records it). Neither version
is a machine translation of the other; the Italian is the one the author studies
from, so where the two diverge, the Italian is fixed to be clearer rather than
the English to be more faithful.

**Both teach with M-program-corpus's programs** — code known to compile, run and
pass its own tests in three configurations, rather than snippets that were true
once.

---

## The names

Milestones were numbered until 2026-08-12 and are named now. The algorithm that
assigns the next one is **CLAUDE.md §14** — its only home; this section is only
the map, and it exists because **the record was not rewritten**. `docs/panel/`,
`DESIGN-LOG.md`, `docs/journal/`, `docs/measurements/`, `docs/defects/`,
`docs/book/beats.md`, `tests/golden/`, every commit subject and all twelve legacy
tags keep the identifiers they were written with. Panel 030 R7, as amended, is
the argument.

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
| `M-harness-port` | — | `m-harness-port` | the net in Heroes: the golden harness, the corpus, the record checks. **A new id and not part of M8c** (§14): panel 085 B4 split the archive off the fixpoint, and the archive's precondition is a milestone of its own — 4,279 lines of Rust harness, and the instrument that dies with the bootstrap is the one whose expectation *is* the bootstrap |
| `M-bootstrap-archive` | M8c, in part | `m-bootstrap-archive` | `crates/` → `archive/bootstrap-rs/`. **The clause M8c was carrying and could not pay**: the port read its standard library from the directory being archived, so the compiler failed outside this repository, and `heroes measure` was still not in the port |
| `M-separate-compilation` | M9 | — | one `.c` per module, prototypes across TUs, the cache |
| `M-package-layout` | — | — | `use` paths, the qualifier, where a program's files live. **A new id rather than an area annexed** (§14, author decision 2026-08-25): M-separate-compilation delivers the build architecture and M-package-manager delivers `heroes add`, so the directories question — which had been scheduled inside the latter since 2026-08-12 — is a third deliverable and takes a name of its own |
| `M-corpus-coverage` | — | `m-corpus-coverage` | every language form has a program that runs it. **A new id rather than a reopening of `M-program-corpus`** (§14, author instruction 2026-09-02), which closed 2026-08-13 delivering *many whole programs, all of them run*; this one delivered *every language form has a program*. **Done 2026-09-02**: twenty programs, `examples/` 15 → 35, and the first program written to close the measured library gap found a compiler defect on its first run |
| `M-selfhost-nesting` | — | `m-selfhost-nesting` | the compiler's own modules move into directories by subsystem. **Done 2026-09-02**, and the row keeps its scheduling note because two of its numbers were wrong and the record should say so: it said **169** modules where the tree held **170**, and it said nesting puts 32 files into 13 last-part collision groups that `module_last_parts_collide` refuses — true of the program-wide rule, which panel 100 R3 replaced with a per-file one before this milestone ran. Under the rule that actually landed the number is **41 files needing 43 bindings disambiguated**, of which 10 modules took `as` and 3 locals were renamed. The block it named — panel 100's verdict on the alias — was real and was lifted. What no version of this row foresaw is panel 102: the prefixes had been holding module names out of the namespace where values live, so the collisions that mattered were **local-vs-module**, 0 flat and 23 nested |
| `M-package-manager` | M10 | — | `heroes add`/`heroes fetch`, bindings in place of a standard library |
| `M-isolated-threads` | M11 | — | Part 7.13: per-thread heaps, copying at the boundaries, no scheduler |
| `M-qbe-backend` | M12 | — | Part 7.14: the proof that the IR is not C in disguise |
| `M-lsp-server` | M13 | — | `heroes lsp` |
| `M-vscode-extension` | M14 | — | the extension, complete |
| `M-documentation-site` | M15 | `m-documentation-site` | the site. **Done 2026-09-02, out of chain order by author instruction**: the module system had just landed and the site had no page about it, so a reader could learn how numbers, errors and the C boundary work and leave with no idea how to write a program in more than one file |
| `M-interpolation-verdict` | — | — | the ruling on string interpolation — design.md Part 7 item 7, evaluated for the first time since the fixpoint made Part 7 admissible at all. **The verdict is the deliverable and the name says so** (§14, author instruction 2026-09-02): the milestone has to be able to close with a refusal, so an id naming the feature would claim the very thing the sitting exists to decide, and a later milestone could falsify it |
| `M-journey-book` | M16 | — | the journey |
| `M-guide-book` | M17 | — | the guide |
| `M-argv-execution` | — | — | the compiler runs programs by argument list, and the shell stops being the boundary |
| `M-publication-gate` | M18 | — | the last gate before anything goes outward |
| `M-robustness-guards` | — | `m-robustness-guards` | the guards that shut the holes §1.12 named. **Done 2026-09-03**, the day it opened: six steps, two sittings (103, 104), every landing measured on the Mac, the Linux image and the Windows box before its commit |
| `M-corpus-depth` | — | — | the rung between a program and the compiler: nine programs chosen for shape — oracle-checked, deep, FFI at program scale. **A new id rather than a third reopening of the corpus** (§14, author instruction 2026-09-03): `M-program-corpus` delivered *many programs run*, `M-corpus-coverage` *every form has a program*, and this one delivers *size, depth and an external oracle*, which neither name claims |
| `M-core-packages` | — | — | small packages that compose, organised as Go's tree, in Heroes or over C. **A new id rather than an area annexed** (§14, author instruction 2026-09-03): M-package-layout delivered how a `use` reaches a module, M-package-manager delivers `heroes add`/`heroes fetch` and where a fetched package lives, and this one delivers the packages themselves — a deliverable neither name claims. The word is Odin's `core:` collection, which §1.11 cites for the reason a package can be redesigned and a built-in cannot |
| `M-web-framework` | — | — | the framework that composes the core packages, Go/Echo style. **A second id and not a step of the one above** (§14, author instruction 2026-09-03, *"poi quello web che li usa tutti"*): the packages are a deliverable with or without a framework, and a framework is falsifiable on its own — it exists when the corpus's `todo` is served over HTTP on three platforms |

**`M8` has no row, because it meant three different things.** It was an umbrella
that predates the a/b/c/e/p split and no heading has carried it since. In the
record it reads as the **fixpoint** (panels 006, 027 ×2, 028, 029), as the
**port** (panel 022 — *"instruction lists at M8"*), and as the **whole span**
(panel 005 — *"all of M0–M8"*). Read the sentence, not the number. Its two living
sites were resolved by hand to `M-selfhost-fixpoint`.

**`M10`–`M13` resolve two ways, and the date decides which.** Packages were
inserted ahead of concurrency after the chain was first written down, so
everything from `DESIGN-LOG.md`'s *"the post-fixpoint order, by author
instruction"* row onward uses the table above. Two earlier sites use the
superseded numbering, where **M10 was concurrency, M11 QBE and M12 `lsp`**:
`DESIGN-LOG.md`'s build-order row and panel 030's own body. This ambiguity
predates the rename and is exactly the failure mode R7's precedent warns about —
a number that still resolves, to the wrong thing. A name cannot do it, which is
the whole reason for this section.

**Two names are not the deliverable's obvious one, and the reason is on the
record.** `M-generics-library` covers four concerns (sugar, tests, generics,
library); the runner-up `M-language` was refused because four of §1.0's rows were
still open, so it overclaimed. `M-strings-ownership` names both halves rather
than just `str`, because the ownership pass is the architecturally load-bearing
one and the milestone's journal slug names both too.
