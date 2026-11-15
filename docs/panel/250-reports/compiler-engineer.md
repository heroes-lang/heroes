# Panel 250, the compiler-engineer's report

Status: **written**, 2026-11-15, from 00:07 to about 02:15 CET, stopped from about 00:40 to 01:47 by an
API rate limit. Every number below names the run that produced it, in my folder.

**Said first, in the shared brief's words.** The route I would adopt, placement left whole (W),
changes no program, no catalogue, no `use` line, no message a reader acts on, no help line and no
sentence of the spec. **The route I built to cost the question, M with F1 in the variant H, changes
what a program meets, the messages a reader acts on and the verbs' help, and no catalogue, no `use`
line and no sentence of the spec**, measured: a correct `use` written after the fetch is refused, 17
of 17 programs, with a new diagnostic class `module_unplaced`; the help of `add` and `fetch`; what a
placed tree holds (215 files where W places 540); and the listing moved out of place is told as 21
files *"taken out"*.

## Verdict

- `verdict`: **object** to M, G and H as the sitting's resolution and as the way to feature 2315.
  **No veto**: built and run, no edit of the falsifier's or of the route's own made a program compile
  another module, and none changed a placed tree in silence (§ Log, the falsifier). **Approve W**,
  placement unchanged. On R2's ruler: **no placement route meets it for `core` 0.5.0, in any unit**
  (route 9 shrinks bytes and code lines only by counting none of the package as the compiler's, and is
  objected), so the sitting cannot unblock 2315 by placement; whether R2 states its unit or counts reached modules is
  the spec-warden's reading, and from the ceiling a ruler counted by reach is M's count for no line of
  compiler code.
- `section`: design.md §1.1 (the ceiling), §1.7 (*does it remove a special case?*: M adds one, a second
  way to hold a placed tree, and a diagnostic class), and §1.11's R2 sentence (`docs/design.md:575-581`
  in the base, *"its own tree, placed files counted, is smaller after the step than before"*).
- `implementation_cost`: M with F1 (variant H), built as `routeM/` v2: **12 files, +421 −32**;
  `selfhost/fetch/wanted_set.hero` +121 (new, the closure from every `.hero` of the program's folder,
  through `module/paths.uses_of`), `selfhost/module/subset.hero` +107 (new, the listing beside the
  folder, F1's check, `module_unplaced`'s question), `selfhost/fetch/staging.hero` (448 lines at the
  base) +103 −13, `fetch/placing.hero` (71) +32 −3, `fetch/git_source.hero` (398) +17 −7,
  `fetch/leftover.hero` (301) +11 −1, `fetch/index_source.hero` (475) +7 −4,
  `module/package_told.hero` +8, `module/packages.hero` (416) +6, `module/fingerprint.hero` (244) +4,
  `fetch/add_wanted.hero` +3 −2, `cli/table.hero` +2 −2: **+389 lines, +294 code lines** of the
  `layout` suite's unit. Not built and owed beyond it: tests of M's own behaviour, a closure across
  packages, `offered_modules` reading the listing, a case listing the files placed (the `fetch` form
  prints folders, `tests/harness/suite_fetch.hero:452-456`), `module_unplaced`'s golden and annotation,
  the listing-moved message, re-placing 18 programs and the site. **W with R2 counted by reach: 0
  lines of compiler**, its instrument unbuilt.
- `needed_for_self_hosting`: **no**. The compiler builds itself with no package (stage 2 at the base,
  `sources 5bac791363384945`); 2315 is an acceptance condition, CLAUDE.md § Precedence rank 5.
- `argument`: Panel 242's R2 is why the author convened this. Measured over `core` 0.5.0, the reached
  set does not satisfy it either: the 15 live copies route (A) deletes net −46 code lines, and
  `strings`' two modules add 166, so M, G, route (C) and a ruler counted by reach all leave the
  compiler +120 code lines, +406 lines; W leaves +4,165. Placement is not what fails R2; the copies are
  too few. M then costs the compiler +294 code lines of its own, a module cycle answered by a function
  value, a refusal 17 of 17 programs meet and W never does, and a placement that depends on the order
  `add` and `use` ran.
