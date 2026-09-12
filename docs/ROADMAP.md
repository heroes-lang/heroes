# ROADMAP — where the project is, and what is next

Autonomous work sessions need the goal chain **in the repository**: tags say
where the project **is**, not what is **next**. This file is the distillation of
the approved bootstrap plan (revision 2, reviewed by panel 000); the build
order's rationale is design.md Part 10 and the language-level acceptance criteria
are design.md Part 0.

**Five rules hold its shape**, each one bought by a reorganisation this file
needed (author instructions 2026-08-25, 2026-08-26, 2026-09-03 and 2026-09-07,
with what each one measured in the git history and in `DESIGN-LOG.md`):

- **The past never stands in front of the future.** A closed milestone's record
  goes to its own journal, indexed at `docs/records/journal/README.md`; § Where we are is
  held under **15 lines** by `/step`'s checklist and by `suite_records.hero`'s
  `where_we_are`. This file once carried 512 lines about the past before its
  first line about the future, growing about 66 lines per close.
- **§ The chain carries the ORDER and nothing else.** Every scheduling note,
  ratification and author decision that used to live inside a cell is a line
  under the table, keyed **by name** — a reorder moves a number and never a name
  (CLAUDE.md §14).
- **The chain runs closed, then scheduled**, so a reader meets the whole past
  before the first line of the future.
- **One section per milestone that still has something to say**, in the chain's
  order, and a section whose milestone has closed says so in its heading.
- **One copy of a duty.** The two verification blocks used to sit 700 lines
  apart, which is how one of them rots; they are one section under the summary
  table.

---

## Where we are

| | |
|---|---|
| **Current milestone** | **M-stated-grammar** — **OPEN**, row 75: the productions enter the specification, and `heroes grammar` prints the half a compiler can derive. Beside it **M-reflection-verdict**, worked in another lane since 2026-09-11 with its row left `scheduled`, which § The chain has allowed since 2026-09-12 |
| **Last closed** | **M-rotated-records**, 2026-09-12, `m-rotated-records` ([049](journal/049-rotated-records.md)) — the records became directories and the files they were stay as maps · before it **M-named-callbacks** ([048](journal/048-named-callbacks.md)) and **M-labelled-types** ([047](journal/047-labelled-types.md)) · v1 **reached** at M-selfhost-fixpoint, 2026-08-18 |
| Milestones closed | **50** of 75 · **49** milestone tags, the legacy `m0`-`m8` included |
| The compiler | **58,665** lines of Heroes in **199** modules (`find selfhost -name '*.hero'`) · the seed **790,756** lines of C |
| The spec | **4430** on the vendored ranks, unchanged by this milestone, which spent no spec token · the real count is `heroes measure --refresh`'s and was last taken 2026-09-11 |
| Records | sittings **131** · journals **50** · milestone files **44** · entries: `docs/records/log/` **621**, `docs/records/done/` **491**, `docs/records/book/beats/` **115** · **open defects 0** |
| Waiting on the author | **1** decision: panel 133's ratification. The ceiling question it queued was answered the same day, 8192 by author decision |

**Re-measured 2026-09-12 at M-rotated-records' close, not carried.** `records` is
**20** checks, the net's own tests **137**, and the suite reads 23 s against 17.7 s on 2026-09-06, over 1,227 files more.

---

## Verify it yourself

### From a cold checkout, right now

