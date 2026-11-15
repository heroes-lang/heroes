# Panel 250, the brief every seat reads first

Convened 2026-11-14 by the author's answer **`1a`**, given at about 22:20 to a batch put in plain text
with the coordinator's recommendations, and recorded twice: as a comment on feature 2315 (`gh issue
view 2315 --repo heroes-lang/heroes --comments`, its last comment, *"**The author's answer of
2026-11-14, at about 22:20, `1a`** ... a sitting is convened on placing only the modules a program
uses"*), and on the milestone's page in commit **`b5323a21c`** (*"M-core-packages: the author's answers
of 2026-11-14 evening, a sitting on placing only the modules a program uses"*), which stands on branch
`b36-round` alone (`git branch -a --contains b5323a21c`: `b36-round`), is **not pushed**, and is one
commit on top of this sitting's base touching that page alone (`git log --oneline 722ae76bf..b5323a21c`:
one line; `git diff --stat 722ae76bf b5323a21c`: `docs/roadmap/milestones/M-core-packages.md | 17
+++`). **Your copy of the base does not hold that paragraph**: read it with `git -C
/Users/joseph/Temp/heroes/heroes-lang show b5323a21c:docs/roadmap/milestones/M-core-packages.md`, its
last section, *"The author's answers of 2026-11-14, evening: the ruler moved away, so a sitting"*.

**The seats**: the compiler-engineer, the ffi-pragmatist, the spec-warden, the historian (advisory),
and the completeness critic, who reads these briefs before any seat starts and the reports after.
**No blind seat** (the llm-ergonomist): the question as asked changes no line a program writes. A
program still writes `use <core>/strings` and its catalogue still holds one entry with a `version`
and a `sum`; what moves is which files the verbs place under `heroes.packages/` and what the build
checks of them. **That is a claim about the routes listed below, and two of them break it** (a sum
per module written into the catalogue, and `core` split into packages of their own, § The routes):
**a seat that finds a route it would adopt changes what a program, its catalogue or a reader of the
spec writes says so first, in those words**, and the coordinator then asks the author for the blind
seat's budget before the synthesis. No paid run is named in this sitting, by anyone: no `claude -p`,
no API call, no `heroes measure --refresh` (the spec-warden's counts are the vendored tables, a lower
bound, said in those words), no cloud run. A seat that finds one worth running names it in its report
with its size and cost, for the coordinator.

**Where this brief is wrong, say so first.** Every number, path and count below carries the command
that produced it, run by the coordinator between about 22:28 and 22:55 on 2026-11-14, every output
under `.claude/worktrees/p250-scratch/briefs/`; a figure copied from an issue, a report or an earlier
sitting says *carried* beside it, and is yours to re-run before you lean on it.

## Words used here

`package` is two things in this repository, and this sitting needs both apart:

- an **entry**: what a catalogue names and the index versions, `core`, one `version` and one `sum`
  (panel 235's R15, *"`core`: one catalogue entry"*);
- a **module**: one `.hero` file; a **face** is the module at a Go path that a program names,
  `strings.hero`, `net/http.hero` (`examples/packages/README.md` § 1, rule 2), and a **Go package** is a
  face with its private modules under `<path>/internal/` (rule 3);
- **placed**: what `heroes add` and `heroes fetch` write under a program's `heroes.packages/<key>/`;
  **reached**: the modules a program's `use <name>/…` lines name, closed under the entry's own `use`
  lines, which is what the compiler compiles.

## The mandate, and the lane

- **CLAUDE.md § 4** (*"Ask once per milestone, then convene without asking again"*): the milestone's
  gate was asked at panel 242, whose status reads *"convened without asking again (the author's panel
  gate for M-core-packages was asked at 07:35 on 2026-11-10)"* (`docs/panel/242-*.md:3-4`); and this
  sitting was convened by the author's own answer above.
- **Why the full panel and not the soundness lane** (`.claude/skills/panel/SKILL.md:18-34`): the
  question touches the tool surface (`heroes add`, `heroes fetch`, `.claude/rules/cli-surface.md`'s
  falsifier), the architecture of what the build reads, a sentence of spec § 1 (*"names with its
  source, exact version and `sum`"*), and design.md §1.11's ruler (*"placed files counted"*), each a
  trigger of CLAUDE.md § 4. The blind seat is left out for the reason above, and its escape stands.

## What is being asked

**Today a package is placed whole.** `heroes fetch` places every file of a version, held to the
catalogue's `sum`, which is taken over the version's whole tree; and panel 242's R2 (ratified
2026-11-10, `docs/panel/242-*.md:140-152`) lets the compiler take a package *"only where its own tree
shrinks or a measured defect closes"*, the tree counted *"placed files counted"*. The milestone's
page at `b5323a21c` reads the consequence: *"so every version moves the ruler further"*, and the
acceptance condition of feature 2315 (*"The package manager rewritten over the core packages, every
golden of its interface unchanged"*, `gh issue view 2315 --json title`, OPEN, M-core-packages) *"could
not be met as the rules stand"*. **That is the page's reading, not a measurement at this base**: whether
any set of deletions could meet R2 over `core` 0.4.0 placed whole is the compiler-engineer's to count.

**The sitting asks whether, and how, a program places only the modules it reaches** (with the C files,
headers and archives those modules' groups name, and the entry's own catalogue), and what that changes
in: the fingerprint (`sum`: what is verified, and when); `heroes fetch` and `heroes add`, and the
interface task 2312 is fixing by goldens now; the build's reading of a placed tree; the catalogue
(spec § 1); the falsifier of `.claude/rules/cli-surface.md` § Four input classes; panel 235's R2 and
R6 (and R7, R15, R19, which the same sentences reach); panel 234's R2 and R13; panels 241's and 242's
rulings; and decision 2528 (the author's *2b*). And the alternatives a reader would ask about, each
stated below and **none adopted by these briefs**.

## Whole placement today, read in the code and run

**What `heroes fetch` places** (`selfhost/fetch/placing.hero:32-46`, `placed_tree`): a tree under
`heroes.packages/` kept where `fingerprint.of(at)` equals the entry's `sum`, otherwise fetched whole
from its source, `git_source.fetched` or `index_source.fetched`. The index's layout
(`selfhost/fetch/index_source.hero:1-23`): `<index>/<name>/versions`, `<index>/<name>/<version>/list`
(*"the very text `sum` is the SHA-256 of"*), and `<index>/<name>/<version>/files/<path>`, *"each file
at its own address, as JSR serves them, so the first version needs `curl` alone"*; *"Checked twice
before it is placed: each file against its line of the listing as it lands ... and the staged tree's
fingerprint, read from the disk, against the entry's sum"*.

**What the sum is** (`selfhost/module/fingerprint.hero:1-17`): *"the SHA-256 of a listing of every
regular file under the package's root, one line per file, its SHA-256 in hexadecimal, two spaces, its
name ... sorted by the names' bytes"*, Go's `h1` in hexadecimal; every file hashed in C
(`hero_file_sha256`), *"Checked at every build that reads the tree"*. The index publishes that listing
as `list` (`examples/packages/README.md` § 10, step 3, *"Every file is listed, the cards included"*).

**What the build reads** (`selfhost/module/packages.hero`): `rooted` (142-167) puts a fetched entry's
modules at `heroes.packages/<placed.key(e, index)>/`; `placed_said` (215-234) computes
`fingerprint.of(package.opened)` over the whole placed folder and compares it with `e.sum`, a mismatch
told by `changed_said` (249-265), which tries `fingerprint.added` (`fingerprint.hero:95`: the files
added since placing, found by subtracting listing lines until the hash meets the sum). The folder's key (`selfhost/module/placed.hero`, `key` at 118, `keyed` at 128): `name@version-`
and the first sixteen hexadecimal digits of the SHA-256 of the source, name, version and sum, so it
names no subset.

**Run at the base by the coordinator** (`.claude/worktrees/p250-scratch/briefs/partial/`, a `git archive`
of `examples/ini`, `examples/server` and `packages` at the base, each output below in its `examples/`, the base's stage 2 `heroes-next`, every
run under `timeout -s KILL 120` or `300`):

- `examples/ini` with its placed tree whole: `heroes-next check main.hero`, **exit 0**, nothing printed
  (`ini-whole-check.txt`).
- The same with only the three files it reaches or documents kept, `strings.hero`, `strings.md`,
  `strings/internal/scan.hero` (the other 23 removed, `find ... | xargs rm`): `check` **exit 1**,
  *"error[package_unread]: `<core>` cannot be read: the tree placed at
  `heroes.packages/core@0.4.0-c65a01aeebf10d3c/` is `sha256-ae99678252e9...`, and the catalogue's `sum`
  for it is `sha256-1b39ba5f301c...`: a file of it was changed, added or taken out since it was placed"*,
  at `main.hero:28:5` (`ini-partial-check.txt`).
- `heroes-next fetch` in that pruned copy: **exit 1**, *"heroes.toml:5: error[sum_mismatch]: `core`
  0.4.0 ... that tree was changed since it was placed: `encoding/json.hero` taken out, ... and 13 more;
  the change is yours, so nothing was placed over it"*, the three files left (`ini-partial-fetch.txt`).
  So today a pruned tree is an edit by the author, and neither verb nor build repairs it.
- `heroes-next fetch` in a copy holding only `heroes.toml`, `main.hero`, `main.expected`: **exit 0**,
  *"placed core 0.4.0 heroes.packages/core@0.4.0-c65a01aeebf10d3c"*, 26 files placed (`find
  heroes.packages -type f | wc -l`), `diff -r` against the tracked tree silent (`ini-fresh-fetch.txt`).

## The numbers that make the question

Each run by the coordinator; `wc -l` counts lines, newlines included.

- **`core` at the base**: `packages/core/versions` holds one line, `0.4.0
  sha256-1b39ba5f301cc858ecec923dd4c7efd45e4a72ff39e43eb28034109abfe05089`; `find
  packages/core/0.4.0/files -type f | wc -l`: **26** files; `... -exec cat {} + | wc -l`: **6,140**
  lines; of them **20** `.hero` files, **5,998** lines, and 6 cards (`.md`). No file of 0.4.0 is C
  (`find packages/core/0.4.0/files -type f \( -name '*.c' -o -name '*.h' \) | wc -l`: 0), and its one
  `extern` group is `net/hero_net.hero:15`, `extern "hero_net.h"`, a runtime header (`grep -rn
  '^extern' packages/core/0.4.0/files`).
- **Earlier versions**, removed from the index by decision 2528 and read from history (`git ls-tree -r
  --name-only <c> packages/core/<v>/files | while read f; do git show <c>:"$f"; done | wc -l`, `<c>`
  `2ca3fd0a8` = `42f4e2718^` for the first three, `00dfa9151` = `ad32e8a34^` for 0.3.0): **0.1.0** 6
  files, 522 lines; **0.2.0** 13 files, **2,389** lines (the figure the author was given, re-measured);
  0.2.1 15 files, 4,259; 0.3.0 26 files, 5,962.
- **0.5.0** is lane b37-core's, committed at `abfdc9e32` on its branch and not in the base (`git
  ls-tree` of that commit as above): **30** files, **6,574** lines, the four new files `slices.hero`,
  `slices.md`, `slices/internal/merge.hero`, `slices/internal/sample.hero`. Read at 22:28, read-only; a
  seat measures 0.4.0, the base's.
- **What the programs carry**: `git ls-files examples | grep -c '/heroes.packages/'`: **416** files in
  16 programs, each holding one tree `core@0.4.0-c65a01aeebf10d3c` of 26 files; of them **320** `.hero`
  (`grep -c '\.hero$'`) and 96 cards; 98,240 lines (`| xargs cat | wc -l`); and 26 more under
  `tests/harness/heroes.packages/`, the net's own catalogue (`tests/harness/heroes.toml`). **The 320 the
  author was given is the `.hero` count**, not every file placed.
- **What each program reaches**: `.claude/worktrees/p250-scratch/briefs/reach/programs.sh`, which
  reads each program's own `use <core>/…` lines and closes them under the placed tree's bare `use`
  lines with `closure.sh` beside it (`programs-722ae76bf.txt`):

  | programs | names | reached modules | their lines | cards |
  |---|---|---|---|---|
  | adventure, ini, ledger, logs, markdown, maze, wordbands, wrap, `tests/harness` | `strings` | 2 | 368 | 1 (21 lines) |
  | assembler, query, rpn, spreadsheet, todo | `strconv`, `strings` | 4 | 489 | 2 (33) |
  | json | `encoding/json` | 8 | 3,715 | 1 (22) |
  | server | `net`, `net/http`, `strconv`, `strings` | 12 | 2,283 | 5 (120) |
  | packages (the driver) | `encoding/json`, `net/http`, `strconv`, `strings` | 20 | 5,998 | 6 (142) |

  In all, **78** reached modules of 340 placed `.hero` files (17 programs × 20), 17,753 of 104,380
  placed lines (442 files, the 26 of `tests/harness` included). **That table is a textual closure, not
  the compiler's answer.** Asked of the compiler for two programs, the way the `packages` suite's
  `reach` check asks it (a module is reached where a `#line` of `--emit-c` names it,
  `tests/harness/suite_packages.hero:37-40`): `heroes-next build main.hero --emit-c` in `ini` and
  `server`, the placed paths of its `#line`s (`grep -o -E '#line [0-9]+ "[^"]*heroes\.packages/[^"]*"'
  | sort -u`): ini **2** (`strings.hero`, `strings/internal/scan.hero`), server **12**, the same files as
  the table (`partial/examples/*.reached-by-line.txt`). That suite's own caveat: *"a module that declares
  only records emits no `#line`"* (its lines 68-71), so `#line`s undercount where such a module is
  reached.
