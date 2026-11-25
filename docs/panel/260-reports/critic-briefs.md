# Panel 260, the completeness critic: first reading (the briefs, before any seat starts)

Written 2026-11-25 from 10:49 on, as I go. My copy: the detached worktree `.claude/worktrees/260-critic` at `80e2e39ad`
(`git log --oneline -1`). Long output in `.claude/worktrees/260-critic-scratch/`. Load read at the start: `uptime` 93.77,
106.55, 114.88; four containers of other sessions running (`docker ps`, 10:49).

Every line below says **ran** (a command I ran, with it) or **read** (a file I opened). No route is judged.

## Status

**Done 11:14.** No route judged. No container left (`docker run --rm`, one at a time, `p260-critic`); no paid run, no
commit, no GitHub write. Scripts and outputs: `.claude/worktrees/260-critic-scratch/` (`build.sh`, `time.sh`,
`shapes.sh`, `reach.py`, `reach_examples.py`, `own_deep.py`, `nm/`, `lnx/`, `probes/`, `probes-a/` and their `.txt`).

## A. The base moved under the briefs (the coordinator's message of about 10:55, checked)

- **Ran** `git log --oneline -8 main` (10:56): main's head is **`630f9a105`**, the merge of lane blocking-small2, whose
  step **182c `cb4d420b7`** (authored 10:50:54) sits on `80e2e39ad`; `git merge-base --is-ancestor 80e2e39ad main` holds.
  The briefs' base, `80e2e39ad`, is therefore not `main` any more, and `00-shared.md` § 4.2's column `main` is the
  base's compiler, not main's head.
