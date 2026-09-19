# ROADMAP — where the project is, and what is next

The status and the chain. Why the table looks the way it does is
`docs/roadmap/shape.md`; who scheduled each row and what ratified it is
`docs/roadmap/scheduling.md`.

**Open: 3 defects · 2 decisions.** They live in `docs/work/DEFECTS.md` and
`docs/work/DECIDE.md`, whose `**OPEN:**` banners are what this line must equal.
A preamble, these two counts and the tables are the whole of this file — author
instruction 2026-09-16, `.claude/rules/records.md` § A live list is a preamble,
a count and its items — and the counts get an instrument rather than a
convention because on the day the rule was given this line read *open defects 0*
against a banner reading 3.

## Where we are

| | |
|---|---|
| **Current milestone** | **M-declared-extents**, row 61, opened 2026-09-18 ([its file](../work/milestones/M-declared-extents.md)) — panel 164's route 6, the one row scheduled behind a MEASUREMENT rather than a decision. Step 1 ran it: `docs/measurements/035-not-one-byte-array-is-spelled-the-same-way-on-both.md`. **This cell named `M-core-packages` as the next row until 2026-09-18**, which is row 63; row 61 has been `M-declared-extents` since M-readable-bytes's close entered it |
| **Last closed** | **M-readable-bytes**, 2026-09-18, `m-readable-bytes` ([059](journal/059-readable-bytes.md)) — a C byte field becomes text, and the second wall it was opened on turned out not to exist · before it **M-arm-platform** ([058](journal/058-arm-platform.md)), the fourth real machine · v1 **reached** at M-selfhost-fixpoint, 2026-08-18 |
| Milestones closed | **60** of 81 · **60** tags matching `m*`, the legacy `m0`-`m8` included |
| The compiler | **63,035** lines of Heroes in **219** modules (`find selfhost -name '*.hero'`) · the seed **831,603** lines of C, regenerated twice here and the fixpoint verified byte-identical each time — the second regeneration was not optional, because a COMMENT shifts the `#line` directives the emitted C carries |
| The platforms | **four**, since 2026-09-18: Linux x86-64, **Linux arm64**, Darwin arm64, Windows x86-64. The two Linux legs are one axis apart by construction — same Debian, same clang, same libc — so a divergence between them has one candidate cause. Windows is the one the author starts by hand |
| The spec | **6089** on the vendored ranks and **8106** on the reader's own, against a ceiling of **10240**. Panel 164's row is **+48 vendored and +66 real**, the closest any row has run to the 50-token `DELTA_GATE`, and it is paid by panel 162's unspent **−6** removal and five registered predictions. **2134 free** and 2074 net of the FFI floor |
| The contract | **7449** on the reader's own against a ceiling of **12288**, judged on `claude-opus-5`, refreshed 2026-09-15 with the wakeup instruction: of the 36 vendored tokens the pin was behind, 14 predate that session and no commit had named them |
| Records | sittings **162** · journals **61** · milestone files **50** · measurements **35** · entries: `docs/records/log/` **681**, `docs/records/done/` **560**, `docs/records/book/beats/` **127** |
| Waiting on the author | **nothing.** Panels 155 to 159 were ratified on 2026-09-16 and **panel 161 on 2026-09-18**, all of them **by delegation and not by reading**, under instructions of those days, and every file says so in those words rather than crediting a reading that did not happen. What each leaves owed is named with its trigger: panel 155's `float_map_key` through a generic, which waits under Principle 0; panel 156's two refused spec merges; panel 157's R4 instrument, filed with its price in `docs/work/milestones/M-package-manager.md`; panel 159's refused route, defining `<` on `str`; and panel 161's two unpriced routes. **Panel 164 was ratified the same way on 2026-09-18**, and the gap panel 161 found — that no route leads from a `char[N]` field to C — is closed in both directions; what it leaves owed is its route 6, scheduled as **M-declared-extents** behind the one measurement that decides it |

**Re-measured 2026-09-18, the full net on a compiler built from the regenerated
seed, which is CI's own configuration.** `check` **127**, `run` **127**,
`emission` **462**, `determinism` **156**, `descriptors` **218**, `surface`
**109**, `warnings` **187**, `lines` **128**, `annotations` **166**, `records`
**24**, `spec` **20**, `corpus` **55**, `canonical` **2**, `layout` **2**,
`runtime` **8**, `fixes` **25**, `unsupported` **15**, the compiler's own **659**
and the net's own **158**. **The net reads 1891 passed, 0 failed.**