- **The compiler** carries no catalogue at the base (`ls selfhost/heroes.toml`: *No such file or
  directory*); its tree is 849 `.hero` files, 221,545 lines (`git ls-files selfhost | grep '\.hero$' |
  xargs cat | wc -l`); `selfhost/cli/strings.hero` is 233 lines (`wc -l`), 8 functions (`grep -c
  '^function '`), used by 157 files (`grep -rl '^use cli/strings' selfhost | wc -l`). **Carried from panel 242** (its compiler-engineer, at base
  `1edabb957`, over `core` 0.2.0): taking `core` compiler-wide deleted *"16 functions, 186 lines, in 12
  modules"* and added 2,334 placed lines, *"the compiler's tree grows by 2,325 lines net"*
  (`docs/panel/242-*.md:53-56`); panel 241 measured the compiler reaching *"368 of the placed tree's 2,334 lines"* (`241-*.md:68`). **An
  arithmetic, never a measurement**: 368 reached lines against 186 deleted would still grow the tree by
  182, so placing only the modules used may not, by itself, meet R2 over `strings` alone; what the
  compiler could delete over 0.4.0 (its `strconv`, its JSON code: `git ls-files selfhost | grep -i json`
  lists 7 files, 1,386 lines; panel 241 found *"none [no contract] for ... `encoding/json`"* at 0.2.0,
  carried) is the compiler-engineer's to count at this base.

