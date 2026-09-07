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

**Reorganised 2026-08-25 by author instruction** — *"rewrite it completely
without losing any information, in a tidier way"*. Nothing was dropped.
What changed is that the same milestones used to be listed in **three** separate
tables — the order, the done list, and the name map — and are now in one, with
the name map kept separately because CLAUDE.md §14 cites it and a cited record is
not merged away.

**Reorganised again 2026-08-26 by author instruction** — *"put the summary table
at the top and then all the steps below it, in order"*. That pass merged the
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

**Reordered a third time 2026-09-03 by author instruction** — *"reorder the roadmap
with everything that has been done before everything that is still to do, and
move all the notes out of the table … I want the table clean"*. Two
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
| **Current milestone** | **M-discard-refusal** — **OPEN**, row 38: `_ =` on a fallible value becomes a compile error. The verdict is the author's and given (`/decide` answer `3a`, 2026-09-03); the sitting is still owed, because a spec clause and a diagnostic class are both language (CLAUDE.md §4) |
| **Last closed** | **M-declared-freer**, 2026-09-07, tag `m-declared-freer` ([036](journal/036-declared-freer.md)) — five steps, two sittings, four `internal error`s repaired. `owned <C function>` frees the string C hands you, in both positions · v1 **reached** at M-selfhost-fixpoint, 2026-08-18 |
| Milestones closed | **37** of 59 · **37** tags |
| The compiler | **55,050** lines of Heroes in **189** modules · the seed **747,095** lines of C · runtime ABI **21**, unmoved: the mark needed no runtime entry point, because the release is built out of instructions the IR already had |
| The spec | **3871** of a hard 4096 · headroom **225** — spent at M-declared-freer step 2 on panel 109's wording, and **unmoved since**: the `@` cell the milestone finished with was already described by the sentence that step bought |
| Records | sittings **115** · journals **37** · examples **55** programs, **118** files, **545** `test` blocks · open defects **1** · the site **46** pages, 23 English and 23 Italian |
| Waiting on the author | **0** decisions · **41** in `SCHEDULED.md` · **1** in `DEFECTS.md` (016, `assert` drops both sides for every aggregate against `spec:202`) · **319** in `LEARN.md` (never a gate) · an outstanding veto (`docs/panel/101` R3) |

Every number re-counted 2026-09-07 at M-declared-freer's close, none carried. Three suites green: **588**, **1609**, **113**. What moved for the milestone is the compiler and the shape of two files: **186 → 189** modules, `ir/lower.hero` **1511 → 174** and `parse/decl.hero` **638 → 217**, both now under §11's own 300 and out of `suite_layout.hero`'s `DECIDED` table altogether — panel 109's coordinator had predicted **≤ 900** and **≤ 400**. The number to watch is the other direction: `check/walk.hero` is at **1695 of a decided 1700**, five lines of room, and it is the file every new diagnostic reaches.

---

## Verify it yourself

### From a cold checkout, right now

```sh
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes   # the compiler, from C alone (~3.5 s)
./heroes run tests/harness/main.hero -- ./heroes             # the net
./heroes test tests/harness/main.hero                        # the net's own tests (108, 11 s, 2026-09-05)
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

One table, one row per milestone, **closed first and scheduled after**: rows 1–37
are done, in the order they closed, and rows 38–59 are what is next, in the order
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
| 33 | **M-corpus-depth** | done 2026-09-04 | `m-corpus-depth` | [032](journal/032-corpus-depth.md) | the rung between a program and the compiler: nine programs, and half of every frame · **§1.1** |
| 34 | **M-c-callbacks** | done 2026-09-05 | `m-c-callbacks` | [033](journal/033-c-callbacks.md) | a Heroes function reaches a C callback parameter, and a foreign thread is refused by name rather than left to corrupt · **§1.11**, **§1.12** |
| 35 | **M-isolated-threads** | done 2026-09-06 | `m-isolated-threads` | [034](journal/034-isolated-threads.md) | Part 7.13 concurrency: three of four measured corruption classes closed, and the door was in the checker |
| 36 | **M-thread-stacks** | done 2026-09-06 | `m-thread-stacks` | [035](journal/035-thread-stacks.md) | the guard speaks on every thread, and a worker's floor is the thread that ran `main` · **§1.12**
| 37 | **M-declared-freer** | done 2026-09-07 | `m-declared-freer` | [036](journal/036-declared-freer.md) | `owned <C function>`: the string C hands you is freed by the name its own declaration gives · **§1.12**
| 38 | **M-discard-refusal** | **OPEN** | — | — | `_ =` on a fallible value becomes a compile error · **§1.1**
| 39 | **M-closures-verdict** | scheduled | — | — | the ruling on Part 7 items 1 and 12, closures and inline blocks — a decision, not a feature |
| 40 | **M-interpolation-verdict** | scheduled | — | — | the ruling on design.md Part 7 item 7, string interpolation — a decision, not a feature |
| 41 | **M-reflection-verdict** | scheduled | — | — | the ruling on reflection — at run time, and as compile-time derivation over a record's fields — a decision, not a feature |
| 42 | **M-deferral-ledger** | scheduled | — | — | every Part 7 item with no milestone gets a dated verdict or a return condition |
| 43 | **M-check-completeness** | scheduled | — | — | what `heroes check` accepts, `heroes build` compiles — through a generic too · scheduled, no warrant
| 44 | **M-core-packages** | scheduled | — | — | small packages that compose, organised as Go's tree, in Heroes or over C |
| 45 | **M-package-manager** | scheduled | — | — | `heroes add`/`heroes fetch`; bindings instead of a standard library |
| 46 | **M-web-framework** | scheduled | — | — | composes the core packages, Go/Echo style: explicit routes, records, no magic |
| 47 | **M-doc-generator** | scheduled | — | — | `heroes doc`, the one direction Part 6's literate-source row promises · scheduled, no warrant |
| 48 | **M-panic-location** | scheduled | — | — | a panic names the `.hero` file, line and function · **§1.12** |
| 49 | **M-typed-inspection** | scheduled | — | — | a stopped program shows Heroes values: the name the author typed, and `[T]`, `{K: V}`, `T?`, a variant and a record shown as themselves · scheduled, no warrant |
| 50 | **M-generated-programs** | scheduled | — | — | programs nobody wrote: a generator that composes valid Heroes and knows the answer before the compiler is asked · **§1.12** |
| 51 | **M-thesis-harness** | scheduled | — | — | Part 11's metrics 2 and 4 run for the first time, as a Heroes program · **§1.1** |
| 52 | **M-lsp-server** | scheduled | — | — | `heroes lsp`, and the incremental frontend it needs |
| 53 | **M-vscode-extension** | scheduled | — | — | the extension, complete |
| 54 | **M-qbe-backend** | scheduled | — | — | Part 7 item 15 — the proof that the IR is not C in disguise |
| 55 | **M-journey-book** | scheduled | — | — | the journey — how this language came to be |
| 56 | **M-guide-book** | scheduled | — | — | the guide, as a book you would find in a shop · **§1.1** |
| 57 | **M-install-channels** | scheduled | — | — | a Homebrew tap, winget, a Nix flake, a Docker image, all built from the seed, and a version scheme |
| 58 | **M-online-compiler** | scheduled | — | — | the compiler reached without installing anything: the site's visitor writes Heroes and gets its answer · scheduled, no warrant |
| 59 | **M-publication-gate** | scheduled | — | — | the last gate before anything goes outward · CLAUDE.md §14 |

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
  rather than in a row's position, which is what the author's *"I want the
  table clean"* decided.
- **M-isolated-threads** — **moved ahead of packages by author instruction,
  2026-08-25.**
- **M-interpolation-verdict** — **scheduled by author instruction 2026-09-02.**
- **M-robustness-guards** — **opened by author instruction 2026-09-03**, ahead of
  M-isolated-threads, out of a `/decide` sitting that closed ten items at once:
  *"ratify everything … choose the most robust and complete solutions over the
  cheaper ones … favour consolidation and the better solutions, not the
  shortcuts … more history and less attention to the token … more robustness on
  every platform, do not silence errors"*. Its six steps are the six `SCHEDULED.md`
  items that name it; two of them are sittings (the FFI pointer verdict, full five
  seats; the stack guard, soundness lane). The `records/names` check found the id in
  a list before it had a row here, which is the order CLAUDE.md §14 wants.
- **M-corpus-depth** — **closed 2026-09-04.** Scheduled by author instruction 2026-09-03, *"think
  about whether to add further examples, some of them more complex too, to have
  a wider net; look at what other languages have done as well"*, from a
  plan measured and approved the same day. The author placed it after
  M-robustness-guards, which closed that afternoon, so it first stood at 34 behind
  the open M-isolated-threads. Its step 0 — the first `heroes mutate` score over
  the 35-program corpus since 2026-08-13, and the corpus leg timed alone — landed
  with the scheduling, so the "before" exists before the first program does.
  **Then step 0 found `heroes mutate` unable to read `examples/` at all**, refused
  since 2026-09-02 (measurement 014), and the author moved the milestone to 33
  and opened it the same evening — *"bring those two steps forward right now, before
  M-isolated-threads"*, with *"the gate on every branch + the full score on the
  tags"*
  for the CI question — so that the metric the thesis rests on is repaired
  before anything else is measured against it. **Two milestones are open at
  once**, in two sessions, and the table says so rather than hiding one.
- **M-core-packages** and **M-web-framework** — **scheduled by author instruction
  2026-09-03**, the same evening, out of a reasoning session
  (`DESIGN-LOG.md:537`, which carries what it measured): *"I would like
  to have every tool needed to build a web framework in the style of Rails or
  Django, or even thinner, like Go, Echo or FastAPI"*, and then, when a single
  toolkit was proposed, *"I picture several packages that combine, and then the
  web one that uses them all; the model is Go, as organisation"*. Placed after
  M-package-manager because `heroes fetch` is what makes a package a thing you
  distribute. **Reversed later the same evening** — the packages stand before the
  manager since the reorder, and the bullets below say why. **Two rows because they are two deliverables** (CLAUDE.md §14): the
  packages, and the framework that composes them. The level is Go's and Echo's —
  everything explicit — because the Rails and Django shape rests on Part 6's own
  rows, metaprogramming, dynamic dispatch and inheritance (`design.md:2382-2404`).
  **The sitting sits at the opening, not at the scheduling**, on
  M-interpolation-verdict's precedent, and the author added its sixth question the
  same night: conditional compilation — *"conditional compilation with the
  if macro that is not a macro, though, or another keyword"*.
- **The chain was re-read and reordered late on 2026-09-03, by author
  instruction** — *"let us do a big think about the roadmap and about the
  things in decide and scheduled too … let us judge whether there are steps we
  have not considered so far … then we reorder them all in a very logical order"* — out
  of a reasoning session (`DESIGN-LOG.md:539`). Three
  faults in the order were measured and each moved a row: the language's one
  scheduled ruling sat at row 41, behind the packages, the framework, QBE and
  both tools, so **51,788 lines of Heroes in 178 modules** — the size of the one
  body of that kind today, measured 2026-09-03 — would have been written
  before it; Part 7 item 1 — closures, *"v1.5, immediately after the first
  running program"*, and that program ran on 2026-08-04 — had no row at all while
  M-web-framework's entry planned its middleware around the absence; and
  M-package-manager stood ahead of M-core-packages while the packages' own step
  13 was `heroes fetch`. **Seven rows entered, every one put to the author with
  a recommendation and every one accepted, one against it.** The numbers in the
  bullets below are **the ones those rows had on 2026-09-03**; four rows entered
  on 2026-09-04 and moved every number after 34, which is why this section keys
  by name and why the table above is the only current answer to *where*.
- **M-closures-verdict, M-reflection-verdict and M-deferral-ledger** — rows 35,
  37 and 38, with **M-interpolation-verdict** moved from 41 to 36 beside them:
  every ruling on the language's shape sits before M-core-packages, because the
  packages and the framework are the largest body of Heroes that will ever be
  written against the spec after the compiler, and a form that lands after them
  is a form they were written without. The spec had **378** tokens of headroom
  that evening (`heroes measure`: 3718 of 4096).
- **M-core-packages before M-package-manager** — rows 39 and 40, reversing the
  same evening's earlier scheduling: one distributes what exists, the packages'
  step 13 was already `heroes fetch`, and question (ii) of their opening sitting
  decides where a package lives, which is what `fetch` has to know. The `fetch`
  step moves to M-package-manager together with the root-level driver.
- **M-doc-generator** — row 42, **accepted against the recommendation**:
  `heroes doc` is in the record once, as the promise in Part 6's literate-source
  row, and CLAUDE.md §10's stopping rule refused two ROADMAP-scheduled verbs
  before it (`docs/measurements/003` rider 3, `outline` and `explain`). It
  stands after M-web-framework so that its witness — the packages' own API —
  exists, and its entry names the rule as the first question of its opening.
- **M-panic-location** — row 43, the cheapest half of *"a strong runtime"*: a
  panic today prints `panic: <msg>` and aborts with no line and no function
  (`runtime/parts/panic.c`), while the emitted C already carries `#line`. Placed
  after the runtime leaves M-isolated-threads' hands; it depends on nothing else
  and may be taken the day the threads land.