- **Read** `git show cb4d420b7`: `cli/include_stands` walks, **before any clang, the runtime's opening and each group's
  header** under its unit's words, every `#include`, `#include_next`, `#import`, `#embed`, `__has_include` and a
  `#define`d include read as clang finds it (the includer's folder, then the search `clang -E -v` prints), a block under
  `#if 0` passed over and **every other `#if` read as true**; it asks **`clang -E -v` once per unit's words**. Its body's
  cost, *carried, not re-run by me*: warm builds of a program with one group **and of one with none**, 6 alternated pairs
  at load 7.3, a median **+0.06 s of user time** (1.52 to 1.64 s without), noise 0.005 s. So **a program that binds no
  layer and no group pays it** (the body measured the program with none). Touched: `cli/include_stands.hero` (366),
  `include_lines.hero` (212), `include_told.hero` (99), `header_stands.hero`, `shadowed.hero` (its call at
  `shadowed.hero:120`), two `unsupported` cases (`git show --stat=200 --format= cb4d420b7`).
- **What it changes for the sitting** (each a repair below): (i) the compiler-engineer is asked for *"`print(1)` at
  `old`'s cost, warm and cold"*: 182c's walk and its `clang -E -v` are paid by every build whatever the layer, so the
  target must say whether they are inside it (0.06 s against `old`'s 0.20 to 0.29 s warm is about a quarter, an
  arithmetic on numbers taken at different loads, an inference); (ii) **the walk is a sixth reading of *binds the
  layer* nobody listed**, already built and run before the first clang: it reaches a member's header through a header
  of the program's own (§ 6.2's nine) at no further clang call, and it reads every `#if` but `#if 0` as true, so a check
  resting on it sees a door under a never-taken `#if` (the ffi-pragmatist's fifth door) and would demand a declaration
  there, the loud direction, which collides with § 7.4's lean that a declaration binding nothing is an error;
  (iii) seats whose copies stand at `80e2e39ad` build what they adopt beside a walk their copy lacks: their landing
  merges against `shadowed.hero` and the walk's reads.

## B. The framing facts, re-run (each held unless it says otherwise)

### § 3.1, § 4.1 and the tree's counts (ran in my copy, 10:51 to 10:56)

- **§ 4.1** `ls runtime/platform | wc -l` 72; 43 `.c`, 6 `.swift`, 23 `.h`; `git ls-tree --name-only 17c50eba6
  runtime/platform/` names `hero_net.c` and `hero_time.c`; `grep -c '^#include "platform/' runtime/runtime.c` 72, five
  `hero_layer_*.h` above line 394, and `awk 'NR>=394 && ...'` 67 (43 `.c`, 18 `.h`, 6 `.swift`). **Held.**
- **§ 3.1** 41 of 43 members have `runtime/<member>.h`, the two without `hero_ui_print_gtk` and `hero_ui_print_win32`
  (their headers in `platform/`). The grep over 43 member names: **54** of 9,511 tracked `.hero` files; 32
  `rebel/library`, 20 `tests/golden/run` (one under a `.case/`), `examples/server/net.hero`,
  `tests/harness/rebel_apps_run.hero`. Rebel: 32 floors in 21 top-level packages, 37 files naming some `hero_*.h`, the
  5 others naming `hero_os.h` alone (`os/hero_os.hero`, `path/hero_os.hero`, `threads/hero_os.hero` and two
  `internal/fixture.hero`). `tests/golden/run`: 1,206 tracked, 162 under 67 `.case/` folders, 1,044 at the top, 99
  naming a `hero_*.h`, 19 a member's. **Held.** No group line is indented and every group string list is a header then
  `link` or `package` (`grep -nE '^[[:space:]]+extern "'` prints nothing), so `^extern "` misses no group.
- **Member headers hold no conditional but their guard** (panel 234's R6 condition): no `runtime/<member>.h` holds more
  than one `#if`/`#ifdef`/`#ifndef`/`#elif` line (a loop over the 41). **Held.**
- **The compiler-engineer's table**: every `wc -l` matches (97, 98, 275, 349, 400, 283, 587, 252 and 409, 525, 341 and
  315, 855 and 359); every cited line lands on what the brief says (`sed -n <n>p` of each of the 41 citations of
  § 4.1, § 5 and the table: `produce.hero:156`, `layer_reach.hero:49/72/97/248`, `layer.hero:71/86/94/127/154/161/183/
  255/305/330/422/465/576`, `reached_each.hero:38/163/235`, `opening.hero:171/291/326`, `value_run.hero:194`,
  `assemble.hero:263`, `link.hero:50/342/432`, `link_report.hero:77`, `supplied.hero:148`, `runtime_key.hero:69/107/
  129`, `externs.hero:70/94/116`, `toolchain.hero:398`, `units.hero:470`, `produce.hero:407`, `verbs.hero:289`). One
  slip: **`selfhost/cli/supplied*.hero` is *"357 to 457"* in the table; the twelve files span 145 to 457**
  (`supplied_versions.hero` 145, `supplied_windows.hero` 150; 357 is `supplied.hero` alone). The selfhost groups:
  `hero_os.h` 23, `hero_compiler.h` 10. **Held.**
- **§ 5's issues** (`gh issue view <n> --json title,state`): 2289, 2625, 1399 (defect 134), 1786 (defect 443), titles as
  quoted, all closed. **Held.**
- **§ 3.2, § 3.3's quotations** (`sed -n`): spec lines 35 to 36 and 42 to 44, line 401; design.md 503 to 506, 925, 934
  to 938, 2842 to 2852, 3794 to 3798; `spec-shape.md:67-69`; `records.md:179`. Spec: 516 lines, 34,437 bytes, sha256
  `c915f0f2...2bee` (`wc -l -c`, `shasum -a 256`). **Held.** `measure` waits for my compiler (below).

### § 6.1, the nm closure, both platforms (ran)

- **macOS arm64** (Apple clang 21.0.0, 10:55, load 64 to 96): `ground/mac-nm.sh` with only its paths changed to my copy;
  43 objects, 0 `.err` non-empty, `closure-mac.txt` equal to the clerk's line for line: **9**, all `hero_audio` to
  `hero_audio_coreaudio`. **Held.**
- **Linux arm64** (`heroes-linux-arm64:latest`, image `338a1044ac90`, `docker run --rm --pull never --name p260-critic`,
  Debian clang 22.1.8, container load 3.1): `ground/linux-nm.sh` unchanged over a copy of my runtime: 43 compiled,
  **107**, `closure-linux.txt` equal to the clerk's. **Held.** The container's clock is UTC (its file says 09:55 at 10:55
  here; the clerk's 09:13 is 10:13 local, not a run before the sitting).
- **What `closure.py` is**: a textual walk of the runtime's quoted includes that **ignores every `#if`** (its `inc`
  regex), the same walk route (a)'s `layer_reach.closure` makes over `runtime_key.made_of` (read in `route-a.patch`,
  which `git apply --check` accepts at the base, 105 lines, `cmp` equal to `rebel-scratch/panel260/route-a.patch` and
  to `git -C .claude/worktrees/lane-layer-speed-a diff`, that lane at `0097b3f26`). So the 9 and 107 are what (a)
  misses, not what a `clang -M` closure would miss; on Linux the walk lists the AppKit members under `hero_ui.h`, which
  `-M` would not reach. A seat that prices a `-M`-based closure must re-count.

### What the tree holds beside § 3.1, nobody counted (ran)