- `prediction`: at M-core-packages' close, for the newest `core` the index then serves, the compiler
  taking `strings` with every live copy of its contract deleted leaves `selfhost/` plus its reached
  placed modules **larger than at the step's base in the `layout` suite's code lines** (+120 at 0.5.0
  here, by `units.py`'s mirror), under every placement; falsified by one measured step that shrinks it.
- `condition`: (1) a live copy of a `core` contract measured crashing or answering wrong on an input a
  caller reaches: R2 then admits route (A) under W, and I approve it with no placement change; (2) a
  count at a later base where the reached-grain step shrinks the compiler's tree while W grows it: M
  would then carry a case, and I would still ask for R2 counted by reach first, since it costs no
  compiler code; (3) a program measured needing a subset placed for a reason other than R2.

Folder: `.claude/worktrees/p250-scratch/250-compiler-engineer/`, its `tree/` a `git archive` of
`48f53b3437c6cd46aba5923f4f4780ac7b347283`.

## Where the brief is wrong

1. **`00-shared.md:87-88`**, *"The worktree `.claude/worktrees/p250` still stands at `722ae76bf`"*:
   `git -C .claude/worktrees/p250 log --oneline -1` printed `48f53b343` between 01:51 and 01:53 (`uptime` before it, `date` after; the
   spec-warden found the same at 23:51). And `b37-round` no longer heads at `48f53b343`: `git
   rev-parse --short b37-round` printed `478097ea2`; my copy is the archive of `48f53b343`, so
   nothing here moved with it.
2. **The bound and the instrument the brief asks for conflict.** `00-shared.md` bounds every
   compile and `check` by `timeout -s KILL 300` and asks for instructions retired by `/usr/bin/time
   -l`; `/usr/bin/time -l timeout -s KILL 300 heroes-next check selfhost/main.hero` printed
   **16,813,356** instructions (`instr-compiler-timeout-wrapped.txt`), `timeout`'s own process, where
   the same `check` unwrapped prints **131,342,460,263**. Every count below is of the compiler
   unwrapped (a `check` ends on its own); the one `fetch` count I took wrapped (16,634,837) is
   `timeout`'s and is not used.
3. **Route (A)'s arithmetic has turned**: the brief carries panel 242's *"14 lines fewer"* for the
   compiler's own modules; at this base the same move writes **33 lines more** (157 importers of
   `cli/strings` against 125), so the reached set's best case is worse than the brief's figures
   suggest, not better. Not an error of the brief, which asks for the re-measure.
4. **`00-shared.md` § Panel 246's R2**: *"A `fetch` that adds the newly reached files to a placed
   tree, or removes the newly unreached, writes into a placed tree"*. The route I built does not
   write into one: it verifies the standing part against its listing, removes it whole, and renames
   a freshly staged part into place (`fetch/staging.hero` in `routeM/`), so the act R2 governs, an
   edit discarded, cannot happen to a part that verifies, and a part that does not verify is refused
   as today. Measured below.
5. **`compiler-engineer.md` § What you measure, 3**, lists `edited`, `placed_beside` and `standing`
   among what M changes: the route built changes none of the three (`git diff --no-index --numstat`
   names neither), since a listing beside the folder is a file `placed_beside` (folders only,
   `module/placed_beside.hero:109-134`) and `leftover` (`.fetching-` names only) pass by. It does
   change `leftover.taken_away`, which held a dead run's tree to its whole sum and would have left a
   killed run's part placed.

## Log of runs

In the order run. Only the times `date` or `uptime` printed are written (00:07, and 00:26 when the
log was corrected: clock times first written here for the entries between were not read from a
clock, and were taken out).

- 00:07 `uptime`: load averages 7.28 75.51 105.09.
- 00:07 archive of `48f53b343` extracted into `tree/` (running).
- seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` exit 0 (`seed.log`).
- stage 2: `./heroes build selfhost/main.hero -o heroes-next` exit 0, `heroes-next --version`
  prints `heroes 0.6.0`, **`sources 5bac791363384945`**: on the base. `903,603,412,857 instructions
  retired` (`/usr/bin/time -l`, `stage2.log`); load 25 to 200 during it, no duration is a measurement.
- **reach, re-run in my copy** (`reach/lines48-ce.sh`, the coordinator's `lines48.sh` with my
  tree's paths, output `reach/lines48-ce.txt`): **18 of 18 programs `same`**, every `build --emit-c`
  exit 0 and every stderr empty. The brief's figure holds.
- **route (A) re-built at this base** in `routeA/` (an APFS clone of `selfhost/`, `runtime/`,
  `packages/`): `heroes-next add core --index file://../packages -C selfhost` exit 0, *"added core
  0.5.0"*, 30 files and 6,574 lines placed, a 5-line `selfhost/heroes.toml`. Panel 242's
  `route_a.py` (copied to `p242/route_a_250.py`, its live list less `cli/clang_died.has_text`, which no
  longer exists at the base: `grep -c '^function has_text('` 0) moved **804 calls** (524 of
  `cli/strings`' three, 45 inside the copies' own modules, 235 in others) and deleted **15 functions,
  165 lines** (`routeA-phase1.out`); `fix_leftovers` 14 more calls; `cli/strings`' tests of the three
  removed (50 lines) and 9 dead `use` lines the checker named; `heroes-next check selfhost/main.hero
  --brief` **exit 0**; `heroes-next fmt --in-place` on the 172 touched modules, 0 failures.
