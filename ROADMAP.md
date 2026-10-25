# ROADMAP — where the project is, and what is next

The status and the chain. Why the table looks the way it does is
`docs/roadmap/shape.md`; who scheduled each row and what ratified it is
`docs/roadmap/scheduling.md`.

**The open issues are GitHub's**, https://github.com/heroes-lang/heroes/issues,
and this file counts none of them, GitHub's counts being the truth (the
author's answers of 2026-10-12). A preamble and the tables are the whole of it
(author instruction 2026-09-16, `.claude/rules/records.md` § A live list is a
preamble and its items).

## Where we are

| | |
|---|---|
| **Current milestone** | none `**OPEN**` since M-rechecked-issues closed on 2026-10-25; row 72, **M-panic-location**, stays `scheduled` until somebody opens it |
| **Next** | M-panic-location, row 72: a panic names the `.hero` file, line and function |
| **Last closed** | **M-rechecked-issues**, 2026-10-25 ([070](docs/records/journal/070-rechecked-issues.md)): every issue open at its start run again against the tree and given a dated comment, 15 of the 57 closed with the commit or the reason, 3 re-homed; the formatter's probe, which could not end (defect 2139), ends: the formatter asked a chain's every part again at every level above it, and asking each once, panel 229's R1, the seed it named was judged whole, 84,027 variants held; panel 228's hole in every `check --json` document, ratified, and panel 229's run split across processes, provisional · before it **M-thesis-harness**, 2026-10-24 ([069](docs/records/journal/069-thesis-harness.md)): metric 2 ran for the first time, on forty held-out tasks the author wrote and sealed; the primary read +0.0 points with no discordant pair, inconclusive and published so ([052](docs/measurements/052-the-first-held-out-run-the-primary-inconclusive-at-zero-points-with-no-discordant-pair-the-strict-arm-paying-at-turn-one-and-level-by-turn-three-and-no-model-writing-a-variant.md)) · before it **M-witnessed-seed** ([068](docs/records/journal/068-witnessed-seed.md)) · v1 **reached** at M-selfhost-fixpoint, 2026-08-18 |
| Milestones closed | **71** of 91 · **71** tags matching `m*`, the legacy `m0`-`m8` included; `m-rechecked-issues` placed 2026-10-25 over a clean list, no `blocking` or `systemic` defect, panel 229's ratification the one decision open ([070](docs/records/journal/070-rechecked-issues.md)) |
| The compiler | **177,905** lines of Heroes in **684** modules (`find selfhost -name '*.hero'`) · the seed **271,387** lines of C, 27,000,911 bytes, refreshed by the release `v0.5.0` on 2026-10-25 (`63137e422`, its fixpoint over two generations) and before it unchanged from `m-witnessed-seed` at 263,383 lines and 26,240,524 bytes, in panel 219's s4 shape, refreshed only when the two-stage build goes red and at each release (M-bounded-seed's rhythm), the compiler built in two stages and stage 2 judging |
| The platforms | **four**, since 2026-09-18: Linux x86-64, **Linux arm64**, Darwin arm64, Windows x86-64. The two Linux legs are one axis apart by construction — same Debian, same clang, same libc — so a divergence between them has one candidate cause. On both, gcc builds the seed too, and the compiler it makes must emit the C the compiler clang makes emits (M-witnessed-seed). Windows is the one the author starts by hand |
| The spec | **7854** on the vendored ranks and **10,297** on the reader's own (`heroes measure spec/heroes-spec.md`, 2026-10-24), against the ceiling below, the claim in the opening line giving 2 back that day; until then **7856** and **10,299** (2026-10-21), panel 227's built-ins sentence spending 13 and the consolidation block 251 of its 1,500, and before them **7846** and **10,286** (2026-10-18), and before them **7703** and **10,108** (2026-10-17), and before them **7655** and **10,048**, against a ceiling of **16384**, raised from 10240 by author decision on 2026-10-14 with a budget of 1,500 for the consolidation block (`heroes measure spec/heroes-spec.md`, 2026-10-13). Panel 210's landing in batch 21 spent the 20 above M-inferred-cell's 10,024: an `@` parameter nothing writes or ends refused, and `@_` refused; the claim in the opening line, by author instruction, spent 6 more on 2026-10-12, gave 2 back on 2026-10-13, when it named LLMs, and 2 more on 2026-10-24, when it became *a language for the age of agents* |
| The contract | **8215** on the reader's own against a ceiling of **12288**, judged on `claude-opus-5`, refreshed 2026-10-25; **6201** on the vendored ranks, 2026-10-25 |
| Records | sittings **227** · journals **71**, numbered files only · milestone pages **61** in `docs/roadmap/milestones/` · measurements **51**, the documents alone · issues on GitHub, counted there since 2026-10-12 · questions **502** in `docs/learn/`, 477 open, not issues since 2026-10-05 · `docs/records/book/beats/` **138** |
| Waiting on the author | the open decision issues on GitHub, the `kind:decision` label; CodeQL's default setup switched off by the author on 2026-10-12 and replaced by `.github/workflows/codeql.yml` (defect 674) |