- **Whether the declaration travels decides who pays, and nobody counted how far it travels.** Rebel's packages reach
  each other by plain `use` from the library's root (`use time` in 306 modules, `use bytes` in 504; `grep -hE '^use '`;
  0 modules write `use <`). Following `use` lines from each face (`260-critic-scratch/reach.py`, module-level `use`
  lines only): **78 of Rebel's 115 faces, and 735 of its 1,914 modules, reach one of the 32 member floors**; `json`,
  `strings`, `bytes`, `numbers`, `unicode` and `path` do not; `time` reaches `time/hero_time` and five `os/` floors and
  `io/hero_stream`; `toml` and `csv` reach `io/hero_stream` (`reach.txt`). Of `rebel/examples`' 43 folders with a
  catalogue, **30 reach a member floor through a package and none binds a member's header itself**
  (`reach_examples.py`, `reach-examples.txt`). So if the declaration travels by `use` and a declared build pays R6
  whole, a program reading a TOML file pays the layer's `-M` and its cold archive though its link takes no member: the
  author's *"paying more when you use libraries that need the layer is fine"* read against what *use* means is a
  question the seats must answer with this count, not without it.

### § 3.1 and § 6.2's *nine*: **ten** (ran)

- `ground/own-headers.sh` and `own-bound.sh` re-run on my copy print the clerk's files byte for byte (`diff`). **But
  both read one include deep.** Widened (`260-critic-scratch/own_deep.py`: every tracked C-family file outside
  `runtime/` and `seed/`, `#include`, `#import` and `#include_next`, followed through tracked files to any depth;
  output `own-deep.txt`): **a tenth `run` golden binds the layer only through a header of its own, two includes deep**:
  `tests/golden/run/the-platform-layer-ends-a-wait-at-its-sockets-deadline.hero` (step 83a, `2fe8b450e`) names only
  `the-platform-layer-deadline.h`, whose line 12 includes `fixedbugs-2502-waiters.h`, which includes a member's header
  (`grep -n '#include'`); it binds seven `deadline_*` functions, none `hero_*`. A second depth-two header,
  `the-platform-layer-ask-stop.h`, is reached only by a golden that also names `hero_net.h`. So § 3.1's *"9 more"*, § 6.2's
  *"nine"* and its table row, the compiler-engineer's item 4 and the ffi-pragmatist's item 1 all owe **ten**, and the
  nested-include shape the critic brief names is **in the tree, not in `shapes/`**: under route (a) it would be the
  ninth *internal error* (an inference from its eight siblings; unrun).
- No tracked header outside `runtime/` and `tests/golden/run/` reaches a member's header at any depth (the same
  script; the other hits are `tests/emission/*.c`, emitted C). No tracked file spells a member's header with a path
  (`runtime/`, `../`): every include names it bare. **Held**, widened.
- The runtime's own headers reach none: `heroes_runtime.h` includes only `<stdbool.h>`, `<stddef.h>`, `<stdint.h>`;
  `hero_os.h` and `hero_compiler.h` only `heroes_runtime.h`. So an opening-reach reading does not see every program.
  Members' headers include each other (`hero_audio.h` includes `hero_net.h` and `hero_stream.h`; 14 more such edges,
  and the arms' headers `platform/hero_*_arm.h`): a program binding `hero_audio.h` reaches `hero_net.h` too.