- **The compiler's own modules under route (A)**: `git diff --no-index --shortstat tree/selfhost
  cmpA1/selfhost` (catalogue and placed tree left out) -> **172 files changed, 1200 insertions(+),
  1167 deletions(-): 33 lines MORE**, against panel 242's 14 fewer at `1edabb957`. The swap now
  writes **170 `use <core>/strings` lines and 168 blank lines** (`grep -c` over the diff), because
  `cli/strings` has 157 importers here against 125 there.
- **route (C) re-built** (`routeC/`: `core`'s `strings.hero` and `strings/internal/scan.hero`
  copied to `selfhost/`'s root, every `use <core>/strings` rewritten `use strings`, the blank line
  after it dropped, `fmt --in-place` on the touched modules, 0 failures; `check selfhost/main.hero
  --brief` **exit 0**): the importers' diff `git diff --no-index --shortstat tree/selfhost
  cmpC/selfhost` -> **172 files changed, 1033 insertions(+), 1167 deletions(-): 134 fewer**, and the
  copy adds 368: **+234 net** (panel 242: +215).
- **every unit, by `units.py`** (my script: `wc -l`'s newlines, bytes, files, and a mirror of
  `tests/harness/suite_layout.hero:1034` `code_lines`, a non-blank line outside a `test "` block,
  comments counted): base `selfhost/` 221,820 lines, 10,109,788 bytes, 150,532 code lines, 849
  files; route (A)'s modules 221,853 / 10,104,662 / 150,486 / 849; route (C)'s 221,686 / 10,103,305 /
  150,486 / 849. Placed whole: 6,574 / 253,311 / 4,211 / 30. `strings` reached (`strings.hero` +
  `strings/internal/scan.hero`): 368 / 14,034 / 166 / 2; its card 21 / 1,941 / 0 / 1; `list` 30 /
  2,663 / 0 / 1; the catalogue 5 / 148 / 0 / 1. The compiler's placed key here is
  `core@0.5.0-683195a84f2d9e81`, not the examples' `f58b55a0517ed0eb`: the key hashes the `index`
  line's spelling (`file://../packages` against `file://../../packages`).
- **the other `strings` contracts, enumerated from the tree and read** (`fnsize.py`,
  `body.py`): `(str) -> [str]` functions, 98, of which the 30 whose name says line, word, field or
  split read whole: **0 copies of `split_lines`** (core's drops a `\r` before `\n`; every one of the
  13 line splitters keeps it, and several also differ on `""` or a trailing `\n`), **0 copies of
  `fields`** (core's splits on Unicode white space; the six word splitters on space and tab, or
  quotes); `(str) -> str` functions whose name says trim, strip, space or blank, 22 read: **0 copies
  of `trim_space`** (the closest, `mutate/spaces.stripped`, is Python's `strip`, which also takes
  U+001C to U+001F off); `(str, str) -> <record>?`: **none of `split_once`'s shape**. A negative
  sentence, so the search is the claim: shapes searched as named here, bodies read, nothing run.
- **The other faces' reached sets** (`faces.sh`): `strconv` 2 modules, 121 lines, 49 code lines;
  `slices` 3, 427, 218; `encoding/json` 8, 3,715, 2,815; `net` 3, 698, 375; `net/http` 12, 2,283,
  1,178. Their live copies in `selfhost/`: `atoi`, the six `(str) -> i64?` number readers read, **0**
  (sign, leading zeros, `_`, spaces, a marker: each differs); `sort_by`, every `sorted*` function,
  **5 functions, 76 lines** with their comments; JSON, the 7 files whose name holds `json`, **911
  code lines** whatever they do. Each face's placed code lines exceed every line of copy it could
  delete, so no face pays for itself.
- **a records-only module** (`probe-records/`: a local index publishing `rec` 0.1.0, `shapes.hero`
  two records and nothing else, `other.hero` one function; `main.hero` builds a `shapes.Box`): `add`
  exit 0, `build --emit-c` exit 0, **7 `#line`s, none naming `shapes.hero`**, its records emitted as
  `h_1recshapes_Point` and `h_1recshapes_Box`. The provenance key, asked through a `path` entry
  (`app2/`): `2cba02be6b87ad7d`, the same as the placed build's; unchanged after a line added to the
  unreached `other.hero`; **`7d2786627279b83a` after a line added to `shapes.hero`**. So the
  `#line` instrument misses a records-only module and the provenance key does not: the key is the
  instrument for *reached*.
- **today's refusal of a pruned tree, re-run** (`pruned/`): `ini` kept to `strings.hero`,
  `strings.md`, `strings/internal/scan.hero`: `check` **exit 1** `error[package_unread]` at
  `main.hero:28:5`, the tree `sha256-ae99678252e9...` against the sum `sha256-38e61943...`;
  `fetch` **exit 1** `error[sum_mismatch]` *"`encoding/json.hero` taken out, ... `net.hero` taken
  out and 17 more; the change is yours, so nothing was placed over it"*, 3 files left. `server`
  kept to its 12 reached modules and 5 cards (17 files): `check` **exit 1** `package_unread` at
  `main.hero:33:5` (`use <core>/net`), the tree `sha256-26da4e5a3fa2...`; `fetch` **exit 1**
  `sum_mismatch`, *"... `slices.hero` taken out and 3 more"*, 17 files left. The brief's figures
  hold, and `server` refuses the same way.
- **Route (A) built in two stages** (`routeA-stage2.log`, `routeA-stage3.log`): stage 2 by the seed's
  compiler, `ce-a2`, exit 0, **`sources 51cff5cc2f0d6f8c`**, `931,358,518,129 instructions retired`;
  stage 3 by `ce-a2`, `ce-a3`, exit 0, the same `sources`.
