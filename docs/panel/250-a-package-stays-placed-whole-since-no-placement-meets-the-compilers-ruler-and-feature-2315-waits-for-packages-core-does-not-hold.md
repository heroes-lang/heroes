# 250: a package stays placed whole, since no placement meets the compiler's ruler, and feature 2315 waits for packages `core` does not hold

**Status** a full sitting, convened 2026-11-14 by **the author's answer *1a*, given at about 22:20** to a
batch put in plain text with the coordinator's recommendations: *"a sitting is convened on placing only
the modules a program uses (*b* was moving 2315 to a later milestone, *c* waiting), 2315 staying an
acceptance condition of this milestone until it rules"* (`docs/roadmap/milestones/M-core-packages.md:497-509`
in the base; `250-briefs/00-shared.md:3-11`). The milestone's panel gate was asked at panel 242
(`00-shared.md:57-60`), so the coordinator convened without asking again (CLAUDE.md § 4). **Why it was
convened**: panel 242's R2 (ratified 2026-11-10) lets the compiler take a package only where its own tree,
*"placed files counted"*, is smaller after the step or a measured defect closes, and a package is placed
whole, so feature 2315 (the package manager rewritten over the core packages) could not meet it
(`00-shared.md:119-139`). **The full panel, not the soundness lane**, since the question touches the tool
surface, the architecture of what the build reads, a sentence of spec § 1 and design.md §1.11's ruler
(`00-shared.md:61-65`). **No paid run was named in the briefs** (`00-shared.md:16-19`); **the blind seat
was added after the four seats had reported, approved by the author on 2026-11-15 at about 02:03 with a
ceiling of 10 USD**, by the coordinator's relay (`250-briefs/llm-ergonomist.md:3-5`;
`250-briefs/critic-reports.md:52-58`), and spent **3.139598 USD** (24 sessions, `250-reports/llm-ergonomist.md:31-33`;
re-summed by this synthesis from the 24 `run.json` files, `python3 -I`, 24 values, sum 3.139598). The
resolution is `provisional — author ratification pending` (§ Author's verdict).
**Seats** the full panel: compiler-engineer, ffi-pragmatist, spec-warden, historian (advisory),
llm-ergonomist (the blind seat, 24 sessions, four a cell). The completeness critic read the briefs before
any seat (24 items, `250-reports/critic-briefs.md:402-534`; the briefs repaired on them, the first versions
kept as `250-briefs/*.before-critic.md`) and the reports after (15 items, `250-reports/critic-reports.md:318-385`),
its second pass correcting the coordinator's own note of 08:50 (§ The coordinator's note, read against the
reports).
**Base** `48f53b3437c6cd46aba5923f4f4780ac7b347283` (tree `d5453ae5f3d91854d12190a4ca02689568b67804`),
*"Batch 37 round: merge lane b37-check, feature 2299 and defect 2532"*, holding `core` 0.5.0 alone; **the
sitting was first written over `722ae76bf` and rebased here** because batch 37's round published 0.5.0 and
removed 0.4.0 (`00-shared.md:67-88`). Stage 2 printed `sources 5bac791363384945` in every building seat's
copy (`compiler-engineer.md:103-105`, `ffi-pragmatist.md:94-96`, `spec-warden.md:61-66`) and in Linux arm64
(`ffi-pragmatist.md:248-249`).
**Platforms** Darwin arm64 (this Mac, Apple clang 21.0.0), Linux arm64 (`heroes-linux-arm64:latest`,
Debian clang 22.1.8, the ffi-pragmatist's runs); a Windows arm preprocessed by `zig cc -target
x86_64-windows-gnu`, never run (`ffi-pragmatist.md:238-244`). **The Windows box was off**
(`00-shared.md:605-609`); nothing of this sitting ran there.
**Briefs** `docs/panel/250-briefs/`, **reports** `docs/panel/250-reports/`.

## The proposal

Verbatim in `250-briefs/00-shared.md`, § What is being asked and § The routes (lines 119 to 532); in short:

- **The question**: whether, and how, a program places only the modules it reaches (with the C files,
  headers and archives those modules' groups name, and the entry's own catalogue), and what that changes in
  the fingerprint, `heroes fetch` and `heroes add`, the build's reading of a placed tree, the catalogue,
  the falsifier of `.claude/rules/cli-surface.md`, and the rulings of panels 234, 235, 238, 241, 242, 244,
  246 and decision 2528 (`00-shared.md:132-139`).
- **The placement unit** (`:422-437`): **W**, the version whole (today); **M**, the reached modules, the
  entry's catalogue and the C files its reached groups read; **G**, by Go package, a face with its whole
  `internal/`; **H**, the reached `.hero` modules and every file that is not `.hero` placed whole. For
  every program at the base G is M and H is M with the cards.
- **The fingerprint** (`:449-469`): **F1**, `sum` unchanged, the version's listing placed, every build
  holding the listing to `sum` and each placed file to its line; **F2**, a sum per module in the catalogue;
  **F3**, a sum over the placed subset written by the verb; **F4**, the sum checked at fetch alone.
- **The alternatives** (`:471-485`): R2's ruler counted by the modules reached (**rM2**, the spec-warden's
  wording); `core` split into entries; the placed trees not committed; placement left whole and the ruler
  kept, with feature 2315 **moved to a later milestone (*b*)** or **waiting (*c*)**.
- **Nine routes nobody listed in the first briefs**, the critic's (`:487-518`): 234's grain restored; W
  kept and R2 met by its defect branch (route 2); M for the compiler alone (route 3); a listing kept in the
  catalogue; a key over the reached set (route 5); a package declaring its grain to the index; a sum over a
  subset the catalogue declares; M as a flag; the compiler reading `core` from `packages/core/<v>/files/`
  (route 9).
- **The spec's drafts** (`spec-warden.md:96-106`): **p0**, no sentence; **p1**, *"`heroes fetch` places the
  modules the program's `use` lines reach, each file held to the listing the `sum` names"*; **p1r**,
  *"`heroes fetch` places the modules the `use` lines reach, so a `use` written after it is refused until
  it runs again"*; **p2**, a `sum` per module; **fM**, the falsifier's paragraph for M; **rM2** and **rU**
  on design.md:578-579, rU writing the ruler's unit, *"its own tree, every line of every placed file
  counted"*.

## The verdicts

| seat | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object** to M, G and H as the resolution and as the way to feature 2315; **no veto**; **approve W**, placement unchanged; *"no placement route meets [R2] for `core` 0.5.0, in any unit"*; route 9 and route 3 objected (`compiler-engineer.md:17-24`, `:385-401`) | design.md §1.1, §1.7, §1.11's R2 sentence (`:25-27`) | M with F1 in the variant H built, v2: **12 files, +421 −32; +389 lines, +294 code lines, +16,240 bytes, +2 files**; W with R2 counted by reach: 0 lines of compiler, its instrument unbuilt (`:28-40`, `:321-322`) | at M-core-packages' close, for the newest `core`, the compiler taking `strings` with every live copy deleted leaves `selfhost/` plus its reached placed modules larger in code lines (+120 at 0.5.0) under every placement (`:50-53`) | (1) a live copy measured crashing or wrong: route (A) under W approved; (2) a later base where the reached-grain step shrinks while W grows; (3) a program measured needing a subset placed for another reason (`:54-58`) |
| ffi-pragmatist | **veto** on M as the rule for a package's C, any placement of C files computed from a closure; **approve** H's rule for C under two conditions, whatever grain the `.hero` files take; *"The veto does not reach placing only the reached `.hero` modules"*; without condition (2), **object** to H (`ffi-pragmatist.md:18-22`, `:50-55`) | §1.11, §4.19; §1.12 for a wrong constant at exit 0 (`:23-26`) | nine probe packages and three subsets of 0.5.0, on Darwin and Linux arm64; the C side of H built as published subsets; `add` and `fetch` placing H, and F1 in the build, not built by this seat (`:27-33`, `:292-295`) | a package binding libcurl through its door, placed by M on this Mac and built in Linux arm64, reads another file of the package or none (`:41-46`) | the veto lifts on a closure rule, built, that places on one machine every file any build reads on three platforms at every level without clang at fetch; H's approval on (1) F1 refusing an absent non-`.hero` file at every build and (2) H's residue closed, *"unbuilt"* (`:47-55`) |
| spec-warden | **object, provisional**, to M, G and H; **veto, provisional**, to p2 (F2); **approve, provisional**, p0 under W with **rU** where the ruler stays or **rM2** where it moves (`spec-warden.md:21-24`) | design.md §1.2, §1.6, §1.0, §1.7, §1.12; panel 244's R1 (`:25-28`) | vendored only, a lower bound: p1 +29 / +29, p1r +30 / +30, p2 +29 / +29 on the spec; fM +70 / +69 on `cli-surface.md`; rM2 +9 / +10, rU +4 / +4 on design.md; real unmeasured, p1 about +38 real *"an inference, unrun"* (`:29-32`, `:96-109`) | (a) the compiler's step taking 0.5.0's `strings` leaves its tree larger, counted either way; (b) under a built M, 17 of 17 programs refused on a `use` of a face they do not reach, against 0 of 17 under W (`:46-52`); with p1r at least 7 of 10 blind runs run `heroes fetch` before their first `check`, with p0 at most 3 of 10 (`:433-435`) | the compiler-engineer's count showing the reached grain shrinking the compiler's tree while W grows; a blind reading showing p0's readers predict the refusal drops p1r; a program needing a per-module `sum` F1 cannot verify lifts the p2 veto (`:53-57`) |
| historian | **approve (advisory)**, *"of a program placing the reached grain, by Go package"*, with four readings: grain, fingerprint, when checked, the reader (`historian.md:71-89`) | precedent | | | a system that placed the reached grain and returned to the whole for a stated cause; a recorded failure of a per-version listing of per-file hashes; a system checking a placed subset at every build; why Go modules did not carry dep's digest; a page of Go's showing a new import refused in vendor mode (`:560-573`) |
| llm-ergonomist | an experiment, graded by its registered rule (§ The blind seat) | design.md Part 11 | 3.139598 USD | the decision rules, registered before any session (`250-briefs/llm-ergonomist.md:268-294`) | |

## What each seat measured, in short

- **compiler-engineer** (`compiler-engineer.md`): the coordinator's reach re-run, **18 of 18 programs the
  same files** (`:106-108`). **Route (A) re-built at this base**: 804 calls moved, 15 functions and 165
  lines deleted, `check` exit 0, and **the compiler's own modules 33 lines MORE** (172 files, +1200 −1167),
  against panel 242's 14 fewer at `1edabb957`, since `cli/strings` has 157 importers here against 125
  there (`:109-122`); route (C), the two `strings` files at `selfhost/`'s root, **+234 net** (`:123-128`).
  **No copy of `split_lines`, `fields`, `trim_space` or `split_once`** among 98 `(str) -> [str]` and 22
  trim-named functions read (*"A negative sentence, so the search is the claim"*, `:138-146`); every other
  face places more code lines than every copy it could delete (`:147-153`). **A records-only module**: its
  `#line`s name nothing, the provenance key moves with it (`:154-161`). **Route M with F1 built** after the
  language refused its first shape, `error[module_cycle]` (*"parse uses module/offered_modules uses ...
  uses fetch/wanted_set uses module/paths uses parse"*), so the closure is handed down as a function value
  (`:174-191`). **18 of 18 programs re-placed** under it, the placed `.hero` set equal to the closure 18 of
  18, every `check` exit 0, the 17 examples' binaries as `main.expected` says; **215 files placed against
  W's 540** (`:206-216`). **The falsifier**: *"no edit compiles another module"*; a reached file removed
  is refused with the new class **`module_unplaced`**; the brief's edit refused, then placed by `fetch`; the
  listing moved out told as 21 files *"taken out"* (`:217-242`). **Emitted C** `cmp`-equal whole against
  pruned for `ini` and `server`, the `hero_program_sources` line included (`:243-248`). **R2's table**, every
  unit and route (§ The ruler, measured). **Instructions**: the compiler under M against W **−0.019 %,
  inside either side's spread**; `ini`'s `check` **−16.24 %**, `json`'s **−1.75 %** (`:294-310`). **v2**, on
  the ffi-pragmatist's condition (1): a removed card refused (`:314-323`). **The net refused by the guard**
  for every compiler of a copy with no `.git`; a differential probe in its place, **44 of 44 golden case
  folders and 11 of 11 `fetch` cases the same** between the base and v2 (`:327-347`). **The spec-warden's
  (b): 17 of 17 refused under M, 0 of 17 under W** (`:348-352`). **`heroes add` before the `use` line
  places 30 files and no listing**: *"placement under M is a function of the program's history"*
  (`:353-361`). **What 2315 replaces**: 0 of the package manager's 10 C groups and 1 helper,
  `fetch/catalogue_edit.sorted` through `slices` (`:363-378`). Own tests: the base `2164 tests, all
  passed`, v2 `2165` (`:452-456`). Route 9's `check` exit 0 with its *"unverified, no fingerprint holds
  it"* note (`:457-461`).
- **ffi-pragmatist** (`ffi-pragmatist.md`): **a package's C closure moves with the platform, the level and
  the including unit** (`:34-40`). `arms` (`:128-158`): Darwin's `-O0` closure built at `-O2` prints the
  level `1` reading **the SDK's `err.h`**; the Linux closure built on Darwin prints `10 1 1 1`, **the SDK's
  `util.h`**; in Linux, Darwin's closure reads **glibc's `error.h` and `err.h`** (`:256-263`): each exit 0,
  a wrong constant, nothing said. `ask` (`:176-189`): **H has one residue**, a C file naming one of its own
  `.hero` files, `__has_include` false, `2 77 1` where the whole tree prints `2 77 2`, on both platforms.
  `qt`/`rb` (`:204-213`): a closure is *"a function of the unit that includes it, not of the package"*.
  `server` under a pruned tree: no file of `core` read for the platform layer, the archive linked as at the
  base, output `cmp`-equal (`:215-226`). `lnk`: a fetched package's `link` group reads the machine's
  libraries, never the package's (`:228-236`). **H built on both platforms gives every value the whole tree
  gives, but for the one residue** (`:276-295`). **F1 must refuse an absent non-`.hero` file**, else a
  removed header falls through to the SDK's or glibc's (`:297-310`).
- **spec-warden** (`spec-warden.md`): counts at the base as the brief's, real **11303** (`:77-82`); every
  draft priced, vendored (`:96-106`); **no spelling of F2 inside R5's subset** (`:114-120`); the brief's
  refusal re-run, and **the set M would place is refused today** for want of a listing (`:130-138`);
  **which sentences each route makes false**: spec § 1 none under W, M, G, H, F1, F4, the split or the
  rulers; README's opening and § 1 rule 4 under M, G, H; the guide's line 112; `unknown_entry`'s *"a package
  is fetched whole"*, pinned by no golden (`:155-208`); the route-by-route table of what a reader meets
  (`:212-222`); **244's bar for p1** and why p1r over p1 (`:228-248`); **M departs from R6's and R7's
  wording, not R2's rule**, G being the grain that copies Go (`:250-274`); **234's R2 placed the reached
  grain by Go package; whole placement arrived unweighed**, two searches printing nothing, put as a
  question (`:276-303`); **F1's listing is no fifth class on three conditions; a record of which modules
  were placed is one** (`:305-333`); **R2's unit and its reading**, *"every placed file counts, whatever
  its kind"*, rM2 an argument and not a measurement, route 9 the reading that empties the ruler
  (`:359-394`); 246's R2 under M (`:396-409`); Principle 0 route by route (`:411-435`).
- **historian** (`historian.md`): **Go's `go mod vendor` places the reached Go packages and checks no
  vendored byte at build**, both on Go's own reference (`:37-49`, `:95-131`); Go's recorded cost, C code in a
  non-package directory pruned, answered by a layout rule (golang/go#26366, `:140-153`); Cargo vendors each
  crate whole with a per-file listing checked at a build command, *"not a security mechanism"*
  (`:200-244`), and its requests to vendor less stand open since 2019 (`:430-456`); JSR serves files one by
  one, Deno's lock hashes the version's listing, the vendor copy *"only checked when copying into the
  vendor folder"* (`:246-289`); dep pruned by import graph, hashed the pruned tree, checked on `dep check`,
  with `noverify` (`:336-378`); **R7's every-build check over a subset has no precedent among the systems
  read** (`:410-415`); a ruler counted by modules reached *"not found as a stated rule"* (`:475-478`); why
  this project went from the reached grain to the whole: *"None found"* in panel 235's survey and report
  (`:527-536`).
- **The critic's pass over the reports** (`critic-reports.md`): **the coordinator's note (3) misstates two
  seats** (`:24-28`, `:40-53`); **the prototype every later run used fails the ffi-pragmatist's condition
  (2)**, `ask` printing `2 77 1` under v2 and `2 77 2` under the base, `b.hero` left out (`:55-69`); **R2's
  unit is not the question, its form is**, every unit reproduced by another instrument and every version's
  counts taken, `strings`' two files byte-identical 0.1.0 to 0.5.0, and **rS**, a ruler nobody listed
  (`:71-148`); **panel 242's R7 already rules the default**, *"Feature 2315 waits for that package"*, quoted
  by no report or brief (`:113-121`); **the blind seat's Q2 and the spec-warden's prediction rest on a
  reading made after the sessions**, and p1r is false of the graded prototype in the spec's own order
  (`:150-197`); the contradictions between seats, each with the run (`:199-224`); the precedents checked
  (`:226-250`); what is unrun (`:252-281`); the reader-facing escape (`:283-297`); the questions the
  sitting should have asked (`:299-316`).

## The ruler, measured

The compiler's tree counted as `selfhost/`, after the step against before, the step deleting route (A)'s
15 live copies of `strings`' contracts (−46 code lines, +33 lines, −5,126 bytes, no file) and taking no
other face. **The compiler-engineer's table** (`compiler-engineer.md:262-271`), **reproduced unit for unit
by the critic's own mirror of the `layout` suite's `code_lines`** (`critic-reports.md:73-86`), with the
critic's rows for rM2 and rS (`:105-111`):

| route or ruler | what counts | lines | bytes | code lines | files | smaller? |
|---|---|---|---|---|---|---|
| W, route (A): 30 files placed, catalogue | R2 as written | +6,612 | +248,333 | +4,165 | +31 | no, any unit |
| M strict, the 2 reached modules, catalogue | R2 as written | +406 | +9,056 | +120 | +3 | no, any unit |
| M as built (H: + 7 cards), listing, catalogue | R2 as written | +585 | +23,772 | +120 | +11 | no |
| the same, M's own code in the compiler (v1: +382, +15,654, +288, +2) | R2 as written | +967 | +39,426 | +408 | +13 | no |
| G (for the compiler, M's `.hero` set) | R2 as written | +406 | +9,056 | +120 | +3 | no |
| route (C), the 2 modules at `selfhost/`'s root | R2 as written | +234 | +7,551 | +120 | +2 | no |
| **rM2**, W placed, R2 counted by the modules reached | rM2 | +406 | +9,056 | +120 | +3 | **no, any unit** |
| **rS** (the critic's, unlisted): the compiler's own modules, placed files not counted, catalogue | rS | +38 | **−4,978** | **−46** | +1 | **code lines and bytes yes; lines and files no** |
| route 9, a `path` entry to `../packages/core/0.5.0/files`, `packages/` not counted | route 9 | +35 | −5,068 | −46 | +1 | code lines and bytes |

**What it settles**: R2 is not met by `core` 0.5.0 under any placement, in any unit; the nearest is +120
code lines, the same under M, G, route (C) and rM2, because `strings`' two modules carry 166 code lines and
the 15 copies they replace net 46 (`compiler-engineer.md:273-276`). **Placement does not decide R2 here; the
size of `strings` against the copies does.** The unit decides only under rS, which is route 9's count
without route 9's `path` entry (`critic-reports.md:113-116`). M v2's own code, +294 code lines
(`compiler-engineer.md:321-322`), counted in the compiler as the table's v1 row counts +288, makes the step
+414 code lines by arithmetic (120 + 294).

**Every version** (`critic-reports.md:88-100`, `:123-130`): `strings.hero` and `strings/internal/scan.hero`
are byte-identical at all six versions 0.1.0 to 0.5.0, so under rM2 the gap is +120 code lines at every
version; under W with R2 as written the step's gap was **+169, +1,323, +2,984, +3,875, +3,947, +4,165**
from 0.1.0 to 0.5.0, arithmetic over measured parts with this base's deletions held fixed: it has grown at
every version, and closes only for a version whose whole tree is at most 45 code lines, or after about 4,166
more code lines of copies are deleted.

**Under the entry's reading** of *"that package's contract"* (every face's copies deleted), each other face
places more code lines than every copy it could delete: `slices` 218 against at most 76 lines of sorts,
`encoding/json` 2,815 against 911 code lines of every JSON-named file, `strconv` 49 against 0 copies found
(`compiler-engineer.md:281-284`).

**Route 2, R2's defect branch**: the 12 live copies read whole compare bytes or characters and none slices
the text, so none carries the abort of defects 296 and 2409 to 2411; **read, not run**, no differential run
against `core` made (`compiler-engineer.md:286-290`).

## Measured by this synthesis, beside the base

The trunk has moved since the base: lane b38-hooks published **`core` 0.6.0** at `d7264587c`
(2026-11-15 09:04, *"strconv.parse_float, path/filepath and os, published"*), on branches `b38-hooks` and
`b38-round` and on no remote branch (`git branch -a --contains d7264587c`; `git branch -r --contains HEAD`
in the `b38-round` worktree printed nothing). Run in that worktree, every count by `git ls-tree` and `git
show` with the critic's `code_lines` mirror re-typed inline (`python3 -I -c`, no file written):

- **0.6.0 whole: 39 files, 7,914 lines, 310,147 bytes, 5,018 code lines**, 29 `.hero` files, no `.c` or
  `.h`. Under W with R2 as written, this base's deletions held fixed, the step's gap would be **+4,972 code
  lines** and +7,952 lines (arithmetic: 5,018 − 46; 7,914 + 33 + 5).
- **`strings.hero` and `strings/internal/scan.hero` are byte-identical at 0.6.0** (`sha256` prefixes
  `cac66ea67a60021e` and `3a1b953c9bcee1da`, the critic's at the six versions before), so rM2's gap stays
  +120 code lines.
- **0.6.0's `os` face has four functions, `getwd`, `env`, `stat`, `lstat`** (`grep -n '^function'` of
  `os.hero`), over `hero_env_shown`, `hero_fs_why`, `hero_fs_resolved_shown`, `hero_fs_kind`,
  `hero_fs_mode` and `hero_run_exe_suffix` (`os/hero_os.hero`'s group); **the package manager's 10 `extern`
  groups stand in 6 modules** (`git grep -c '^extern "' 48f53b343 -- 'selfhost/fetch/*.hero'
  'selfhost/module/*.hero'`: `index_source` 2, `leftover` 1, `run_lock` 2, `fingerprint` 2, `reading` 2,
  `spelling` 1, the critic's count, `critic-reports.md:142-147`, and not the report's 7), **over
  `hero_file_sha256`, `hero_text_sha256`, `hero_dir_scan`, `hero_dir_at_shown`, `hero_dir_release`,
  `hero_lock_take`, `hero_lock_drop`, `hero_file_key` and `hero_file_read_shown`**, none of which 0.6.0's
  `os` binds. A comparison of names, not of behaviour: whether 0.6.0's `path/filepath` replaces a Heroes
  helper of `fetch/` or `module/` is unmeasured.

## The blind seat, graded by its registered rule

**What it measures** (`250-briefs/llm-ergonomist.md:12-37`, registered before any session): whether a
reader holding only the specification writes, builds and predicts a program correctly when it adds a `use`
of a module of a package it already holds, with p0 and with p1r, and whether p1r suffices; the bar is panel
244's R1 (*"a reader holding only the specification would otherwise write a wrong program or be unable to
predict a refusal, measured"*). **Two worlds**, both run before any session: **W**, the base's stage 2;
**M**, the compiler-engineer's prototype `ce-m2` v2 (`sources 1593fa56260f5ec3`), whose refusal reads
*"error[module_unplaced]: the module `strconv` of the package `text` is in the version the catalogue names
and not placed under ...: `heroes fetch` places every module the program's `use` lines reach, and `build`,
`check` and `run` never fetch"*. **Two arms**, label-stripped: **d** = p0, **h** = p1r (+30 / +30 vendored).
**Three tasks**, four sessions a cell: **t1** the edit written, **t2** the edit predicted, **t3** a second
package, where p1r could hurt (`:61-77`). Every reference and trap answer run in both worlds before any
session, each trap separated from its reference (`:79-100`). **The rules**: Q1, *"p1r enters with M only if
arm h gives at least four more C-right answers than arm d over its twelve sessions"*; Q2, *"p1r suffices as
worded if arm h is C-right in at least 6 of its 8 t1 and t2 sessions and in at least 3 of its 4 t3
sessions"*; the spec-warden's prediction read as at least 6 of 8 for arm h and at most 2 of 8 for arm d
(`:268-294`).

**What ran** (`250-reports/llm-ergonomist.md:8-52`): 24 sessions, every one exit 0 `success`, `run.err`
empty, the model `claude-opus-5-5`, CLI `2.1.311`; **3.139598 USD**, median 0.130788, largest 0.142985,
every one inside its 0.40 cap. **No session wrote `report.md`**: each gave its whole report in `run.json`'s
`result`, saying it had no tool that writes files, `permission_denials` empty in all 24; why `--tools
"Read,Write"` gave no Write tool is untraced. The answers were copied byte for byte and graded by 80 runs of
the registered instrument.

**The grades** (`:145-157`), C deciding every cell, every P right in both worlds:

| task | arm d in W | arm d in M | arm h in W | arm h in M |
|---|---|---|---|---|
| t1, right | 4 of 4 | **0 of 4** | 4 of 4 | 4 of 4 |
| t2, right | 4 of 4 | **0 of 4** | **0 of 4** | 4 of 4 |
| t3, right | 4 of 4 | 4 of 4 | 4 of 4 | 4 of 4 |
| **over twelve** | **12** | **4** | **8** | **12** |

**By the rule as the grader read it** (`:159-182`): **Q1, arm h 12 against arm d 4 in M, +8: p1r enters
with M**; **Q2, arm h 8 of 8 and 4 of 4: p1r suffices as worded**; **the spec-warden's prediction holds on
both halves**, arm h 8 of 8, arm d 0 of 8 (`:184-199`). Recorded beside it: arm d in W against arm h in M,
**12 against 12, a gap of 0**; **M costs a reader of today's spec 8 of 12 first-try answers**, every t1 and t2
answer, each a build predicted accepted and refused `module_unplaced`; **arm h in W 8 of 12**, the four t2
answers predicting a refusal W does not make. No session ran `heroes fetch` in place of `heroes add units`
(`:137-143`).

**The reading that decides, the critic's run** (`critic-reports.md:165-178`): every t1 and t3 answer writes
its steps as a numbered list item's inline code, ``1. `heroes build main.hero` ``. Rule 1 read strictly
(the line must *"begin ... with `heroes `"* once white space and a prompt are taken off) **finds no command
in 16 of the 24 answers**; the grader's reading 1 takes the inline code as the item's text
(`llm-ergonomist.md:56-57`), **written down before the grades, and not registered**. **Strictly, Q1 still
holds** (arm h 4, arm d 0, the registered 4), **Q2 fails** (arm h 4 of 8, 6 asked; 0 of 4, 3 asked), **and
the spec-warden's prediction fails on arm h** (4 of 8, 6 asked). The critic's own reading favours the
grader's, *"the reading under which the list clause can match a Markdown list at all"*, and asks that Q2
and the prediction be said to rest on it.

**The history the tasks did not tell** (`critic-reports.md:180-189`): every task wrote the `use` line before
`heroes add`, by design. Run by the critic in the spec's own order: `heroes add text` before any `use`
places 30 files and no listing under both compilers; the t1/t2 program then builds at exit 0 printing
`3/12/` under the prototype, no fetch. **So p1r is false of the graded prototype in that order**, and Q2's
verdict covers the use-before-add history alone; the add-first cell is unmeasured for readers.

**Context** (`llm-ergonomist.md:201-216`): **24 of 24 answered `no`**; none voided. **18 sessions report
that their prompt listed the sibling folders' names**, opening none; `t2-h-1` names *"The user's message"*
as the source, while `run.sh`'s prompt holds no list (`critic-reports.md:191-195`). Where the list came from
is untraced, since `run.json` keeps no transcript; the names carry each folder's task and arm letter and
nothing of the specifications.

## The resolution adopted

`provisional — author ratification pending`. Robust over conservative wherever the two differ, the
conservative form recorded in § Conservative alternative.

**R1, the placement unit: W stays, every version placed whole.** M, G and H are not adopted; F1 to F4 are
not adopted; none of the nine routes nobody listed is adopted.

- **Placement cannot meet R2 at this base, measured in every unit** (§ The ruler, measured): the reached set
  alone leaves the compiler +120 code lines, +406 lines, under M, G, route (C) and rM2 alike, and the same
  two files are byte-identical at each of the seven versions measured, 0.1.0 to 0.6.0
  (`critic-reports.md:99-100`; § Measured by this synthesis).
  So M would not deliver what the sitting was convened for.
- **What M costs, built and run**: +294 code lines and +389 lines of compiler in 12 files (v2), a module
  cycle the language refused and a function value handed down to break it, a new diagnostic class
  `module_unplaced` (`compiler-engineer.md:174-191`, `:321-322`); **a correct `use` refused, 17 of 17
  programs, where W refuses 0 of 17** (`:348-352`); **placement a function of the order the verbs ran**, `add`
  before the `use` giving the whole version for good (`:353-361`; `critic-reports.md:180-189`); both verbs'
  help rewritten and `unknown_entry`'s *"a package is fetched whole"* left standing in the prototype
  (`critic-reports.md:201-208`); the listing moved out of place, then `fetch`, told as 21 files *"taken
  out"*, an edit the author did not make (`compiler-engineer.md:240-242`). **Unrun under M by anyone**: the net (the guard refuses a copy's
  compiler), the `fetch` and `packages` suites, the 55 golden case folders as M-placed trees, `emission`,
  `warnings`, `corpus`, the site's build; no case lists the files placed; a git source; a closure across
  packages (`critic-reports.md:252-273`).
- **What a placement of C would cost**: the ffi-pragmatist's veto stands on wrong constants at exit 0
  (§ What a veto compels), and H, the one subset rule for C it approves, holds only on condition (2), which
  is unbuilt and which the prototype every later run used fails (`critic-reports.md:55-69`): by the seat's
  own words its verdict on H as built is *object*.
- **What M would buy, measured**: 215 placed files against 540; `ini`'s `check` −16.24 % instructions and
  `json`'s −1.75 %; the compiler −0.019 %, inside either side's spread (`compiler-engineer.md:294-310`); no
  emitted byte moved where the reached modules are the same (`:243-248`; a third program,
  `critic-reports.md:275-281`). Performance is not a justification (CLAUDE.md § 13).
- **Against it, recorded**: the historian, advisory, approves the reached grain by Go package on Go's
  vendor model, dep and Deno (`historian.md:71-89`), and Cargo's requests to vendor less have stood open
  since 2019 (`:430-456`); the ffi-pragmatist approves H's rule for C on its conditions. Neither gives a
  verdict on W (`critic-reports.md:40-53`).
- **W changes nothing a reader meets**: no program, catalogue, `use` line, message, help line or spec
  sentence (`compiler-engineer.md:6-8`; `spec-warden.md:7-10`; `critic-reports.md:285`).
- **Not refused**: a later sitting may take subset placement up on the compiler-engineer's conditions (2) or
  (3) (`compiler-engineer.md:55-58`); what it inherits is § Left open's first item.

**R2, panel 242's R2 stands, and its unit is written down (rU).** design.md §1.11's sentence at 578-579
reads, in the spec-warden's draft, *"and it takes one only where its own tree, every line of every placed
file counted, is smaller after the step than before"* (`spec-warden.md:106`; its draft
`.claude/worktrees/p250-scratch/250-spec-warden/drafts/design-rU.md`, whose `diff` against the base's
design.md changes lines 578-579 alone, run by this synthesis), +4 / +4 vendored, a
lower bound, design.md under no ceiling (`spec-warden.md:33-35`).

- **Why the unit is written**: R2 states none (`00-shared.md:307-313`; `spec-warden.md:361-367`), the
  sitting read it three ways (lines, code lines, the compiler's own modules), and **the one reading under
  which the unit decides anything is rS** (`critic-reports.md:113-116`), which is also the reading the
  `layout` suite takes for another rule, §11's ceiling, *"a tree a fetch places under `selfhost/` is no
  module of the compiler's to count"* (`critic-reports.md:310-314`, citing `suite_layout.hero:1067-1071`).
  rU fixes R2's reading where panel 242 counted it (lines of every placed file and the catalogue, 2,389 =
  2,334 + 55, `00-shared.md:307-313`), so the ceiling's reading cannot be carried into the admission's by a
  reader.
- **Why not rM2**: it changes a ratified ruler and unblocks nothing (+120 code lines at every version), its
  instrument is unbuilt, `#line`s missing a records-only module (`compiler-engineer.md:402-407`), the
  historian found no ruler counted by modules reached stated anywhere (`historian.md:475-478`), and the
  spec-warden names what would make it wrong: a repository carrying unreached code that a later step reaches
  with no step of its own (`spec-warden.md:419`). Its case is an argument, *"not a measurement"*
  (`:368-375`).
- **Why not rS**: it is route 9's count without route 9's `path` entry, the reading the spec-warden says
  *"placed files counted"* was written to forbid (`spec-warden.md:373-382`; `critic-reports.md:113-116`); it
  is met in code lines and bytes and not in lines or files (`:110`), so the unit then decides; and it would
  admit a step that places 30 files and 4,211 code lines for −46 of the compiler's own, delivering a 2315
  that replaces 0 of the package manager's 10 C groups and 1 helper (R3).

**R3, feature 2315 waits for packages `core` does not hold; where it waits is the author's, and (*b*) is
recommended.**

- **What 2315 would replace, by count**: its notes say *"the package manager's parts over C"*
  (`gh issue view 2315`, its body's lines 21-22); **`core` 0.5.0 replaces 0 of its 10 C groups and 1 Heroes
  helper** (`compiler-engineer.md:363-378`), and under any admitting ruler the rewrite is route (A)'s 52
  call moves in `fetch/` and `module/` with no function deleted there (`critic-reports.md:142-148`); 0.6.0's
  `os` binds none of those groups' doors (§ Measured by this synthesis). The two SHA-256 doors stay C by
  panel 242's R4. So the feature's substance waits for faces that cover a directory scan, a file read and a
  run lock, whatever R2's form: a reading of the counts, not a run.
- **The five answers, each with its measured effect**:
  - **(*b*) 2315 moved to a later milestone**, R2 standing (with rU, R2): M-core-packages' row no longer
    closes with 2315 (`M-core-packages.md:413-417`, *"this row closes with it"*), R7's *"Feature 2315 waits
    for that package"* holding in the later milestone; nothing in the compiler moves.
  - **(*c*) R7 as ratified, 2315 waiting as an acceptance condition of M-core-packages**
    (`242-*.md:189-194`, *"Feature 2315 waits for that package; M-core-packages' acceptance condition gains
    R2's sentence"*): the row waits for the compiler's first package by R2's ruler, under W a gap that has
    grown at every version, +4,165 code lines at 0.5.0 and +4,972 at 0.6.0 by arithmetic.
  - **rM2**: design.md:578-579 and 242's R2 amended (+9 / +10 vendored); the gap +120 code lines, +406
    lines, at every version; it closes after 121 more code lines of `strings`' contract are deleted, or a
    smaller `strings`; its instrument unbuilt.
  - **rS**: R2 amended to count the compiler's own modules; met today in code lines (−46) and bytes
    (−4,978), not in lines (+38) or files (+1); route (A) admitted under W with 30 files placed; 2315
    delivered as 52 call moves, 0 C groups and 1 helper replaced.
  - **R2's defect branch**: W and R2 unchanged; admits route (A) if a live copy is measured crashing or
    answering wrong on a caller's input; the 12 copies were read and none slices, **no differential run
    made**; its instrument, each copy against `core`'s function on its callers' inputs, unbuilt. A copy
    found wrong is a `blocking` defect of the compiler by itself, whatever R2 admits (design.md §1.12).
- **Recommended: (*b*)**, with R2 and rU. **The reason, measured**: under R2 as ratified none of the seven
  versions of `core` measured meets the ruler, and W's gap grew at each one (+169 to +4,165 code lines, and
  +4,972 at 0.6.0 by arithmetic);
  rM2 leaves the same +120 at every version; and the one ruler that admits a step today, rS, admits a 2315
  that replaces nothing of what its notes name. Under (*c*) M-core-packages' row waits on a condition no
  measured route meets; under (*b*) it closes on what it can deliver and 2315 waits where the packages it
  needs can be built first.
- **Default until the author reads**: (*c*), R7 as ratified, since moving an acceptance condition the author
  set and a GitHub milestone are the author's (`.claude/rules/records.md` § The issues: *"a milestone, a
  label and every other write to GitHub stay asked every time"*). No issue moves before the reading.

**R4, spec § 1 stays as it is (p0); p1r is not adopted.** Panel 244's R1 stands. Under W p1r would be
false: arm h in W 8 of 12, the four t2 answers predicting a refusal W does not make (§ The blind seat).
**The blind seat's result is read as about M and the prototype it graded**: p1r would enter with M under
either reading of rule 1 (Q1); its sufficiency holds under the grader's reading alone, and its wording is
false of the prototype in the spec's own order (`critic-reports.md:180-189`). A sitting that reopens M owes
both before p1r's wording (§ Left open).

**Refused**: M as the rule for a package's C, any computed closure (vetoed by the ffi-pragmatist); p2 and
F2 (vetoed by the spec-warden); route 3, M for the compiler alone, a special case against panel 242's R3
(objected by the compiler-engineer and the spec-warden, `compiler-engineer.md:385-389`, `spec-warden.md:427`);
route 9, a `path` entry against R15's *"never `path` entries"*, read from a folder no fingerprint guards
(objected by both, `compiler-engineer.md:397-401`, `spec-warden.md:376-382`); F3 (objected, the folder key,
every `#line` path and the seed moving with every `use` change, `spec-warden.md:423`); F4 (objected on rank
3, an edited placed tree compiling at exit 0, `spec-warden.md:424`; failing the ffi-pragmatist's test,
`ffi-pragmatist.md:299-302`); the split (objected, `spec-warden.md:425`; **cannot be spelled**, `use
<encoding/json>/json` refused `error[package_mark_holds_a_path]`, `critic-reports.md:219-223`); trees not
committed (objected, `spec-warden.md:426`). **Not adopted, not refused**: M, G, H, F1, routes 1, 2 and 4 to 8
(R1); rM2 and rS (R2, put to the author in R3).

## What a veto compels

- **The ffi-pragmatist's veto on M as the rule for a package's C**: no placement of a package's C files
  computed from a closure, by clang's `-M` on one platform or level, the groups' header names, a textual
  include scan or a per-package closure (`ffi-pragmatist.md:18-22`). Its ground, run on both platforms: a
  closure taken on one platform or level builds on the other at exit 0 against the SDK's `err.h` or
  `util.h` or glibc's `error.h` and `err.h`, or another branch of its own header, printing a wrong constant
  (`:142-158`, `:256-263`). R1 places no subset, so nothing is computed; a later sitting placing one takes
  H's rule for C with both conditions built (§ Left open).
- **The spec-warden's veto on p2 (F2)**: no `sum` per module in the catalogue; F1 checks the same files with
  no catalogue change, and no spelling of F2 exists inside R5's subset (`spec-warden.md:114-120`, `:422`).
- **The compiler-engineer cast none** (`compiler-engineer.md:17-20`). The historian is advisory.

## Disagreements, not smoothed

- **Placement.** The compiler-engineer and the spec-warden object to M, G and H and approve W; the
  ffi-pragmatist approves H's rule for C on two conditions and gives no verdict on W; the historian approves
  the reached grain by Go package (`critic-reports.md:40-53`). The compiler-engineer's line on the
  difference: *"the precedents show subset placement is viable; the sitting's question was whether it is
  needed, and for R2 at 0.5.0 it buys nothing"* (`compiler-engineer.md:446-448`). R1 keeps W.
- **The verbs' help.** The spec-warden: *"not made false by M"*, incomplete (`spec-warden.md:185-189`); the
  prototype rewrites both summaries (`critic-reports.md:201-205`). The run stands, by the spec-warden's own
  rule (`spec-warden.md:348`).
- **H's count for the compiler.** The spec-warden's *"H counts 419"* (`:390`) is **547** by H's definition
  (368 + 149 + 30, `critic-reports.md:209-212`). No verdict moves.
- **"No golden moves."** The compiler-engineer's probe read 44 and 11 cases the same (`:337-347`); none
  placed a subset, every one held a tree with no listing read whole, and `find ... -name '*.list'` printed
  0 (`critic-reports.md:213-218`): it measures the fall-back, not M in a golden case.
- **R2's unit.** The spec-warden reads every placed file counted, in lines, and offers rU and rM2 alike; the
  compiler-engineer counts code lines for its prediction and leaves the reading to the spec-warden; the
  critic finds the unit decides only under rS. R2 writes rU.
- **The package manager's C groups**: 7 modules (`compiler-engineer.md:367-368`) against 6
  (`critic-reports.md:142-146`); this synthesis's `git grep -c` at the base prints 6.
- **The blind seat's rule 1**: the grader's reading against the strict one (§ The blind seat). R4 reads the
  result as about M alone.
- **Whether README § 4 rule 5 and panel 241's R7, unenforced for a header with platform arms, are a
  defect**: the critic lists them among *"three reader-facing defects under W, today"*
  (`critic-reports.md:291-297`); the ffi-pragmatist calls them *"rulings the compiler does not enforce"*
  (`ffi-pragmatist.md:72-73`). Enforcing them would be a new refusal, a diagnostic class, which CLAUDE.md
  § 4 sends to a sitting; this record files it as a question (§ Found beside the question).
- **Deno's grain.** The historian's argument writes *"Deno fetches only imported JSR modules"*
  (`historian.md:552-553`) without the hedge its own item 6 carries; the critic: the argument says more than
  its pages (`critic-reports.md:242-245`). Nothing here rests on it.

## The coordinator's note, read against the reports

The second-pass brief carried the coordinator's note of about 08:50 (`250-briefs/critic-reports.md:52-65`):

- **(2)**, the worktree at `48f53b343` and not `722ae76bf`: **holds**, as the compiler-engineer, the
  spec-warden and the critic each printed (`compiler-engineer.md:65-69`; `spec-warden.md:14-16`;
  `critic-reports.md:4-5`).
- **(3)**, *"Every seat objects to placing only the reached modules and approves whole placement (W)"*:
  **false for two of four**, the ffi-pragmatist and the historian (`critic-reports.md:320-324`). Its other
  sentences hold: the ffi-pragmatist vetoes a computed C closure, and the compiler-engineer measured that no
  placement meets R2 for 0.5.0.

## Conservative alternative, recorded

- **(*c*) with R2's text untouched**: no rU, panel 242's R7 as ratified, feature 2315 waiting as an
  acceptance condition of M-core-packages for the compiler's first package by R2's ruler. Its effect: the
  milestone's row does not close until a version of `core` meets R2 under W, a gap of +4,165 code lines at
  0.5.0 and +4,972 at 0.6.0 by arithmetic, or until route 2's differential run finds a live copy wrong, a run
  nobody has built.

## Prior art, per resolution

None. No resolution of this sitting rests on a precedent the historian sourced: R1 keeps whole placement on
the compiler-engineer's count, M's measured costs and the ffi-pragmatist's veto, against the historian's
advisory reading, which favours the reached grain; R2 writes a unit for which the historian found no stated
rule anywhere (`historian.md:475-478`); R3 rests on the counts of what 2315 replaces; R4 on the blind seat.
The historian's candidate lines stand in `historian.md:639-705` for a sitting that adopts subset placement,
each to be re-read then.

## Found beside the question

- **Filed from this sitting's seats**, each `OPEN` by `gh issue view` at this writing, each naming panel
  250's ffi-pragmatist as its origin and read by the coordinator in the seat's report, not re-run:
  **defect 2555**, `blocking`, *"a fetched header's #embed of its own file is told as reading outside its
  root"* (created 2026-11-14T23:33:29Z); **defect 2556**, `blocking`, *"ffi_missing_header's note says a
  fetched unit never reads another package's files"* (23:33:40Z); **defect 2557**, `improvement`, *"a
  package's link group naming an archive it carries is told only that the machine has none"* (23:33:50Z).
  The critic's second pass calls the first two *"unfiled"* (`critic-reports.md:291-297`, item 14), which
  was stale when it wrote.
- **Unfiled, and a question for a sitting rather than a defect**: whether the compiler should enforce
  README § 4 rule 5 (*"No package ships platform C of its own"*) and panel 241's R7 for a header with
  platform arms, which nothing refuses on either platform today (`ffi-pragmatist.md:72-73`, `:137-140`);
  enforcing them is a new refusal, a diagnostic class (CLAUDE.md § 4).
- **Unfiled: feature 2315's notes name the withdrawn `selfhost/core/`** (its body's lines 21-22, *"by
  packages copied into the compiler's tree under `selfhost/core/`"*, and its Origin's *"as panel 234's R13
  already gives `core/slices`"*), the form panel 241's R3 withdrew (`spec-warden.md:299-303`): a comment
  owed, with its date.
- **Near-copies**: by vocabulary, 27 `(…: str) -> [str]` functions named line, word, field or split, 394
  code lines, and 22 `(…: str) -> str` named trim, strip, space or blank, 163 code lines, an upper bound by
  names (`critic-reports.md:132-140`); the compiler-engineer read 30 and 22 of them and found none a copy by
  behaviour. Turning some into copies changes what the compiler does, rank 3's question before R2's.
- **Two places count a placed tree opposite ways**: R2 for admission, the `layout` suite out of §11's
  ceiling (`critic-reports.md:310-314`); R2 now says which it means (R2).
- **The placed folder's key hashes the `index` line's spelling**: the compiler's tree placed 0.5.0 at
  `core@0.5.0-683195a84f2d9e81`, the examples at `f58b55a0517ed0eb`, `file://../packages` against
  `file://../../packages` (`compiler-engineer.md:135-137`).
- **The `#line` instrument misses a records-only module; the provenance key covers it** (`:154-161`); the
  `packages` suite's own warning (`00-shared.md:272-275`), now measured.
- **Facts of the instruments, for the next brief**: `/usr/bin/time -l timeout ...` counts `timeout`'s
  instructions, 16,813,356 against 131,342,460,263 unwrapped (`compiler-engineer.md:70-76`); the guard
  refuses every compiler built in a copy with no `.git`, judging it against the trunk, so the brief's carried
  *"a narrowed suite does"* stopped holding at this base (`compiler-engineer.md:327-332`;
  `critic-reports.md:254-258`); the blind sessions were given no Write tool by `--tools "Read,Write"`,
  untraced (`llm-ergonomist.md:34-40`).

## What the landing owes

- **R1**: nothing in `selfhost/`, `runtime/`, `examples/` or the goldens.
- **R2**: design.md:578-579 in rU's words, in its own commit after this file and its decision issue,
  `heroes measure docs/design.md` before and after (vendored; the real count unmeasured unless a
  `--refresh` is asked).
- **R3**, on the author's answer: under (*b*), feature 2315's milestone moved on GitHub (asked), the
  M-core-packages page's acceptance sentence (`M-core-packages.md:413-417`) and the ROADMAP's row amended,
  and R7's last clause read as amended by the answer; under (*c*), nothing; under rM2 or rS, design.md and
  panel 242's R2 amended in their own commit; under the defect branch, route 2's differential run built in
  a lane.
- **The comment on feature 2315** correcting its `selfhost/core/` notes (`tests/harness/issues.hero --
  comment`, asked once).
- **The evidence archive**, `250-reports/evidence.tar.xz` (2,327,144 bytes, sha256 `06d629442f1e4772`) and
  `evidence.md` (`/panel` 3c), packed in this synthesis's commit: 1,631 files read back byte for byte, no
  credential found by its scan, the folder `.claude/worktrees/p250-scratch/` the author's to remove.

## Left open, as questions

- **What a sitting that reopens subset placement inherits**: the ffi-pragmatist's veto (H's rule for C,
  conditions (1) and (2) built and run, condition (2)'s closure unbuilt); F1 as the ffi-pragmatist words it,
  the listing outside the package's root; the add-first history ruled, pruning on `fetch` or `add` placing
  the reach, before p1r's wording, and the add-first cell read blind, a paid run (`critic-reports.md:315-316`);
  p1r re-read under a rule 1 registered for list items; a case listing the files placed; the net, the `fetch`
  and `packages` suites and the 55 golden folders in a worktree with its own `.git`; the `packages` suite's
  `placed` check, which an M-placed example fails by its words (`suite_packages.hero:33-36`, read, unrun); a
  git source; a closure across packages; `module_unplaced`'s golden and annotation; the messages whose
  advice M makes false (`spec-warden.md:190-205`).
- Whether any live copy in `selfhost/` answers wrong on an input its callers reach (route 2), unrun.
- Whether 0.6.0's `path/filepath` replaces a Heroes helper of `fetch/` or `module/`, unmeasured.
- Whether any record weighed a subset against the whole when `31c34d962` placed `core` whole: two searches
  with different vocabularies printed nothing (`spec-warden.md:288-298`; `00-shared.md:211-216`), and the
  historian found none in panel 235's survey and report (`historian.md:527-536`).
- Where the 18 sessions' list of sibling folder names came from, and why `--tools "Read,Write"` gave the
  sessions no Write tool.
- Whether Cargo's listed-file check runs at every build (`historian.md:230-234`), and why Go modules did
  not carry dep's digest (`:371-374`).

## Predictions to score

- compiler-engineer: the compiler taking `strings` with every live copy deleted leaves `selfhost/` plus its
  reached placed modules larger in code lines under every placement, +120 at 0.5.0
  (`compiler-engineer.md:50-53`): open, at M-core-packages' close; at 0.6.0 the two files are unchanged
  (§ Measured by this synthesis), so +120 by arithmetic while the compiler's copies stand.
- ffi-pragmatist: the libcurl door package placed by M on this Mac and built in Linux arm64 reading another
  file or none (`ffi-pragmatist.md:41-46`): **unscored**, M not adopted and no such package built.
- spec-warden: **(a) held**, +406 lines and +120 code lines under M, +234 and +120 under route (C)
  (`compiler-engineer.md:424-427`); **(b) held**, 17 of 17 against 0 of 17 (`:348-352`); the registered
  prediction on p1r: **held under the grader's reading** (8 of 8, 0 of 8), **failed on arm h under the
  strict reading** (4 of 8, 6 asked) (`critic-reports.md:172-178`).
- The blind seat's rules: **Q1 held** under both readings (+8; strictly +4); **Q2 held** under the grader's
  reading and **failed** under the strict one.
- historian: conditions, no predictions.

## Process notes

- **The base moved before any seat ran**: first written over `722ae76bf`, rebased to `48f53b343` on the
  coordinator's word (`00-shared.md:72-79`); the brief's sentence that the worktree still stood at
  `722ae76bf` was false, said first by two seats and corrected by the coordinator's note (2).
- **The critic read the briefs before any seat**, 24 items, every one answered in the briefs
  (`00-shared.md:13-16`), among them the nine routes nobody listed and the measured refusal that made the
  question reader-facing (`critic-briefs.md:259-292`).
- **The blind seat came after the seats**: no brief named a paid run; the author approved the seat at about
  02:03 on 2026-11-15 with a ceiling of 10 USD, as the coordinator relays it; the seat's brief was written
  between about 02:00 and 02:30, last saved at 02:24:44 (`stat`), with its rules registered before any
  session; the sessions ran from 07:58:53 to 08:25:44 by their `run.json` files (`stat`), one at a time; the
  grading finished at about 08:45. **The sessions had no Write tool**, so every answer was taken from its
  `run.json`'s `result`. **18 sessions saw their sibling folders' names**, from a source not traced.
- **The compiler-engineer stopped on the API's rate limit at about 00:40 and resumed at 01:47**, by its
  report (`compiler-engineer.md:3-4`, `:202-205`); the M prototype's build had finished during the stop,
  `ce-m2` dated 00:36, not rebuilt.
- **The compiler-engineer started runs from inside a script the provenance guard does not read**,
  `goldens.sh`, before the guard refused the same run typed directly; it stopped the script on seeing what
  it was, and no verdict of it is used (`compiler-engineer.md:333-336`). On disk: the one run's output,
  `goldens-m2/check-package-.txt`, holds 0 bytes, and the script's log, `goldens-m2.txt`, 5 bytes, `done`
  (`ls -la`, `od -c`).
- **Files written outside the repository's root**: the first-pass critic's `/tmp/x` at 23:12, 708 bytes,
  read back and removed (`critic-briefs.md:361-363`); the blind seat's brief writer's copy of the base spec
  in the session scratchpad at 02:06, to hash it, removed at once (`250-briefs/llm-ergonomist.md:298-300`).
  The historian could not say whether its fetching tool wrote a file outside the root
  (`historian.md:538-545`); the critic's second pass found in the coordinator's session folder 10 files in
  `tool-results/` newer than 2026-11-14 22:00, each the coordinator's own long output, none a fetched page
  (`critic-reports.md:246-250`).
- **The coordinator's session stood idle from about 02:24 to 07:50 on 2026-11-15**, by the coordinator's
  account; no report records it. The blind brief's last save (02:24:44) and the first session's end
  (07:58:53) bound it.
- **The Windows box was off** (`00-shared.md:605-609`); every Windows fact here is a reading or MinGW's
  preprocessing.
- **The machine** stood at one-minute load averages of 308 at 23:23 (`00-shared.md:107-109`), 183.60 at
  00:08 (`ffi-pragmatist.md:96-97`) and 130.58 at 08:50 (`critic-reports.md:3`), so every count is
  instructions, exits or bytes, and no duration is a measurement.
- **No seat made a paid run, a GitHub write or a commit**; the spec-warden's counts are vendored, no
  `--refresh` being named. Defects 2555 to 2557 were filed by the coordinator from the ffi-pragmatist's
  report.

## Author's verdict

Pending: the author's reading of R1 to R4, asked by the decision issue drafted beside this sitting and not
yet filed. A yes settles R1 (W stays), R2 (R2 stands with rU's unit), R4 (p0 stands, p1r not adopted) and
R3's reading of what 2315 waits for; **R3's choice among (*b*), (*c*), rM2, rS and the defect branch is the
author's answer**, (*b*) recommended and (*c*) running until it is given. It does not settle a reopening of
subset placement, nor the predictions, on their own clocks.

**Filed 2026-11-15** as decision 2564, which asks the author's reading of R1 to R4 and R3's choice; the
paragraph above is left as written. R2's unit landed in design.md §1.11 at a1109217a, provisional.

**Ratified, R1 to R4, 2026-11-15**: the author's answer at about 09:35, *I accept all your
recommendations*, to the coordinator's recommendations put in plain text that morning with decision
2564, recorded as a reading. **What the yes settles**: R1 to R4 as adopted, every version placed
whole, panel 242's R2 standing with its unit as landed at a1109217a, spec § 1 unchanged and p1r not
adopted; and R3 answered **(*b*)**: feature 2315 moved to row 78, M-package-index, on GitHub that
morning (its milestone alone written, its body read back byte for byte unchanged), M-core-packages'
row no longer closing with it, panel 242's R7 *"Feature 2315 waits for that package"* holding in
M-package-index. The conservative alternative, (*c*), not taken. **What it does not**: a reopening of
subset placement; the predictions, on their own clocks.

**Invalidated in part by M-rebel-library, 2026-11-19**: R1's *every version placed whole* is
amended by name by panel 253's R6 (ratified 2026-11-19): an entry with a `packages` line places
only the packages it names, with each one they use, and an entry without one keeps R1. R3's
*(b)*, feature 2315 waiting at M-package-index for packages `core` does not hold, no longer holds:
2315 is M-rebel-packages-manager's, the package manager written with Rebel (the author's decisions
4 and 8 of 2026-11-15). R2 and R4 stand. Entry 12 of the register of the decisions M-rebel-library
invalidates, <register>.