- **`header_needs.hero` reads only after a failed link** (its doc, lines 1 to 45: *"a name the link reports is this
  reader's only where ..."*, told `ffi_missing_link` *"no group in this program says which library has it"*). So the
  *"names a group's header bodies call"* reading of § 6.2 is a **post-link** reading in today's code, and under the
  principle an undeclared program's failed link would reach this reader first: the false *"no group says which library
  has it"* the prototype printed for `ownhdr` is what today's code tells an undeclared program unless the new
  diagnostic is asked before it. § 5's last bullet names the class of risk; no brief names this reader as the one that
  would answer.

### The shapes beside § 6.2 nobody ran (read; none run, my build was not ready)

`shapes/` holds none, one family (`clock`, `audio`, `ui`), a used module, one-deep own headers, a package floor. Not
there, each a question for the seats: **two families in one program** (`hero_time.h` and `hero_net.h`, or `hero_audio.h`,
which reaches `hero_net.h` itself); **nested own headers** (the tenth golden above, tracked); **a face over a floor**
(`use <rebel>/time`, not `<rebel>/time/hero_time`: `shapes/rebel` reaches the floors directly, so question 5's
importer is unrun); **a package importing a declared package** (`toml` over `time`, above); **a group naming a member's
header that binds only a constant or a type**, or whose functions are called only inside `test` blocks (a declaration
the link never uses, § 7.4's case from the other side); **`heroes test`** of a floor, **`heroes check`**, **`heroes run`**,
**`--emit-c`** and **the value probe** (`value_run.hero:194`), each a verb the check must answer the same way;
**a platform that takes a different member** (`hero_ui.h`: Swift AppKit members on this Mac, `hero_ui_gtk` on Linux,
Win32 on Windows), so a reading that rests on the link's names answers per platform; **an MSVC target**, unrun anywhere
without the box; **a Swift member's C entry**.

### The blind registration (read)

- `task-t1.md`: sha256 `3076b3d7...080c` and 2,401 bytes (`shasum -a 256`, `wc -c`), as registered. It names the three
  headers alike (*"The language provides three C headers"*, each *"declares `int64_t ...(void);`"*), and no word of
  it says which needs a declaration (`grep -niE 'panel|defect|rebel|seat|critic|coordinator|author|clerk|layer|native|
  declar|member|archive'` finds only *"declares"*, used of each header alike). The three functions exist with those
  signatures (`hero_time.h:40`, `hero_os.h:339`, `hero_net.h:193`). **No project name leaks** beyond the language's.
- **W1 cannot be written in Heroes alone.** *"the nanoseconds since the first call to it in this process (0 at that
  first call)"* needs state that outlives a call, and spec line 154 says *"There are no mutable globals"*, a constant's
  body *"computes over literals and other constants"*. A reader must either write a **C header of their own** holding a
  `static` and a `static inline` over `hero_time_monotonic_ns`, which **is the own-header shape of § 6.2**, or answer
  something else (counted *other*). So W1 does not measure *"a package over `hero_time.h`'s clock"*: it measures the
  hardest shape, and the grader's table does not say whether a group naming the reader's own header owes the
  declaration. Either W1 is rewritten to need no state (a `mark()` and an `elapsed_ns(since:)`), or the registration
  says it is the own-header job and the grader's reading answers it per arm.
- **The two layer headers the task uses are the two the spec names** (`grep -n 'hero_[a-z_]*\.h' spec/heroes-spec.md`:
  line 401 alone, naming `hero_os.h` as *"the language's"* and `hero_net.h`, `hero_time.h` as *"its platform
  layer's"*). So the run cannot tell a text that lists the layer's headers from one that gives a reader a rule for the
  39 others (`hero_audio.h`, `hero_crypto.h`, ...): the spec-warden's question 3 has no blind measurement. A W4 over a
  member's header the spec does not name, described alike, is what would measure it.
- **Graded two ways by two graders**: *declared-right* is *"in the home the session's arm names and nowhere else"*, but
  W1 is two files (the package and the program with its catalogue); the grader's table has one column for W1. A reader
  who also declares in `w1/main.hero`, or puts a catalogue key in `w1/heroes.toml` rather than `w1/ticks/`'s, is right
  for one grader and wrong for another until the table names the file each job owes, relative to the job's folder.
- **The session brief's file name**: panel 256's `brief-session.md` (read whole, 281 words) names the spec
  `heroes-spec.md` and gives four fields (`answer`, `choice_points`, `prediction`, `context`); the registration's folder
  holds `spec.md` and t1 asks a `confidence` per job. Panel 257's adaptation, which `llm-ergonomist.md` says changed
  these, is *carried, not re-read*: the runner's `registration-runner.md` diff must show both changed before any session.
- Panel 257's price, *"spent 6.297764 USD in all"*: `sed -n 10,14p` of its report. **Held.**

### § 4.2, § 4.3, § 4.4, § 8 and § 10's carried and counted facts (ran)

- **§ 4.2's table** is the arithmetic of `ground/time-mac.txt`: each cell the best `real` of rounds 2 to 4 with its own
  `user`+`sys`, each ratio to `old` (`old` 0.36/0.29, 1.02/0.75; `new` 4.77/2.84, 42.05/28.49; `main` 2.35/1.70,
  12.38/7.79; `a` 0.45/0.33, 1.38/0.96; 13.3, 9.8, 41.2, 38.0, 6.5, 5.9, 12.1, 10.4, 1.25, 1.14, 1.35, 1.28). **Held**
  as arithmetic; my own interleaved row is in § D below. § 4.1's call logs sum to 1.774 s (1.412 the `-M`) and 0.46 s.
- **§ 4.3**: the lane's Linux arm64 table, `ground/layer-speed-routes.md:119-120`, reads `old` 0.20, `main` 1.42, `c1`
  1.36, `a` 0.23 warm and 0.81, 7.28, 8.88, 1.29 cold: **held** as a quotation; its runs are carried, and so is the CI.
- **§ 4.4**, carried: the three `unsupported` cases with a `directory` file exist (`git ls-files | grep /directory$`).
  *"about ten fresh `build/selftest-*` folders"* is not checked (242 distinct `build/selftest-` literals stand in
  `selfhost/`, not all of them the layer's). **The one conclusion that rests on § 4.4 alone** is § 4.4's last sentence,
  *"What the net pays is the warm cost"*, which decides whether (P1)/(P2) buy the net anything; a seat that weighs them
  owes a reading of where the harness starts its compilers.
- **§ 10**: `perf/old/seed/heroes.c` `cmp`-equal to `git show 17c50eba6:seed/heroes.c`, and its 49 runtime files
  equal 17c50eba6's 49 (`git show ... | cmp -s`). The three commits' bodies each say *"Seed not regenerated"*, and 184c
  renames `<listing>-rule` to `<listing>.rule` and nothing else (`git show`). **Held.**
- **§ 8**: no issue holds the two side defects (`gh issue list --state all --search` for `nextafter`, `cups.h`,
  `every build`, `platform layer`, `layer`, 8 each). **Held**, widened past the forty newest. **Two issues that search
  surfaced, which no brief names**: **defect 2788** (closed 2026-11-24, blocking: *"On Windows every program fails to
  build: `hero_ui_parts_gtk.c` has no platform guard"*), the class R6 makes of every member that fails to compile and
  the principle bounds to declared programs, beside `d-lane-layer-speed-1`; and **defect 2687** (open, improvement:
  *"A macOS program that only digests with `crypto` links Security, CryptoKit, Foundation and the Swift runtime, about
  3 ms a start, because the layer's crypto member is one object"*), what a declared build pays at run time, which
  § 7.5 prices only at build time. Task 2593 (open: *"about 151 function-pointer slots in four members, found by
  `dlsym`"*) is named in § 7.5, as it should be.

### The negative sentences (ran)

- **§ 7.5 (f), *"`grep -n 'mtime\|modif\|stat(' runtime/hero_os.h` finds no door for a file's time at the base"*: the
  grep is right and the conclusion is FALSE.** `runtime/hero_os.h:209-211`: *"1 when `path` is newer than `reference`,
  0 when it is not, -1 when either is not there"*, `int64_t hero_fs_newer_than(const char *path, const char
  *reference);`, defined at `runtime/parts/fs.c:226` by `stat` (seconds, then `st_mtimespec`'s nanoseconds on Apple,
  `st_mtim` where defined) and `GetFileAttributesExA` on Windows, both of which take a directory. So the lane's carried
  reason for setting (f) aside, *"it needs a stat the runtime does not offer"*, does not hold as written: a door that
  says whether a directory is newer than a record file exists. Whether every file system moves a directory's time on
  every entry stays (f)'s question.
- **§ 3.2, *"No package-side file in Rebel"***: `find rebel/library -name heroes.toml` prints nothing, **held** for a
  catalogue and for a manifest; every package does carry a `.ledger` (185 tracked), which is read by seven harness
  modules (`rebel_*.hero`, `suite_rebel.hero`, `suite_records.hero`) and **by no build**: `selfhost/fetch/closure.hero`
  leaves *"its ledgers and cards"* out of a placed package, *"since no build reads one (Rebel ships no C, panel 253's
  fact (1))"*. No Rebel module writes `use <` (`grep -lE '^use <'`, 0), so no Rebel package needs a catalogue of its
  own today; spec § 1's *"the package's own catalogue"* stands unexercised by the tree.
- **§ 6.2, *"no tracked header outside `runtime/` and `tests/golden/run/` includes one"***: held at any depth (§ B
  above).

## C. Homes, readings and routes nobody listed (read; each a question, none judged)

**Homes (§ 7.1)**
- **The floor's own file name.** All 32 floors are named `hero_<name>.hero` (§ 3.1, my `grep`), and the 5 files binding
  `hero_os.h` alone are too. A rule *"only a module named so may bind a layer header"* is a declaration where the code
  is defined that costs no syntax; Go's file-name build constraints are the precedent to source. Not in § 7.1.
- **A `use` of the layer itself in the binding module** (Go's `import "C"` is this shape: the file that binds imports a
  pseudo-package). § 7.1's *"a word on a `use`"* sits under the face module; a `use` of the layer in the floor that
  binds is a different home, and travel then follows `use` with no new rule.
- **The `link` word the group already has**, naming the layer as the library that has the symbols (`extern
  "hero_time.h" link "<the layer>"`): no new grammar; spec § 13 already says *"`link` a library when the symbols need
  one"*. **Panel 234's R6 refused exactly this** (*"found and linked by the compiler with no `link` word or flag in a
  package"*, the ffi-pragmatist's condition on P2, `docs/panel/234-*.md:418-419`): **no brief quotes that condition,
  and the principle reverses it**; the ffi-pragmatist must say whether its seat's own earlier condition still binds.
- **Where the check runs**: § 7.2 names `check` and `build`; `heroes add` and `heroes fetch` (`selfhost/cli/
  table.hero:200-201`) place a package and could refuse an undeclared one before any build. Unlisted.
- **The catalogue's own refusal list**: `.claude/rules/cli-surface.md:73-76`, *"a compiler flag, a search path, a
  library or package of spec § 13, a language version, a mode or the program's own name is refused by the reader,
  never ignored"*. § 3.2 quotes the catalogue's *"and nothing else"* but not this clause, which is the one a
  catalogue key *"needs the platform layer"* meets head on (it names a § 13 library, or is a mode).

**Readings of *binds the layer* (§ 6.2, § 7.2)**
- **Step 182c's walk** (§ A above): before any clang, every include of every group's header to any depth, at no
  further clang call; read from its commit body and not run: it would see the ten and `ownhdr`, not `owndecl`; it reads every `#if` but `#if 0` as true.
- **The undefined names of the program's own objects, read by `nm` before the link.** Distinct from *"the link's
  missing names"*, which needs a link that fails: one `nm` per object, the objects the build already made, the reader
  already in the code (`layer.defined_by`, `layer.taken`, `layer.hero:255` and `:305`, read an archive so). It sees
  every door whose reference lands in the program's objects (the ten's `static inline` bodies, `owndecl`, `ownhdr`, a
  macro, a function pointer taken in a header); it does not see `dlsym`, nor a `link` library's object calling the
  layer (that is `cli/library_needs.hero`'s, defect 2365). Unlisted.
- **A group naming a header under `runtime/platform/`**: `extern "platform/hero_audio_arm.h"` names no member's header
  (its stem is no member), yet declares functions `hero_audio_coreaudio.c` defines. Its comment says *"no Heroes group
  names it"* (`runtime/platform/hero_audio_arm.h:17`); **nothing in `selfhost/check` or `selfhost/cli` refuses it**
  (`grep -rln 'platform/' selfhost` names only `layer*.hero` and `runtime_key.hero`). The groups' header-name reading
  misses it unless `platform/` is the layer. Run in § D below.

**Routes (§ 7.5)**
- **The declaration names the families it needs** (`hero_time`, `hero_net`), so a declared build archives those and the
  members they reach, the closure of § 6.1 then the compiler's from a family table, and a link short of a name told as a
  family undeclared. Unlisted between R6 whole and (a).
- **The layer as a shared library, built once per machine and toolchain**: a constant link cost and no archive per
  build folder; what would have to be true is that a Heroes binary may depend on a library at run time, which
  `.claude/rules/records.md:179`'s *"no binary is uploaded"* does not settle. A question, unlisted.

## D. Runs under the base's compiler (and route (a)'s prototype), this Mac

**My compiler**: `260-critic-scratch/build.sh`, `ground/build.sh`'s route in my copy: stage 1 by clang on the seed
(10:50:20 to 10:51:08), stage 1's `--emit-c` of `selfhost/` (37,850,570 bytes, `real 532.27`, `user 271.30`, `sys
65.21`), `clang -O2` of it beside my runtime (`real 222.75`, `user 113.27`), done 11:03:43 at load 97. `heroes
--version`: `sources 2c4d979d320af5a3`, **the same key as the clerk's `main`** (`panel260-scratch/main-o2/heroes
--version`). Not stage 2 by `heroes build selfhost/main.hero -o heroes-next`: the `-O2` build of stage 1's emission is
the compiler `ground/build.sh` names `main`, and its key says it is built from the base's sources.

- **§ 3.3** `measure spec/heroes-spec.md` from my copy's root: claude-legacy **9,877**, cl100k_base **10,054**, real
  **13,185** (`claude-opus-5, 2026-11-23`, no `STALE`), headroom **3,199**, **13,245** against the ceiling. **Held.**
- **§ 4.2, one row re-run** (`260-critic-scratch/time.sh`, `ground/time.sh` with its folder changed, `programs/print.hero`,
  `old` = `perf/old/heroes` beside its runtime, `main` = mine; a prime, then four rounds each warm then cold, the two
  compilers interleaved; 11:04:07 to 11:05:20, load **97.5 to 104.6**; `time-mac.txt`). Rounds 2 to 4, best `real`
  (its `user`+`sys`): warm `old` **0.41 (0.28)**, `main` **2.10 (1.77)**, ratio **5.1** real, **6.3** user+sys; cold
  `old` **0.86 (0.73)**, `main` **10.28 (7.50)**, ratio **12.0** and **10.3**. The clerk's 6.5 and 5.9 warm, 12.1 and
  10.4 cold: **held** within the spread of two runs at load 100 (the warm `real` ratio moves 5.1 to 6.5 between them,
  so a seat reports a range, not a figure).
- **§ 6.2 under `main`** (`260-critic-scratch/shapes.sh`, `ground/shapes.sh` with its folder changed and
  `shapes/rebel/heroes.toml`'s `path` pointed at my copy's library; 11:05:28 to 11:08:14, load 91 to 105): **all 21
  exit 0** (the clerk's 20 and the tenth golden). **Held**, and the tenth is one more R6 builds.
- **My probes under `main`** (`260-critic-scratch/probes/`, sources kept; `probes-main.txt`; each built, the safe ones
  run; members taken read by `nm bin | grep -c ' T _hero_<family>_'`):

| probe | what it is | `main` | members taken | route (a), the lane's `seed-a/heroes` |
|---|---|---|---|---|
| `platformhdr` | a group naming `platform/hero_audio_arm.h`, binding `hero_audio_arm_permission` | **exit 0** | audio 39, net 36 | **exit 1**, `ffi_missing_link` *"no group in this program says which library has it"* (false: the layer has it) |
| `twofam` | `hero_time.h` and `hero_net.h` in one program | exit 0, prints `true`, `0` | time 5, net 36 | exit 0 |
| `constonly` | a group naming `hero_net.h` that binds only `constant HERO_NET_INVALID` | exit 0, prints `1` | **none** | exit 0 |
| `testonly` | a used module whose clock binding is called only in a `test` block; `main` never reads the clock | exit 0, prints `42`; `heroes test clockmod.hero` 1 passed | **time 5** | (not run) |
| `face` | `use <rebel>/time`, calling `time.mark()` only | exit 0, prints `true` | **time 5, net 36, disk 35** | exit 0 |
| the tenth golden | own header two includes deep | exit 0 | (not read) | **exit 2**, *internal error: linking failed* |

**What these say, for the readings** (an inference from the table, the readings not built):
- **`constonly` is a correct program the readings disagree on.** The header-name reading, the opening reach and the
  bound-name reading say it binds the layer (it names `hero_net.h`, and `HERO_NET_INVALID` is a layer header's name);
  the `nm` and link readings say it binds nothing (no member taken). A check that reads *binds* by name and *binds
  nothing* by the link refuses it **undeclared and declared alike**: the case *"no correct program refused"* must be
  run on.
- **`testonly` and `face`: what a link takes is not what a program calls.** `emit/extern_refs.hero` holds a `used`
  reference to every function a module's unit calls through a group (its lines 1 to 16; panel 202's R2, defect 562),
  a `test` block's call included, so under R6 a program that uses a module takes every member that module's bindings
  name: `time.mark()` alone takes `hero_disk` (35 symbols) and `hero_net` (36). design.md §1.11's *"whose members the
  linker takes into a program only where the program calls them"* (line 505) is, measured, *where a module the program
  reaches binds them*. A reading of *binds the layer* that follows the link inherits this.
- **`platformhdr`: a door no header-name reading sees**, builds under R6, and under (a) is told the same false
  `ffi_missing_link` as `ownhdr`.

**Not run**: these shapes on Linux arm64 and x86-64; `--emit-c`, `check`, `run` and the value probe on them; any
`-O2` or `--sanitize` build of them; step 182c's cost (main's head, `630f9a105`, not built by me).

## E. What each seat was handed

- **The historian has the web and no shell**: nothing in its brief needs a shell. **But the ffi-pragmatist's item 7
  sends *"pkg-config's `Libs.private`, libtool's `.la` `dependency_libs`"* to *"the historian's sources"*, and the
  historian's brief has no such item** (its ten questions are Cargo, cgo, Nim, Zig, Java, Dart, npm, SwiftPM, Python,
  and Cabal/Deno/`no_std`). Go's file-name build constraints (above) are not in it either.
- **The ffi-pragmatist has a shell and no web**: its item 7's question is correctly sent away; nothing else in its
  brief needs the web.
- **The spec-warden is asked to predict D1 to D4 *"for the text the sitting adopts and for its rival"*** before either
  exists and before t2 is written; it can predict only for its own texts. The registration should say how a
  prediction for a text not adopted is read.
- **No seat is handed § A** (main moved, step 182c's walk and its cost), **the tenth golden**, **Rebel's `use` reach**
  (78 of 115 faces), **defects 2788 and 2687**, or **panel 234's `link`-word condition**.
- **§ 4.3 and § 4.4 carried**: no conclusion of the briefs rests on § 4.3 alone; § 4.4's *"the net pays the warm
  cost"* does (above).

## F. What I could not verify, or did not run

Step 182c's cost (+0.06 s user, carried from its body; I did not build main's head); § 4.3's container and CI cells and
the lane's `old`/`new` tables (quoted, not re-run); § 4.4's *"about ten"* `selftest` folders; the `new` compiler's
cells of § 4.2 (my row interleaved `old` and `main` only); panel 257's runner adaptation inside
`llm-ergonomist-blind-answers.tar.xz` (not opened); the two side defects' runs; every shape on Linux and every shape at
`-O2` and `--sanitize`; Windows and MSVC, which no one holds.

## What the coordinator must repair before any seat starts

1. **The base moved.** Main is `630f9a105` (step 182c, `cb4d420b7`, merged after the briefs were written). Say which
   base the seats build on; whichever it is, hand every seat § A: 182c's walk of every group's header **and of the
   runtime's opening**, before any clang, with one `clang -E -v` per unit's words, **paid by a program with no group**
   (+0.06 s user per warm build, carried, about a quarter of `old`'s 0.20 to 0.29 s, an inference). The
   compiler-engineer's *"`print(1)` at `old`'s cost, warm and cold"* must say whether that walk is inside the target.
   And list the walk in § 6.2 and § 7.2 as a sixth reading of *binds*, built and run before the first clang, which
   reads every `#if` but `#if 0` as true.
2. **Nine is ten.** § 3.1's *"9 more"*, § 6.2's paragraph and table row, the compiler-engineer's item 4 and the
   ffi-pragmatist's item 1: add `tests/golden/run/the-platform-layer-ends-a-wait-at-its-sockets-deadline.hero` (its
   group names `the-platform-layer-deadline.h`, which includes `fixedbugs-2502-waiters.h`, which includes a member's
   header), with `the-platform-layer-deadline.h`, to `shapes/`; say that `own-headers.sh` reads one include deep. Under
   the base it builds; under (a) it is *internal error: linking failed*, exit 2 (run, § D).
3. **The blind task's W1 cannot be written in Heroes alone** (*"There are no mutable globals"*, spec line 154): its
   *"since the first call ... in this process"* drives a reader to a C header of their own over `hero_time.h`, the
   own-header shape, or to *other*. Rewrite W1 to need no state, or register it as the own-header job and have the
   grader's reading say what it owes in each arm. Split the grader's table by file (the package's module, the program,
   each catalogue) so *"nowhere else"* reads one way. Record that W1 and W3 use the two layer headers the spec already
   names (line 401), so the run cannot tell a listed rule from a rule for the 39 it does not name, or add a W4 over one
   of those. Have the runner's registered diff show `brief-session.md`'s `heroes-spec.md` made `spec.md` and the
   `confidence` field added.
4. **Put the shapes the readings disagree on in `shapes/`** (sources in `260-critic-scratch/probes/`):
   `constonly` (a group naming `hero_net.h` binding only a constant: builds, takes no member; name readings say it
   binds, link readings that it binds nothing, so a name-and-link check refuses it declared and undeclared alike),
   `platformhdr` (`extern "platform/hero_audio_arm.h"`: builds under R6, no header-name reading sees it, nothing in
   the checker refuses it), `testonly` and `face` (the link takes what a reached module's bindings name, test blocks'
   calls included, not what the program calls: `time.mark()` takes `hero_disk` and `hero_net`, `emit/extern_refs.hero`),
   `twofam`. Say in § 6.2 that design.md line 505's *"only where the program calls them"* is, measured, *where a
   module the program reaches binds them*.
5. **§ 7.5 (f) is falsified as written**: `hero_os.h:211`, `hero_fs_newer_than(path, reference)`, compares two files'
   (or directories') times on every platform. Correct the sentence and the carried *"needs a stat the runtime does
   not offer"*.
6. **Give question 5 (travel) its count**: 78 of Rebel's 115 faces, 735 of its 1,914 modules and 30 of
   `rebel/examples`' 43 folders reach a member floor through `use` alone, none of the 43 binding one itself
   (`reach.py`, `reach_examples.py`). Whether a program reading TOML *uses a library that needs the layer* is the
   author's word to apply with that number in hand.
7. **Quote what the principle reverses or meets**: panel 234's R6, *"found and linked by the compiler with no `link`
   word or flag in a package"* (`docs/panel/234-*.md:418-419`), an ffi-pragmatist condition the declaration overturns
   (and the existing `link` word as a home, § C); and `.claude/rules/cli-surface.md:73-76`, *"a library or package of
   spec § 13 ... a mode ... is refused by the reader, never ignored"*, which the catalogue home meets head on.
8. **Say that the bodies' reading runs after a failed link** (`cli/header_needs.hero`'s doc): under the principle an
   undeclared program's failed link reaches it first and today tells the false *"no group in this program says which
   library has it"*, as (a) printed for `ownhdr` and `platformhdr`. The new diagnostic must be asked before it.
9. **The historian's brief lacks what the ffi-pragmatist's item 7 sends it**: pkg-config's `Libs.private` and
   libtool's `.la` `dependency_libs`; and for the unlisted homes, Go's file-name build constraints and cgo's `import
   "C"` as a declaration by import.
10. **Name defects 2788 and 2687**: R6's *every program on a platform fails when one member does not compile* (2788,
    closed) is the class the principle bounds to declared programs, beside `d-lane-layer-speed-1`; 2687 (open) is what
    a declared build pays at run time.
11. **List the unlisted readings and routes as questions**: the program's own objects' undefined names by `nm` before
    the link (`layer.defined_by`/`taken` read archives so already); a declaration that names the families it needs; the
    layer as a shared library per machine.
12. **Small corrections**: `supplied*.hero` spans 145 to 457 lines; `closure.py` and (a) ignore every `#if`, so § 6.1's 9
    and 107 are (a)'s counts and a `-M` closure re-counts; the Linux files' clock is UTC; § 4.2's warm ratio is a range
    (5.1 to 6.5 real across the clerk's run and mine); § 4.4's *"the net pays the warm cost"* rests on a carried fact
    alone; the spec-warden's predictions for a text not yet written need a rule for how they are read.
