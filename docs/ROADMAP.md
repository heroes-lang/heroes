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
| **Current milestone** | **none open on the trunk.** **M-reflection-verdict** is worked in another lane since 2026-09-11 with its row left `scheduled`, which § The chain has allowed since 2026-09-12 |
| **Last closed** | **M-stated-grammar**, 2026-09-12, `m-stated-grammar` ([050](journal/050-stated-grammar.md)) — every form stated once, beside the prose that governs it · before it **M-rotated-records** ([049](journal/049-rotated-records.md)) and **M-named-callbacks** ([048](journal/048-named-callbacks.md)) · v1 **reached** at M-selfhost-fixpoint, 2026-08-18 |
| Milestones closed | **51** of 75 · **50** milestone tags, the legacy `m0`-`m8` included |
| The compiler | **59,155** lines of Heroes in **202** modules (`find selfhost -name '*.hero'`) · the seed **790,756** lines of C |
| The spec | **5655** on the vendored ranks and **7531** on the reader's own, against a ceiling of 8192 raised by author decision this day so the grammar's productions could go inside it |
| Records | sittings **131** · journals **51** · milestone files **45** · entries: `docs/records/log/` **624**, `docs/records/done/` **493**, `docs/records/book/beats/` **116** · **open defects 0** |
| Waiting on the author | **0** decisions. Panel 133's was answered the day it was filed: the ceiling rises to 8192 |

**Re-measured 2026-09-12 at M-stated-grammar's close, not carried.** `records`
is **21** checks, the net's own tests **145**, and `grammar` is the twentieth suite.

---

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
| 75 | **M-stated-grammar** | done 2026-09-12 | `m-stated-grammar` | [050](journal/050-stated-grammar.md) | every syntactic form stated once, in Wirth's notation, inside the specification beside the prose that governs it, and `heroes grammar` for the half the compiler can print from its own tables · **§1.1** |

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

---

**What used to stand under this file lives in `docs/roadmap/`** since 2026-09-12,
by author instruction: this file is the chain and the status, and nothing else.
`verify.md` is how to check the project from a cold checkout, `production-ready.md`
what that phrase means here and who owns each part, `milestones.md` the reasoning
a milestone's own row cannot hold, `decisions.md` what this file has decided
about itself, and `names.md` the map from every retired identifier to the one in
use. Each keeps its own heading, so a citation repoints by file rather than by
searching.