**The fourth leg was green in CI on its first push**, all four jobs, which
settles the two things M-arm-platform wrote down as unrun: the
`ubuntu-24.04-arm` label is accepted for this repository, and the leg passes the
whole net rather than only the container at home.

---

## The chain

One table, one row per milestone, **closed first and scheduled after**: rows 1–60
are done, in the order they closed, and rows 61–81 are what is next, in the order
they will be taken. **This sentence said 1–37 and 38–59
until 2026-09-10**, 1–47 and 48–71 until 2026-09-12, and 1–51 and 52–76 until
2026-09-13, when two closed rows were found parked at 75 and 76 behind the
scheduled block, then 1–53 and 54–76 later that same day, when
`M-handle-verdict` entered at 55 and the twenty-two rows after it each moved one
down, and 1–53 and 54–77 until M-deferral-ledger closed that evening, and 1–55
and 56–77 until M-cleanup-verdict closed on 2026-09-14 — a close that both ticked
its own row and entered `M-marked-acquisition` at 57, the form its own verdict
admitted, so the twenty-one rows after it each moved one down and the table is
78 long, and 1–56 and 57–78 until M-marked-acquisition closed later the same
day, which is the first close in this table that opened no row after itself, and
1–57 and 58–78 until M-check-completeness closed on 2026-09-17. **That close did
not move it, and the OPENING of row 59 later the same day is what found it**: the
sentence read *1–57 done* over a table whose row 58 said `done 2026-09-17` eight
lines below. Then 1–58 and 59–78 until M-arm-platform closed on 2026-09-18, and
1–59 and 60–79 when `M-readable-bytes` entered at 60 the same day, on author
instruction and on panel 161's largest unresolved finding, so the nineteen rows
after it each moved one down and the table was 79 long, and 1–60 and 61–79
when M-readable-bytes closed later that same day, the second close in this table
that opened no row after itself. **That last clause was false as it was written,
and the OPENING of row 61 on 2026-09-18 found it**: the same close entered
`M-declared-extents` at 61 and `M-buildable-structs` at 62, both out of panel
164, so it opened two rows rather than none, **the table is 81 long**, and the
scheduled block is 61–81. The sentence restated a length it had not recounted,
which is the one thing this paragraph exists to prevent. It is the
one number in this file that no instrument reads, so it is
restated here and re-read at every close with § Where we are — and at every
OPENING too, which is what moved it twice on one day, what caught it at row 59,
and what caught it again here. `warrant` is why a milestone exists: **v1** (the self-hosting
finish line), **closure list** (design.md §1.0 — the compiler needs it), **§1.1**
(comprehension is the objective), or **scheduled, no warrant**.

