# Panel 235: dependencies named by an alias in angle brackets that the program's catalogue resolves, Go's modules copied whole, a TOML catalogue listing every package, and no fetched code run at build

2026-11-07, the opening sitting of M-package-manager (row 77), which opened that day beside
M-core-packages at `b32ac8632`. **Full panel**: the compiler-engineer; the ffi-pragmatist; the
historian, who wrote a survey of thirteen package systems and seven indexes before the sitting
(`235-reports/historian-survey.md`, read by the author first) and then its report; the
spec-warden; and the llm-ergonomist's seat run blind, as one fresh `claude -p` session outside
the project's context. Beside them the coordinator's reader test, 241 `claude -p` sessions. A
completeness critic read the survey's brief before the survey ran (27 items,
`235-reports/critic-survey-brief.md`), the briefs before any seat started (22 items,
`235-reports/critic-briefs.md`, every one repaired before the first seat, `ca8e2dee0`) and the
reports after (13 findings and 9 things the synthesis must not adopt without a further
measurement, `235-reports/critic-reports.md`). **Status:
`provisional — author ratification pending`**, queued as a `decision` issue drafted at
`.claude/worktrees/235-coordinator/ratify/decision-235.md`, whose address is written here when it
is filed.

Convened by the author's instruction of 2026-11-06, meant as *call a sitting for these choices*,
the once-per-milestone yes of CLAUDE.md § 4. The author's position in seven points, the criteria,
the answers of 2026-11-06 (a survey first, read by the author; paid runs up to 25 USD in all), the
word on CLAUDE.md § 2 (meant as *if it is needed, let us relax this rule*) and the position after
reading the survey (meant as *the sitting decides between Go and Cargo; the sitting decides the
catalogue's format, YAML, TOML or another; I want the most modern and current solution, the one
that works best for language models*) stand on the milestone's page,
`docs/roadmap/milestones/M-package-manager.md`, from *Opened 2026-11-07*. Positions 3 and 7, the
alias in angle brackets and *the bracket is the whole difference between mine and fetched*, were
the author's call and not the sitting's to rule (the critic's first pass, item 1); R1 records them
`retro-record`, with the two controls the reader test ran beside them. The briefs are
`235-briefs/` (committed before the critic at `95a995bf7`, repaired on it at `ca8e2dee0`, the
reader test's materials pinned by `blind/manifest.sha256` and dry-run at `328524be8`, 118
hand-made answers graded as expected and 0 problems), the reports `235-reports/`, the evidence to
archive listed at `.claude/worktrees/235-coordinator/ratify/evidence-list.txt` (not yet packed).
The seats worked in worktrees of `235-base`, `3b071e4f1`, a merge never pushed whose parents are
`328524be8` (this lane, the briefs as repaired) and `9791d4787` (`core-b1`, the first batch of
the core packages, so the tree held the programs that carry `core/`), **its tree
`f4dbb2b8db0c072de8a01cc6558307af4a4b89e1`**, recorded here because the commit dies with its
worktree (`git -C .claude/worktrees/235-base log -1 --format='%H %P %T'`, run at the synthesis).
Each seat built its compiler from the seed there, `heroes 0.6.0`, `sources 1a1e340778db789a`.

The seats found three defects running the shapes beside the proposal, each filed that day:
**2324**, the ffi-pragmatist's (a folder named by `--include` is searched before the runtime's
headers, so a `heroes_standard.h` placed there switches off the overflow abort in the program's
own code), **2325**, the spec-warden's (`use ./json/parse` and `use .heroes/json/parse` are told
the path climbs, and neither climbs), and **2327**, the compiler-engineer's (a module named
`_0opt` spells C names inside the emitter's own namespace). 2324 and 2325 are `blocking`, 2327
`adjacent`. Beside the sitting, batch 23's gate found **2323** (`records/tagged` judged a tag by
a class given after it), repaired and closed in that batch at `03ed8115f`, and its lane found
**2326** (`adjacent`).

## The proposal, as the seats received it

`235-briefs/proposal.md`, whole:

> Heroes gets its dependency system in M-package-manager, decided and built before the core
> packages. A program names a fetched dependency in a `use` line by a name between angle brackets
> its catalogue resolves, `use <json>/parse`; a `use` with no brackets stays a module of the
> program's own tree, as today (the author's positions 3 and 7, which the sitting does not rule).
> The catalogue gives each entry its source (the Heroes index by name and version, git by address
> and tag, or a local folder while a package is developed), an exact version and a fingerprint.
> The semantics copy one widely used system whole, Go's modules or Cargo, in its newest release,
> without workspaces, with Heroes' own names. The sitting chooses the base, the spelling's grammar
> and its mistakes' messages, the catalogue's file and format, where fetched code lives, how
> versions are selected, how the first version fetches over C, and the verbs; it rewrites the
> rules this touches; and it measures every choice against the author's criteria: the most solid,
> robust and simple, the most modern, the best for language models.