**Re-measured 2026-10-25 at `m-rechecked-issues`'s tag**: the full net on stage
2 over the merged lane, 32 suites, 8,974 passed and 0 failed, the two-stage
build's stage 2 and stage 3 emitting 27,000,911 identical bytes, the compiler's
own 1,750 and the net's own 425, the seed not refreshed; the two emissions this
Mac skips blessed on Linux arm64. The account is
[070](docs/records/journal/070-rechecked-issues.md); M-thesis-harness's close is
[069](docs/records/journal/069-thesis-harness.md).

**The by-hand probe `.claude/rules/verification.md` owes where `selfhost/print/`
moved runs after the push, beside the CI, as § The optimistic chain says since
2026-10-09 and panel 229's R4 restates**; the run after M-rechecked-issues' push
judged defect 2139's seed whole, and the rest of it is written on that defect's
issue when it ends.

---

## The chain

One table, one row per milestone, **closed first and scheduled after**: rows 1–70
are done, in the order they closed, row 71 is open, and rows 72–91 are
what is next, in the order they will be taken. **This sentence said 1–37 and 38–59
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
which is the one thing this paragraph exists to prevent. Then 1–60 and 61–81
until 2026-09-23: M-declared-extents's close ticked row 61 and entered
`M-agreed-retention` at 62, so **the table is 82 long** and the scheduled block
is 62–82, and that close restated neither this sentence nor *61 of 81* in § Where
we are; the OPENING of row 62 found both. Then 1–61 and 62–82 until M-agreed-retention
closed on 2026-09-28 and opened no row, the table still 82 long, counted at that
close. Then 1–62 and 63–82 until 2026-10-12, false from 2026-10-04: M-issue-files
opened at 83 behind the scheduled block that day, and M-inferred-cell entered at
84 and closed there on 2026-10-10, two rows past a sentence that named 82 which
neither close restated. **The OPENING of M-typed-inspection on 2026-10-12 found it**,
and by author instruction re-ordered the table closed rows first and renumbered
it: the two late closes take 63 and 64 in the order they closed, M-typed-inspection
65, and the nineteen scheduled rows 66–84, the table 84 long by a count over it. It is the
one number in this file that no instrument reads, so it is
restated here and re-read at every close with § Where we are — and at every
OPENING too, which is what moved it twice on one day, what caught it at row 59,
and what caught it again here. Then 1–64, 65 open and 66–84 until
M-typed-inspection closed on 2026-10-13, opening no row and restating neither;
**the OPENING of M-buildable-structs on 2026-10-14 found it**, row 66 open and
the table still 84 long by a count over it. Then 1–65, 66 open and 67–84 until
later the same day, when, by the author's instruction, M-bounded-seed opened at
67, five more rows entered (M-witnessed-seed at 68, M-rechecked-issues at 70,
M-latency-ceiling at 74, M-compared-languages at 79 and M-launch-posts at 86)
and the eighteen scheduled rows took a new order, consolidation first: the
table is 90 long by a count over it. Then 1–65, 66 and 67 open and 68–90 until
M-buildable-structs closed on 2026-10-17, opening no row. Then 1–66, 67 open and
68–90 until M-bounded-seed closed the same day, opening no row. **That close
restated the history and not this paragraph's first sentence, and the ENTRY of
row 68 on 2026-10-18 found it**, still reading *1–66 done, 67 open*. Then 1–67
and 68–90 until that day, when M-readonly-lend entered at 68 by panel 217's R6,
ratified the evening before, and the twenty-three scheduled rows after it each
moved one down: the table is 91 long by a count over it. Then 1–67 and 68–91
until later that day, when M-readonly-lend opened at 68, opening no other row. Then
1–67 done and 68 open until it closed that night, opening no other row, and
1–68 done and 69–91 until M-witnessed-seed opened at 69 the next morning,
opening no other row; then 1–68 done, 69 open and 70–91 until it closed that
night, opening no other row, and 1–69 done and 70–91 until M-thesis-harness
opened at 70 on 2026-10-20, opening no other row; then 1–69 done, 70 open
and 71–91 until it closed on 2026-10-24, opening no other row and restating
neither this sentence nor § Where we are's *Next*. **The OPENING of
M-rechecked-issues on 2026-10-25 found it**, the table 91 long by a count over
it, 70 rows `done` and 21 `scheduled` before it opened at 71. `warrant` is why a milestone exists: **v1** (the self-hosting
finish line), **closure list** (design.md §1.0 — the compiler needs it), **§1.1**
(comprehension is the objective), or **scheduled, no warrant**.

