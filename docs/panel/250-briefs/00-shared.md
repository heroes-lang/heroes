# Panel 250, the brief every seat reads first

Convened 2026-11-14 by the author's answer **`1a`**, given at about 22:20 to a batch put in plain text
with the coordinator's recommendations, and recorded twice: as a comment on feature 2315 (`gh issue
view 2315 --repo heroes-lang/heroes --comments`, its last comment, *"**The author's answer of
2026-11-14, at about 22:20, `1a`** ... a sitting is convened on placing only the modules a program
uses"*), and on the milestone's page in commit **`b5323a21c`**, which stands on branches `b36-round`
and `b37-round` and on no remote branch (`git branch -a --contains b5323a21c`; `git branch -r
--contains b5323a21c` printed nothing). **The base below holds it** (`git merge-base --is-ancestor
b5323a21c 48f53b343` exit 0): read it in your copy, `docs/roadmap/milestones/M-core-packages.md:497`,
*"The author's answers of 2026-11-14, evening: the ruler moved away, so a sitting"*.

**The seats**: the compiler-engineer, the ffi-pragmatist, the spec-warden, the historian (advisory),
and the completeness critic, who read these briefs before any seat started
(`docs/panel/250-reports/critic-briefs.md`, 24 numbered items, every one answered below; the briefs as
they stood before it are kept beside each as `*.before-critic.md`) and reads the reports after. No paid
run is named in this sitting, by anyone: no `claude -p`, no API call, no `heroes measure --refresh`
(the spec-warden's counts are the vendored tables, a lower bound, said in those words), no cloud run. A
seat that finds one worth running names it in its report with its size and cost, for the coordinator.

**A route can change what a reader of a program meets, measured.** In a copy of `examples/ini` at the
base, `use <core>/strconv` added above its `use <core>/strings` and `print(strconv.atoi(s:
"7").default(0).to_str())` as the first line of `main`, `heroes-next fmt main.hero --in-place` exit 0:
with the placed tree whole, `check` exit 0, `build` exit 0, the binary under `timeout -s KILL 60` exit 0
printing `7` first; the same edit over a tree holding only `strings.hero`, `strings.md` and
`strings/internal/scan.hero`, `check` exit 1, `error[package_unread]` at `main.hero:28:5`, the new `use
<core>/strconv` line (`.claude/worktrees/p250-scratch/briefs/partial48/examples/added-whole-check.txt`,
`added-pruned-check.txt`; the critic measured the same at the earlier base, its § K). So under a route
that places only what a program reaches, a `use` written after the fetch is refused until `heroes fetch`
runs again (R8, *"`build`, `check` and `run` never fetch"*), where today it builds. **A seat that finds
a route it would adopt changes what a program, its catalogue, a `use` line, a message a reader acts on,
or a sentence of the spec writes, says so first, in those words.**

**Where this brief is wrong, say so first.** Every number, path and count below carries the command
that produced it, run by the coordinator on 2026-11-14 between about 22:28 and 22:55 at the first base
and between about 23:20 and 23:45 at this one, every output under
`.claude/worktrees/p250-scratch/briefs/`; a figure copied from an issue, a report or an earlier sitting
says *carried* beside it, and is yours to re-run before you lean on it. Where a figure is the critic's
and not re-run by the coordinator, it says so.

## Words used here

`package` is two things in this repository, and this sitting needs both apart:

- an **entry**: what a catalogue names and the index versions, `core`, one `version` and one `sum`
  (panel 235's R15, *"`core`: one catalogue entry"*);
- a **module**: one `.hero` file; a **face** is the module at a Go path that a program names,
  `strings.hero`, `net/http.hero` (`examples/packages/README.md` § 1, rule 2), and a **Go package** is a
  face with its private modules under `<path>/internal/` (rule 3);
- **placed**: what `heroes add` and `heroes fetch` write under a program's `heroes.packages/<key>/`;
  **reached**: the modules a program's `use <name>/…` lines name, closed under the entry's own `use`
  lines, which for every program here is the set the compiler compiles (§ The numbers, asked of the
  compiler for each).

## The mandate, and the lane

- **CLAUDE.md § 4** (*"Ask once per milestone, then convene without asking again"*): the milestone's
  gate was asked at panel 242, whose status reads *"convened without asking again (the author's panel
  gate for M-core-packages was asked at 07:35 on 2026-11-10)"* (`docs/panel/242-*.md:3-4`); and this
  sitting was convened by the author's own answer above.
- **Why the full panel and not the soundness lane** (`.claude/skills/panel/SKILL.md:18-34`): the
  question touches the tool surface (`heroes add`, `heroes fetch`, their help, `.claude/rules/cli-surface.md`'s
  falsifier), the architecture of what the build reads, a sentence of spec § 1 (*"names with its
  source, exact version and `sum`"*), and design.md §1.11's ruler (*"placed files counted"*), each a
  trigger of CLAUDE.md § 4.

## The base, and why it moved

**`48f53b3437c6cd46aba5923f4f4780ac7b347283`**, its tree **`d5453ae5f3d91854d12190a4ca02689568b67804`**
(`git log -1 --format='%H %T %ci %s' 48f53b343`), *"Batch 37 round: merge lane b37-check, feature 2299
and defect 2532"*, 2026-11-14 23:16:12 +0100, the head of branch `b37-round` (`git rev-parse
b37-round`), on no remote branch (`git branch -r --contains 48f53b343` printed nothing). **The sitting
was first written over `722ae76bf` and rebased here on the coordinator's word**, because batch 37's
round published `core` 0.5.0 and removed 0.4.0 (`git show 48f53b343:packages/core/versions`: one line,
`0.5.0 sha256-38e6194310472ac1e5779c6bf0fd30c0b725db0db8bcf149cb44f97cbde04204`), moved to it the 16
programs of `examples/` and the net's own that held 0.4.0, gave `examples/interpreter` a catalogue (`git
ls-tree --name-only <c> examples/interpreter/` at each base), and committed lane b37-pm's goldens for task
2312: a sitting over 0.4.0 would measure a version the trunk will not hold. `git log --oneline
722ae76bf..48f53b343`: 17 commits; `git diff --stat 722ae76bf 48f53b343`: 835 files changed. Of the documents these briefs cite, the round
left unchanged `spec/heroes-spec.md`, `CLAUDE.md`, `docs/design.md`, `.claude/rules/cli-surface.md`,
`.claude/skills/`, `site/src/lib/examples-model.ts` and `.github/workflows/deploy-site.yml` (`git diff
--stat 722ae76bf 48f53b343 --` over them and `tests/harness/suite_spec.hero` printed the last alone, a
test's placed path moved from `core@0.4.0-…` to `core@0.5.0-…` at its line 4215); in `selfhost/` it
moved 11 files, none of `selfhost/fetch/` and of `selfhost/module/` only `binding_taken.hero` (feature
2299), which no brief cites (`git diff --name-only 722ae76bf 48f53b343 -- selfhost`); it re-cut the
site's packages chapter in both editions and moved `examples/packages/README.md` after its line 218.
The worktree `.claude/worktrees/p250` still stands at `722ae76bf` (`git worktree list`); your copy is an
archive of this commit, made from that worktree, whose objects are the repository's.

Every path below is from the repository's root, `/Users/joseph/Temp/heroes/heroes-lang`. Every seat that
builds copies the commit and builds its own compiler in two stages:

```
mkdir -p .claude/worktrees/p250-scratch/250-<seat>/tree .claude/worktrees/p250-scratch/250-<seat>/tmp
git -C .claude/worktrees/p250 archive 48f53b3437c6cd46aba5923f4f4780ac7b347283 | tar -x -C .claude/worktrees/p250-scratch/250-<seat>/tree
cd .claude/worktrees/p250-scratch/250-<seat>/tree
export TMPDIR=$PWD/../tmp
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes       # the seed
./heroes build selfhost/main.hero -o heroes-next                 # stage 2
./heroes-next --version                                          # second line: sources 5bac791363384945
```

Measured by the coordinator at 23:22 to 23:27 in `.claude/worktrees/p250-scratch/briefs/base48/tree`,
made exactly that way: the archive holds no `build/`, the seed and stage 2 both exit 0,
`heroes-next --version` printed `heroes 0.6.0` and **`sources 5bac791363384945`**, `heroes-next doctor`
printed `ok   clang        Apple clang version 21.0.0 (clang-2100.4.1.1)` and `ok   runtime      runtime`
(`briefs/base48/seed.log`, `stage2.log`). The durations (seed `real 11.29`, stage 2 `real 126.69` against
`user 95.76`, `sys 21.50`) were taken at one-minute load averages of 308 and 48 (`uptime` at 23:23: *307.95 143.68
74.83*; at 23:25: *48.33 98.92 65.82*) and are not measurements. **A seat whose stage 2 prints another `sources` line is not on the base
and says so first.** No duration a seat writes is a measurement while other sessions run (CL-025):
count instructions (`/usr/bin/time -l`, *instructions retired*), or say why a clock was still. **The
copy has no `.git`**: the `records` suite cannot run there (`.claude/rules/platforms.md` § Measuring in
a copy); a narrowed suite does (the critic: `./heroes-next run tests/harness/main.hero -- ./heroes-next
fetch fixedbugs-2493` in its copy, *"fetch (only fixedbugs-2493): 3 passed, 0 failed"*, carried); and a
`file://` index is admitted *"only for a path inside the program's own repository root"* (panel 235's
R9, C4): the coordinator's `fetch` runs below, inside `.claude/worktrees/p250-scratch/` under the
repository's root, read the index; a seat whose fetch is refused for its address says what refused it.

