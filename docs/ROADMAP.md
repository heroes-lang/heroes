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

---

## Where we are

| | |
|---|---|
| **Current milestone** | **M-separate-compilation** — open since 2026-08-19 |
| **State** | four repairs · steps 1–4 done · **step 5** lands acceptance row 1 |
| **v1** | **reached** at M-selfhost-fixpoint, 2026-08-18 — the compiler compiles itself |
| Milestones closed | 22 of 36 · 25 tags |
| The compiler | **38,364 lines** of Heroes in 157 files |
| The seed | **769,830** lines of generated C — the whole way in |
| The spec | **3592** tokens of a hard 4096 · headroom 504 |
| Runtime ABI | 15 |
| Panels held | **92**, every one ratified · journals 25 · measurements 13 · examples 15 |
| Waiting on the author | **nothing** — three answered the day they were asked · 12 assigned in `SCHEDULED.md` · 264 in `LEARN.md` (never a gate) |

### Verify it yourself, right now

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

### What landed in M-separate-compilation so far — four repairs, then three steps

- **`docs/defects/002`** — a legal program that handled both arms of a fallible
  was killed before either. Five lines of runtime C, +0 spec tokens (panel 087).
- **A dead descriptor** in every unit that named a container type it never built
  (panel 088's neighbour, `/decide` 3c).
- **The compiler's last POSIX binding.** `selfhost/cli_io.hero` bound
  `extern "unistd.h"`, so `seed/heroes.c` carried `#include <unistd.h>` and the
  self-hosted compiler could not be built on Windows at all. Nothing caught it
  because the Rust bootstrap had no such dependency, so the third CI leg was
  testing a compiler that did not have the problem. `hero_write_err` replaces it;
  the Windows leg runs every step again, **untested from here until the first
  tag**.
- **The C text boundary, closed over two sittings.** Panel 089 landed
  `validated(c: cstr) -> str?` and `args_checked() -> [str?]`; panel 090 retired
  `cstr.to_str()`, the spelling that aborted, and made the spec say that `args()`
  aborts while `args_checked` does not. On the way the two sittings closed a
  defect nobody had looked for — **the shipping compiler could be killed by an
  environment variable** — and left the spec **five tokens lighter** than it
  started, because the second sitting's warden found a sentence worth more out
  than in.
- **A field declared as an array where the header has a scalar** stopped being
  `internal error` at exit 2 and became the author's own diagnostic at exit 1;
  and **a real C name declared under the wrong header** stopped doing the same,
  because clang has three words for that mistake and the mapper knew two.
- **Two sentences in which the specification contradicted itself**: its own
  § Tests example did not compile for seven days — it is now a golden that runs —
  and `spec:14` claimed strings may hold any UTF-8 when a raw carriage return is
  refused in a string and accepted in a comment.

**The 46× question is answered, repaired and closed all the way down**
(measurements 012 and 013). The writer and the loader fell to two repairs, then
the author pulled panel 037's trigger and the place store landed: the compiler
builds itself in **188.51 s against 944.76 s the day before — 5.01×** — the suite
6.2× faster, the spec 6 tokens lighter, and one masked use-after-free found and
repaired on the way.

**Step 1 — the `ORDER:` mark stops being a grep** (2026-08-25). A map walk whose
order somebody can see owes an explicit `sort` and a mark saying by what and who
watches; the inventory was a grep, and a grep cannot tell a mark from a sentence
quoting one — it returned **10 hits in 8 files, four of them prose**. Measured
against the item's own premise: all four walks it called unmarked were **already
sorted**, so the fixpoint was green because the sorts were paid, not by luck. Now
`tests/harness/suite_order.hero` fails the net when a walk in `selfhost/` carries
no mark, and it was made to fire before it was believed. Inventory **12 files /
16 marks**, three previously unjudged walks ruled `ORDER: none`.

**And the net got a net** (author decision the same day, `/decide` answer `a`).
Running step 1's own new cases meant running `heroes test tests/harness/main.hero`
— which **nothing ran**, and **two of its 81 blocks had been red** long enough to
predate the step that found them. Both were the test being wrong: a count that a
later commit made stale, and a self-test that reached a six-day-old binary
because `./heroes` was missing from the candidate list while the **archived
bootstrap's path was on it** — the file's own comment has said *"the path they
must not name is the bootstrap's"* since the archive, with the list underneath
naming it. It is a CI leg now, on every push: **13.5 s**, because it compiles the
5,606-line harness rather than the 38,021-line compiler.

**Step 2 — the block stays, because the architecture already deletes the
duplication** (panel 091, five judges, ratified). The last `SCHEDULED.md` item
this milestone owed was panel 087's unpruned `extern` asserts: **27 of a
`print(1)` program's 166 lines of C** exist for a library group it never touches,
and **138 of 155 modules** would carry that under one `.c` per module. Every
option that prunes was **vetoed with compiled evidence** — pruning the `#include`
deletes a group record's C type; an *uncalled* group holds up a used one
(`<stdio.h>` before `<jpeglib.h>`); the prune takes the `-l` with the declaration
and a constructor library stops running at exit 0; and the probe is a
**memory-safety instrument**, the only thing that sees a `size_t` out-parameter
declared `i32`, which when executed wrote four bytes into an adjacent object with
ASan and UBSan silent. What replaces them costs **nothing**: the library is one
module, so one `.c` per module plus acceptance row 1 puts every group in exactly
one TU. Option B would have cost +40 compiler lines and 146 re-blessed emissions
to buy what the close gives away.

**Step 3 — the check was there all along, and a macro was hiding it from half of
libc** (panel 092, five judges, ratified). Convened on the claim that an
`extern`'s parameter width and sign are unchecked. **They are checked** —
`-Werror=shorten-64-to-32` and `-Werror=sign-conversion` have shipped all along —
and the silent set was never clang's builtins: `strncmp`, `malloc`, `calloc`,
`abs` and `fwrite` are builtins and are all caught. What hid the rest is
**`_FORTIFY_SOURCE`**, which rewrites `memset` into `__builtin___memset_chk(…)`
and discards the diagnostic; `-fno-builtin` cannot touch it, because a **macro**
does the redirect. **Two production lines fix it** — `(void)(f)(…)` is not a
function-like macro invocation — and the repair immediately found a **live defect
in this repository**: `examples/curl` declared `option: i32` where `CURLoption`
is unsigned, invisible since the example was written. A third line stops the
emitter routing **its own** warning to the author's line. Cost: 922 re-blessed
lines across 146 emissions, 36 hand-edited in the six `emit/` goldens where
`UPDATE_GOLDEN` is forbidden, and **+12 spec tokens** for the one mapping row a
reader cannot derive — `u64` where C says `size_t`.

**Step 4 — the float half closes with four flags where the sitting expected one**
(2026-08-25, landing panel 092's queued half). `-Wdouble-promotion` was measured a
QUARTER of the answer: `f64` against a header's `float` still demoted in silence
under it, a float against a header's integer dropped its fraction in silence, and
the integer family had the same gap below the 64→32 window (`i64` against `short`).
The probe block's pragma now carries **four** errors, each with a witness that
fired first — and the scope is measured load-bearing: enabled globally the four
fire on **19 of 146** blessed emissions of correct programs, so none can ever
enter `flags()`. All four wrong directions are `error[ffi_parameter_type]` at
exit 1 on the author's line with the right fix; the spec names what stays silent,
once, at **+32** (3592): what C converts exactly, and what a pointer points at.

**Step 5 — acceptance row 1: an `extern` never crosses a module boundary**
(2026-08-25, executing panel 033 R5 at panel 091's measured price). The seven
qualified `cli_toolchain.system` call sites route through `cli_shell.shell`,
a Heroes function beside the extern; the rule lands as
`error[extern_across_modules]` on the qualified **reference** — the call and the
extern taken as a value alike — and closed two shapes the ruling had not named,
both measured open: the library's externs were reachable unqualified
(`hero_args_count()` was check-clean from any program, zero users in the
repository), and UFCS reached them through the dot. Group constants and records
stay reachable, deliberately: their C is compiler-written on both sides. Both
directions pinned: `externroute/wrong.hero` exit 1, `right.hero` runs exit 0.
The layout check fired on the landing — `resolve_walk.hero` was frozen at its
366-line pin — and the answer was a seam, not a squeeze: the name lookup left
as `resolve_names.hero`, and the walk is under the default ceiling for the
first time since the port.

---

## The chain

One table, one row per milestone. `warrant` is why it exists: **v1** (the
self-hosting finish line), **closure list** (design.md §1.0 — the compiler needs
it), **§1.1** (comprehension is the objective), or **scheduled, no warrant**.

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
| 26 | **M-separate-compilation** | **OPEN** | — | — | one `.c` per module, prototypes across TUs, the cache · closure list |
| 27 | **M-package-manager** | scheduled | — | — | `heroes add`/`heroes fetch`; bindings instead of a standard library |
| 28 | **M-isolated-threads** | scheduled | — | — | Part 7.13 concurrency |
| 29 | **M-qbe-backend** | scheduled | — | — | Part 7.14 — the proof that the IR is not C in disguise |
| 30 | **M-lsp-server** | scheduled | — | — | `heroes lsp` |
| 31 | **M-vscode-extension** | scheduled | — | — | the extension, complete |
| 32 | **M-documentation-site** | scheduled | — | — | the site, anchored to programs that run |
| 33 | **M-journey-book** | scheduled | — | — | the journey — how this language came to be |
| 34 | **M-guide-book** | scheduled | — | — | the guide, as a book you would find in a shop · **§1.1** |
| 35 | **M-publication-gate** | scheduled | — | — | the last gate before anything goes outward · CLAUDE.md §14 |

Three closed milestones have no tag of their own because they were parents or
sub-steps: **M-checker-core**, **M-data-declarations** and **M-rich-diagnostics**
landed inside `m3`, and **M-native-backend** was the parent of the four backend
milestones and was never tagged. § The names carries them.

`git tag --list --sort=creatordate` gives the same chronology from git itself.

---

## What is next, in detail

### M-separate-compilation — one `.c` per module *(open)*

One `.c` per module, prototypes across translation units, the per-module cache:
the second half of the modules row, moved past the fixpoint because it costs
+700–1000 lines against the namespace's ~+330, because Nim has not finished its
own per-module cache since 2018 and Rust shipped 1.52.1 to disable incremental,
and because — measured — it is where §4.19's guarantee can quietly die.

**Four acceptance rows, and they are what lift the ffi-pragmatist's veto**
(panel 030 R2):

1. the header travels with the `extern` into **every calling TU**, or externs are
   module-private and the *type checker* refuses the qualified call. **Panel 033
   found a third answer that costs nothing and is not a visibility rule**: an
   `extern` declaration is never callable across a module boundary, a qualified
   call to one is a type error, and the route is a Heroes function in the
   declaring module. Compiled, linked and run — and the shape this row exists to
   prevent reproduced exactly beside it (header only in the declaring TU: exit 1
   there, **exit 0 in the caller**, `rc=0 db=open`, with the wrong signature);
2. headers and link flags enter the cache key, with `runtime_text()` kept;
3. every dependency's emitted interface enters the key — `-flto` does not catch
   cross-TU signature skew and changes the answer;
4. one two-module FFI-shaped golden with a wrong `extern` signature: **exit 1 in
   both TUs**, `#~` annotated.

### M-package-manager — packages, and what stands in for a standard library

**Scheduled, no warrant.** Not a decision to take later: design.md:637 already
fixes the shape — *"No package manager exists before modules do; when it arrives
it will be `heroes add`/`heroes fetch` — inside the same binary"* (never a second
binary, CLAUDE.md §6 and §10). Its real prerequisite is
**M-separate-compilation**, not M-module-namespace: without separate compilation,
installing a package means recompiling the world on every build.

**And this is where "a standard library that wraps C" goes.** §1.11 refuses a
standard library permanently, and that refusal is the founding constraint rather
than a shortage of effort — but what a standard library is *wanted* for arrives
here in a form the constraint permits: **distributable bindings**, ordinary
Heroes modules over real C headers, each with its link flag declared next to the
`extern` that needs it (§3.5). The difference is not cosmetic: a binding is
verified by clang against the header it names, and a standard library is verified
by whoever wrote it.

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

### M-documentation-site — the whole language, anchored to programs that run

**Scheduled, no warrant.** `site/` exists (`index.html`, its CNAME, and the
register rules in `site/README.md` § Style guide, which stay in force — song
titles as section nods, never lyrics; personality in the packaging, precision in
the substrate).

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

## What the closed milestones settled

Their records are the journals; what is kept here is the reasoning a future
milestone still has to honour.

### M-selfhost-port — the port

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

### M-selfhost-fixpoint — the fixpoint and the seed · **v1**

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

### M-harness-port — the net, in Heroes

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

### M-bootstrap-archive — the third language dies

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
| `M-package-manager` | M10 | — | `heroes add`/`heroes fetch`, bindings in place of a standard library |
| `M-isolated-threads` | M11 | — | Part 7.13: per-thread heaps, copying at the boundaries, no scheduler |
| `M-qbe-backend` | M12 | — | Part 7.14: the proof that the IR is not C in disguise |
| `M-lsp-server` | M13 | — | `heroes lsp` |
| `M-vscode-extension` | M14 | — | the extension, complete |
| `M-documentation-site` | M15 | — | the site |
| `M-journey-book` | M16 | — | the journey |
| `M-guide-book` | M17 | — | the guide |
| `M-publication-gate` | M18 | — | the last gate before anything goes outward |

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

---

## End-to-end verification, per milestone

```sh
heroes doctor                                  # M-day-zero
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes    # M-bootstrap-archive: the way in
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
./A build selfhost/heroes.hero --emit-c -o B.c && clang … B.c -o B
./B build selfhost/heroes.hero --emit-c -o C.c && diff B.c C.c
```