The frames no seat argued past (`235-briefs/00-shared.md` § Frames no seat argues past): the
author's rulings are positions, and a control that beats the author's form by the reader test's
registered rule goes to the author as a measured case, never a resolution the sitting adopts; the
author's criterion is measured, never assumed, and a seat's claim about what models write is a
question unless the reader test measured it; robustness ranks above elegance, cost and speed, and
a fetched dependency is code the program trusts; design.md §1.11 stands, a package being code a
program carries (panel 234's R1); CLAUDE.md § 2 forbids a declared version or a mode before
`1.0.0`, and the author's word relaxes it where the sitting measures the need; and no paid run
but the ones named, all within the author's 25 USD.

## What was measured

Every figure below is a seat's or the critic's run, named where it stands in
`docs/panel/235-reports/`, or the coordinator's ledger. **The machine was not still**: the seats
and a batch gate ran on it together (`uptime` read load averages from 2.66 to 13.91, the
compiler-engineer's report), so its costs are instructions retired, which five repeated `check`s
of one compiler spread by 0.05%, and no duration is a measurement.

- **The author's spelling in the compiler** (the compiler-engineer, § 2, § 3 and § The
  prototype's cost). A prototype of `use <json>/parse`, a catalogue `heroes.toml` beside the
  compiled file, a tree placed at `heroes.packages/<entry>/`, a package's own `use` lines read
  from its root and `internal/` enforced, built in two stages from the seed's compiler, passes the
  compiler's own tests (*1797 tests, all passed*), builds and runs its demo (`IV` and `25`) and
  the `ini` example with `core` as a catalogue entry (25 tests passed, output equal to
  `main.expected`). **475 lines** in the layout unit (394 less comments): parser 103, discovery
  and the module graph 371, resolver 1; checker, lowering, descriptors, ownership, emitter and
  runtime 0. `selfhost/parse/` then reads **8,839 against its budget of 8,737**
  (`layout/budget`, narrowed). The controls cost the parser nothing; under U-bare discovery asks
  the catalogue for every `use`'s first part, and the compilation must carry the entries, which
  `source.Source` has no field for. The formatter prints `use <roman>/numeral` back unchanged.
- **The C names** (the compiler-engineer, § 3). Today's filter makes `<roman>/numeral` and the
  program's own `roman/numeral` one component, so a correct program was refused
  `module_names_collide`; the prototype leads a package's component with `1`
  (`h_1romannumeral_to_roman` beside `h_romannumeral_to_roman`, the program then running). And
  the premise that makes `1` safe is false one level down: a module `_0opt.hero` emits
  `h_0opt_five` inside the `h_0…` namespace the emitter keeps for its own names (defect 2327).
- **The mistakes** (the compiler-engineer, § 2's table, every one exit 1 on the prototype). The
  whole path inside the brackets, the brackets forgotten where the entry exists and no own module
  does, and a space inside the mark each get a `certain` fix; a bracket added to the program's
  own module gets a `guess`; four are told badly and owed (`use <roman>.numeral`,
  `use <"roman">/numeral`, `use <github.com/acme/roman>/numeral`, and `use
  github.com/acme/roman/numeral`, told badly today and under every spelling). **A fix crossed the
  mark and was wrong**: with the catalogue deleted beside a program holding its own
  `roman/numeral.hero`, the first prototype printed *fix (certain): write `use roman/numeral`*,
  which would have compiled the program's module in the package's place; repaired to a `guess`.
  **A mistake in the catalogue has no machine-applicable fix**, a `Fix` carrying a span into the
  compilation's text and the catalogue being no file of it (`selfhost/diag.hero:47-51`). **And a
  catalogue that does not read must name nothing**: the first prototype compiled the demo at exit
  0 with the entries read before a refused line, then was repaired, with a test.
- **The formats' readers** (the compiler-engineer, § 4 and § 20): five readers, each written to
  its reader-test section and each refusing its own trap, 12 tests passed. In the layout unit,
  then less comments: the line format **60 / 54**, 2 productions; Heroes' own syntax **59 / 53**,
  2 productions over the compiler's lexer; JSON **54 / 49**, reusing the RFC 8259 reader the
  compiler already carries (`selfhost/mutate/json_read.hero`, 278 / 231), 3 productions over
  JSON's; a TOML subset **100 / 98**, 3 productions; a YAML subset **188 / 173**, 4 productions
  and a table of the plain scalars YAML 1.2.2's core schema and YAML 1.1 type. None runs code.
  The critic ran the prototype's TOML reader over the 60 angle-TOML first turns of the reader
  test: each built and printed the expected output (critic-reports item 2).
- **Selection** (the compiler-engineer, § 0). Go's own `cmd/go/internal/mvs` reads 521 code lines
  in Go 1.27.3's installed sources; minimal version selection in Heroes, written for the sitting,
  **63 lines in the layout unit, 54 less comments**, 2 tests passed, outside the compiler. Cargo's
  resolver was not on this machine, so its size in any language is unrun. Under MVS the selected
  versions are a function of the catalogues alone and no lock file adds anything; under Cargo the
  lock is what makes a build reproducible.
- **The fingerprint** (the ffi-pragmatist, § 4; the compiler-engineer, § 5). Go's `h1`,
  recomputed from Go's own data over `github.com/google/uuid` v1.6.0 (31 files), equals Go's
  recorded `ziphash`; a git clone of the same tag gives the same `h1` from its working tree, its
  `git archive` and its blobs. One commit's archive bytes, made by five producers, gave four
  different SHA-256s. A clone with `core.autocrlf=true` moves the working tree's `h1` and `git
  archive`'s and leaves the blobs' where it was; a repository's own `.gitattributes` saying `*
  text eol=crlf` moves the working tree and `git archive` the same way. The
  compiler's `sha256.of` matches `shasum` on text and refuses a PNG, `not_text`; it costs about
  **1,603 instructions a byte** at `-O0` (the compiler-engineer) and `real 1.30` over the
  compiler's 8,259,507 bytes against `shasum`'s 0.05 (the ffi-pragmatist). Reading bytes is
  already done (`hero_file_bytes`, `hero_file_key`); a SHA-256 over them is what is missing.
- **Where fetched code can stand** (the spec-warden, § 6; the compiler-engineer, § 7; the critic,
  item 3). Any folder of the program's tree whose path is a run of identifiers is a module a
  bracket-less `use` reaches: `use json/parse`, `use build/parse` and `use core/strings/cut` each
  compile at exit 0 over a folder of that name. A path holding `-`, `@` or opening with `.` is
  one no `use` can spell, and so is the prototype's `heroes.packages/`. On the prototype,
  deleting an entry with its copy left, or deleting the catalogue, gives `unknown_entry`; but
  `path = "../decoy/core"`, a folder with the same module names and one function changed, builds
  at exit 0 and prints `attacker.example`, and a mistyped `sum`, a mistyped `version` and an
  edited placed tree each pass `check` at exit 0, the prototype checking no fingerprint.
- **A fetched package that binds C** (the ffi-pragmatist, § 1 and § 2). A package's header beside
  its module is refused at build, `ffi_missing_header`, and builds only with `--include` naming
  the package's folder. Every unit's search line is the `--include` folders, a `package`'s `-I`,
  the runtime's, then the compiled file's folder, one line for every unit. **A
  `heroes_standard.h` in a folder ahead of the runtime's switched off the overflow abort in the
  consumer's own `main`**: `-9223372036854775808` at exit 0 against `panic: integer overflow` at
  exit 134 (defect 2324). Scoped by hand, each unit searching the runtime first and then its own
  package's root, the program built and ran, and a planted header reached only its own package's
  unit (`clang -M`). Six hostile `link` values were each refused at exit 1.
- **What fetching or building runs** (the ffi-pragmatist, § 2's table). `git clone` ran no hook
  of the source repository; **a URL handed to `git` without `--` ran `--upload-pack`** (the
  marker written), and after `--` git refused it; `ext::` is refused by git 2.56.0's default; a
  committed symlink came through the clone pointing outside the package, and `clang -M` names the
  link, not its target; two names that differ only in case left one file on this Mac's disk, git
  warning that they collided; `#embed` in a header copied a file of the build machine into the
  binary. **A header that binds a `constant` and carries a constructor ran its C during a cold
  `build`, `--emit-c` and `test`** (two files written, the program never run), and not warm: panel
  216's R1, *`heroes build` may run a program's header code to judge its constants*, which the
  author granted for **the program's own** headers. The critic reproduced it on the prototype
  (`RAN-AT-BUILD.txt`, item 4). **The route that runs nothing** (the critic, item 11): `enum : long
  long { v = (PKG_LIMIT) }` under `clang -std=c2x -fsyntax-only -Xclang -ast-dump=json` read
  1099511627779 for `((1LL << 40) + 3)` and the header's constructor wrote nothing, on Apple clang
  21 alone.
- **The C under the first version** (the ffi-pragmatist, § 3). `/opt/homebrew/bin/git` 2.56.0
  and the developer tools' `git` 2.54.0; `/usr/bin/curl` 8.7.1; libcurl in the SDK, a 30-line
  fetch built against it; **libgit2 absent** (`git2.h` not found, `pkg-config` not finding it).
  `ci.yml:359` installs no `git` or `curl` package on Linux, and `ci.yml:495` says curl is not on
  the Windows leg.
- **A static index read by `curl`** (the ffi-pragmatist, § 4 and § 5). Per package a `versions`
  file, per version a listing of every file's SHA-256, each file at its own address: `curl -fsS
  --proto '=https,file'` fetched the 31 files, checked them against the listing and the listing
  against the catalogue's `h1`, and the tree equals the git clone's byte for byte. One byte added
  on the index was refused, **after 25 files had been written**, so a fetch fills a staging folder
  and moves it in whole; a listing naming `../../../escaped.txt` was refused before writing. A tag
  moved by `git tag -f` gave another commit and another `h1`: a catalogue holding the fingerprint
  refuses it, one holding the tag alone builds a different program in silence.
- **The base's build reaches the network** (the ffi-pragmatist, § 6): Go's `go build` with
  `go.sum` holding the module's line, an empty module cache and `GOPROXY=off` printed `go:
  downloading github.com/google/uuid v1.6.0`, then refused at exit 1.
- **The specification** (the spec-warden, § 3, every count by `heroes measure --refresh` on its
  worktree): 10,514 real today; the arms' sections **+662 to +723 real**, the largest leaving 11,297
  with the FFI floor against the ceiling of 16,384; the spec-warden's `merged-min`, § 1 edited in
  place with no new section, **+136 real**. Tokens cannot choose between the arms: the F arms
  span 33 real tokens and the U arms 53.
- **What a reader must learn** (the spec-warden, § 8; the compiler-engineer, § 20). 11 concepts
  shared by every arm; U-angle and F-toml **14** in all, U-bare, U-pkg and F-lines 15, F-yaml 14
  and 1 unstated (a plain scalar's type), F-json and F-hero 14. Files a program must have:
  **none** with no dependency, one, the catalogue, with dependencies, plus the placed tree the
  verb writes; Go brings two with a dependency and requires `go.mod`'s `module` line, Cargo two
  and requires `[package]`'s `name`. One word collides: every arm's *package* beside spec § 13's
  `package "raylib"`, which the spec-warden counts a sixteenth concept.
- **CLAUDE.md § 2** (the spec-warden, § 5, read from the survey's Table 1): `go`, `godebug`,
  `edition`, `resolver` and `cargo-features` change what a program means, so each is a mode;
  `toolchain`, `rust-version` and `Cargo.lock`'s `version` do not.
- **The latency** (the compiler-engineer, § The latency, instructions retired, the two compilers
  alternated, the first pair discarded). `check selfhost/main.hero`, a build with no catalogue:
  **+0.095%**, 5 of 5 pairs slower, under the floor of effect of 0.5% and a breach of *never slow
  the compiler down* by its own word, owed a sentence in the landing commit. A warm build: 2 of 4,
  no difference resolved. A build with a catalogue against the same modules as the program's own:
  median +0.69%, 4 of 5.
- **Precedent** (the historian's report and survey, every page read 2026-11-07, nothing run). A
  mark on the import line added late has a page stating the collision or confusion it was added
  for (Rust
  2018, Nim 0.18.0, Node's `node:`, Deno's `npm:` and `jsr:`); the one design found marking own
  against external on every line was Rust's anchored paths, refused in 2018 (§ 1). Dart compiles
  one file reached by two spellings as two libraries (§ 1). Cargo's alias key was ignored by
  older Cargo, a squatting risk (§ 2). Python chose TOML over JSON, YAML and configparser for
  stated reasons; Go kept a line format of its own after weighing them (§ 3). MVS's costs in Go
  (`retract`, graph pruning) and the solver's in Cargo (untested minimal versions, the lock-file
  guidance) (§ 4). Cargo, CocoaPods and Homebrew moved their index off git to files over HTTP
  (§ 5). No index read began with namespaces or signing, and PyPI removed PGP in 2023 (§ 8). **No
  page measures how models write either base's files** (§ 9).
- **What it cost.** The ledger (`.claude/worktrees/235-coordinator/sealed/ledger.jsonl`): the
  reader test **5.4654 USD over 241 sessions**, the cap probe **0.0005**, the blind seat
  **0.2419**, 5.7079 in all; the spec-warden's `--refresh` made **27 `count_tokens` requests**
  over nine runs, about **0.40 USD** by its reckoning at the list price, no bill read (its
  § 10). About 6.11 USD of the author's 25.

## The reader test, and what it can carry

`235-briefs/blind/README.md`: eight arms (U-angle, U-bare, U-pkg for the spelling; F-yaml, F-toml,
F-json, F-lines, F-hero for the format, each reading U-angle's spelling; U-angle and F-toml the
same section run twice, an A/A pair), three tasks, `claude-haiku-5-5` and `claude-sonnet-5-5`,
five samples a cell: **240 first turns**, drawn by `random.Random(235).shuffle` at 17:34:01, after
the spec-warden's predictions were sealed at 17:33:57 (`sealed/predictions.sha256`, its hash
matching its report's § 1). **236 right at the first turn**; one first turn was refused with
messages (F-yaml, T2, Sonnet, the compiler's `cannot_infer`) and its one second turn passed, so
237 after it; the other three misses `silent`, all Haiku, each writing `print(a, b, "\n")`. 0
void, 0 capped, 0 grader errors (`sealed/grades.jsonl`, counted at the synthesis).

| arm | pass@1 | pass@2 |
|---|---|---|
| U-angle | 30/30 | 30/30 |
| U-bare | 30/30 | 30/30 |
| U-pkg | 29/30 | 29/30 |
| F-yaml | 28/30 | 29/30 |
| F-toml | 30/30 | 30/30 |
| F-json | 30/30 | 30/30 |
| F-lines | 29/30 | 29/30 |
| F-hero | 30/30 | 30/30 |

The registered rule, as `score` printed it: the A/A pair's four p-values all 1.0000, so the
comparisons are read; **no control above U-angle, *the author's form stands as measured***; every
format left tied, sent to the sitting's other measures. The habit count read 0 in every row. The
cap probe held: at most 64 output tokens a call over five calls.

**Its ceiling, as the critic states it** (critic-reports item 1): *the test measured copying an
example, not the forms.* None of the 240 first turns failed on its catalogue or its `use` lines;
the four misses are program faults. Every answer copied its section's example: 120 of 120 TOML
catalogues use `[dependencies.<name>]` tables, 31 of 31 YAML catalogues indent 2 and 4 spaces, 30
of 30 F-hero catalogues indent 4; the `use` lines are 242 `use <x>/x`, 120 `x/x` and 40 `pkg/x/x`,
not one with the whole path inside the brackets. YAML's `1.10` trap fired in 0 of 10 sessions:
all ten wrote `"1.10"` quoted, and 8 left `v1.2.0` plain. One repair turn ran in all, so no arm's
messages were tested. **So the test measured that a model copies a given example; it did not
separate the forms**, and nothing below rests on it but the author's form standing as measured.
What it was built never to measure (its README, § What the test cannot measure) stands too: a
tool writing the catalogue, ranges and lock files, a dependency's own dependencies, a local folder,
the fingerprint's work, and a real implementation's messages.

**The blind seat** (`235-reports/llm-ergonomist.md`): `claude-opus-5-5`, 0.2419 USD, 8 tool
calls, every one a `Read` inside its folder, so the void check passed; its `context` answer names
the harness's operating instructions and nothing of the project. Its variants, by
`blind/seat/variants.tsv`: variant-1 U-pkg, variant-2 U-angle, variant-3 U-bare. It approved
U-pkg and U-angle as local, and **vetoed U-bare** on locality: *whether `use x/y` reaches a
package or the program's own folder depends on `heroes.toml`, a file the line neither names nor
marks*. Its verdict line writes variant-3's spelling as `use <entry>/<module>`, which is U-angle's;
the veto is read by `variants.tsv`, as U-bare's (critic-reports item 10). Its registered
condition: *lower it to object if variant 3 matches or beats both other variants, and no measured
failure comes from the collision rule or a misresolved `use`*. U-bare read 30 of 30 against 30
and 29, and its T3, built to make the collision rule fire, 10 of 10: **the veto stands lowered to
an objection by its own condition** (critic-reports item 2 and *must not adopt* item 8).

## The verdict table

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object**: the proposal leaves unsaid whose catalogue names a dependency's `<x>`, what a half-read catalogue means, which C names a package gets and whether a `certain` fix may cross the mark, each measured with a stated repair; a **veto** on two routes, should the resolution take either: `build`, `check` or `run` fetching, and a fetched tree reachable by a `use` the program's own modules also spell; Go's base from the compiler's side | design.md §1.1, §1.7 and Part 5, §1.12 with CLAUDE.md § Precedence rank 3, §3.5 | the prototype 475 lines (394 less comments): parser 103, discovery and module graph 371, resolver 1, every later pass and the runtime 0; `selfhost/parse/` 8,839 of 8,737; `check selfhost/main.hero` +0.095%; MVS 54 lines outside the compiler; the verbs, the fingerprint door and the alias rule unbuilt | the first version's landing changes 0 lines under `selfhost/check/` and `selfhost/ir/`, and under U-angle adds 95 to 165 lines to `selfhost/parse/` | approve on: the program's catalogue as the build list, one package compiled once; a catalogue read all or nothing; a package's C component no program module can have, no program component led by a digit; a fix crossing the mark `certain` only where nothing answers on the other side; the fingerprint over bytes in C, checked at every build; the `parse/` row raised in writing |
| ffi-pragmatist | **object** until the resolution carries C1 to C5; **veto** held on one route: a fetched package's root on the program-wide search path ahead of the runtime's | design.md §1.11, §1.12, §4.19; build-time execution covered by panel 216 alone, its `grep` finding no line of design.md, the spec or the rules on it | a per-unit header search; a `-M` containment check over a listing the compiler already makes; fetch-time refusals; SHA-256 over bytes in C in every runtime, size unmeasured; no new C for the transport | (P1) under C1, a fetched copy of `examples/ledger/db/sqlite.hero` and a package shipping its own header build with no `--include`, and a planted `heroes_standard.h` appears in no unit's `deps.txt`; (P2) under C5 as Go's goal, a fetched constructor binding a `constant` writes nothing at a cold build | C1 to C5 adopted makes it approve; the author extending panel 216's yes to fetched headers meets C5 as Cargo's stance; the veto lifts only if the runtime's headers come first on every unit's line, measured |
| spec-warden | **object**, no veto; tokens measured on the binding instrument | design.md §1.6, §1.2, §1.0, §3.5 | arms +662 to +723 real, the largest 11,297 of 16,384 with the floor; `merged-min` +136; 27 `count_tokens` requests, about 0.40 USD | the reader test's 32 cells (scored here: falsified); W1 at the landing commit; W3 and W4 at the close | approve on: fetched code out of every bare `use`'s reach, R2's `core/` included; the landed text naming the verb, priced within W1; § 2 unchanged or only its refusal-only sentence; **veto** on Principle 0 if `pkg`'s reservation, a catalogue-first lookup or a catalogue grammar of Heroes' own enters with no measured lead, and on the budget only past 16,384 real |
| historian | **approve** (advisory, no veto) | design.md §1.11, §1.12, §3.5 | none of its own; prior-art lines, its § 10 | (One) if one module of fetched code can be reached with and without brackets, it compiles as two modules, Dart's recorded failure; (Two) under a solver with exact pins the first two packages of M-core-packages' resumed batch that pin different versions of one dependency fail to resolve, under MVS they meet at the higher pin | a page recording a language that required a mark on fetched imports and dropped it for a measured cost; a page recording MVS failing what a solver resolved under exact pins and one version per program; a measurement that models write Cargo's files right more often than Go's |
| llm-ergonomist (blind seat) | **approve** U-angle and U-pkg, local; **veto** on U-bare as non-local, **lowered to an objection by its own registered condition** | the specification alone | one session, 0.2419 USD | first turns of 30: U-pkg 25, U-angle 23, U-bare 15 (scored here) | U-pkg or U-angle to object if 5 or more of 30 below U-bare, or if its most frequent mistake builds and runs wrong; U-bare's veto to object if it matches or beats both and no failure comes from the collision rule (it fired) |
| reader test (the coordinator's measurement, no verdict) | the author's form stands as measured; every format tied | design.md Part 11 metrics 2 and 4 | 241 sessions, 5.4654 USD of the 19.00 registered | the spec-warden's cells and the blind seat's, both scored here | a task that separates the forms: an entry written without its sum, a dependency with dependencies, two pins of one package, a catalogue to edit, a reading question (critic-reports item 13) |

## Predictions to score

| seat | prediction | checked at |
|---|---|---|
| compiler-engineer | the commits that land the first version change 0 lines under `selfhost/check/` and `selfhost/ir/` (`git diff --stat`), and under U-angle add 95 to 165 lines to `selfhost/parse/` in `code_lines`' unit | M-package-manager's tag |
| ffi-pragmatist | (P1) under C1 a fetched copy of `examples/ledger/db/sqlite.hero` and a fetched package with a header of its own build with no `--include`, and a `heroes_standard.h` planted at the package's root appears in no unit's `deps.txt` | M-package-manager's first version, on the four CI legs |
| ffi-pragmatist | (P2) under C5 read as Go's goal, a fetched header with a constructor that binds a `constant` writes nothing during a cold `heroes build` | the same |
| spec-warden | the reader test's 32 cells, scored by its own rule: **falsified**. Level: a mean absolute error of **3.97** against the 2.5 allowed (127 over 32 cells). Calibration: **15 of 32** inside its intervals against the 24 required, every miss above its ceiling. Structure: 5 of 6 claims held (Sonnet at least Haiku at pass@1, the A/A pair unflagged, the author's form standing, F-yaml lowest of the F arms, no F pair significant); S2 failed (Haiku gained 0 from pass@1 to pass@2, Sonnet 1). Its two registered mechanisms both failed: the YAML trap fired in 0 of 10 sessions, and U-bare's T3 lost nothing, 10 of 10; and the structure that held is mostly the ceiling's (its § 12) | scored here |
| spec-warden | W1: the landing commit adds at most +250 real if it carries the `use` form, the verb, the dependency's own root and one version per program without a format example, at most **+800 real** with one (R13 keeps the example, so +800 binds) | the landing commit, by `--refresh` |
| spec-warden | W3: each listed mistake exits 1 with a `certain` fix and none compiles. **Falsified as registered** by the rule R18 keeps (critic-reports item 5: `use <geom>/area` draws a `guess`, and a domain in a `use` no fix); restated in R18 | M-package-manager's close |
| spec-warden | W4, **as registered** (kept): a case for the catalogue deleted, an entry deleted, each field of an entry mistyped, and a fetched copy left after its entry is deleted; each exits 1, none compiles. As registered it is falsified by design for a mistyped `path`, which R6 builds and tells instead of refusing (the critic measured a decoy `path` compiling, item 3). **Restated 2026-11-07** (the coordinator's ruling): a case for the catalogue deleted, an entry deleted, each of `sum`, `version`, `tag` and `commit` mistyped, the placed tree edited, and a fetched copy left after its entry is deleted, each exiting 1 with nothing compiled; and a case for a mistyped `path`, **the one exemption**, which builds and says on stderr that the folder it names is unverified (R6) | M-package-manager's close; R7 counts question 17 met only when the restated cases pass |
| historian | (One) if one module of fetched code can be reached both with brackets and by a path without them, the first version compiles it as two modules | M-package-manager's goldens, a case naming one file both ways |
| historian | (Two) under MVS, the first two packages of M-core-packages' resumed batch that pin different versions of one dependency meet at the higher pin, where a solver with exact pins would fail to resolve them | M-core-packages' resumption |
| blind seat | first turns of 30: U-pkg 25, U-angle 23, U-bare 15. **Measured 29, 30 and 30**: every prediction low, and the order wrong, U-bare tying the author's form and above U-pkg | scored here |

## Disagreements, stated plainly

**U-bare, kept out.** The blind seat vetoed it on locality and its own condition lowered the
veto; the compiler-engineer's veto condition is any placement where a bare `use` reaches fetched
code; the spec-warden's is a catalogue-first lookup with no measured lead. The first draft of R1
kept U-bare out on robustness, not on locality. **Not smoothed**: question 17's failure under U-bare is measured
where fetched code stands where a bare `use` reaches it, as the grader placed it at `json/` and as
panel 234's R2 places `core/`; the spec-warden finds that U-bare meets question 17 where fetched
code sits outside the tree (its § 6). With R7's placement, then, U-bare's robustness case narrows,
and what also keeps it out is the spec-warden's Principle 0 condition (no lead at 30, 30 and 29),
the author's position 7, and its cost in the compiler, a lookup in the catalogue for every `use`'s
first part (the compiler-engineer's § 2 table). **Ruled by the coordinator, 2026-11-07**, on this
paragraph: R1 now states U-bare out on Principle 0 (a new rule with no measured lead), on the
author's position 7, and on the blind seat's locality objection; the robustness argument holds
wherever fetched code could sit inside the program's tree, and narrows under R7's placement, as
the spec-warden finds. The first draft's *out on robustness, not on locality* stands above as what
was ruled on.

**Alias scope and what a placed package is keyed by.** The compiler-engineer recommends reading a
dependency's own `use <x>/…` through the dependency's own catalogue and mapping it by source to
the program's entry, so two catalogues giving one alias to two packages is no conflict and an
index package may be aliased (its § 16); its prototype instead reads `<x>` through the program's
catalogue by text, which it calls right only where every catalogue uses the program's aliases.
The ffi-pragmatist keys C1's root by the package, *never the alias* (its § 7), the
compiler-engineer the placed tree and its C names by the program's alias (the critic's fifth
contradiction, item 7, unbuilt). The synthesis's first draft of R3 settled the keying by the
package's identity and took the prototype's reading of `<x>`, `<x>` being the program's entry
wherever it is written, and wrote down its cost: a dependency whose catalogue names a package `x`
would build only where the program gives that package the alias `x`, so two dependencies giving
one alias to two packages could not share a build, and the compiler, reading no dependency's
catalogue, would leave the check that the program's version is at least each dependency's to the
verb. **Ruled by the coordinator, 2026-11-07: R3 takes the compiler-engineer's reading, made
robust** (R3 below): each module's `<x>` is resolved through the catalogue of the package that
holds it, every alias maps to a package identity, and the program's catalogue holds the build list
by identity; so two dependencies may give one alias to two packages, and two aliases to one package
compile once. The keying the critic found in contradiction is settled by identity either way.

**`path` entries against the falsifier.** The compiler-engineer turns the 13 copies of `core/`
into `path` entries (its § 11); the spec-warden's falsifier says no mistyped field changes which
module compiles, and a local folder has no fingerprint (its § 6); the critic measured a decoy
`path` building at exit 0 (item 3). Both seats are right; the author's page lists the local folder
as a source. R6 keeps `path` for the author's own work and names it in the falsifier as the one
exemption, told at every build and refused at release; R15 takes `core` out of `path` entirely.

**The prototype's cost against the boundary's.** The compiler-engineer writes that nothing
downstream can tell a fetched module from the program's own; the ffi-pragmatist's C1, C2 and C5
need the build to tell a fetched unit apart (`cli/compiling.hero`, `cli/value_run.hero`), and the
fingerprint's door is in `runtime/`. So **the 475 lines and *runtime 0* are the prototype's cost,
not the first version's** (critic-reports item 4); the compiler-engineer's scored prediction, 0
lines under `check/` and `ir/`, still stands.

**Fix certainty.** The spec-warden's W3 registered a `certain` fix for a bracket added to the
program's own module and for a domain written in a `use`; the compiler-engineer's rule, which the
critic re-ran on the prototype, gives the first a `guess` and the second no fix (item 5). R18
keeps the compiler-engineer's rule and restates W3.

**The base.** No seat measured Go against Cargo for models: the reader test never varied the base
and the historian found no page that measures it. The spec-warden sized the run that would (two
arms per base copied whole, 60 first turns, about 1.5 USD, 3.1 with a second turn each, its § 9)
and judged it likely to tie at this test's ceiling (its § 12); it was not run. R2 chooses Go on
the compiler's side alone, and says so.

**The format.** Tied in the test. TOML is chosen over YAML on the hazard YAML's own
specifications state (a plain `1.10` read as a number, which the reader test saw fire in 0 of 10
sessions) and a reader of 188 lines, nearly twice TOML's subset; over Heroes' own syntax on the
spec-warden's Principle 0 condition. **Over the line format and JSON it is not chosen on a
robustness measurement**: the line format's reader is smaller (60 lines against about 100) and
reads every word as text, so `1.10` stays `1.10` with no number to be read in its place; JSON's is
the smallest, 54 lines over a reader the compiler already trusts. TOML's
lead over them is comments (over JSON), named keys and one concept fewer (over the line format,
14 against 15), and its being Cargo's and `pyproject.toml`'s format; Go, weighing the same
formats, kept a line format of its own (the historian's § 3). The question that would separate
them, how a catalogue is read, diffed and merged by people and models once a verb writes it, is
the one the sitting should have asked (critic-reports item 13), and is owed. **Ruled by the
coordinator, 2026-11-07**: R5 states the reasons TOML has (comments, named keys, one concept
fewer, familiarity from Cargo and `pyproject.toml`, no implicit typing) and says plainly that they
are not a robustness measurement; the line format and JSON are the conservative alternatives.

**The specification's text.** The spec-warden priced `merged-min`, § 1 edited in place with no
format paragraph, 526 to 587 real tokens under each arm's section; the critic refused to drop the
example on the strength of 236 of 240, every answer having copied it (*must not adopt* item 2).
R13 takes the short draft as the base and keeps the example.

**C5, code a fetched header runs at build.** Panel 216's yes covers the program's own headers; Go's
stated goal is that neither fetching nor building runs fetched code; Cargo runs a package's
`build.rs`. The ffi-pragmatist set the choice before the sitting or the author, never inherited in
silence. R9 takes the critic's dump route, measured on this Mac alone, and refuses the probe for a
fetched header until it is measured on the four legs; the author's extension of panel 216 is the
conservative resolution below.

**The git source's commit.** The ffi-pragmatist recommends that a git entry hold the commit beside
its tag (its § 5); the synthesis's first draft of R6 held the tag and the fingerprint, which
refuses a moved tag by itself (measured, the same section), and recorded no commit. **Ruled by the
coordinator, 2026-11-07: the ffi-pragmatist's recommendation is adopted**, a git entry recording
the commit beside the tag, so a moved tag is refused, not followed (R6).

**The word *package*.** The spec-warden counts *package* beside spec § 13's `package "raylib"` as
a sixteenth concept every arm carries (its § 8), and the historian notes that copying Go's words
whole would give *module* two meanings, spec § 1 calling a file a module (its § 7). Question 1 is
answered here only by use: *package* for the unit with a version, *catalogue* for `heroes.toml`,
*fingerprint* for its `sum`, `add` and `fetch` for the verbs; the collision with § 13 is not ruled.
**Ruled by the coordinator, 2026-11-07**: an open question for the landing, under question 1, the
words.

## The resolution adopted, provisionally

`provisional — author ratification pending`. The most robust and complete resolution on every
item, never the cheapest (CLAUDE.md § 4); where it parts from what a conservative sitting would
adopt, the next section says what that would have been.

**R1, the spelling (`retro-record` of the author's positions 3 and 7): `use <name>/module`
reaches a package's module, and a `use` without brackets is the program's own module, from the
root as today.** Spec § 1's production becomes `Use = "use" [ "<" ident ">" "/" ] ident { "/"
ident } [ "as" ident ] NEWLINE .`; a bracketed `use` resolves through the catalogue alone and a
bare one through the tree alone, with no fallback either way (the spec-warden's first condition,
its § 6). Measured: U-angle **30 of 30**, the controls tied (U-bare 30 of 30, U-pkg 29 of 30), no
control above the author's form, so `score` printed *the author's form stands as measured*.
**U-bare is out on Principle 0, on the author's position 7, and on the blind seat's locality
objection** (the coordinator's ruling, 2026-11-07, Disagreements): a catalogue-first lookup is a
new rule with no measured lead, the spec-warden's veto condition (30 of 30 against 30 and 29);
position 7 makes the bracket the whole difference between mine and fetched; and the blind seat's
veto on locality stands as an objection, lowered by its own registered condition (critic-reports
item 2). **The robustness argument, question 17**, holds wherever fetched code could sit inside
the program's tree, where a bare `use` that reaches it compiles it as the program's own once its
entry or the catalogue is deleted (the compiler-engineer's veto condition; the spec-warden's
placement table); under R7's placement it narrows, as the spec-warden finds (its § 6). **U-pkg is out**:
a reserved first name with no measured lead, the spec-warden's veto condition, at 53 real tokens
more than U-angle. **The price, written down**: the author's spelling is the only one whose cost
lands in `selfhost/parse/`, 103 lines in the prototype, taking the directory to 8,839 against its
budget of 8,737; the landing raises that row in writing, read beside its neighbours (the
compiler-engineer's approval condition).

**R2, the base: Go's modules in their newest release, without workspaces, chosen on the
compiler's side and never *for language models*.** Go 1.17's rule is copied: **the program's
catalogue lists every package in the build, transitive ones included, each at the version
minimal version selection chose and each with its fingerprint**, held by identity (R3), so every
package of the build stands once; exact versions; one version per package per program; **no lock
file**, the catalogue holding each entry's fingerprint. The compiler never touches the network and
solves nothing; at build it checks each entry's shape and each placed tree's fingerprint (R7), and
**`build` refuses a catalogue that does not hold the build list whole** (R3). **The check that the
program's version of a package is at least each dependency's lives in the verb** (the
coordinator's ruling, 2026-11-07; the compiler-engineer had placed it at build, its § 6). MVS runs
in the verb that writes the catalogue (R11): 54 lines of Heroes less comments, written and tested
by the compiler-engineer, kept outside the compiler.
Go's `h1` gives one fingerprint whichever way a package came (R6). *For language models* is
unmeasured: the reader test never varied the base, the historian found no page that measures it
(its § 9), and the critic refused the claim (*must not adopt* item 3). **Cargo's solver is the
recorded alternative** (next section). The bends from Go, each traced to the rule that demands
it: the alias in brackets and no address in code (positions 3 and 7); no line naming the program,
which the catalogue being optional makes unnecessary and R12's wording refuses (the spec-warden's
§ 9 notes that no rule of the author's demands the refusal, only that no such line be required);
the sums inline rather than in a second file (one file, R12); no `go`, `toolchain` or `godebug`
line (R14); Heroes' own names (position 2).

**R3, alias scope: the compiler-engineer's reading, made robust** (the coordinator's ruling,
2026-11-07, on the first draft's program-catalogue-only reading, Disagreements). **Each module's
`<x>` is resolved through the catalogue of the package that holds it**, the program's own for the
program's modules (the compiler-engineer, § 16), and **every alias maps to a package identity**, its
source, version and fingerprint. **The program's catalogue is the build list**, holding every
package of the build by identity (Go 1.17's rule, R2), and a dependency's entry must match one of
the program's by identity, its version the one the verb's minimal version selection chose. So **two
dependencies may give one alias to two different packages**, each alias read in its own catalogue,
and **two aliases to one package compile once**, a placed package being keyed by its identity and
never by an alias; within the program's own catalogue, two entries for one package are refused (the
compiler-engineer, § 16). **The check that the program's version is at least each dependency's
lives in the verb, and `build` refuses a catalogue that does not hold the build list whole.** The
critic's fifth contradiction (item 7, the compiler-engineer's keying by alias against the
ffi-pragmatist's by package) is settled by package identity. Unbuilt: the prototype reads `<x>`
through the program's catalogue by text; matching each dependency's catalogue by source is, by the
compiler-engineer's estimate, about the size of `module/placed.hero` again (34 lines), unrun.

**R4, inside a dependency, its own unbracketed `use` lines start from the dependency's own
root.** Six lines in the prototype (`module/placed.hero`), measured on the demo
(`heroes.packages/roman/numeral.hero`'s `use internal/check` loaded the package's own file) and on
the `ini` example (the canonical tree's `use strings/internal/scan`) (the compiler-engineer, § 3).

**R5, the format: `heroes.toml`, a subset of TOML.** Beside the file compiled; each entry a table
`[dependencies.<name>]`, every value a string between double quotes; every other TOML form refused
with its line (inline tables, dotted keys, literal and multi-line strings, escapes, arrays,
numbers), so `version = 2.1.0` and `tag = 1.10` are refused and never converted; three
productions; **read all or nothing**, a catalogue that does not read naming no entry (the
compiler-engineer's § 2, measured on its first prototype and repaired); reading it runs no code.
Beside its entries the file holds **one `index` line**, the address of the index the verbs read,
the Heroes index when the line is absent (R9's C4, the coordinator's ruling of 2026-11-07); the
compiler reads no index. **Never chosen for language models**: the reader test tied every format
because every answer copied its section's example (critic-reports item 1). **The reasons TOML has,
and they are not a robustness measurement** (the coordinator's ruling, 2026-11-07,
Disagreements): comments are allowed, which JSON's are not (the historian's § 3, PEP 518); its keys
name each field, where the line format's fields go by position; a reader learns one concept fewer
than with the line format (14 against 15, the spec-warden's § 8); it is familiar as the manifest
format of Cargo and of Python's `pyproject.toml` (the historian: PEP 518 chose TOML; Go kept its
own format, golang/go#23966); and it has no implicit typing, an unquoted number being refused and
never converted, against YAML's core-schema trap, whose reader costs 188 lines, most of them that
table. The readers' sizes: the TOML subset about 100 lines, YAML's 188, JSON's 54 reusing the
existing reader, the line format's 60, Heroes' syntax's 59. **The line format and JSON are the
conservative alternatives** (next section). **Heroes' own syntax is out** by the spec-warden's veto
condition: a second grammar with no measured lead. **The format's example stays in the
specification** (R13; *must not adopt* item 2).

**R6, an entry and its fingerprint.** Three sources: the Heroes index, by the entry's name and
`version`; git, by `git`, `tag` and **`commit`**, the commit the tag named when the entry was
written, so **a moved tag is refused, not followed** (the ffi-pragmatist's recommendation, its § 5,
adopted by the coordinator's ruling of 2026-11-07; Disagreements); and a local folder, `path`, the
author's own source while a package is developed. **`sum` is required for the index and git**:
`sha256-` and 64 hex digits over
a canonical listing of the package's files, one line per file of its SHA-256 and its name, sorted
by the name's bytes, Go's `h1` shape (the ffi-pragmatist recomputed Go's own `h1` and matched it,
its § 4); over **the committed blobs, not the working tree**, so `core.autocrlf` cannot move it
(measured, the same section, where a repository's own `.gitattributes` moved the working tree and
`git archive` too, the reason that seat reads the blobs); every regular file under
the package's root, `.hero` or not, since a header reads any file. A file that is not text is read
through **one more runtime door** beside `hero_file_read_shown` and `hero_file_key`, a SHA-256 in C
over its bytes (the compiler-engineer's § 5, the ffi-pragmatist's route (a)), its size in every
program's runtime measured before it lands; the listing, being text, may be hashed by `sha256.of`.
A name that is not UTF-8 or holds a newline is refused at fetch. Whom it trusts, written as such: a
first `add` from git trusts on first use, one from the index trusts the index, and a checksum log
is row 78's (the ffi-pragmatist, § 5). **`path` entries are kept**, because the author listed the
source, and the falsifier names them as its one exemption (critic-reports items 3 and 4): **every
build that uses one says so on stderr, naming the folder as unverified, and the verb that
publishes or records a release refuses a catalogue holding one.** This is a cost written down, not
hidden: a mistyped `path` to a folder of the same module names compiles other code, measured.

**R7, where fetched code lives: inside the program, in a folder no `use` can spell** (the
compiler-engineer's prototype, `heroes.packages/`, a path's part holding no `.`), one tree per
package keyed by its identity (R3); the prototype named each tree by its alias,
`heroes.packages/<entry>/`, so the name below that folder is R19's to fix in a golden, as is the
listing's exact form, with or without Go's `name@version/` prefix, which the ffi-pragmatist left to
the sitting (its § 4). Deleting an entry or the catalogue then refuses the program
rather than compiling something else (question 17; the critic measured both deletions on the
prototype). Every name the compiler prints of a fetched module is relative and the same on every
machine, no user cache folder is read and no lock between two programs is needed (the
compiler-engineer, § 7). **The placed tree is checked against its fingerprint at every build**,
since a tree inside the program is a folder anything can edit; **question 17 counts as met only
when W4's cases pass**: `sum`, `version`, `tag` and `path` mistyped, and the placed tree edited
(critic-reports, *must not adopt* item 5), in W4's restatement of 2026-11-07, which adds `commit`
and names `path` as the one exemption, told (Predictions to score). On the prototype, which checks no fingerprint, a
mistyped `sum`, a mistyped `version` and an edited tree each pass `check` at exit 0. **The suites
that walk the tree, `corpus`, `packages`, `records` and `unseen`, exempt that folder by name**;
`build/`'s per-module cache needs nothing (the compiler-engineer, § 7).

**R8, the network: `build`, `check` and `run` never fetch; only the verbs that add and fetch reach
the network.** A package missing or not matching its fingerprint is an error that names `heroes
fetch`. The compiler-engineer's veto condition, and `selfhost/cli/refresh.hero`'s rule, *the
network is admissible only where the answer is pre-recorded*, the fingerprint being that answer.
Stricter than the base: Go's `go build` reached for the network for a pinned module (the
ffi-pragmatist, § 6, measured), and Go 1.16's build commands, which no longer modify `go.mod`, are
taken a step further.

**R9, the C boundary: the ffi-pragmatist's C1 to C5, adopted whole.**

1. **C1**: every unit's header search puts the runtime's directory first, then the root of its own
   package (a fetched module's units) or of the program, and nothing of another package; this also
   answers defect 2324 for a program's `--include` folders.
2. **C2**: every file a package's unit reads lies under its package's root, the runtime or the
   toolchain's own directories, judged on clang's `-M` listing, which the compiler already asks
   (`build/reads/`) and keeps (`build/tu-*/main.deps.txt`), before any compile. **Ruled by the
   coordinator, 2026-11-07**, on the question this synthesis raised (What the seats could not
   run): a package's own files are read only inside its root, and the include folders
   `pkg-config` names for a `package "<name>"` group are the machine's, and allowed.
3. **C3**: the fetch fills a staging folder and moves it in only when every file passes: a symlink
   (mode 120000) and a gitlink (160000) refused, two names that collide under case folding refused,
   and a name holding `..`, a leading `/`, `\` or `:` refused, read from `git ls-tree -r -z`.
4. **C4**: `--` before every word a package controls handed to `git` or `curl`, and a source is
   `https://` by allow-list, the way `package_words.hero` judges a `.pc`'s words. The synthesis's
   first draft found this **not reconciled**: R15 and R19's goldens read an index over `file://`,
   and R19 a git source that is a local repository, which an `https://`-only allow-list refuses;
   and where the index's address is given to the verbs was unruled. **Ruled by the coordinator,
   2026-11-07**: **`https://` for every network source; `file://` only for a path inside the
   program's own repository root** (R15's local static index, R19's goldens' local git
   repositories); **every other scheme refused**. The index's address is given to the verbs **in the
   catalogue's own `index` line**, the Heroes index by default (R5), **and on the command line for
   `add` only**.
5. **C5, no fetched code runs at build**: a fetched header's integer constants are read from
   clang's AST dump and never by running a probe (the critic's route, measured on this Mac alone:
   the value read, the constructor not run). Provisional until measured on the four CI legs at the
   first version; until then **a fetched header that needs the probe is refused, never run**.

The ffi-pragmatist's veto route, a package's folder ahead of the runtime's on the program-wide
search path, is not taken.

**R10, the C under the first version: `git` and `curl` run as programs, never libgit2 or
libcurl bound into `heroes`.** `process.run` already runs `pkg-config` and `curl`; libgit2 is absent
on this Mac; the seed's one clang line stays as it is, so nothing new is installed for the
compiler to build. `build`, `check` and `run` need neither program once the tree is placed (R8),
so M-standalone-compiler (row 83) holds for them; that row owes that `heroes fetch` names what it
needs when `git` or `curl` is missing. Whether the CI's runner images carry both is unread
(`ci.yml:359` installs neither). The second version, over the core packages, meets the seed's
question again (the ffi-pragmatist, § 3).

**R11, the verbs: `heroes add` and `heroes fetch`**, design.md §3.5's two. `add` takes a bare name
against the index, a git address with its tag, or `--path` a folder, runs MVS over the catalogues
it fetches and writes the build list with its sums; `fetch` fetches and verifies every entry and
places it. The command's verbs go from 13 to 15 (`selfhost/cli/table.hero`'s two `Command(` rows,
its pin moved to 15 in the same commit, two arms in `selfhost/main.hero`), the site's count with
them (`site/src/lib/claims.ts`'s `verbs()` reads the rows, so every page saying *thirteen* says
*fifteen* before the site builds), and the landed specification names the verb (the spec-warden:
the fingerprint makes the verb a necessity, since a model writing an entry by hand has no sum to
write).