**The cells hold no prose.** Who scheduled a milestone, which panel ratified it,
and why one overtook another are in `docs/roadmap/scheduling.md`.

| # | milestone | state | tag | journal | what it delivers · warrant |
|---|---|---|---|---|---|
| 1 | **M-day-zero** | done 2026-08-03 | `m0` | [000](docs/records/journal/000-setup.md) | git, workspace, four hand-written C targets |
| 2 | **M-token-stream** | done 2026-08-04 | `m1` | [001](docs/records/journal/001-lexer.md) | the lexer, and `--dump-tokens` |
| 3 | **M-syntax-tree** | done 2026-08-04 | `m2` | [002](docs/records/journal/002-parser.md) | parser, AST, the canonical formatter |
| 4 | **M-name-resolution** | done 2026-08-04 | `m3a` | [003](docs/records/journal/003-resolver.md) | scopes, no shadowing, order-free top level |
| 5 | **M-typed-frontend** | done 2026-08-04 | `m3` | [004](docs/records/journal/004-checker.md) | the frontend complete: names, types, errors as a product |
| 6 | **M-ir-lowering** | done 2026-08-04 | `m4` | [005](docs/records/journal/005-lowering.md) | desugaring, the three-address IR with basic blocks |
| 7 | **M-scalars-run** | done 2026-08-04 | `m5a` | [006](docs/records/journal/006-scalars-run.md) | the first native binary |
| 8 | **M-strings-ownership** | done 2026-08-05 | `m5b` | [007](docs/records/journal/007-strings-and-ownership.md) | `str` in C, and the ownership pass |
| 9 | **M-value-aggregates** | done 2026-08-10 | `m5c` | [008](docs/records/journal/008-aggregates.md) | records and variants by value, copy-on-write |
| 10 | **M-optional-map** | done 2026-08-10 | `m5d` | [009](docs/records/journal/009-the-map-and-the-fallible.md) | `T?` and `{K: V}` — the last two types |
| 11 | **M-generics-library** | done 2026-08-11 | `m6` | [010](docs/records/journal/010-sugar-tests-generics-library.md) | sugar, `heroes test`, generics, the library |
| 12 | **M-module-namespace** | done 2026-08-12 | `m8a` | [011](docs/records/journal/011-modules-the-namespace.md) | a program is many files |
| 13 | **M-ffi-ladder** | done 2026-08-12 | `m-ffi-ladder` | [012](docs/records/journal/012-the-ffi-ladder.md) | Heroes calls C; SQLite with no shim |
| 14 | **M-header-constants** | done 2026-08-12 | `m-header-constants` | [013](docs/records/journal/013-header-constants.md) | the number leaves the file |
| 15 | **M-literal-bases** | done 2026-08-12 | `m-literal-bases` | [014](docs/records/journal/014-literal-bases.md) | `0x` `0o` `0b`, the `_` separator |
| 16 | **M-sized-integers** | done 2026-08-12 | `m-sized-integers` | [015](docs/records/journal/015-sized-integers.md) | eight widths; `int` stops being a word |
| 17 | **M-program-corpus** | done 2026-08-13 | `m-program-corpus` | [016](docs/records/journal/016-the-program-corpus.md) | nine programs, six compiler defects, a CI |
| 18 | **M-binding-fidelity** | done 2026-08-14 | `m-binding-fidelity` | [017](docs/records/journal/017-binding-fidelity.md) | what an `extern` accepts |
| 19 | **M-struct-passing** | done 2026-08-15 | `m-struct-passing` | [018](docs/records/journal/018-struct-passing.md) | a struct crosses by value |
| 20 | **M-complete-structs** | done 2026-08-15 | `m-complete-structs` | [019](docs/records/journal/019-complete-structs.md) | every field form a C header can write |
| 21 | **M-selfhost-probe** | done 2026-08-15 | `m-selfhost-probe` | [020](docs/records/journal/020-selfhost-probe.md) | the lexer ported, to measure what was missing |
| 22 | **M-selfhost-port** | done 2026-08-17 | `m-selfhost-port` | [021](docs/records/journal/021-selfhost-port.md) | the compiler twice over, and one hash · **v1** |
| 23 | **M-selfhost-fixpoint** | done 2026-08-18 | `m-selfhost-fixpoint` | [022](docs/records/journal/022-selfhost-fixpoint.md) | the fixpoint and the seed · **v1** |
| 24 | **M-harness-port** | done 2026-08-18 | `m-harness-port` | [023](docs/records/journal/023-harness-port.md) | the net in Heroes · closure list |
| 25 | **M-bootstrap-archive** | done 2026-08-19 | `m-bootstrap-archive` | [024](docs/records/journal/024-bootstrap-archive.md) | the third language dies · v1's last clause (design.md:82) |
| 26 | **M-separate-compilation** | done 2026-08-26 | `m-separate-compilation` | [025](docs/records/journal/025-separate-compilation.md) | one `.c` per module, prototypes across TUs, the cache · closure list |
| 27 | **M-argv-execution** | done 2026-08-31 | `m-argv-execution` | [026](docs/records/journal/026-argv-execution.md) | the compiler runs programs by argument list, and the shell stops being the boundary |
| 28 | **M-package-layout** | done 2026-09-02 | `m-package-layout` | [027](docs/records/journal/027-package-layout.md) | `use` paths, the qualifier, `as`, and where a program's files live |
| 29 | **M-selfhost-nesting** | done 2026-09-02 | `m-selfhost-nesting` | [028](docs/records/journal/028-selfhost-nesting.md) | the compiler's own modules move into ten directories |
| 30 | **M-corpus-coverage** | done 2026-09-02 | `m-corpus-coverage` | [029](docs/records/journal/029-corpus-coverage.md) | every language form has a program that runs it |
| 31 | **M-documentation-site** | done 2026-09-02 | `m-documentation-site` | [030](docs/records/journal/030-documentation-site.md) | the site, anchored to programs that run |
| 32 | **M-robustness-guards** | done 2026-09-03 | `m-robustness-guards` | [031](docs/records/journal/031-robustness-guards.md) | the guards that shut the holes §1.12 named: `@` on an immutable, the stack, the C pointer verdict, the harness scratch · §1.12 |
| 33 | **M-corpus-depth** | done 2026-09-04 | `m-corpus-depth` | [032](docs/records/journal/032-corpus-depth.md) | the rung between a program and the compiler: nine programs, and half of every frame · **§1.1** |
| 34 | **M-c-callbacks** | done 2026-09-05 | `m-c-callbacks` | [033](docs/records/journal/033-c-callbacks.md) | a Heroes function reaches a C callback parameter, and a foreign thread is refused by name rather than left to corrupt · **§1.11**, **§1.12** |
| 35 | **M-isolated-threads** | done 2026-09-06 | `m-isolated-threads` | [034](docs/records/journal/034-isolated-threads.md) | Part 7.13 concurrency: three of four measured corruption classes closed, and the door was in the checker |
| 36 | **M-thread-stacks** | done 2026-09-06 | `m-thread-stacks` | [035](docs/records/journal/035-thread-stacks.md) | the guard speaks on every thread, and a worker's floor is the thread that ran `main` · **§1.12**
| 37 | **M-declared-freer** | done 2026-09-07 | `m-declared-freer` | [036](docs/records/journal/036-declared-freer.md) | `owned <C function>`: the string C hands you is freed by the name its own declaration gives · **§1.12**
| 38 | **M-open-repository** | done 2026-09-08 | `m-open-repository` | [037](docs/records/journal/037-open-repository.md) | the repository opens, and every page that says it is shut stops saying so · CLAUDE.md §14
| 39 | **M-discard-refusal** | done 2026-09-08 | `m-discard-refusal` | [038](docs/records/journal/038-discard-refusal.md) | `_ =` on a fallible value becomes a compile error, in two positions and not three · **§1.1**
| 40 | **M-closures-verdict** | done 2026-09-08 | `m-closures-verdict` | [039](docs/records/journal/039-closures-verdict.md) | the ruling on Part 7 items 1 and 12: both refused, closures to Part 6 and inline blocks left unplaced · **§1.1**
| 41 | **M-cstr-lifetime** | done 2026-09-09 | `m-cstr-lifetime` | [040](docs/records/journal/040-cstr-lifetime.md) | a `cstr` may not outlive the `str` it was lent from: three positional clauses, 0 of 638 files moved. **The tag came later than the close, at the author's word on 2026-09-09**: this milestone shipped with defect 024 open by panel 122 R5's design, the lend C RETAINS, and `records/tagged` reads the newest tag's own commit for clean lists, so the tag stands on the commit that ratified panel 125, the first where every list this milestone left open is clean; the tag's own message says so · **§1.12**
| 42 | **M-held-bytes** | done 2026-09-09 | `m-held-bytes` | [041](docs/records/journal/041-held-bytes.md) | §4.19's FOURTH case: `x: cstr @ s.lease()` is a COPY C may read until `end_lease(@x)`. Two sittings, four refused rules, one capability; defect 024 CLOSED with its limit named, and the first tag since 2026-09-08 because the list is clean · **§1.12**
| 43 | **M-interpolation-verdict** | done 2026-09-09 | `m-interpolation-verdict` | [042](docs/records/journal/042-interpolation-verdict.md) | the ruling on design.md Part 7 item 7: string interpolation ENTERS as `f"line {n}: {word}"`, three counts and one sitting, the spelling the author's; closed with the verdict and not the form, as it was named to · **§1.1** |
| 44 | **M-interpolated-strings** | done 2026-09-09 | `m-interpolated-strings` | [043](docs/records/journal/043-interpolated-strings.md) | `f"line {n}: {word}"`: one token kind, one AST node, three helper modules, four decided ceilings moved by their measured lines, the spec at +138 real with R6's two sentences, and `heroes mutate` reaching the holes · **§1.1**
| 45 | **M-anchored-spec** | done 2026-09-11 | `m-anchored-spec` | [044](docs/records/journal/044-anchored-spec.md) | the specification takes the shape of a Report: thirteen numbered sections in reference order, one home per rule, the numbers as the citation anchors, two checks that keep the form, 81 citations converted, and the site's page copying the text in one click · **§1.6** |
| 46 | **M-labelled-builtins** | done 2026-09-11 | `m-labelled-builtins` | [045](docs/records/journal/045-labelled-builtins.md) | defect 025 repaired: the same-typed-argument rule reaches the two built-ins it did not, both halves of it, 1022 call sites relabelled, and the classic argument inversion drops from 367 survivors to 75 with the swapped `fail(` at zero · **§1.2** |
| 47 | **M-positional-values** | done 2026-09-11 | `m-positional-values` | [046](docs/records/journal/046-positional-values.md) | defect 026 narrowed: a label at a call through a function value is refused, the note stops prescribing the hole in silence, and `spec § 3` says the call is positional — a sentence vetoed as false at the ballot that the refusal made true · **§1.2** |
| 48 | **M-labelled-types** | done 2026-09-11 | `m-labelled-types` | [047](docs/records/journal/047-labelled-types.md) | defect 026 repaired: a function type names the parameters that can be confused, the names are part of its identity, and the classic inversion through a function value goes from 17% to 100% — at 432 lines against a condition of about 150, which is the sitting's own headline · **§1.2** |
| 49 | **M-named-callbacks** | done 2026-09-11 | `m-named-callbacks` | [048](docs/records/journal/048-named-callbacks.md) | the role inversion through a generic callback, closed at 7 of 7 against a cheaper reading's 2 of 7: `fold`'s type names its accumulator and its element, and a function handed to it names them the same way — a naming mandate the author chose over the sitting's own recommendation, with both numbers in front of them · **§1.2** |
| 50 | **M-rotated-records** | done 2026-09-12 | `m-rotated-records` | [049](docs/records/journal/049-rotated-records.md) | the records become directories, one entry per file and one milestone per file, and the file that held each stays behind as a map so every citation still resolves · CLAUDE.md §14 |
| 51 | **M-stated-grammar** | done 2026-09-12 | `m-stated-grammar` | [050](docs/records/journal/050-stated-grammar.md) | every syntactic form stated once, in Wirth's notation, inside the specification beside the prose that governs it, and `heroes grammar` for the half the compiler can print from its own tables · **§1.1** |
| 52 | **M-reflection-verdict** | done 2026-09-12 | `m-reflection-verdict` | [051](docs/records/journal/051-reflection-verdict.md) | reflection refused at run time and as a general tag; a field's name becomes a name the checker resolves · **scheduled, no warrant** |
| 53 | **M-declared-thresholds** | done 2026-09-12 | `m-declared-thresholds` | [052](docs/records/journal/052-declared-thresholds.md) | every ceiling and floor says which way it may move, and something notices when it should have moved and did not · **§1.1** |
| 54 | **M-deferral-ledger** | done 2026-09-13 | `m-deferral-ledger`, taken 2026-09-13 once all three defects closed | [053](docs/records/journal/053-deferral-ledger.md) | every Part 7 item with no milestone gets a dated verdict or a return condition, and Part 8's homeless warts with it · nine sittings, and **nine times the item's stated reason was measured false or expired before its verdict could be written** · five refused to Part 6 with falsifiers, two deferred again with conditions somebody can check · coverage refused as Part 6's **first row whose subject is a tool** · wart 8 rewritten because the cost is in the **spelling**: the same push is 0.02 s at a million through a plain name or a lent field, and 11.51 s at fifty thousand through a field in place |
| 55 | **M-handle-verdict** | done 2026-09-14 | `m-handle-verdict` | [054](docs/records/journal/054-handle-verdict.md) | the ruling on telling one C handle from another, and three defects that were one question · **§1.12**, defects 029, 030, 031 |
| 56 | **M-cleanup-verdict** | done 2026-09-14 | `m-cleanup-verdict` | [055](docs/records/journal/055-cleanup-verdict.md) | the ruling on a scope-bound release for a C handle: a releaser keyed on the TYPE refused to Part 6, because ownership is a property of the CALL · a decision, not a feature |
| 57 | **M-marked-acquisition** | done 2026-09-14 | `m-marked-acquisition` | [056](docs/records/journal/056-marked-acquisition.md) | the mark goes where the obligation is CREATED — on the acquiring call — so a C handle nobody releases stops being silent · panel 147 R4, **§1.12** |
| 58 | **M-check-completeness** | done 2026-09-17 | `m-check-completeness` | [057](docs/records/journal/057-check-completeness.md) | ten defects the hunt turned up, closed — and three were not what their own entry said · scheduled, no warrant |
| 59 | **M-arm-platform** | done 2026-09-18 | `m-arm-platform` | [058](docs/records/journal/058-arm-platform.md) | the fourth real machine: arm64 Linux, where a container runs and where `char` is unsigned · **§1.12** |
| 60 | **M-readable-bytes** | done 2026-09-18 | `m-readable-bytes` | [059](docs/records/journal/059-readable-bytes.md) | a C byte buffer becomes text a program can print: the inbound direction `str` has never had · **§1.11** |
| 61 | **M-declared-extents** | done 2026-09-21 | `m-declared-extents` | [060](docs/records/journal/060-declared-extents.md) | how far C may read through a lent address, and what becomes of the pointer afterwards: `counted_by` declares the extent and the emitter checks it against the field's own `sizeof` in C; then the author flipped the default, so a C parameter is taken to KEEP what it is handed and a lend reaches only one declared `lent`; and where no word can reach — C frees the bytes from a callback, or from a later call with no pointer at all — the runtime names the leases that were live. **Seven defects, all closed** · **§1.12**, **§4.19** |
| 62 | **M-agreed-retention** | done 2026-09-28 | `m-agreed-retention` | [061](docs/records/journal/061-agreed-retention.md) | one C function, one story about who frees what it is handed: two modules may declare it with contradictory retention marks and the compiler takes both; one declaration cannot carry a contract chosen per call, which is `sqlite3_bind_text`'s three modes; and nothing says a pointer C made is C's to free once. Opened at M-declared-extents's close, because its four carried items had no home a reader would look in · **§1.11**, **§4.19** |
| 63 | **M-inferred-cell** | done 2026-10-10 | `m-inferred-cell`, deleted 2026-10-11 and placed again the same day on `eeb23937` | [062](docs/records/journal/062-inferred-cell.md) | a mutable cell declared by its own symbol, `@=`, and typed by its value as a binding is; a cell nothing re-binds refused with a certain fix; the tree rewritten through the compiler's own fix. Panel 209, ratified 2026-10-10; four steps in one lane the same day |
| 64 | **M-issue-files** | done 2026-10-12 | `m-issue-files` | [063](docs/records/journal/063-issue-files.md) | every item this project tracks is one GitHub issue: one file each of `issues/` first, a card on each, then 2,029 issues imported in the order they were born with their own days, then GitHub alone and the folder out of the tree. Opened 2026-10-04 by author instruction, ahead of every scheduled row because the lists it replaces are where every other milestone's work is filed |
| 65 | **M-typed-inspection** | done 2026-10-13 | `m-typed-inspection` | [064](docs/records/journal/064-typed-inspection.md) | a stopped program shows Heroes values: the frame lists the author's bindings alone, the emitter declaring what it invented `nodebug`, and one fixed formatter, `editors/lldb/heroes_lldb.py`, which the editor loads, shows a record, a `str`, a `[T]`, a `{K: V}`, a `T?`'s live arm and a variant's case in Heroes' syntax; the names keep their C prefix, refused in the emitted C by panel 213's two vetoes and kept by the author's answer of 2026-10-14 on its issue 2037 · scheduled, no warrant |
| 66 | **M-buildable-structs** | done 2026-10-17 | `m-buildable-structs` | [065](docs/records/journal/065-buildable-structs.md) | a C struct with long arrays is built by naming the fields that matter and ending with `rest: zero`. Panel 178 counted 42 public structs on Darwin with an array longer than 8, and panel 194's recount 33 and 27 on the two Linux legs, where the repository has none; a five-field `utsname` needed 1280 literal zeros until `rest: zero` landed in batch 13 (panel 164) · **§1.11**, **§1.12** |
| 67 | **M-bounded-seed** | done 2026-10-17 | `m-bounded-seed` | [066](docs/records/journal/066-bounded-seed.md) | the seed bounded in shape and in rhythm: its re-anchoring `#line`s joined to their anchors, refreshed only when the two-stage build needs it and at each release, every program carrying the key of what it was compiled from, and kept in the history at every tag, the history rewritten once and announced · scheduled, no warrant |
| 68 | **M-readonly-lend** | done 2026-10-18 | `m-readonly-lend` | [067](docs/records/journal/067-readonly-lend.md) | a function reads the caller's place without claiming to change it: `lent` on a parameter, C's `const T *` bound read-only, and beside it an `@` of the same place and a read-only lend past a variadic prototype refused · **§1.1**, **§1.11** |
| 69 | **M-witnessed-seed** | done 2026-10-18 | `m-witnessed-seed` | [068](docs/records/journal/068-witnessed-seed.md) | the seed witnessed by a second implementation: the archived Rust bootstrap rebuilt and agreeing with the seed at its tag, a gcc leg beside clang, every seed since the archive rebuilt from the one before it, 39 of 39, and every refresh since naming its route for the CI; then the bootstrap out of the tree, kept at its tag · scheduled, no warrant |
| 70 | **M-thesis-harness** | done 2026-10-24 | `m-thesis-harness` | [069](docs/records/journal/069-thesis-harness.md) | Part 11's metrics 2 and 4 run for the first time, as a Heroes program: forty held-out tasks sealed and a prediction pushed before the run, the primary inconclusive at +0.0 points with no discordant pair, the strict arm paying at turn 1 and level by turn 3 · **§1.1** |
| 71 | **M-rechecked-issues** | done 2026-10-25 | `m-rechecked-issues` | [070](docs/records/journal/070-rechecked-issues.md) | every open issue re-checked against the tree: closed with the commit that settled it, closed with the reason it no longer applies, repaired, or re-homed on a row still to come · scheduled, no warrant |
| 72 | **M-panic-location** | scheduled | — | — | a panic names the `.hero` file, line and function · **§1.12** |
| 73 | **M-generated-programs** | scheduled | — | — | programs nobody wrote: a generator that composes valid Heroes and knows the answer before the compiler is asked · **§1.12** |
| 74 | **M-compatibility-promise** | scheduled | — | — | the paragraph `1.0.0` rests on, and the suite that makes a broken promise red · CLAUDE.md §14 |
| 75 | **M-latency-ceiling** | scheduled | — | — | a latency the compiler may not lose without a decision: `heroes check` and clang on the seed timed at every close, the incremental frontend its return condition · CLAUDE.md § Verification |
| 76 | **M-core-packages** | scheduled | — | — | small packages that compose, organised as Go's tree, in Heroes or over C |
| 77 | **M-package-manager** | scheduled | — | — | `heroes add`/`heroes fetch`; bindings instead of a standard library |
| 78 | **M-web-framework** | scheduled | — | — | composes the core packages, Go/Echo style: explicit routes, records, no magic |
| 79 | **M-doc-generator** | scheduled | — | — | `heroes doc`, the one direction Part 6's literate-source row promises · scheduled, no warrant |
| 80 | **M-compared-languages** | scheduled | — | — | the same tasks written by a model in Heroes, Go, Rust, C and Zig, scored on hidden tests, turns and silent failures, the prediction registered before the first run · scheduled, no warrant |
| 81 | **M-deployable-binary** | scheduled | — | — | what the machine that RUNS a Heroes program needs, on the three platforms, and which `-O` a shipped artifact carries |
| 82 | **M-install-channels** | scheduled | — | — | a Homebrew tap, winget, a Nix flake, a Docker image, all built from the seed and pinned to a `v*` release tag; the version scheme itself was decided ahead, 2026-09-07 |
| 83 | **M-online-compiler** | scheduled | — | — | the compiler reached without installing anything: the site's visitor writes Heroes and gets its answer · scheduled, no warrant |
| 84 | **M-lsp-server** | scheduled | — | — | `heroes lsp` |
| 85 | **M-vscode-extension** | scheduled | — | — | the extension, complete |
| 86 | **M-publication-gate** | scheduled | — | — | the last gate before anything goes outward · CLAUDE.md §14 |
| 87 | **M-launch-posts** | scheduled | — | — | the first posts, each with the numbers its command produced, after the first hour a stranger spends was tried · scheduled, no warrant |
| 88 | **M-microcontroller-verdict** | scheduled | — | — | the ruling on a Heroes program running on a microcontroller under an RTOS, RISC-V first; the two facts a 32-bit build refuses today are its brief · **§1.12**, Part 2 |
| 89 | **M-qbe-backend** | scheduled | — | — | Part 7 item 15 — the proof that the IR is not C in disguise |
| 90 | **M-journey-book** | scheduled | — | — | the journey — how this language came to be |
| 91 | **M-guide-book** | scheduled | — | — | the guide, as a book you would find in a shop · **§1.1** |

Three closed milestones have no tag of their own because they were parents or
sub-steps: **M-checker-core**, **M-data-declarations** and **M-rich-diagnostics**
landed inside `m3`, and **M-native-backend** was the parent of the four backend
milestones and was never tagged. § The names carries them.

`git tag --list --sort=creatordate` gives the same chronology from git itself.