## The rulings this question meets, each quoted from the base

- **Panel 235's R2** (`235-*.md:545-566`), the sitting whose title says *"Go's modules copied whole"*:
  *"the base: Go's modules in their newest release, without workspaces"*; *"the program's catalogue
  lists every package in the build, transitive ones included, each at the version minimal version
  selection chose and each with its fingerprint"*; *"at build it checks each entry's shape and each
  placed tree's fingerprint (R7)"*. **Read the phrase as the sitting wrote it**: *copied whole* names
  Go's module rules taken as the base, not a placement rule; whether placing a subset departs from R2
  or from Go is the spec-warden's reading and the historian's evidence, not a premise here.
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
  cases of it (`ls -d tests/golden/check/package-*.case`): 13, among them `package-catalogue-deleted`,
  `-entry-deleted`, `-sum-mistyped`, `-version-mistyped`, `-tag-mistyped`, `-commit-mistyped`,
  `-tree-edited`.
- **R15** (`:744-756`), *"`core`: one catalogue entry"*; **R19** (`:788-796`), *"the interface that
  stays fixed from the first version to the second, as goldens: ... the fingerprint's algorithm ...;
  the placement folder; the verbs, their flags and the catalogue each writes, byte for byte"*; **R20**
  as amended by decision 2528 (`:971-983`).