**R12, the three-inputs rule rewritten (`retro-record` of the author's call).** The spec-warden's
wording (its § 7) for `.claude/rules/cli-surface.md` § There is no fourth slot, whose heading
becomes *Four input classes: the catalogue is the fourth, and there is no fifth*, for design.md
§3.5's *examined and deliberately unplaced* paragraph, and for CLAUDE.md § 10's clause, *the
fourth input class, the catalogue, and the refusal of a fifth*, landed in one commit, the
contract's count re-measured with it. The catalogue is admitted by the author's call and not by
panel 056's conditions, whose second cannot hold for a file whose entries the `use` lines name;
its purpose is kept as **the falsifier: deleting the catalogue, deleting one of its entries, or
mistyping any field of one never changes which module compiles, only whether the program builds,
and a fetched copy left behind after its entry is gone compiles nothing; `path` entries excepted
and told (R6)**. The `check` golden form holds one case for each, and one program that compiles
another module after such an edit returns the admission to a sitting. A fifth input class, a lock,
a workspace or a configuration beside the catalogue, stays refused on panel 056's three
conditions. The spec-warden's wording says the catalogue holds the dependencies and nothing else;
since the coordinator's ruling on C4 (2026-11-07) it also holds the `index` line the verbs read
(R5), and the landing writes that clause so.

**R13, the specification.** One section, the spec-warden's short draft (`merged-min`) as the base:
§ 1's rule and its `Use` production edited in place, `heroes add` named, a dependency's own root
(R4) and one version per program stated, **and the catalogue's format example kept** (critic, *must
not adopt* item 2). Priced by `--refresh` in the landing commit within **W1's bound as registered,
+800 real** for a text that carries a format example (`merged-min` alone read +136). **Where it
lands** (the coordinator's ruling, 2026-11-07, on the first draft's open reading of *one
section*): where `.claude/rules/spec-shape.md` puts a new section, unless the spec-warden's
measured draft edits § 1 in place; the landing decides by that rule and W1's bound.