- **M-thesis-harness** — row 44: Part 11's metrics 2 and 4 have never run, and
  the instrument is a Heroes program over the HTTP client M-core-packages
  delivers. **The author's decision the same night**: a small seed of tasks
  written by the author at its opening, the bulk still at M-guide-book as decided
  2026-08-24.
- **M-lsp-server takes the incremental frontend** — the `SCHEDULED.md` item that
  asked *which milestone does it* has its answer: a server that re-checks on every
  save cannot wait 8 s for `heroes check` on the compiler's own source.
- **M-qbe-backend after the tools** — row 47, from 38: the proof that the IR is
  target-agnostic is worth most when the IR has stopped moving, and every ruling
  above it may move it. Its cells said *Part 7.14* and now say item 15: item 14
  has been declaration visibility since panel 033 (`design.md:2562`, `:2579`).
- **M-install-channels** — row 50, before the gate: a Homebrew tap, winget, a Nix
  flake and a Docker image, all building from the seed with the one clang line,
  because `heroes` without clang compiles nothing and a prebuilt binary alone
  would be a decoy; prepared and tested in private, published as the gate's
  outward act. `heroes --version` printed `heroes 0.0.1` that evening and the
  tags are milestone names, so a version scheme comes with it.
- **M-declared-freer, M-thread-stacks, M-discard-refusal and M-check-completeness**
  — **four rows entered together on 2026-09-04 by author instruction**, *"fix
  SCHEDULED too, so that every item is attached to a roadmap step that is still
  to be done; if you cannot find one, create it"*. The instruction is a rule about
  `docs/work/SCHEDULED.md` and these rows are what it cost: of its **29** items,
  measured that day, **twelve had no home a reader could reach** — nine named a
  waiting condition instead of a milestone (*"the next panel that touches
  emission"*, *"trigger, not a milestone"*), two named **M-corpus-depth**, which
  had closed that morning, and one named **M-ffi-ladder**, closed 2026-08-12. A
  list that schedules work at a closed milestone is a list that schedules nothing,
  and §14's rule that an id is never reopened is what turns the last one into a
  name of its own.
  - **M-declared-freer** carries panel 109's ratified `owned <C function>` mark
    and the two file splits that come before it. The name is **not**
    `M-foreign-ownership` on §14's rule that an id may make no claim a later
    milestone can falsify: the `ptr owned` half is refused under a standing veto
    with measured return conditions, so the ownership area has to stay free for
    the milestone that may deliver it. What this one delivers is narrower and
    exact — the freer is named in the declaration.
  - **M-thread-stacks** carries panel 107's adopted per-thread guard **and** the
    Windows `-Wl,/STACK:67108864` divergence that hid defect 008, because they are
    one subject seen from two sides: what happens when a stack runs out, and how
    much stack there is. Plural on purpose — 107 refused a uniform number on six
    measurements, so a name in the singular would assert what the sitting refused.
    It sits behind M-isolated-threads because 107 named that milestone as the one
    that inherits the bounds.
  - **M-discard-refusal** had an alias row since 2026-09-03 and no chain row, which
    its own `SCHEDULED.md` item said in its first line. The author places it: here,
    with the language's other rulings, because the verdict is given (`/decide`
    answer `3a`) and what is left is a sitting on the spec sentence and the
    diagnostic class.
  - **M-check-completeness** is the only one of the four with **no warrant**, and
    the row says so. Principle 0 holds it — panel 082 R3 ruled the direction and
    measured that nothing on the closure list needs it — so what schedules it is
    that its two items had been parked on a `grep` since 2026-08-16 with no row to
    read them. The phrase is the record's own: *check accepts ⇒ build succeeds*,
    false today wherever a generic stands between the rule and the type.
- **M-online-compiler** — **row 56, scheduled by author instruction 2026-09-06**,
  placed immediately before the gate for M-install-channels' reason and not for
  its subject: everything here is built and tested in private, and the outward
  act is the gate's. **It reverses half of a recorded refusal, and the row says
  which half.** `DESIGN-LOG.md:539` refused *a web playground* on 2026-09-03 —
  one of five candidates `docs/work/DONE.md:2415` records as refused *so the
  candidate is not proposed again as new* — with the parenthesis *Part 2 and Part
  9: wasm breaks the FFI premise*. Both cited passages, `design.md:590` and
  `design.md:2869`, are about a **Heroes program** targeting the web, and the
  compiler is a different program: it reads text and writes diagnostics and C,
  and needs no C library on the visitor's behalf. The half that stands is the
  visitor's program, and it is the milestone's whole subject — measured the day
  the row entered, **20 of 56** programs under `examples/` declare an `extern`,
  so what a stranger may name is a decision before it is an engine.
- **M-generated-programs** — **row 49, scheduled by author instruction
  2026-09-06**, placed after M-panic-location by the author out of four options
  put with a recommendation: *"Consider adding a step to the roadmap, towards the
  end, called something like consolidation — you say when it makes sense, maybe
  after the packages. It should create as many valid Heroes programs as possible
  that nonetheless make the compiler crash: hunt for bugs, looking too at the bugs
  that were found in other similar languages and compilers. The difficult
  conditions are the interfacing with C and very deeply nested structures. Purely
  and simply with fuzzing."* Rows 49–57 each move down one, which is this
  section's own shape: the numbers in the bullets above are the ones those rows
  had when they were written. **The gap it fills was measured before the row was
  written, and it is the direction of every instrument here.** `heroes mutate` is
  the inverse one — its own module doc says it takes the corpus programs that
  check clean and makes *one plausible mistake per site*, counting how many the
  compiler catches — so everything this repository points at the compiler is
  pointed at **wrong** programs, and nothing has ever pointed a machine at it with
  right ones. Of the **fourteen** defect entries in `docs/work/DONE.md` (the
  record numbers them 001–015 and uses 014 twice, which is a fault of the record
  rather than of the count), every *found by* field names a person: writing a
  program (006, 007), a panel seat (004, 010, 013, both 014s, 015), the baseline
  net run before touching anything (002), a `fmt` sweep (003, 005), the Windows
  box (008), a program's first run (009), the post-M8a sweep (001). **Four of them
  are at the C boundary and one is depth**, which is why the author named those
  two conditions and not others: 010, 013 and both 014s are FFI, and 007 —
  `heroes check` at exit **139** with nothing on either stream — was a valid
  program nesting deeper than the compiler's own recursive descent. `selfhost/`
  holds **63** points where the compiler declares a case impossible
  (`hero_unreachable`/`unreachable()`, `emit/structural.hero` alone 12), and
  defect 006 is one of them reached by a program `heroes check` had just accepted.
  **The author took the widest option on all four questions** — the name, the
  placing, five oracles rather than crashes alone, and the harness rather than a
  new verb — so §10's stopping rule is not touched and the tool convenes no
  sitting; what may convene one is a repair that reaches the language.