## What is being asked

**Today a package is placed whole.** `heroes fetch` places every file of a version, held to the
catalogue's `sum`, which is taken over the version's whole tree; and panel 242's R2 (ratified
2026-11-10, `docs/panel/242-*.md:140-152`) lets the compiler take a package *"only where its own tree
shrinks or a measured defect closes"*, the tree counted *"placed files counted"*. The milestone's page
reads the consequence (`M-core-packages.md:497-512` in the base, its lines 501 and 505): *"so every version moves the ruler
further"*, and the acceptance condition of feature 2315 (*"The package manager rewritten over the core
packages, every golden of its interface unchanged"*, `gh issue view 2315 --json title`, OPEN,
M-core-packages) *"could not be met as the rules stand"*. **That is the page's reading, not a
measurement at this base**: § R2's ruler, measured below, says what panel 242 itself measured, and
whether any route meets R2 over `core` 0.5.0 is the compiler-engineer's to count.

**The sitting asks whether, and how, a program places only the modules it reaches** (with the C files,
headers and archives those modules' groups name, and the entry's own catalogue), and what that changes
in: the fingerprint (`sum`: what is verified, and when); `heroes fetch` and `heroes add`, and the
interface task 2312 fixed by goldens tonight; the build's reading of a placed tree; the catalogue
(spec § 1); the falsifier of `.claude/rules/cli-surface.md` § Four input classes; panel 235's R2, R6,
R7, R15 and R19; panel 234's R2 and R13; panel 238's R10; panels 241's and 242's rulings; panel 244's
R1; panel 246's R2 and defect 2493; and decision 2528 (the author's *2b*). And the alternatives a
reader would ask about, each stated below and **none adopted by these briefs**.

## Whole placement today, read in the code and run

**What `heroes fetch` places** (`selfhost/fetch/placing.hero:32-46`, `placed_tree`): a tree under
`heroes.packages/` kept where `fingerprint.of(at)` equals the entry's `sum`, otherwise fetched whole
from its source, `git_source.fetched` or `index_source.fetched`. The index's layout
(`selfhost/fetch/index_source.hero:1-23`): `<index>/<name>/versions`, `<index>/<name>/<version>/list`
(*"the very text `sum` is the SHA-256 of"*), and `<index>/<name>/<version>/files/<path>`, *"each file
at its own address, as JSR serves them, so the first version needs `curl` alone"*; *"Checked twice
before it is placed: each file against its line of the listing as it lands ... and the staged tree's
fingerprint, read from the disk, against the entry's sum"*. **No listing is placed**: `ls -a` of
`examples/ini/heroes.packages/core@0.5.0-f58b55a0517ed0eb/` in the base prints the faces, their cards
and folders, and no `list`.

**What the sum is** (`selfhost/module/fingerprint.hero:1-17`): *"the SHA-256 of a listing of every
regular file under the package's root, one line per file, its SHA-256 in hexadecimal, two spaces, its
name ... sorted by the names' bytes"*, Go's `h1` in hexadecimal; every file hashed in C
(`hero_file_sha256`), *"Checked at every build that reads the tree"*. The index publishes that listing
as `list` (`examples/packages/README.md` § 10, step 3, *"Every file is listed, the cards included"*); at
0.5.0 it is 30 lines (`git show 48f53b343:packages/core/0.5.0/list | wc -l`).

**What the build reads** (`selfhost/module/packages.hero`): `rooted` (142-167) puts a fetched entry's
modules at `heroes.packages/<placed.key(e, index)>/`; `placed_said` (215-234) computes
`fingerprint.of(package.opened)` over the whole placed folder and compares it with `e.sum`, a mismatch
told by `changed_said` (249-265), which tries `fingerprint.added` (`fingerprint.hero:95`: the files
added since placing, found by subtracting listing lines until the hash meets the sum). The folder's key
(`selfhost/module/placed.hero`, `key` at 118, `keyed` at 128): `name@version-` and the first sixteen
hexadecimal digits of the SHA-256 of the source, name, version and sum, so it names no subset.