**R14, CLAUDE.md § 2 stands as written.** No version, toolchain or edition line from Go's or
Cargo's files is needed (the spec-warden, § 5: the `1.0.0` promise makes an edition unnecessary
for 1.x, every dependency is pinned by version and fingerprint, and the one service such a line
gives, refusing a package by name on an older compiler, is unmeasured and could live in the index;
the compiler-engineer, § 0, concurs). The author's relaxation is not used.

**R15, `core`: one catalogue entry.** The 13 copies, plus the canonical tree, which is not a copy
(the compiler-engineer's correction of the brief's *14 copies*, confirmed by the critic, item 6),
become fingerprinted entries resolved from **a local static index read over `file://`** (the
critic's route, item 12, over the ffi-pragmatist's measured `curl` and `h1`), **never `path`
entries**, so every entry stays under the falsifier. The 41 `use core/…` lines of `examples/`
become `use <core>/…`, no old spelling kept (CLAUDE.md § 2). **R15 amends panel 234's R2, ratified
2026-11-06**: that resolution placed each program's copy at `<program>/core/`, reached by a bare
`use core/…` and held byte-identical by a check until `heroes fetch` placed it; the spec-warden
found that placement fails question 17 under every spelling (its § 6), so the copies become
catalogue entries placed where no `use` can spell them, and the author's ratification of this
sitting must cover the amendment (the coordinator's ruling, 2026-11-07). The `packages` suite checks every placed tree against its
fingerprint; its `copies` label has nothing left to hold, `internal` becomes the compiler's, and
the rest stay over the canonical tree (the compiler-engineer, § 11).

**R16, `internal/`, enforced by the compiler**, as the prototype built it (`module/package_told.hero`,
the `packages` suite's rule word for word: a module under an `internal` folder belongs to the module
before the last `internal` part, and only it and the modules under it name it). Measured: `use
<core>/strings/internal/scan` written in a program is refused `private_module`, and the package's
own use of it is not. **It binds the program's own `internal/` folders too**, as the prototype
does, a widened refusal no program of `v0.6.0` crosses (`compat`, all ok); the compiler-engineer
left that half to the sitting (its § 12), the first draft read it from the prototype as built, and
the coordinator confirmed it, 2026-11-07.

**R17, C names: a package module's C names live in a namespace no program module can spell.**
Defect 2327 shows that the emitter's premise, *what separates a name the EMITTER invents from a
name an author could write: a leading digit* (`emit/ctype.hero:98-101`), is already false for a
module led by `_` and a digit, so the
mangler reserves the namespace explicitly, and no component of the program's own begins with a
digit (the compiler-engineer, § 3; no tracked module or folder begins with `_`, so no emitted C in
the tree moves).

**R18, fixes.** Each plausible spelling mistake is refused with a fix tagged by
`.claude/rules/diagnostics-and-goldens.md`'s rule, **`certain` only where it repairs the defect it
names**; a fix that moves a `use` across the mark, either way, is `certain` only where nothing on
the other side answers the same path (the compiler-engineer, § 2). **W3 is not adopted as
registered** (critic-reports item 5), and its correction is: the whole path inside the brackets,
`certain`; the brackets forgotten where the entry exists and no own module answers, `certain`; a
bracket added to the program's own module, `guess`; an address written in a `use`, bracketed or
not, no `certain` fix, *an address goes in the catalogue; the `use` names its entry* with a `guess`
of the address's last part, owed; `use <roman>.numeral` and `use <"roman">/numeral`, each a
`certain` fix (write `/`, unquote), owed; `import`, refused today as `reserved_word` (the critic's
first pass, item 15), its fix unmeasured; a mistake in the catalogue, told with its line and never
machine-applied.

**R19, the interface that stays fixed from the first version to the second, as goldens**: the
`use` form; the catalogue's file, format and fields (`version`, `git`, `tag`, `commit`, `path`,
`sum`, and the `index` line); the fingerprint's algorithm, as the sum of a
fixed tree holding a file that is not text; the placement folder; the verbs, their flags and the
catalogue each writes, byte for byte; the error codes and texts; and the exit codes (a fetch that
fails on the network exit 2, a sum that does not match exit 1, the compiler-engineer's § 12). The
goldens need no network: an index readable from a folder and a git source that is a local
repository; a case needing a catalogue and a placed tree is a folder, which the `check` form must
learn (unbuilt). The second version replaces the transport alone.

**R20, row 78, M-package-index.** A static index, files over HTTPS (Go's proxy protocol and
crates.io's sparse index as precedent, the historian's § 5); names first-come; a withdrawn version
kept and marked, never deleted (crates.io's yank, PyPI's PEP 592, its § 8); no signing at first
(PyPI removed PGP in 2023, the same section); the index applies C3's refusals at publish, so it
never serves a tree a consumer's fetch refuses (the ffi-pragmatist, § 7). **The first version
reads a static index over `https://`, and over `file://` only inside the program's own repository
root** (C4): the one R15 and R19's goldens use, and the one row 78 serves.

## What the conservative resolution would have been, so the author can choose it

- **R2, the base**: Cargo's solver with its lock file, kept as the alternative only where the seats
  measured it, which is little: Cargo prefers the highest version available, so its answer moves
  with the index and needs a lock and an index listing every version (the survey's 2.2, read);
  under the author's exact versions the solver degenerates in the root and stays whole one level
  down, where a dependency's ranges still need a lock (the compiler-engineer's inference from Table
  1B, unrun); the resolver was not on this machine.
- **R5, the format**: the line format or JSON (the coordinator's ruling, 2026-11-07). The line
  format's reader is 60 lines and reads every word as text, so no value can be read as a number;
  JSON's is the smallest, 54 lines over the RFC 8259 reader the compiler already carries, no new
  parser, and no comments in the file.
- **R9's C5**: the author extending panel 216's yes to fetched headers by name, Cargo's stance, so
  the probe runs a fetched header's C at build, bounded at 30 s and 1 MiB as it is today and in no
  sandbox, in place of the dump route.
- **Every other item**: the conservative and the adopted resolution coincide.

## Prior art

Every precedent below is one the historian sourced and marked verified on 2026-11-07, in its report
(`235-reports/historian.md`, § 10 and the rows it cites) or its survey
(`235-reports/historian-survey.md`), and re-read 2026-11-07 by this synthesis with WebFetch, each
quotation matched against what the page returned; N1570 was read from the PDF that fetch returned,
its pages 182 and 183. The decision issue carries the same lines.

- **Reached first by C**: [ISO/IEC 9899:201x, N1570, §6.10.2 Source file inclusion](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf): a directive `# include <h-char-sequence>` "searches a sequence of implementation-defined places for a header identified uniquely by the specified sequence between the `<` and `>` delimiters", and `# include "q-char-sequence"` "causes the replacement of that directive by the entire contents of the source file identified by the specified sequence between the `"` delimiters."
- **Reached first by Rust's anchored paths**: [Anchored and Uniform Paths](https://without.boats/blog/anchored-uniform/), November 2, 2018: "The use statements of anchored paths are syntactically disambiguated as to where they come from:"
- **Reached first by Cargo**: [Specifying Dependencies, Renaming dependencies in `Cargo.toml`](https://doc.rust-lang.org/cargo/reference/specifying-dependencies.html): "For some projects, though, you may wish to reference the crate with a different name in the code regardless of how it’s published on crates.io." and "The `package` key, if not specified, defaults to the name of the dependency being requested."
- **Reached first by Node.js**: [Packages, Subpath imports](https://nodejs.org/api/packages.html): "Entries in the `"imports"` field must always start with `#` to ensure they are disambiguated from external package specifiers." and "Unlike the `"exports"` field, the `"imports"` field permits mapping to external packages."
- **Reached first by Deno**: [What we got wrong about HTTP imports](https://deno.com/blog/http-imports), July 29, 2024: "Import maps allow you to get short and memorable specifiers back and manage versions across many files", and "On their own, however, import maps don’t solve the semantic versioning problem or the reliability problem".
- **Taken from Go**: [Go Modules Reference, the `go` directive](https://go.dev/ref/mod): at `go 1.17` or higher, "The `go.mod` file includes an explicit `require` directive for each module that provides any package transitively imported by a package or test in the main module."
- **Taken from Go**: [Minimal Version Selection](https://research.swtch.com/vgo-mvs), Russ Cox, February 21, 2018: "This approach leads to reproducible builds without lock files", and minimal version selection "also produces high-fidelity builds".
- **Taken from Python**: [PEP 518, Rejected Ideas, Other file formats](https://peps.python.org/pep-0518/): "TOML was ultimately selected as it provided all the features we were interested in, while avoiding the downsides introduced by the alternatives."; of JSON, "the syntax is more verbose than necessary while not allowing for comments"; of YAML, "One is that the specification is large: 86 pages if printed on letter-sized paper."
- **Reached first by Zig**: [build.zig.zon, `dependencies`](https://raw.githubusercontent.com/ziglang/zig/master/doc/build.zig.zon.md): "Each dependency must either provide a `url` and `hash`, or a `path`."; of `hash`, "This is computed from the file contents of the directory of files that is obtained after fetching `url`".
- **Taken from Go**: [Go Modules Reference, Authenticating modules](https://go.dev/ref/mod): "For module zip files, the `go` command computes the hash from the names and contents of files within the archive in a deterministic order."; and of the `.ziphash` file, "Note that the `.zip` file itself is not hashed, so file order, compression, alignment, and metadata don't affect the hash."
- **Taken from Go**: [How Go Mitigates Supply Chain Attacks, Building code doesn't execute it](https://go.dev/blog/supply-chain), Filippo Valsorda, 31 March 2022: "It is an explicit security design goal of the Go toolchain that neither fetching nor building code will let that code execute, even if it is untrusted and malicious."
- **Taken from Go**: [Go 1.16 Release Notes, Modules](https://go.dev/doc/go1.16): "Build commands like `go build` and `go test` no longer modify `go.mod` and `go.sum` by default."
- **Taken from Go**: [Go 1.4 Release Notes, Internal packages](https://go.dev/doc/go1.4): "When the go command sees an import of a package with internal in its path, it verifies that the package doing the import is within the tree rooted at the parent of the internal directory."
- **Taken from Go**: [Go Modules Reference, GOPROXY protocol](https://go.dev/ref/mod): "The requests have no query parameters, and no specific headers are required, so even a site serving from a fixed file system (including a `file://` URL) can be a module proxy."
- **Reached first by Cargo**: [RFC 2789, sparse index](https://rust-lang.github.io/rfcs/2789-sparse-index.html): "Most importantly, the proposed solution works with static files and doesn’t require custom server-side APIs."
- **Taken from crates.io**: [Publishing on crates.io](https://doc.rust-lang.org/cargo/reference/publishing.html): "The version can never be overwritten, and the code cannot be deleted."; and of a yank, "The semantics of a yanked version are that no new dependencies can be created against that version, but all existing dependencies continue to work."
- **Taken from Python**: [PEP 592, Adding "Yank" Support to the Simple API](https://peps.python.org/pep-0592/): "Yanking a file allows authors to effectively delete a file", and "the obvious solution of deleting the existing file from a repository will break users who have followed best practices".
- **Taken from PyPI**: [Removing PGP from PyPI](https://blog.pypi.org/posts/2023-05-23-removing-pgp/), Donald Stufft: "In the last 3 years, about 50k signatures had been uploaded to PyPI by 1069 unique keys."; "only 36% of them were capable of being meaningfully verified"; and "PyPI has removed support for uploading PGP signatures with new releases."

## What the seats could not run

- **Linux x86-64, Linux arm64 and Windows x86-64**, every seat: each C measurement above is this
  Mac's (the ffi-pragmatist's report says what file it read for every other platform, never what
  the platform does). Unrun there: C1's per-unit search, C2's listing, the dump route of C5, `git`
  and `curl` on the CI's runner images, libcurl on the Windows leg.
- **No prototype of the first version's boundary**: the verbs and their transport, MVS inside
  `add`, the fingerprint's C door and its check at every build, R3's identity keying, C1 to C5 in
  the compiler, the four messages R18 owes, R17's rule for the program's own components. Every
  line count of them is unbuilt, and the 475 lines are the prototype's (critic-reports item 4).
- **Cargo's resolver**, not on this machine; and **Go against Cargo for models**, sized by the
  spec-warden (60 first turns, about 1.5 USD) and not run.
- **Reading, diffing and merging catalogues: the question the sitting should have asked, who
  writes the catalogue** (critic-reports item 13). The fingerprint makes the verb necessary (the
  spec-warden) and the catalogue lists every transitive package with its sum (R2), so in use
  `heroes add` writes the file and people and models read it, review it and merge it; the test
  measured a hand copying a given sum from an example. The tasks that could part the forms: an
  entry without its sum, a dependency with dependencies, two pins of one package, a catalogue to
  edit, and a reading question (*which file does line N reach, and what changes if entry X
  goes?*). **About 19 USD is left under the author's cap** (19.29 by the ledger, 18.89 less the
  spec-warden's reckoned 0.40), **recorded as an owed measurement, not spent.**
- **A real implementation's messages**: one repair turn ran in all, so no arm's messages were
  tested.
- **Questions no seat ran**: whether C2's list admits the include folders a `package` group's
  `pkg-config` adds (the ffi-pragmatist's raylib, under `/opt/homebrew/Cellar/raylib/6.2/include`,
  § 1b); whether a header defining another C library's name runs code at build or at run (one,
  `malloc`, ran nothing, its § 2); whether the `count_tokens` endpoint bills (the spec-warden,
  § 10); where the index's address is given to the verbs, since the catalogue admits nothing but
  entries (R12); and why the blind seat's session had no `Write` tool though its command named one.
  **Two of these were ruled by the coordinator, 2026-11-07, unrun**: the include folders
  `pkg-config` names are the machine's and allowed (R9's C2), and the index's address is the
  catalogue's `index` line, or the command line for `add` only (R9's C4, R5). Neither ruling is a
  measurement; the landing's goldens are where each is first run.

## Found beside the sitting, each filed

- **Defect 2324**, `blocking`, the ffi-pragmatist's (`h1/`): a folder named by `--include` is
  searched before the runtime's headers, so a `heroes_standard.h` there switches off the overflow
  abort in the program's own code; R9's C1 is its repair for packages.
- **Defect 2325**, `blocking`, the spec-warden's (its § 11, question 17's placements): `use
  ./json/parse` and `use .heroes/json/parse` are told the path climbs, and neither climbs.
- **Defect 2327**, `adjacent`, the compiler-engineer's (its § 3): a module named `_0opt` spells C
  names inside the emitter's own namespace; R17 rests on it.
- **Defect 2323**, `blocking`, the coordinator's at batch 23's gate B beside the sitting:
  `records/tagged` judged a tag by the class a defect was given after it, so a reclassification
  turned a tag legal when placed red in every net; repaired and closed in batch 23, `03ed8115f`.
- **Defect 2326**, `adjacent`, batch 23's lane b23-records, beside 2323's repair: a defect carrying
  two `class:` labels is read by the first in sorted order.

## Process, recorded against this sitting

- **The critic changed the briefs before any seat ran**: 22 items (`critic-briefs.md`), every one
  repaired at `ca8e2dee0`, among them the spelling's status (item 1), U facts contradicting spec
  § 1 (8), the line format's Go words (9), no JSON arm (10), a YAML hazard that could not fire (11),
  the spend bounded by reported cost alone (13), an unpinned grader (14), `235-base`'s parents
  (16), and six questions no brief asked, the blind seat among them (17 to 22). Its reading of the
  survey's brief, 27 findings, had the three main ones repaired before the survey ran
  (`148ec45b1`).
- **The runner's grader carried defects found before any session** (critic-briefs items 2, 3, 5
  and 6): a valid TOML catalogue crashed it, right answers failed, the repair messages leaned
  toward the angle arms, and second turns never run scored as failed. Each was repaired with a new
  case in `blind/dry/`, and the dry run graded 118 answers as expected.
- **The briefs' framing facts the seats found false or incomplete**: *14 copies of `core/`* is 13
  and the canonical tree (the compiler-engineer and the critic, who corrected its own first pass's
  item 15 in its second, item 6); *a file that is not UTF-8 has no fingerprint today* and *`-I` is
  the compiled file's directory*, both incomplete (the compiler-engineer and the ffi-pragmatist);
  and the ffi-pragmatist's brief named *a header is compiled* where a cold build also runs a
  header's C, which no section of design.md covers.
- **The blind seat's session reported no `Write` tool** though its command named `Write` in
  `--tools` and `--allowedTools`; the diagnosis is unrun, and its report is its result text,
  copied byte for byte by the coordinator from `build/b235-seat-out/seat.jsonl`.
- **The first Linux bless of batch 23, beside the sitting, was contaminated by macOS `tar`'s `._`
  files**, and was redone with `COPYFILE_DISABLE=1` (the coordinator's account).
- **The five-hour usage limit stopped the coordinator and an agent from about 15:45 to 16:50**
  (the coordinator's account).
- **Writes outside the repository's root**, against CLAUDE.md § Hard stops: the ffi-pragmatist
  copied a scratch file to `/tmp/x`, removed at once, and its `brew list --versions` downloaded
  Homebrew's API data into Homebrew's cache (its § 0, both reported by the seat); earlier, the
  survey's builder wrote to `/private/tmp` (the coordinator's account).
- **The spec-warden's predictions were sealed before the first session**: `sealed/predictions.sha256`
  at 17:33:57, the draw at 17:34:01, the sealed section matching its report's § 1.
- **The critic rebuilt the compiler-engineer's prototype in its own worktree** from its twelve
  changed files (`sources 0b1e13e910551b34`, matching the seat's) before measuring on it.
- **`235-base`**: `3b071e4f12f2d3357ca2bd3bc338464dc54aa655`, parents `328524be8` and `9791d4787`,
  tree `f4dbb2b8db0c072de8a01cc6558307af4a4b89e1`.
- **The synthesis's first draft raised eleven points against the resolution it was handed, and the
  coordinator ruled on them the same evening, 2026-11-07**, each ruling written beside the objection
  it answers: R3 taken to the compiler-engineer's reading, made robust; C4's schemes and the
  index's address; U-bare's reasons; TOML's reasons and its conservative alternatives; the git
  commit recorded beside the tag; W4 restated, its registered form kept; R15 named as an amendment
  of panel 234's R2; R16 and R13 confirmed or settled; C2's `pkg-config` folders allowed; and the
  word *package* left open for the landing. Two figures the first brief carried were corrected
  without a ruling: the money left under the cap, about 19 USD by the ledger where the brief said
  about 19.5, and the critic's *contradiction 5*, which is its item 7.

## Author's verdict

Pending, asked by the sitting's decision issue (drafted at
`.claude/worktrees/235-coordinator/ratify/decision-235.md`): ratify R1 to R20, and inside them the
three choices where the adopted resolution parts from the conservative one: the base, Go's modules
with MVS and no lock file or Cargo's solver with its lock; the format, a TOML subset or the line
format or JSON; C5, fetched headers' constants read from clang's dump and the probe refused or panel
216's yes extended to fetched headers. **The ratification must also cover R15's amendment of panel
234's R2**, ratified 2026-11-06: the copies of `core/` leave `<program>/core/` and a bare `use`, and
become catalogue entries. No control beat the author's spelling, so no measured case against
positions 3 and 7 is put. Beside the ratification, the owed measurement: whether to spend part of
the 19 USD left on the tasks that ask how a catalogue is read, edited and merged.

**RATIFIED, 2026-11-07**, R1 to R20 as adopted, and inside them the three choices as the sitting
took them: Go's modules with minimal version selection and no lock file; the TOML subset; C5 by
clang's dump, a fetched header that needs the probe refused. And R15's amendment of panel 234's R2
with them: the copies of `core/` leave `<program>/core/` and become fingerprinted entries of the
program's catalogue. The author answered in conversation that evening, meant as: *ok, I ratify*,
on the coordinator's account of the resolution, which recommended ratifying R1 to R20 as adopted
and named the three places where the author might choose otherwise and the amendment of panel
234's R2. So **the conservative resolution of § What the conservative resolution would have been
is taken in none of the three**. Recorded as a reading (CLAUDE.md § 4). The author may overturn
it. It does not settle defects 2324 to 2327, filed beside the sitting, nor the measurement left
owed, the tasks that ask how a catalogue is read, edited and merged; it opens M-package-manager's
first version, over C.