**The cells hold no prose.** Who scheduled a milestone, which panel ratified it,
and why one overtook another are in `docs/roadmap/scheduling.md`.

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
| 42 | **M-held-bytes** | done 2026-09-09 | `m-held-bytes` | [041](journal/041-held-bytes.md) | §4.19's FOURTH case: `x: cstr @ s.lease()` is a COPY C may read until `end_lease(@x)`. Two sittings, four refused rules, one capability; defect 024 CLOSED with its limit named, and the first tag since 2026-09-08 because the list is clean · **§1.12**
| 43 | **M-interpolation-verdict** | done 2026-09-09 | `m-interpolation-verdict` | [042](journal/042-interpolation-verdict.md) | the ruling on design.md Part 7 item 7: string interpolation ENTERS as `f"line {n}: {word}"`, three counts and one sitting, the spelling the author's; closed with the verdict and not the form, as it was named to · **§1.1** |
| 44 | **M-interpolated-strings** | done 2026-09-09 | `m-interpolated-strings` | [043](journal/043-interpolated-strings.md) | `f"line {n}: {word}"`: one token kind, one AST node, three helper modules, four decided ceilings moved by their measured lines, the spec at +138 real with R6's two sentences, and `heroes mutate` reaching the holes · **§1.1**
| 45 | **M-anchored-spec** | done 2026-09-11 | `m-anchored-spec` | [044](journal/044-anchored-spec.md) | the specification takes the shape of a Report: thirteen numbered sections in reference order, one home per rule, the numbers as the citation anchors, two checks that keep the form, 81 citations converted, and the site's page copying the text in one click · **§1.6** |
| 46 | **M-labelled-builtins** | done 2026-09-11 | `m-labelled-builtins` | [045](journal/045-labelled-builtins.md) | defect 025 repaired: the same-typed-argument rule reaches the two built-ins it did not, both halves of it, 1022 call sites relabelled, and the classic argument inversion drops from 367 survivors to 75 with the swapped `fail(` at zero · **§1.2** |
| 47 | **M-positional-values** | done 2026-09-11 | `m-positional-values` | [046](journal/046-positional-values.md) | defect 026 narrowed: a label at a call through a function value is refused, the note stops prescribing the hole in silence, and `spec § 3` says the call is positional — a sentence vetoed as false at the ballot that the refusal made true · **§1.2** |
| 48 | **M-labelled-types** | done 2026-09-11 | `m-labelled-types` | [047](journal/047-labelled-types.md) | defect 026 repaired: a function type names the parameters that can be confused, the names are part of its identity, and the classic inversion through a function value goes from 17% to 100% — at 432 lines against a condition of about 150, which is the sitting's own headline · **§1.2** |
| 49 | **M-named-callbacks** | done 2026-09-11 | `m-named-callbacks` | [048](journal/048-named-callbacks.md) | the role inversion through a generic callback, closed at 7 of 7 against a cheaper reading's 2 of 7: `fold`'s type names its accumulator and its element, and a function handed to it names them the same way — a naming mandate the author chose over the sitting's own recommendation, with both numbers in front of them · **§1.2** |
| 50 | **M-rotated-records** | done 2026-09-12 | `m-rotated-records` | [049](journal/049-rotated-records.md) | the records become directories, one entry per file and one milestone per file, and the file that held each stays behind as a map so every citation still resolves · CLAUDE.md §14 |
| 51 | **M-stated-grammar** | done 2026-09-12 | `m-stated-grammar` | [050](journal/050-stated-grammar.md) | every syntactic form stated once, in Wirth's notation, inside the specification beside the prose that governs it, and `heroes grammar` for the half the compiler can print from its own tables · **§1.1** |
| 52 | **M-reflection-verdict** | done 2026-09-12 | `m-reflection-verdict` | [051](journal/051-reflection-verdict.md) | reflection refused at run time and as a general tag; a field's name becomes a name the checker resolves · **scheduled, no warrant** |
| 53 | **M-declared-thresholds** | done 2026-09-12 | `m-declared-thresholds` | [052](journal/052-declared-thresholds.md) | every ceiling and floor says which way it may move, and something notices when it should have moved and did not · **§1.1** |
| 54 | **M-deferral-ledger** | done 2026-09-13 | `m-deferral-ledger`, taken 2026-09-13 once all three defects closed | [053](journal/053-deferral-ledger.md) | every Part 7 item with no milestone gets a dated verdict or a return condition, and Part 8's homeless warts with it · nine sittings, and **nine times the item's stated reason was measured false or expired before its verdict could be written** · five refused to Part 6 with falsifiers, two deferred again with conditions somebody can check · coverage refused as Part 6's **first row whose subject is a tool** · wart 8 rewritten because the cost is in the **spelling**: the same push is 0.02 s at a million through a plain name or a lent field, and 11.51 s at fifty thousand through a field in place |
| 55 | **M-handle-verdict** | done 2026-09-14 | `m-handle-verdict` | [054](journal/054-handle-verdict.md) | the ruling on telling one C handle from another, and three defects that were one question · **§1.12**, defects 029, 030, 031 |
| 56 | **M-cleanup-verdict** | done 2026-09-14 | `m-cleanup-verdict` | [055](journal/055-cleanup-verdict.md) | the ruling on a scope-bound release for a C handle: a releaser keyed on the TYPE refused to Part 6, because ownership is a property of the CALL · a decision, not a feature |
| 57 | **M-marked-acquisition** | done 2026-09-14 | `m-marked-acquisition` | [056](journal/056-marked-acquisition.md) | the mark goes where the obligation is CREATED — on the acquiring call — so a C handle nobody releases stops being silent · panel 147 R4, **§1.12** |
| 58 | **M-check-completeness** | done 2026-09-17 | `m-check-completeness` | [057](journal/057-check-completeness.md) | ten defects the hunt turned up, closed — and three were not what their own entry said · scheduled, no warrant |
| 59 | **M-arm-platform** | done 2026-09-18 | `m-arm-platform` | [058](journal/058-arm-platform.md) | the fourth real machine: arm64 Linux, where a container runs and where `char` is unsigned · **§1.12** |
| 60 | **M-readable-bytes** | done 2026-09-18 | `m-readable-bytes` | [059](journal/059-readable-bytes.md) | a C byte buffer becomes text a program can print: the inbound direction `str` has never had · **§1.11** |
| 61 | **M-declared-extents** | **OPEN** | — | — | panel 164's route 6: a C header spells a parameter as an array, `function arr_len(s: i8[8])`, and it is the only route where the compiler CHECKS the extent instead of trusting the author or the callee. Its whole value is a number nobody has measured — how many real headers spell a parameter that way |
| 62 | **M-buildable-structs** | scheduled | — | — | a real five-field `utsname` needs 1280 literal zeros, 4312 bytes for a nine-line program, measured and RUN at panel 164. Not a defect, because nothing is broken: what is missing is a way to write *the rest are zero* |
| 63 | **M-core-packages** | scheduled | — | — | small packages that compose, organised as Go's tree, in Heroes or over C |
| 64 | **M-package-manager** | scheduled | — | — | `heroes add`/`heroes fetch`; bindings instead of a standard library |
| 65 | **M-web-framework** | scheduled | — | — | composes the core packages, Go/Echo style: explicit routes, records, no magic |
| 66 | **M-doc-generator** | scheduled | — | — | `heroes doc`, the one direction Part 6's literate-source row promises · scheduled, no warrant |
| 67 | **M-panic-location** | scheduled | — | — | a panic names the `.hero` file, line and function · **§1.12** |
| 68 | **M-typed-inspection** | scheduled | — | — | a stopped program shows Heroes values: the name the author typed, and `[T]`, `{K: V}`, `T?`, a variant and a record shown as themselves · scheduled, no warrant |
| 69 | **M-generated-programs** | scheduled | — | — | programs nobody wrote: a generator that composes valid Heroes and knows the answer before the compiler is asked · **§1.12** |
| 70 | **M-thesis-harness** | scheduled | — | — | Part 11's metrics 2 and 4 run for the first time, as a Heroes program · **§1.1** |
| 71 | **M-lsp-server** | scheduled | — | — | `heroes lsp`, and the incremental frontend it needs |
| 72 | **M-vscode-extension** | scheduled | — | — | the extension, complete |
| 73 | **M-deployable-binary** | scheduled | — | — | what the machine that RUNS a Heroes program needs, on the three platforms, and which `-O` a shipped artifact carries |
| 74 | **M-install-channels** | scheduled | — | — | a Homebrew tap, winget, a Nix flake, a Docker image, all built from the seed and pinned to a `v*` release tag; the version scheme itself was decided ahead, 2026-09-07 |
| 75 | **M-online-compiler** | scheduled | — | — | the compiler reached without installing anything: the site's visitor writes Heroes and gets its answer · scheduled, no warrant |
| 76 | **M-compatibility-promise** | scheduled | — | — | the paragraph `1.0.0` rests on, and the suite that makes a broken promise red · CLAUDE.md §14 |
| 77 | **M-publication-gate** | scheduled | — | — | the last gate before anything goes outward · CLAUDE.md §14 |
| 78 | **M-microcontroller-verdict** | scheduled | — | — | the ruling on a Heroes program running on a microcontroller under an RTOS, RISC-V first; the two facts a 32-bit build refuses today are its brief · **§1.12**, Part 2 |
| 79 | **M-qbe-backend** | scheduled | — | — | Part 7 item 15 — the proof that the IR is not C in disguise |
| 80 | **M-journey-book** | scheduled | — | — | the journey — how this language came to be |
| 81 | **M-guide-book** | scheduled | — | — | the guide, as a book you would find in a shop · **§1.1** |

Three closed milestones have no tag of their own because they were parents or
sub-steps: **M-checker-core**, **M-data-declarations** and **M-rich-diagnostics**
landed inside `m3`, and **M-native-backend** was the parent of the four backend
milestones and was never tagged. § The names carries them.

`git tag --list --sort=creatordate` gives the same chronology from git itself.
