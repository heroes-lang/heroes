# Panel 253, spec-warden. Predictions WRITTEN at 15:51 CET 2026-11-19, before any measurement and before the blind run: the runner may start.

## Predictions registered before the blind run

Registered at 15:51 CET (`date`: 15:50:49 read just before writing), by the spec-warden, before any count, refresh or build of this seat and before the blind seat's runner started. Each is worded in the counts of `llm-ergonomist.md` § The decision rules and § The grading (as repaired after the critic's first reading), five sessions a cell; where a cell is left short by the stop rule, each prediction is read on the replicates every cell of its task reached, as the rules read them.

- **P-t2-a (Q2, `doc-r` suffices)**: at least **4 of 5** `doc-r` (label `m`) answers are right by the t2 row.
- **P-t2-b (Q2, `doc-r` needed)**: at most **1 of 5** `now` (label `g`) answers are right by the t2 row; so `doc-r`'s right answers exceed `now`'s by at least 3, and the rule reads *needed*.
- **P-t2-c (Q2, recorded, the `now` default)**: in at least **3 of 5** `now` answers the example is written somewhere other than a `test` whose title opens `count_words:` (in the comment, as a code fence or as prose); and at least **3 of 5** `now` answers write the parameters in a form other than one `` # - `name`: `` line each after a line holding `#` alone.
- **P-t4-a (Q4, the hazard under today's spec)**: at least **3 of 5** `now` (label `g`) answers write a test that reports `ok` when run, having checked no connection; so the rule records *"a reader of today's spec writes the silent pass the outcome exists to refuse"*.
- **P-t4-b (Q4, `skip-r` suffices)**: at least **4 of 5** `skip-r` (label `f`) answers are right by the t4 `skip-r` row.
- **P-t4-c (Q4, the outcome reduces the hazard)**: **0 of 5** `skip-r` answers still end the absent case in silence; with P-t4-a, `now`'s silent passes exceed `skip-r`'s by at least 3, and the rule reads *reduces the hazard*.
- **P-t6-a (Q6, `umb-r` suffices)**: at least **4 of 5** `umb-r` (label `q`) answers are right by the t6 `umb-r` row.
- **P-t6-b (Q6, recorded, `now`'s question 4)**: at least **3 of 5** `now` answers predict **accepted** for the build with `use <rebel>/time` added.
- **P-real (the budget, scored by this seat's own refreshes, not by the blind run)**: the refresh of `now` prints `real 11384`, the pin, exactly. The real delta of each arm stands to its vendored delta between 1.15 and 1.60 times: `doc-r` +112 vendored to **+130 to +180** real; `skip-r` +59 to **+68 to +95**; `umb-r` +134 to **+155 to +215** (the `sum`'s 64 hex digits may tokenise worse on the real tokeniser); `stack-r` +13 to **+13 to +22**; `all-r` +305 to **+350 to +480**; `all-r-stack` +318 to **+365 to +500**.

If a prediction above is falsified, the sentence it pays for is paid by nothing under §1.6's rule until a named removal is found; this seat says so per question below.

## Report, written as I go

(Measurements and verdicts follow, each block with its commands. Nothing below this line was written before the runner's gate could read the heading above.)

### The folder, the compiler, and the counts that settle the budget

**Folder**: `.claude/worktrees/rebel-scratch/253-spec-warden/`, made 15:51 by `git archive 181555e9a32d0b18878f9f10e48d2eadd939c285 | tar -x`, `find -type f -not -path tmp/*` **20,037** files, the `.git` file written as the brief shows. Seed `clang -I runtime seed/heroes.c runtime/runtime.c -o sw-heroes`, exit 0; stage 2 `./sw-heroes build selfhost/main.hero -o sw-heroes-next`, exit 0, `--version` **`heroes 0.6.0`, `sources 23c7d2a626b29fd5`**, the frozen key (`sw-stage2.log`). No clock taken.

**design.md §1.6, grepped today in my copy** (`grep -n '^### 1.6' docs/design.md`: 253): the ceiling is **16384**, measured by `claude-opus-5` through `count_tokens` (line 255-256); the payment rule unconditional (324-326); the consolidation block's 1,500 at 291-293. The spec: 492 lines, 29,407 bytes, sha256 `1d532d514831aa70...`, `cmp` equal to `bw/arms/spec-now.md`.

**Vendored** (`./sw-heroes-next measure <arm>` from my copy's root, `sw-measure-arms.txt`), every row equal to the brief's table: `now` 8551/8702, `doc-r` 8657/8814, `skip-r` 8609/8761, `umb-r` 8682/8836, `stack-r` 8564/8715, `all-r` 8846/9007, `all-r-stack` 8859/9020 (legacy/cl100k, the maximum the second).

**Real, the binding instrument**: seven `--refresh` calls, the brief's maximum, all in one shell with the key loaded from `.env` (length 108 printed, never the value; `grep -c sk-ant` of the log: 0), each arm copied over my copy's `spec/heroes-spec.md`, refreshed, the frozen spec restored and `cmp`-checked against `bw/arms/spec-now.md` after each (7 of 7 clean). Log `sw-refresh.txt`; model `claude-opus-5` every time, taken `2026-11-19`:

| arm | time | sha256/16 | SPEC_REAL_TOKENS | SPEC_DIGEST | real delta | vendored delta | real/vendored |
|---|---|---|---|---|---|---|---|
| `now` | 15:54:19 | `1d532d514831aa70` | **11384** | `583fa94c01eddbf8` | 0 (the pin of 2026-11-15 holds exactly) | 0 | |
| `doc-r` | 15:54:20 | `656b8364f33abaeb` | 11529 | `160fdb3b66a5cebf` | **+145** | +112 | 1.29 |
| `skip-r` | 15:54:22 | `a47a36c1f157cd29` | 11458 | `2c73778f7f4ff6e3` | **+74** | +59 | 1.25 |
| `umb-r` | 15:54:23 | `07acf83d2ec062f9` | 11550 | `4f4ad93a43744791` | **+166** | +134 | 1.24 |
| `stack-r` | 15:54:24 | `5f64ac703c5f7978` | 11398 | `6c4f7abb0f8d6bcf` | **+14** | +13 | 1.08 |
| `all-r` | 15:54:25 | `48d807010b254507` | 11769 | `5b1906dd463fe20d` | **+385** | +305 | 1.26 |
| `all-r-stack` | 15:54:27 | `cb648afa1ec30da0` | 11783 | `488be87e3a611729` | **+399** | +318 | 1.25 |

The three robust sentences add exactly: 145 + 74 + 166 = 385 = `all-r`'s delta. **Against the ceiling**: `all-r-stack` at 11,783 real plus the FFI floor's 60 is 11,843 against 16,384, **4,541 free**; no arm comes near a veto on the budget. **P-real scored**: `now` prints 11384 exactly (hit), and every arm's real delta falls inside its registered range (doc-r 145 in 130-180, skip-r 74 in 68-95, umb-r 166 in 155-215, stack-r 14 in 13-22, all-r 385 in 350-480, all-r-stack 399 in 365-500): **7 of 7 hits**, scored by this seat's own instrument at 15:54, after registration at 15:51. Seven refreshes spent; every other draft below is priced **vendored, a lower bound**, in those words.

### The consolidation block's 1,500: a premise of the brief that no longer binds

The brief's question 6 asks whether `all-r` *"fits the consolidation block's 1,500 real tokens (line 292) beside what sittings A and B spend"*. design.md §1.6 (291-293) defines the block as **the rows from M-buildable-structs to M-compatibility-promise**, spending at most 1,500 real **before the thesis harness has run**. `ROADMAP.md` at the frozen commit: M-buildable-structs row 66, done 2026-10-17; M-thesis-harness row 70, **done 2026-10-24** (Part 11's metrics 2 and 4 run); M-compatibility-promise row 74, **done 2026-10-30**. M-rebel-library has **no row** in the frozen chain (`grep -n -i rebel ROADMAP.md`: nothing, though decision 4 on its page says *"A new row, at once"*), and the page encloses M-core-packages (row 76) and M-doc-generator (row 80), both after row 74. The ledger (`docs/measurements/010-spec-budget-ledger.md`) counts the block in rows 7655, 7846 and 7856 only (*"251 of the consolidation block's 1,500 spent"*, its last mention, `grep -n consolidation`); no row since names it. **So the 1,500 binds nothing in this sitting**: what binds is the ceiling (16384) and the unconditional payment rule. For the record anyway: sitting A's R2 is **+9 real** (panel 251's synthesis, R2, carried), sitting B's `proposal` arm **+160 vendored, a lower bound** (`docs/panel/252-briefs/00-shared.md` line 111, read in the rebel-round worktree where its briefs stand untracked; no `252` file exists at the frozen commit, as that brief says itself), and this sitting's `all-r-stack` **+399 real**: about +600 real together at a ratio near 1.26, roughly 12 % of the headroom.

## Q1: design.md §1.11's sentence, and the register

- `verdict`: **approve the robust design arm (`design-r`) at 0 spec tokens; object to the conservative (`design-c`)**
- `section`: design.md §1.6 (no spec token moves), §1.11 and Part 2's bullet; CLAUDE.md § 2; the author's decisions 9, 10 and 24 (`M-rebel-library.md`)
- `spec_token_delta`: **0** under both arms (neither touches `spec/`); design.md +333 (robust) and +88 (conservative) vendored, under no ceiling (`./sw-heroes-next measure docs/design.md` says so)
- `removal`: none owed, no spec token spent
- `needed_for_self_hosting`: no
- `argument`: The spec names no library (`grep -n -i -E '\b(core|rebel|standard library)\b' spec/heroes-spec.md`: nothing), and § 13's first sentence already holds Rebel as a package. **The spec's silence holds only if Q6 lands without the word `rebel`**: `umb-r` writes it twice into § 1. The conservative leaves Part 1 false three times against decision 24 (*"What reads data from outside is written in Heroes, never bound from a C library"*): the box's *"comes from C libraries through the FFI"*, *"That is the whole standard library"*, and a table recommending PCRE2, cJSON, zlib and libcurl where Rebel's `regex`, `json`, `compress` and `http` faces stand (`[ -f ]`, run). Decision 10 orders living texts rewritten clean, so the conservative may not carry them.
- `prediction`: at M-rebel-library's close, `grep -c -i -E '\b(rebel|core|standard library)\b' spec/heroes-spec.md` prints **0** (with `umb-w`, Q6), and every row of §1.11's table that names a C library for a need Rebel has a face for also names that face.
- `condition`: a reading showing a reader writes worse programs for not being told in the spec that Rebel exists (no cell measured it); or Q6 landing `umb-r` as written, which moves this verdict to *object* until the name leaves the spec.

**What the spec owes**: nothing. § 13 line 377 (*"Anything beyond this document comes from packages or C libraries"*) is true of Rebel as written.

**The register (decision 10), proposed from the set the milestone holds**, `bw/q1/core-milestone-issues.tsv` (sha256 `c09d85c6ac5817d9`, re-hashed; 231 lines, no header line, so 231 issues; 19 decisions, 39 open), with decisions 2560, 2561 and 2562 read by `gh issue view` (reads only). *Invalidated* means a later decision falsifies its ruling; *stands* means nothing of this milestone falsifies it; *moves* means its work goes on under another row.

| decision | proposal | on what |
|---|---|---|
| 2304, panel 234 | **partly invalidated**: R1's admission test, R2's Go's tree under `core/`, R3's conventions page and R5's card are invalidated; **B5's veto** (no package source in `selfhost/library_source.hero`) and the platform C as an archive beside the runtime **stand** | decisions 2, 6, 8, 22 and 23 |
| 2403, recursive faces walked by loops | invalidated | its reason is a line of 234's card |
| 2404, the first batch's `path` admits no function | invalidated | 234's R1 test, replaced by decision 2's coverage |
| 2415, panel 241 | **stands** (the compiler takes no package; decision 6 makes that permanent for Rebel); its `crypto/sha256` ruling **moves** to Rebel's `crypto` (Heroes, as ruled); the package manager's rewrite **moves** to M-rebel-packages-manager | decisions 6, 24 |
| 2423, panel 242 | **stands**: R1 to R4 are unfalsified; R2's ruler binds the compiler taking a package, which decision 6 makes moot for Rebel without falsifying | decision 6 |
| 2449 (243), 2475 (245), 2497 and 2498 (246), 2517 (247), 2527 (248), 2563 (249) | **stand**: the C boundary, the runtime and fetch, nothing of `core` | |
| 2461, panel 244 | **stands**; `heroes new --package`'s template re-read for any `core` it writes | decision 8 |
| 2528, every program on the newest `core` until 1.0.0 | invalidated | decision 8; Rebel's one version is item 35's and Q6's |
| 2560, placing only the modules to meet 242's R2 | invalidated | its premise was `core`'s ruler; decision 6 and Q6 answer it for Rebel |
| 2561, hooks in Heroes over `core` | invalidated | decision 6 (*"Only the package manager will"*), decision 8 |
| 2562, the paired run's instrument for a sealed suite of package tasks | **moves** to M-rebel-library | decision 15 (*"Building Rebel is also a measure"*) |
| 2564, panel 250 | **stands** for a package placed whole; its *"2315 waits for packages `core` does not hold"* invalidated, 2315 moving to M-rebel-packages-manager; superseded for an umbrella if Q6 lands | decision 8, Q6 |
| 2590, panel 251 | **stands** (sitting A of this milestone) | |

**The 14 open features and tasks**: **kept and moved** to M-rebel-library: 2551 (the built-in name sentence, which Q4 meets), 2460 (a `path` build telling what a fetched build will refuse, which Q6's `path` clause meets), 2399, 2358, 2298, 2297, 2296, 1103, 1072, 925, 927. **Closed with a note**: 2313 (the register is its outcome for `core`; the rest is decision 16's review), 2082 (invalidated by decision 6; hooks in Heroes over no library is the author's to file again if wanted), 1067 (invalidated by decisions 7 and 8: the first group of examples uses no library). **The 25 open defects**: kept, classes unchanged, moved to M-rebel-library, except **2577** (core 0.6.0's `strconv` comment), closed with a note by decision 8; 2570 is re-read when `examples/query` loses `core`. **The 17 catalogues** (`bw/q1/core-catalogues.txt`): each `[dependencies.core]` entry deleted and the program rewritten to use no library (decision 7's first group), **not moved to Rebel**. A question, unrun: does `compat`, which reads `examples/` as each release tag holds it, still fetch `core` 0.6.0 through `file://../../packages` once `packages/core` leaves the tree?

**Where the register lives**: **one `decision` issue**, decision 10's register, drafted outside the tree and filed and closed the same day by `tests/harness/issues.hero` (`.claude/rules/records.md` § The issues: *a decision taken ... closed the day it is opened*), its body the table above. Each invalidated closed issue gains a dated `comment` naming it (a closed issue is corrected underneath by `comment`, never `edit`); each sitting it invalidates (234, and 250's 2315 clause) gains a dated note appended underneath (append-only records). The milestone page cites the issue in its reasoning and carries no list (§ A live list). **Not a file of `docs/`**: an index beside the issues is a second place where truth lives (§ A tree of entries).

**CLAUDE.md:279, the author's**: replace *"A standard library."* with *"A standard library shipped with the compiler or the runtime, or one the compiler is written with; Rebel is a package a program takes or leaves."* (+26 vendored on CLAUDE.md, 6347 to 6373 cl100k, `sw-drafts/CLAUDE-w.md`; its real count unrun, the contract's own ceiling judging it.) **The ffi-pragmatist's line 3**, the author's: *"Panel judge for the C boundary (design.md §1.11, §4.19): the runtime imposes no standard library, and what a program needs beyond it comes from C through the FFI or from a package it carries."*

## Q2: the doc comment's shape, and `heroes doc`

- `verdict`: **object, provisional on the blind seat's t2**: approve the home and the shape; object to the refusal clause as written (it meets none of the measured mistakes); adopt `doc-w`
- `section`: design.md §1.6 (payment), §1.2 (real cost), Part 6's literate-source row and panel 136's ruling (design.md:2601-2604); CLAUDE.md § 2 and § 12; `.claude/rules/spec-shape.md` § Where a rule lives
- `spec_token_delta`: **+145 real** (11,384 to 11,529, `--refresh` 15:54:20), +112 vendored; `doc-w` **+1 vendored** over `doc-r` (8815 against 8814, a lower bound)
- `removal`: none named; paid by **P-t2-a and P-t2-b**, registered 15:51 before the runner, scored by the t2 cell's grading row at this sitting (M-rebel-library), an instrument that existed when registered
- `needed_for_self_hosting`: no
- `argument`: The arm promises one refusal, *"a parameter line naming no parameter"*. Of Rebel's 642 functions whose lines disagree with their signature, **0** name a non-parameter, **0** are out of order, **16** leave a parameter without a line and **626** have no line at all (`sw-tools/doc_kinds.py`, the brief-writer's reading of a face kept). The refusals the brief's robust route lists meet **none** of the 642: the shape would be broken 642 times in silence while the spec names a refusal that never fires. `doc-w` refuses what § A7's own check holds, *"each parameter once, in order"*, and drops *"the same bytes every time"*, a promise about the tool (panel 230's ruling on § 6), at +1 vendored.
- `prediction`: P-t2-a and P-t2-b (above); and **`heroes doc` built with `doc-w`'s refusal, run over Rebel 0.1.0's 149 faces as frozen, refuses 642 functions, 626 of them for a missing list; built with the arm's literal refusal, it refuses 0**.
- `condition`: t2 reading `doc-r` not needed (right answers ahead of `now` by fewer than 3) or not sufficient (fewer than 4 of 5): then the +145 is unpaid, my verdict is **veto**, and the shape stays in § A7 and the verb's help, at 0 spec tokens.

**The sentence I would adopt**, § 1's comment bullet (`sw-drafts/spec-doc-w.md`, sha256 `a73ea1f4a35c0199`, vendored 8658/8815):

```
- `#` comments to end of line, markdown inside. A comment directly above a
  declaration, a field or a case documents it: a first sentence that stands
  alone, then, after a line holding `#` alone, one `` - `name`: `` line per
  parameter in the signature's order. A `test` with a comment directly above
  it and a title opening with a function's name and a colon is that
  function's example. `heroes doc` prints a module's declarations, what
  documents them and their examples, and refuses a comment whose lines do
  not name each parameter once, in order. `##` is a section heading.
```

Its reader-facing half is the arm's, word for word; only the tool's clause changed, which no t2 answer is graded on, so the t2 measurement carries to it by that argument and not by a reading. If the synthesis wants the measured words exactly, `doc-w2` keeps *"the same bytes every time"* (+7 vendored over the arm).

**The brief's sub-questions.** **Home**: § 1, merged into the bullet where *"A comment directly above a declaration documents it"* stands (*"Merging beats appending"*); § 12 holds `test`, but the rule governs a comment. **A verb named in the spec**: one precedent, *"`heroes add json` writes that entry"* (line 35; `grep -n -o '`heroes [a-z]+'`: that line alone; § 12 names no `heroes test`). It is the reader's act that earns the words, not the tool's behaviour, which is why the byte promise goes. **A diagnostic class**: `heroes doc`'s refusal is a tool's, not the checker's; it owes a golden form (`tests/golden/doc/`) with each case's `#~` annotation in its source, and no spec words beyond the clause. **Panel 136's ruling**: the arm's example sentence *is* the documented test block; it needs no words of its own. **`## Examples`**: decision 22 and § A7 put examples under that heading, and the arm does not say it; recommended, `heroes doc` finds an example by the arm's rule wherever it stands, the heading staying Rebel's convention checked by Rebel's own check, at 0 tokens. Otherwise the heading costs its own payment. **The third refusal** (an example title naming no function): not in the arm, and by the arm's own definition such a test is not an example, so nothing to refuse; recommended not built. **A widening to `heroes check`**: refused by the counts. The shape is broken by **5,371** documented functions of `selfhost/` (6,862 with parameters and no line, 1,491 of them with no comment), 1,043 of `examples/` and 1,738 of `tests/harness/` (`doc_kinds.py` over each). The checker would refuse the compiler's own tree.

## Q3: the system's cryptography on macOS

**0 spec tokens in every route as posed** (H, S, C, V, conservative): none changes a sentence a reader reads. One condition: route H, as the brief says, lives with § 7's *"bitwise ... (i64 only ...)"* and *"Overflow aborts at every width"*; **a wrapping operator, a bitwise operator on a narrower width, or a constant-time mark would be a new form**, owing its own sitting, its own price and § 2's ticket. Nor does a crypto door of the platform layer owe § 13's sentence naming `hero_os.h`, `hero_net.h` and `hero_time.h`, so long as programs reach it only through Rebel.

## Q4: a skipped outcome for `heroes test`

- `verdict`: **approve the form in § 12, provisional on t4; object to *"fails no run for it"*; adopt `skip-v2`**
- `section`: design.md §1.6, §1.2; CLAUDE.md § 2 and § Precedence (robustness, the truth of a green run); `.claude/rules/verification.md`'s first warning (*"a run that selected nothing is a green run that tested nothing"*); `tests/harness/report.hero:15-21, 49-53`
- `spec_token_delta`: **+74 real** (11,384 to 11,458, `--refresh` 15:54:22), +59 vendored; `skip-v2` **−3 vendored** against the arm (8758 against 8761 cl100k, 8607 against 8609 legacy; a lower bound)
- `removal`: none named; paid by **P-t4-a, P-t4-b and P-t4-c**, registered 15:51; `skip-v2` costs no token beyond the measured arm, so no removal is owed for it
- `needed_for_self_hosting`: no (the compiler's tests wait for no service)
- `argument`: 106 of Rebel's tests read `ok` without running their subject, a count the brief re-ran with two tools and calls a floor. That is the thesis's hazard: a silent pass. The arm's *"fails no run for it"* moves the same hazard one level up: a machine with no service runs `heroes test mongodb.hero`, every test skips, and the run is green having tested nothing. The project's own harness refuses exactly that (*"Whoever skips also floors its skips ... so a machine that can judge nothing is a failure"*, `report.hero:49-51`). `skip-v2` says it, cheaper than the arm.
- `prediction`: P-t4-a, P-t4-b, P-t4-c; and **at landing, `heroes test` over a file whose every test calls `skip` exits 1 and prints each title with its reason, and over a file with one pass and one skip exits 0**, each a golden case.
- `condition`: t4 reading `skip-r` short of sufficient (fewer than 4 of 5), or the hazard not reduced (`now`'s silent passes ahead of `skip-r`'s by fewer than 3): the +74 is then unpaid, and **veto**.

**The sentence I would adopt**, § 12 after the `assert` sentence (`sw-drafts/spec-skip-v2.md`, sha256 `4cbe5ca0ebe795cf`):

```
`skip(reason: "no server named")` ends a test there as skipped, never passed:
`heroes test` prints its title and reason and counts it apart, and a run
where every test skipped fails; only a `test` block calls it.
```

**What this departs from, said plainly**: t4's `skip-r` grading row scores a prediction that the run *"ends with status 0"*, and under `skip-v2` t4's file of one skipped test ends 1. So the floor clause is **not measured by the blind run**. It is the project's own rule, at no extra token. P-t4-a and P-t4-c (the hazard and its reduction) carry; P-t4-b's exit half does not. The robust choice is the floor. What conservative would have been is the arm's words, *"fails no run for it"*.

**The name**: a built-in's name is taken everywhere and the spec says so nowhere (task 2551, open). That is a debt of every built-in (`map`, `find`, `sort`), not one `skip` creates; `skip` does not owe 2551's sentence first. Taking the name meets `error[builtin_name_taken]`, a compile error with a message, which is the thesis's shape. **The renames are owed** (CLAUDE.md § 2: before 1.0.0 nothing is kept for an older program): the 7 declarations the brief counts, among them **Rebel's public `io.skip`** (`packages/rebel/0.1.0/files/io.hero:447`, `function skip(@r: Reader, n: i64) -> i64?`), a face's function renamed, and the bindings named `skip`. If the compiler-engineer takes another spelling, the sentence lands with one word changed, carried by argument and not by the reading. **Where it stands**: § 12, the section of the operation it governs. `offered` reads only the `Built-ins:` sentence and only spec to compiler, and `named` sees `skip` in a code span anywhere. `spec-shape.md`'s *"a name the compiler offers appears once in the `Built-ins:` sentence"* is already not held literally: `read_file`, `exit` and `end_lease` are in `selfhost/inventory.hero` and stand outside the sentence (§ 11's prose, § 13).

## Q5: the whole stack of calls when a program aborts

- `verdict`: **veto `spec-stack-r.md`'s sentence; approve the runtime's work at 0 spec tokens**
- `section`: design.md §1.6 (unpaid), CLAUDE.md § 2 (no measured effect), CLAUDE.md § 12 (spec beats compiler: the sentence would be false on landing); panel 230's ruling on the same sentence (`docs/panel/230-*.md:68-75`, `:137`)
- `spec_token_delta`: **+14 real** (11,384 to 11,398, `--refresh` 15:54:24), +13 vendored
- `removal`: **nothing, and that is a problem**: no removal named, no prediction registered, no blind cell
- `needed_for_self_hosting`: no
- `argument`: Panel 230 ruled this very sentence, § 6's *"saying why and where"*, at **zero tokens**: *"The place names itself in the output, no program is written against it, and *saying why* stays true under every route; the narrowing goes on the milestone's page instead, where a promise about the tool belongs."* Nothing since changes that: a stack names itself too, and no program reads it. The sentence would also be **false the day it lands** where the walk is not built: Windows (*"Windows has no walk"*, `runtime/parts/stack.c:858`), and x86-64 at `-O2`, where the driver omits frame records and `flags.hero` carries no `-fno-omit-frame-pointer` (`grep -c`: 0). Route (B) would make it false everywhere beyond the boundary.
- `prediction`: § 6 still reads *"An abort ends the program at once, saying why; no `T?` carries one."* at M-rebel-library's close (`grep -c` of that sentence: 1), whatever the runtime prints.
- `condition`: a blind cell, registered before its run, where the program hides which call aborted and a reader's repair turns on the promised chain, showing the sentence changes at least 3 of 5 answers; **and** the walk built and passing on the three platforms at both levels before the sentence lands.

## Q6: one umbrella, one version, placed package by package

- `verdict`: **object to `umb-r` as written; approve `umb-w`, provisional on t6**
- `section`: design.md §1.6; CLAUDE.md § 2 and § 12; `.claude/rules/cli-surface.md` § Four input classes (the falsifier and the catalogue's fields); `.claude/rules/spec-shape.md` (*"the whole language and nothing else"*, and *"an example in the one document a reader is told to trust is a claim"*)
- `spec_token_delta`: **+166 real** (11,384 to 11,550, `--refresh` 15:54:23), +134 vendored; `umb-w` **−4 vendored** against `umb-r` (8832 against 8836, a lower bound); `umb-p`, the arm with only `path` bound, +5 vendored
- `removal`: none named; paid by **P-t6-a**, registered 15:51 (P-t6-b is recorded, not a gate)
- `needed_for_self_hosting`: no
- `argument`: It keeps *"one version of each package"* (one entry, one version, a subset placed) and the falsifier (deleting a name changes only whether the program builds). It is no fifth class, but it amends `cli-surface.md`'s *"per entry, its source, its exact version or tag and its fingerprint"*, in the same landing. Two defects: it names a real library at its real version with a `sum` nothing in the tree holds (`grep -rl` of the 64 digits: 0 files; Rebel 0.1.0 has no listing), which a reader copies into a real catalogue; and it leaves undecided whether a `path` entry binds `packages`, the one route all 161 Rebel examples take.
- `prediction`: P-t6-a; and **under `umb-w`, `examples-rebel/heroes.toml`'s `packages` lists exactly the distinct first parts of its programs' `use <rebel>/` lines, and 0 of the 161 programs is refused**.
- `condition`: t6 reading `umb-r` short of sufficient (fewer than 4 of 5): unpaid, **veto**. Or the synthesis keeping `rebel` and its `sum` in the example: my objection stands.

**The sentence I would adopt** (`sw-drafts/spec-umb-w.md`, sha256 `bf083eac6c0d338e`), § 1 after *"has no catalogue."*:

```
  catalogue. An entry publishing several packages under one version, or
  its `path`, places only those its `packages` names, with each one they
  use: `heroes add kit/csv` adds one, and a `use <kit>/flags` it does
  not name is refused.
```

and the example's entry `[dependencies.kit]`, `version = "0.1.0"`, `packages = ["csv", "json"]`, its `sum`. A hypothetical name, as `json` and `yaml` already are. **What it departs from**: t6's readers read `rebel`, so the change of name is carried by argument and not measured; the `path` clause is unmeasured.

**Panel 250's drafts**: `umb-r` revives none verbatim. p1 and p1r place by the `use` lines (route M). The umbrella places by a declared list, the critic's unlisted routes *"a listing kept in the catalogue"* and *"a sum over a subset the catalogue declares"* (`docs/panel/250-*.md:56-59`). It keeps p1's *"each file held to the listing the `sum` names"* in the brief's prose, not in the arm's words. And p1r's refusal returns with its trigger moved from the order the verbs ran to the list, which removes 250's order finding. The conservative is W with p0, 0 tokens. **The `path` question**: the falsifier already exempts a `path` entry from *"which module compiles"* (`cli-surface.md`: *"a `path` entry ... is the one exemption, and every build that uses one says so"*), and nothing exempts it from a refusal. Robust is one rule for both, so a program never builds over `path` and is refused once fetched; that is the shape open feature 2460 exists to warn about. **A package that uses the umbrella** (the nested shape): *"its brackets name entries of the package's own catalogue"* (line 38-39) already makes each catalogue's list govern its own `use` lines, placement being their union, so no word is owed; a golden case holds it.

## Question 7: the diagnostic classes each route adds

| route | class | what it owes |
|---|---|---|
| Q2, `heroes doc` | one refusal: a comment whose lines do not name each parameter once, in order (`doc-w`) | a golden form `doc`, each case's `#~` annotation in its source and its `.expected`; the spec clause above; no `Fix` claimed `certain` (writing a parameter's meaning is a guess at best) |
| Q2, the example title naming no function | none recommended | by the arm's definition such a test is no example |
| Q4, `skip` outside a `test` block | a new checker class | a `tests/golden/check/` case annotated `#~`, its `.expected`; the spec words *"only a `test` block calls it"* (in the arm); judged by every golden tree (`verification.md`: a new refusal) |
| Q4, a declaration named `skip` | **no new class**: `builtin_name_taken` exists | the renames; task 2551 owns the sentence |
| Q6, a `use <kit>/x` the list does not name | a new class in module resolution | a `check` or `fetch` golden case, annotated; the spec words *"is refused"*. **The command goes in the message text**, as `selfhost/module/package_told.hero:102` already does (*"`heroes add <entry>` writes one"*, no `Fix`). A `certain` `Fix` is owed only where `check --apply` can write the catalogue and the golden's applied fix builds offline, which a `path` case can and a fetched one cannot without a fetch |

## Premises of the briefs found false

1. **The consolidation block's 1,500** (`spec-warden.md` lines 6-7 and item 6): the block is the chain's rows 66 to 74, all done by 2026-10-30, and the thesis harness ran (row 70, 2026-10-24); the ledger stopped counting it at row 7856. It binds nothing here.
2. **"A spec that named it would make it the language's, which decision 9 refuses"** (`00-shared.md` § Question 1): the same brief's Q6 robust arm names `rebel` twice in spec § 1, and decision 9's words are *"imposes no standard library and offers one that is optional"*; the refusal to name it is the brief-writer's inference, which this seat shares on `spec-shape.md`'s ground, not on decision 9's.
3. **"`heroes doc` over Rebel today would meet 642 functions whose parameter lines disagree"** (§ Question 2), read with the robust route's refusals: those refusals meet **0** of the 642 (0 naming a non-parameter, 0 out of order; 16 with a parameter left out, 626 with no list).
4. **"The refusal a diagnostic with a `certain` fix naming the `heroes add`"** (§ Question 6): today's practice puts that command in the message, not in a `Fix` (`package_told.hero:102`), and a `certain` fix's golden must build after applying.
5. **Q6's arm example**: its `sum` for `rebel` 0.1.0 exists nowhere in the tree and cannot be true; the brief does not say so.
6. **§ Question 5's record**: the brief cites panel 230's R1 (the library's place) and not its ruling on this very sentence (`230-*.md:68-75`, § 6 at zero tokens), the record that decides Q5's spec half.
7. **Not the brief's, the tree's**: `spec-shape.md`'s *"A name the compiler offers appears once in the `Built-ins:` sentence"* is not held literally today (`read_file`, `exit`, `end_lease` stand outside it); and the frozen `ROADMAP.md` has no M-rebel-library row though decision 4 says *"A new row, at once"*.

Written by the spec-warden, 15:50 to 16:05 CET 2026-11-19 (`date`). Seven refreshes spent, every one logged; no other paid call; nothing read under the blind area.