- **Route M with F1 built far enough to run** (`routeM/`, a clone of `selfhost/` and `runtime/`):
  `heroes-next check selfhost/main.hero --brief` **exit 0** after five rounds. **The language refused
  the first shape**: `error[module_cycle]` *"parse uses module/offered_modules uses module/packages
  uses fetch/leftover uses fetch/staging uses fetch/wanted_set uses module/paths uses parse"*
  (`routeM-check1.log`). A fetch-time closure that reads `use` lines with the real lexer (panel 031's
  R8, *"a prefix scanner would be a second grammar"*, `selfhost/modules.hero:13-16`) cannot be
  imported by `fetch/staging`, `fetch/index_source` or `fetch/git_source`, all three in discovery's
  own closure (`module/packages` uses `fetch/leftover` and `module/placed_beside`, which use
  `fetch/staging` and `fetch/index_source`); it is handed down as a function value from
  `fetch/placing` and `fetch/add_wanted`, which are not. The diff, `git diff --no-index --numstat
  tree/selfhost routeM/selfhost`: **12 files, +414 −32**: `fetch/wanted_set.hero` +121 (new),
  `module/subset.hero` +100 (new), `fetch/staging.hero` +103 −13, `fetch/placing.hero` +32 −3,
  `fetch/git_source.hero` +17 −7 (its tests' calls), `fetch/leftover.hero` +11 −1,
  `fetch/index_source.hero` +7 −4, `module/package_told.hero` +8, `module/packages.hero` +6,
  `module/fingerprint.hero` +4, `fetch/add_wanted.hero` +3 −2, `cli/table.hero` +2 −2. In every
  unit (`units.py`): **+382 lines, +15,654 bytes, +288 code lines, +2 files**. It has no test of its
  own behaviour beyond `own_module`'s, no closure across packages (an entry reached only through
  another package is placed whole), and leaves `offered_modules` reading the placed tree.
- **Route (A)'s fixpoint**: `ce-a2 build selfhost/main.hero --emit-c` and `ce-a3`'s, both exit 0,
  **`cmp` exit 0**: 35,574,612 bytes, 340,804 lines, 71,699 `#line`s (`wc -c`, `wc -l`, `grep -c`);
  **13 `#line`s name the placed folder**, 10 `.../core@0.5.0-683195a84f2d9e81/strings.hero` and 3
  `.../strings/internal/scan.hero`, as at 0.2.0.
