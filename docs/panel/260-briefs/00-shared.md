# Panel 260, the shared brief: a package says where it is defined that it needs the platform layer

**A full panel** (`.claude/skills/panel/SKILL.md` § Two lanes): `compiler-engineer`, `ffi-pragmatist`, `spec-warden`,
`historian`, and `llm-ergonomist` **registered** (its paid run **authorised by the author**, *ok a*, relayed by the
coordinator at 10:37, a ceiling of 20 USD, and started by the coordinator after the synthesis: `llm-ergonomist.md`), and
the completeness critic, who reads these briefs FIRST, before any seat starts
(§ 3c). The sitting was convened at about 10:05 as a soundness-lane question, *how does a build choose and ask the
platform layer's members, so that it costs what the program reaches, never refuses a correct program and never serves
a stale archive?*; **the author's instruction of 2026-11-25, relayed by the coordinator before 10:33, made it a full
panel**: its answer is a new form (a declaration) and a new diagnostic class (CLAUDE.md § 4), and the most robust
solution wins even at the cost of a new form (CLAUDE.md § Precedence, the author's amendment of 2026-11-22). The
author's words, meant as: *I want a truly robust algorithm: a library declares that it is native when it is DEFINED,
not when it is imported; if a library uses the layer and does not declare it, that is an error, never a fallback that
compiles everything; paying more when you use libraries that need the layer is fine, paying when you do not use it is
not; that is the principle, or we would not have built the whole compiler and all the compiler's and the net's examples
without a standard library.*

**The base is `main` at `c3885b81b`** (`c3885b81b3d9c98b012cee1465c23c53282844a7`, 2026-11-25 11:04:38, the merge of
lane dlsym-make), **re-frozen at 11:18 after the critic's first reading** (`git -C .claude/worktrees/panel260 checkout
--detach main`, `git log -1`; `main` still `c3885b81b` at 12:31, `git log --oneline -1 main`), unless the
coordinator's prompt names another; every seat's copy and every timing of a
seat stands on it. The briefs were first written at `80e2e39ad` (`80e2e39adc5d1bcf2550b6f5d0e9a8ff63ac56ea`, lane
layer-speed's steps 184a to 184c merged), and the critic read them there. **What moved since** (`git log
80e2e39ad..c3885b81b`, `git show --stat` of each step): **step 182c** (`cb4d420b7`, merged `630f9a105`), the walk of
every file a build's first clang would open, before that clang (`selfhost/cli/include_stands.hero`, `include_lines.hero`,
`include_told.hero`, new; `header_stands.hero` and `shadowed.hero` changed; two `unsupported` cases), **paid by every
build, a program with no group included: § 4.5**; **steps 188a and 188b** (`8720edfaa`, `b9c91363f`), one line in each
of 38 generated tables of Rebel's library and three in `.github/workflows/rebel.yml`, no `use` line among them; **step
185a** (`a4bdf4257`, merged `c3885b81b`), five files of `runtime/parts/` (`rebind.c`, `rebind_image.c`, `sigjump.c`,
`siglookup.c`, `stack_lane.c`), seven `run` goldens and two headers of their own (none reaching a member's header,
`shapes/own_deep.py` below), seven emissions and `tests/harness/platform_skips.hero`. Nothing under `spec/`, `docs/`,
`.claude/`, `seed/`, `runtime/platform/` or `runtime/*.h` moved (`git diff --stat 80e2e39ad c3885b81b -- spec docs .claude
CLAUDE.md seed` prints nothing; `-- runtime` names the five `runtime/parts/` files alone), and in `selfhost/` only 182c's
five files did. Every number, path, line and count below was produced by the command named beside it, **re-run at
`c3885b81b` by the brief's repair on 2026-11-25 from 11:18 on**, in `.claude/worktrees/panel260-repair-scratch/` (its
scripts and outputs copied into `ground/` here, `repair-*`), or names the base it was run at, or is marked *carried from
<file>* where nobody re-ran it. The clerk's first runs, from 10:05 at `80e2e39ad`, are kept in `ground/` as they were.
**No seat or
critic starts a paid run, a `claude -p` session, an API call or a cloud run its brief does not name; none prints a
credential** (`gh auth status` masks it, `gh auth token` never); `gh` reads only; no file outside the repository's root
(no `/tmp`); no `pkill`.

## 1. The proposal (the author's principle)

A package declares that it needs the platform layer **where it is defined**, and the compiler holds the declaration to
the code: a package that binds a layer member's header without declaring it is an error at the package's own check and
build, seen by its writer and never by a program that imports it; a program that binds the layer itself declares it the
same way. A program whose modules and packages declare nothing gets **no archive, no reach asked and nothing of the
layer on its link, and no fallback**; a declared one gets the layer. What the sitting decides: where the declaration
lives and how it is spelled; what *binds the layer* means and how the check reads it; the diagnostic's class, words and
fix; what a declaration that binds nothing is; and how much a declared build pays (R6 whole, or a route of § 7.5).

## 2. The questions the sitting settles

1. **Where the declaration lives, and its spelling** (§ 7.1): a line in the package's face module, a mark on the group,
   the package's own catalogue (spec § 1 already names one, § 3.2), a manifest file per package, or another; what each
   costs the spec (§ 3.3) and CLAUDE.md § 10's input classes (`.claude/rules/cli-surface.md`).
2. **What *binds the layer* means, and what the check reads** (§ 7.2, § 6.2's seven readings): the header names a group
   writes, what each unit's opening reaches, a bound name a layer header declares, the names a group's header bodies
   call (read today only after a link fails), the link's missing names, **step 182c's walk** of every include before
   the first clang (§ 4.5), or the undefined names of the program's own objects read by `nm` before the link; for a
   package, a program, a used module, and every shape of § 6, the five on which the readings disagree among them
   (§ 6.2: `constonly`, `platformhdr`, `testonly`, `face`, `twofam`).
3. **The diagnostic** (§ 7.3): its class and code, its words, where it points, its fix (`certain` or `guess`,
   `.claude/rules/diagnostics-and-goldens.md`), and the annotated golden that holds it.
4. **A declaration that binds nothing** (§ 7.4): an error too (the coordinator leans so: a false declaration costs every
   user), a warning, or nothing.
5. **Whether the declaration travels**: a program importing a declared package declares nothing itself and pays the
   layer; a package importing a declared package, the same, or not. **With its count** (§ 3.1, the critic's
   `ground/reach.py` and `reach_examples.py`, re-run at the base): following `use` lines alone, **78 of Rebel's 115
   faces and 735 of its 1,914 modules reach one of the 32 member floors**, and **30 of `rebel/examples`' 43 programs
   with a catalogue** do, none of the 43 binding a member's header itself. So if the declaration travels by `use` and
   a declared build pays R6 whole, a program reading a TOML file (`toml` reaches `io/hero_stream`) pays the layer's
   `-M` and its cold archive, whatever its link then takes (unrun for `toml`; `face` in § 6.2 shows `time.mark()`
   alone taking three members): whether that program *uses a library that needs the layer*, in the author's words
   (§ 1), is the author's word to apply with this number in hand.
6. **How much a declared build pays** (§ 7.5): R6 whole (every member, the linker choosing) or (a), (d), (P1), (P2),
   (f), (g) to (i), with the guards of § 5 kept.
7. **What the landing owes the tree** (§ 3.1): every tracked package and program that binds the layer declared in the
   same change, and nothing kept for an older spelling (CLAUDE.md § 2: *before `1.0.0`, no form enters to keep an
   older program compiling*).
8. **What the spec says** (§ 3.3): the sentence or sentences, priced, and where they go.

## 3. The tree and the language today

### 3.1 What binds the layer, counted

- **A layer header** is a member's: 41 of the 43 C members have a header at `runtime/<member>.h` (`for b in ...; [ -f
  runtime/$b.h ]`), the two without being `hero_ui_print_gtk` and `hero_ui_print_win32`, whose headers stand in
  `platform/`; `hero_os.h` and `hero_compiler.h` are the runtime's, no member being named for them
  (`runtime_key.layer_members`, `selfhost/cli/runtime_key.hero:107`, lists the `.c` files under `platform/`). Each
  arm's own header (`hero_audio_coreaudio.h`, `hero_audio_pulse.h`, `hero_ui_gtk.h`, ...) stands at `runtime/` too.
- **Tracked `.hero` files whose groups name a member's header**: **54** of 9,523 (`git ls-files -z '*.hero' | xargs -0
  grep -lE '^extern "(<member>|...)\.h"'`, the 43 members from `ls runtime/platform/*.c`): **32** under
  `rebel/library`, **20** under `tests/golden/run` (19 at its top, one under a `.case/` folder), **1** under `examples`
  (`examples/server/net.hero`), **1** `tests/harness/rebel_apps_run.hero`. No group line is indented
  (`grep -lE '^[[:space:]]+extern "'` over the tracked `.hero` files prints nothing), so `^extern "` misses none.
- **Rebel**: the 32 are every one a floor module named `hero_<name>.hero` under its package
  (`crypto/hero_crypto.hero`, `net/hero_net.hero`, `os/hero_stop.hero`, `fs/xattr/hero_xattr.hero`, ...), in **21**
  top-level packages (audio, crypto, fs, http, i18n, io, ipc, keyring, net, os, poll, printing, process, serial,
  service, sql, syslog, term, time, tls, ui); **37** files name some `hero_*.h`, the other **5** binding `hero_os.h`
  alone: `os/hero_os.hero`, `path/hero_os.hero`, `threads/hero_os.hero`, `fs/internal/fixture.hero` and
  `process/internal/fixture.hero` (`comm -23` of the two lists). 36 tracked files are named `hero_*.hero`: the 32, the
  three `hero_os.hero`, and `math/hero_math.hero`, which binds none. A catalogue names a floor as a package of its own
  (`rebel/examples/reach/heroes.toml` lists `time` and `time/hero_time`), and a face reaches its floor by `use`.
- **How far a floor reaches by `use`** (the critic's `ground/reach.py` and `reach_examples.py`, module-level `use` lines
  followed from each face, re-run at the base, output `ground/repair-reach.txt` and `repair-reach-examples.txt`, equal
  to the critic's at `80e2e39ad` by `diff`): Rebel's packages reach each other by plain `use` from the library's root
  (266 modules write `use time` and 451 `use bytes`, `git grep -lE '^use time$'`; 306 and 504 lines open `use time` and
  `use bytes`, submodules included; no module writes `use <`); **78 of the 115 faces and 735 of the 1,914 modules reach
  one of the 32 floors**; `json`, `strings`, `bytes`, `numbers`, `unicode` and `path` reach none; `time` reaches
  `time/hero_time`, five `os/` floors and `io/hero_stream`; `toml` the same seven; `csv` `io/hero_stream`. **30 of
  `rebel/examples`' 43 folders with a catalogue reach a floor through a package, and none binds a member's header
  itself.** This is question 5's count.
- **`tests/golden/run`**: of the **1,051** `.hero` files at its top (`git ls-files 'tests/golden/run/*.hero' | grep -c
  '^tests/golden/run/[^/]*$'`; 1,044 at `80e2e39ad`, step 185a adding seven), **99** name a `hero_*.h` and **19** a
  member's header (the other 80 `hero_os.h` or `hero_compiler.h`); **10 more reach a member only through a header of
  their own** (§ 6.2: nine one include deep, one two). The coordinator's *104 of 1,044* counts the files below the top
  too: **1,213** in all (`git ls-files 'tests/golden/run/*.hero' | wc -l`; 1,206 at `80e2e39ad`), 162 of them in 67
  folders below the top, 123 of those under `.case/` folders.

### 3.2 What the language already has where a declaration could live

- **No package-side catalogue or manifest in Rebel**: `find rebel/library -name heroes.toml` prints nothing; a package
  is a folder of modules with a face (`time.hero`), its floors (`time/hero_time.hero`), `internal/` modules and a
  `.ledger` (a coverage ledger, `acme.ledger`'s first lines; 185 tracked, `git ls-files 'rebel/library/*.ledger'`),
  which harness modules read and **no build reads** (`selfhost/fetch/closure.hero` leaves *"its ledgers and cards"*
  out of a placed package, *"since no build reads one"*, its line 16). No Rebel module writes `use <`
  (`git ls-files -z 'rebel/library/*.hero' | xargs -0 grep -lE '^use <'`, none), so no Rebel package needs a catalogue
  of its own today. **But spec § 1 names a package's own catalogue** (lines 42 to 44): *"Inside a package a `use`
  starts at the package's own root, and its brackets name entries of the package's own catalogue"*. Whether a
  published package carries a `heroes.toml` of its own today, and whether a key there is the definition-side home the
  author asks for, is a question (`.claude/rules/cli-surface.md` § Four input classes: *"A program with no
  `use <name>/…` has no catalogue and reads none"*, and the catalogue holds *"the dependencies a `use <name>/…`
  reaches and the address of the index the verbs read, and nothing else"*; *"a second per-project file beside the
  catalogue ... is a fifth class"*, refused on panel 056's conditions). **And the clause a catalogue key meets head
  on**, `.claude/rules/cli-surface.md:73-76` (`sed -n 73,76p` at the base): *"The catalogue holds, per entry, its
  source, its exact version or tag and its fingerprint; a compiler flag, a search path, a library or package of spec
  § 13, a language version, a mode or the program's own name is refused by the reader, never ignored."* A key saying
  *needs the platform layer* names a § 13 library, or is a mode: the catalogue home must say which, and what of this
  clause it changes.
- **What the principle reverses**: panel 234's R6, which made the layer, holds every condition the ffi-pragmatist set
  on P2, among them *"found and linked by the compiler with no `link` word or flag in a package"*
  (`docs/panel/234-*.md:418-419`, `sed -n 416,420p` at the base). A declaration in a package is such a word: the
  sitting says whether that condition still binds and what replaced its reason (`ffi-pragmatist.md`, item 1).
- **Spec § 4** holds the four kinds of top-level declaration; **§ 13** the group, `extern "<header>" link "<library>"`
  (grammar in its section), and line 401: *"sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`,
  linked where a group names one"*. **The spec names three such headers and no other**, `hero_os.h`, `hero_net.h` and
  `hero_time.h`, at lines 401 and 428 (`grep -noE '[a-z_]*hero_[a-z_]*\.h' spec/heroes-spec.md`), line 428 naming
  `hero_net_catch_stop`, `hero_net_ask_stop` and `hero_net_stop_asked` of `hero_net.h` and two `hero_signal_*`
  functions of `hero_os.h`; the other 39 member headers it names nowhere.
- **design.md** §1.11 (lines 503 to 506) states R6 (*"whose archive stands on every program's link line and whose
  members the linker takes into a program only where the program calls them"*); §3.5 (from line 925): lines 934 to 936,
  *"a dependency is a link flag declared in the source next to the `extern` that needs it, or a package the program's
  catalogue names"*, and line 938, *"A per-project file is admitted once, as the program's catalogue"*; §4.19 (2842 to
  2852, defect 2489, the members' needs). Reached by `grep -n`, never from memory.

### 3.3 The spec's counts at the base

`heroes measure spec/heroes-spec.md` from the base's root with the base's compiler, no `--refresh` (the repair's, at
`c3885b81b`, 11:33, its compiler `sources 41f32a8f4b745c0c`, § 10; the clerk read the same at `80e2e39ad`, 10:33, and
the spec has not moved between them): claude-legacy **9,877**, cl100k_base **10,054** (the vendored maximum), **real
13,185** (`claude-opus-5`, the pin of 2026-11-23, no `STALE` printed), headroom **3,199** against **16,384**, the FFI
floor mortgaging 60 (13,245 against the ceiling). The file: 516 lines, 34,437 bytes, sha256
`c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee` (`wc -l -c`, `shasum -a 256`).

## 4. Where the cost is, measured (the convening question's ground, still the second question's)

### 4.1 The layer and what a build asks of it

- `ls runtime/platform | wc -l` at the base: **72** files, **43** `.c`, **6** `.swift`, **23** `.h`. At 17c50eba6,
  `git ls-tree --name-only 17c50eba6 runtime/platform/` names **2**, `hero_net.c` and `hero_time.c`.
- `runtime/runtime.c` names all 72 (`grep -c '^#include "platform/'`): the five `hero_layer_*.h` at lines 213, 231, 236,
  322 and 323, and **67** in the block from line 394 (`awk 'NR>=394 && /^#include "platform\//' runtime/runtime.c |
  wc -l`: 43 `.c`, 6 `.swift`, 18 `.h`).
- What a linking build asks, read in the code: `produce.hero:156` calls `layer_reach.for_build` (`layer_reach.hero:49`),
  which asks nothing for `--emit-c` (step 184a) and otherwise `layer.archive` (`layer.hero:71`): `reaches` (183) asks
  **one `clang -M -MT dep` over all 43 C members at every linking build** (`reached_each.hero:38`, step 184b), the key
  folds its answer in (86 to 88), and a warm build is served where the record vouches and the seal holds (94). Cold,
  `compiled` (127) compiles all 43, asks each object what it needs (154, defect 2489), compiles the Swift members the
  program reaches (161, defect 2625) and archives them. The value probe's link asks the same (`value_run.hero:194`).
- **A warm `print(1)` with the base's compiler calls clang 7 times; the layer's one `-M` over the 43 members (the call
  whose last word is `platform/hero_scm.c`) took 1.400 s of the 1.786 s spent in clang**, and step 182c's walk is one
  more call, `clang -E -v` over `build/package-reads/empty.c`, 0.047 s (`ground/repair-calls.sh`, `ground/calls.sh` with
  the program an argument, clang wrapped to log each call, `ground/repair-calls-b1-print.log`, 11:34, load about 80; its count line
  says *"of them -M 0"* because the log keeps a call's first 50 and last 60 characters, and the `-M` stands between).
  At `80e2e39ad` the same read **6 calls**, the `-M` 1.401 s of 1.725 s (`repair-calls-b0-print.log`, the same
  minute; the clerk's `calls-main.log`, load about 135: 6 calls, 1.412 of 1.774 s). `clock`: 9 calls against 8
  (`repair-calls-b1-clock.log`, `-b0-clock.log`). Route (a)'s prototype, at `0097b3f26` with its patch: **5 calls,
  0.46 s** in clang (`ground/calls-a.log`, the clerk's), with no walk.

### 4.2 `print(1)` on this Mac, the clerk's run, at `80e2e39ad`

**Run before the base moved**: the column `main` below is the compiler of `80e2e39ad` (`sources 2c4d979d320af5a3`),
which lacks step 182c's walk; the base's own compiler, with the walk, is timed against it and `old` in § 4.5.
`ground/time.sh`, 10:38:40 to 10:44:54, `heroes build print.hero -o bin` (`programs/print.hero`, `-O0`), four compilers
interleaved inside each round: `old` (`perf/old`, 17c50eba6's seed), `new` (`perf/new`, 0521cd903's seed), `main` (the
compiler of `80e2e39ad`, `ground/build.sh`) and `a` (route (a)'s prototype binary, § 6.2); each beside its own runtime. A priming
build first, then four rounds, each a warm build in the compiler's own folder and a cold one in a fresh folder; **round
1 discarded, rounds 2 to 4 kept** (`ground/time-mac.txt`, every run). **Load 103 to 141** (`uptime` at each run), so
`real` stands far above `user` plus `sys` in every cell: the machine was waiting, the `user` plus `sys` column is the
better reading, and **the ratios are what stand**.

| compiler | warm, best `real` (its `user`+`sys`) | warm, ratio to `old` (`real`; `user`+`sys`) | cold, best `real` (its `user`+`sys`) | cold, ratio to `old` |
|---|---|---|---|---|
| `old` | 0.36 (0.29) | 1 | 1.02 (0.75) | 1 |
| `new` | 4.77 (2.84) | 13.3; 9.8 | 42.05 (28.49) | 41.2; 38.0 |
| `main` | 2.35 (1.70) | **6.5; 5.9** | 12.38 (7.79) | **12.1; 10.4** |
| `a` | 0.45 (0.33) | 1.25; 1.14 | 1.38 (0.96) | 1.35; 1.28 |

**The ratios are a range, not a figure**: the critic re-ran the `old` and `main` row at `80e2e39ad` (its
`time-mac.txt`, 11:04 to 11:05, load 97.5 to 104.6) and read warm 5.1 (`real`) and 6.3 (`user`+`sys`), cold 12.0 and
10.3, against the clerk's 6.5 and 5.9, 12.1 and 10.4; the warm `real` ratio moved 5.1 to 6.5 between two runs at load
about 100 (`critic-briefs.md` § D). A seat reports its own interleaved range.

`new` compiled the Swift members cold for `print(1)` (a `swift-frontend` over `perf/new/runtime/platform/hero_ui_appkit.swift`
stood in `ps` during its cold build in a first attempt at about 10:36, whose script sent `time`'s lines into the
compiler's error file and which the clerk stopped by its PIDs and discarded whole): 0521cd903 predates defect 2625's
repair. The defect's own reading (05:35, load 6.9 to 8.7: `old` 0.25 s
warm, `new` 2.61 s) and the lane's (`main` at 849bb174a, 3.45 s) are *carried* and not comparable cell by cell with these.

### 4.3 Linux arm64 and the CI

- **Carried from `ground/layer-speed-routes.md`** (the lane's container table, 2026-11-25, load 1.4 to 7, not re-run by
  the clerk): `print(1)` at `-O0`, warm, `old` 0.20 s, `main` at 849bb174a 1.42 s, the lane's 184a and 184b (`c1`,
  0097b3f26) 1.36 s, route (a)'s prototype 0.23 s; cold 0.81, 7.28, 8.88 and 1.29 s. Its probe: the 43 members'
  preprocessing 1.66 s in one run against 1.76 s in 43, *"on Linux the preprocessing is the cost, not the processes"*.
  The brief that convened this sitting says *"1.36 s on `main` with steps 184a to 184c"*: that cell is `c1`, 184a and
  184b at 0097b3f26 (184c renames a file and nothing else, `git show --stat dec556d50`), not `main` at the base.
- A compiler built from its emitted C by `clang -O2` in the arm64 image took 204 to 240 s there (*carried from the lane's
  `linux/results.txt`*).
- **The CI, read with `gh run view <id> --json jobs`**: run 45013609088 (17c50eba6, 2026-11-22) against run 45493660465
  (fc19cb341, 2026-11-24), minutes per job. Darwin's net parts 63, 62, 84, 71 against 211, 190, 352, 203 (**2.9 to 4.2
  times**); Windows' part 2 97 against 241 (2.5 times), its parts 1, 3, 4 and 5 (93, 74, 74, 88 before) **cancelled at
  360**; Linux arm64 213 against **360, cancelled**; Linux x86-64 191 (a failure) against **360, cancelled**. The net
  grew between the two runs too (the `run` suite 833 cases against 1,062 on Linux arm64, *carried from the defect*), so
  a ratio of jobs is not a ratio of builds; the defect's per-case reading, `run` 4.6 s a case against 9.1, is *carried*.

### 4.4 Where the net's builds put `build/` (carried from `ground/layer-speed-routes.md`, the lane's reading)

The compiler keeps its cache in `build/` under the folder it starts in, and the harness starts every compiler in the
repository's root, so **the whole net shares one `build/`** and pays one cold archive per key; the exceptions are three
`unsupported` cases with a `directory` file, `fetch`'s seven `cd <dir> && heroes ...` commands, which link nothing, and
the compiler's own tests, whose layer tests make about ten fresh `build/selftest-*` folders. **What the net pays is the
warm cost**, and under the author's principle a program that declares nothing pays none of it. **That sentence rests
on this carried reading alone**: nobody in this sitting has read where the harness starts its compilers (the three
`directory` cases are tracked, `git ls-files | grep -c /directory$` 3; *"about ten"* `selftest` folders is unchecked,
271 distinct `"build/selftest-` literals standing in `selfhost/`, not all the layer's, `grep -rhoE
'"build/selftest-[^"]*' selfhost | sort -u | wc -l`). A seat that
weighs (P1) or (P2) by what they buy the net reads it first.

### 4.5 Step 182c's walk: what every build pays since `80e2e39ad`, re-run at the base

**What it is**, read in the code at `c3885b81b`: `cli/include_stands.hero` (366 lines; its doc, lines 1 to 39) walks,
**before any clang, the runtime's opening and each group's header and every file they reach**, each name found as
clang's search finds it (a quoted one from its includer's folder first, then the search `clang -E -v` prints over an
empty file, `search_of` at 178 asking `package_reads.toolchain_own`, `package_reads.hero:88`; an `#include_next` from
the folder after its includer's), each file read once through the compiler's own door, and a FIFO, a socket, a device
or a link to one refused before clang opens it. `cli/include_lines.hero` (212) reads the names: `#include`,
`#include_next`, `#import`, `#embed`, `__has_include`, `__has_include_next`, `__has_embed` and an include a `#define`
names; **a block under `#if 0` is passed over and every other `#if` is read as true** (its doc, lines 15 to 17; its
test at 207). The walk is asked by `shadowed.checked` (`shadowed.hero:120`), which `produce.hero:219` asks, and
`produce.produce` is what `heroes build` (`--emit-c` among it), `run` and `test` call (`cli/verbs.hero:60`, `:91`,
`:159`); that `heroes check` walks too is read only in the commit body, which lists `check` among its probes
(*carried*). **A program with no group pays it**: the runtime's opening is walked for every program.

**What it costs, re-run by the repair on this Mac** (the base's compiler, `b1`, against `80e2e39ad`'s, `b0`, the
clerk's `-O2` build copied beside its runtime, `sources 2c4d979d320af5a3`, its runtime equal to `80e2e39ad`'s by
`diff -r`; between the two, `selfhost/` differs by step 182c alone and the runtime by 185a's five `runtime/parts/`
files):
- **One more clang call per build**: `clang -E -v` over `build/package-reads/empty.c`, 0.047 s in a warm `print(1)`
  and 0.051 s in a warm `clock`, by the clang wrapper (§ 4.1: 7 calls against 6, 9 against 8). The headers it reads,
  it reads in the compiler's own process, which no wrapper times.
- **Warm builds in pairs** (`ground/repair-pairs.sh`: `b0` and `b1` alternated, the order inside a pair alternating
  too, two primes each, 13 pairs, the first discarded; `ground/repair-pairs-mac.txt`, 11:40 to 11:46): `print`, the
  base slower in **8 of 12** pairs, by a median of **+0.060 s** of `user`+`sys` (range −0.050 to +0.160), +0.18 s of
  `real`, load 48 to 62; `clock`, slower in **8 of 12**, median **+0.030 s** (−0.070 to +0.180), `real` +0.045, load
  19 to 23. **The noise floor**, measured the same way between them: `b0` against a byte-identical copy of itself
  (`cmp`), `print`, load 25 to 36: a median of −0.015 s, a range of **−0.290 to +0.230 s**, the copy slower in 5 of 12.
  **So at this load twelve pairs do not resolve the walk's cost**: 8 of 12 has a one-sided sign-test tail of 0.19
  (794/4096, an arithmetic), and the medians sit inside the copy's spread. The commit body's reading, a median **+0.06
  s of user time** over 6 pairs at load 7.3 against a noise of 0.005 s, is *carried*, and agrees with the median here.
- **Four rounds, `old`, `b0` and `b1` interleaved** (`ground/repair-time.sh`, `ground/time.sh` with the program an
  argument; a prime, then each round a warm build in each compiler's own folder and a cold one in a fresh folder,
  round 1 discarded; `ground/repair-time-mac.txt`, 11:34 to 11:40, load 68 to 108, so `real` stands far above
  `user`+`sys` and the ratios are what stand). Best `real` of rounds 2 to 4 (its `user`+`sys`):

| program | compiler | warm | warm, ratio to `old` (`real`; `user`+`sys`) | cold | cold, ratio to `old` |
|---|---|---|---|---|---|
| `print` | `old` | 0.41 (0.28) | 1 | 0.89 (0.68) | 1 |
| `print` | `b0`, `80e2e39ad` | 2.22 (1.68) | 5.4; 6.0 | 10.42 (7.61) | 11.7; 11.2 |
| `print` | `b1`, the base | 2.17 (1.74) | 5.3; 6.2 | 12.06 (8.04) | 13.6; 11.8 |
| `clock` | `old` | **refused**, exit 1, `ffi_missing_link` on `hero_time_monotonic_ns` | | refused | |
| `clock` | `b0` | 2.82 (1.94) | | 14.89 (8.80) | |
| `clock` | `b1` | 3.73 (2.05) | | 15.97 (8.91) | |

**Against `old`**: the walk's carried +0.06 s against `old`'s 0.28 s of warm `user`+`sys` is about a fifth of what
`old` spends on `print(1)` (an arithmetic on a median of one run and a best of another, an inference). **What the
compiler-engineer's target counts**: *"`print(1)` at `old`'s cost, warm and cold"* **counts the walk**. The walk is not
the layer's, every build pays it whatever the layer, and robustness ranks above speed (CLAUDE.md § Precedence), so a
seat states its row twice, the whole build against `old` and the same with the walk's share taken out (the pairs
above, or its own); an undeclared program meets the target when what stands above `old` is the walk alone, with no
`-M`, no member compiled and nothing of the layer on the link (the clang log of `ground/repair-calls.sh`, read line by
line, and `nm` of the binary say which).

**And the walk is a reading of *binds the layer*** that nobody listed, already built and run before every build's
first clang: § 6.2 and § 7.2.

## 5. The guards every route keeps, each with its tests

- **Defect 2289** (`gh issue view 2289`): *a header a program adds at its root after a build is not seen by the warm
  cache, so one tree runs warm and is refused cold*. A record of the files a compile READ cannot name a file found now
  ahead of one it read, nor one a `__has_include` now finds (`opening.hero:1-46`), so clang is asked again at every build.
  Tests: `opening.hero:291` and `:326`; `reached_each.hero:163` and `:235`; `layer.hero:465` (a `stdint.h` written ahead
  of the members' after the archive, refused, then served again once gone).
- **Defect 2625** (`gh issue view 2625`; panel 253's R3, the mixed route's Swift member): a Swift member is compiled only
  for a program whose headers reach it, read from the runtime's own text (`layer_reach.hero:97`). Tests:
  `layer_reach.hero:248`, `layer.hero:422`.
- **Defect 134** (issue 1399, *two builds at once in one tree write private names, publish by rename, and say what
  failed*). Tests: `link.hero:342` and `:432`, `toolchain.hero:398`, `units.hero:470`, `produce.hero:407`.
- **Defect 443** (issue 1786, *a zeroed object in `build/` is served by every later build*): the seal (`served.hero`).
  Tests: `verbs.hero:289`, `layer.hero:465`.
- **Defect 2489**, which the convening brief did not name (design.md §4.19, lines 2842 to 2852): each member's needs are
  recorded beside the archive, and a library of the program defining a name a member the link took calls is told on its
  group. Test: `layer.hero:576`.
- **Step 182c's walk** (draft defect d-lane-blocking-small-2, no issue number in its body; § 4.5): a FIFO, a socket, a
  device or a link to one found first at a name the build's first clang would open is refused before that clang, never
  a hang. Tests: `include_lines.hero:196` and `:207`, `include_stands.hero:304` and `:327`; cases
  `tests/golden/unsupported/fixedbugs-a-header-one-include-below-a-groups-header-found-as-a-link-to-a-device-is-refused.case`
  and `fixedbugs-a-library-header-found-beside-the-program-as-a-link-to-a-device-is-refused.case` (`grep -n '^test "'`,
  `git ls-files`). A route that moves what a build reads before its first clang keeps it.
- **R6's own test**, `layer.hero:330` (*every link carries the layer, and a program takes only the members it calls*), is
  the one the principle rewrites: an undeclared program's link carries no layer. Say what replaces it.
- **The C boundary's told refusals** (`.claude/rules/c-boundary.md`, its fifteen members, line 51): the new diagnostic
  must not take a name one of them tells today, nor leave a link that fails on a layer name as *internal error* at exit
  2. **Nor leave it to `cli/header_needs.hero`**: that reader is asked **only after a link fails** (its doc, lines 1 to
  45: *"a name the link reports is this reader's only where ..."*; defect 2372), and tells `ffi_missing_link`, *"no
  group in this program says which library has it"*. Under the principle an undeclared program's failed link reaches it
  first, and that sentence is false of a name the layer has: route (a)'s prototype printed it for `ownhdr`, `owndecl`
  and `platformhdr` (§ 6.2). The new diagnostic is asked before that reader.

## 6. What a reading of *binds the layer* must see: the shapes, measured

### 6.1 Calls between members a header closure misses (`nm`, both platforms)

Route (a)'s prototype (`ground/route-a.patch`, the lane's, 105 lines, `cmp` equal to
`.claude/worktrees/rebel-scratch/panel260/route-a.patch` and to `git -C .claude/worktrees/lane-layer-speed-a diff`;
`git apply --check` passes at `80e2e39ad` and, re-run by the repair, at `c3885b81b`) archives the members whose own
header the program's headers reach. Each of the
base's 43 C members compiled alone under the base's `-O0` words (`flags()`'s list, `flags.hero:136-154`, then
`level_words`, `flags.hero:234`, adding `-O0 -g`), every object read by `nm`, the lane's `closure.py` run unchanged:
`ground/mac-nm.sh` (Apple clang 21.0.0), `ground/linux-nm.sh` in `heroes-linux-arm64:latest` (image `338a1044ac90`,
Debian clang 22.1.8). All 43 compiled on both. **Re-run at `c3885b81b` by the repair** (the scripts with their folder
changed; this Mac 11:23, load 106; the container `p260-repair`, `--rm`, its clock UTC, so its file's *10:23:27* is
11:23 here, as the clerk's *09:13* was 10:13): the same 9 and 107, `closure-mac.txt` and `closure-linux.txt` equal to
the clerk's by `diff` (`ground/repair-mac-nm.txt`, `repair-linux-nm.txt`). **What the counts are**: `closure.py` is a
textual walk of the runtime's quoted includes that **reads no `#if`** (its `inc` regex), the walk (a)'s
`layer_reach.closure` makes over `runtime_key.made_of`; so 9 and 107 are what (a) misses, not what a closure by
`clang -M` would miss (on Linux the textual walk lists the AppKit members under `hero_ui.h`, which `-M` would not
reach): a seat that prices a `-M`-based closure counts again.

| platform | calls outside the closure | by member pair |
|---|---|---|
| macOS arm64 | **9** | `hero_audio` to `hero_audio_coreaudio`, 9 |
| Linux arm64 | **107** | `hero_audio` to `hero_audio_pulse`, 9; `hero_ui` to `hero_ui_gtk`, 98 |

The lane read the same counts from a built archive (*carried*). The reason, read in the files: each arm's own header at
`runtime/` includes `platform/hero_audio_arm.h` or `platform/hero_ui_arm.h`, which the family's member includes, and no
file a program reaches includes the arm's own header. **Not read**: the Swift members' objects, Linux x86-64, Windows,
and calls `nm` cannot see. A declaration that gives a declared build R6 whole does not meet this hole; one that pays by
(a) does.

### 6.2 Programs and how they reach the layer, under the base's compiler and under (a)'s prototype

`ground/repair-shapes.sh <label> <compiler>` (`ground/shapes.sh` with its folder changed and the critic's five probes
added to its list) builds each program in a copy of `shapes/` with one shared `build/`: **`main` is the base's
compiler, `c3885b81b`** (`ground/repair-build.sh`, `sources 41f32a8f4b745c0c`), 11:46 to 11:50, load 19 to 45,
`ground/repair-shapes-main.txt`; `a` **the lane's prototype binary** (`.claude/worktrees/lane-layer-speed-scratch/seed-a/heroes`,
0097b3f26 with the patch, beside its runtime), **not re-run by the repair**: its column is the clerk's
(`ground/shapes-a.txt`, 10:23 to 10:25, load about 130) and, for the tenth golden and the probes, the critic's
(`critic-briefs.md` § D, 11:10 to 11:12). `-O0`, this Mac. **Members taken**: `nm <binary> | grep -c ' T
_hero_<family>_'`, the count of defined symbols so named, per family (`ground/repair-members-taken.txt`, 11:50).

| shape (`shapes/`, `programs/`) | how it reaches the layer | `main` (`c3885b81b`) | members taken under `main` | `a` |
|---|---|---|---|---|
| `print` | not at all | 0 | none | 0 |
| `clock` | a group names `hero_time.h` | 0 | time 5 | 0 |
| `audio` | a group names `hero_audio.h`; the member calls its arm through `hero_audio_arm.h` | 0 | net 36, audio 39, stream 23 | **2**, *internal error: linking failed*, `_hero_audio_arm_beep` and 8 more undefined |
| `ui` | a group names `hero_ui.h` (AppKit's Swift members on this Mac) | 0 | net 36, ui 301, stream 29, poll 17 | 0 |
| `usemod/` | a group in a module of the program's own that `main` uses | 0 | time 5 | 0 |
| `ownhdr/` | a group names `myclock.h`, which includes `hero_time.h`, and binds `hero_time_monotonic_ns` | 0 | time 5 | **1**, `error[ffi_missing_link]: ... no group in this program says which library has it` (false: a member has it) |
| `owndecl/` | a group names `myclock.h`, which declares `hero_time_monotonic_ns` and includes nothing of the layer | 0 | time 5 | **1**, the same words |
| `rebel/reach_hero_time`, `reach_hero_net` | a Rebel floor's group, through the catalogue's `path` entry | 0, 0 | (not read) | 0, 0 |
| `rebel/reach_hero_audio` | a Rebel floor over `hero_audio.h` | 0 | (not read) | **2**, as `audio` |
| the ten goldens of the next paragraph | a header of their own including a member's header, nine one include deep and **the tenth two** | 0 (ten of ten) | the tenth: net 36 | **2** on eight, *internal error: linking failed*; 0 on `fixedbugs-2696-...`; **2** on the tenth (the critic's) |
| `the-platform-layer-tells-a-socket-it-does-not-hold` | names `hero_net.h` beside its own header (a control) | 0 | (not read) | 0 |
| **`constonly/`** (the critic's) | a group names `hero_net.h` and binds only `constant HERO_NET_INVALID` | 0, runs and prints `1` | **none** | 0 |
| **`platformhdr/`** (the critic's) | a group names `platform/hero_audio_arm.h`, **no member's header**, and binds `hero_audio_arm_permission`, which `hero_audio_coreaudio.c` defines; nothing in `selfhost/check` or `selfhost/cli` refuses it | 0, runs and prints `linked` | net 36, audio 39, stream 23 | **1**, `ffi_missing_link`, *"no group in this program says which library has it"* (false: the layer has it) |
| **`twofam/`** (the critic's) | `hero_time.h` and `hero_net.h` in one program | 0, prints `true`, `0` | time 5, net 36 | 0 |
| **`testonly/`** (the critic's) | a used module whose `hero_time.h` binding is called only in a `test` block; `main` never reads the clock | 0, prints `42`; `heroes test clockmod.hero`, 1 passed | **time 5** | (not run) |
| **`face/`** (the critic's) | `use <rebel>/time`, the face, calling `time.mark()` alone | 0, prints `true` | **time 5, net 36, disk 35, stream 23, procs 3, sys 32, users 8** | 0 |

**The base's compiler builds all 26 shapes at exit 0** (`grep -c 'exit=0'` of `repair-shapes-main.txt`, 26 of 26: the
clerk's 20, the tenth golden and the five probes), and every probe it runs ends at exit 0 (each binary under `timeout
20`, its output cut at 4 KiB). At `80e2e39ad` the clerk's `main` built the first 20 (`ground/shapes-main.txt`, 10:32 to
10:35) and the critic's the 21st and the probes; the prototype refuses 12 of the clerk's 20 and the tenth
(`grep -c 'exit=[12]'`), and `platformhdr` too.

**What a link takes is not what a program calls**, measured above: `testonly` takes `hero_time` though `main` never
reads the clock, and `face`'s `time.mark()` alone takes `hero_disk`, `hero_net` and five more families' symbols,
because `emit/extern_refs.hero` holds a `used` reference to each C function a module's unit calls through a group (its
lines 1 to 16; panel 202's R2, defect 562), whatever the program calls. So design.md §1.11's *"whose members the linker
takes into a program only where the program calls them"* (line 505, `sed -n 505p` at the base) is, measured, **where a
module the program reaches binds them**; a reading of *binds the layer* that follows the link inherits this. And
**`constonly` is a correct program the readings disagree on**: the header-name reading, the opening reach and the
bound-name reading say it binds the layer (it names `hero_net.h`, and `HERO_NET_INVALID` is a layer header's name), the
`nm` and link readings that it binds nothing (no member taken); a check that reads *binds* by name and *binds nothing*
by the link refuses it, declared and undeclared alike (§ 7.4).

**Ten tracked `run` goldens bind the layer only through a header of their own**: their groups name no member's header
and name a tracked header under `tests/golden/run/` that reaches one. **Nine** reach it one include deep: they name one
of the 11 tracked headers that include a member's header (`ground/own-headers.sh`, output `ground/own-headers.txt`,
re-run at the base and equal by `diff`); **`own-headers.sh` reads one include deep**, and the critic's widening,
`shapes/own_deep.py` (every tracked C-family file outside `runtime/` and `seed/`, `#include`, `#import` and
`#include_next` followed through tracked files to any depth; output `ground/repair-own-deep.txt`, re-run at the base
and equal to the critic's by `diff`), finds **the tenth**, two deep:
`the-platform-layer-ends-a-wait-at-its-sockets-deadline.hero` (added by `6a6e50dcd`, batch 36's lane b36-net, last
touched by step 83a, `2fe8b450e`, `git log`) names only `the-platform-layer-deadline.h`, whose line 12 includes
`fixedbugs-2502-waiters.h`, whose line 13 includes `hero_net.h` (`grep -n '#include'`). A second depth-two header,
`the-platform-layer-ask-stop.h`, is reached only by a golden that also names `hero_net.h`. No tracked file outside
`runtime/` and `tests/golden/run/` reaches a member's header at any depth but emitted C under `tests/emission/`, and
none spells a member's header with a path (`own_deep.py`'s third list is empty); step 185a's two new headers reach
none. **None of the ten binds a name a layer header declares**: every function their groups bind is their own
header's, `static` or `static inline`, whose body calls the layer (`ground/own-bound.sh`, output
`ground/own-bound.txt`: 21 bound functions for the nine, none named `hero_*`; the tenth binds seven `deadline_*`
functions, none `hero_*`, the same command run on it; `close_wakes_at_once` at
`tests/golden/run/fixedbugs-2502-waiters.h:135`, `pseudo_sides_join_a_set` at `fixedbugs-2667-ready.h:50`).

So each reading of *binds* sees a different part, **read from the files, not run as a check**: the names the groups
write see none of the ten, not `owndecl` and not `platformhdr`, and do see `constonly`; what each unit's opening reaches
(the rows defect 2289 already asks, `opening.reached`) sees the ten and not `owndecl` (a header of the program's own
declaring `hero_time_monotonic_ns` and including none of the layer's, the coordinator's shape); a bound name a layer
header declares (as `cli/runtime_bound.hero` reads the runtime's names) sees `owndecl` and none of the ten; the names a
group's header bodies call (as `cli/header_needs.hero` reads a header's bodies for `ffi_missing_link`, defect 2372,
`.claude/rules/c-boundary.md`; **today asked only after a link fails**, § 5) sees the ten and, as the clerk reads it,
not `owndecl`, whose call is the binding's own; the link's missing names see both, and so does the pair of a bound name
and the bodies' calls, and neither sees `constonly`. **Two readings the first list lacked**: **step 182c's walk**
(§ 4.5), already built and run before every build's first clang, follows every include of each group's header to any
depth at no further clang call, so it would see the ten and `ownhdr`, not `owndecl`, and it reads every `#if` but
`#if 0` as true, so it would also see a member's header under a never-taken `#if` (an inference from its code,
`include_lines.hero`'s doc and its test at 207; no check rests on it yet); and **the undefined names of the program's
own objects, read by `nm` before the link** (one `nm` per object the build already made; `layer.defined_by` and
`layer.taken`, `layer.hero:255` and `:305`, read an archive so today), which sees every door whose reference lands in
the program's objects (the ten's `static inline` bodies, `owndecl`, `ownhdr`, `platformhdr`, a macro, a function
pointer taken in a header) and not `dlsym`, nor a `link` library's object calling the layer (`cli/library_needs.hero`'s,
defect 2365). **A group in a used module of the program's own is seen** (`usemod`), and so is a Rebel package's group
(`rebel/reach_hero_time`, `reach_hero_net`).

## 7. The routes, as questions

### 7.1 Where the declaration lives, and how it is spelled

None is built, priced or chosen. For each: the spec's sentence and its count (the spec-warden), whether a reader writes
it unprompted where it belongs (the registered llm-ergonomist), what the compiler reads to check it and when (the
compiler-engineer), and what a binding author meets (the ffi-pragmatist).

- **A line in the package's face module**: a new top-level form (spec § 4, a fifth kind), or a word on a `use` or a
  module comment; where a package's face is (`time.hero` beside `time/`), and what a program's own root module writes.
- **A mark on the group itself**: the `extern "hero_time.h"` line already names the layer, so a group binding a
  member's header could carry the declaration where it is written (`extern "hero_time.h" native`, a spelling for the
  question, not a proposal); the author's *where it is defined* read as *where the binding is written*. What then
  propagates to the package and to a program.
- **The package's own catalogue**: spec § 1's *"the package's own catalogue"*, a key there; what it does to
  `.claude/rules/cli-surface.md`'s four input classes and the catalogue's *"and nothing else"*.
- **A manifest file per package**: a new file, which the cli-surface rule counts as a fifth input class, refused on
  panel 056's conditions unless the sitting meets them.
- **The importer's catalogue** (`heroes.toml` beside the program): the opposite of the author's *"when it is DEFINED,
  not when it is imported"*, listed so the sitting says why not.
- **And three homes the critic named that the first list lacked** (`critic-briefs.md` § C), each a question:
  - **the `link` word the group already has**, naming the layer as the library that has the symbols (`extern
    "hero_time.h" link "<the layer>"`): no new grammar, spec § 13 already saying *"`link` a library when the symbols
    need one"* (line 401); **panel 234's R6 refused exactly this**, *"found and linked by the compiler with no `link`
    word or flag in a package"* (§ 3.2), so the ffi-pragmatist says whether its seat's condition still binds;
  - **the floor's own file name**: all 32 floors are named `hero_<name>.hero` (§ 3.1), so a rule *"only a module so
    named may bind a member's header"* is a declaration where the binding is written that costs no syntax (the
    historian sources Go's file-name build constraints, `historian.md` question 11); the three `hero_os.hero` files
    and `math/hero_math.hero` are named so and bind no member's header;
  - **a `use` of the layer itself in the module that binds**, as cgo's `import "C"` makes the file that writes it a
    cgo one (the historian's question 11): a different home from *"a word on a `use`"* in the face above, and travel
    would then follow `use` with no new rule (§ 2's question 5 and its count).

### 7.2 What the check reads, and when

At `heroes check` of the package's own module, which the writer runs, the checker must know which headers are a member's:
today that list is the runtime's (`runtime_key.layer_members` reads `runtime.c`), found through the toolchain the build
locates. **Two precedents split the same question by verb**: `check/runtime_header.hero` (97 lines, panel 232's R6,
defect 2256) refuses a group naming `heroes_runtime.h` at `check`, from the group's own string, reading no header; and
`cli/runtime_bound.hero` (98 lines) refuses at `build` a binding of a name the runtime's header declares, whatever
header the group names, a header of the program's own that includes it among them. Questions: does `heroes check` read
the layer's list, or does the language carry a rule a header's name obeys? Which reading of *binds* (§ 6.2: the groups'
header names, each unit's opening reach, a bound name a layer header declares, the names a group's header bodies call,
the link's missing names, step 182c's walk, the program's own objects' undefined names by `nm` before the link) or
which pair of them is sound against every shape of `shapes/`, and at which verb each is told? **The walk is already
built and runs before every build's first clang** (§ 4.5): it reaches a member's header through a header of the
program's own at any depth at no further clang call, and it reads every `#if` but `#if 0` as true, so a check resting
on it would see a member's header under a never-taken `#if` (the ffi-pragmatist's fifth door) and demand a declaration
there, the loud direction, which meets § 7.4's lean that a declaration binding nothing is an error. **The bodies'
reading runs only after a failed link today** (`cli/header_needs.hero`, § 5): a check that wants it at `check` or
before the link moves it. What does an undeclared program that reaches the layer by a header of its own get, with no
fallback: a told error naming the declaration, never *internal error: linking failed*, never the false *"no group in
this program says which library has it"*? What do `heroes test` of a package, `heroes check`, `heroes run`, `--emit-c`
and the value probe (`value_run.hero:194`) do, each the same way? A face module that binds no header but `use`s its
declared floor: must it declare? **And where else the check could run**: `heroes add` and `heroes fetch`
(`selfhost/cli/table.hero:200-201`) place a package, and could refuse an undeclared one before any build.

### 7.3 The diagnostic

A new class (CLAUDE.md § 4): its code beside the C boundary's (`ffi_missing_link`, `ffi_header_refused`, ...,
`.claude/rules/c-boundary.md`), its words carrying everything needed to fix it without opening another file (design.md
§4.17), the line it points at (the group, the package's face, the declaration's home), its fix (`certain` only if
applying it repairs the defect and nothing else), and its golden, annotated in the source that provokes it (CLAUDE.md
§ 9). **Asked before `cli/header_needs.hero`**, which today answers a failed link first and says *"no group in this
program says which library has it"* (§ 5).

### 7.4 A declaration that binds nothing

An error (the coordinator's lean: a false declaration makes every importer pay the layer), a warning, or allowed; and
what *binds nothing* means for a face whose floors bind. **`shapes/constonly/` is the case the readings split on**: a
group naming `hero_net.h` that binds only `constant HERO_NET_INVALID`, a correct program that builds and takes no
member (§ 6.2). The name readings say it binds the layer, the `nm` and link readings that it binds nothing, so a check
that reads *binds* by name and *binds nothing* by the link refuses it declared and undeclared alike; *"no correct
program refused"* is run on it. And a declaration whose functions are called only in `test` blocks
(`shapes/testonly/`) binds for `heroes test` and, by `emit/extern_refs.hero`, for the link too.

### 7.5 How much a declared build pays

| route | R6 | state |
|---|---|---|
| 184a, `--emit-c` asks the layer nothing | kept | committed `a2b288e5d` |
| (c), one `clang -M` for every member | kept | committed `0097b3f26`, `dec556d50` |
| (c'), the members asked in parallel | kept | not built: *"it buys the net nothing"*, the CPU being what the net pays (*carried*) |
| (b), the reach answered from the records | kept | **measured unsound**: a `cups.h` written into `/usr/include/cups` after a cold build, which `hero_printing.c` probes by `__has_include`, makes the next build exit 2, and a record of what was read names no such file (*carried*, the lane's container run) |
| R6 whole for a declared build | kept where declared | the principle's default |
| (a), the archive of what the program's headers reach | changed | prototype; misses § 6.1's calls and § 6.2's shapes |
| (d), the link names the members it took | kept | unbuilt |
| (P1), one archive per platform and level, shipped | kept | unbuilt (the author's) |
| (P2), one archive per machine, every build folder sharing it | kept | unbuilt (the coordinator's) |
| (f), the reach re-validated by the search's directories' times | kept | unbuilt (the coordinator's question) |

**(a), guarded** (the lane's recommendation): the closure held to the archive's member-to-member calls by a compiler test
on every leg; the lane's second half, a link retried with the whole archive, **is the fallback the author refuses**, so
under the principle (a) must be whole without it. Questions: which rule closes § 6.1 (the `*_arm.h` headers a member
includes; the arms' own headers; the members' symbols)? What does a test reading with `nm` miss (a function pointer, a
weak symbol, a library a member opens at run time, design.md §4.19's task 2593, a Swift member's C entry)? How many
distinct closures do the declared programs make, each a cold archive in the one shared `build/`?

**(d)**: R6's archive served on its record's bytes, the link asked which members it took (Apple's `-map`, GNU ld's and
lld's `-t` or `-Map`, link.exe's `/MAP`; readers exist in `cli/supplied_read.hero` for a library the program names),
only those asked what they reach now. Its cold cost is R6's (11 to 22 s on this Mac and 7 to 31 s on Linux arm64 in the
lane's tables, *carried*). **An inference from the code, unrun**: with a `cups.h` planted to refuse `hero_printing`
(`ground/d-lane-layer-speed-1.md`), a warm declared build under (d) that takes no printing member is served the old
archive at exit 0, while the same program in a fresh folder compiles all 43 and exits 2, defect 2289's shape.

**(P1), one archive per platform and level, built once and shipped: the author's proposal, 2026-11-25** (relayed by
the coordinator before 10:28), meant as: *precompile the layer's .a per platform with everything in it; executables
grow a little, compiling is faster; the usual time against space*. Questions the seats measure rather than assume:
which cost it removes (the archive's cold build) and which it leaves (defect 2289's reach, asked at every build);
whether an executable grows at all, the linker taking only the members a program references; what a member compiled
against the CI machine's headers does on a machine whose headers differ (`hero_printing.c`'s probe for `cups.h`, GTK's
version, glibc's): a refused link or a silent ABI mismatch, rank 3 of CLAUDE.md § Precedence; and the release rule
*"the CI is the instrument, and no binary is uploaded"* (`.claude/rules/records.md:179`).

**(P2), one archive per machine: the coordinator's variant of (P1)**, built on that machine against its own headers, in
a cache every build folder shares, keyed as `toolchain.runtime_object` keys today; the convening brief's example, *the
member objects cached per machine rather than per build folder*, is the same route at a member's grain. Questions:
where (no build writes outside the folder it starts in today), keyed how, what it does to defects 134 and 443 and to
CLAUDE.md § 10's input classes, and whether it removes anything but the cold cost.

**(f), the reach re-validated without preprocessing: the coordinator's question, unmeasured**: by the modification times
of every directory on the members' include search path together with the recorded files' bytes; route (b) was measured
unsound for the record of the files read alone. The lane named this and set it aside (*carried*: it needs a stat the
runtime does not offer, and rests on every file system updating a directory's time on every entry). **The first half
of that reason is false as written** (the critic's finding, re-read at the base): `runtime/hero_os.h:209-211` declares
`int64_t hero_fs_newer_than(const char *path, const char *reference);`, *"1 when `path` is newer than `reference`, 0
when it is not, -1 when either is not there"*, defined at `runtime/parts/fs.c:226` by `stat` (seconds, then
`st_mtimespec`'s nanoseconds on Apple, `st_mtim`'s where defined) and by `GetFileAttributesExA` on Windows, both of
which take a directory as well as a file (`grep -n newer_than`, `sed -n 220,260p`). The grep this brief first ran,
`grep -n 'mtime\|modif\|stat(' runtime/hero_os.h`, prints nothing because the door's words are *newer*, not *mtime*.
So a door that says whether a directory is newer than a record file exists today on every platform; whether every file
system moves a directory's time on every entry stays (f)'s question. The convening brief's example,
*the reach of a member asked only when its own key moves*, is this question. Questions: which directories (clang's own
list, `-v`, the SDK's frameworks, `CPATH`), what a directory's time does on APFS, ext4, overlayfs (the containers) and
NTFS, its cost against (c)'s one run, and what a runtime door owes `hero_os.h`'s promise (decision 2228).

**And the clerk's, each a question, none built**: **(g)** the member, not the archive, as the cached unit, each object
keyed, sealed and served alone; **(h)** a closure seeded by the units' own reach and widened by each member's own `-M`
rows; **(i)** a link-driven closure, the members that define the names the linker reports missing added to a fixed
point, never the whole archive; and what would have to be true for a route outside all of these to exist.

**And the critic's, each a question, none built** (`critic-briefs.md` § C): **(j)** the declaration **names the
families it needs** (`hero_time`, `hero_net`), so a declared build archives those and the members they reach, § 6.1's
closure then the compiler's from a family table, and a link short of a name is told as a family undeclared: between R6
whole and (a); **(k)** the layer as **a shared library built once per machine and toolchain**, a constant link cost and
no archive per build folder; what would have to be true is that a Heroes binary may depend on a library at run time,
which `.claude/rules/records.md:179`'s *"no binary is uploaded"* does not settle. And what a declared build pays **at
run time**, not only at build time: defect 2687 (§ 8), the crypto member linking four frameworks for a program that
only digests.

### 7.6 Context: the inferred switch the declaration replaces

**(S), the author's proposal of 2026-11-25** (relayed before 10:28), meant as: *when I build, look in the manifest at
which libraries I use; if they are marked native, GTK say, do the whole layer work, else skip it*; and **the
coordinator's refinement**, the mark read from the code rather than written by hand. The author's instruction before
10:33 replaced it with the checked declaration: the mark is written by the package's writer and the compiler checks it,
so neither a hand mark that lies nor an inference that misses a shape (§ 6.2) decides what a build links. What (S)
measured stays useful: the counts of § 3.1 and the shapes of § 6.2 are what a check must agree with.

## 8. Found beside, not filed

Two defects the lane drafted on 2026-11-25, copied into `ground/` (their `github:` is `none`; a search of the forty newest
issues, `gh issue list --state all --limit 40`, holds neither): `d-lane-layer-speed-1.md` (adjacent: a header one member
reads, planted, stops every program's build, `print(1)` included) and `d-lane-layer-speed-2.md` (blocking: a program
binding `hero_ui.h` does not link on Linux arm64 at `-O0` or `-O2`, `hero_ui_parts_gtk.c:1260` calling `nextafter` with
no `-lm`). Under the principle the first stops only declared programs. The defect that convened this sitting,
`ground/d-layer-every-build.md`, is unfiled too. (The critic widened the search past the forty newest, `gh issue list
--state all --search` for `nextafter`, `cups.h`, `every build`, `platform layer`, `layer`: neither is filed.)

**Two filed defects of the same class, which the first briefs named nowhere** (`gh issue view <n> --json
title,state,closedAt,labels`, re-read by the repair before 11:24):
- **Defect 2788** (closed 2026-11-24, `class:blocking`, `area:runtime`): *"On Windows every program fails to build:
  `hero_ui_parts_gtk.c` has no platform guard"*. It is R6's class, *one member that does not compile fails every
  program on that platform*, `print(1)` included, which the principle bounds to declared programs; it stands beside
  `d-lane-layer-speed-1`.
- **Defect 2687** (open, `class:improvement`, `area:runtime`): *"A macOS program that only digests with `crypto` links
  Security, CryptoKit, Foundation and the Swift runtime, about 3 ms a start, because the layer's crypto member is one
  object"*: what a declared build pays **at run time**, which § 7.5 prices only at build time.
- Task 2593 (open: *"about 151 function-pointer slots in four members, found by `dlsym`"*) is named in § 7.5 already.

## 9. What this folder holds

- `ground/`: the defect (`d-layer-every-build.md`), the lane's routes and tables (`layer-speed-routes.md`), its closing
  text (`closing-d-layer-every-build.md`), the two defects found beside, `route-a.patch`, and the clerk's scripts and
  outputs (`build.sh`, `time.sh`, `time-mac.txt`, `calls.sh`, `clang-wrapper.sh`, `calls-main.log`, `calls-a.log`,
  `shapes.sh`, `shapes-a.txt`, `shapes-main.txt`, `own-headers.sh`, `own-headers.txt`, `own-bound.sh`,
  `own-bound.txt`, `mac-nm.sh`, `mac-nm.txt`, `linux-nm.sh`, `linux-nm.txt`, `closure.py`, `closure-mac.txt`,
  `closure-linux.txt`), all at `80e2e39ad`; and **the repair's, at `c3885b81b`**, each named `repair-*`: `repair-build.sh`
  and `repair-build.txt` (§ 10), `repair-time.sh`, `repair-pairs.sh`, `repair-time-mac.txt`, `repair-pairs-mac.txt`
  (§ 4.5), `repair-calls.sh` and its four `repair-calls-*.log` (§ 4.1), `repair-shapes.sh` and `repair-shapes-main.txt`
  (§ 6.2), `repair-after.sh` (which ran the shapes, read the members taken, ran the probes and built the t1 reference)
  and `repair-members-taken.txt` (§ 6.2), `repair-mac-nm.sh` (`mac-nm.sh` with its folder changed; `linux-nm.sh` ran
  unchanged), `repair-mac-nm.txt`, `repair-linux-nm.txt` (§ 6.1), `repair-own-deep.txt`
  (§ 6.2), and the critic's `reach.py` and `reach_examples.py` with `repair-reach.txt` and
  `repair-reach-examples.txt` (§ 3.1). The scripts name their scratch folder; a seat copies one into its own folder
  and edits the path.
- `blind/`: the blind seat's registration, its first task as repaired (`llm-ergonomist.md`), and `reference-t1/`, the
  four jobs as written under `now` with their run (`results.txt`), never an input of a session.
- `programs/`: `print.hero` (`examples/gallery/00-first.hero`), `clock.hero`
  (`tests/golden/run/the-platform-layer-clock-never-goes-back.hero`), `ui.hero` and `audio.hero` (the lane's).
- `shapes/`: **the ten** tracked programs of § 6.2 and one control that names `hero_net.h` too, with their headers (the
  tenth, `the-platform-layer-ends-a-wait-at-its-sockets-deadline.hero`, with `the-platform-layer-deadline.h`, which
  includes `fixedbugs-2502-waiters.h`); `ownhdr/`, `owndecl/`, `usemod/` and `rebel/` (three of
  `rebel/examples/reach/`'s programs); **the critic's five probes** `constonly/`, `platformhdr/` (made canonical by
  `heroes fmt`, one blank line added), `testonly/`, `face/` and `twofam/`; and `own_deep.py`, the critic's widening of
  `ground/own-headers.sh` to any include depth. The catalogues of `rebel/` and `face/` point at
  `../../../../../rebel/library/0.1.0/files`, the library of the tree that holds them: a seat whose copy of `shapes/`
  stands elsewhere points them at its own copy.

## 10. How a seat works

- **Your own copy**: the detached worktree `.claude/worktrees/260-<seat>` the coordinator makes at the base,
  `c3885b81b` (the llm-ergonomist's area is its own, `llm-ergonomist.md`); **a copy made at `80e2e39ad` before the
  re-freeze is behind by step 182c's walk** and is moved to the base before any build (`git checkout --detach
  c3885b81b`), so a seat builds what it adopts beside the walk and its landing merges against `shadowed.hero` and the
  walk's reads. Build its compiler from the seed, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (the
  repair's at the base, `ground/repair-build.sh`, 11:18 to 11:31 at load 64 to 97: `real 41.32`, `user 22.19`; stage 1
  emitting the base's `selfhost/` with `--emit-c`, 38,004,504 bytes, `real 518.54`, `user 261.84`, `sys 61.55`;
  `clang -O2` of that C, `real 189.69`, `user 117.25`; `heroes --version` of the result: `sources 41f32a8f4b745c0c`,
  the key its emitted C carries; the clerk's at `80e2e39ad`, `ground/build.sh`, read `real 112.68`, `981.04` and
  `417.47` at load 87 to 95), then stage 2, `./heroes build selfhost/main.hero -o heroes-next`: **the seed predates
  steps 184a to 184c and 182c** (*seed not regenerated*, each commit's body), so stage 1 is not the base's compiler.
  For a timing against the seeds, build as `ground/repair-build.sh` does, as the seeds of
  `.claude/worktrees/rebel-scratch/perf/old` (17c50eba6's, `cmp` equal to `git show 17c50eba6:seed/heroes.c`, its 49
  runtime files equal to 17c50eba6's by `cmp`, both re-run by the repair) and `perf/new` (0521cd903's, its 188 equal,
  *carried from the clerk*) were; read those two, never write in them. Never use the trunk's compiler or another
  seat's; never read, build or run inside another seat's folder.
- **This Mac is loaded and will stay loaded**: `uptime` read 87 to 179 during the clerk's runs (the one-minute
  average; `ground/mac-nm.txt` holds the 179.36), with other sessions' lanes and five containers running (`docker ps`,
  10:12), and 64 to 110 during the repair's (four containers of other sessions, `docker ps`, 11:18). Interleave the compilers inside each round, discard the first, read `real`
  against `user` plus `sys`, and report ratios; a duration taken alone is no reading. Nothing a seat does may stop or
  slow another session's process.
- **Linux**: `heroes-linux-arm64:latest` and `heroes-linux:latest` (`docs/platforms/linux/LINUX-MACHINE.md`), with
  `docker run --rm --pull never`, one container at a time, named `p260-<seat>`, removed when idle. Windows is a question
  unless the coordinator's prompt hands you the box.
- **Write your report as you go** to `docs/panel/260-reports/<seat>.md` of your copy and copy it to the same path of
  `/Users/joseph/Temp/heroes/heroes-lang` when you finish or stop, so a killed seat leaves what it had. **Never commit,
  never edit the trunk's tracked files**: the coordinator commits the sitting.
- **Contract**: your verdict (approve, approve with a condition, object, veto), the section it rests on, the cost it
  names, **one falsifiable prediction** with the measurement at which it is checkable, and every place this brief was
  false or silent. Say what you ran and what you read, apart. A veto is a refusal, never a price. The resolution adopted
  is the most robust and complete one, never the cheapest (CLAUDE.md § 4); where robust and conservative disagree, say
  what conservative would have been.
- Never `pkill` by name; stop a process you started by its PID.