**Run at the base by the coordinator** (`.claude/worktrees/p250-scratch/briefs/partial48/`, a `git
archive` of `examples/ini` and `packages` at `48f53b343`, each output below in its `examples/`, the
base's stage 2 `heroes-next`, every run under `timeout -s KILL 300`):

- `examples/ini` with its placed tree whole: `heroes-next check main.hero`, **exit 0**, nothing printed
  (`ini-whole-check.txt`).
- The same with only the three files it reaches or documents kept, `strings.hero`, `strings.md`,
  `strings/internal/scan.hero` (the other 27 removed, `find ... | xargs rm`): `check` **exit 1**, at
  `main.hero:28:5`, whole: *"error[package_unread]: `<core>` cannot be read: the tree placed at
  `heroes.packages/core@0.5.0-f58b55a0517ed0eb/` is
  `sha256-ae99678252e9ee279850e1fc8d85beb25d1391dd330253e277de1f335a9648a5`, and the catalogue's `sum`
  for it is `sha256-38e6194310472ac1e5779c6bf0fd30c0b725db0db8bcf149cb44f97cbde04204`: a file of it was
  changed, added or taken out since it was placed — move it out of `heroes.packages/` to keep the
  change, or remove it, and `heroes fetch` places it again"* (`ini-pruned-check.txt`). The pruned tree's
  hash is the one the same three files gave at 0.4.0 on the first base: they did not change.
- `heroes-next fetch` in that pruned copy: **exit 1**, *"heroes.toml:5: error[sum_mismatch]: `core`
  0.5.0 ... that tree was changed since it was placed: `encoding/json.hero` taken out, ... `net.hero`
  taken out and 17 more; the change is yours, so nothing was placed over it: move the folder out of
  `heroes.packages/` to keep the change, or remove it to have the tree placed again"*, the three files
  left (`ini-pruned-fetch.txt`). So today a pruned tree is an edit by the author, and neither verb nor
  build repairs it; **the advice both messages give is a sentence a route that places a subset makes
  false.**
- `heroes-next fetch` in a copy holding only `heroes.toml`, `main.hero`, `main.expected`: **exit 0**,
  *"placed core 0.5.0 heroes.packages/core@0.5.0-f58b55a0517ed0eb"*, 30 files placed (`find
  heroes.packages -type f | wc -l`), `diff -r` against the archive's tree silent (`ini-fresh-fetch.txt`).
- The edit of § The seats above (a `use <core>/strconv` added): builds whole, refused pruned.

## Before whole placement: the project placed what a program reached, for two days

Under panel 234's R2 (`234-*.md:287-310`, *"it carries under its own root the packages it reaches,
byte-identical to the canonical tree"*) the examples held copies of the Go packages they reached. `git
log -1 --format='%h %ad %s' --date=short 43aae88d8`: *"M-core-packages step 5: the hand copies in
examples/ replaced by core/strings, and `contains` admitted"*, 2026-11-06; `git ls-tree -r --name-only
43aae88d8 examples/ini`: `core/strings.hero`, `core/strings/internal/scan.hero`, `main.expected`,
`main.hero`. At `31c34d962^`, `git ls-tree -r --name-only 31c34d962^ examples | grep -c '/core/'`: 42
files, of which 6 the canonical tree in `examples/packages/core/` (its two cards, `strings.md` and
`strconv.md`, among them) and 36 the copies of 13 programs, none a card (`ini` 2, `todo` 4, and so on: `... | grep '/core/'
| sed -E 's|^(examples/[^/]+)/.*|\1|' | sort | uniq -c`); and the net held each copy to the canonical tree (`git show
31c34d962^:tests/harness/suite_packages.hero`, line 18: *"packages/copies each file of a copy is its
original's, byte for byte"*). **Whole placement arrived with `31c34d962`**, 2026-11-08, *"M-package-manager
step 14c: `core` becomes one catalogue entry, fetched by fourteen programs from a local static index"*,
the landing of panel 235's R15, whose body says each program *"holds the catalogue `heroes add core
--index file://../index` wrote and the tree it placed"* (its body's line 9): the verb's behaviour. **Whether
any sitting weighed a subset against the whole is a question**: the critic's `grep -n -i -E 'subset of
(the|a) (package|version|tree)|only the (modules|packages|files) (it|the program|a program)|reache[ds]?
(only|alone)|placed whole|whole tree|partial(ly)? plac|prun'` over panel 235's record, reports and briefs
printed nothing about placement (carried; its § B). So the sitting may be asking whether to return to
the pre-235 grain under a fingerprint checked at every build, rather than to invent a grain.

## The numbers that make the question

Each run by the coordinator at `48f53b343` unless it names the first base; `wc -l` counts lines,
newlines included.

- **`core` at the base**: 0.5.0 alone; `git ls-tree -r --name-only 48f53b343 packages/core/0.5.0/files
  | wc -l`: **30** files; their lines (`... | while read f; do git show 48f53b343:"$f"; done | wc -l`):
  **6,574**; of them **23** `.hero` files, **6,425** lines, and 7 cards (`.md`); `.c` or `.h`: 0 (`grep
  -c -E '\.(c|h)$'` of the listing). Its one `extern` group is `net/hero_net.hero:15`, `extern
  "hero_net.h"`, a runtime header; no `heroes.toml` among the 30, so an M route's *"the entry's own
  catalogue"* is empty for `core`. The four files 0.5.0 added to 0.4.0: `slices.hero`, `slices.md`,
  `slices/internal/merge.hero`, `slices/internal/sample.hero`.
- **The versions before** (removed from the index by decision 2528, read from history: `git ls-tree -r
  --name-only <c> packages/core/<v>/files | while read f; do git show <c>:"$f"; done | wc -l`, `<c>`
  `2ca3fd0a8` = `42f4e2718^` for the first three, `00dfa9151` = `ad32e8a34^` for 0.3.0, the first base for
  0.4.0):

  | version | files | lines |
  |---|---|---|
  | 0.1.0 | 6 | 522 |
  | 0.2.0 | 13 | 2,389 (the figure the author was given) |
  | 0.2.1 | 15 | 4,259 |
  | 0.3.0 | 26 | 5,962 |
  | 0.4.0 | 26 | 6,140 (the author's; 20 `.hero` files, 5,998 lines) |
  | 0.5.0 | 30 | 6,574 |

- **What the programs carry**: `git ls-tree -r --name-only 48f53b343 examples | grep -c
  '/heroes.packages/'`: **510** files in 17 programs, each one tree `core@0.5.0-f58b55a0517ed0eb` of 30
  files, 391 `.hero` and 119 cards; and 30 under `tests/harness/heroes.packages/`: **540** placed files in
  18 catalogues (`examples/*/heroes.toml` and `tests/harness/heroes.toml`, `git ls-tree ... | grep -E
  '^(examples/[^/]+|tests/harness)/heroes.toml$' | wc -l`: 18), 118,332 lines (`programs.sh` below).
  **The author was given *"320 placed files"*, the `.hero` count at the first base** (16 programs × 20,
  `git ls-files examples | grep '/heroes.packages/' | grep -c '\.hero$'` there: 320, of 416 files).
- **What each program reaches**: `.claude/worktrees/p250-scratch/briefs/reach/programs.sh`, run on the
  worktree `.claude/worktrees/b37-round` (head `48f53b343`, `git status --short` empty), which reads each
  program's own `use <core>/…` lines and closes them under the placed tree's bare `use` lines with
  `closure.sh` beside it (`reach/programs-48f53b343.txt`):

  | programs | names | reached modules | their lines | cards |
  |---|---|---|---|---|
  | adventure, ini, ledger, logs, markdown, maze, wordbands, wrap | `strings` | 2 | 368 | 1 (21 lines) |
  | interpreter | `strconv` | 2 | 121 | 1 (12) |
  | assembler, rpn, spreadsheet, todo | `strconv`, `strings` | 4 | 489 | 2 (33) |
  | `tests/harness` | `slices`, `strings` | 5 | 795 | 2 (28) |
  | query | `slices`, `strconv`, `strings` | 7 | 916 | 3 (40) |
  | json | `encoding/json` | 8 | 3,715 | 1 (22) |
  | server | `net`, `net/http`, `strconv`, `strings` | 12 | 2,283 | 5 (120) |
  | packages (the driver) | `encoding/json`, `net/http`, `slices`, `strconv`, `strings` | 23 | 6,425 | 7 (149) |

  In all, **89** reached modules of 414 placed `.hero` files (18 × 23), **19,155** of 118,332 placed lines,
  35 cards. **Asked of the compiler for every program**: `reach/lines48.sh`, in each program's folder of
  the base's archive, `timeout -s KILL 300 ../../heroes-next build main.hero --emit-c`, the placed paths
  of its `#line`s against `closure.sh`'s set (`reach/lines48.txt`, the sets in `reach/lines48/`): **18 of
  18 programs, the same files**, every `build` exit 0 and every stderr empty (`tests/harness` asked of its
  `main.hero`, which reaches `slices` too). The `packages` suite asks the same way
  (`tests/harness/suite_packages.hero:37-40`) and warns *"a module that declares only records emits no
  `#line`"* (its lines 68-71); where the two sets agree, no records-only module is reached (an
  inference: such a module would stand in the closure with no `#line`). **Two
  readings `programs.sh` gets wrong** (the critic, § E): its `[a-z_/]+` cuts a face with a digit
  (`crypto/sha256` read `crypto/sha`), and it reads every `.hero` of a folder, not a root's reach;
  `tests/harness` holds six roots (`git grep -l -E '^function main\(' -- tests/harness
  ':!tests/harness/heroes.packages'`: `generate`, `issues`, `latency`, `main`, `package_cards`, `thesis`),
  and its `slices` comes from `suite_compat.hero:63` alone.
- **At the first base, kept for the arithmetic of a version move**: 17 programs, 442 placed files, 78
  reached modules, 17,753 reached lines (`programs-722ae76bf.txt`); the critic asked the compiler for all
  17 there and found the same files as the closure (its § E). At 0.5.0 the 18 programs place 540 files
  (18 × 30), and only `query`, the driver and `tests/harness` reach `slices`, its four new files.
- **The compiler**: no catalogue (`ls selfhost/heroes.toml`: *No such file or directory*); 849 `.hero`
  files, 221,820 lines (`git ls-files selfhost | grep '\.hero$' | xargs cat | wc -l` in the `b37-round`
  worktree); `selfhost/cli/strings.hero` 233 lines (`wc -l`), 8 functions (`grep -c '^function '`), used
  by 157 files (`grep -rl '^use cli/strings' selfhost | wc -l`); 7 files whose name holds `json`, 1,386
  lines (`git ls-files selfhost | grep -i json | xargs wc -l`).

## R2's ruler, as panel 242 measured it

**Carried from panel 242's compiler-engineer** (`docs/panel/242-reports/compiler-engineer.md`, at base
`1edabb957`, over `core` 0.2.0; each line re-read by the coordinator):