```sh
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes   # the compiler, from C alone (~3.5 s)
./heroes run tests/harness/main.hero -- ./heroes             # the net
./heroes test tests/harness/main.hero                        # the net's own tests (121, 2026-09-07)
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

## What production-ready means, and who owns each part

**The yardstick the chain is judged by, written 2026-09-10 because it did not
exist.** design.md defines v1 as *"the language is finished for v1 when it can
compile itself"* (`:114`, reached 2026-08-18) and defines production readiness
**nowhere**: a sweep of all 3,612 lines found no section on distribution,
versioning, stability or a 1.0 promise, and no occurrence of semver, deprecation,
backward compatibility, release cadence, LTS or an installer. The only `1.0` in
the file is **Go's** (`:3202`). So every row of § The chain had been scheduled on
its own reason and none had ever been measured against a standard.

**The standard is not a mature language**: the question is whether a stranger can
put **production code that is not mission critical** in Heroes, which is a lower
bar than either half of that phrase suggests on its own. **And it is a list, so
it is a measurement** (CLAUDE.md § RUN IT): the
ten rows come from what Go 1, Nim 1.0 and Rust 1.0 each shipped, read against
what this tree already reaches for. **Six of the ten were already owned**, which
is the finding.

| | what a production user needs | who owns it | state on 2026-09-10 |
|---|---|---|---|
| 1 | a **written promise** about what keeps compiling | M-compatibility-promise | the paragraph was owned, **the instrument was not** |
| 2 | **getting the compiler** | M-install-channels | owned |
| 3 | **the machines it runs on** | M-arm-platform | three legs, and **arm64 Linux unowned** |
| 4 | **shipping the binary you built** | M-deployable-binary | **owned by nobody** |
| 5 | **depending on other people's code** | M-core-packages · M-package-manager | owned; pinning is that sitting's |
| 6 | **the capabilities a real program needs** | M-core-packages' package table | owned in the large, silent in four places |
| 7 | **diagnosing a failure in production** | M-panic-location · M-typed-inspection | owned |
| 8 | **not falling over** | M-generated-programs · M-check-completeness | owned |
| 9 | **editing it** | M-lsp-server · M-vscode-extension · M-doc-generator | owned, and **the colouring had drifted** |
| 10 | **knowing the claim is true** | M-thesis-harness · both books | owned |

**Row 6's four silences** are `docs/work/SCHEDULED.md (retired 2026-09-12)` items at M-core-packages —
text to a number, width and precision, a signal handler that cannot record that
it fired — except the fourth, a release bound to a scope, which is a question
about the language and became **M-cleanup-verdict**. **Row 9's drift** was the VS
Code grammar four days behind `f"…"` while the site's highlighter kept up, with
nothing judging either: the repair is an item at M-vscode-extension and the rule
that prevents the next one is `.claude/rules/diagnostics-and-goldens.md`'s walk,
widened the same day.

**The thirteen candidates this reading refused, each with the rule that refused
it, are in `docs/work/DONE.md`** with the five `DESIGN-LOG.md:539` had already
refused on 2026-09-03 — because that entry's own stated reason is *so the
candidate is not proposed again as new*.

---

*******************************************************************************

## The chain

One table, one row per milestone, **closed first and scheduled after**: rows 1–47
are done, in the order they closed, and rows 48–71 are what is next, in the order
they will be taken. **This sentence said 1–37 and 38–59 until 2026-09-10**, which
is seven closes behind: it is the one number in this file that no instrument
reads, so it is restated here and re-read at every close with § Where we are. `warrant` is why a milestone exists: **v1** (the self-hosting
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
| 38 | **M-open-repository** | done 2026-09-08 | `m-open-repository` | [037](journal/037-open-repository.md) | the repository opens, and every page that says it is shut stops saying so · CLAUDE.md §14
| 39 | **M-discard-refusal** | done 2026-09-08 | `m-discard-refusal` | [038](journal/038-discard-refusal.md) | `_ =` on a fallible value becomes a compile error, in two positions and not three · **§1.1**
| 40 | **M-closures-verdict** | done 2026-09-08 | `m-closures-verdict` | [039](journal/039-closures-verdict.md) | the ruling on Part 7 items 1 and 12: both refused, closures to Part 6 and inline blocks left unplaced · **§1.1**
| 41 | **M-cstr-lifetime** | done 2026-09-09 | `m-cstr-lifetime` | [040](journal/040-cstr-lifetime.md) | a `cstr` may not outlive the `str` it was lent from: three positional clauses, 0 of 638 files moved. **The tag came later than the close, at the author's word on 2026-09-09**: this milestone shipped with defect 024 open by panel 122 R5's design, the lend C RETAINS, and `records/tagged` reads the newest tag's own commit for clean lists, so the tag stands on the commit that ratified panel 125, the first where every list this milestone left open is clean; the tag's own message says so · **§1.12**
| 42 | **M-interpolation-verdict** | done 2026-09-09 | `m-interpolation-verdict` | [042](journal/042-interpolation-verdict.md) | the ruling on design.md Part 7 item 7: string interpolation ENTERS as `f"line {n}: {word}"`, three counts and one sitting, the spelling the author's; closed with the verdict and not the form, as it was named to · **§1.1** |
| 43 | **M-held-bytes** | done 2026-09-09 | `m-held-bytes` | [041](journal/041-held-bytes.md) | §4.19's FOURTH case: `x: cstr @ s.lease()` is a COPY C may read until `end_lease(@x)`. Two sittings, four refused rules, one capability; defect 024 CLOSED with its limit named, and the first tag since 2026-09-08 because the list is clean · **§1.12**
| 44 | **M-interpolated-strings** | done 2026-09-09 | `m-interpolated-strings` | [043](journal/043-interpolated-strings.md) | `f"line {n}: {word}"`: one token kind, one AST node, three helper modules, four decided ceilings moved by their measured lines, the spec at +138 real with R6's two sentences, and `heroes mutate` reaching the holes · **§1.1**
| 45 | **M-anchored-spec** | done 2026-09-11 | **untagged**, defect 025 | [044](journal/044-anchored-spec.md) | the specification takes the shape of a Report: thirteen numbered sections in reference order, one home per rule, the numbers as the citation anchors, two checks that keep the form, 81 citations converted, and the site's page copying the text in one click · **§1.6** |
| 46 | **M-labelled-builtins** | done 2026-09-11 | **untagged**, defect 026 | [045](journal/045-labelled-builtins.md) | defect 025 repaired: the same-typed-argument rule reaches the two built-ins it did not, both halves of it, 1022 call sites relabelled, and the classic argument inversion drops from 367 survivors to 75 with the swapped `fail(` at zero · **§1.2** |
| 47 | **M-positional-values** | done 2026-09-11 | **untagged**, defect 026 | [046](journal/046-positional-values.md) | defect 026 narrowed: a label at a call through a function value is refused, the note stops prescribing the hole in silence, and `spec § 3` says the call is positional — a sentence vetoed as false at the ballot that the refusal made true · **§1.2** |
| 48 | **M-labelled-types** | done 2026-09-11 | `m-labelled-types` | [047](journal/047-labelled-types.md) | defect 026 repaired: a function type names the parameters that can be confused, the names are part of its identity, and the classic inversion through a function value goes from 17% to 100% — at 432 lines against a condition of about 150, which is the sitting's own headline · **§1.2** |
| 49 | **M-named-callbacks** | done 2026-09-11 | `m-named-callbacks` | [048](journal/048-named-callbacks.md) | the role inversion through a generic callback, closed at 7 of 7 against a cheaper reading's 2 of 7: `fold`'s type names its accumulator and its element, and a function handed to it names them the same way — a naming mandate the author chose over the sitting's own recommendation, with both numbers in front of them · **§1.2** |
| 50 | **M-reflection-verdict** | scheduled | — | — | the ruling on reflection — at run time, and as compile-time derivation over a record's fields — and, since 2026-09-06, on a general annotation mechanism · a decision, not a feature |
| 51 | **M-deferral-ledger** | scheduled | — | — | every Part 7 item with no milestone gets a dated verdict or a return condition |
| 52 | **M-cleanup-verdict** | scheduled | — | — | the ruling on a scope-bound release — `defer` or another form — for the two obligations `owned` and `lease` put on every path · a decision, not a feature |
| 53 | **M-check-completeness** | scheduled | — | — | what `heroes check` accepts, `heroes build` compiles — through a generic too · scheduled, no warrant
| 54 | **M-arm-platform** | scheduled | — | — | the fourth real machine: arm64 Linux, where a container runs and where `char` is unsigned · **§1.12** |
| 55 | **M-core-packages** | scheduled | — | — | small packages that compose, organised as Go's tree, in Heroes or over C |
| 56 | **M-package-manager** | scheduled | — | — | `heroes add`/`heroes fetch`; bindings instead of a standard library |
| 57 | **M-web-framework** | scheduled | — | — | composes the core packages, Go/Echo style: explicit routes, records, no magic |
| 58 | **M-doc-generator** | scheduled | — | — | `heroes doc`, the one direction Part 6's literate-source row promises · scheduled, no warrant |
| 59 | **M-panic-location** | scheduled | — | — | a panic names the `.hero` file, line and function · **§1.12** |
| 60 | **M-typed-inspection** | scheduled | — | — | a stopped program shows Heroes values: the name the author typed, and `[T]`, `{K: V}`, `T?`, a variant and a record shown as themselves · scheduled, no warrant |
| 61 | **M-generated-programs** | scheduled | — | — | programs nobody wrote: a generator that composes valid Heroes and knows the answer before the compiler is asked · **§1.12** |
| 62 | **M-thesis-harness** | scheduled | — | — | Part 11's metrics 2 and 4 run for the first time, as a Heroes program · **§1.1** |
| 63 | **M-lsp-server** | scheduled | — | — | `heroes lsp`, and the incremental frontend it needs |
| 64 | **M-vscode-extension** | scheduled | — | — | the extension, complete |
| 65 | **M-qbe-backend** | scheduled | — | — | Part 7 item 15 — the proof that the IR is not C in disguise |
| 66 | **M-journey-book** | scheduled | — | — | the journey — how this language came to be |
| 67 | **M-guide-book** | scheduled | — | — | the guide, as a book you would find in a shop · **§1.1** |
| 68 | **M-deployable-binary** | scheduled | — | — | what the machine that RUNS a Heroes program needs, on the three platforms, and which `-O` a shipped artifact carries |
| 69 | **M-microcontroller-verdict** | scheduled | — | — | the ruling on a Heroes program running on a microcontroller under an RTOS, RISC-V first; the two facts a 32-bit build refuses today are its brief · **§1.12**, Part 2 |
| 70 | **M-install-channels** | scheduled | — | — | a Homebrew tap, winget, a Nix flake, a Docker image, all built from the seed and pinned to a `v*` release tag; the version scheme itself was decided ahead, 2026-09-07 |
| 71 | **M-online-compiler** | scheduled | — | — | the compiler reached without installing anything: the site's visitor writes Heroes and gets its answer · scheduled, no warrant |
| 72 | **M-compatibility-promise** | scheduled | — | — | the paragraph `1.0.0` rests on, and the suite that makes a broken promise red · CLAUDE.md §14 |
| 73 | **M-publication-gate** | scheduled | — | — | the last gate before anything goes outward · CLAUDE.md §14 |
| 74 | **M-rotated-records** | done 2026-09-12 | `m-rotated-records` | [049](journal/049-rotated-records.md) | the records become directories, one entry per file and one milestone per file, and the file that held each stays behind as a map so every citation still resolves · CLAUDE.md §14 |
| 75 | **M-stated-grammar** | **OPEN** | — | — | every syntactic form stated once, in Wirth's notation, inside the specification beside the prose that governs it, and `heroes grammar` for the half the compiler can print from its own tables · **§1.1** |

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
- **M-held-bytes** — **§4.19's fourth case, and the reasoning a later milestone must honour is in three sentences.** A declaration-site mark for pointer retention cannot be made right, because the decision is the C function's own ARGUMENT (`sqlite3.h:4888`, `curl_easy_setopt`'s second) and the header types both spellings `const char *`; a later milestone that reaches for `kept` or `transient` on a parameter re-argues panel 124 against a measurement. The lease is a COPY and never a pin, and its release is WRITTEN and never inferred (panel 124 R3, R4): a pin aliases `.cstr()` under copy-on-write, and an inferred release is the escape analysis panel 122 refused and panel 124 priced at 1.44 MB against 81.9 MB. And the pointer lives in ONE cell (panel 125): a lease's name stands only as an argument of a call, nothing but `end_lease` writes its cell, and a runtime guard that dereferences to validate is undefined on a foreign pointer — so a later `Ty` case, a registry, or a magic word is a route already measured and refused, and the record says by how much. **What it did not close is named**: a lease C retains past the `end_lease` the program wrote, or launders through a C function that returns its argument, is undecidable, and its instrument is the Linux `--sanitize` leg.
- **M-cstr-lifetime** — **placed first and alone by author decision 2026-09-09**, chosen over the coordinator's recommended order of three milestones (the budget instrument, then the parser seam, then the interpolation implementation). Its ground is § Precedence: the defect is a silent wrong answer with memory corruption, robustness is rank 3, and a milestone is tagged only over a clean list — so nothing else can close while `docs/work/DEFECTS.md` holds 022.
- **M-interpolation-verdict** — **scheduled by author instruction 2026-09-02**, closed 2026-09-09 with the verdict and not the form. What a later milestone honours: the brace is active ONLY behind an `f`, because 211 literals in the compiler's own modules hold one and an ungated brace is a two-stage bootstrap; a hole admits ANY expression, because the narrow rule costs more tokens and refuses a `???` the spec promises anywhere; and the form is ONE AST node with the holes as children, never a payload on `.str_lit`, because 59 walks would otherwise treat it as a leaf.
- **M-interpolated-strings** — **opened and closed 2026-09-09** (panel 121 R10). What a later milestone honours: only `{{` is doubled, because a `}` in text mode is text, and a hole whose expression begins with `{` needs a space after the opener for Python's reason; the desugar is the lowering's and the printers re-print the literal verbatim; a hole's type is `to_str`'s set, refused with `to_str`'s words. The constraints it inherits, measured at the open: a new node touches **85 sites in 27 files** that enumerate `.str_lit`; `grammar_expr.hero` is at 1002 of 1002 and a helper module that calls `parse_expr` closes a `use` cycle (R5), so the parser's hook stays in the knot and the DECIDED number rises by exactly its lines with the reason written; `check/walk.hero` 1700 of 1700, `ir/flatten.hero` 1102 of 1110, `print/fmt.hero` 1136 of 1150 get one-line arms that call helper modules where a helper needs no call back into the knot. The two spec sentences R6 accepted land WITH the clause, and the clause is re-measured on today's spec first.
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
- **M-doc-generator** — **accepted against the recommendation**:
  `heroes doc` is in the record once, as the promise in Part 6's literate-source
  row, and CLAUDE.md §10's stopping rule refused two ROADMAP-scheduled verbs
  before it (`docs/measurements/003` rider 3, `outline` and `explain`). It
  stands after M-web-framework so that its witness — the packages' own API —
  exists, and its entry names the rule as the first question of its opening.
- **M-panic-location** — the cheapest half of *"a strong runtime"*: a
  panic today prints `panic: <msg>` and aborts with no line and no function
  (`runtime/parts/panic.c`), while the emitted C already carries `#line`. Placed
  after the runtime leaves M-isolated-threads' hands; it depends on nothing else
  and may be taken the day the threads land.
- **M-thesis-harness** — Part 11's metrics 2 and 4 have never run, and
  the instrument is a Heroes program over the HTTP client M-core-packages
  delivers. **The author's decision the same night**: a small seed of tasks
  written by the author at its opening, the bulk still at M-guide-book as decided
  2026-08-24.
- **M-lsp-server takes the incremental frontend** — the `SCHEDULED.md` item that
  asked *which milestone does it* has its answer: a server that re-checks on every
  save cannot wait 8 s for `heroes check` on the compiler's own source.
- **M-qbe-backend after the tools** — moved behind them at the reorder: the proof that the IR is
  target-agnostic is worth most when the IR has stopped moving, and every ruling
  above it may move it. Its cells said *Part 7.14* and now say item 15: item 14
  has been declaration visibility since panel 033 (`design.md:2562`, `:2579`).
- **M-install-channels** — before the gate: a Homebrew tap, winget, a Nix
  flake and a Docker image, all building from the seed with the one clang line,
  because `heroes` without clang compiles nothing and a prebuilt binary alone
  would be a decoy; prepared and tested in private, published as the gate's
  outward act. `heroes --version` printed `heroes 0.0.1` that evening and the
  tags are milestone names, so a version scheme comes with it.
- **M-declared-freer, M-thread-stacks, M-discard-refusal and M-check-completeness**
  — **four rows entered together on 2026-09-04 by author instruction**, *"fix
  SCHEDULED too, so that every item is attached to a roadmap step that is still
  to be done; if you cannot find one, create it"*. The instruction is a rule about
  `docs/work/SCHEDULED.md (retired 2026-09-12)` and these rows are what it cost: of its **29** items,
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
- **M-online-compiler** — **scheduled by author instruction 2026-09-06**,
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
- **M-generated-programs** — **scheduled by author instruction
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

- **M-typed-inspection** — **scheduled by author instruction 2026-09-06**:
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
- **M-cleanup-verdict, M-arm-platform, M-deployable-binary and
  M-compatibility-promise** — **four rows entered together on 2026-09-10 by
  author decision**, out of the session that asked whether the chain was missing
  steps for the language to be usable for **production code that is not mission
  critical**. What produced them is § What production-ready means, and **six of
  its ten rows turned out to be owned already**. All four were put with a
  recommendation and all four accepted; each row's section carries its own
  measurements. **Two positions were choices rather than consequences and both
  are recorded**: M-arm-platform stands before
  M-core-packages, the robust position, where the cheaper one was before
  M-install-channels at the price of every package binding owing a
  re-measurement (CL-040); and M-compatibility-promise stands before the gate
  rather than inside it, because the gate is a checklist of outward acts and this
  is a suite. **What entered no row is recorded too**, in `docs/work/DONE.md`,
  because a candidate met by a rule must not come back as new.
- **M-core-packages' step order was reversed at the top on 2026-09-10 by author
  decision**, in the same session and from a different question: the author asked
  for a step before M-web-framework delivering mini packages, JSON and a small
  HTTP server in Go's shape, and **that step was already this one, already
  there**. What the question exposed, and what it cost, is in that milestone's own
  section; the author's question about how thick a wrapper over C should be became
  its opening sitting's question (ix) rather than a decision taken here.
- **M-microcontroller-verdict** — **entered 2026-09-10 by author decision**, out
  of the session that asked whether supporting a microcontroller such as the
  ESP32, or a program under an RTOS, would be worth a step, given a compiled
  language with no garbage collector. Three recommendations were put and all
  three accepted: a row now rather than a note, a verdict rather than a target,
  and the RISC-V chips first. The record was silent on the question, measured,
  so the row was written on what the compiler and the runtime answered on this
  Mac: its section carries the findings and
  `docs/measurements/026-the-two-facts-a-32-bit-target-refuses.md` the commands.
  The conservative route, a note and no row, was on the table and declined
  (CL-040). No board exists yet; the author is ordering one.
- **M-rotated-records** — **entered 2026-09-12 by author decision**, out of the
  session that asked whether more than one step could be taken at a time, with
  worktrees or otherwise. **The conflict was measured rather than assumed**: of
  the 100 most recent commits, `DESIGN-LOG.md` is touched by **40**,
  `docs/ROADMAP.md` by **31**, `docs/work/DONE.md` by **26** and
  `docs/work/DECIDE.md` by **24** — so two sessions meet in the registers and
  never in the compiler, and the answer is a shape rather than a git workflow.
  The author asked for the historical entries to move as well as the new ones,
  and what makes that safe was measured too: the 161 line-number citations into
  the two large records resolve to **54 distinct lines**, and the 43 in
  `DESIGN-LOG.md` every one of them open an entry — so a map with the same
  geometry keeps all 161 true, and makes them checkable for the first time,
  which CL-037 says no instrument can do today. **The row stands at the end of
  the table and is taken out of chain order**, next after the milestone now
  open: putting it in position would have moved twenty-three row numbers while a
  parallel session was committing into this same checkout, and the fifth
  renumbering (2026-09-08) is the reason that is worth avoiding.

---

## The milestones, one by one

**Moved to `docs/work/milestones/` on 2026-09-12**, one file per milestone, and
each file carries that milestone's own open items as well — the work the list
at `docs/work/SCHEDULED.md` held until 2026-09-12, when it was retired into these
files. One milestone is one file is one lane: two sessions on two milestones
write two files and never one line.

The order is § The chain's, above, and lives nowhere else. What a scheduled
section must say is in `.claude/rules/records.md`; `records/homes` is what
reads these files and refuses an item filed under a milestone that is not its
own.

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
`DESIGN-LOG.md`, `docs/records/journal/`, `docs/measurements/`, `docs/work/DONE.md`,
`docs/records/book/beats.md`, `tests/golden/`, every commit subject and all twelve legacy
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
| `M-cstr-lifetime` | — | `m-cstr-lifetime` | a `cstr` may not outlive the `str` it was lent from, which is defect 022: `return ("a" + n.to_str()).cstr()` builds with zero diagnostics under all fourteen flags and hands C a pointer into freed memory. **Named for the guarantee and not for the bug** (§14): the deliverable is a rule about how long a lent string lives, which is why `M-cstr-escape` was refused — it names the symptom, and a symptom is not a deliverable. **Deliberately not `M-use-after-free`**, which annexes a whole class of failure this language can reach by other routes and would leave no name for the next one; §14 forbids an id to make a claim a later milestone can falsify. Both runners-up are in `tests/harness/suite_records.hero`'s `REFUSED`. Placed first and alone by author decision 2026-09-09 · **§1.12** |
| `M-held-bytes` | — | — | §4.19's **fourth** case, which that section does not name: all three it reserved are about a pointer **C** made, and this is bytes **Heroes** made, that **Heroes** frees, which C may read for as long as the program says. **Named for what it delivers and not for what it refuses** (§14): panel 124 refused four candidate refusals, and the one thing that closed the two shapes no refusal could reach was a capability, so an id naming a rule would name the half of the sitting that failed. **Deliberately not `M-borrowed-pointer`**, which was the coordinator's suggestion in the brief and which names §4.19's case 2 — the case panel 124 measured to be unreachable from any declaration, because `sqlite3.h:4888` puts the retention decision in the fifth ARGUMENT. **And not `M-pinned-bytes`**, because a pin is what panel 124 R3 refuses: the buffer is a copy, or `s.cstr()` and the held pointer alias and a copy-on-write mutation rewrites what C is reading · **§1.12** |
| `M-interpolated-strings` | — | — | the implementation panel 121 R10 said takes an id of its own: `M-interpolation-verdict` was named for a RULING so that it could close with a refusal, and an id may not claim the thing the sitting existed to decide (§14). **Named for the literal a reader writes and not for the feature's area** — `M-string-interpolation` names the topic and would leave nothing for a second milestone that touches it, and `M-f-strings` names the spelling, which is the one part of the ruling the author may still rename · **§1.1** |
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
| `M-reflection-verdict` | — | — | the ruling on reflection, at run time and as compile-time derivation over a record's fields — a question the record had never given a row (§14, author instruction 2026-09-03); and, since 2026-09-06, on a general annotation mechanism, the author's question (iii), whose only refusal was a footnote |
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

| `M-open-repository` | — | — | the repository stops being private, and every sentence that promised it would open stops promising. **A new id rather than a step of `M-publication-gate`** (§14), for the reason `M-thesis-harness` and `M-install-channels` are: the gate is the outward act of a finished thing, and being readable is delivered with or without the thesis measured, the compatibility paragraph written or a channel installed. **Named for the act and not for the area**, which keeps `publication` free for the gate that still owes it |
| `M-cleanup-verdict` | — | — | the ruling on a release bound to a scope. **Named for the deliverable, which is a verdict** (§14), on `M-closures-verdict`'s precedent: a milestone named for the ruling can close with a refusal, and one named for the form would claim what the sitting exists to decide. **Not a step of `M-deferral-ledger`**, which dates Part 7's items: this is on no list at all, so a row that inherited it would report a promise the record does not hold. Scheduled 2026-09-10 by author decision with **no warrant** |
| `M-arm-platform` | — | — | arm64 Linux as a measured platform. **Not a step of `M-install-channels`** (§14): that row delivers the channels, this one the architecture they would otherwise ship for unmeasured, and the two are separable in both directions. **Deliberately not `M-fourth-platform`**, because `M-online-compiler` already calls a browser the fourth platform under panel 114, so the number would name two things; **and not `M-arm-linux`**, because what is delivered is a platform under `.claude/rules/platforms.md`'s own rules, three legs becoming four, rather than an operating-system port. Both runners-up are in `tests/harness/suite_records.hero`'s `REFUSED`. Scheduled 2026-09-10 by author decision · **§1.12** |
| `M-deployable-binary` | — | — | what the machine that RUNS a Heroes program needs, and which `-O` a shipped artifact carries. **Not a step of `M-install-channels`** (§14): that row delivers *how a person gets the compiler* and this one *what a person needs to run what the compiler made*, which are two deliverables and two audiences. **Named for the artifact and not for the act**, so `deployment` and `release` stay free — this row publishes nothing. Scheduled 2026-09-10 by author decision with **no warrant** |
| `M-microcontroller-verdict` | — | — | the ruling on a Heroes program running on a microcontroller under an RTOS. **Named for the deliverable, which is a verdict** (§14), on `M-cleanup-verdict`'s precedent: a milestone named for the ruling can close with a refusal, and one named for the device or its area would claim what the sitting exists to decide. **Not a step of `M-arm-platform`**: that row adds a machine the compiler runs on, this one asks about a machine only the program can reach. Scheduled 2026-09-10 by author decision · `docs/measurements/026-the-two-facts-a-32-bit-target-refuses.md` |
| `M-positional-values` | — | — | a call through a function value is positional, said by the document and enforced by a refusal, and the inversion it still admits measured and left open. **Named for the deliverable** (§14): the values a positional call carries; `M-function-labels` was the runner-up and was refused because the milestone delivers the absence of labels there rather than their presence |
| `M-labelled-types` | — | — | a function type carries the parameter names that keep two positions apart, and carries them in its identity. **Named for the deliverable** (§14): the types that are labelled, not the labels themselves and not the area; `M-named-parameters` was the runner-up and was refused because a declaration's parameters have been named since v0 and the id would have claimed a topic this milestone does not deliver |
| `M-named-callbacks` | — | — | the callbacks that carry names, and the milestone that makes a generic one able to. **Named for the deliverable** (§14): `M-generic-labels` was the runner-up and was refused because the milestone delivers names on callbacks and not a generics feature |
| `M-labelled-builtins` | — | — | defect 025 repaired: §4.9's same-typed-argument rule reaching the two built-ins that escaped it, `fail` and `slice`, both halves of the rule, and the 1022 call sites that pays for. **Named for the deliverable** (§14): the built-ins that carry their labels. `M-label-completeness` and `M-labelled-fail` were the runner-up names and stayed in the session |
| `M-anchored-spec` | — | — | the specification re-shaped into thirteen numbered sections in reference order, the numbers as the citation anchors, the checks that keep the form, and the site's page copying the text in one click. **Named for the deliverable** (§14): a specification with stable anchors; `M-numbered-spec`, `M-spec-anchors` and `M-ordered-spec` were the runner-up names and stayed in the session's plan |
| `M-compatibility-promise` | — | — | the paragraph `1.0.0` rests on, and the suite that makes a broken promise red. **Not a step of `M-publication-gate`** (§14), which owns that paragraph as one bullet of a checklist of outward acts: this is a suite, built and tested in private like everything else. **Named for the promise and not for the version**, since `1.0.0` is the author's act on a clean `main` and never a milestone's (`.claude/rules/records.md` § Release tags). Scheduled 2026-09-10 by author decision · CLAUDE.md §14 |
| `M-stated-grammar` | — | — | every syntactic form of Heroes stated once, in Wirth's notation, inside `spec/heroes-spec.md` beside the prose that governs it, plus `heroes grammar` for the half a compiler can print from its own tables. **The id is a phrase the sitting that produced it already uses** (§14 prefers one the record uses over an invented one): panel 133's proposal opens *"stating every syntactic form of Heroes once"*. **Deliberately not `M-grammar`**, one word where §14 asks for two, which appropriates a topic a later milestone will need. **Deliberately not `M-inline-grammar`**, which names where the productions sit rather than what is delivered, and would be falsified the day the collected view is printed as well. **And deliberately not `M-formal-syntax`**, which names the area |
| `M-rotated-records` | — | — | the records become directories, one entry per file and one milestone per file, and the file that held each stays behind as a map |

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