- **The package manager under route (A)** (`selfhost/fetch/` and `selfhost/module/`): 20 files, +93
  −72, **21 lines more**; 52 calls move to `<core>/strings`, **no function of either folder is
  deleted** (none of the 15 copies lives there). `units.py`: `fetch/` 6,054 to 6,069 lines, 3,813 to
  3,815 code lines; `module/` 4,986 to 4,992, 3,137 code lines either way. Its one sort,
  `fetch/catalogue_edit.sorted` (15 lines, by the names, correct on a catalogue's unique names), is
  the one helper of the package manager 0.5.0 could replace, through `slices` (218 code lines placed).
- **Stopped at about 00:40 by an API rate limit; resumed at 01:47** (`date`), load averages 3.32
  2.53 3.44. The M prototype's build started before the stop had finished: `routeM-build.log`
  *"wrote ce-m2"*, exit 0, **`sources 063273fe7295b637`**, `894,818,719,962 instructions retired`;
  `ce-m2` dated 00:36; `pgrep -f 250-compiler-engineer` empty. Not rebuilt.
- **Every program re-placed under route M** (`mrun.sh ce-m2`, output `mrun-m.txt`, each program a copy
  of the base's holding no placed tree, `mrun/packages` the index): **18 of 18 `fetch` exit 0**,
  each placing `core@0.5.0-f58b55a0517ed0eb` and its listing beside it; **the placed `.hero` set
  equals the coordinator's closure for 18 of 18** (`cmp` against `reach/lines48/<p>.closure`),
  `tests/harness` included, whose six roots the walk over every `.hero` of the folder covers (it places
  `slices`, which only `suite_compat.hero` names); **18 of 18 `check main.hero` exit 0**; the 17
  examples' binaries **17 of 17 as `main.expected` says** (13 at the first pass; `interpreter`,
  `logs`, `query` and `spreadsheet` differed by my runner, which kept the `!exit: N` trailer
  `tests/harness/expectation.hero:115` takes off and gave `query` no `build/`; `rejudge.sh` by that
  rule: same, exits 0, 1, 0, 1). **Placed under the variant H** (every card kept): **215 files** and
  18 listings, against W's 540; 89 modules, the brief's 89.
- **The falsifier under route M** (`falsifier.sh ce-m2`, `falsifier-m.txt`; the fetch cases
  `falsifier-fetch.sh`, `falsifier-fetch-m.txt`; each edit in a fresh copy of `mrun/examples/ini` as
  `ce-m2`'s fetch placed it, `strings.hero`, `strings/internal/scan.hero`, 7 cards and the listing
  beside): **no edit compiles another module.** Catalogue deleted, entry deleted, the entry left
  behind with its tree: `check` exit 1 `unknown_entry`. `version` 0.5.1, `sum` one digit, `index`
  `packagez`: exit 1 `package_unread`, *"not placed"*. A reached file edited: exit 1, *"holds
  `strings.hero`, changed since it was placed"*; an unlisted `evil.hero` added: exit 1, *"added
  since it was placed"*; the listing edited: exit 1, *"the listing ... is `sha256-7a602e4e...`"*;
  the listing moved out: exit 1, the tree read whole, *"is `sha256-7a6d4561...`"*. **A reached file
  removed**: exit 1 **`error[module_unplaced]`** *"the module `strings/internal/scan` of the package
  `core` is in the version the catalogue names and not placed"*, told on the package's own `use`
  line. **The program's own `strings.hero` written beside `main.hero` and the placed one removed**:
  exit 1 `module_unplaced`; the program's own file is not compiled in its place. **A listed,
  unreached file put back byte for byte** (`strconv.hero`): exit 0, the program's output unchanged,
  the module set unchanged (nothing reaches it). **The measured edit of the brief** (`use
  <core>/strconv` and `print(strconv.atoi(s: "7")...)` added): `check` exit 1 `module_unplaced` at
  that `use` line; **then `fetch`: exit 0**, *"placed core 0.5.0"*, 11 files, `strconv.hero` and
  `strconv/internal/shown.hero` added; `check` exit 0; the binary prints **`7`** first. **The `use`
  line removed again**: `check` exit 0 before any fetch (placed files nothing reaches are accepted);
  `fetch` exit 0, the part replaced by the smaller one, `strconv` gone. **Panel 246's R2**: a reached
  file edited, then `fetch`: exit 1 `sum_mismatch` *"is placed ... in part, and ... holds
  `strings.hero`, changed since it was placed"*, nothing replaced; `evil.hero` added, then `fetch`:
  exit 1, `evil.hero` still there; a reached file removed, then `fetch`: exit 0, put back. **The
  listing moved out, then `fetch`**: exit 1, and the message lists 21 files *"taken out"*: the tree
  with no listing is read whole, so the author is told an edit they did not make, though following
  its advice (remove the folder, fetch) places the tree again.
- **Emitted C, whole against pruned** (`emitcmp/`): `ce-m2 build main.hero --emit-c` in
  `tree/examples/<p>` (the base's whole tree, no listing) and in `mrun/examples/<p>` (M's part), and
  `heroes-next`'s in the whole tree, all six exit 0: for `ini` and `server`, **`cmp` exit 0 every
  pair, the `hero_program_sources` line included** (84,705 and 500,557 bytes; 13 and 171 `#line`s
  naming the placed folder). Placement moves no emitted byte, no `#line` and no provenance key where
  the reached modules are the same: the folder's key names no subset (`module/placed.hero:118-134`).
- **The compiler under M** (`routeAM/`: route (A)'s tree, its placed folder removed, `ce-m2 fetch -C
  selfhost` exit 0, *"placed core 0.5.0 selfhost/heroes.packages/core@0.5.0-683195a84f2d9e81"*):
  9 files placed (`strings.hero`, `strings/internal/scan.hero` and the 7 cards, the variant H), 517
  lines, 26,087 bytes, 166 code lines, and the listing beside, 30 lines, 2,663 bytes.

## R2's ruler over `core` 0.5.0, every unit, every route

The compiler's tree counted as `selfhost/` (the spec-warden reads what R2 means by it), after the step
against before, the step deleting every live copy found of `strings`' contracts (route (A)'s 15
functions; no copy of `split_lines`, `fields`, `trim_space` or `split_once` was found, § Log), and
taking no other face, since none pays for itself (§ Log, `faces.sh`). Computed from the `units.py`
counts above (lines are `wc -l`'s newlines; code lines the `layout` suite's unit, mirrored):

| route | lines | bytes | code lines | files |
|---|---|---|---|---|
| W, route (A): 30 files placed, catalogue | **+6,612** | +248,333 | +4,165 | +31 |
| M strict (the 2 reached modules), catalogue | **+406** | +9,056 | **+120** | +3 |
| M as built (variant H: + 7 cards), listing, catalogue | +585 | +23,772 | +120 | +11 |
| the same, with M's own code in the compiler (+382, +15,654, +288, +2, before the H repair below) | +967 | +39,426 | +408 | +13 |
| G (for the compiler, M's `.hero` set; `strings/internal/` is reached whole) | +406 | +9,056 | +120 | +3 |
| route (C), the 2 modules at `selfhost/`'s root, no catalogue | +234 | +7,551 | +120 | +2 |
| W placed, **R2 counted by the modules reached** (rM2) | +406 | +9,056 | +120 | +3 |
| route 9, a `path` entry to `../packages/core/0.5.0/files`, `packages/` not counted | +35 | **−5,068** | **−46** | +1 |

**R2 is not met by `core` 0.5.0 under any placement, in any unit.** The nearest is +120 code lines,
and it is the same +120 under M, G, route (C) and a ruler counted by reach: `strings`' two modules
carry 166 code lines and the 15 copies they replace net 46. **Placement does not decide R2 here; the
size of `strings` against the copies does.** Only route 9 shows a smaller tree, in bytes and code
lines alone, and only by counting none of the package's code as the compiler's, which is the reading
the spec-warden objects to as emptying the ruler (its Q8) and which collides with R15's *"never
`path` entries"* and with a `path` entry being unverified (R6): no fingerprint guards it.

**Under the entry's reading** (every face's copies deleted): each other face places more code lines
than every copy it could delete, by the bounds in § Log (`slices` 218 against at most 76 lines of
sorts; `encoding/json` 2,815 against 911 code lines of every JSON-named file; `strconv` 49 against 0
copies found), so that reading only widens the gap.

**Route 2, R2's defect branch**: the 12 live copies read whole (`body.py`): every one compares bytes
(`text[at + i]`, `bytes.bytes_at`, `bytes.locate`) or characters (`ffi_lookup.char_index_of`), and
none slices the text, so none has the abort of defects 296 and 2409 to 2411. **Read, not run**: no
differential run against `core` was made; panel 242's reading that the character copies differ only
on an ill-formed needle is carried, not re-run.

## What a build costs, in instructions retired

`instr.sh` (`/usr/bin/time -l <compiler> check <file>`, the sides in turn each round, round 1 a
warm-up and not counted; medians of 5; load averages 4 to 5 throughout, `uptime` at 01:51 and 01:56):

- **`check selfhost/main.hero`** (`instr-compiler.txt`): the base's `heroes-next` on the base
  **131,376,258,534** (spread 91,158,748); `ce-a2` on route (A)'s whole tree **131,802,175,925**,
  **+425,917,391, +0.324 %**; `ce-m2` on route (A)'s whole tree 131,799,336,164 (+0.322 %); `ce-m2`
  on the same tree re-placed by M (`routeAM/`) 131,774,828,468 (+0.303 %). **M against W for the
  compiler: −24,507,696, −0.019 %, inside either side's spread** (186,586,460 and 197,144,073). The
  step's cost is the tree's, as panel 242 found (+0.193 % then); placement moves it by nothing
  measurable. Every run exit 0.
- **`check main.hero`** (`instr-ini.txt`, `instr-json.txt`): `ini`, the base whole 384,012,235,
  `ce-m2` whole 384,157,395, **`ce-m2` on M's part 321,759,750: −62,397,645, −16.24 %**; `json`,
  1,542,096,816, 1,541,976,857, **1,515,032,135: −26,944,722, −1.75 %**. What the check reads: under
  W the whole tree, 253,311 bytes hashed at every build; under F1 the listing (2,663 bytes) and the
  files present (`ini`'s 9, 26,087 bytes). For a small program the fingerprint is most of a check,
  and F1 over a part is cheaper; performance is no justification here (CLAUDE.md § 13), and nothing
  is slower.

## The prototype's second build: the ffi-pragmatist's condition 1

The first `ce-m2` held F1 as `00-shared.md` words it, *"a file absent being unreached"*, for every
file: **a card removed from `ini`'s part, `check` exit 0** (`fals/card-removed-v1/check.txt`), which
for a package carrying a header is the ffi-pragmatist's measured hazard (a header gone, clang finding
one of its name elsewhere, exit 0). Repaired in `module/subset.hero` (+7 lines: a listed file that
is not `.hero` and is absent is refused), rebuilt by `heroes-next`: **`ce-m2` v2, exit 0, `sources
1593fa56260f5ec3`** (`routeM-build2.log`); the same tree, **`check` exit 1** *"lacks `strings.md`,
taken out since it was placed"*; the 18 programs' `check main.hero` again, **18 of 18 exit 0**. The
prototype now: `git diff --no-index --shortstat tree/selfhost routeM/selfhost` **12 files, +421
−32**; `units.py` **+389 lines, +16,240 bytes, +294 code lines, +2 files**. That is the wording the
ffi-pragmatist gives (its § The fingerprint, for C), built.

## The golden cases, the suites, and two seats' questions, run on v2

- **The net, refused by the guard, and not got around.** `./heroes-next run tests/harness/main.hero
  -- <folder>/routeM/ce-m2 check package-`, from my `tree/`, was refused by the PreToolUse hook:
  *"`.../routeM/ce-m2` says it was compiled from sources 1593fa56260f5ec3, and the tree this run
  judges, /Users/joseph/Temp/heroes/heroes-lang, keys 2ab7ca0a644692a1"*: a copy with no `.git` is
  judged against the repository's trunk, so the guard refuses every compiler of mine, the base's
  `heroes-next` (`5bac791363384945`) included. **So the `fetch` and `packages` suites and the 55
  golden cases are unrun by the net under any route here**; a lane, a worktree with its own `.git`,
  is where the coordinator can run them. **Said plainly**: before that refusal I had started the same
  runs from inside a script (`goldens.sh`), which the hook does not read; I stopped it on seeing what
  it was, its one output file held 0 bytes, and no verdict of it is used.
- **In its place, a differential probe** (`diffcases.sh`, `diffcases.txt`): each of the **44** golden
  case folders of `check` (22), `run` (9), `unsupported` (8), `full` (2) and `surface-fixtures` (3)
  that holds a placed tree, copied out of `tree/`, `check` of its root by the base's `heroes-next` and
  by `ce-m2` v2: **44 of 44 the same output and the same exit** (check 22 exit 1, full 2 exit 1, run
  9 exit 0, unsupported 8 exit 0, surface-fixtures 2 exit 0 and 1 exit 1). And the **11** `fetch`
  cases (`difffetch.sh`, `difffetch.txt`: each line of `command` run with `<work>` the copy, the
  catalogue left and the files left under `heroes.packages/` compared): **11 of 11 the same** between
  the two compilers; 9 of 11 also match the exits their `expected` holds, and the two lock cases of
  defect 2453 do not, for both compilers alike, since the harness's scratch set-up is not
  reproduced. **No golden moves under the route as built**, because a tree with no listing keeps
  today's whole-tree check, and **this is a measurement of the prototype, not the net's verdict**.
- **The spec-warden's prediction (b), scored** (`unreached.sh`, `unreached.txt`): in each of the 17
  programs but the driver, a `use` of a face of `core` it does not reach (`strconv`, or
  `encoding/json` where it reaches `strconv`) and one call added to `main.hero`: **M (`ce-m2` v2 on
  M's part): 17 of 17 `check` exit 1, `error[module_unplaced]`; W (the base's `heroes-next` on the
  whole tree): 17 of 17 exit 0.** The prediction holds as written.
- **The spec's own order, `heroes add` before the `use` line** (`addfirst/`): a `main.hero` with no
  `use`, `ce-m2 add core --index file://../../packages` exit 0, **30 files placed and no listing**
  (no `use` names the entry, so the reach is the whole version); then `use <core>/strings` and a call
  written, `check` exit 0; `fetch` exit 0, *"kept core 0.5.0"*, still 30 files. **Under the route as
  built, the order in which an author ran the verbs decides what is placed**: `add` then `use` gives
  W for good, `use` then `fetch` gives M. A route that pruned a whole tree on `fetch` would give one
  answer, and would move every hand-placed `fetch` case whose program names its entry; the prototype
  keeps whole trees so that no golden moves. Either way, placement under M is a function of the
  program's history, where under W it is a function of its catalogue.

## What 2315 replaces, by count

`gh issue view 2315 --json body` (read 02:05): *"What it replaces: the package manager's parts over C,
by packages copied into the compiler's tree under `selfhost/core/`"*, the form panel 241's R3
withdrew. **The package manager's parts over C** (`grep -n '^extern "' selfhost/fetch/*.hero
selfhost/module/*.hero`): **10 `extern` groups in 7 modules**, over `hero_os.h` and
`hero_compiler.h`: the two SHA-256 doors (`fingerprint`, `index_source`), which panel 242's R4 keeps
in C; the directory scan (`hero_dir_scan`, `_at_shown`, `_release`), the file reads
(`hero_file_read_shown`, `hero_file_key`), the run lock (`hero_lock_take`, `_drop`) and the status
constants. **`core` 0.5.0 holds no face that replaces one of them** (its faces: `strings`, `strconv`,
`slices`, `encoding/json`, `net`, `net/http`; no `os`, `path/filepath` or `crypto`, `git ls-tree -r
--name-only 48f53b343 packages/core | grep -c crypto` 0, carried from the brief and consistent with
the 30-file listing). **Of its Heroes helpers**, 0.5.0 could replace **one function**,
`fetch/catalogue_edit.sorted`, through `slices`; its 52 calls of `cli/strings`' three predicates move
under route (A) with no function of `fetch/` or `module/` deleted (§ Log). **So by count, 0.5.0
replaces 0 of the 10 C groups and 1 helper**, under any placement route, M included.

## The routes nobody listed, from the compiler's side

- **2, W kept and R2's defect branch**: read, not run (§ R2's ruler): no live copy slices, so none
  carries the abort of defects 296 and 2409 to 2411. If one were measured wrong, R2 admits the step
  under W with no placement change and no code beyond route (A)'s.
- **3, M for the compiler alone**: its R2 count is M's, **+406 lines, +120 code lines** (§ R2's
  ruler), so it does not meet R2 either; it needs F1's build half at least (`module/subset.hero` 107
  lines, `packages.hero` +6, `package_told.hero` +8, `fingerprint.hero` +4, by the prototype's diff)
  and makes the compiler the one program placed otherwise, against panel 242's R3 (*"the one mechanism
  every program already uses, so no special case (§1.7)"*). Objected, with the spec-warden.
- **5, a key over the reached set**: the build finds a package's folder from its entry alone
  (`module/packages.hero:156`, `placed.key(e, index: ...)`), before reading any of its modules, so a key
  over the reached set needs that set before the first module is read: discovery would compute the
  closure twice, or list `heroes.packages/` for `<name>@<version>-*` and choose. **Unbuilt; an
  inference from those two lines.** What it moves, measured: **13 `#line`s** of the compiler's seed name
  the placed folder at this base (`a2.c`), so every change of the compiler's `use <core>` set would
  rename them, a seed refresh; F3 moves the key the same way (`placed.hero:128-134` hashes the `sum`).
- **9, the compiler reading `core` from `packages/core/<v>/files/`**: the one route under which a unit
  shrinks (−46 code lines, −5,068 bytes; § R2's ruler), only because it counts no line of the package
  as the compiler's. A `path` entry is unverified (R6), so the compiler would build from a folder no
  fingerprint guards, against R15's *"never `path` entries"* and design.md §1.11's *"checks every
  placed tree's fingerprint, its own included"*. Objected. Its `check` is run below.
- **R2 counted by the modules reached (rM2)**: for `core` 0.5.0 it counts the 2 `strings` modules,
  **+406 lines, +120 code lines**, the same as M, for **no line of compiler code**, where M costs the
  compiler **+389 lines, +294 code lines** (v2). **The instrument**: `#line`s miss a module that
  declares only records (measured, § Log); the provenance key covers it (measured) but prints only a
  hash, its rows built in `cli/produce.hero` and written nowhere. So rM2 needs its instrument written,
  a listing of the reached modules' files the `packages` suite's `reach` check could read; unbuilt.
- **The split** (`core` as entries per face): for the compiler, taking the `strings` entry whole
  places `strings.hero`, `scan.hero` and `strings.md`, **+427 lines, +120 code lines**: R2 unmet.
  In `selfhost/module/` and `selfhost/fetch/` an entry is generic (`packages.reach` by alias,
  `placing.placed_tree` per entry), so the compiler's code would not move by reading; unbuilt. The
  spec-warden reads the `Use` production's `ident` as refusing `encoding/json` and `net/http` as entry
  names (its Q1); I did not run that.
- **G against M, and H**: the shape that parts G from M is a Go package whose `internal/` holds a
  module none of its face's closure reaches; at 0.5.0 every reached face's `internal/` is reached whole
  (the brief's `ls`, consistent with the 18 placed sets equal to the closures). H parts from M by every
  non-`.hero` file: for `core`, the 7 cards (149 lines). H is what the prototype builds, since F1 can
  then refuse an absent non-module file without any C closure (the ffi-pragmatist's condition 1).

## Where I agree with the other seats, and where not

Read after my measurements stood (their reports in `docs/panel/250-reports/`, 02:00 to 02:10).

- **The spec-warden** (object to M, G, H; approve W with rU or rM2; veto on F2). **Agree**, on other
  grounds than its budget: the ceiling's. Its prediction (a), *"the compiler's step taking `core`
  0.5.0's `strings` leaves its tree larger"*: **holds**, +406 lines under M, +234 under route (C),
  +120 code lines either way. Its prediction (b): **holds**, 17 of 17 against 0 of 17 (`unreached.txt`).
  Its question whether the spec's order, `heroes add` before the `use` line, meets the refusal: under
  the route as built it does not, and places the version whole instead (`addfirst/`), so placement
  follows the order the verbs ran. Its Q1 leaves to my run whether the goldens that pin *"places it
  again"* move: under a whole-tree fall-back for a tree with no listing, **none of the 44 case folders
  and 11 `fetch` cases moved in the differential probe**; that is the prototype's choice, and a route
  that prunes on `fetch` would move the `fetch` cases. F2 I did not build and hold no opinion on from
  the compiler; F3's key moving with every `use` change, I measured the 13 `#line`s it would rename.
- **The ffi-pragmatist** (veto on M as the rule for a package's C; approve H for C on two conditions).
  **Agree that if any subset is placed it is H**, and the build agrees with its F1 wording: the
  brief's wording, *"a file absent being unreached"*, built as my v1, accepted a removed non-module file
  at exit 0; its wording, built as v2 (+7 lines), refuses it. The listing stands outside the package's
  root, as it prefers. Its residue (2), a C file naming a `.hero` file, has no instance in `core` and I
  did not build its closure. **Where I part**: its approval of H answers *how* to place a subset; mine
  is that no subset should be placed now, since none buys R2.
- **The historian** (approve, advisory, the reached grain by Go package). **Agree on its readings**:
  G or H over M where they part, F1 closest to JSR's listing of per-file hashes, and R7's check at every
  build over a subset having no precedent read. The prototype adds one fact: that check is buildable
  and cheaper per build than today's for a small program (`ini` −16.24 % instructions), and changes
  nothing measurable for the compiler (−0.019 %). **Where I part**: the precedents show subset
  placement is viable; the sitting's question was whether it is needed, and for R2 at 0.5.0 it buys
  nothing.

## The last runs

- **The compiler's own tests** (`selftest.log`; each in a full-tree copy, its output whole in a file):
  the base's `heroes-next test selfhost/main.hero` in `tree/`, exit 0, **`2164 tests, all passed`**,
  1,287,284,787,752 instructions; `ce-m2` v2 in `routeMfull/` (the base's tree with the prototype's
  `selfhost/`, `diff -rq` against `routeM/selfhost` silent), exit 0, **`2165 tests, all passed`**, the
  one more being `own_module`'s, 1,282,231,396,003 instructions. Route (A)'s own tests: **unrun**.
- **Route 9** (`route9/`: route (A)'s modules, `selfhost/heroes.toml` two lines, `path =
  "../packages/core/0.5.0/files"`, no placed tree): `heroes-next check selfhost/main.hero` **exit 0**,
  and every build would print *"note: the package `core` is read from the folder
  `selfhost/../packages/core/0.5.0/files/`, a `path` entry of the catalogue: unverified, no
  fingerprint holds it"* (`route9-check.txt`). Built no further.
- **Left behind for the coordinator**: nothing running (`pgrep -f 250-compiler-engineer` names only my
  own shell); the folder holds `tree/`, `routeA/`, `routeC/`, `routeM/` (`ce-m2` v2, `ce-m2-v1`),
  `routeAM/`, `routeMfull/`, `route9/`, `mrun/`, `fals/`, `fals2/`, `unreached/`, `addfirst/`,
  `diffcases/`, `difffetch/`, `emitcmp/`, `probe-records/`, `pruned/` and every script and log named
  above. **A paid run** would add one thing: `heroes measure --refresh` of the help lines and messages
  M changes, a few hundred tokens, for the spec-warden's budget if the sitting adopts M; I did not run
  it.