- **Route (A), `core` placed whole as a catalogue entry**: *"it deletes 16 functions, 186 lines, in 12
  modules, no whole file, moves 654 calls, and adds 2,334 placed lines: the compiler's tree grows by
  2,325 lines net"* (`242-*.md:53-56`), and the report's own whole diff: *"`git diff --shortstat --
  selfhost` -> 144 files changed, 950 insertions(+), 964 deletions(-): 14 lines fewer, against 2,334
  placed and 5 of catalogue added"* (`242-reports/compiler-engineer.md:107-109`). **The compiler's
  modules shrank by 14 lines, not 186**: the deletions are offset by the `use <core>/strings` lines 142
  modules gain and the 140 blank lines the formatter writes after them (its lines 101-106).
- **Route (C), placing exactly the reached `strings` files**, `core`'s two `strings` files copied to
  `selfhost/`'s root: *"the importers' diff is +811 −964, 153 lines fewer than the base, and the tree
  gains the copy's 368: +215 lines net, against route (A)'s +2,325"* (its lines 287-296). **So the
  reached set alone was built at panel 242 and measured growing the tree.**
- **R2 states no unit.** R2 (`242-*.md:140-152`) and design.md:575-581 say *"its own tree, placed files
  counted, is smaller"*. Panel 242 counted lines of Heroes (*"2,334 lines of Heroes and 55 of `.md`"*,
  `242-reports/compiler-engineer.md:80-81`); the author was given every file's lines (the page's *"`find
  packages/core/<v>/files -type f -exec cat {} + | wc -l`"*, 2,389 = 2,334 + 55). Whether the ruler is
  lines, the `layout` suite's code lines, bytes or files, and whether cards, a placed listing and the
  compiler's own catalogue count, is unruled; by files, no step that deletes no whole module shrinks the
  tree (route (A) deleted *"no whole file"*).
- **Those counts are of a tree that has moved**: `git diff --shortstat 1edabb957 48f53b343 -- selfhost`:
  *"235 files changed, 18900 insertions(+), 2725 deletions(-)"*, 378 commits (`git log --oneline
  1edabb957..48f53b343 | wc -l`); `cli/strings.hero` 233 lines here against the report's *"218"* (its line
  95), moved by `867a7078f` (defect 2508). The compiler-engineer re-measures them.

## The rulings this question meets, each quoted from the record

- **Panel 235's R2** (`235-*.md:545-566`), in a sitting whose proposal said *"The semantics copy one
  widely used system whole, Go's modules or Cargo, in its newest release"* (`235-*.md:60`), so its title's
  *"Go's modules copied whole"* names Go's module rules taken as the base, in the record's own words; R2:
  *"Go 1.17's rule is copied: the program's catalogue lists every package in the build, transitive ones
  included, each at the version minimal version selection chose and each with its fingerprint"*; *"at
  build it checks each entry's shape and each placed tree's fingerprint (R7)"*. The same proposal: *"The
  sitting chooses ... where fetched code lives"* (`:61-62`), which R7 answered with no grain stated.
- **R6** (`:612-633`): *"`sum` ... over a canonical listing of the package's files ... every regular
  file under the package's root, `.hero` or not, since a header reads any file"*.
- **R7** (`:635-651`): *"one tree per package keyed by its identity"*; *"The placed tree is checked
  against its fingerprint at every build, since a tree inside the program is a folder anything can
  edit"*; W4's cases (*"`sum`, `version`, `tag` and `path` mistyped, and the placed tree edited"*).
- **R12** (`:712-727`) and its home, `.claude/rules/cli-surface.md:52-78`: **the falsifier**,
  *"deleting the catalogue, deleting one of its entries, or mistyping any field of one never changes
  which module compiles, only whether the program builds, and a fetched copy left behind after its
  entry is gone compiles nothing"*; *"A fifth input class is refused ...: a second per-project file
  beside the catalogue, a lock, a workspace or a configuration, is a fifth class"*. The `check` form's
  cases of it (`ls -d tests/golden/check/package-*.case | wc -l`): 13.
- **R15** (`:744-756`), *"`core`: one catalogue entry"*, *"never `path` entries"* (747-748).
- **R19** (`:788-796`), **a ruling the question meets in its own right**: *"the interface that stays
  fixed from the first version to the second, as goldens: ... the fingerprint's algorithm ...; the
  placement folder; the verbs, their flags and the catalogue each writes, byte for byte; ... The second
  version replaces the transport alone"*; panel 242's R5 says the second version *"replaces no
  transport"* (`242-*.md:169`).
- **R20** as amended by decision 2528 (`235-*.md:971-983`).
- **Panel 234's R2** (`234-*.md:287-310`), quoted above, amended by 235's R15 (`234-*.md:692-699`:
  *"R2's placement, a copy of `core/` in each program's own root ... fails that sitting's question 17"*,
  R2's choice of Go's tree standing). **R13** (`234-*.md:488-499`): *"the compiler takes a copy under
  `selfhost/core/` only if the admission test admits it for its 26 lines"*, whose form panel 241's R3
  withdrew (`241-*.md:192-203`); **feature 2315's own notes still name it** (`gh issue view 2315 --json
  body`, its line 22: *"by packages copied into the compiler's tree under `selfhost/core/`"*).
- **Panel 238's R10** (`238-*.md:282-290`): *"every placed folder renamed ... 84 files under 14
  folders"*, the precedent for re-placing every example in one landing and publishing the site with it.
- **Panel 241**: R1 (`241-*.md:176-182`, *"the compiler takes no package of `core` 0.2.0"*, *"it adds
  2,334 lines"*), R3 (`:192-203`, *"When the compiler takes a package, it takes it as route 1 takes
  `core`: a catalogue entry, a placed tree, a fingerprint. And only scoped to **replace**"*), R8
  (`:258-269`).
- **Panel 242**: R1 (`242-*.md:129-139`, design.md:570-572 amended), **R2** (`:140-152`, *"either the
  compiler's tree, placed files counted, is smaller after it than before, or a copy it deletes is
  measured crashing or answering wrong on an input a caller reaches"*), **R3** (`:153-162`, the form (A),
  *"the one mechanism every program already uses, so no special case (§1.7)"*), **R4** (`:163-168`, *"The
  compiler's fingerprint of every placed tree stays the runtime's C doors"*), R7 (`:189-194`). design.md
  §1.11 carries R2's words (`docs/design.md:575-581`).
- **Panel 244's R1** (`244-*.md:80-100`, ratified 2026-11-11 at its line 216): the spec states three
  package rules and no other; *"Out: `heroes fetch` and `heroes.packages/`, the stand-in and
  outside-reading clauses"* (95-96); its bar, *"a sentence enters where a reader holding only the
  specification would otherwise write a wrong program or be unable to predict a refusal, measured,
  never because it is true"* (22-25).
- **Panel 246's R2** (`246-*.md:264-276`, *"ratified 2026-11-12"*): *"An edit inside a placed package
  tree is the author's work and is never discarded in silence"*; repaired by defect 2493, `d86722098`,
  *"a fetch or an add never replaces an edited placed tree"* (`git log -1 --format=%s d86722098`). A
  `fetch` that adds the newly reached files to a placed tree, or removes the newly unreached, writes into
  a placed tree, the act that ruling governs.
- **Decision 2528** (`gh issue view 2528`, CLOSED, *"Until the core packages reach 1.0.0, every program
  moves to the newest version of core and the older versions are removed from the index"*), written as
  § 10's steps 5 and 6 of `examples/packages/README.md`.
- **Spec § 1** (`spec/heroes-spec.md:22-55`): *"the program's catalogue `heroes.toml`, beside the file
  you compile, names with its source, exact version and `sum`"*, the example with one `sum` an entry;
  `grep -n 'heroes.packages' spec/heroes-spec.md` and `grep -n -w core` print nothing, as panel 244's R1
  ruled.
- **Where the same facts are written for readers** (`git grep -l 'heroes.packages' -- docs/design.md spec
  site/src docs/roadmap/milestones`, at the first base): `docs/design.md` (§1.11, 562-581; §3.5,
  912-925), `docs/roadmap/milestones/M-core-packages.md`, `site/src/html/docs/packages.html` and its
  Italian twin (both re-cut by the round), `site/src/html/examples/_fetched.html` and its twin,
  `site/src/lib/examples-model.ts`; `examples/packages/README.md`'s opening (16-24) and § 1 rule 4
  (44-53); and **the verbs' help**, `selfhost/cli/table.hero:194-195` (`add`: *"writes every package of
  the build with its sum, and places each"*; `fetch`: *"place every package heroes.toml names under
  heroes.packages/, each checked against its sum"*), which `git grep -l 'place every package heroes.toml
  names'` finds in `selfhost/cli/table.hero` and `seed/heroes.c` alone: no golden pins it. **The site's
  build reads every placed tree** (`site/src/lib/examples-model.ts:205-255`, `fetchedOf`), and
  `.github/workflows/deploy-site.yml`'s `paths:` hold `examples/**` and `packages/**` (its lines 63-64), so
  a landing that re-places the examples publishes the site with its push.

