# Panel 250, the ffi-pragmatist's report

Written as I go (2026-11-15, from 00:08). Folder: `.claude/worktrees/p250-scratch/250-ffi-pragmatist/`
(below, `F/`), tree `F/tree/` a `git archive` of `48f53b343`. Status: **complete**, 2026-11-15 about 00:50.

## Said first, in the shared brief's words

**The route I would adopt for a package's C, H, changes a message a reader acts on.** A build refuses a
placed tree from which a file that is not `.hero` was taken out (F1's wording below), where today the
whole-tree sum refuses it in other words. It also changes what a program's placed tree holds (every
non-`.hero` file of the version, the unreached modules left out). Like every route that places a subset,
it refuses a `use` written after the fetch until `heroes fetch` runs (00-shared.md's measurement, not
repeated here). It changes no `use` line, no catalogue, and no sentence of spec § 13 (*"A group names its
header"* stays true: the header and everything the package ships beside it are placed).

## Verdict

- `verdict`: **veto** on M as the rule for a package's C. That covers any placement of C files computed
  from a closure, whether clang's `-M` on one platform or level, the groups' header names, a textual
  include scan, or a per-package closure. **Approve** H's rule for C under the two conditions below,
  whatever grain the `.hero` files take (M or G). The veto does not reach placing only the reached
  `.hero` modules.
- `section`: design.md §1.11 (*"verified ... over C, by clang against the header it names"*, the package
  paragraph at its lines 562-581 in the base) and §4.19 (*"`inline` functions and `#define` constants are
  reachable because the C compiler sees the real header"*); §1.12 for what a wrong constant at exit 0 is.
  The document does not cover a placement of part of a package's C; this rests on those sentences.
- `experiment`: nine probe packages (`deep`, `arms`, `embed`, `ask`, `pt`, `ra`, `qt`, `rb`, `lnk`) and three subsets of `core` 0.5.0 in scratch indexes, each subset published as its own version so the
  base's compiler, unpatched, builds the pruned folder. Run on Darwin arm64 and Linux arm64; the Windows
  arm preprocessed by `zig cc -target x86_64-windows-gnu`. clang accepted every whole tree but `embed`
  and `lnk`. Of the subsets, a closure taken by `-M` on the other platform, or at the other level, built
  at **exit 0 against the SDK's `util.h`/`err.h` or glibc's `error.h`/`err.h` with the wrong value**. A
  `__has_include` of a pruned file compiled the other branch. H built every shape with the whole
  tree's values on both platforms, but for one residue.
- `argument`: A package's C closure is not a property of the package. It moves with the platform
  (`arms`: three closures for three targets), with the level (`build` at `-O0` against `run` at `-O2` read
  different files on one machine), and with the including unit (`qt_wide.h` read by `rb`'s unit, not
  `qt`'s). A placement made on one machine is built on another, so any computed closure omits files
  another build reads. When it does, clang finds a same-named header of the toolchain or of a
  `pkgconfig` folder, or answers `__has_include` false, and verifies the binding against a header the
  package never published, at exit 0. That breaks the thesis at the boundary. H needs no closure.
- `prediction`: A package binding libcurl through `examples/curl/curl_bound.h` as its door, with an arm
  on `LIBCURL_VERSION_NUM` reading a file of its own (that header's comment already parts 8.7.1 from
  8.18), placed by M on this Mac (SDK libcurl 8.7.1, `curlver.h`) and built in the Linux arm64 image
  (libcurl 8.14.1, `pkg-config --modversion`), reads another file of the package or none, at exit 0 or
  `ffi_missing_header`. Under H it reads on each machine exactly what the whole tree reads, and no CI leg
  reports `ffi_missing_header` for a file an H-placed package publishes.
- `condition`: The veto on M lifts if a closure rule is shown, built, that places on one machine every
  file any build of the program reads on Darwin, Linux and Windows at every level, `__has_include` targets
  included, without clang at fetch. I know of none, and doubt one exists for C's preprocessor; that doubt
  is an inference, unrun. H's approval holds on two conditions. (1) F1 as worded in § The fingerprint, for C, below: an absent
  non-`.hero` file refused at every build. (2) H's residue closed: a C file of the package naming one of its
  own `.hero` files (`ask`: `1` for `2`, both platforms). The closure I propose, **unbuilt**: `fetch`
  places every `.hero` file of a version any of whose non-`.hero` files holds the bytes `.hero`, so a
  literal name gets the whole tree's build. A name assembled by token pasting would escape it, said.
  Without (2) my verdict on H is **object**.

## Where the brief is wrong, or less exact than the run

1. **The listings' names.** `ffi-pragmatist.md` (*"`build/tu-*/main.deps.txt`"*): a fetched module's unit
   keeps its own, `build/tu-<k>/<n><package><module>.deps.txt` (`1deepa.deps.txt`, `1ptpt.deps.txt`,
   `1rasum.deps.txt`, `ls build/tu-*/`). `main.deps.txt` is the root's unit, and it read no package file in
   any probe here.
2. **Shape 3's premise**, that *"today's build refuses a package file that shadows a C library header"*,
   holds for the headers the runtime's opening reads (`stdint.h`, the 2357 case). It does not hold for a
   package's own `util.h`, `err.h` or `error.h`, names the SDK's or glibc's `usr/include` hold: `arms`
   whole builds at exit 0 on both platforms. So the names that can replace a pruned header are every
   header of the toolchain's directories that the opening does not read, plus a `pkgconfig` group's
   folders. On this Mac `pkg-config --cflags sdl3` answers `-I/opt/homebrew/include`, holding 131 headers at
   its top and 531 at any depth (`find ... -name '*.h' | wc -l`), among them single-file libraries' names a
   package could carry (`blake2.h`, `lz4.h`, `md4c.h`, `llhttp.h`). That substitution through
   `pkgconfig` was **not built**; the SDK and glibc cases show the mechanism.
3. **README § 4 rule 5 and panel 241's R7 are rulings the compiler does not enforce** for a header with
   platform arms (shape 2's question, answered by the run: nothing refuses `arms` on either platform).
4. **"H is M with the cards"** (00-shared.md § The routes) needs its count fixed. H as defined places
   **every** card: for `server`, 7 cards (19 files), where the brief's table counts the 5 reached ones.
5. 00-shared.md's *"Unrun by the coordinator: stage 2's `sources` line in that image"*: now run, equal.

## Findings beside the question, for the coordinator (nothing written to GitHub)

- **A false message**: a fetched header's `#embed "data.bin"` of a file inside its own root is told
  `package_reads_outside` naming the file as outside. Clang (21 on Darwin, 22 on Linux) writes an `#embed`
  target absolute and an `#include` target relative, and C2 compares lexically. The verdict would stand
  (`-Wc23-extensions` is an error in the header region); the message is false. Class by the rules:
  `blocking` (*"a false message"*), yours to file.
- **`ffi_missing_header`'s note under any pruning route** (*"A header it needs ships inside the
  package"*) is false for a file the verb left out; under W and H that half stays true. Its other half,
  *"never ... another package's files"*, is inexact under W already, for a unit reaching another package's
  record, which searches that package's folder (defect 2357; `rb`/`qt` here).

## The compiler

- Seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` in `F/tree`, exit 0, 86,891,621
  instructions retired (`/usr/bin/time -l`, `F/seed.log`).
- Stage 2: `./heroes build selfhost/main.hero -o heroes-next`, exit 0, 916,814,184,063 instructions
  retired; `heroes-next --version` printed `heroes 0.6.0` and **`sources 5bac791363384945`**, the base's
  (`F/stage2.log`). `doctor`: `Apple clang version 21.0.0 (clang-2100.4.1.1)`. Durations are not
  measurements (load average 183.60 at 00:08, `uptime`).

## How a pruned tree was measured: a third way, no patch

The brief offers (1) a compiler patched to skip the sum, (2) the `-M` listings of a whole build. I used a
third: **each subset published as the same name and version in an index of its own**
(`F/probes/index-<v>/`, by README § 10's two shell lines, `F/probes/publish.sh`), placed by `heroes add
<name> --index file://../index-<v>` (`F/probes/probe.sh`). What clang sees at build is the pruned folder
byte for byte; only the `sum` and the key differ, and the build's check then passes honestly, so the base's
compiler, unpatched, measures the C of a pruned tree. Route (1) was not built.

## Runs on Darwin arm64 (heroes-next of the base)

Every output in `F/probes/<prog>/out.txt`; each program built with `heroes-next build main.hero -o fp-bin`,
under `timeout -s KILL 300`, its binary under `timeout -s KILL 60`, output through `head -c 65536`.

### Shape 1, a header including another of the package's (`deep`)

`a.h` includes `inner/deep.h` and `only_a.h` (named by no group); `b.h` (module `b`, unreached) includes
`inner/deep.h` too. Program `use <deep>/a`.

| placed | build | prints | package files in the unit's `-M` listing |
|---|---|---|---|
| whole (6 files) | exit 0 | `42` | `a.h`, `inner/deep.h`, `only_a.h` (not `b.h`) |
| M by Darwin's `-M`: `a.hero a.h inner/deep.h only_a.h` | exit 0 | `42` | same |
| the groups' headers alone: `a.hero a.h` | **exit 1** `ffi_missing_header` | — | — |

The groups'-headers rule is loud here, but its note says *"A header it needs ships inside the package"*:
**false of a tree the verb pruned** (the package shipped it), the class of advice 00-shared.md already
found false in `package_unread`'s message.

### Shapes 2 and 3, platform arms and the build's level (`arms`)

`a.h` includes, all files of the package: `win.h` under `_WIN32`, else `posix.h`; `util.h` under
`__APPLE__`, `error.h` under `__linux__` (names the SDK and glibc also hold: `util.h` is in the macOS SDK's
`usr/include` and not in the Linux image's, `error.h` the reverse, `ls` of each, `F/probes/linux-names.txt`);
`tune_linux.h` under `defined(__linux__) && __has_include("tune_linux.h")`, `tune_apple.h` likewise in the
`#elif`; `err.h` under `__OPTIMIZE__` (in both the SDK and glibc). Defaults `1` where the package's file is
not read.

**The base builds it as a fetched package, and nothing refuses it**: README § 4 rule 5 (*"No package ships
platform C of its own"*) and panel 241's R7 are not enforced for this shape; the package's own `util.h` and
`err.h` are not refused as stand-ins either (the shadow check judges the runtime's opening and the
groups' library headers, not these).

| placed | level | prints (family, os, tune, level) | package files in `-M` |
|---|---|---|---|
| whole | `-O0` (`build`'s default) | `10 300 5000 1` | `a.h posix.h tune_apple.h tune_linux.h util.h` |
| whole | `-O2` (`run`'s default) | `10 300 5000 2` | the same **and `err.h`** |
| **Darwin `-O0` closure**, built `-O2` | `-O2` | `10 300 5000` **`1`**, exit 0 | `err.h` read from **the SDK**: `.../MacOSX.sdk/usr/include/err.h` |
| **Linux closure** (hypothesis, measured in Linux below), built on Darwin | `-O0` | `10` **`1 1`** `1`, exit 0 | `util.h` read from **the SDK**: `.../MacOSX.sdk/usr/include/util.h`; `tune_apple.h` absent, so `__has_include` false |

Two facts about clang's listing the M route would lean on: **`-M` names `tune_linux.h` on Darwin**, where
its `#if` is false at `defined(__linux__)`, because clang looks up a `__has_include` operand even when the
left of `&&` is false; and it names no file of an untaken `#elif` (`error.h`, `win.h`). So the listing
is neither the files a build reads on another machine nor a superset of them.

**This is the veto's ground, measured**: a pruned tree whose C closure was taken on one platform, or at
one level, builds on the other at exit 0 against a header its package did not publish (the SDK's `err.h`,
the SDK's `util.h`), or against another branch of its own header (`__has_include` false), and prints a
wrong constant with nothing said. The same program reads different package files under `heroes build`
(`-O0`) and `heroes run` (`-O2`) on one machine.

### Shape 4, `#embed` and `__has_include` (`embed`, then `ask`)

**`#embed` of a file inside the package's root is refused at the base, with a false message**
(`F/probes/prog-embed-w/out.txt`): `error[package_reads_outside]: a unit of the package at
heroes.packages/embed@1.0.0-5baba16d5189bd25 reads /Users/.../prog-embed-w/heroes.packages/embed@1.0.0-5baba16d5189bd25/data.bin,
outside its root`, exit 1. The file is inside the root. Clang's raw `-M` of the same header (`clang
-std=gnu11 -I <key> -M`) writes the `#include` and `__has_include` targets relative
(`heroes.packages/embed@.../a.h`, `.../b.hero`) and **the `#embed` target absolute**, and C2 compares paths
lexically (`package_reads.hero`'s comment, *"Paths are judged as written"*), so an absolute path under a
relative root reads as outside. Without C2, the header region would refuse it anyway: `-Wall` compile of
the header prints `warning: #embed is a C23 extension [-Wc23-extensions]`, and panel 245's R1 makes every
non-`-Wall` warning in a header an error. So **the verdict is right and the message is false** (it tells
the author the header reads outside its root). A defect beside the question, for the coordinator to file;
I wrote nothing to GitHub. Pruning a file `#embed` reads is moot at the base, since no fetched header can
`#embed`.

`ask` (no `#embed`): `a.h` asks `__has_include("asked_only.h")` (never included), includes `table.inc`,
and asks `__has_include("b.hero")`, `b` an unreached module.

| placed | build | prints (asked, table, reads_b) | package files in `-M` |
|---|---|---|---|
| whole | exit 0 | `2 77 2` | `a.h asked_only.h b.hero table.inc` |
| **H**: reached `.hero` + every other file: `a.hero a.h asked_only.h table.inc` | exit 0 | `2 77` **`1`** | `a.h asked_only.h table.inc` |
| a textual rule following `#include` only: `a.hero a.h table.inc` | exit 0 | **`1`** `77` **`1`** | `a.h table.inc` |

So: **clang's `-M` names every file `__has_include` found**, a `.hero` file included, and an M by `-M`
taken on the build's own platform and level would place `b.hero` though no `use` reaches it. **H has one
measured residue**: a C file of the package naming one of its own `.hero` files. A pruned tree then
answers `__has_include` false and compiles another branch of the package's own header, silently. No file
foreign to the package is read, but the build is not the published one.

### Shape 5, a third package's record (`ra` reaches `pt`'s `Point`)

`pt.h` includes `pt_types.h` (the field type); `pt` also holds `extra.hero`/`extra.h`, unreached. Program
`use <ra>/sum`; `heroes add ra` placed `pt` for it (*"added pt 1.0.0 for ra"*).

| placed | build | prints | which units read `pt`'s files |
|---|---|---|---|
| whole | exit 0 | `3` | `1ptpt` and `1rasum` each read `pt.h`, `pt_types.h` |
| M: `pt.hero pt.h pt_types.h`, `ra` whole | exit 0 | `3` | the same |

So when only the record is reached, the placement holds the module declaring it (it is reached by `ra`'s
`use <pt>/pt`), its group's header and that header's own reads, as `ra`'s unit reads them from `pt`'s
folder after its own route (defect 2357). Unrun: a reaching unit whose own header defines a macro that
changes what `pt.h` includes. **Built afterwards** (`F/probes/prog-rb-w`, `prog-rb-q`): package `qt`'s
`qt.h` reads `qt_wide.h` under `#if defined(QT_ASK_WIDE)`; package `rb`'s own group header `rb.h` defines
`QT_ASK_WIDE`, and `rb` holds `qt.Point` by value. Whole: exit 0, `103`; **`qt`'s own unit's `-M` names
`qt.h` alone, `rb`'s unit names `rb.h`, `qt.h` and `qt_wide.h`**. So a package's C closure is a function of
the unit that includes it, not of the package. `qt` placed by its own closure (`qt.hero qt.h`): exit 1,
`error[ffi_missing_header]: qt_wide.h is not found where a unit of the fetched package at
heroes.packages/rb@... looks`. That is loud, but it names the wrong package (`rb`, where `qt` shipped
the file), and its note says such a unit never reads *"another package's files"*, while this one searches
`qt`'s folder (defect 2357). Had `qt_wide.h` carried a name the SDK also holds, shapes 2 and 3 show it
would have compiled the SDK's.

### Shape 6, the platform layer (`examples/server`)

Whole (`F/probes/server-w/out.txt`): `build` exit 0; `--emit-c`'s `#line`s name **12** placed modules
(`reached.txt`); **no unit's `-M` names any file under `heroes.packages/`**; `hero_net.h` is read from
`F/tree/runtime/hero_net.h`; the layer archive `layer-<k>.a` is built; `nm fp-server | grep -c ' T
_hero_net'`: 74. Pruned (`F/probes/server-m/`, `core` 0.5.0 published with those 12 files alone,
`sha256-5545...4d8a`, 12 files placed): the same in every line, 74 symbols. The two layer archives
differ in 4 bytes (`cmp -l | wc -l`). `ar t` and `nm` hash alike, and each archive holds its program
folder's path 4 times (`strings | grep -c server-w` / `server-m`), so the program folder's path is the
whole difference. Both binaries, run under `timeout -s KILL 60`, exit 0, their output `cmp`-equal to
`main.expected`. **No file of `core` is read for the platform layer, and the archive beside the runtime
is linked as at the base, under a pruned tree.**

### Shape 7, `link` naming an archive the package carries (`lnk`)

`lnk` carries `libpkl.a` (built here with `clang -c` and `ar rcs`) beside `pkl.h`; its group is
`extern "pkl.h" link "pkl"`. Whole tree: **exit 1, `error[ffi_missing_library]: this machine has no library
called pkl`**. The linker does not search the placed root. With the program passing `--library
heroes.packages/lnk@1.0.0-53d354246e3728bb`: exit 0, prints `42`. So the source reading is confirmed by
the run: **a fetched package's `link` group reads the machine's libraries, never the package's**. No
placement rule for an archive arises (H places it as any file, inert; M need not), and an archive is
platform C, which README § 4 rule 5 already forbids.

### The Windows arm, as MinGW (zig 0.15.2, `-target x86_64-windows-gnu`, preprocessing only, never run)

Over the whole `arms` tree the `-M` listing names `win.h` (and `err.h` at `-O2`), and the macros read
`PK_FAMILY` 20. Over the Darwin subset and over the Linux subset the unit stops at `a.h:3:10` (`win.h`
not found): **loud, not silent**, for this name. Under the base's build that is `ffi_missing_header` with
the note *"A header it needs ships inside the package"*, false of a pruned tree. MSVC, the project's
Windows target, is owed to the CI's leg; the box is off.

## Runs in Linux arm64 (`heroes-linux-arm64:latest`, Debian clang 22.1.8)

`F/linux/run.sh` and `run2.sh`, the base's archive on stdin, the seed and stage 2 built in the container
(seed exit 0, stage 2 exit 0, **`sources 5bac791363384945`**: the line 00-shared.md left unrun in that
image), each program folder **copied as Darwin placed it** (`F/linux/progs/`, `progs2/`, no `build/`).
That is the case M must survive: a placement made on one machine, committed, built on another. Logs:
`F/linux/run.log`, `run2.log`. Outer bound: `timeout -s KILL 1500` for the first run, which built stage 2
inside it, rather than the brief's 900; 900 for the second. `docker ps --filter name=fp-250` printed
nothing after each.

| program (placed on Darwin) | Linux build | prints | the file read where the package's was meant |
|---|---|---|---|
| `deep` whole / M / groups-only | 0 / 0 / **1** `ffi_missing_header` | `42` / `42` / — | — |
| `arms` whole `-O0` | 0 | `10 400 6000 1` | — (`-M`: `a.h error.h posix.h tune_linux.h`, **Linux's closure**; `tune_apple.h` not listed, since the taken `#if` skips the `#elif`) |
| `arms` whole `-O2` | 0 | `10 400 6000 2` | — (adds `err.h`) |
| **`arms` Darwin's `-O0` closure, `-O0`** | 0 | `10` **`1`** `6000 1` | **`/usr/include/error.h`** (glibc's) |
| **`arms` Darwin's `-O0` closure, `-O2`** | 0 | `10` **`1`** `6000` **`1`** | **`/usr/include/error.h`, `/usr/include/err.h`** |
| `arms` Linux's closure (the hypothesis above, now measured equal to Linux's `-M`) | 0 | `10 400 6000 1` | — (and on Darwin it printed `10 1 1 1`, the SDK's `util.h`) |
| `ask` whole / **H** | 0 / 0 | `2 77 2` / `2 77` **`1`** | — / `b.hero` absent: `__has_include` false |
| `embed` whole | **1** `package_reads_outside` | — | the same false message: Linux's clang also writes the `#embed` target absolute (`/work/p/.../data.bin`) |
| `lnk` whole | **1** `ffi_missing_library` | — | with `--library <placed root>`: **1** `ffi_missing_link`, since the archive the package carries is Darwin's (a platform-C archive, which README § 4 rule 5 already forbids) |
| `ra` whole / M / **H** | 0 / 0 / 0 | `3` | — |
| `qt`/`rb` whole / `qt` by its own closure | 0 / **1** `ffi_missing_header` naming `rb` | `103` / — | — (`rb`'s unit's `-M`: `rb.h qt.h qt_wide.h`, as on Darwin) |
| `server` whole / M (12 files) / **H** (12 + 7 cards) | 0 / 0 / 0 | `cmp`-equal to `main.expected` each | — (no unit reads a file of `core`) |

`deep`'s closure, `pt`'s, `ask`'s and `core`'s are the same on both platforms; `arms`'s is not. On Darwin
it is `a.h posix.h util.h tune_apple.h tune_linux.h`, on Linux `a.h posix.h error.h tune_linux.h`, as
MinGW `a.h win.h tune_apple.h tune_linux.h`, each with `err.h` added at `-O2`. And `qt`'s depends on the
unit that includes it.

## H, the route I recommend for a package's C, built on both platforms

H places **every file of the version but the `.hero` modules the program does not reach**. That set is
computed from the version's listing and the reached set alone: **no clang, no platform, no level, no
`pkg-config` answer**. Its C behaviour, built here as published subsets (`F/probes/index-hh/`,
`index-h/`), on Darwin and in Linux arm64:

| shape | H's placement | Darwin | Linux arm64 |
|---|---|---|---|
| 1 `deep` | whole but `b.hero` (keeps `b.h`) | `42` | `42` |
| 2/3 `arms` | whole (one module) | `10 300 5000 1` / `-O2` `…2` | `10 400 6000 1` / `-O2` `…2` |
| 4 `ask` | whole but `b.hero` | `2 77` **`1`** | `2 77` **`1`** (the residue) |
| 5 `ra`→`pt` | whole but `extra.hero` | `3` | `3` |
| 5 `rb`→`qt` | whole | `103` | `103` |
| 6 `server`→`core` | 12 modules + 7 cards = 19 files | `main.expected` | `main.expected` |

Every value is the whole tree's on that platform, but for the one residue. **What I built is the C side of H
(placements and their builds); `heroes add`/`fetch` placing H, and F1 in the build, are not built by me:
reported as not built.** For `core` 0.5.0 H and M differ only by cards (`core` has no `.c` or `.h`,
00-shared.md).

## The fingerprint, for C

- **F4** (checked at fetch alone) refuses nothing at build: a header edited, removed or added under the
  placed tree compiles. A removal is exactly the pruned trees above. In `arms`, a header gone from the
  tree read the SDK's or glibc's file of that name at exit 0. Not run as F4 (no compiler of it was built),
  since that outcome is F4's definition plus the measured removals. **It fails the veto's test.**
- **F1 as 00-shared.md words it** (*"a file absent being unreached"*) refuses a header changed or added,
  but **not a header removed**. Then the filesystem is the pruned tree's, and the build falls through to
  the SDK's `util.h`, glibc's `error.h` or a `__has_include` false. **F1 must read: at every build, each
  placed file held to its line of the listing; a file present and unlisted refused; a file absent refused
  unless it is a `.hero` module this build does not reach.** The build knows its reached set, so this
  needs no clang and holds on every platform. With that wording H refuses all three edits before any C
  is read. Under M no such check exists: what is absent cannot be told from what is unreached without
  the C closure, which is a function of the platform, the level and the including unit.
- **Where the listing stands**: anything inside `<key>/` is on every package unit's include search
  (`-I <root>`, `package_units.hero`'s opening comment). From the C side the listing belongs outside the
  root, or under a name no C include spells. A preference, not run.

