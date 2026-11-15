# Panel 250, the completeness critic's first pass, over the briefs

*Written as it went, 2026-11-14, from about 23:00. Base `722ae76bf` (the worktree `.claude/worktrees/p250`,
branch `p250`); every command below was run by the critic in that worktree, in the trunk's checkout
read-only, or in `.claude/worktrees/p250-scratch/critic/`, and its output is quoted as it printed. No
verdict on any route.*

## Read

`docs/panel/250-briefs/` whole: `00-shared.md` (450 lines), `compiler-engineer.md` (113),
`ffi-pragmatist.md` (101), `spec-warden.md` (137), `historian.md` (103), `critic-briefs.md` (67),
`critic-reports.md` (45) (`wc -l`); `.claude/skills/panel/SKILL.md` § 2 and § 3c; the author's paragraph
at `b5323a21c` (`git show b5323a21c:docs/roadmap/milestones/M-core-packages.md`, its last section) and
that commit's body.

## Findings, as they were reached

### A. The convening commit's branch

`00-shared.md:9` says `b5323a21c` *"stands on branch `b36-round` alone (`git branch -a --contains
b5323a21c`: `b36-round`)"*. Run at about 22:58: `git branch -a --contains b5323a21c` printed `b36-round`
and `b37-round`; `git branch -r --contains b5323a21c` printed nothing (so *not pushed* holds); `git log
--oneline 722ae76bf..b5323a21c` one line; `git diff --stat 722ae76bf b5323a21c`
`docs/roadmap/milestones/M-core-packages.md | 17 +++`. True when written, stale now: the round branch
opened after the brief carries it.

### B. The project placed only what a program reached, for two days, under panel 234's R2

No brief says it. `git log --diff-filter=A -- 'examples/ini/core/*'`: first added at `43aae88d8`,
2026-11-06, *"M-core-packages step 5: the hand copies in examples/ replaced by core/strings"*; `git
ls-tree -r --name-only 43aae88d8 examples/ini`: `core/strings.hero`, `core/strings/internal/scan.hero`,
`main.expected`, `main.hero`. Removed at `31c34d962`, 2026-11-08, *"M-package-manager step 14c: `core`
becomes one catalogue entry, fetched by fourteen programs from a local static index"* (`git log
--diff-filter=D`). At `31c34d962^`: `ini` 2 files under `core/`, `todo` 4, `server` 0, `json` 0, 42 in
all under `examples/` (`git ls-tree -r --name-only 31c34d962^ examples | grep -c '/core/'`), no card in
any program's copy, the cards in the canonical tree alone. A net check held each such subset to the
canonical tree: `git show 31c34d962^:tests/harness/suite_packages.hero`, line 18, *"packages/copies
each file of a copy is its original's, byte for byte"*, and line 168's rule. That is panel 234's R2 as
written (`234-*.md:287-310`, *"it carries under its own root the packages it reaches, byte-identical to
the canonical tree"*): the practice was by Go package reached, with internals, and no cards.

Whole placement arrived with `31c34d962`'s landing of 235's R15, whose body says *"Each of the fourteen
programs holds the catalogue `heroes add core --index file://../index` wrote and the tree it placed"*:
the verb's behaviour, not a ruling read. Whether any sitting weighed a subset against the whole is a
**question**: `grep -n -i -E 'subset of (the|a) (package|version|tree)|only the (modules|packages|files)
(it|the program|a program)|reache[ds]? (only|alone)|placed whole|whole tree|partial(ly)? plac|prun'` over
`235-*.md`, `235-reports/*.md` and `235-briefs/*.md` printed nothing about placement (its hits are
Go's graph pruning and a raylib path). So the sitting may be asking whether to return to the
pre-235 grain under a fingerprint, rather than to invent one; the seats should know.

### C. The reading of panel 235's R2 (*copied whole*)

`00-shared.md:192-194` reads the title's *"Go's modules copied whole"* as Go's module rules taken as
the base. The record says so in its own words, at a line the brief does not cite: `235-*.md:60`, the
proposal as the seats received it, *"The semantics copy one widely used system whole, Go's modules or
Cargo, in its newest release"*, and R2 itself at 545-546, *"Go 1.17's rule is copied"*. So the reading
is the record's, not the coordinator's; the brief should cite 60. The same proposal says *"The sitting
chooses ... where fetched code lives"* (235:61-62), so placement was R7's choice, whose words are *"one
tree per package keyed by its identity"* (635-638) with no grain stated. Also: 235's R19 ends *"The
second version replaces the transport alone"* (`235-*.md:796`) and lists *"the placement folder"*
among what stays fixed; under M the interface R19 fixes moves, so R19 itself, not only 2315's goldens,
is among the rulings the question meets. The brief lists R19 but only as the source of *unchanged*.

A small one: `00-shared.md:213` writes *"amended by 235's R15 (`:692-699`: ..."*; the quoted text is
at `234-*.md:692-699` (the amendment note in 234's record, checked by `sed -n 690,700p`), while
`235-*.md:692-699` is R10. Write the file.

### D. The carried figures and 00-shared.md's arithmetic (the brief's item 1)

**The citations land.** `sed -n 68p 241-*.md`: *"the compiler reaches 368 of the placed tree's 2,334
lines."*; `sed -n 53,56p 242-*.md`: *"it deletes 16 functions, 186 lines, in 12 modules, no whole file,
moves 654 calls, and adds 2,334 placed lines: the compiler's tree grows by 2,325 lines net"*.

**The 368 holds at 0.4.0**: `wc -l packages/core/0.4.0/files/strings.hero
packages/core/0.4.0/files/strings/internal/scan.hero` 258 + 110 = 368; the same two files at 0.2.0
(`git show 2ca3fd0a8:packages/core/0.2.0/files/<f> | wc -l`) 258 and 110, and `strings.hero`'s SHA-256
begins `cac66ea67a60021e` at both versions.

**The arithmetic is not the one R2's ruler asks, and a measurement already replaces it.** 00-shared.md
(176-184) sets 368 reached lines against 186 deleted and reads +182. Panel 242's own report shows why
that subtraction is wrong, and gives two measured nets the brief does not quote:

- `242-reports/compiler-engineer.md:105-109`: under route (A) *"The whole diff of the compiler's own
  modules: `git diff --shortstat -- selfhost` -> 144 files changed, 950 insertions(+), 964
  deletions(-): 14 lines fewer, against 2,334 placed and 5 of catalogue added."* The 186 deleted
  lines are offset by what the move adds (142 modules gaining a `use <core>/strings` line, 140 blank
  lines the formatter writes after it, its lines 101-106): the importers shrink by 14, not 186. So
  the same step under M, the same deletions placing only `strings`' two files, reads at 0.2.0's base
  +368 placed +5 catalogue −14 = **+359 lines**, before a listing (F1) or a card is counted.
- `242-reports/compiler-engineer.md:287-296`, route (C): `core`'s two `strings` files copied to
  `selfhost/`'s root, the importers' diff *"+811 −964, 153 lines fewer"*, *"the tree gains the copy's
  368: +215 lines net, against route (A)'s +2,325"*. That is placing exactly the reached set, built,
  and measured growing.

**And the unit is unstated.** R2 (`242-*.md:140-152`) and design.md:578 say *"its own tree, placed
files counted, is smaller"*: no unit. Panel 242 counted lines of Heroes: *"13 files, 10 modules,
2,334 lines of Heroes and 55 of `.md`"* (`242-reports/compiler-engineer.md:80-81`), so its 2,325 left
the cards out. The author's figures are another unit: 2,389 for 0.2.0 and 6,140 for 0.4.0 count every
file (`b5323a21c`'s page, *"`find packages/core/<v>/files -type f -exec cat {} + | wc -l`"*), and 2,389 =
2,334 + 55. Under M with F1 the placed files gain a listing (26 lines at 0.4.0, `wc -l <
packages/core/0.4.0/list`) and perhaps a card (21 lines); by files, no route that deletes no whole
module shrinks the tree (242's (A) deleted *"no whole file"*). Whether the ruler is lines, the
`layout` suite's code lines, bytes or files, and whether cards, a listing and the catalogue count, is
a question the sitting does not ask; the compiler-engineer's step 6 counts three ways but no seat is
asked which one R2 means.

**What at this base would settle it**: the cited copies have moved since 242's base `1edabb957`.
`selfhost/cli/strings.hero` is 233 lines here (`wc -l`) against *"218 to 126"* there (242 report, lines
95-96); the one commit between, `867a7078f` (defect 2508, `git log --oneline 1edabb957..722ae76bf --
selfhost/cli/strings.hero`), rewrote `holds` to ask the needle's first byte and added its test, +17 −2.
And `git diff --shortstat 1edabb957 722ae76bf -- selfhost`: *"227 files changed, 18543 insertions(+),
2643 deletions(-)"*, 361 commits. *(Corrected while writing: a first draft of this paragraph named
defects 2409 to 2411 as the move; 242's report already counts their tests at its base, line 92.)* So
the 16 functions, 186 lines and 654 calls are counts of a tree that has moved; the
compiler-engineer's step 6 re-measures them, and the brief should say they are stale, not only carried.

### E. The closure table (item 2)

Re-run: `programs.sh` copied into `critic/reach/` (its one absolute path to `closure.sh` rewritten to
the copy), run on the p250 worktree under bash; `diff` against the coordinator's
`programs-722ae76bf.txt` silent. Read for the three misreadings the brief names: `grep -rn -E
'^[[:space:]]+use ' packages/core/0.4.0/files` printed nothing (no indented `use`, so none inside a
`test` block); no module of 0.4.0 holds a bracketed `use` (all 30 `use` lines of `grep -rn -E
'^[[:space:]]*use ' packages/core/0.4.0/files` are bare); the programs' `use <core>/strings as
core_strings` (tests/harness, 10 shown of `grep ... ' as '`) read as `strings` by `programs.sh`'s
`[a-z_/]+`. **Two readings it would get wrong later**: `[a-z_/]+` and `closure.sh`'s `[a-z_0-9/]*`
for the program side differ, so a face with a digit (`crypto/sha256`, 242's R4 candidate) is read
`crypto/sha` by `programs.sh`; and it reads every `.hero` of the program's folder, not the files a
root reaches, which matters for `tests/harness`, a folder of several roots.

**Asked of the compiler for all seventeen, not two**: my stage 2 (`sources 2ab7ca0a644692a1`), in each
program's folder of my archive, `timeout -s KILL 300 ../../heroes-next build main.hero --emit-c`, the
`#line` paths under `heroes.packages/` taken (`critic/lines/*.txt`), every stderr empty (`wc -c
lines/*.err`: 0 total), compared under bash with `closure.sh`'s set per program: **17 of 17 the same
files** (adventure 2, assembler 4, ini 2, json 8, ledger 2, logs 2, markdown 2, maze 2, packages 20,
query 4, rpn 4, server 12, spreadsheet 4, todo 4, wordbands 2, wrap 2, tests/harness 2). **The 78 and
the 17,753 stand** (9×368 + 5×489 + 3,715 + 2,283 + 5,998 = 17,753); and since a records-only module
reached would show as a closure line with no `#line`, none is reached by these seventeen. The brief
can say *asked of the compiler for every program*, and drop the two-program caveat.

### F. The measured refusals (item 7)

Re-run with my compiler in `critic/partial/examples/` (a copy of my archive's `examples/ini` and
`packages`): whole `check` exit 0, nothing printed; pruned to `strings.hero`, `strings.md`,
`strings/internal/scan.hero`, `check` exit 1 `package_unread`, the same two hashes
(`sha256-ae99678252e9...`, `sha256-1b39ba5f301c...`); `fetch` there exit 1 `sum_mismatch`, *"`encoding/json.hero`
taken out, ... and 13 more"*, 3 files left; a copy of `heroes.toml`, `main.hero`, `main.expected`
alone, `fetch` exit 0, *"placed core 0.4.0 heroes.packages/core@0.4.0-c65a01aeebf10d3c"*, 26 files,
`diff -r` against the whole copy silent. All as 00-shared.md:110-124 says. Two things the brief does
not say and a seat will need: the quoted `package_unread` text stops before its advice, *"— move it out
of `heroes.packages/` to keep the change, or remove it, and `heroes fetch` places it again"*, which is
the sentence a route M makes false; and **no `list` is placed today** (`ls -a` of the placed key folder:
the faces and folders alone), so F1's *"the listing placed beside the files"* adds a file the
fingerprint must not cover: inside the key folder it is *"a regular file under the package's root"*,
which R6 (`235-*.md:621-622`) puts under the sum, and a package may publish a file of that name. Where
the listing goes is a question F1 leaves open.

### G. The negative sentences (item 5)

- *"No file of 0.4.0 is C"*: `find packages/core/0.4.0/files -type f | sed -E 's/.*\.//' | sort | uniq -c`
  printed `20 hero`, `6 md`; the `.c`/`.h` count 0; one `extern`, `net/hero_net.hero:15`, `extern
  "hero_net.h"`. **Holds.** No `heroes.toml` among the 26, so M's *"the entry's own `heroes.toml`"* is
  empty for `core`.
- *"the spec names neither `core` nor the placement folder"*: `grep -c '<core>'` 0, `grep -n -w core`
  nothing, `grep -n heroes.packages` nothing. **Holds.** It is also a ruling, which no brief cites:
  panel 244's R1 (`244-*.md:80-100`), *"Out: `heroes fetch` and `heroes.packages/`, the stand-in and
  outside-reading clauses"*, a sentence entering *"where a reader holding only the specification would
  otherwise write a wrong program or be unable to predict a refusal, measured"* (its lines 22-25). The
  spec-warden's draft **p1 puts `heroes fetch` into § 1**, which 244's R1 ruled out; the brief must
  hand the seat 244's R1 and its test, or p1 is priced against a bar nobody showed it.
- *"`heroes fetch` takes no operand"*: `selfhost/cli/table.hero:195`, `operand: .no_operand`, and
  `fetch_flags` (152-154) is `-C` alone, *"the program's folder, where its heroes.toml is"*. **Holds.**
  Not said: the row's `summary`, *"place every package heroes.toml names under heroes.packages/, each
  checked against its sum"*, and `add`'s (194), *"writes every package of the build with its sum, and
  places each"*, are the help a reader reads (cli-surface.md:49-50, *"One argv table parses the
  arguments and prints the help"*); `grep -rln 'place every package heroes.toml names'` finds it in
  `selfhost/cli/table.hero` and `seed/heroes.c` alone, so no golden pins it, and a route M moves it.
- *"the compiler carries no catalogue at the base"*: `ls selfhost/heroes.toml` *No such file or
  directory*; `find selfhost -name heroes.toml -o -name heroes.packages` nothing; outside `examples/`
  and `tests/golden/`, `git ls-files | grep -E '(^|/)heroes\.toml$'` names `tests/harness/heroes.toml`
  alone. **Holds.**
- *A fetched package's `link "x"` and an archive it carries*: read further than line 298 and still a
  question. `grep -n -E '"-L"|"-L|-L ' selfhost/cli/*.hero` finds `-L` only in pkg-config answers'
  filters and tests (`libraries.hero`, `library_file.hero`, `pkgconfig_missing.hero`, `produce.hero`'s
  test) and in `clang_jobs.hero:222`'s word list, none built from a placed root; `link.hero`'s
  *package* is pkg-config's. So by the source read, `link "x"` reaches the toolchain's and pkg-config's
  folders, not the package's root: **unrun**, and the ffi-pragmatist's shape 7 is the run that settles
  it. The brief's sentence can stay a question with this search written beside it.

### H. Rulings the question meets that the briefs do not list

Enumerated from the records, not from the brief: `grep -c -E 'heroes\.packages|placed tree|placed
folder|the placement|places? the version'` over every `docs/panel/2[3-4][0-9]-*.md` names 235 (24), 246
(11), 241 (9), 242 (7), 244 (4), 240 (3), 238 (2).

- **Panel 246's R2** (`246-*.md:264-276`, `ratified 2026-11-12`): *"An edit inside a placed package
  tree is the author's work and is never discarded in silence"*, repaired by defect 2493
  (`d86722098`, *"a tree at the key is kept where it sums to the entry's sum and never replaced where
  it does not"*). Under M a `fetch` after a `use` changes **writes into a placed tree** (adds the newly
  reached, keeps or removes the newly unreached), which is exactly the act 246's R2 and 2493 govern;
  and under F1 an author's removal of an unreached file is not an edit at all. 00-shared.md names
  `fetch/edited.hero` and 2493 among the files M would change (311) but not the ruling.
- **Panel 244's R1**, above (G): the spec's bar for a package sentence, and `heroes fetch` ruled out.
  Its D1, *"a `.DS_Store` in a placed tree refuses every build"* (`244-*.md:61`), is today's
  *"a file present and unlisted refused"*, which F1 keeps.
- **Panel 238's R10** (`238-*.md:282-290`): *"every placed folder renamed (the key holds the `index`
  line as written ...)"*: the precedent for what re-placing every example costs (84 files under 14
  folders then, 442 under 17 now).

### I. The examples' fetched trees and the goldens (item 9)

- **The lane has moved since 22:30.** `git -C .claude/worktrees/b37-pm log --oneline -1`: `10494dc23`
  *"Batch 37 lane b37-pm: task 2312, a golden for each observable of the package manager's interface
  no case held"*, committed 22:57:54; `git branch -a --contains 10494dc23`: `b37-pm`, `b37-round`
  (merged); `git diff --name-only 722ae76bf 10494dc23` adds **13 `fetch` and 28 `check` case folders**,
  none holding a placed tree (`grep -c '/heroes.packages/'` 0); the lane's `status --short
  tests/golden/fetch` now prints nothing. *"10 untracked"* is stale, and the 28 `check` cases are new to
  the count.
- **The fetch suite does not pin which files are placed.** `tests/harness/suite_fetch.hero:46-49`:
  *"`heroes.packages:` and the folders placed"*; its code at 347-352 lists `list_directories` of
  `heroes.packages` and prints each `last_part`: folder names only. Over the 64 base cases, the
  `heroes.packages:` sections print `(none)` 33 times and one key otherwise. So 00-shared.md:249-250's
  premise, *"a route that changes what is placed ... changes those goldens"*, is **false for the
  `fetch` form**: M under an unchanged key moves no transcript there by what it places, and 2312's
  goldens, held *"unchanged"* by 2315, say nothing about placement content either way. A seat should
  be told that a placement route needs a case of its own listing the files placed, and none exists.
- **Where the goldens do move under F1**: every case holding a tree placed by hand (55 folders: `git
  ls-files tests/golden | grep '/heroes.packages/'`, by form check 22, fetch 11, run 9, unsupported 8,
  surface-fixtures 3, full 2, re-run and matching) holds no listing, so F1's *"every build hashes the
  listing against `sum`"* refuses every one of them unless a tree with no listing keeps W's whole-tree
  check. That choice (a fallback, or every hand-placed case rewritten) is not asked. The three
  `fixedbugs-2493` fetch cases each edit by *changing* a file (`load.hero changed`, `cut.hero
  changed`), which F1 still refuses; `package-tree-edited` (check) edits a tree no source recreates,
  so which edit it holds is not readable from the case.

### J. The base is frozen at 0.4.0; the tree that will land no longer holds 0.4.0

00-shared.md:142-145 reads 0.5.0 as lane b37-core's, *"not in the base; a seat measures 0.4.0"*. Since
22:28: `git log -1 --format='%h %ad %s' --date=format:%H:%M` on each: `915c1a639` 22:36 *"step 5: every
program moves from core 0.4.0 to 0.5.0"*; `98aac9ea5` 22:40 *"examples/query and the compat suite
sort by slices.sort_by"*; `8c22c5cbe` 22:43 *"examples/interpreter reads its numbers by
strconv.atoi"*; `5aa8bf5b8` 22:55 *"step 6: core 0.4.0 removed from the index"*; `069eaf077` 23:02,
merged into `b37-round`. `git show b37-round:packages/core/versions`: one line, `0.5.0
sha256-38e61943...`. Catalogues on `b37-round`: **18** (`examples/interpreter/heroes.toml` new, `diff`
of the two `ls-tree` listings). `programs.sh` run read-only on the `b37-round` worktree (its `git
status --short` empty, head `069eaf077`; `critic/reach/programs-b37-round.txt`): **540 placed files,
118,332 lines; 89 reached modules, 19,155 lines**; `query` now 7 modules (916 lines, `slices` with its
two internals), `tests/harness` 5 (795, `slices`), `interpreter` 2 (121), the driver 23 (6,425). A
textual closure there, not asked of the compiler.

So every seat measures R2 over a version the trunk will not hold when the ruling lands, over 17
programs where the trunk has 18. Under M the compiler's placed set does not move between versions
while it reaches `strings` alone (368 lines at 0.2.0, 0.4.0 and, by `programs-b37-round.txt`,
0.5.0), which is the property the author's page asks for; under W it does (5,998 `.hero` lines at
0.4.0, 0.5.0's 30 files and 6,574 lines). The brief should say the base is frozen on purpose, that the
round holds 0.5.0 alone, and ask the compiler-engineer to repeat step 6's count over 0.5.0 (an
archive of `b37-round` or of `069eaf077`), or say why 0.4.0 stands in for it.

### K. *No blind seat* (item 3)

00-shared.md:18-25 says the question changes no line a program writes and that two routes (F2, the
split) break that. By the briefs' own material, more do:

- **M, G and H refuse a program that builds today.** A `use <core>/net` written in a program whose
  catalogue already holds `core` builds at the base with no fetch. Run here: a copy of `ini` with `use <core>/strconv` and one
  `print(strconv.atoi(s: "7").default(0).to_str())` added, `heroes fmt`, then with the tree whole `check`
  exit 0, `build` exit 0, its binary under `timeout -s KILL 60` exit 0 printing `7` first; the same
  edit over the pruned copy `check` exit 1 `package_unread` at `use <core>/strconv`
  (`critic/partial/examples/ini-whole-added`, `ini-pruned-added`); under M it is refused until `heroes
  fetch` (00-shared.md:272-274). That is a widened refusal and a message a reader acts on, in the edit
  loop a model runs most; and `package_unread`'s advice (*"a file of it was changed, added or taken
  out"*, F above) and the verbs' help summaries (G above) move with it.
- **F3 changes what the catalogue holds**: its `sum` is no longer the index's and moves with the `use`
  lines (00-shared.md:285-287); a reader of spec § 1's example, which shows the index's kind of
  `sum`, writes another value.
- **The spec-warden's own draft p1** (M or H, F1) is a sentence of spec § 1, the one thing the blind
  seat reads.
- **Trees not committed**: a fresh checkout no longer builds before a fetch (R8), a workflow change
  a reader meets first.

Only W, F4, the ruler moved and the author's *b* and *c* leave a reader untouched. **And the escape
arrives late**: the seats run in parallel, so *"says so first"* reaches the coordinator when the seats
end; the blind seat is a `claude -p` run outside the repository (`SKILL.md` § 2, 145-166), a paid run
needing the author's budget; it is about 23:15 on the night the sitting was convened. The brief does
not say what the synthesis does if no budget comes: CLAUDE.md § 4 says the panel never blocks, so
the honest form is either to ask the author for the blind seat's budget **before the seats start**
(M with p1 already meets the trigger by the briefs' own drafts), or to write that a reader-facing
route is adopted `provisional` with the blind reading owed and named. The trigger also binds only
*"a seat that finds a route it would adopt"*; a synthesis that adopts a reader-facing route no seat
recommended escapes it as written.

### L. The routes (item 8): routes, restatements, and what is missing

**At this base, three of the four units are one.** The brief's own claim (compiler-engineer.md step 7)
that every reached face's `internal/` is reached whole holds: `closure.sh` per program (E above) gives
`encoding/json` 8 modules = the face and its 7 internals, `net/http` with its 4, `net`, `strconv` and
`strings` with their 1 each, and on `b37-round` `slices` with both of its 2 (`query`'s 7). So **G is
M** for every program here, at 0.4.0 and 0.5.0; and since `core` holds no C, **H is M with the cards**.
They part only in a later version or another package; the seats can be told so and asked for the
shape that parts them, rather than to build three. W, F1 to F4 and the ruler moved are routes. The
split is a unit route as well (the entry becomes the Go package, placed whole). *Trees not committed*
moves where, not what, and meets the question only through R2's *"the compiler's tree"* (N below).
The author's *b* and *c* are deferrals.

**Routes nobody listed** that a reader of 2315, 241, 242 and the briefs' question asks:

1. **Panel 234's grain, restored**: per-program copies of the reached Go packages, as the examples
   held from `43aae88d8` to `31c34d962` and the `copies` check held them (B above). It is the
   project's own precedent for M, with a check at the net rather than at every build.
2. **W kept, R2 met by its other branch**: R2 admits a package where *"a copy it deletes is measured
   crashing or answering wrong on an input a caller reaches"* (242:142-144). Panel 242 found none
   among 13 copies at `1edabb957` (*"0 `.slice(` calls among them"*, 242:56-58); `selfhost/` has moved
   227 files since (D above). A differential run of each live copy against `core`'s function, on the
   inputs its callers reach, is a route no brief asks for.
3. **M for the compiler alone**, the one program R2 binds, every other program keeping W: no
   example, golden or site page moves. It reads against 242's R3, *"the one mechanism every program
   already uses, so no special case (§1.7)"* (242:153-158).
4. **A listing kept in the catalogue** (the placed subset, or per-file hashes, named there), where F1
   places it in the tree: R5 refuses arrays (235:589-592), so a new field shape; reader-facing.
5. **A placement keyed by the reached set**: today's key hashes `index`, name, version and `sum`
   (`selfhost/module/placed.hero:128-134`), so it names no subset; a key over the subset makes a stale
   placement visible by name, and moves every `#line` path of the compiler (13 at 0.2.0, carried)
   whenever its `use <core>` set moves, so a seed refresh per such change.
6. **A package that declares its grain to the index** (per-face listings, the version's `sum` a hash
   over them, or Zig's `paths` declared by the publisher: the survey's row at
   `235-reports/historian-survey.md:395`, *"Only files included in the package are used to compute a
   package's `hash`"*): one `sum` per entry kept, the fingerprint's algorithm changed, which R19 fixes
   (235:789-791).
7. **A sum over a subset the catalogue declares** (a field naming the faces used, the sum computed
   over them by the verb or the index): F3 with the subset written, not inferred from `use` lines, so
   the falsifier can be asked of the field; reader-facing.
8. **M as a flag of `fetch` and `add`**, W by default: a cli-surface question (one verb, a flag that
   changes how one question is answered, cli-surface.md:40-43's reading for `--package`).
9. **The compiler reading `core` where the repository already holds it**, `packages/core/<v>/files/`,
   so nothing is added under `selfhost/`: it turns on what *"the compiler's tree"* denotes (N below)
   and collides with R15's *"never `path` entries"* (235:747-748) and R6's unverified `path`.

### M. The bounds (item 10)

- **No paid run** is named in any brief; the spec-warden's counts are said to be vendored and a lower
  bound (`spec-warden.md:29, 42, 92`); re-run here with my compiler, `measure spec/heroes-spec.md`
  claude-legacy 8483, cl100k_base 8634, real 11303 (*"claude-opus-5, 2026-11-14"*), headroom 5081;
  `measure CLAUDE.md` 6236, 6347, real 8359; both as written.
- **Programs that may not end** are bounded by time and bytes in 00-shared.md:386-393; the
  compiler-engineer's step 4 (*"each one's `check` and its `run` case"*) reaches `examples/server`,
  whose binary is bounded there; the step should point at that bound.
- **Two collisions a seat will hit.** The recipe (00-shared.md:338-341) builds `heroes` and
  `heroes-next`, and every command in the seats' briefs runs `./heroes-next`, against 00-shared.md:384-385's
  *"every binary you build is named with your prefix"*: say which holds. And 00-shared.md:393's *"every
  output through `head -c 65536`"* reads as covering a suite; the guard refuses a gate piped into
  `head` or `tail` (refused here at about 23:05, *"the net's output piped into `tail` cuts the lines
  that carry the verdict"*). Say: a suite's output to a file, read whole.
- **A copy with no `.git` runs a narrowed suite**: `./heroes-next run tests/harness/main.hero --
  ./heroes-next fetch fixedbugs-2493` in my archive, exit 0, *"fetch (only fixedbugs-2493): 3 passed,
  0 failed"*; the guard admitted it.
- **Windows off**: `ssh -o BatchMode=yes -o ConnectTimeout=15 win 'echo up'` at 23:13:34, *"Operation
  timed out"*, exit 255, as at 22:36; owed to the CI's leg in every brief. The Linux image:
  `docker images heroes-linux-arm64` lists `338a1044ac90` and `75742c2bf90b`, as written.
- **Each seat's copy and compiler its own**: yes, `250-<seat>/`, and the historian builds nothing.
- **Mine**: one file written outside the repository's root by my own command at 23:12, `/tmp/x`
  (708 bytes, a `git diff --name-only` listing of 0.5.0's paths, a redirection I should not have
  typed); read back, then removed (`ls /tmp/x`: *No such file or directory*). Nothing else.

### N. Questions the sitting should ask and does not

1. **R2's unit**: lines, the `layout` suite's code lines, bytes or files; and whether cards, a placed
   listing and the compiler's catalogue count (D above). Panel 242 counted `.hero` lines; the author
   was given every file's lines.
2. **What *"the compiler's tree"* is**: `selfhost/`, or `selfhost/` and `runtime/`, or the repository,
   whose `packages/` already holds every version's files. A placement outside the counted tree, or a
   tree not committed, turns on it.
3. **Which *package* R2's *"every live copy of that package's contract"* means**: the entry (every
   face of `core`, so the compiler must delete its copies of `strconv` and `encoding/json` too, and
   then reaches them) or the Go package it reaches. 00-shared.md separates the words but never applies
   them to R2.
4. **Why whole placement replaced 234's reached grain** at `31c34d962`, and whether any record weighed
   it (B above): the historian and the spec-warden can read 235's briefs, reports and that commit.
5. **Which version the ruling is measured over**: 0.4.0 at the base or 0.5.0 on the round (J).
6. **What `fetch` does to a placed tree when the `use` set moves**, read against panel 246's R2 and
   defect 2493's *"never replaced"* (H above).
7. **Under F3, the folder key moves with the `use` lines** (`placed.hero:128-134` hashes the `sum`):
   a new folder on every `use` change, and for the compiler a seed refresh on every change of its
   `use <core>` set. No brief connects F3 to the key.
8. **Under F1, where the listing stands** so R6's sum does not cover it, and **what a tree with no
   listing means** (the 55 hand-placed goldens, every example until re-placed): a fallback to W's
   check, or every case rewritten (F and I above).
9. **What test would hold which files `fetch` places**: none today, since the `fetch` suite prints
   folder names only (I above); without one, 2315's *"unchanged"* is silent on placement.
10. **Who computes the reached set at `fetch`**: today's `selfhost/fetch/` reads no module's `use`
    lines, by a search (`grep -n -E '^use ' selfhost/fetch/*.hero`: no module of it imports `parse`,
    `lexer` or a discovery module; its `module_text` and `module/reading` serve an address's entry name
    and a file's text, `add_wanted.hero:81`, `git_source.hero:112`), a question rather than a premise; a textual closure and the compiler's agree on all 17
    programs here (E above), but a fetch-time closure is new code in the verbs, and for a git source
    it is computed over files no index lists.
11. **Feature 2315's own notes** still name the form 241's R3 withdrew: *"What it replaces: the
    package manager's parts over C, by packages copied into the compiler's tree under
    `selfhost/core/`"* (`gh issue view 2315 --json body`), and 235's R19 says *"The second version
    replaces the transport alone"* (235:796) where 242's R5 says it *"replaces no transport"*. What
    2315 would replace, and with which package, is not asked; 242's R7 names `crypto/sha256`, which no
    version of `core` holds.

## The list: each fact, the command, what it printed, and the repair

1. **`b5323a21c` *"on branch `b36-round` alone"*** (00-shared.md:9). `git branch -a --contains b5323a21c`
   printed `b36-round`, `b37-round`; `git branch -r --contains b5323a21c` nothing. **Repair**: *"on
   `b36-round` and `b37-round`, on no remote branch"*.
2. **The project placed only what a program reached under panel 234's R2**, unsaid. `git ls-tree -r
   --name-only 43aae88d8 examples/ini`: `core/strings.hero`, `core/strings/internal/scan.hero`;
   `31c34d962^`: 42 files under `examples/*/core/`, `ini` 2, `todo` 4, no card; `git show
   31c34d962^:tests/harness/suite_packages.hero` line 18, *"packages/copies each file of a copy is its
   original's, byte for byte"*; whole placement from `31c34d962` (*"the tree it placed"*); a `grep` of
   235's record, reports and briefs for a weighing of a subset printed nothing. **Repair**: a paragraph
   in § Whole placement today saying so with these commands; route *"234's grain restored"* in § The
   routes; the question *why whole replaced it* to the spec-warden and the historian (N4).
3. **The reading of *"copied whole"*** (00-shared.md:192-194). `sed -n 60p 235-*.md`: *"The semantics
   copy one widely used system whole, Go's modules or Cargo"*; 545-546 *"Go 1.17's rule is copied"*.
   The reading is the record's. **Repair**: cite 235:60 and drop *"read the phrase as the sitting
   wrote it"*; add R19's *"the placement folder"* and *"The second version replaces the transport
   alone"* (235:789-796) to the rulings the question meets, not only as the source of *unchanged*.
4. **`(:692-699: "R2's placement, a copy of core/ ...")`** (00-shared.md:213) reads as 235's file.
   `sed -n 690,700p 234-*.md` holds the quote; `235-*.md:692-699` is R10. **Repair**: write
   `234-*.md:692-699`.
5. **368 against 186, +182** (00-shared.md:179-181). `sed -n 105,109p
   242-reports/compiler-engineer.md`: *"144 files changed, 950 insertions(+), 964 deletions(-): 14
   lines fewer, against 2,334 placed and 5 of catalogue added"*; `sed -n 287,296p`: route (C), the
   reached `strings` files placed at `selfhost/`'s root, *"+215 lines net"*. **Repair**: replace the
   arithmetic with these two carried measurements: the reached set placed was built at 242 and grew
   by 215; under M with (A)'s `use <core>/` lines, +368 +5 −14 = +359 at 0.2.0's base before a listing
   or card. Ask the compiler-engineer to re-measure, not to replace an arithmetic.
6. **R2's unit is unstated.** `sed -n 140-152p 242-*.md` and design.md:575-581: *"smaller"*, no
   unit; `sed -n 80,81p 242-reports/compiler-engineer.md`: *"2,334 lines of Heroes and 55 of `.md`"*;
   the author's 2,389 = 2,334 + 55 (`git show 2ca3fd0a8` files, 13 files 2,389 lines, re-run). **Repair**:
   a question in § Questions and in compiler-engineer step 6 and spec-warden question 8: which unit, and
   whether cards, a placed listing and the catalogue count.
7. **16 functions, 186 lines, 654 calls** carried from `1edabb957`. `wc -l selfhost/cli/strings.hero`
   233 against 242's *"218"*; `git log --oneline 1edabb957..722ae76bf -- selfhost/cli/strings.hero`:
   `867a7078f` (defect 2508); `git diff --shortstat 1edabb957 722ae76bf -- selfhost`: *"227 files
   changed, 18543 insertions(+), 2643 deletions(-)"*. **Repair**: say these counts are of a tree that
   has moved that much, not only *carried*.
8. **The closure table** (00-shared.md:151-172). `programs.sh` re-run: `diff` silent; the compiler
   asked for all 17 programs (`build main.hero --emit-c`, `#line` paths, my stage 2): 17 of 17 the same
   files as `closure.sh`; 78 and 17,753 stand. **Repair**: *"asked of the compiler for every program,
   the same files"*, citing `critic/lines/`; note that `programs.sh`'s `[a-z_/]+` would cut a face
   with a digit and reads every file of a folder, not a root's reach. Compiler-engineer step 1 can then
   be narrowed to a records-only module and to the round's tree (item 18).
9. **The refusals** (00-shared.md:110-124). Re-run: whole `check` 0; pruned `check` 1 `package_unread`,
   same hashes; pruned `fetch` 1 `sum_mismatch`, 3 files left; fresh `fetch` 0, 26 files, `diff -r`
   silent; the placed folder holds no `list` (`ls -a`). **Repair**: quote `package_unread` whole,
   through *"— move it out of `heroes.packages/` to keep the change, or remove it, and `heroes fetch`
   places it again"*; add to F1 *"where the listing stands, since inside the key folder R6's sum
   (235:621-622) covers it"*.
10. **The spec's silence is a ruling.** `grep -c '<core>'` 0, `grep -n -w core` and `grep -n
    heroes.packages` nothing; `sed -n 95,97p 244-*.md`: *"Out: `heroes fetch` and `heroes.packages/`
    ..."*. **Repair**: add panel 244's R1 and its bar (`244-*.md:22-25`, *"where a reader holding only
    the specification would otherwise write a wrong program or be unable to predict a refusal,
    measured"*) to § The rulings and beside the spec-warden's draft p1, which names `heroes fetch`.
11. **The verbs' help summaries are reader-facing and pinned by no golden.**
    `selfhost/cli/table.hero:194-195`; `grep -rln 'place every package heroes.toml names'`:
    `seed/heroes.c`, `selfhost/cli/table.hero`. **Repair**: add them to § What M or H would also change
    and to the reader-facing list (item 19).
12. ***Whether a fetched package's `link "x"` finds an archive it carries***. `grep -n -E
    '"-L"|"-L|-L ' selfhost/cli/*.hero`: `-L` only in pkg-config filters, tests and a word list, none
    from a placed root. **Repair**: write that search beside the question; it stays the
    ffi-pragmatist's shape 7, unrun.
13. **Panel 246's R2 and defect 2493 are not among the rulings.** `sed -n 264,266p 246-*.md`: *"An edit
    inside a placed package tree is the author's work and is never discarded in silence"*; `git log -1
    d86722098`: *"a tree at the key is kept where it sums to the entry's sum and never replaced where it
    does not"*. **Repair**: add to § The rulings; ask the compiler-engineer and the spec-warden what a
    `fetch` under M does to a placed tree when the `use` set moves, read against them.
14. **Panel 238's R10** re-placed every example once (`sed -n 282,290p 238-*.md`, *"84 files under 14
    folders"*). **Repair**: cite it as the precedent for re-placing 442 (or 540) files and publishing
    the site.
15. ***"10 untracked"*** in b37-pm (00-shared.md:245-246). `git -C .claude/worktrees/b37-pm log
    --oneline -1`: `10494dc23`, 22:57:54; `git branch -a --contains 10494dc23`: `b37-pm`, `b37-round`;
    13 `fetch` and 28 `check` case folders added, none with a placed tree; the lane's `status --short
    tests/golden/fetch` empty. **Repair**: update § The interface: committed and merged, 13 + 28, the
    round's `fetch` form 77 cases (64 + 13; the commit's *"Floor 58 -> 77"*).
16. ***"a route that changes what is placed ... changes those goldens"*** (00-shared.md:249-250).
    `tests/harness/suite_fetch.hero:46-49` and 347-352: the transcript lists the placed folders' names
    alone; 33 of 64 base cases print `(none)`. **Repair**: say the `fetch` form does not pin placement
    content, so M under an unchanged key moves none of it; ask whoever builds M for a case that lists
    the files placed; and the spec-warden's question 6 (*"read, not run"*) yields to the
    compiler-engineer's run where they differ.
17. **F1 and the 55 hand-placed trees.** `git ls-files tests/golden | grep '/heroes.packages/'`: 55
    folders (check 22, fetch 11, run 9, unsupported 8, surface-fixtures 3, full 2), none with a listing.
    **Repair**: add to F1 and to compiler-engineer step 4 the choice the route owes: a tree with no
    listing falls back to W's whole-tree check, or every such case is rewritten.
18. **0.4.0 is gone from the round.** `git show b37-round:packages/core/versions`: `0.5.0` alone;
    `5aa8bf5b8` 22:55 removed 0.4.0, `069eaf077` 23:02 merged; 18 catalogues (`interpreter` new);
    `programs.sh` on the `b37-round` worktree: 540 placed files, 118,332 lines, 89 reached modules,
    19,155 lines. **Repair**: a row for the round in § The numbers; compiler-engineer step 6 over 0.5.0
    as well, or the reason 0.4.0 stands in.
19. ***No blind seat***, *"two of them break it"* (00-shared.md:18-25). Run: a copy of `ini` gaining `use
    <core>/strconv` builds and runs with the tree whole (`check` 0, `build` 0, prints `7`) and is
    refused pruned (`check` 1 `package_unread`): M, G and H refuse a program that builds today. F3
    moves the catalogue's `sum`; p1 is a spec sentence; trees not committed change the first build.
    **Repair**: name these as reader-facing; decide the blind seat's budget with the author **before**
    the seats start, or write that a reader-facing route is adopted `provisional` with the blind
    reading owed; make the trigger bind the synthesis as well as a seat.
20. **G and H at this base.** The per-program closures (`critic/lines/*.closure`) reach every face's
    `internal/` whole, at 0.4.0 and on the round (`query`'s 7 with `slices`' 2 internals); `core` holds no
    C. **Repair**: say G is M and H is M with the cards for every program here, and ask the seats for
    the shape that parts them rather than three builds.
21. **Routes nobody listed** (L above): 234's grain restored; W kept and R2 met by its defect branch
    (242:142-144; 242 found none at 56-58, before 227 files moved); M for the compiler alone (against
    242's R3, 153-158); a listing kept in the catalogue; a key over the reached set (moving the seed's
    `#line`s); a grain the package declares to the index (Zig's `paths`, survey:395; the fingerprint's
    algorithm, R19); a sum over a subset the catalogue declares; M as a flag of `fetch`/`add`; the
    compiler reading `core` from the repository's `packages/` (against R15's *"never `path`
    entries"*, 235:747). **Repair**: state each in § The routes, none adopted, with the record it reads
    against.
22. **The bounds.** The recipe builds `heroes` and `heroes-next` (00-shared.md:338-341) against
    *"every binary you build is named with your prefix"* (384-385); *"every output through `head -c
    65536`"* (393) against the guard, which refused my suite piped into `tail` (*"the net's output piped
    into `tail` cuts the lines that carry the verdict"*); compiler-engineer step 4's *"its `run` case"*
    reaches `examples/server`. **Repair**: one name rule; *"a suite's output to a file, read whole"*;
    step 4 points at 00-shared.md's server bound. Held as written: no paid run anywhere; Windows off
    (`ssh ... win 'echo up'` at 23:13:34, exit 255); the Linux image `338a1044ac90` listed; the
    spec-warden's counts vendored and re-measured here the same (8483, 8634, real 11303; CLAUDE.md
    6236, 6347, real 8359); a narrowed suite runs in a copy with no `.git` (`fetch (only
    fixedbugs-2493): 3 passed, 0 failed`).
23. **Questions the sitting does not ask** (N above): R2's unit; what *"the compiler's tree"* is; which
    *package* R2's *"that package's contract"* means; why 234's grain was replaced; which version the
    ruling is measured over; what `fetch` does to a placed tree when the `use` set moves (246's R2);
    F3's key moving with the `use` lines (`placed.hero:128-134`) and the seed refresh it implies; where
    F1's listing stands and what a tree with none means; what case would pin which files are placed;
    who computes the reached set at `fetch` (no module of `selfhost/fetch/` imports a parser, by
    `grep -n -E '^use ' selfhost/fetch/*.hero`); and what 2315 replaces, its notes still naming
    `selfhost/core/` (`gh issue view 2315 --json body`). **Repair**: add each to § Questions no one has
    run, with the seat that would run it.
24. **For the second pass** (`critic-reports.md`): its item 2's *"which golden of
    `tests/golden/fetch/` ... moves"* should also ask whether a case pins the files placed (item 16).
    And one file of mine went outside the repository's root, `/tmp/x` at 23:12, read back and removed
    (M above).