## The interface, fixed tonight, and what it does not pin

Task 2312 (`gh issue view 2312`, OPEN, M-core-packages, *"The package manager's interface fixed by
goldens: the contract its second version passes unchanged"*): lane b37-pm committed its goldens at
`10494dc23` and the round merged them at `952cc0f76`; `git diff --name-only 722ae76bf 10494dc23` adds
**13 `fetch` and 28 `check` case folders**, none holding a placed tree; at the base `ls -d
tests/golden/fetch/*/ | wc -l` prints **77**.

- **The `fetch` form records only the names of the placed folders**: `tests/harness/suite_fetch.hero:64-68`,
  *"`heroes.packages:` and the folders placed"*, and its code at 452-456, `list_directories` of
  `heroes.packages` and each folder's `last_part`. Of the 77 transcripts (`expected` in each case), 44
  print `(none)` after `heroes.packages:` and 33 a folder name, never a file (in the `b37-round`
  worktree, `awk '/^heroes.packages:$/{getline; if ($0=="(none)") f=1} END{exit !f}'` over each
  `tests/golden/fetch/*/expected`, and the lines after that heading listed: folder keys alone, such as
  `yaml@v0.3.1-0ee71d7abaa6570f`). **No golden pins which files are placed**, so a route that places a subset under
  an unchanged key moves no `fetch` transcript by what it places, and 2315's *"unchanged"* says nothing
  of placement either way: a route that changes placement owes a case of its own listing the files
  placed, and none exists.
- **55 golden case folders hold a placed tree by hand** (`git ls-files tests/golden | grep
  '/heroes.packages/'` in the `b37-round` worktree, by form: check 22, fetch 11, run 9, unsupported 8,
  surface-fixtures 3, full 2), **none with a listing**: a fingerprint route that hashes a placed listing
  at every build refuses every one of them, unless a tree with no listing keeps today's whole-tree check
  or every such case is rewritten.
- The order, this sitting's ruling against goldens already merged into the round, is a question for the
  synthesis.

## The routes, stated and none adopted

**Placement unit** (what lands under `heroes.packages/<key>/`):

- **W**, today: the version whole.
- **M**: the reached modules, the entry's own `heroes.toml`, and the C files its reached groups read.
  *Which* C files is the ffi-pragmatist's question: the header a group names, what that header
  includes (`tests/golden/run/fixedbugs-2351-two-fetched-packages-each-read-their-own-header-of-one-name.case`
  places `inner/deep.h` and a `same.h`), what clang's `-M` listing names (the build already keeps it,
  `selfhost/cli/package_reads.hero:1-30`), and what a header names only on another platform.