- **Panel 234's R2** (`234-*.md:287-310`): *"it carries under its own root the packages it reaches,
  byte-identical to the canonical tree"*, amended by 235's R15 (`:692-699`: *"R2's placement, a copy
  of `core/` in each program's own root ... fails that sitting's question 17"*, R2's choice of Go's
  tree standing). **R13** (`:488-499`): *"the compiler takes a copy under `selfhost/core/` only if the
  admission test admits it for its 26 lines"*, whose form panel 241's R3 withdrew (`241-*.md:192-203`).
- **Panel 241**: R1 (`:176-182`, *"the compiler takes no package of `core` 0.2.0"*, *"it adds 2,334
  lines"*), R3 (`:192-203`, *"When the compiler takes a package, it takes it as route 1 takes `core`: a
  catalogue entry, a placed tree, a fingerprint. And only scoped to **replace**"*), R8 (`:258-269`).
- **Panel 242**: R1 (`:129-139`, design.md:570-572 amended), **R2** (`:140-152`, *"either the
  compiler's tree, placed files counted, is smaller after it than before, or a copy it deletes is
  measured crashing or answering wrong"*), R3 (`:153-162`, the form (A)), **R4** (`:163-168`, *"The
  compiler's fingerprint of every placed tree stays the runtime's C doors"*), R7 (`:189-194`). design.md
  §1.11 carries R2's words (`docs/design.md:575-581`, *"it takes one only where its own tree, placed files
  counted, is smaller after the step than before"*).
- **Decision 2528** (`gh issue view 2528`, CLOSED, *"Until the core packages reach 1.0.0, every program
  moves to the newest version of core and the older versions are removed from the index"*), written as
  § 10's steps 5 and 6 of `examples/packages/README.md`.
- **Spec § 1** (`spec/heroes-spec.md:22-55`): *"the program's catalogue `heroes.toml`, beside the file
  you compile, names with its source, exact version and `sum`"*, the example with one `sum` an entry;
  `grep -n 'heroes.packages' spec/heroes-spec.md` prints nothing, so the spec names no placement folder.
- **Where the same facts are written for readers** (`git grep -l 'heroes.packages' -- docs/design.md spec
  site/src docs/roadmap/milestones`): `docs/design.md` (§1.11, 562-581; §3.5, 912-925),
  `docs/roadmap/milestones/M-core-packages.md`, `site/src/html/docs/packages.html` and its Italian twin,
  `site/src/html/examples/_fetched.html` and its twin, `site/src/lib/examples-model.ts`; and
  `examples/packages/README.md`'s opening (16-24) and § 1 rule 4 (44-53). **The site's build reads every
  placed tree** (`site/src/lib/examples-model.ts:205-255`, `fetchedOf`), and
  `.github/workflows/deploy-site.yml`'s `paths:` hold `examples/**` and `packages/**` (its lines 63-64),
  so a landing that re-places the examples publishes the site with its push.

## The interface, being fixed now

Task 2312 (`gh issue view 2312`, OPEN, M-core-packages, *"The package manager's interface fixed by
goldens: the contract its second version passes unchanged"*) is lane b37-pm's tonight. At the base,
`ls -d tests/golden/fetch/*/ | wc -l`: **64** case folders; the lane's worktree held **10** more, untracked
(`git -C .claude/worktrees/b37-pm status --short tests/golden/fetch | wc -l`, at 22:30, read-only). And
**55** golden case folders hold a placed tree by hand (`git ls-files tests/golden | grep
'/heroes.packages/'`, by form: check 22, fetch 11, run 9, unsupported 8, surface-fixtures 3, full 2). So
**2315's "unchanged" is measured against goldens written while this sitting sits**: a route that changes
what is placed, or what a verb prints, changes those goldens, and the order (this sitting's ruling
before or after b37-pm's cases land) is a question for the synthesis, which every seat may answer.

## The routes, stated and none adopted

**Placement unit** (what lands under `heroes.packages/<key>/`):

- **W**, today: the version whole.
- **M**: the reached modules, the entry's own `heroes.toml`, and the C files its reached groups read.
  *Which* C files is the ffi-pragmatist's question: the header a group names, what that header
  includes (`tests/golden/run/fixedbugs-2351-two-fetched-packages-each-read-their-own-header-of-one-name.case`
  places `inner/deep.h` and a `same.h`), what clang's `-M` listing names (the build already keeps it,
  `selfhost/cli/package_reads.hero:1-30`), and what a header names only on another platform.
- **G**: by Go package, a face with its whole `internal/` subtree, Go's own unit of compilation (the
  historian's to source).
- **H**: the reached `.hero` modules, and every file that is not `.hero` placed whole (headers, data,
  cards, the entry's catalogue), so the C closure is never computed.
- And whether cards are placed: none is compiled, all are listed and summed (README § 10, step 3).

**Who decides the set, and when**: `heroes fetch` takes no operand (`selfhost/cli/table.hero:195`,
`operand: .no_operand`) and works on a folder's catalogue, so which roots its closure starts from (every
`.hero` under the folder outside `heroes.packages/`, or a root named) is unanswered; `heroes add`
writes the entry and places (`:194`). A `use <core>/net` added after a fetch makes the build refuse
until the next fetch (R8: *"`build`, `check` and `run` never fetch"*), told how; a `use` removed leaves
placed files nothing reaches, kept, told or refused.

**The fingerprint** (each with what is verified at fetch, and at every build):

- **F1**: `sum` unchanged, over the version's whole listing; the listing placed beside the files (the
  index's `list`, or the one the verb computes for a git source); every build hashes the listing
  against `sum` and each placed file against its line, a file absent being unreached, a file present
  and unlisted refused.
- **F2**: a sum per module, or per Go package, written into the catalogue (R5's subset refuses arrays,
  `235-*.md:589-592`, so one entry per module or a new field shape; a change to what a program's
  catalogue holds, **reader-facing**).
- **F3**: a sum over the placed subset, written by the verb: the catalogue's `sum` then moves with the
  program's `use` lines and is no longer the index's (the `packages` suite's `placed` check holds it to
  the index's, `suite_packages.hero:33-36`).
- **F4**: the sum checked at fetch alone and the build reading the placed files unchecked: the reading
  panel 242's historian gave of Go's `-mod=vendor` (*"The sections read say nothing of a build hashing
  vendored files; that it does not is an inference from that silence, unrun"*,
  `docs/panel/242-reports/historian.md:125-136`), against R7's *"checked ... at every build"*.

**The alternatives a reader would ask about**, each stated, none adopted:

- **R2's ruler changed**: the compiler counted by the modules it reaches, not the files placed. The
  compiler already keys what a program was compiled from by *"every module the root reaches"*
  (`selfhost/emit/provenance.hero:7-9`); a change of design.md §1.11's sentence, Part 1, and of panel
  242's ratified R2.
- **`core` split into entries of its own**: `strings`, `strconv`, `net`, `encoding/json` each a
  catalogue entry with its own version and sum, placed whole. It changes R15, the `use <core>/…` lines
  (16 programs and the net's own), the index's layout under `packages/`, and how decision 2528's *"every
  program moves to the newest version of core"* reads, per entry; **reader-facing**.
- **The placed trees not committed**: fetched into a cache or `build/`, so no program's tree carries
  them. It reads against R7 (*"inside the program"*), R8 (a build never fetches) and a compiler built
  from C alone, a reading of those texts, unrun; stated because a reader asks it.
- **Placement left whole and the ruler kept**: feature 2315 moved to a later milestone, or waiting for
  a package that meets R2 as it stands (the author's *b* and *c*, the milestone page at `b5323a21c`).

**What M or H would also change**, for the seats to confirm or refute by running: `placed_said`'s
whole-tree hash and `fingerprint.added` (`packages.hero:215-265`), `fetch/staging.hero` and
`fetch/edited.hero` (a tree edited since placing, defect 2493), `module/placed_beside.hero`,
`module/standing.hero`, `fetch/leftover.hero`, `module/offered_modules.hero` (*"the modules at the root
of the package's tree ... read from the tree"*, its lines 1-5: a module the version holds and the
program has not placed is offered or not), the `packages` suite's `placed` check (*"the index's files
byte for byte"*), the 55 golden cases above, the `emission`, `warnings` and `corpus` suites over
`examples/` (whether any emitted C moves when the same modules are reached is unrun), the site's build,
and the seed (each `core` version the compiler takes is a refresh, panel 241's R3).

**Arithmetic on measured numbers, not measurements**: moving the 17 programs to 0.5.0 under W places 510
files (17 × 30); under M or H, each program's set above, plus `slices` where a program names it.
Under decision 2528 a program elsewhere holding a removed version keeps building on its placed tree
(README § 9); under M it could not place one more module of that version, the index no longer serving
it, where under W the files already stand: a reading, unrun.

## The base

**`722ae76bf693bebdb7a5cca37052ed92646753c0`**, its tree **`ce81c0b18b1fbeb616f95aeef619d5b8cd23bfbe`**
(`git -C .claude/worktrees/p250 log -1 --format='%H %T %ci %s'`), *"Panel 248 ratified, R1 to R6, and
the two choices of R5's landing kept"*, 2026-11-14 21:55:01 +0100, the head of branch `p250` in
`.claude/worktrees/p250`, its `git status --short` empty when these briefs were begun. Every path below
is from the repository's root, `/Users/joseph/Temp/heroes/heroes-lang`. Every seat that builds copies
the commit and builds its own compiler in two stages:

```
mkdir -p .claude/worktrees/p250-scratch/250-<seat>/tree .claude/worktrees/p250-scratch/250-<seat>/tmp
git -C .claude/worktrees/p250 archive 722ae76bf693bebdb7a5cca37052ed92646753c0 | tar -x -C .claude/worktrees/p250-scratch/250-<seat>/tree
cd .claude/worktrees/p250-scratch/250-<seat>/tree
export TMPDIR=$PWD/../tmp
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes       # the seed
./heroes build selfhost/main.hero -o heroes-next                 # stage 2
./heroes-next --version                                          # second line: sources 2ab7ca0a644692a1
```

Measured by the coordinator at 22:31 to 22:37 in `.claude/worktrees/p250-scratch/briefs/base/tree`, made
exactly that way: the archive holds no `build/` (`ls build`: *No such file or directory*), the seed and
stage 2 both exit 0, `heroes-next --version` printed `heroes 0.6.0` and **`sources 2ab7ca0a644692a1`**,
`heroes-next doctor` printed `ok   clang        Apple clang version 21.0.0 (clang-2100.4.1.1)` and `ok
runtime      runtime` (`briefs/base/stage2.log`). The durations (seed `real 17.48`, stage 2 `real
160.32` against `user 121.83`, `sys 27.08`) were taken at load averages of 46 to 150 (`uptime` at 22:31
and 22:34: *8.44 46.21 91.48*, *150.82 145.40 127.37*) and are not measurements. **A seat whose stage
2 prints another `sources` line is not on the base and says so first.** No duration a seat writes is a
measurement while other sessions run (CL-025): count instructions (`/usr/bin/time -l`, *instructions
retired*), or say why a clock was still. **The copy has no `.git`**: the `records` suite cannot run
there (`.claude/rules/platforms.md` § Measuring in a copy), and a `file://` index is admitted *"only for
a path inside the program's own repository root"* (panel 235's R9, C4): the coordinator's `fetch` runs
above, inside `.claude/worktrees/p250-scratch/` under the repository's root, read the index; a seat
whose fetch is refused for its address says what refused it.

## Where each seat works, and the rules every seat obeys

Every seat's folder is **`.claude/worktrees/p250-scratch/250-<seat>/`**, ignored by git (`git
check-ignore -v .claude/worktrees/p250-scratch/briefs`: `.gitignore:34:.claude/worktrees/`), never the session scratchpad, never `/tmp`: CLAUDE.md § Hard
stops writes no file outside the repository's root. Read-only to the seats, copied into your own folder
before use: `.claude/worktrees/p250-scratch/briefs/` (the coordinator's runs: `reach/closure.sh`,
`reach/programs.sh`, `reach/sets.sh` and their outputs; `partial/`; `base/`), the lanes' worktrees under
`.claude/worktrees/` (`b37-core`, `b37-pm` and the rest), and every other sitting's scratch.

- **Your own folder, and nothing else**: the tree in `tree/`, every probe beside it, `TMPDIR` set to
  its `tmp/` before any build or tool. Never the trunk's `./heroes`, a compiler of
  `.claude/worktrees/p250`, of `p250-scratch/briefs/`, of a lane or of another seat; never read, build or
  run inside another seat's folder.
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
- **One binary name per seat**: every binary you build is named with your prefix, **`ce-`** for the
  compiler-engineer, **`fp-`** for the ffi-pragmatist, **`sw-`** for the spec-warden.
- **A program that may not end** runs as a BINARY under `timeout -s KILL <seconds>`, never `timeout
  heroes run` and never a plain `timeout`; its output to `/dev/null` or through `head -c 65536` into a
  file; afterwards `pgrep -f <your folder>` read and anything left stopped before a verdict
  (`.claude/rules/verification.md` § A run that may not end). `timeout` here is GNU coreutils 9.12 at
  `/opt/homebrew/bin/timeout` (`command -v timeout`, `timeout --version`). The bounds: a compile, `check`, `fetch` or `add`,
  `timeout -s KILL 300`; a built program, `timeout -s KILL 60`; `examples/server`'s binary, if a seat runs
  it, `timeout -s KILL 60` and each client `curl --max-time 5`, the client side under `timeout -s KILL
  90`; a Docker run, below; every output through `head -c 65536`.
- **One heavy command at a time** (a stage 2 build, a suite): other sessions build on this Mac, and the
  load average read 150 at 22:34. Whole suites only where your route changes what the checker refuses,
  named in your report with why; otherwise a form narrowed to your cases with the harness's third word
  (`./heroes-next run tests/harness/main.hero -- ./heroes-next <form> <case-substring>`), the `fetch` and
  `packages` suites by name, one at a time.

## The platforms

- **Darwin arm64**: this Mac, Apple clang 21.0.0 (above). `pmset -g assertions` read
  `PreventUserIdleSystemSleep 1` at 22:31, so something already holds the machine awake
  (`.claude/rules/verification.md` § A long run holds the machine awake): ask it again before a long run,
  `caffeinate -i` only where it reads 0.
- **Linux arm64**: the image `heroes-linux-arm64:latest`, `338a1044ac90` (`docker images
  heroes-linux-arm64`, which also lists `:sdl3-b14`, `75742c2bf90b`). Write a script into a folder of
  yours and run it from the repository's root:

  ```
  git -C .claude/worktrees/p250 archive 722ae76bf693bebdb7a5cca37052ed92646753c0 | timeout -s KILL 900 docker run -i --rm --name <prefix>-250-<n> -v <your folder>/linux:/out heroes-linux-arm64:latest bash /out/run.sh
  docker ps --filter name=<prefix>-250-<n> --format '{{.Names}} {{.Status}}'   # after it, whatever its exit
  docker stop -t 1 <prefix>-250-<n>                                            # where that line printed a container
  ```

  the script reading the tree from its stdin into a folder of the container's own, building the seed
  and stage 2 there, every probe inside it under its own `timeout -s KILL`, its log under `/out`, every
  output bounded with `head -c`. The outer `timeout` bounds the client, not the container (panel 248's
  critic, carried: `docker ps` still listed a container the outer `timeout` had cut), so read `docker
  ps` after every run. One container of yours at a time. Unrun tonight by the coordinator: stage 2's
  `sources` line in that image.
- **Windows: the box is OFF.** `ssh -o BatchMode=yes -o ConnectTimeout=15 win 'echo up'` at 22:36
  printed *"ssh: connect to host apponfly-vps port 22: Operation timed out"*, exit 255
  (`briefs/win-check.txt`). **A seat that needs a Windows measurement names it in its report as owed**,
  and **the CI's Windows leg judges after the push** (`.claude/rules/verification.md` § The optimistic
  chain, its item 4).

## Questions no one has run, posed as questions

- Whether the closure the compiler compiles equals the closure by `use` lines for every program here,
  modules declaring only records included (two programs asked, above).
- Which files a reached group's C build reads, per platform, and whether clang's `-M` listing on one
  platform names what another platform's build needs.
- Whether a fetched package's `link "x"` group can find an archive the package carries at all: the
  coordinator read `selfhost/cli/package_units.hero`'s comments, whose message at its line 298 names
  headers alone (*"A header it needs ships inside the package, or is a library's that a `pkgconfig`
  group names"*), and searched no further.
- What `heroes fetch` should start its closure from, a folder holding several roots.
- Whether any emitted C, any `#line`, any provenance key, or any `emission` golden moves when the same
  modules are reached from a pruned tree.
- What a build costs, in instructions, hashing a 26-file tree against hashing a listing and the files
  reached, on `examples/ini` and on the compiler carrying `core`.
- Whether the compiler's tree, placed files counted, shrinks under any placement route over `core`
  0.4.0, and by which deletions.
- What Go (`go mod vendor`, `vendor/modules.txt`, `go.sum`'s `h1`), Cargo (`cargo vendor`,
  `.cargo-checksum.json`), Deno (its vendor folder, its lock, JSR's per-file serving), npm, pip and uv
  each place or vendor, and what each one's lock or sum covers: the historian's, sourced and dated.
- Whether a listing placed inside the tree, or any record of which modules were placed, is a fifth
  input class under `.claude/rules/cli-surface.md`'s last paragraph.
- Whether 2312's goldens should land before or after this sitting's ruling.