- **M-typed-inspection** — **row 49, scheduled by author instruction 2026-09-06**:
  *"I would like to add a step that implements a debugger for the Heroes language,
  a first thinking over all the possibilities, and then tell me as well where you
  want to put it in the roadmap."* Rows 49–58 each move down one; the numbers in
  the bullets above are the ones those rows had when they were written. Three
  choices were put with a recommendation and the author took all three: the id,
  the row, and opening the `assert` defect below in `docs/work/DEFECTS.md` rather
  than carrying it only as a witness into another sitting. **What a stopped
  program shows was measured before the row was written, and the surprise is that
  half of it already works.** On this Mac, 2026-09-06: a breakpoint set on a
  `.hero` line is hit and the source line is printed, `bt` names Heroes frames at
  `.hero:line`, and a local carries the author's own spelling behind an index
  (`h3_base = 7`). What is broken is four things. `p p` is
  `error: use of undeclared identifier 'p'`, because the C name is `h0_p` and
  lldb's expression parser is C++. A `[T]` and a `{K: V}` are an opaque
  `HeroArrayHeader *` and nothing of the contents. A `T?` prints **both** arms,
  including a garbage `err` half, under a hashed type name. And the frame is
  flooded, because §7 hoists every local to the prologue: **139** in one blessed
  emission (`tests/emission/run-adversarial-aggregate-overwrite.c`, 111 named and
  28 temporaries) against the **247** in `syn/expr.hero::compared` that
  M-qbe-backend's own item already carries. **Two findings placed the row rather
  than the symptom list.** The promise has no instrument: the golden this file
  cites at `:452` is `archive/bootstrap-rs/heroes-cli/tests/golden.rs:1012`,
  nothing has built that tree since M-bootstrap-archive on 2026-08-19, and CI
  installs lldb on the Linux leg for a test it never runs. And design.md's own
  consolation is false — Part 2 (`design.md:594`) refuses typed inspection on the
  ground that *"printing rich values is still `print`'s job"*, while `print(p)` on
  a record is a **compile error** and `assert` shows its two sides only for what
  `print` can print, so `assert [1, 2] == [1, 3]` prints the expression and no
  sides at all, silently, against `spec:202`. **The cheap route was run and not
  argued**: thirty lines of lldb Python read `len` from the header, resolved the
  `elem` descriptor pointer to the symbol `hero_desc_str`, found the C type
  `HeroStr` and printed `"ada"` and `"grace"`, with no compiler change and no
  runtime change — and `nm` shows a user type links as `_h_desc_Room_desc`, so the
  descriptor's own symbol is the type name the runtime does not carry. **Placed
  after M-panic-location** because they are two halves of one sentence and this is
  the expensive half: a program that stops says *where*, then says *what it was
  holding*. **Placed before M-generated-programs** on that row's own argument, one
  order up — its programs are the only ones here that nobody wrote, so reading the
  source helps least exactly there. **Placed before M-vscode-extension**, whose
  Debugging bullet states as a ceiling the thing this row removes, and that bullet
  is amended in the same commit rather than left to expire in silence. **Placed
  after M-reflection-verdict**, because a `to_str` derived over a record's fields
  is that sitting's own question and this row must not take it. **No warrant**: the
  §1.1 argument is available and is not claimed, because the only instrument that
  could measure it is Part 11's metric 4, which has never run. **The row was
  re-checked on 2026-09-07 by author instruction** (*"re-evaluate now, after other
  people's commits"*) after ten commits from a peer session closed
  M-declared-freer, which is the rule about reading a scheduling fact immediately
  before writing it doing its job: rows 48 and 49 had not moved, one staleness this
  commit was going to repair had already been repaired by that session, and two of
  the numbers written above were re-run rather than carried.

---

## The milestones, one by one

One section per milestone that still has something to say, **in the order of the
table above** — so the **eight closed** ones come first and the **nineteen** open or
scheduled ones after (CLAUDE.md §14: the table carries the numbers, the
sections carry the order, and neither repeats the other).

**Rows 1–21 have no section here**, and that is the rule rather than an omission:
a closed milestone's record is its journal, indexed at `docs/journal/README.md`.
The eight closed ones that do appear say **(closed …)** in their heading and are
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
`crates/` (archived 2026-08-19) at run time, so the self-hosted compiler was already broken outside
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

`crates/` → `archive/bootstrap-rs/` on 2026-08-19, and the move was the last commit rather than
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
Nim keeps a `manyloc` corpus under its own `tests/`, Zig a `standalone` one under `test/`, and Rust `rustc-perf`'s pinned
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

### M-c-callbacks — a Heroes function reaches a C callback parameter

**CLOSED 2026-09-05**, the day it opened, tag `m-c-callbacks`, six steps. The
milestone's own record is [033](journal/033-c-callbacks.md); what stays here is
the reasoning a later milestone has to honour, and it is four things.

**The split is settled and is not re-argued.** The permission is bought entirely
by §1.11 and CLAUDE.md §12's FFI-completeness instruction and needs no thread at
all — three seats of `docs/panel/111` asked for the split independently, and
bundled into M-isolated-threads it would have let the weaker half ride the
stronger's Principle 0 ticket.

**Where a callback may stand is a question about position, and never about
vocabulary.** C spells a function pointer everywhere it spells a type, so *can a
header declare this* cannot be the rule. A function value crosses as a
**parameter** and in no other position; one argument refuses all three of the
remaining shapes — an `extern`'s result, an `@` out-parameter and an `extern
constant` — and it is the permission's own mirror: an address passed **out** is
one this compiler emitted, so C calls a body this compiler type-checked; an
address handed **back** is a body nothing here has seen. `spec:224-225` states
it.

**The `const` hole is declared rather than open**, and its shape is measured
rather than assumed. `docs/panel/112` refused a `const` spelling on four
independent grounds and wrote design.md **Part 8 wart 19** at zero spec tokens,
in the order the counting found: over the real `sqlite3.h`'s 106 callback
signatures, a pointer to a struct the header declares is **66.7%** of what cannot
be spelled and a const pointee is **2.6%**. Anyone reopening it starts from R6:
`quals` on the existing `function_ty` case is the **only** admissible route — 2
edits in 1 file, no new `Ty` variant, so panel 083's veto does not fire — and a
new `Ty` case is 173 `non_exhaustive` errors across 49 files.

**R8's condition was met by the guard and NOT by atomics, so M-isolated-threads
still owes them.** Panel 111 R8 said the refcount work lands with the permission
or before it, on the ground that the FFI refusal was by accident the only thing
between a Heroes program and `hero_str_incref`'s race. What landed instead is
R9's guard, emitted into the callback: a foreign thread stops by name at the
entry of every function whose address the program takes, so the corruption is
**unreachable rather than repaired**. `_Atomic` still appears nowhere in
`runtime/`, and `cow.c`'s `if (a->refcount == 1)` is a test-and-mutate that an
atomic would not fix in any case.

### M-isolated-threads — the isolation, or nothing

**OPEN 2026-09-03, re-scoped by `docs/panel/111` on 2026-09-05.** design.md Part
7.13 — isolated per-thread heaps, copying at the boundaries, OS threads, **no
scheduler**. Its width is settled and unopposed: **data parallelism only**, which
is the design's own words at `:2599` (*"the first and probably only rung Heroes
needs"*); the mailbox stays deferred.

**Panel 030 R6's rider is answered, and the answer is yes.** The C11 backend can
express the model: a Heroes function emits exactly the C `pthread_create` wants
and runs on another thread at exit 0 (`docs/measurements/017`). Stack switching
and a CPS transform are what green threads and coroutines need, and Part 7.13
refuses both by name. cfront's fate does not reach this architecture, and the
deferral was never a bet.

**What the sitting found instead is that the deferred half was the deliverable.**
The proposal that opened this milestone — threads now, isolation later — was
**refused on two vetoes**, each with a running program: a shared `str` across 32
threads is `heap-use-after-free` or double-free in **9 of 10** ASan runs; `a == b`
on nested arrays is exit 139 with an empty stderr; copy-on-write double-frees; and
the stack guard is main-thread-only, so a worker overflow is exit 132 in silence.
Four classes, each from one line of ordinary Heroes. **So this milestone delivers
Part 7.13's isolation or it delivers nothing** — the route both compiling seats
accept, and the only one with a precedent: Erlang is the single surveyed language
that needed no type-system change, and per-process heaps are what it paid.

**The refcount half of design.md's own v1 invariant (`:2618-2624`) closed at step
3**, 2026-09-06, the way the document promised it would: one edit, because the
boundary was narrow and never inlined. It cost a measured **+2.0%** on the
compiler's own test suite, five alternating runs per arm from a cleared cache,
which is the sitting's own +1.7% reproduced on a different workload, and CLAUDE.md §12 is what lets it land — robustness outranks
speed, and this closes the class panel 111 measured at nine ASan runs in ten.

**The scratch the runtime keeps between calls closed at step 4**, the same day.
Ten objects are per-thread — `array.c`'s comparison scratch, `dir.c`'s listing,
and `run.c`'s argv buffer, which is on §1.0's closure list — and the buffer
neither counter weighs is given back by the thread that made it, through a key
whose destructor `parts/alloc.c` owns. `f64.c`'s cached C locale stops racing by
compare-and-exchange. **Cost +0.20%, inside the noise.** That is panel 111's
class 2 closed, and class 1 went at step 3.

**What is left is one line, and the sweep that says so is an instrument rather
than a paragraph.** `tests/harness/suite_runtime.hero`'s rule 3 takes the list
from the tree on every run and fails in both directions: **32** objects in
`runtime/` survive between calls — 15 `_Thread_local`, 3 `_Atomic`, **14 still
shared**, six of them inside function bodies. Ten of the fourteen are
M-thread-stacks' or are safe by where they are called, and every one carries its
reason in the file. **The one that matters is `cow.c`'s `if (refcount == 1)`**, a
test and then a mutate that no memory order can close — panel 111's class 3, the
last reason `parts/thread.c`'s guard cannot come down, and now its own item in
`docs/work/SCHEDULED.md`.

**And the instrument has already earned itself.** Rule 3 landed at step 3 and
caught its own author three times in the three hours after: ten allow-list lines
that stopped matching when their objects went per-thread, four newly shared
objects introduced by the repair itself, and a step-3 unit test pinning a fact
step 4 changed. None would have survived a re-reading, because the reasoning was
right each time and the list was short.

### M-thread-stacks — every thread's stack, and what happens when it runs out *(closed 2026-09-06)*

The story is [journal 035](journal/035-thread-stacks.md). What a later milestone
has to honour is here, and nothing else.

**Panel 115 refused three things permanently, each with the measurement that
produced it and a return condition that is its only amendment path.** One stack
size on all three platforms is refused: 8 MiB is glibc's default imported, and on
Windows it is an eightfold **cut** from the 64 MiB `selfhost/cli/flags.hero`
links for a measured reason. A Windows arm passing `dwStackSize` is refused while
`STACK_SIZE_PARAM_IS_A_RESERVATION` is absent, because without it the number sets
the **commit** and not the reserve — libuv ships that defect today, and
`runtime/parts/spawn.c` makes the same call. And a stack-size parameter on
`hero_thread_spawn` is refused at a price both compiling seats measured:
`HERO_RUNTIME_ABI` 21→22, 209 emission goldens, two sites in `seed/heroes.c` and
ten hand-written `extern` groups in `examples/`, for a form no program asked for.
It returns if a program is shown that must choose its own stack.

**What the floor is, so a later reader does not re-derive it.** A thread this
runtime starts is given at least the stack of the thread that ran `main`. It is a
fact about the machine in hand rather than a number, so nothing ages and nothing
reaches the spec. On glibc it is inert; on Windows it does not run. It raises one
platform and lowers none, and the delivered size is verified **after the fact**
because macOS and glibc refuse opposite things and neither refuses a terabyte.

**What a later thread milestone inherits.** `hero_spawn_stack_of_self()` has two
arms and no third, deliberately: a new platform is a compile error at that line
until somebody decides its answer, which is §11's loud-fallback rule rather than
an oversight. And `runtime/parts/thread.c`'s isolation refusal — not this
milestone's guard — is what stops a Heroes callback on a thread a C library made;
narrowing `selfhost/emit/callback_guard.hero`'s set reopens a silent exit 132.

**Still open and re-homed to M-core-packages**: `cow.c`'s `if (refcount == 1)` is
a test and then a mutate, and two sittings have now failed to race it.


### M-declared-freer — the string C hands you, freed by name *(closed 2026-09-07)*

**What happened is [journal 036](journal/036-declared-freer.md).** Panels 109
and 116, both ratified; five steps; `owned <C function>` in both positions, with
the release built in the lowering. What stays here is only what a later
milestone is bound by.

**`ptr owned` as a counted value is REFUSED under a standing veto**, and the
veto did not lapse with this milestone. Its return conditions are measured and
written in `docs/panel/109`; the program they point at is
`examples/ledger/main.hero`'s five leaking error paths. The area is deliberately
still free: this id names the deliverable and not the topic (CLAUDE.md §14), so
a milestone that delivers the `ptr` half takes a new id.

**The third case — C keeping a lent buffer — is queued and not refused.** It
needs a lifetime across two calls that a language without references cannot
state, and the C-side spelling works today (`constant SQLITE_TRANSIENT: ptr`).
A sitting of its own, when a program needs it.

**Two facts a later sitting on the FFI must not re-derive.** A freer is verified
by the probe, and **a probe exists per `extern`** — so a probe can never check a
freer no `extern` declares, nor an arity the release contradicts, and those two
refusals live in the checker (`selfhost/check/freer.hero`) rather than in the
emitter. And **the probe's spelling is the question it asks**: a marked cell is
`char **` in the probe and `(char **)&cell` at the call site, and the two must
agree or clang is being asked two different things.

**The freer's own module is the boundary.** The release is a C call emitted in
the marked declaration's unit, and that unit includes only its own groups'
headers, so a freer declared in another module cannot be called. Anything that
later widens where an `extern` is reachable from touches this.

### M-discard-refusal — `_ =` stops swallowing a failure

**Scheduled 2026-09-03 by author decision** (`/decide` answer `3a`), out of
measurement 014's survivor listing; **the chain row is 2026-09-04's**, and its
absence was the first thing the item itself asked for.

**What is measured and not in dispute.** `_ = risky(0 - 1)` compiles and the
program exits **0** having swallowed the error, while `x = risky(3)` followed by
`x + 1` is `error[bad_operand] … found i64?`. The corpus carries **nine**
witnesses, all one shape — `_ = expect(line, 2)?` with the `?` gone — which is
every `drop-question` survivor the mutation score found.

**The price was measured before the milestone was filed**, by stripping `_ = `
from every discard in a copy of the tree and reading what stopped compiling:
`selfhost/` has **237** discard lines of which **54** throw away a fallible value,
`tests/harness/` 14 and **6**, `examples/` 18 and **1**. **61 lines stop
compiling on the day this lands, the compiler's own source included**, so the
repair is part of the step and not a follow-up — and the two sampled shapes show
it is not uniform: one is a deliberate best-effort discard that wants an escape
hatch, the other is a hole that lets a test pass for the wrong reason.

**The verdict is the author's and is given; the sitting is still owed**, because
both halves are language (CLAUDE.md §4): a clause at `spec:97-98`, and a
diagnostic class whose `certain` fix is `_ = f()?` only where the enclosing
function is itself fallible and a *guess* otherwise. The wall it shares with
M-check-completeness is named there: `_ = f()` inside `function drop<A>(x: A)` is
a discard of a type parameter that may or may not be fallible at the call.

### M-closures-verdict — the ruling on closures and inline blocks

**Scheduled by author instruction 2026-09-03** (§ Who scheduled what), and what it
delivers is a **decision**, not a feature — the same shape as
M-interpolation-verdict, and for the same reason: the milestone must be able to
close with a refusal.

**What it rules on.** design.md Part 7 item 1, *"Closures — v1.5, immediately
after the first running program"* (`design.md:2441-2443`), and item 12, inline
blocks, *"likely lands together with closures"* (`:2503-2507`). The first running
program was M-scalars-run, 2026-08-04; the fixpoint that makes Part 7 admissible
at all was 2026-08-18. Neither item is on the closure list — the compiler
self-hosts with named functions — so the whole warrant is Principle 0's second
branch: a measured Part 11 effect, or a §1 argument the panel accepts.

**What the seats are handed, each measured on the day.** Part 8 wart 1
(`design.md:2608-2613`): the one-line helpers that exist to be passed around,
counted with a grep over `selfhost/` and `examples/` at the opening — that number
was not taken the night this was scheduled and is not guessed here. The
condition panel 013's ffi-pragmatist left
(`docs/panel/013-function-type-marker.md:185-189`, which is where the sitting
itself wrote it; the watch list that also carried it was retired 2026-09-04):
a capturing closure is a record plus a
pointer, so the type system must distinguish capture-free at the C boundary —
against `selfhost/emit/ctype.hero:375-380`, which today emits a function value as
a bare C function pointer *"because v1 has no closures"*, and that is what makes
`qsort` and every raylib callback expressible. The M-web-framework entry below,
which plans middleware *"as a chain of functions, because v1 has no closures"*.
And Part 7's own price: capture by copy, which value semantics makes *"a record
plus a function pointer"*.

**Three questions, in the order they bite.** Whether a closure may capture at all,
or only by copy (Part 7 fixes copy); whether a capturing value may cross the FFI
(panel 013's condition says the type must know); and what the form deletes, since
§1.7's test is subtraction. **A refusal costs the same as a feature** (CLAUDE.md
§12): if the answer is no, a Part 6 row with its falsifier, and item 12 follows
it.

**Why it stands here, before the packages.** The packages and the framework are
written in Heroes against the spec, and they are the largest body of Heroes this
project will write after the compiler. A form that lands after them is a form
they were written without. That is the argument M-interpolation-verdict's entry
made for the books, applied to the code — and it is why all four rulings sit at
rows 35–38. Full five seats: the form has surface, a spec cost and at least one
diagnostic class. Headroom the evening it was scheduled: **378** tokens.

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

**The cost of ordering it here is declared rather than discovered — and it moved.**
It stood at row 41, after the tools and before the two books, because a book is the
expensive consumer: a surface form that lands after M-guide-book rewrites chapters
in two languages, while one that lands after M-vscode-extension adds a rule to a
grammar file. On 2026-09-03 it moved to row 36, before M-core-packages, because the
packages and the framework are a larger consumer still — 51,788 lines of Heroes
in 178 modules is the size of the one body of that kind today, measured
2026-09-03, and a form that lands after them is a form they
were written without (§ Who scheduled what; `DESIGN-LOG.md:539`). The book's
argument holds and is now the second reason. CLAUDE.md §9
is the bill a new form arrives with — the formatter, every `--dump-<stage>` printer,
`heroes mutate`, the diagnostics that quote a program back, and `heroes measure`
where the form has spec text — and it was written the day `as` reached six consumers
one at a time, with the formatter silently deleting it from a working program.

### M-reflection-verdict — the ruling on reflection

**Scheduled by author instruction 2026-09-03**, the author's own example of a
feature never considered (*"reflection, say, just as an example"*), and a
**decision**, not a feature.

**The record has no row for it, and that is the finding.** Measured 2026-09-03
with `grep -rIn -i reflect` over design.md, DESIGN-LOG, the panels and the
reasoning notes: three mentions and no ruling. Panel 018 (`docs/panel/018:99`)
calls an attribute/reflection system *"new semantics with no compiler need
(Principle 0) adjacent to forsworn Part 6 territory"*; panel 039's C4 row
(`docs/panel/039-comptime-and-part-6.md` § Appended 2026-09-04) files
*reflection over types* under Part 6's **Ruby** row rather than its Macros row;
and the packages session noted that FastAPI's decorators and type-driven
validation *"need reflection Heroes has not got"* (`docs/work/DONE.md`, its
2026-09-04 entry — the reasoning note that held both was retired that day and
the C4 table moved into the sitting that had cited it). None is a Part 6 row with a
falsifier, so today the refusal is uncitable (CLAUDE.md §1) and a sitting could be
convened on it as an open question — the shape §1 warns against.

**Two questions, and they are not one.** *Run-time* reflection — a value that
carries its type and can be asked for its fields — is the Ruby row's territory
(`design.md:2395`: *"you cannot tell from the source what is callable … it
requires runtime dispatch, i.e. an interpreter"*), and the expected verdict is a
Part 6 row naming the program that would make it wrong. *Compile-time
derivation* — a `to_str`, a JSON rendering or a `hash` generated over a record's
fields, the way the emitter **already** generates `eq` and `hash` by walking
fields (CLAUDE.md §7) — is refused by nothing in the record, and it is where the
pressure is measured: `examples/json/` is **671** lines that render and parse a
`variant` by hand, and **20** functions named `render`, `show`, `shown`,
`rendered` or `to_str_of` are hand-written across `examples/`
(`grep -rhoE '^function (to_str|render|to_json|from_json|show|format)_?[a-z]*' examples/`,
2026-09-03). The comptime paragraph under Part 6 (`design.md:2406-2428`) is the
nearest ruling, and its three return conditions bind here too.

**Why here.** M-core-packages step 2 is `encoding/json`, and Go's is built on
reflection. Whatever the ruling, that package is written after it. Full five
seats; a refusal lands as a Part 6 row with its falsifier (CLAUDE.md §12).

### M-deferral-ledger — every Part 7 item gets a date

**Scheduled by author instruction 2026-09-03.** Part 7's preamble defers its
items *"until the Principle 0 closure list compiles itself"*; that was
2026-08-18, and on 2026-09-03 the items with no milestone and no verdict since
were **seven**: 5 `alias` (`design.md:2457`), 6 doctests (`:2467`), 8 traits
(`:2469`), 9 variant constructors as values (`:2472`), 11 the `raw` module
(`:2502`, Part 9), 14 declaration visibility (`:2562`, costed at panel 033 and
left to *"a count"*), 16 symmetric variant syntax (`:2585`, *"v2"*). Two more sat
on the panel watch list with no home at all until it was retired on 2026-09-04,
and this milestone is the home they got: raw string literals (Part 8 wart 15,
`design.md:2673` — panel 008's own implementation found that `"C:\temp"` cannot be
made loud, because `\t` is legal) and printing without a
trailing newline (wart 16). Items 1, 7 and 12 have their own rows above; item
10's C-width vocabulary and conditional compilation are questions (v) and (vi)
of M-core-packages' opening sitting and stay there; item 13 is
M-isolated-threads; item 15 is M-qbe-backend.

**What it delivers is a ledger**: each item, in Part 7's order, receives one of
three verdicts with a date — it enters (its own milestone, since a form lands in
every tool that reads the language, CLAUDE.md §9), it is refused (a Part 6 row
with its falsifier, CLAUDE.md §12), or it is deferred **again, with a return
condition** stated as a falsifiable claim, the way item 10's row already does.
A promise without a date is the one shape this milestone exists to end.

**The steps are the sittings**, one per item, full five seats where the item has
surface and the soundness lane where it has none. Nothing lands in the compiler
here except what a sitting adopts; the ledger itself is Part 7's own text,
amended by each verdict's commit. Why before the packages: the same reason as
M-closures-verdict — `alias` (`Handler = alias (function(Request) -> Response)`),
`private` (which functions are a package's API) and symmetric variants (the one
silence Part 11's first-try measurement is told to expect) all change how a
package is written.

### M-check-completeness — what `heroes check` accepts, `heroes build` compiles

**Scheduled, no warrant**, and the row says so on purpose. Principle 0 holds this
one: panel 082 R3 ruled the direction in 2026-08-16 and three seats measured that
nothing on the closure list needs it. What put a row under it on 2026-09-04 is
that its work had been parked on a `grep` for nineteen days with no milestone a
reader could open.

**The promise that is false today.** `heroes check` accepts `first([P(x: 1)])` at
exit 0 and `heroes build` refuses it, and the same gap has three more faces, all
measured: a map built inside `function tally<K>(k: K)` and keyed at `f64` by the
call is `check` 0 and **runs**, where `m: {f64: i64}` written down is exit 1;
`spec:221` promises a compile error for comparing a `partial` group record *"for
it and for any value holding it"*, and through `function same<A>(x: A, y: A)` it
is `check` 0, `build` 0, **run 134**; and a module that is nothing but an
`extern` group is `heroes check` **exit 0 with zero output**, because `check`
never runs clang — that fourth one is M-package-manager's, where a distributable
binding makes it load-bearing.

**Why none of them can be fixed in the body.** Panel 084 R1's shape worked
because `sort`'s domain is restricted, so refusing it on `[A]` deleted nothing. A
map keyed on `K` is legal at `str` and every integer, and `==` is legal on almost
everything, so a refusal inside the generic's body would delete working programs.
The concrete type arrives at **the call**, and that is the only line the author
can edit — which is also why the IR route provably cannot carry the diagnostic:
monomorphisation copies the template's span and discards the call's, while the
checker already keys instantiations **by the call-site span**.

**Refused, with its measurement** (082 R3): running `mono` inside `check` — 57
lines, the file past §11's ceiling, **1.3–2.2× on every keystroke**, a
`--permissive` flag contaminating Part 11's control arm, and a §4.16 hole turning
every `???` file into exit 1. Running clang inside `check` for the fourth face is
the same trade at a larger price.

**The trigger stays a `grep`, not a date**: `grep -rnE '^function [a-z_]+<'
selfhost/` returned zero generics whose body calls `sort(` when it was last
measured, and the day it returns one this is §1.0 compiler-need at any price.
**What the milestone does not lift** is the llm-ergonomist's veto: the generic
body's line stays undecidable from the line plus its signature, and lifting that
needs constraints on generics, which is the author's trade.

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
shape (the loopback program, as an `echo` program under `examples/`) · **11** `net/http` client,
against a `file://` URL · **12** `net/http` server, two requests on one
connection · **13** — **moved to
M-package-manager on 2026-09-03**, when the packages were placed ahead of the
manager: `heroes fetch` under the root, the root-level driver, and the `heroes
check` item panel 091 filed there. The packages live as directories under
`examples/`, reached by `use` from a program in the same root — the rule for this
milestone, not a stopgap until `fetch` exists.

**Acceptance**: a small HTTP server built from the packages and nothing else, in
`examples/`, in the corpus's three configurations, in the loopback shape — server
and client in one process, deterministic output — because a listening server does
not fit `main.expected`.

### M-package-manager — packages, and what stands in for a standard library

**Scheduled, no warrant.** Not a decision to take later: design.md:772 already
fixes the shape — *"No package manager exists before modules do; when it arrives
it will be `heroes add`/`heroes fetch` — inside the same binary"* (never a second
binary, CLAUDE.md §6 and §10). Its real prerequisite is
**M-separate-compilation**, not M-module-namespace: without separate compilation,
installing a package means recompiling the world on every build. **Since
2026-08-25 there is a second one in front of it** — M-package-layout, which rules
on how a fetched package's modules are named and reached; `heroes add` cannot
place files it has no spelling for. **And since 2026-09-03 the packages themselves
stand in front of it**: M-core-packages, row 39, whose step 13 — `heroes fetch`
under the root, the root-level driver and the `heroes check` item panel 091 filed
here — moved into this milestone, because one distributes what exists. **One
question its sitting owes, found the same night**: pinning what `fetch` brings —
Go's `go.sum`, Cargo's lock file — is a per-project file, and CLAUDE.md §10 admits
no fourth input class; the sitting says how a fetched package is fixed to a
version without one, or names which of panel 056's three return conditions it
meets.

**And this is where "a standard library that wraps C" goes.** §1.11 refuses a
standard library permanently, and that refusal is the founding constraint rather
than a shortage of effort — but what a standard library is *wanted* for arrives
here in a form the constraint permits: **distributable bindings**, ordinary
Heroes modules over real C headers, each with its link flag declared next to the
`extern` that needs it (§3.5). The difference is not cosmetic: a binding is
verified by clang against the header it names, and a standard library is verified
by whoever wrote it. **What this milestone does not deliver is the packages**:
those are M-core-packages, the row before it since 2026-09-03 — and the sentence just above,
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

### M-web-framework — the framework that composes the packages

**Scheduled, no warrant** (author instruction 2026-09-03, the same session). It
composes M-core-packages' packages in Go's and Echo's shape, everything explicit:
routes as a table of function values, `(function(Request) -> Response)` keyed by
method and path — top-level functions are values (`spec:112-117`); records for
request and response; middleware as a chain of functions, because v1 has no
closures (`selfhost/emit/ctype.hero:375-380`) — or in whatever shape
M-closures-verdict rules, which sits at row 35 so that this sentence is decided
before the framework is written; templates from `html/template`,
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

**The question the brief has to answer, in the falsifiable form the packages
session left it in** (2026-09-03, recorded here on 2026-09-04 because the note
that held it was retired and this is the only sentence of it that was not
already somewhere): **whether a framework with no closures reaches Echo's level
or collapses into `net/http` itself.** Middleware is planned *"as a chain of
functions, because v1 has no closures"*, and a chain of named top-level
functions that cannot capture may leave the framework with nothing to add over
the package it wraps — in which case the honest outcome is one package and no
framework. It is scored against the running application rather than argued, and
M-closures-verdict sits eight rows earlier precisely so its answer is known
first.

### M-doc-generator — `heroes doc`

**Scheduled, no warrant — and accepted against the recommendation** (author
instruction 2026-09-03; § Who scheduled what has the words). The record names
`heroes doc` exactly once, as the promise in Part 6's literate-source row
(`design.md:2400`: *"`heroes doc` generates the document instead — one direction
only"*): a comment directly above a declaration documents it (`spec:14-15`), and
this is the tool that would read those comments out.

**The first question of its opening is the stopping rule**, asked here so that no
sitting is convened on a silence. CLAUDE.md §10 admits a capability only if the
fixpoint invocation, the golden harness or the Part 11 harness must type it, or
it has a measured Part 11 effect. `heroes doc` meets none today, and the
precedent is exact: `docs/measurements/003` rider 3 refused `outline` and
`explain`, both scheduled by this file, on that rule — *"a rule that only ever
agrees with the plan is not a rule"*. Two routes admit it and both are named
rather than assumed: the site or the guide **consuming its output** to document
the packages' API, which makes the docs build type the verb the way the harness
types `heroes test`; or Part 6's promise read as the §1-derived argument
Principle 0 accepts. A place in the table is not a warrant (§ Decisions this file
records).

**Why after M-web-framework.** Its witness is the packages' own API: a generator
with nothing to generate proves nothing, and M-core-packages is what puts
documented declarations into a tree somebody else will read.

### M-panic-location — a panic names its line

**Scheduled by author instruction 2026-09-03**, the cheapest half of what the
author called *"a strong runtime"*, and its warrant is **§1.12**: a program that
stops must say where.

**Measured 2026-09-03.** `hero_panic` flushes stdout, prints `panic: <msg>` and
calls `abort()` (`runtime/parts/panic.c:21-25`); an out-of-range index, an
overflow, a `.must()` on an error and a slice that splits a character all funnel
through it, and none names a `.hero` file, a line or a function. The one abort
that does is the stack guard: `runtime/parts/stack.c:202-213` walks back with
`dladdr` to the first Heroes frame on POSIX, and `stack.c:292` says Windows names
the failure and not the function until dbghelp and a PDB are measured on the box.
The emitted C carries `#line` (CLAUDE.md §7), so `__FILE__` and `__LINE__` at
every runtime call that can abort already resolve to the `.hero` position — the
information is in the binary and the runtime is not told.

**What it delivers.** Every panic names the `.hero` file, the line and the Heroes
function, on the three platforms, measured on each before its commit (CLAUDE.md
§ Commands); `HERO_RUNTIME_ABI` +1 with the two-phase edit (`seed/README.md`); a
`fixedbugs` case per abort class (CLAUDE.md §9). **Soundness lane**: no surface,
no diagnostic class, no spec token. What it must not do is slow a program's happy
path or the compiler — the location is passed, never computed, and the corpus
leg's time before and after is the measurement.

**Why here, and why it may move.** It touches the runtime, which
M-isolated-threads holds until it closes; nothing else depends on it, so it may
be taken the day the threads land. Its witnesses are the corpus under
`--sanitize` and Part 11's metric 4, whose turns-to-green a panic that names its
line shortens.

### M-typed-inspection — a stopped program shows Heroes values

**Scheduled by author instruction 2026-09-06** — § Who scheduled what carries the
instruction, the three choices the author took and the measurements that placed the
row. **No warrant**, deliberately: the §1.1 argument is available and unclaimed,
because the only instrument that could measure it is Part 11's metric 4, which has
never run, and a place in the table is not a warrant.

**The id is the sentence it retires.** `design.md:594` says *"No typed variable
inspection in v1"* and this file's own M-vscode-extension bullet says *"Typed
inspection is not in v1"*. Both clauses are dated to a v1 that was reached at
M-selfhost-fixpoint on 2026-08-18.

**Half of it already works, and a later reader should not re-derive that.** Measured
on this Mac 2026-09-06, with lldb in batch mode over a hand-written program: a
breakpoint on a `.hero` line resolves and is hit, the source line is printed with a
caret, `bt` names Heroes frames at `.hero:line` (`h_dbg_total(...) at dbg.hero:19`),
a local carries the author's own spelling behind an index (`h3_base = 7`), a `str`
shows its text, and a record shows its fields (`h0_p = (f_x = 3, f_y = 4)`). `-g` is
on every build and the object survives beside the `.c` so Darwin's DWARF resolves
(`selfhost/cli/flags.hero`, `selfhost/cli/toolchain.hero`) — both repaired at
M-selfhost-port, which is why the line half exists at all.

**What it delivers is the other half, and it is four named failures.** `p p` is
`error: use of undeclared identifier 'p'` — the C name is `h0_p` and lldb's
expression parser is C++, so the author must know the mangling to ask a question. A
`[T]` and a `{K: V}` are an opaque `HeroArrayHeader *` and nothing of the contents. A
`T?` prints **both** arms, including a garbage `err` half, under a hashed type name
(`h_0opt_e201354`). And the frame is flooded, because CLAUDE.md §7 hoists every local
to the prologue and `frame variable` has no name filter: **139** in one blessed
emission, 111 named and 28 temporaries, against the **247** in
`syn/expr.hero::compared` that M-qbe-backend's item already carries.

**The route was run rather than argued, and that is what makes this row cheap.**
Thirty lines of lldb Python read `len` out of the array header, resolved the `elem`
descriptor pointer to the symbol `hero_desc_str`, found the C type `HeroStr` and
printed `"ada"` and `"grace"` — no compiler change, no runtime change, memory reads
only. `nm` shows a user type links as `_h_desc_Room_desc`, so **the descriptor's own
symbol name is the type name the runtime does not carry**, which is the same
pointer-identity trick `runtime/parts/sort.c` already uses. A `name` field on
`HeroDesc` is therefore refused before anybody proposes it: it would cost
`HERO_RUNTIME_ABI` a bump to buy a string the linker is already holding.

**The first step is the instrument, because the claim has none.** `design.md:616` and
this file at `:452` both assert that a golden runs lldb in batch mode and asserts a
breakpoint on a `.hero` line is hit. That golden is
`archive/bootstrap-rs/heroes-cli/tests/golden.rs`, nothing has built that tree since
M-bootstrap-archive on 2026-08-19, `tests/harness/` has no lldb suite, and
`.github/workflows/ci.yml` installs lldb on the Linux leg for a test it never runs.
The suite comes back wider than it went away: the three guards the archived test
bought with failures (lldb wrote nothing on either stream · the breakpoint is pending
with no locations · the file, the line and `stop reason`), plus the **stepping** half
that never had a test at all. Its own falsifier is run once by hand and quoted —
`-g` deleted, the suite must go red — because a debugger suite that passes without
DWARF is a decoration. It also settles `docs/panel/085`'s B2 condition, which said in
its own words that the lldb class *"was not tried"*.

**The second step measures four premises before anything is designed on them**, and
one of them decides the shape of the last: of those 139 and 247 locals, how many are
`t<N>` temporaries, how many are `$`-synthetic slots the lowering invented, and how
many are bindings the author wrote. The IR already knows — `SlotKind` carries
`param_slot`/`local_slot`/`synthetic_slot` and `--dump-ir` prints the `$` — and the
distinction dies in the mangler, which drops the leading `$`. If the census says the
named locals are mostly synthetic, the frame filter is free; if it says they are real
bindings spread over a long function, a slot table beside the binary is what buys
scope, and that is a panel question rather than a decision taken here.

**Not in this milestone**: making `print(p)` work. A `to_str` derived over a record's
fields is M-reflection-verdict's own question in its own words, and taking it here
would be a decision made at a sitting convened about something else. What this
milestone does is make that sitting's exhibit: after it, the author can **see** a
`[Token]` in a stopped frame and still cannot `print` one. Also not here: calling a
generated renderer inside the stopped process. It is the obvious optimisation and it
is unrun and unsafe on its face — the moment a debugger earns its keep is the moment
the program has crashed, and allocating on that process's heap, on whatever thread
lldb picks, after M-isolated-threads gave every thread its own, is a debugger that
mutates what it came to look at.

**What it honestly does not deliver is Windows.** `lldb.exe` exits `0xC0000135`
before running a command, which `.github/workflows/ci.yml` records and CI reports
rather than gates. The `#line` mapping itself is covered on all three legs by the
suite that reads emitted C and needs no debugger. What Windows gets here is a
`heroes doctor` row that says the tool is missing, because a named absence beats a
silence, and the way back in is named as a question rather than a plan:
`llvm-pdbutil` reads the CodeView in the 7.7 MB `.pdb` that `-g` already writes and,
unlike `lldb.exe`, has no reason to link Python.

**Why here.** After M-panic-location because they are two halves of one sentence and
this is the expensive half: a program that stops says *where*, then says *what it was
holding*, and most stops never reach a debugger once the panic names its function.
Before M-generated-programs for that row's own reason one order up — its programs are
the only ones in this project that nobody wrote, so reading the source, the ordinary
triage instrument, helps least exactly there. Before M-lsp-server and
M-vscode-extension so that the extension's variables pane is right on the day it
ships. And independent of M-qbe-backend: the flood is filtered debugger-side over a
convention the mangler already enforces, so this row does not wait on one that sits
behind the books.

### M-generated-programs — programs nobody wrote

**Scheduled by author instruction 2026-09-06** — § Who scheduled what carries the
instruction, the four choices the author made and the measurements that placed the
row. The warrant is **§1.12**: a Heroes program must not segfault, and the
compiler is a Heroes program. Defect 007 is that sentence failing on a **valid**
file, `heroes check` at exit **139** with nothing on either stream.

**What it delivers.** A generator, written in Heroes, that composes programs
**valid by construction** and **knows what each one must print before the compiler
is asked**; five oracles over every program it writes; a reducer that takes a
failure down to something a person can read; the defects that come out, repaired
at the class rather than at the witness; and a committed corpus of the reduced
witnesses in the net, which is what keeps them shut.

**Its first step is a catalogue, and it is enumerated from the world** (CLAUDE.md
§1, which says a list names where it came from). Csmith and YARPGen state in their
own papers which classes of bug they found — YARPGen's count is **more than 220**
in GCC, LLVM and the Intel compiler — a survey of compiler fuzzing exists, Zig
carries an issue titled *Compiler crashes found with fuzzing*, Go keeps
`test/fixedbugs` and Nim its `tests/`. The nearest corpus of all is this
repository: **fourteen** defect entries in `docs/work/DONE.md` and **76**
regression cases named after one — 49 with the `fixedbugs-` prefix under `check/`,
`run/` and `unsupported/`, 27 in `tests/golden/fixedbugs/`. The catalogue is a
`docs/measurements/` file naming each shape with its source and marking what was
read and what was not.

**The two moves that make it more than a test of the lexer**, both borrowed, both
named here so the milestone does not re-derive them. Generation goes **from the
type, never from the text**: it starts at *an expression of type `i64` is needed*
and descends, choosing at each node among the forms that type admits, so the
program type-checks by construction and a refusal from `heroes check` is itself a
defect. And **the answer is computed while the program is built**: every node
carries its value, so the generator writes the program and its `main.expected`
together, with no second compiler standing in as judge. That is Csmith's checksum
trick, and it is the only thing that makes the silent class — exit 0, wrong number
— visible at all.

**Heroes has no undefined behaviour, and that changes the generator's job.** An
overflow, a division by zero and an index out of range all abort by design, so
there is nothing to steer around the way YARPGen must for C; there is a choice to
declare instead. The clean arm picks values that make the operation safe by
construction. A smaller, declared arm expects the **abort and its message**, which
is how the guard rails get checked rather than assumed.

**The five oracles**, all of them, by author decision 2026-09-06 against the two
narrower options offered: (1) the compiler does not fall over — no 139, no 134, no
`internal error`, no silent exit, and every `hero_unreachable` reached is a defect,
of which `selfhost/` holds **63**; (2) the answer is the computed one, in the three
configurations the net already runs — `-O0`, `-O2`, `--sanitize`
(`tests/harness/suite_corpus.hero::configurations()`) — whose disagreement is the
only differential arm there is while there is one backend; (3) `heroes fmt`
re-prints it byte for byte, the class that produced defects 003 and 004, one of
which rewrote a compiler source into a different program at exit 0; (4) what
`heroes check` accepts, `heroes build` compiles, which is M-check-completeness's
promise put under a volume nobody writes by hand; (5) no leak, with the Linux leg
under `--sanitize` for whatever declares an `extern` (CLAUDE.md § Commands, where
LeakSanitizer is the reason that leg exists).

**The two hard shapes are the author's own, and the record agrees with them.**
The C boundary — generated `extern` groups against a header generated with them:
structs by value, `@` out-parameters, callbacks, `cstr`, `owned` — is where four
of the fourteen entries are (010, 013 and both 014s). Depth — records inside
variants inside maps, generics
instantiated across `use` lines, expressions hundreds of levels deep — is
defect 007.

**Where it lives, and what it must never become.** A Heroes program under
`tests/harness/`, run by `heroes run` as the net is, so §10's stopping rule is
untouched and no verb is proposed. The long hunt stays **outside** the net: a
suite that generates at random goes red at random, and this project has already
paid for an instrument nobody trusts. What enters the net is the committed,
reduced corpus, deterministic under a seed. The reducer starts from
`selfhost/mutate/sites.hero`, split out as *the primitives every mutation operator
is built out of: a text edit, a span, and the four questions about a tree node*.
A seed is an integer, and `examples/montecarlo/main.hero` already carries a
deterministic stream with a test saying why — so a defect is reported by its
number and anybody can reproduce it.

**Why here.** After the four verdict milestones, because a generator has to know
the final surface; after M-check-completeness, whose promise is oracle 4; after
M-panic-location, because a crash that names its file, its line and its function
is the difference between a triage of minutes and one of hours. Before the books,
the channels and the gate, so that what gets written about and shipped is a
compiler that has been shot at. **The sixth oracle is dated rather than
promised**: when M-qbe-backend exists, the same generated program through two
backends is differential testing in the full sense, and that is the one thing this
milestone deliberately leaves to a later one.

### M-thesis-harness — the thesis, measured

**Scheduled by author instruction 2026-09-03**, and it is the second milestone
after the fixpoint with a warrant: **§1.1**, comprehension measured rather than
asserted, and design.md Part 11, whose metrics 2 and 4 have never run
(`docs/measurements/007:24-25`; M-publication-gate's checklist says so in its own
words). Everything this project claims about first-try rates rests today on
metric 3 alone.

**What it delivers is the instrument**, written in Heroes and run by the one
command: Part 11's protocol — spec-only context, single turn, frozen and hashed
prompt templates, two gradings (compiles · tests pass), Wilson intervals, one
non-Anthropic model as robustness, provenance on every run (spec sha, compiler
sha, model id, prompt sha, suite sha) — and metric 4's turns-to-green over broken
programs, capped at 5. It reaches the model over HTTPS, so it depends on
M-core-packages' `net/http` client (libcurl, step 11) and reads its key from the
environment, one of §10's three input classes.

**The tasks, and a decision of the same night.** Metric 2's held-out tasks must
be author-written or they measure the assistant's priors (panel 011; author
decision 2026-08-24, the M-guide-book item in `SCHEDULED.md`). That decision
stands: the bulk of the set is written at M-guide-book, where the author's own
work *is* writing Heroes. What this milestone adds, by author decision
2026-09-03, is a **small seed of tasks written by the author at its opening**, so
that the instrument runs once on real input before the book grows the set, and
so that the number exists before the books state it.

**Why here.** After the packages that give it a client; before the tools, whose
value it does not need; before the books and the gate, which will print the
number. Corpus material is labelled and never enters the held-out set
(CLAUDE.md §9).

### M-lsp-server — `heroes lsp`

**Scheduled, no warrant.** ~250 lines of JSON-RPC: diagnostics on save,
formatting, hover, documentSymbol. It blocks nothing and could land any time
after M-rich-diagnostics; it is here rather than earlier by the author's choice,
and M-vscode-extension is what consumes it.

**And the incremental frontend, since 2026-09-03.** The `SCHEDULED.md` item that
M-separate-compilation step 6 left open asked *which milestone does it*, and this
is the answer: a server that re-checks a program on every save cannot wait for
`heroes check` on the compiler's own source — about 8 s on 2026-08-26, after step
9 took it down from 88 — and the per-module build's warm 45.2 s against the fused
44.1 s says the emission half is architectural (panel 093 R4 puts the emitted
text in the cache key). The frontend half is the one an editor feels, so it lands
here; the emission half keeps its numbers in that item as its trigger.

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
  under the stopping rule like any other verb, with the reason recorded. **The
  ceiling stated here used to be design.md §2's, and M-typed-inspection is the row
  that removes it** (amended 2026-09-06, in the commit that scheduled that row):
  `p x` showing a mangled C temporary rather than a Heroes value was true of every
  build until then, and the variables pane shows whatever lldb's formatters show,
  so this milestone inherits the answer instead of documenting the ceiling. What is
  unchanged is the ruling: `lldb-dap` composes, this project writes no debug
  adapter, and an editor **consumes** formatters rather than producing them. The
  sentence is corrected rather than deleted, because a bullet that states a limit
  the compiler has already lifted funds the wrong decision at the next sitting.
- **Packaging**: a `.vsix` that installs, with `heroes doctor` as the extension's
  own health check.

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

### M-install-channels — the way in, from a package manager

**Scheduled, no warrant** (author instruction 2026-09-03, *"publishing on brew, for
instance … deploying the language"*). Installing Heroes today is a `git
clone` and one clang line (`site/src/html/index.html:135-137`, `seed/README.md`),
and that is the whole reason this milestone is small: **every channel builds from
the seed with that line**, 3.5 s, and no channel ships a prebuilt binary —
because `heroes` without clang compiles nothing (`heroes doctor` says so), so a
binary on its own would be a decoy, and a formula that depends on a C toolchain
is the honest shape.

**What it delivers.** A Homebrew formula in a tap (`heroes-lang/homebrew-tap`),
for macOS and Linux; a winget or scoop manifest for Windows, whichever the box
(`docs/environment/windows/WINDOWS-MACHINE.md`) measures first; a Nix flake; a
Docker image built from the `Dockerfile` beside
`docs/environment/linux/LINUX-MACHINE.md`. Each is installed and `heroes doctor`
run on its platform before its commit. **And a version scheme**: `heroes
--version` printed `heroes 0.0.1` on 2026-09-03 and every tag is a milestone's
name (CLAUDE.md §14), so nothing a formula can pin exists yet; what a version
number promises is M-publication-gate's compatibility paragraph, and the two are
written together.

**Where the line is.** A formula, a manifest, a flake and a Dockerfile are the
channels' own files, outside `heroes` and outside CLAUDE.md §10's *never a
script*: they invoke the one clang line, they do not replace it. Everything here
is prepared and tested **in private** — a local tap, `brew install
--build-from-source` — and the act that puts a channel where a stranger can reach
it is the gate's, which is why this row sits immediately before it (CLAUDE.md
§14: publishing is a hard stop).

### M-online-compiler — the compiler, reached without installing it

**Scheduled, no warrant** (author instruction 2026-09-06, *"maybe it makes sense
to add a step to the roadmap that says: let us make a compiler that runs inside a
browser, so whoever wants to try the language can try it quickly on the site,
compiling to wasm perhaps"*, and in the same minute the fallback, *"or if that
cannot be done, a remote compiler, but with safeguards so it does not become a
way to break into a machine or to burn resources forever"*). Installing Heroes is
a `git clone` and one clang line (M-install-channels); this row is for the
visitor who will not run even that.

**The refusal this row half-reverses, and the half of it that still stands.**
`DESIGN-LOG.md:539` refused *a web playground* on 2026-09-03, and
`docs/work/DONE.md:2415` records it among five candidates given a recorded
refusal *so the candidate is not proposed again as new*. The reason given was
*Part 2 and Part 9: wasm breaks the FFI premise*, and both cited passages —
`design.md:590` and `design.md:2869` — are about **a Heroes program targeting the
web**, as is `design.md:520`'s *native compilation rather than wasm* among the
decisions that are not revisited. All three rest on one fact: wasm cannot call
native C libraries, so under §1.11 a Heroes program compiled to wasm has nothing
to call. **The compiler is a different program** and the argument does not reach
it. Where it still bites is the second half — whatever runs the **visitor's**
program must reach the headers that program names.

**The number that makes this hard, measured 2026-09-06.** Of **56** programs
under `examples/`, **20** declare an `extern`: `hero_os.h` (10), `math.h` (3),
`stdio.h` (2), `sqlite3.h` (2), `time.h`, `raylib.h`, `curl/curl.h`,
`SDL3/SDL.h`, with two linking `raylib` and two `sdl3`. §1.11 is why — there is
no standard library, so the programs that show what the language is *for* are
exactly the ones that open a window, a socket or a database. **A playground that
refuses `extern` demonstrates a language that does not exist; one that allows it
on a public server is a remote shell**, and `extern "stdlib.h" { function
system(cmd: cstr) -> i32 }` is one line the compiler is right to accept. The
deliverable is therefore a **decision about what a stranger's program may name**,
and then whichever engine enforces it.

**The two engines, in the author's own order of preference.**

- **In the browser.** `seed/heroes.c` is **733,838** lines of C and would be
  compiled to wasm with `runtime/`; the visitor's program is then checked,
  formatted, dumped and emitted as C entirely on their own machine, at zero
  attack surface and zero running cost, inside a site that stays static. What it
  cannot do is **run**: `runtime/parts/run.c` reaches the toolchain through
  `execvp` and `CreateProcess`, and a browser has neither. Running there too
  needs clang itself hosted in wasm, and would still reach only the **36**
  programs that name no header.
- **Remotely.** The real compiler, the real clang and the real libraries, so all
  56 run, `sqlite` and `curl` included. The price is the sandbox the author
  named: one container per request off the image M-install-channels already
  builds, no network, a read-only tree, ceilings on wall clock, CPU, memory and
  output size, and the allow-list above. It also puts a service behind a site
  that is static today (Astro on Cloudflare Pages, `site/public/CNAME`).

**No language change is owed, whichever engine wins** (panel 114; panel 036 as
corrected by 114's spec-warden, which found the record had been reading a
**deferral** of `compile` as a veto). 114 ruled that platform variation lives in
the header a program ships, with the `#ifdef` inside — one `.hero` source, exit 0
on macOS and Windows, `--emit-c` carrying **zero** platform words. A browser is a
fourth platform under that ruling, so the mechanism for its arm exists already at
zero spec tokens and this milestone opens no sitting to obtain one.

**The stack is the browser route's first hard number, and it was measured for
another platform** (M-thread-stacks, on the author's Windows box, 2026-09-06 at
`bcf6c41a`). A recursive-descent compiler in wasm gets a fixed slice of linear
memory chosen once at link time, which is structurally the Windows main thread's
1 MB rather than a stack that grows. That box now builds the seed with **the
contract's plain line**, no `/STACK` flag, and takes `build selfhost/lexer.hero
--dump-ir` and `build selfhost/main.hero --emit-c` to exit 0 — the first being
the module that in September died at **exit 127 with both streams empty**. So the
compiler compiling itself fits under a megabyte, and the failure mode when it
does not is silence, which is the one a browser would also give. CI now
**asserts** the module case on the Windows leg rather than reporting it
(`.github/workflows/ci.yml`); the whole-compiler case was measured and is not
asserted.

**And the spawn question is a build error there rather than a silence.** Read
2026-09-06, correcting the note that carried the number above, which had it that
the new floor goes inert on such a target. `runtime/parts/spawn.c:142-148` gates
`hero_spawn_floor = 0` on **`_WIN32` alone** and not on the absence of an OS to
ask, so a third platform takes the `#else`, and `hero_spawn_stack_of_self()` at
`:120-134` splits again on `__APPLE__` — landing a wasm build in the glibc arm,
on `pthread_getattr_np`. **There is no third arm**, so such a build either
compiles to a real query or stops at that line; its author states the two-arm
shape is deliberate on §11's loud-fallback rule, `_ => hero_unreachable()`
beating `_ => false` at M5c, so a platform arriving there should stop the build
until somebody decides that platform's answer. The two readings are opposite
risks for this row: a silent skip is a defect found in a browser months later, a
build failure is an arm somebody writes on purpose. Whether a wasm libc provides
that function is untested here.

**What is not measured, written as a question rather than as a premise**
(CLAUDE.md §1). Apple clang 21.0.0 on the author's Mac has **no WebAssembly
target compiled in** — `clang --target=wasm32 -c` answers *No available targets
are compatible with triple "wasm32"*, measured 2026-09-06 — and neither `emcc`
nor `wasm-ld` nor a wasi-sdk is installed, so the browser route needs a **second
toolchain**, against M-install-channels' rule that every channel builds from the
seed with the one clang line. Whether the seed compiles under one is untested.
Whether a wasm-hosted clang is a real option is untested. What the languages
closest to this one actually ship is unverified and belongs to the historian, not
to the convener.

**Its opening convenes a panel** (CLAUDE.md §4 — this is the tool surface, and
what a stranger's program may name is language-facing). Three questions for it:
which engine; whether a wasm build of `heroes` is a second binary under §10 or
the same program for another target; and whether the allow-list is a property of
the playground or a `heroes` flag, which is §10's stopping rule asked about a
capability with no other caller.

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
- **The outward act of M-install-channels**: the tap, the manifest, the flake and
  the image go where a stranger can reach them here and not before, and each is
  installed once more from its public address.
- **The version scheme is in force** — a number `heroes --version` prints and a
  formula can pin, with the compatibility paragraph above saying what it
  promises; `heroes 0.0.1` and milestone-named tags are what stood on 2026-09-03.
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

**Both exist in Italian and English, one of CLAUDE.md §11's declared
exceptions** to "everything written is English" (§11 records it). Neither version
is a machine translation of the other; the Italian is the one the author studies
from, so where the two diverge, the Italian is fixed to be clearer rather than
the English to be more faithful. **This said *the one* declared exception until
2026-09-04**, when §11 was corrected and these two dependants were not: the
exceptions are a class — a translation that is itself a deliverable — and the
site's Italian edition, 23 pages measured that day, had been the second one for
seventeen days.

**Both teach with M-program-corpus's programs** — code known to compile, run and
pass its own tests in three configurations, rather than snippets that were true
once.

---

## The names

Milestones were numbered until 2026-08-12 and are named now. The algorithm that
assigns the next one is **CLAUDE.md §14** — its only home; this section is only
the map, and it exists because **the record was not rewritten**. `docs/panel/`,
`DESIGN-LOG.md`, `docs/journal/`, `docs/measurements/`, `docs/work/DONE.md`,
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
| `M-bootstrap-archive` | M8c, in part | `m-bootstrap-archive` | `crates/` → `archive/bootstrap-rs/` (2026-08-19). **The clause M8c was carrying and could not pay**: the port read its standard library from the directory being archived, so the compiler failed outside this repository, and `heroes measure` was still not in the port |
| `M-separate-compilation` | M9 | — | one `.c` per module, prototypes across TUs, the cache |
| `M-package-layout` | — | — | `use` paths, the qualifier, where a program's files live. **A new id rather than an area annexed** (§14, author decision 2026-08-25): M-separate-compilation delivers the build architecture and M-package-manager delivers `heroes add`, so the directories question — which had been scheduled inside the latter since 2026-08-12 — is a third deliverable and takes a name of its own |
| `M-corpus-coverage` | — | `m-corpus-coverage` | every language form has a program that runs it. **A new id rather than a reopening of `M-program-corpus`** (§14, author instruction 2026-09-02), which closed 2026-08-13 delivering *many whole programs, all of them run*; this one delivered *every language form has a program*. **Done 2026-09-02**: twenty programs, `examples/` 15 → 35, and the first program written to close the measured library gap found a compiler defect on its first run |
| `M-selfhost-nesting` | — | `m-selfhost-nesting` | the compiler's own modules move into directories by subsystem. **Done 2026-09-02**, and the row keeps its scheduling note because two of its numbers were wrong and the record should say so: it said **169** modules where the tree held **170**, and it said nesting puts 32 files into 13 last-part collision groups that `module_last_parts_collide` refuses — true of the program-wide rule, which panel 100 R3 replaced with a per-file one before this milestone ran. Under the rule that actually landed the number is **41 files needing 43 bindings disambiguated**, of which 10 modules took `as` and 3 locals were renamed. The block it named — panel 100's verdict on the alias — was real and was lifted. What no version of this row foresaw is panel 102: the prefixes had been holding module names out of the namespace where values live, so the collisions that mattered were **local-vs-module**, 0 flat and 23 nested |
| `M-package-manager` | M10 | — | `heroes add`/`heroes fetch`, bindings in place of a standard library |
| `M-isolated-threads` | M11 | — | Part 7.13: per-thread heaps, copying at the boundaries, no scheduler |
| `M-qbe-backend` | M12 | — | Part 7 item 15 — the row read *7.14* until 2026-09-03, and item 14 is declaration visibility today (`design.md:2562`, `:2579`): the proof that the IR is not C in disguise |
| `M-lsp-server` | M13 | — | `heroes lsp` |
| `M-vscode-extension` | M14 | — | the extension, complete |
| `M-documentation-site` | M15 | `m-documentation-site` | the site. **Done 2026-09-02, out of chain order by author instruction**: the module system had just landed and the site had no page about it, so a reader could learn how numbers, errors and the C boundary work and leave with no idea how to write a program in more than one file |
| `M-interpolation-verdict` | — | — | the ruling on string interpolation — design.md Part 7 item 7, evaluated for the first time since the fixpoint made Part 7 admissible at all. **The verdict is the deliverable and the name says so** (§14, author instruction 2026-09-02): the milestone has to be able to close with a refusal, so an id naming the feature would claim the very thing the sitting exists to decide, and a later milestone could falsify it |
| `M-journey-book` | M16 | — | the journey |
| `M-guide-book` | M17 | — | the guide |
| `M-argv-execution` | — | — | the compiler runs programs by argument list, and the shell stops being the boundary |
| `M-publication-gate` | M18 | — | the last gate before anything goes outward |
| `M-c-callbacks` | — | `m-c-callbacks` | a Heroes function reaches a C callback parameter. **Done 2026-09-05**, the day it opened: six steps, [033](journal/033-c-callbacks.md). **Proposed by `docs/panel/111` and placed at row 34 by the author the same day**, ahead of M-isolated-threads: *"can we put M-c-callbacks in right away? I would not wait too long to do it"*. `docs/work/SCHEDULED.md`'s rule puts the placing of a new id with the author, and this is that placing. The name reaches this table first, which is CLAUDE.md §14's order and what the `records/names` check reads — it fired on this very id minutes after the sitting closed. **A new id rather than a part of M-isolated-threads**: three seats found the permission is bought entirely by §1.11 and §12's FFI-completeness instruction and needs no thread at all, so bundled it let the weaker half ride the stronger's Principle 0 ticket. **Named for what it delivers and not for the area**: the callbacks, not the FFI, which must stay free for whatever binds `const void *` |
| `M-robustness-guards` | — | `m-robustness-guards` | the guards that shut the holes §1.12 named. **Done 2026-09-03**, the day it opened: six steps, two sittings (103, 104), every landing measured on the Mac, the Linux image and the Windows box before its commit |
| `M-corpus-depth` | — | — | the rung between a program and the compiler: nine programs chosen for shape — oracle-checked, deep, FFI at program scale. **A new id rather than a third reopening of the corpus** (§14, author instruction 2026-09-03): `M-program-corpus` delivered *many programs run*, `M-corpus-coverage` *every form has a program*, and this one delivers *size, depth and an external oracle*, which neither name claims |
| `M-core-packages` | — | — | small packages that compose, organised as Go's tree, in Heroes or over C. **A new id rather than an area annexed** (§14, author instruction 2026-09-03): M-package-layout delivered how a `use` reaches a module, M-package-manager delivers `heroes add`/`heroes fetch` and where a fetched package lives, and this one delivers the packages themselves — a deliverable neither name claims. The word is Odin's `core:` collection, which §1.11 cites for the reason a package can be redesigned and a built-in cannot |
| `M-web-framework` | — | — | the framework that composes the core packages, Go/Echo style. **A second id and not a step of the one above** (§14, author instruction 2026-09-03, *"then the web one that uses them all"*): the packages are a deliverable with or without a framework, and a framework is falsifiable on its own — it exists when the corpus's `todo` is served over HTTP on three platforms |
| `M-closures-verdict` | — | — | the ruling on Part 7 items 1 and 12, closures and inline blocks. **The verdict is the deliverable and the name says so** (§14, author instruction 2026-09-03), on M-interpolation-verdict's precedent: an id naming the feature would claim what the sitting exists to decide |
| `M-reflection-verdict` | — | — | the ruling on reflection, at run time and as compile-time derivation over a record's fields — a question the record had never given a row (§14, author instruction 2026-09-03) |
| `M-deferral-ledger` | — | — | every Part 7 item with no milestone receives a dated verdict or a return condition. **The ledger is the deliverable** — not a feature and not a refusal, but the end of promises without a date (§14, author instruction 2026-09-03) |
| `M-doc-generator` | — | — | `heroes doc`, the one direction Part 6's literate-source row promises. Scheduled against the recommendation, with CLAUDE.md §10's stopping rule as its opening question (§14, author instruction 2026-09-03) |
| `M-panic-location` | — | — | a panic names the `.hero` file, line and function — §1.12's guard on the one abort that said nothing about where (§14, author instruction 2026-09-03) |
| `M-thesis-harness` | — | — | Part 11's metrics 2 and 4, run for the first time by an instrument written in Heroes. **A new id rather than a step of M-guide-book or M-publication-gate** (§14): the instrument is a deliverable with or without the book's tasks, and the gate consumes its number rather than building it |
| `M-discard-refusal` | — | — | `_ =` on a fallible value becomes a compile error, so the one place a `?` can be forgotten without the type system noticing stops being silent. **A new id and not a step of a corpus milestone**: `M-corpus-depth`'s instrument is what *found* it — all nine `drop-question` survivors of measurement 014 are this one shape — but what it delivers is a language rule, which that milestone claims nothing about. Scheduled 2026-09-03 by author decision (`/decide` answer `3a`); the verdict is given and the sitting still owes the two halves that are language (a sentence at `spec:97-98`, a diagnostic class, CLAUDE.md §4). Priced before it was filed: **61 lines in this repository stop compiling**, 54 of them in `selfhost/` |
| `M-install-channels` | — | — | a Homebrew tap, winget, a Nix flake, a Docker image, all built from the seed, and a version scheme. **A new id rather than a step of M-publication-gate** (§14): the channels are built and tested in private before the gate, and the gate performs only their outward act |
| `M-declared-freer` | — | — | `owned <C function>`: an `extern` names the function that frees what C hands back, and the compiler calls it. **A new id and not a reopening of `M-ffi-ladder`** (§14), which closed 2026-08-12 delivering *Heroes calls C, SQLite with no shim*; panel 109 ratified this on 2026-09-04 and its item had been scheduled at the closed id. **Deliberately not `M-foreign-ownership`**: the `ptr owned` half is refused under a standing veto with measured return conditions, so the area stays free for the milestone that may deliver it, and an id must make no claim a later milestone can falsify |
| `M-thread-stacks` | — | — | the stack guard reads the calling thread's own bounds and installs its own alternate stack, so an overflow on a library's thread stops with a message instead of an empty exit 132 — and the size stops being a link flag that exists in one CI file and no document. **A new id rather than a reopening of `M-robustness-guards`** (§14), which closed 2026-09-03 delivering the guard on the **main** thread; panel 107 found the rest of it on 2026-09-04. **Plural because panel 107 refused a uniform number** on six measurements: the number cannot be the same on three platforms, the abort can |
| `M-check-completeness` | — | — | what `heroes check` accepts, `heroes build` compiles — through a generic too. **A new id and not a step of a checker milestone** (§14): `M-typed-frontend` and `M-checker-core` closed 2026-08-04 delivering the frontend itself, and what this delivers is a **promise about two commands agreeing**, which neither name claims. Scheduled 2026-09-04 by author instruction with **no warrant** — Principle 0 holds it, panel 082 R3 ruled the direction on 2026-08-16, and the trigger is a `grep` rather than a date |
| `M-online-compiler` | — | — | the compiler reached without installing anything: the site's visitor writes Heroes and gets its answer. **A new id and not a step of `M-documentation-site` or `M-install-channels`** (§14): the site closed 2026-09-02 delivering pages anchored to programs that run, and the channels deliver installation, while this one delivers the case where nothing is installed at all. **Deliberately neither `M-wasm-playground` nor `M-browser-compiler`**: the engine is this milestone's own opening question, so an id naming either would claim exactly what the sitting exists to decide, and `wasm` has to stay free for the later half of Part 7 item 15 (`design.md:2645`). Scheduled 2026-09-06 by author instruction with **no warrant** |
| `M-generated-programs` | — | — | programs nobody wrote, and the defects they find: a generator that composes valid Heroes by construction, knows what each program must print before the compiler is asked, and reduces every disagreement to a case. **A new id and not a fourth corpus** (§14, author instruction 2026-09-06): `M-program-corpus` delivered *many programs run*, `M-corpus-coverage` *every form has a program* and `M-corpus-depth` *size, depth and an external oracle* — all three are programs a person sat down and wrote, and this one delivers programs **nobody wrote**. It is also not a reopening of `M-robustness-guards`, which closed 2026-09-03 delivering the guards that shut four named holes: a guard is a hole you have found, and what this delivers is the machine that finds them. **Deliberately neither `M-adversarial-corpus` nor `M-compiler-survival`**: the first annexes an area three milestones already share, and the second states a promise a later milestone can falsify, which §14 forbids an id to do. Both runners-up are in `tests/harness/suite_records.hero`'s `REFUSED` |
| `M-typed-inspection` | — | — | a stopped Heroes program shows Heroes values: `p p` under the name the author wrote, and `[T]`, `{K: V}`, `T?`, a variant and a record shown as themselves rather than as an opaque `HeroArrayHeader *`, a raw tagged union with a garbage `err` arm, and a hashed type name. **The id is a phrase the record already uses, and both times as the non-goal** (§14 prefers one the record uses over an invented one): `design.md:594` says *"No typed variable inspection in v1"* and this file's M-vscode-extension bullet said *"Typed inspection is not in v1"* until the row existed. **A new id and not a reopening of `M-selfhost-port`**, which closed 2026-08-17 and carried `-g` in as a defect so lldb could break on a `.hero` line at all: breaking, stepping and a backtrace at `.hero:line` work today, and what this delivers is what the frame SHOWS once the program has stopped. **It is also not a step of `M-vscode-extension`**, whose Debugging bullet rules that `lldb-dap` composes: an editor consumes formatters and never produces them, and the author debugs a compiler written in Heroes from a terminal with no editor open. **Deliberately not `M-debugger`**: one word where §14 asks for two, so it appropriates a topic, and it would claim the line-level half a closed milestone already delivered while leaving no name for the `heroes dap` this file already reserves. **Deliberately not `M-stopped-values`**: `print` and `assert` render values too, and that half is compile-time derivation over a record's fields, which is `M-reflection-verdict`'s question and not this row's. **And deliberately not `M-frame-values`**: it names the compiler's word for the place rather than the author's for the thing, and the deliverable is the value seen, not the frame. All three runners-up are in `tests/harness/suite_records.hero`'s `REFUSED`. Scheduled 2026-09-06 by author instruction with **no warrant** |

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

**Three names are not the deliverable's obvious one, and the reason is on the
record.** `M-generics-library` covers four concerns (sugar, tests, generics,
library); the runner-up `M-language` was refused because four of §1.0's rows were
still open, so it overclaimed. `M-strings-ownership` names both halves rather
than just `str`, because the ownership pass is the architecturally load-bearing
one and the milestone's journal slug names both too. And `M-declared-freer`
(2026-09-04) refused the runner-up **`M-foreign-ownership`** for the same reason
as `M-language`: panel 109 adopted the mark for a `cstr` and **refused `ptr
owned` under a standing veto** with measured return conditions, so a name over
the whole ownership area would claim exactly what a later milestone may deliver
or refuse. Both runners-up are listed in `tests/harness/suite_records.hero`'s
`REFUSED`, which is how a name written down without being a milestone stays
legal — and how the reasoning survives the decision.