- **G**: by Go package, a face with its whole `internal/` subtree.
- **H**: the reached `.hero` modules, and every file that is not `.hero` placed whole (headers, data,
  cards, the entry's catalogue), so no C closure is computed.
- **For every program here G is M and H is M with the cards**: every reached face's `internal/` folder
  is reached whole (`encoding/json/internal/` 7 of 7, `net/http/internal/` 4 of 4, `slices/internal/` 2 of
  2, `net/internal/`, `strconv/internal/` and `strings/internal/` 1 of 1 each: `ls` of each folder at the
  base against the closures above), and `core` holds no C. They part only in a later version or another package; the
  seats are asked for the shape that parts them, not for three builds.

**Who decides the set, and when**: `heroes fetch` takes no operand (`selfhost/cli/table.hero:195`,
`operand: .no_operand`; its one flag `-C`, *"the program's folder, where its heroes.toml is"*, 152-154);
`heroes add` writes the entry and places (`:194`). Today no module of `selfhost/fetch/` reads a program's
`use` lines, by a search: `grep -h -E '^use ' selfhost/fetch/*.hero | sort | uniq -c` names no parser,
lexer or discovery module (its `module_text`, `module/reading` and `source` serve an address's entry
name and a file's text, the critic's reading of `add_wanted.hero:81` and `git_source.hero:112`). Which
roots a fetch-time closure starts from (every `.hero` under the folder, or a root named; `tests/harness`
holds six) is unanswered. A `use` added after a fetch is refused until the next fetch (measured above);
a `use` removed leaves placed files nothing reaches, kept, told or refused.

**The fingerprint** (each with what is verified at fetch, and at every build):

- **F1**: `sum` unchanged, over the version's whole listing; the listing placed with the files (the
  index's `list`, or the one the verb computes for a git source); every build hashes the listing against
  `sum` and each placed file against its line, a file absent being unreached, a file present and
  unlisted refused. **Where the listing stands is open**: inside the key folder it is *"a regular file
  under the package's root"*, which R6 puts under the sum (`235-*.md:621-622`), and a package may
  publish a file of that name; **and what a tree with no listing means** (the 55 hand-placed goldens,
  every example until re-placed).
- **F2**: a sum per module, or per Go package, written into the catalogue (R5's subset refuses arrays,
  `235-*.md:589-592`, so one entry per module or a new field shape): what a program's catalogue holds
  changes.
- **F3**: a sum over the placed subset, written by the verb: the catalogue's `sum` then moves with the
  program's `use` lines and is no longer the index's (the `packages` suite's `placed` check holds it to
  the index's, `suite_packages.hero:33-36`); and since the key hashes the `sum` (`placed.hero:128-134`),
  the placed folder's name moves with every `use` change, and for the compiler every change of its `use
  <core>` set would be a seed refresh.
- **F4**: the sum checked at fetch alone and the build reading the placed files unchecked: the reading
  panel 242's historian gave of Go's `-mod=vendor` (*"The sections read say nothing of a build hashing
  vendored files; that it does not is an inference from that silence, unrun"*,
  `docs/panel/242-reports/historian.md:125-136`), against R7's *"checked ... at every build"*.

**The alternatives a reader would ask about**, each stated, none adopted:

- **R2's ruler changed**: the compiler counted by the modules it reaches, not the files placed. The
  compiler already keys what a program was compiled from by *"every module the root reaches"*
  (`selfhost/emit/provenance.hero:7-9`); a change of design.md §1.11's sentence, Part 1, and of panel
  242's ratified R2.
- **`core` split into entries of its own**: `strings`, `strconv`, `net`, `encoding/json`, `slices` each a
  catalogue entry with its own version and sum, placed whole. It changes R15, the `use <core>/…` lines of
  17 programs and the net's own, the index's layout under `packages/`, and how decision 2528's *"every
  program moves to the newest version of core"* reads, per entry.
- **The placed trees not committed**: fetched into a cache or `build/`, so no program's tree carries
  them. It reads against R7 (*"inside the program"*), R8 and a compiler built from C alone, a reading of
  those texts, unrun; a fresh checkout would not build before a fetch.
- **Placement left whole and the ruler kept**: feature 2315 moved to a later milestone, or waiting for a
  package that meets R2 as it stands (the author's *b* and *c*, the milestone page).

**Nine routes nobody listed in the first briefs** (the critic's § L), each a question for the seats, none
adopted:

1. **Panel 234's grain, restored**: per-program copies of the reached Go packages, as the examples held
   from `43aae88d8` to `31c34d962` and `packages/copies` held them, now under a fingerprint: does it differ
   from M with F1 in anything but where the check runs?
2. **W kept, R2 met by its other branch**: R2 admits a package where *"a copy it deletes is measured
   crashing or answering wrong on an input a caller reaches"* (`242-*.md:142-144`); panel 242 found none
   among 13 copies at `1edabb957` (*"0 `.slice(` calls among them"*, 56-58), and `selfhost/` has moved 235
   files since: does a differential run of each live copy against `core`'s function, on its callers'
   inputs, find one now?
3. **M for the compiler alone**, every other program keeping W: no example, golden or site page moves.
   Does it read against panel 242's R3 (*"the one mechanism every program already uses, so no special
   case"*)?
4. **A listing kept in the catalogue** (the placed subset, or per-file hashes, named there) rather than
   placed in the tree: what shape would R5's subset admit, and what does a reader of the catalogue then
   read?
5. **A placement keyed by the reached set**: a key over the subset makes a stale placement visible by
   name; what does it cost in `#line` paths and seed refreshes (13 `#line`s naming the placed folder at
   0.2.0, carried from panel 241's compiler-engineer)?
6. **A package that declares its grain to the index** (per-face listings, the version's `sum` a hash
   over them, or a publisher's file list as Zig's `paths`: the survey's row at
   `235-reports/historian-survey.md:395`, *"Only files included in the package are used to compute a
   package's `hash`"*): one `sum` per entry kept, the fingerprint's algorithm, which R19 fixes, changed.
7. **A sum over a subset the catalogue declares** (a field naming the faces used, the sum computed over
   them by the verb or the index): F3 with the subset written rather than inferred, so the falsifier can
   be asked of the field.
8. **M as a flag of `fetch` and `add`**, W by default: a flag that changes how one question is answered
   (`.claude/rules/cli-surface.md:22-25` and 39-43's reading for `--package`)?
9. **The compiler reading `core` where the repository already holds it**, `packages/core/<v>/files/`,
   so nothing is added under `selfhost/`: does it turn on what *"the compiler's tree"* denotes, and does
   it collide with R15's *"never `path` entries"* and R6's unverified `path`?

**What M or H would also change**, for the seats to confirm or refute by running: `placed_said`'s
whole-tree hash and `fingerprint.added` (`packages.hero:215-265`), `fetch/staging.hero` and
`fetch/edited.hero` (defect 2493, panel 246's R2), `module/placed_beside.hero`, `module/standing.hero`,
`fetch/leftover.hero`, `module/offered_modules.hero` (*"the modules at the root of the package's tree
... read from the tree"*, its lines 1-5: a module the version holds and the program has not placed is
offered or not), the two messages quoted above, the verbs' help, the `packages` suite's `placed` check
(*"the index's files byte for byte"*), the 55 hand-placed golden cases, the `emission`, `warnings` and
`corpus` suites over `examples/` (whether any emitted C moves when the same modules are reached is
unrun), `suite_spec.hero:4215`'s placed path, the site's build, and the seed (each `core` version the
compiler takes is a refresh, panel 241's R3). **Arithmetic, not a measurement**: under decision 2528 a
program elsewhere holding a removed version keeps building on its placed tree (README § 9); under M it
could not place one more module of that version, the index no longer serving it, where under W the
files already stand.

## Where each seat works, and the rules every seat obeys

Every seat's folder is **`.claude/worktrees/p250-scratch/250-<seat>/`**, ignored by git (`git
check-ignore -v .claude/worktrees/p250-scratch/briefs`: `.gitignore:34:.claude/worktrees/`), never the
session scratchpad, never `/tmp`: CLAUDE.md § Hard stops writes no file outside the repository's root,
and a redirection to `/tmp` is such a write (the critic made one, `/tmp/x`, read back and removed, its
§ M). Read-only to the seats, copied into your own folder before use: `.claude/worktrees/p250-scratch/briefs/`
(the coordinator's runs: `reach/closure.sh`, `programs.sh`, `sets.sh`, `lines48.sh` and their outputs;
`partial/` at the first base; `partial48/` and `base48/` at this one), `.claude/worktrees/p250-scratch/critic/`,
the lanes' worktrees under `.claude/worktrees/` (`b37-round` and the rest), and every other sitting's
scratch.

- **Your own folder, and nothing else**: the tree in `tree/`, every probe beside it, `TMPDIR` set to
  its `tmp/` before any build or tool. Never the trunk's `./heroes`, a compiler of
  `.claude/worktrees/p250`, of `p250-scratch/briefs/`, of the critic, of a lane or of another seat; never
  read, build or run inside another seat's folder.
- **Your report**: `docs/panel/250-reports/<seat>.md` in `.claude/worktrees/p250`, written as you go,
  not only at the end, in your definition's output structure (`.claude/agents/<seat>.md`). Write nothing
  else in `.claude/worktrees/p250`.
- **Every seat that builds builds the route it recommends, in its own copy, and runs the cases it
  names, before the synthesis** (`SKILL.md` 3c): a route that does not build is reported as not built,
  never adopted.
- **Every number with the command that produced it; every negative sentence run, or posed as a
  question**; the words *only*, *never*, *provably* with what was searched and where (`/panel` step 2;
  CL-077). **A claim about another language's tool from memory is a question for the historian, never
  a premise**, in your report as in these briefs.
- **No paid run** (above). **Never print a credential**: `gh auth status` at most, never `gh auth token`.
  No GitHub write, no issue, no commit, no push, no tag; `gh issue view` and `gh issue list` are reads.
- **Names**: the two compilers keep the recipe's names, `heroes` and `heroes-next`, inside your own
  `tree/`; **every other binary you build** (a program, a probe, a compiler you patch) is named with your
  prefix, **`ce-`** for the compiler-engineer, **`fp-`** for the ffi-pragmatist, **`sw-`** for the
  spec-warden, so `pgrep` and a crash report name whose it is.
- **A program that may not end** runs as a BINARY under `timeout -s KILL <seconds>`, never `timeout
  heroes run` and never a plain `timeout`; afterwards `pgrep -f <your folder>` read and anything left
  stopped before a verdict (`.claude/rules/verification.md` § A run that may not end). `timeout` here is
  GNU coreutils 9.12 at `/opt/homebrew/bin/timeout` (`command -v timeout`, `timeout --version`). The
  bounds: a compile, `check`, `fetch` or `add`, `timeout -s KILL 300`; a built program, `timeout -s KILL
  60`; `examples/server`'s binary, if a seat runs it, `timeout -s KILL 60` and each client `curl
  --max-time 5`, the client side under `timeout -s KILL 90`; a Docker run, below. **A program's output**
  goes to `/dev/null` or through `head -c 65536` into a file. **A suite's or the harness's output goes
  whole into a file and is read from it**: the guard refuses a gate piped into `head` or `tail` (the
  critic was refused, *"the net's output piped into `tail` cuts the lines that carry the verdict"*, its §
  M; `.claude/rules/verification.md` § A suite is the last judge, layer 1).
- **One heavy command at a time** (a stage 2 build, a suite): other sessions build on this Mac, and the
  one-minute load average read 150 at 22:34 and 308 at 23:23. Whole suites only where your route changes what the
  checker refuses, named in your report with why; otherwise a form narrowed to your cases with the
  harness's third word (`./heroes-next run tests/harness/main.hero -- ./heroes-next <form>
  <case-substring>`), the `fetch` and `packages` suites by name, one at a time.

## The platforms

- **Darwin arm64**: this Mac, Apple clang 21.0.0 (above). `pmset -g assertions` read
  `PreventUserIdleSystemSleep 1` at 22:31, so something already holds the machine awake
  (`.claude/rules/verification.md` § A long run holds the machine awake): ask it again before a long run,
  `caffeinate -i` only where it reads 0.
- **Linux arm64**: the image `heroes-linux-arm64:latest`, `338a1044ac90` (`docker images
  heroes-linux-arm64`, which also lists `:sdl3-b14`, `75742c2bf90b`). Write a script into a folder of
  yours and run it from the repository's root:

  ```
  git -C .claude/worktrees/p250 archive 48f53b3437c6cd46aba5923f4f4780ac7b347283 | timeout -s KILL 900 docker run -i --rm --name <prefix>-250-<n> -v <your folder>/linux:/out heroes-linux-arm64:latest bash /out/run.sh
  docker ps --filter name=<prefix>-250-<n> --format '{{.Names}} {{.Status}}'   # after it, whatever its exit
  docker stop -t 1 <prefix>-250-<n>                                            # where that line printed a container
  ```

  the script reading the tree from its stdin into a folder of the container's own, building the seed
  and stage 2 there, every probe inside it under its own `timeout -s KILL`, its log under `/out`, every
  program's output bounded with `head -c`. The outer `timeout` bounds the client, not the container
  (panel 248's critic, carried: `docker ps` still listed a container the outer `timeout` had cut), so
  read `docker ps` after every run. One container of yours at a time. Unrun by the coordinator: stage 2's
  `sources` line in that image.
- **Windows: the box is OFF.** `ssh -o BatchMode=yes -o ConnectTimeout=15 win 'echo up'` at 22:36
  printed *"ssh: connect to host apponfly-vps port 22: Operation timed out"*, exit 255
  (`briefs/win-check.txt`), and the critic's at 23:13:34 the same. **A seat that needs a Windows
  measurement names it in its report as owed**, and **the CI's Windows leg judges after the push**
  (`.claude/rules/verification.md` § The optimistic chain, its item 4).

## Questions no one has run, posed as questions, with the seat that would run them

- **R2's unit** (lines, the `layout` suite's code lines, bytes, files; whether cards, a placed listing
  and the compiler's catalogue count): the compiler-engineer counts every unit, the spec-warden reads
  which one design.md:575-581 and panel 242's R2 mean.
- **What *"the compiler's tree"* is**: `selfhost/`, `selfhost/` and `runtime/`, or the repository whose
  `packages/` holds every version's files (route 9 turns on it): the spec-warden.
- **Which *package* R2's *"every live copy of that package's contract"* means**: the entry (every face of
  `core`, so the compiler deletes its copies of `strconv`, `encoding/json` and `slices` too) or the Go
  package it reaches: the spec-warden reads it, the compiler-engineer counts both.
- **Why whole placement replaced panel 234's reached grain at `31c34d962`**, and whether any record
  weighed it: the spec-warden and the historian read panel 235's briefs, reports and that commit.
- **Which version the ruling is measured over**: 0.5.0 at this base; whether the compiler's reached set
  over `strings` moves between versions (`strings.hero` 258 lines and `strings/internal/scan.hero` 110 at 0.2.0 and at 0.5.0, `git show
  <c>:packages/core/<v>/files/<f> | wc -l` at `2ca3fd0a8` and `48f53b343`; 368 at 0.4.0 by the first
  table): the
  compiler-engineer.
- **What `fetch` does to a placed tree when the `use` set moves**, read against panel 246's R2 and defect
  2493's *"never replaces"*: the compiler-engineer builds it, the spec-warden reads it.
- **Where F1's listing stands**, and what a tree with none means: the compiler-engineer.
- **What case would pin which files are placed**: whoever builds M writes it.
- **Who computes the reached set at `fetch`**, and over which roots: the compiler-engineer.
- **What 2315 replaces, and with which package**, its notes still naming `selfhost/core/`, panel 242's R7
  naming `crypto/sha256`, which no version of `core` holds (`git ls-tree -r --name-only 48f53b343
  packages/core | grep -c crypto` prints 0): the compiler-engineer counts, the spec-warden reads.
- Which files a reached group's C build reads, per platform, and whether clang's `-M` listing on one
  platform names what another platform's build needs: the ffi-pragmatist.
- Whether a fetched package's `link "x"` group can find an archive the package carries: by the source
  read, `-L` is built in `selfhost/cli/link.hero:167` for each folder of its `search_library` and at
  `:819` in a test, and elsewhere only in `pkg-config` answers' filters and tests (`grep -n -E
  '"-L"|"-L|-L ' selfhost/cli/*.hero`), none traced to a placed root: the ffi-pragmatist runs it.
- Whether any emitted C, any `#line`, any provenance key, or any `emission` golden moves when the same
  modules are reached from a pruned tree: the compiler-engineer.
- What a build costs, in instructions, hashing a 30-file tree against hashing a listing and the files
  reached: the compiler-engineer.
- What Go (`go mod vendor`, `vendor/modules.txt`, `go.sum`'s `h1`), Cargo (`cargo vendor`,
  `.cargo-checksum.json`), Deno (its vendor folder, its lock, JSR's per-file serving), npm, pip and uv
  each place or vendor, and what each one's lock or sum covers: the historian, sourced and dated.
- Whether a listing placed inside the tree, or any record of which modules were placed, is a fifth input
  class under `.claude/rules/cli-surface.md`'s last paragraph: the spec-warden.
