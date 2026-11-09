# Panel 240: compiler-engineer report

Seat: compiler-engineer. Worktree: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/240-compiler-engineer/`,
detached at `429464c94012e0691dfa091e0b38070c5c9550f0` (`git -C <wt> rev-parse HEAD`, run 2026-11-09 21:50).
Written as I go; finished 2026-11-09, 22:48 by the clock (`date`).

Stage 1: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `/usr/bin/time -p`: real 9.79, user 9.49, sys 0.18.

## 1. The fourteen codes, by sense, read at their definitions

Command: `git grep -n '"<code>"' -- 'selfhost/*.hero'` for each code, then each
definition read with `sed -n`.

| code | sense | where it is made | what it tells |
|---|---|---|---|
| `ffi_package` | **C** | `selfhost/emit/ffi_build.hero:285` (`package_problem`) | the driver's `heroes-ffi-package ` line: `pkg-config` could not answer for a group's package |
| `package_comparison` | **C** | `selfhost/head_package.hero:76` (`thesis`) | a `package` string holding `<`, `>`, `=`, `!`, pkg-config's version comparison |
| `no_pkg_config` | **C** | `selfhost/cli/libraries.hero:308` | `pkg-config` absent or not startable |
| `package_unmarked` | fetched | `selfhost/package_mark.hero:111`, `selfhost/module/package_told.hero:129` | `use x` where `x` is a catalogue entry: write `use <x>/m` |
| `package_reads_outside` | fetched | `selfhost/cli/package_units.hero:271,281,289`; constant `selfhost/cli/package_reads.hero:68` | a fetched package's unit reads outside its places (panel 235 R9/C2). **Its messages name the C-sense mark** (`package_units.hero:276`: "the folders its `package` groups' answer names") |
| `package_wants_a_slash` | fetched | `selfhost/package_mark.hero:265` | `use <json>.parse` |
| `package_wants_a_name` | fetched | `selfhost/package_mark.hero:44` | `use <` with no name |
| `package_wants_a_module` | fetched | `selfhost/package_mark.hero:68` | `use <json>` with no module |
| `package_name_quoted` | fetched | `selfhost/package_mark.hero:227` | `use <"json">/parse` |
| `package_mark_unclosed` | fetched | `selfhost/package_mark.hero:57` | `use <json/parse` |
| `package_mark_holds_a_path` | fetched | `selfhost/package_mark.hero:253` | `use <json/parse>` |
| `package_mark_apart` | fetched | `selfhost/package_mark.hero:74` | `use < json >/parse` |
| `unknown_package_module` | fetched | `selfhost/module/package_told.hero:62` | the package has no such module |
| `package_unread` | fetched | `selfhost/module/package_told.hero:144` | the graph's own refusal of a package |

**Three of fourteen are of the C sense.** Every other code emitted by the C-sense
files is shared with `link` or header heads and says nothing of *package*
(`grep -oE 'code: "[a-z_]+"'` over `head_package.hero`, `head_names.hero`,
`cli/package_*.hero`, `emit/ffi_build.hero`, `emit/ffi_lookup.hero`,
`cli/libraries.hero`): `unwritable_name`, `option_like_name`,
`machine_locked_path`, `ffi_missing_library`, `ffi_missing_header`,
`ffi_missing_link`, `escape_in_header_name`, `undefined_header_name`,
`unshowable_name`. They keep their names.

**Proposed names** (built in the prototype, § 5): `ffi_package` -> `ffi_pkgconfig`
(it keeps the `ffi_` family of `ffi_missing_library`, `ffi_missing_header`,
`ffi_missing_link`); `package_comparison` -> `pkgconfig_comparison`;
`no_pkg_config` **keeps its name**: it is of the C sense and already names the
tool, not *package*. New: `old_package_word`, the retired spelling told (§ 6).
Not `old_package_mark`: *package mark* is already the fetched sense's term
(`selfhost/package_mark.hero`, codes `package_mark_apart`,
`package_mark_unclosed`, `package_mark_holds_a_path`).

## 2. Every place the mark is read, printed or coloured

Measured at the base, read-only (the prototype's `git diff --stat` comes in § 5).

**Volume, both senses together** (`git grep -wic package -- 'selfhost/*.hero'`):
1011 lines in 115 files; 503 of them not comments; 415 lines in 70 files carry
the word inside a string literal (`build/p240/instr.py`, a regex over string
literals, both senses). The C sense is a minority of these; the rename touches
only it, and § 5 counts what it actually moved.

**The mark itself, in the compiler**:

- `selfhost/keywords.hero:83-84` (`WORD_PACKAGE`, `"package"`) and `:140`
  (`contextual()`, which `heroes grammar` prints). The family is closed at
  sixteen (design.md §4.19, line 2669); a respelling keeps it at sixteen.
- the parser: `selfhost/parse/group_head.hero:102,114,116,124,130`
  (`library_word`, `one_library`, the message for a missing string);
  `selfhost/heads_told.hero:68` (`head_word`, which hands the mark's own text
  to `head_names` as `what`).
- the AST: `selfhost/ast.hero:444-454`, `variant Library`, cases `link` and
  `package`. The case `.package` is matched on **16 lines in 12 files**
  (`git grep -nP '\.package(?![a-z_])'`, less `module/placed.hero`'s two lines,
  a fetched-sense record field): `ast.hero:695`, `cli/library_needs.hero:82`,
  `cli/package_missing.hero:164`, `cli/supplied.hero:78`,
  `cli/supplied_words.hero:301,316,340`, `emit/externs.hero:292`,
  `emit/ffi_build.hero:94`, `emit/ffi_lookup.hero:189`, `parse/decl.hero:346`,
  `parse/group_head.hero:102,107,130`, `print/dump.hero:165`, `print/fmt.hero:363`.
- the printers: `selfhost/print/fmt.hero:363` and `selfhost/print/dump.hero:165`
  each write the literal `" package "`, **not** the constant: a respelled
  `WORD_PACKAGE` alone would leave `fmt` printing the old word, that is
  rewriting every program into one the parser refuses. This is the walk's
  first lesson (`.claude/rules/diagnostics-and-goldens.md`) arriving again.
- the head judge: `selfhost/head_names.hero:42,108,131-132,164,222-226`
  compare `what == "package"` and build the advice; `noun(what)` (line 123)
  returns the mark itself for anything but `link`, so under `pkgconfig` a
  message would read "this pkgconfig's name".
- the codes: § 1's three, plus `selfhost/diag.hero:228` (`package_comparison`
  in `is_thesis_rule`'s list, dropped under `--permissive`).
- the driver marker `"heroes-ffi-package "` (`selfhost/emit/ffi_lookup.hero:35-36`):
  40 lines in 6 selfhost files (`git grep -c heroes-ffi-package`), written by
  `cli/libraries.hero`, `cli/package_answer.hero`, `cli/shell_split.hero`, read by
  `emit/ffi_build.hero:279` and `emit/ffi_lookup.hero`; its user-facing form
  replaces it with `"the package "` (`ffi_build.hero:283`).
- `selfhost/cli/shadowed_words.hero:44-45`, origin kind `PACKAGE` (C sense,
  told at :121 *"the package `X` names that folder in its `pkg-config`
  answer"*) beside `FETCHED` (told at :127 *"the package `X` is placed in that
  folder"*): **one note, two senses, printed today**.
- advice that tells the user to write the old spelling (none is a `Fix`; no
  `Fix` writes `package` today, `git grep -n 'Fix(' | grep -i package` finds
  only the fetched sense's `use <x>/m` repairs): `emit/ffi_build.hero:104,252`,
  `head_names.hero:132,222,225,226`, `parse/group_head.hero:124`,
  `cli/package_answer.hero:117-118`, `emit/header_reach.hero:121`,
  `head_package.hero:22,31,67,76`.

**What pkg-config itself calls the thing**, read on this Mac 2026-11-09:
`/opt/homebrew/bin/pkg-config -> ../Cellar/pkgconf/3.0.10/bin/pkg-config`;
`man pkg-config` (pkgconf 3.0.10): *"pkgconf [options] **module** ..."*, *"the
libraries given by the module arguments"*; `pkg-config --cflags
heroes-p240-absent`: *"**Package** heroes-p240-absent was not found in the
pkg-config search path"*, exit 1. **Both of the tool's own nouns are taken in
Heroes** (*module* = one file, *package* = fetched code). Its notes are relayed
verbatim under `ffi_package` (`emit/ffi_build.hero:288-293`), so **a reader
still meets "Package" in a relayed note under a renamed group**; that is the
tool's text, quoted, and no rename reaches it.

**Outside `selfhost/`** (each measured where it is used): the spec (§ 13's
sentence and `Extern` production, spec lines 479 and 482); the editor grammar
(`heroes.tmLanguage.json:76-77`) and `editors/README.md:25,27`; the site's
`highlight.ts:434,468`, `examples-model.ts:179` and the two `the-c-boundary.html`
pages; `examples/raylib/main.hero` (code line 58, comments 6, 9, 54),
`examples/sdl/main.hero` (code line 48; comments 10-11 quote panel 050's
history and stay) and `examples/README.md:87`; six harness files (§ 4); the
goldens (§ 4, § 5); the recovery corpus, measured: **34** `.hero.frozen` files
with an extern head in the old spelling, one line each (35 files and 37 lines
counting comments), **3** lines of `plan-singles.jsonl`, none of
`plan-pairs.jsonl`, and `corpus-messages.jsonl` naming neither the spelling nor
`ffi_package` (`git grep`, `grep -cF`); `seed/heroes.c` (4 lines); and the live
docs listed in § 5.

## 3. Internal names

The critic's split of the twelve `selfhost/` files whose names say *package*
is confirmed by reading each head: **C sense (6)** `cli/package_answer.hero`
(370 lines), `cli/package_macros.hero` (130), `cli/package_missing.hero` (303),
`cli/package_words.hero` (424), `head_package.hero` (80 at the base),
`emit/header_packages.hero` (88); **fetched sense (6)** `cli/package_stand_in.hero`,
`cli/package_reads.hero`, `cli/package_units.hero`, `package_mark.hero`,
`module/package_told.hero`, `module/packages.hero` (`wc -l`, each head read).

**What renaming the six C-sense files costs** (`git grep -lE '^use ...<m>$'`,
`git grep -hoP '(?<![a-z_])<m>\.[a-zA-Z_]+'`): 14 `use` lines and 67 qualified
references in `selfhost/` (`package_words` 29, `package_answer` 10,
`package_macros` 10, `head_package` 7, `header_packages` 6, `package_missing`
5); outside it, `tests/harness/absence.hero:197` names
`selfhost/cli/package_answer.hero` **by path** (its `spellings()`, which the
net's own tests run), and comments in `absence.hero` (8 mentions),
`suite_layout.hero:622`, `.claude/rules/verification.md`'s map row and
`docs/design.md`. The frozen recovery corpus holds copies of
`head_names.hero`, `absence.hero` and `suite_layout.hero` naming them; frozen,
they do not move.

**The identifiers inside**: in the fourteen C-sense files (the six above plus
`emit/ffi_lookup`, `emit/ffi_build`, `cli/libraries`, `emit/externs`,
`cli/library_needs`, `cli/supplied`, `cli/supplied_words`, `cli/shell_split`),
code with comments and string literals taken out holds **293** identifiers
saying *package* (`grep -vE '^\s*#' | sed` strings out `| grep -oiE
'[a-z_]*package[a-z_]*'`): `package` 174, `package_words` 27, `want_package`
18, `packages` 14, `package_problem` 12, `package_flags` 8, `with_package` 7,
and smaller; `cli/libraries.hero` alone holds 139.

**Would a reader of `selfhost/` meet *package* in two senses if they stay?
Yes**, measured: `cli/libraries.hero`'s 139 C-sense identifiers against
`module/packages.hero`'s fetched-sense ones, and `cli/shadowed_words.hero`
printing both senses with one noun until the prototype changed `PACKAGE` to
`PKGCONFIG` (§ 2). **What the prototype renamed internally**: the AST case
(`.package` -> `.pkgconfig`, 16 match lines in 12 files), the constant
(`WORD_PACKAGE` -> `WORD_PKGCONFIG`), `shadowed_words.PACKAGE` -> `PKGCONFIG`,
the three codes and the driver marker (`heroes-ffi-package` ->
`heroes-ffi-pkgconfig`, 40 lines in 6 files). Those are the names a *message*
or a *case* can show. **What it left**: the six file names and the 293
identifiers. My recommendation is to rename them in a separate, mechanical
commit after the tag: they change no behaviour (every name is checked by the
compiler, so a miss is a build error, not a silent drift), they would collide
with every lane that touches `cli/libraries.hero`, and inside the C sense
*package* is pkg-config's own noun (its error text, measured § 2), so the
identifiers are not false, only ambiguous. That commit owes no panel (how the
compiler does it, not what the language does: CLAUDE.md § 4).

## 4. Goldens, precedent, silent readers

**The precedent exists, and it is a surface spelling, not a code.** The
critic searched for an earlier rename of a contextual word and found none;
searching for a *spelling* change instead (`git log --diff-filter=M
-G'#~ [a-z_]+' -- tests/golden/check tests/golden/unsupported`, then each
hit's body) finds **`fcec76d8d`, M-inferred-cell step 2, 2026-10-10, panel
209**: the cell's symbol `@` became `@=`, *"`v: i64 @ 0`, the form until today,
is told once, `old_cell_symbol`, with a certain fix that writes `@=` ... That
fix is how the tree moved: `check --apply --in-place` over every root"*, then a
census of every tracked golden and example by the new compiler; the spec, the
pin (`heroes measure --refresh`, ledger row 107) and `parse/`'s line budget
moved in the same commit. `git show --stat fcec76d8d | grep -c tests/golden`
gives **62** golden files rewritten in place. Its two sequels are precedent
too: **`88077be1a`** (defect 608, 2026-10-11, the old symbol on a wildcard told
again so `fmt` never prints it) and **`a6832251f`** (defect 661, 2026-10-11,
*"the recovery plan cut again from the migrated tree, the old one's record
kept under cut/"*: the frozen plan of 2026-09-30, replayed after the
migration, read **19,186** findings *"every one the migration's"*). And
`tests/harness/suite_compat.hero:127` carries `old_cell_symbol` as a known
change. **So this rename takes that route**: the retired word told with a
certain fix, the goldens moved by that fix, the expected files following, a
`known()` row, and, if R2 reads the rename as findings, the plan re-cut as
`a6832251f` did. Codes before `1.0.0` may be renamed: design.md §4.17's rule
that a code is never renamed is from `1.0.0` in a `1.x` (`87b3b48df`, panel
232's R9).

**What this one owes that `@=` did not**: (a) **columns**: `pkgconfig` is two
characters longer than `package`, so every expected diagnostic located after
the mark on the mark's line moves two columns, and every excerpt's caret line
under such a line moves too (`@=` was one character longer than `@`, so the
same class existed there); (b) **a second sense in the same files**: the
goldens' comments say *package* in both senses, and their header comments
quote history (`unsupported/ffi-records-only-group-cannot-claim-its-package.hero:3`
quotes *"internal error: heroes-ffi-package …"*, a message of the past), so the
prototype rewrites code lines and `#~` marks only and leaves comments as the
record they are (count in § 5); (c) **four golden case NAMES** say *package*
in the C sense (the critic's list); a name is a record and the prototype keeps
them, so `tests/golden/` keeps both senses in its file names whatever the
rename does.

**Readers, measured with the prototype compiler against the unedited reader**
(2026-11-09, `build/p240/proto0-*.txt`):

| reader | what it reads | stops matching | measured |
|---|---|---|---|
| `tests/harness/suite_grammar.hero` | the spec's productions | **in silence**: `grammar` 9 passed 0 failed with the spec's `Extern` still saying `"package"` | run |
| `editors/vscode/syntaxes/heroes.tmLanguage.json:77` | `(link\|package)` | **red**: `spec/colouring` *"declare and ... does not paint ...: pkgconfig"* | run |
| `site/src/lib/highlight.ts:468` | `word === 'package'` position rule; the set from `keywords.hero` (`tables.ts:186`) | in silence, by reading: `pkgconfig` in the set and painted nowhere; `lexed.ts:125` only refuses a painted word outside the set | **unrun** (no `node_modules` in this worktree) |
| `site/src/lib/examples-model.ts:179` | `/\bpackage "([^"]+)"/` | in silence by reading: `pkg` becomes `null` | **unrun** |
| `tests/harness/absence.hero` (`PACKAGE_CODE`, `PACKAGE_HEAD`, `PACKAGE_MARK`, `TOOL_OPEN`) | the compiler's words, pinned | **red**: the net's own tests, 584 tests, 3 failed, one being *"fixedbugs: the spellings are the compiler's own, read out of the files that write them (defect 328)"* | run (`build/p240/proto0-nettests.txt`) |
| `tests/harness/absence.hero:550`, `PACKAGE_WORD " package "` | which package groups a case binds, so a case whose package a machine lacks is skipped there | in silence on this Mac (every package here is present): the net's own test *"a case binds its groups' headers and packages"* went red only because it plants the new spelling; on a leg lacking a package, a skip would become a red | run |
| `tests/harness/suite_golden.hero:906`, `absence.hero:1724` | the compiler's sentence planted at its column, `1:26`, `1:27` | red: the string stands two columns later | run |
| `tests/harness/probe.hero:173` | `needle: "ffi_package"` | **in silence**, but nothing depends on it: no probe row of the net names `ffi_package` (`git grep -n ffi_package tests/harness` lists absence, mark_readers, probe and suite_golden only) | read |
| `tests/harness/suite_emission.hero:633-634` | `" package \""` on extern rows, to hand pkg-config's `--cflags` to clang | **in silence, as a skip**: `SKIP emission/examples/raylib/main/compiles` *"this machine lacks `raylib.h`"*, and the same for `sdl/main`, where the base passed both and `pkg-config --exists raylib` and `sdl3` succeed on this Mac | run (`build/p240/proto1-emission.txt`) |
| **the advice inside messages of cases that hold no mark** | *"or name a `package` and let `pkg-config` answer"* in header and `link` messages | **red**, and not in the brief's count: `check` 746 passed, 7 failed, four of them (`216-a-backslash-in-a-header-name`, `225`, `233`, `250`) holding no mark at all; by the phrases, **27 expected files** (8 check, 1 permissive, 18 unsupported) | run |

The rest of that `check` run, read fragment by fragment (expected against
actual, `build/p240/proto-check.txt`): every difference is one of five
wordings the prototype changed on purpose (the advice, *"into packages and
versions: a group names one package"*, *"a package and no version: write the
package's name"*, and the noun through `noun()`); none is a regression.
`full` 73 passed, 0 failed.

## 5. Run

**The base**, `build/p240/heroes-base` (stage 2 of the base built by stage 1
from the seed: `/usr/bin/time -p`, real 111.72, user 85.19, sys 18.00, beside
other seats' work, so not a timing), each suite `./heroes run
tests/harness/main.hero -- build/p240/heroes-base <suite>`, read whole:

| suite | base |
|---|---|
| check | 753 passed, 0 failed |
| unsupported | 429 passed, 0 failed |
| permissive | 23 passed, 0 failed |
| full | 73 passed, 0 failed |
| annotations | 1343 passed, 0 failed |
| fixes | 1075 passed, 0 failed |
| grammar | 9 passed, 0 failed |
| spec | 35 passed, 0 failed |
| records | 27 passed, **1 failed**: `records/names`, *"tests/harness/suite_latency.hero: M-package"* has no alias row; at the base, not this rename's |
| corpus | 71 passed, 0 failed |
| compat | 1 passed, 0 failed; *"v0.6.0: 79 programs ..., 78 compiled ..., 1 unfinished ...: all ok"* |
| surface | 450 passed, 0 failed |
| emission | 2519 passed, **11 failed**, 1 floor outgrown: all 11 *"nothing blessed"* for defects 2388, 2389, 2395's new `run` cases; at the base |
| run | stopped unfinished at about four fifths (run counter 1787 of roughly 2,200, its own scratch folder), by me, for time; no count |

**The prototype**, built in three steps, each stage 2 from stage 1 of the
seed (the seed needs no refresh: `selfhost/` writes no extern group of its
own, `git grep -cE '^\s*extern "[^"]*" (package|pkgconfig) '` = 0 files):
`heroes-next` (the rename), `heroes-near` (+ the near spellings), and the
final `build/p240/heroes-p240` (the refusals moved out of `parse/`).

| suite | prototype | against the base |
|---|---|---|
| compiler's own tests (`heroes-next test selfhost/main.hero`, first build) | 1989 tests, 3 failed | the 3 were assertions pinning the old noun (`cli/shadowed.hero:398`, `emit/ffi_build.hero:358,455`) |
| **compiler's own tests, final source** (`build/p240/heroes-p240 test selfhost/main.hero`) | **1989 tests, all passed** | |
| **check**, after the phrase script | **753 passed, 0 failed** | the base's 753, 0 |
| **permissive**, after the phrase script | **23 passed, 0 failed** | the base's 23, 0 |
| canonical | 2 passed, 0 failed | not run at the base |
| probe (the fixtures under every family, the tree under `multi`) | 31 passed, 0 failed | not run at the base; the by-hand probe owed after a push touching `selfhost/print/` stays owed |
| surface / special / records | 450, 0 / 14, 0 / 27, 1 (`records/names`, the base's own red) | surface and records the base's counts |
| fixpoint | stage 3 built by the prototype's stage 2; both `build selfhost/main.hero --emit-c`; `cmp` silent, 33,258,356 bytes each | |
| run, narrowed to the 9 cases that hold the mark (7 filters) | 1 + 1 + 3 + 1 + 1 + 1 + 1 passed, 0 failed, 0 skipped: `ffi-a-construction-polls-an-sdl3-event` and `413-libuv` among them, so the new mark asks pkg-config, compiles, links and runs on this Mac | the base's whole `run` was stopped unfinished at about four fifths (my decision, for time); the form compares a program's output and never a message, so the rename reaches it only through these 9 programs, an inference the landing's whole `run` settles |
| grammar | 9 passed, 0 failed | same |
| layout | 6 passed, 0 failed, after moving the refusals out of `parse/` (first try: `selfhost/parse/: 8794 lines of code, past its budget of 8782`; the base stood at exactly 8782, and `code_lines` counts comments, `suite_layout.hero:1030-1045`) | same budget, untouched |
| order | 2 passed, 1 failed: `cli/const_elements.hero:347`, `emit/shadowed.hero:189`, two files this diff does not touch | the base's (the critic saw the round change both) |
| spec | 31 passed, 4 failed: `spec/budget` *"the spec measures 8329 and the record says 8325 — a delta of 4"*, `spec/spendable`, `spec/real`, `spec/ledger` | the four pins any spec edit moves until `heroes measure --refresh` (a paid run, the coordinator's at the landing) and a ledger row; `spec/colouring` green once the editor grammar follows |
| annotations | 1343 passed, 0 failed | same as the base (1343, 0) |
| fixes | 1075 passed, 0 failed | same |
| full | 73 passed, 0 failed | same |
| compat | 1 passed, 0 failed; v0.6.0: 76 compiled, 1 unfinished, 2 refused, *"all ok (2 old_package_word ...); 2 of them healed by their certain fixes alone"* | base: 78 compiled, all ok; the two are the raylib and SDL examples |
| emission (harness unedited, `run/` goldens not yet moved) | 2497 passed, 22 failed | the base's 11, plus 9 `run` cases refused by `old_package_word` until their goldens move, plus `examples/raylib/main` and `examples/sdl/main`, which differ from their blessing **only** at `hero_program_sources` (*"line 443: blessed ... 49d304020269d89a ... now ... 6e2299730926c9b6"*, 449 lines either way): **an edited source re-blesses its emission**, 11 emissions in all |
| emission's silent reader | `SKIP emission/examples/raylib/main/compiles` *"this machine lacks `raylib.h`"*, the same for `sdl/main` | at the base both **passed** (no SKIP line), and `pkg-config --exists raylib` and `sdl3` both succeed on this Mac: **two checks lost and told as the machine's lack**, the critic's d.3 measured |
| unsupported (harness unedited) | 408 passed, 21 failed, every one an advice wording in a case holding no mark (defects 158 x14 *"`package` asks the"*, 232, 2351 *"its `package` groups'"*) | |
| **unsupported**, harness edited, after the phrase script | **429 passed, 0 failed** | the base's 429, 0 |
| **emission**, harness edited, `run/` goldens moved | **2508 passed, 22 failed**, 1 floor outgrown, 4 skips (the base's 4: the examples' `compiles` pass again) | the base's 11 *nothing blessed*, plus **11 to bless again, every one the rename's**: 9 differ only at `hero_program_sources` (the source key), and the two `396-sha256` cases also in byte offsets the emitter writes into its `heroes-ffi-buffer` assertion text (`md 695 697` -> `md 697 699`), two bytes later after a mark two bytes longer; with every number masked, `diff` leaves only the key line |
| **check** with the two new cases | **755 passed, 0 failed** | base 753 + 2 |
| **annotations** with the two new cases | **1345 passed, 0 failed** | base 1343 + 2 |
| **fixes** with the two new cases | **1077 passed, 0 failed** | base 1075 + 2: each new case's `.fixed` checks clean |

**The net's own tests** (`heroes test tests/harness/main.hero`, 584 tests):

| harness | compiler the tests spawn | result |
|---|---|---|
| unedited | `./heroes`, stage 1 of the old seed (`shell.hero:984-989`'s first candidate) | 3 failed: `absence.hero`'s pinned spellings (defect 328, **loud**), the layout walk while `parse/` was over budget, and the latency level |
| edited | `./heroes`, stage 1 of the old seed | 6 failed: the latency level, one needle I had missed (`absence.hero:2196`), and **four tests that build a program in the new spelling with a compiler that refuses it** (defects 328, 198 twice, 528) |
| edited | `HEROES_COMPILER=build/p240/heroes-p240` | 2 failed: the latency level and the same missed needle |
| edited, needle fixed | `HEROES_COMPILER=build/p240/heroes-p240` | **584 tests, 1 failed**: the latency level alone |

The latency test (*"the newest row under the level passes ... (decision
2282)"*, `suite_latency.hero:314`) fails in every run and no file this diff
touches is its input. **The seed**: the CI names stage 2 for these tests
(`ci.yml:851-858`, `HEROES_COMPILER: ./heroes`, built by the seed's stage 1
from `selfhost/`), so a stale seed does not redden the CI; but the local
checkout CLAUDE.md § Commands builds `./heroes` from the seed, and there four
of the net's own tests fail until the seed knows `pkgconfig`. The bounded
seed's rule refreshes it when the two-stage build goes red or at a release
(`seed/README.md:128-140`); this is a third case it does not name, a form the
**harness** compiles. I recommend the landing refresh the seed in its commit
(route `one-stage`, measured: `./heroes`, stage 1 of the old seed, and
`build/p240/heroes-p240` each `build selfhost/main.hero --emit-c` from the
prototype's sources, and `cmp` of the two files is silent, 33,258,356 bytes
each; that file is the refreshed seed).

**The two new golden cases** (`tests/golden/check/`, each `.hero` annotated
`#~`, `.expected` from `check --brief`, `.fixed` from `check --apply`, read
line by line before they were kept): `ffi-the-old-package-word-is-told-with-its-fix`
(two groups, two `old_package_word`, the fixed program exit 0) and
`ffi-the-tools-own-spelling-is-told-with-its-fix` (`pkg-config`, `pkg_config`,
`pkgConfig`, `PkgConfig`, four `pkgconfig_spelled`, the fixed program exit 0).
A first draft of each held a second mistake with no fix and calls of `cstr`
functions with string literals, so its fixed text did not check; rewritten,
since `.fixed` must.

**The whole prototype**, `git diff --stat`: **110 files changed, 431
insertions, 342 deletions**, plus 6 new files (two golden cases, each `.hero`,
`.expected`, `.fixed`); all 116 are in `compiler-engineer.diff`. By area
(`git diff --numstat`, summed): `selfhost/` 28 files +257 -169;
`tests/golden/check` 23 files +56 -56; `tests/golden/unsupported` 28 +37 -37;
`tests/golden/permissive` 6 +13 -13; `tests/golden/run` 9 +9 -9;
`tests/harness/` 6 +38 -37; `site/` 4 +9 -9; `examples/` 3 +6 -6;
`editors/` 2 +4 -4; `spec/` 1 +2 -2. Of `selfhost/`'s net +88 lines, the two
refusals and their comments are `head_package.hero`'s +89 -7; the rest is
respelling at equal length. **Not in the prototype**, left to the landing and
counted in § 2 and § 4: `docs/design.md` (6 lines), `.claude/rules/platforms.md:42`,
`docs/metrics/trap-families.md:62`, `M-core-packages.md:32`, `ci.yml`'s 5 comment
lines, the ledger row and the pin (paid), 11 emissions to bless again, and
`seed/heroes.c` (4 lines naming the old codes and marker), which the bounded
seed's rhythm refreshes at the next release, not here.

**The spec's cost on the vendored instrument: +4 tokens (8325 -> 8329)**, from
`spec/budget`'s own message; the real count is unrun (no paid run tonight).

## 6. The old spelling after the rename

Built and run (prototype stage 2, `build/p240/try/`, 2026-11-09 ~22:05):

    extern "zlib.h" package "zlib"      # old.hero

`heroes-next check old.hero`, exit 1:

    error[old_package_word]: `package` named what `pkg-config` is asked about until panel 240; a package is now only code others publish, reached by `use <name>/module`, and a group names its library for `pkg-config` with `pkgconfig`, as `pkgconfig "raylib"` — write `pkgconfig`
      at old.hero:1:17
      fix (certain): write `pkgconfig`

`check --apply --in-place` rewrote it to `pkgconfig "zlib"`, which then checks
at exit 0; `fmt` refuses the old file (exit 1, *refusing to format a file with
diagnostics*) and round-trips the new one byte for byte (`fmt new.hero | diff`
empty); `heroes-next run new.hero` asked pkg-config for zlib and printed `ok`;
`heroes-next grammar` prints *Contextual words (16)* with `pkgconfig` where
`package` stood. The base compiler checks `old.hero` at exit 0 and refuses
`new.hero` with `expected_end_of_line` at 1:17.

**Is the refusal owed?** CLAUDE.md § 2: before 1.0.0 *"a change that refuses a
0.x program is told, never undone for it"*. The old word is not kept: it is
refused at exit 1, and the head read on as the `pkgconfig` group it is so the
rest of the file is judged, exactly `old_cell_symbol`'s shape
(`selfhost/cell_errors.hero:19-38`, panel 209, `fcec76d8d`). Without it the old
spelling gets `expected_end_of_line` (*"found a name (`package`)"*), true but
naming no new spelling and offering no fix, which design.md §4.17 refuses for a
mistake whose repair is known. And `tests/harness/suite_compat.hero:40-45`
makes it owed in a second way: its `known()` table excuses a release's refused
program only by a code that names the change, and *"a code that names a whole
class (`type_mismatch`) is never listed"*, so a rename told by
`expected_end_of_line` could never be marked a known change.

**Measured on the release** (`./heroes run tests/harness/main.hero --
build/p240/heroes-p240 compat`, with the `known()` row the prototype adds):
*"v0.6.0: 79 programs of examples/ and tests/thesis/, 76 compiled by 0.6.0's
compiler, 1 unfinished, ..., 2 refused, each by a change 0.x made on purpose:
all ok (2 old_package_word, ...); 2 of them healed by their certain fixes
alone"*. The base read *"78 compiled ..., 1 unfinished ...: all ok"*. So the
rename refuses exactly two released programs, the raylib and SDL examples, and
both heal by `--apply`.

**The sentence the rename exists to fix** (the critic's d.8, a fetched
package whose own groups use the mark) now reads, in the prototype's
messages: *"a unit of the fetched package at `...` looks for a header: the
package's own root, the runtime's directory, the toolchain's own and the
folders its `pkgconfig` groups' answer names"* (`cli/package_units.hero:276`,
`cli/package_reads.hero:330`): one word per sense in one sentence.

**What it costs**, final (`git diff --numstat` on the three files, which also
carry the rename's respelled lines): `selfhost/head_package.hero` +89 -7, now
162 lines (`wc -l`): the constant `RETIRED`, `retired()`, `near()`,
`near_width()`, `read_word()` and their comments, and the file's head and four
messages respelled; `selfhost/parse/group_head.hero` +10 -10, **net zero**
(one `use`, the third clause of a condition, the `is_package` binding dropped
for `word != keywords.WORD_LINK` at its two uses), so `parse/` stays at
8782 of its 8782 (`layout` green; a first placement inside `parse/` read
8794, then 8784, then 8783, the last being a comment line, which `code_lines`
counts); `selfhost/heads_told.hero` +7 -1 (the old word's and a one-token near
spelling's string judged once, as a `pkgconfig` string). One `known()` row in
`suite_compat.hero` and two golden cases (§ 5). No new construct: the AST gains
no case, the checker and the emitter see a `.pkgconfig` library exactly as they
saw `.package`.

**What it does not catch**, run the same way (`build/p240/try/near.hero`):
`pkg-config "zlib"`, `pkg_config "zlib"`, `pkgConfig "zlib"` and
`Package "zlib"` each get `expected_end_of_line` (*"found a name (`pkg`)"*,
etc.) with no fix, as any unknown word in that position does at the base. The
first matters: **`pkg-config` is the tool's own spelling and is not one
token**, so a mark named after the tool invites a spelling the lexer splits.
`package` invited no such neighbour. See § 7 and the prediction.

**So the prototype also tells the near spellings** (built second,
`build/p240/heroes-near`, then the prototype's compiler): `pkgconfig_spelled`,
one error with a `certain` fix writing `pkgconfig`, for `pkg-config` (three
tokens touching), `pkg_config`, `pkgConfig` and `PkgConfig`; run on each:
one `error[pkgconfig_spelled]`, `fix (certain): write pkgconfig`, and after
`check --apply --in-place` the line reads `extern "zlib.h" pkgconfig "zlib"`
and checks at exit 0. `pkg - config` with spaces and a bare `pkg` keep
`expected_end_of_line`, as the fetched mark reads only touching tokens
(`selfhost/package_mark.hero:1-4`). Cost, measured when it was first added
(`git diff --numstat` against the first build): `head_package.hero` +34,
`parse/group_head.hero` +12, `heads_told.hero` 0 more (the same line widened),
about 46 lines; in the final placement the parser's share moved into
`head_package.read_word`, so `parse/` carries none of it (below). It is a new
code, so it is this sitting's to adopt or refuse; I recommend it as the
condition of `pkgconfig` (§ 7).

## 7. The name

**From the compiler's input: `pkgconfig`, with the near spellings told.**

What the compiler can measure about each candidate (`git grep -wic <w> HEAD
-- 'selfhost/*.hero'`, the spec the same, `keywords.hero` and
`foreign_words.hero` the same, at the base):

| candidate | one token | selfhost lines already using the word | spec lines | what it already names in the compiler |
|---|---|---|---|---|
| `pkgconfig` | yes | 2 (`--atleast-pkgconfig-version`, a test value) | 0 | nothing |
| `pkgconf` | yes | 16 (comments naming the implementation's versions) | 0 | the implementation Homebrew and vcpkg ship, not the command the compiler starts |
| `system` | yes | 113 | 1 | the toolchain's own headers (`cli/system_paths.hero`) |
| `library` | yes | 1086 | 2 | what `link` names, `variant Library`, `--library <dir>`, the noun of every `link` message |
| `pc` | yes | 115 | 0 | the `.pc` file (`machine_locked_path`'s subject) |
| `machine` | yes | 288 | 0 | "the machine is asked" in the AST's own comment, and a hundred other things |
| `pkg-config` | **no**, three tokens | | | |

1. **It names exactly what the compiler does.** The compiler starts a process
   called `pkg-config` (`selfhost/cli/libraries.hero:290-291`: `program:
   "pkg-config"`, words `pkg-config --cflags --libs -- <name>`;
   `no_pkg_config` at :308 when it cannot start) and does nothing else for this
   mark. `system` and `machine` claim a generality the implementation does not
   have; `library` makes `link "sqlite3"` and `library "raylib"` read as
   synonyms and shares a noun the diagnostics already use for `link`
   (`head_names.noun`); `pkgconf` names the implementation and not the
   protocol (the compiler asks for `pkg-config` "by that name",
   `.github/workflows/ci.yml:454-462`); `pc` names the file the compiler refuses
   (`machine_locked_path`).
2. **Both of the tool's own nouns are taken.** pkgconf 3.0.10 calls its
   argument a *module* and its error says *Package* (§ 2, run). Only the tool's
   name is free, so a mark that says what it asks cannot borrow the tool's noun;
   the messages say *library*, which the same man page uses ("the libraries
   given by the module arguments").
3. **Its one cost is the near spelling**, which § 6 measured and the prototype
   closes for 46 lines. Without that, `pkg-config "raylib"` gets
   `expected_end_of_line` with no fix, and I expect a model to write it.
4. **The noun in messages changes in every candidate**, since `noun()` returns
   the mark itself: under `pkgconfig` it is one line (`head_names.hero`, `noun`
   returns *library* for both marks), measured in § 5.

**The conservative route** (the critic's d.2), recorded as CLAUDE.md § 4 asks:
keep the mark `package` and change only the prose. It costs nothing in the
compiler and leaves the collision the author ruled against; the compiler's own
`shadowed_words` note already prints both senses with one noun (§ 2), so prose
alone does not reach the place where a user meets the collision. **Folding into
`link` with a qualifier** (`link pkgconfig "raylib"`) removes a contextual word
but makes the AST's two-case variant a link with a flag, and adds a token the
parser must read in a second position; no measured benefit over a respelling.

## 8. Verdict, in the seat's structure

- `verdict`: **approve, with conditions** (the shared brief's *adopt with a
  condition*). No veto: the rename adds no core construct and breaches no
  ceiling, measured below.
- `section`: design.md §1.7 (core plus elaboration: no new construct, the AST's
  `variant Library` keeps two cases, renamed), §4.19 line 2669 (the contextual
  words are one closed family of sixteen; still sixteen, `heroes grammar`
  run), §4.17 (a mistake whose repair is known is told with a `certain` fix:
  the retired word and the tool's own spelling). design.md does not cover
  whether a contextual word may be **renamed**; the precedent is `fcec76d8d`'s
  surface change, not a design.md rule.
- `implementation_cost`: see § 5's final `git diff --stat`. The refusals are
  in `selfhost/head_package.hero` (80 lines at the base), the parser's change
  is net zero lines in `selfhost/parse/` (8782 of 8782, `layout` green), and
  the rest is respelling: 28 `selfhost/` files in all for the mark, the AST case, the
  codes, the marker and the noun; the spec (+4 vendored tokens); the editor
  grammar and README; two site readers and two pages; two examples; six harness
  files; and the goldens (§ 4).
- `needed_for_self_hosting`: **no** (Principle 0). `selfhost/` writes no extern
  group of its own (0 files), so the compiler compiles itself identically under
  either spelling; the rename is the author's ruling on one name for one thing,
  not compiler-need.
- `argument` (under 120 words): A respelled contextual word, not a construct:
  the AST keeps two cases, the checker and emitter treat `.pkgconfig` exactly
  as `.package`, the family stays sixteen. Built, it is 116 files, `parse/`
  held at 8782 of 8782, the compiler's own tests 1989/0, the fixpoint
  identical, every golden form at the base's counts plus two new cases, the
  net's own tests 584 with only the base's latency red.
  The strongest case against it is the name's own: pkg-config's nouns
  (*module*, *Package*) are both Heroes words, and its spelling `pkg-config`
  is three tokens, so the mark invites a mistake `package` never did; the
  prototype tells it with a certain fix. And the rename stops at the surface:
  293 identifiers and six file names keep *package* in the C sense.
- `prediction`: **At the landing round's gate, panel 187's R2 over the frozen
  recovery corpus reads no finding caused by this rename**: each of the 34
  frozen programs that hold `package "..."` gains exactly one message of its
  own per group, `old_package_word`, which R2 subtracts as the program's own
  (`mutate/differential.hero:16-18`). Falsified by any R2 finding that names a
  mutant of one of those 34 programs and no other cause; then the plan is
  re-cut from the migrated tree as `a6832251f` did.
- `condition`: My verdict turns to **object** if the sitting adopts `pkgconfig`
  **without** `pkgconfig_spelled` (a mark named for a tool whose own spelling
  gets a fix-less `expected_end_of_line`), or adopts `library` (1086 selfhost
  lines already use the word, `link`'s noun, `variant Library`, `--library`).
  It turns to an unconditional approve if a held-out thesis run shows models
  writing `pkgconfig` and never `pkg-config` after reading § 13; and to a
  **veto** only if the landing's whole net shows a case changing verdict that
  no rewritten expected file explains, which none of the forms run here did.

## 9. What I could not measure, as questions

1. **The site's build** (`npm run build` in `site/`): no `node_modules` in this
   worktree, and installing would fetch from the network. Does `lexed.ts` hold
   the edited `highlight.ts` to `heroes lex`, and does the unedited one paint
   `pkgconfig` nowhere in silence, as I read it?
2. **The real-token count of the spec** (`heroes measure --refresh`, a paid
   run). The vendored count moves +4 (8325 -> 8329, `spec/budget`'s own
   message); what does `claude-opus-5` say?
3. **Panel 187's R2 over the frozen recovery corpus**: 34 `.hero.frozen`
   programs hold an extern head in the old spelling (one line each) and 3
   lines of `plan-singles.jsonl` write it (measured, § 2). Does R2 read zero findings once each
   program's own `old_package_word` is subtracted (`mutate/differential.hero:16-18`),
   or does the plan need re-cutting as `a6832251f` did after `@=`?
4. **The CI's legs**: Linux and Windows run the raylib and SDL examples, and
   the `unsupported` package cases, through a different `pkg-config`. Does any
   leg tell `ffi_pkgconfig` with a wording this Mac never produces?
5. **The formatter's probe** (`heroes probe` over the three roots), owed after
   the push because `selfhost/print/fmt.hero` moved a literal; unrun.
6. **Whether a model writes `pkg-config`**: the near-spelling refusal is built,
   but how often a model reaches for the tool's hyphenated name after reading
   § 13 is a thesis question no instrument here has asked.
7. **The internal rename's cost in merges**: 293 identifiers and 6 file names
   in files the round's lanes also touch; how many conflicts would one
   commit cause tonight?
8. **The sealed held-out thesis suite** (`ci.yml:1143-1160`, graded by stage 2,
   *"every reference solution green in the strict arm and in the control
   arm"*, its digests pinned in `hashes.txt`): does any sealed reference bind a
   library with `package "..."`? If one does, the rename turns that job red and
   the sealed file cannot be edited without its key. The open `tests/thesis/`
   references at `v0.6.0` hold none (`compat`: its 2 refused programs are the
   two examples).
9. **The sealed spec copies** `tests/thesis/contexts/spec-without-productions.md`
   and `spec-with-one-worked-diagnostic.md` keep `package "raylib"` (their
   sha256 is pre-registered). A thesis run that hands one to a model on a task
   binding pkg-config would measure the rename's refusal, not the model: should
   such a run be told so, or the task set checked for pkg-config first?
