# Panel 219, the historian: how other self-hosting compilers keep and refresh their bootstrap

Written 2026-10-17 by the historian seat, from the shared brief
(`docs/panel/219-briefs/00-shared.md`) and `docs/roadmap/milestones/M-bounded-seed.md`.

**What this seat ran, and what it did not.** Its instrument is the primary sources: every page
below was read on 2026-10-17 through the session's fetch tool, which hands a page through a
summarising reader, so a quotation is marked as the tool returned it and a sum it computed over a
long listing is marked *approximate*. This seat built nothing and measured nothing on the tree, and
had no shell: every Heroes number below is the brief's or measurement 045's as cited, *unrun here*.
No paid run. *Heroes of code* was not needed as an index for these rooms and no claim rests on it.

**Marks.** *verified*: the named page, file or API answer was read in this session and says what
the line says. *unverified*: only a search summary or a secondary page carried it.

## verdict

**approve (advisory)**, per shape and per rhythm as below; one shape objected to.

| question | reading | the precedent it rests on |
|---|---|---|
| shape A: the seed without `#line` | **approve** | Nim's csources and Gambit's committed C carry no source-line directive in any file read |
| shape B: `#line N` with no file name | **object** | no precedent found; it keeps a quarter of the bytes `#line` costs and, by inference, the churn |
| shape C: a bootstrap root without `mutate`, `probe`, `measure` | **approve, on the brief's own condition** | Zig's `dev = .bootstrap`, six compiler features and nothing else |
| how it is asked for | **approve one fixed regeneration command; object to a general flag** | Zig's `update-zig1`, Nim's `koch csource -d:danger`, OCaml's `make bootstrap`: the profile is fixed by the step, never tuned by the user |
| rhythm 1: refreshed only when the two-stage build needs it | **approve** | Zig since 2022, OCaml |
| rhythm 2: refreshed at each release | **approve** | Gambit since 2012, Rust's stage0 |
| the anti-precedent | (warning) | V refreshes its emitted C per upstream commit: a 1,937,894 KB repository |

## precedents

### Zig: a committed WebAssembly bootstrap, refreshed when the compiler needs it

- **The 2022 change.** Andrew Kelley, *Goodbye to the C++ Implementation of Zig*, 2022-12-07
  (https://ziglang.org/news/goodbye-cpp/): 80,000 lines of C++ removed; compiling the compiler to C
  gave, in the post's words as returned, *"an 80 MiB C file"*, *"orders of magnitude larger than I
  would feel comfortable with"* to commit; instead `zig1.wasm`, *"a 2.6 MiB file"* at
  `ReleaseSmall`, 2.4 MiB after `wasm-opt`, 637 KB under zstd. It needs updating only when a change
  affects the compiler building itself (a bug the build never triggers can wait; a language change
  the compiler uses for its own build cannot), by `zig build update-zig1`; *"zig1 only has the C
  backend enabled."* **verified**
- **Raw, not compressed, two days later.** Commit `89d1ccc477c1dbc8bb8d13ad2598ef0190c25898`,
  Veikka Tuominen co-authored by Andrew Kelley, committed 2022-12-10: `stage1/zig1.wasm.zst` removed,
  `stage1/zig1.wasm` added; the reasons given are smaller release tarballs, a possibly smaller git
  repository because git can relate versions of an uncompressed file, and no vendored zstd
  (https://api.github.com/repos/ziglang/zig/commits/89d1ccc477c1dbc8bb8d13ad2598ef0190c25898).
  The same finding as `seed/README.md` § Why raw C (panel 085 R1), reached independently. **verified**
- **Today.** `stage1/zig1.wasm` is 3,183,342 bytes in the GitHub repository, which says *"Moved to
  Codeberg"*, last pushed 2025-11-27, 379,756 KB
  (https://api.github.com/repos/ziglang/zig/contents/stage1,
  https://api.github.com/repos/ziglang/zig). **verified**
- **How often.** The commits touching `stage1/zig1.wasm`, read in two parts: 53 dated, from
  2022-12-09 to 2025-11-22, five in December 2022, 25 in 2023, 9 in 2024, 14 in 2025, one entry cut
  at the join between the two parts
  (https://api.github.com/repos/ziglang/zig/commits?path=stage1/zig1.wasm&per_page=100).
  **verified, count approximate**
- **The bootstrap profile, fixed by the step.** `build.zig`'s `update-zig1` sets `.optimize =
  .ReleaseSmall`, `wasm32-wasi`, `enable_logging` false and `dev` = `.bootstrap`, hard-coded, then
  `wasm-opt -Oz` (https://raw.githubusercontent.com/ziglang/zig/master/build.zig). `src/dev.zig`
  documents `bootstrap` as *"zig1 features: `-ofmt=c` only; `-OReleaseFast` or `-OReleaseSmall`
  only; no `@setRuntimeSafety(true)`"*, and it supports six features: `build_exe_command`,
  `build_obj_command`, `ast_gen`, `sema`, `c_backend`, `c_linker`; every other returns false
  (https://raw.githubusercontent.com/ziglang/zig/master/src/dev.zig). **verified**
- **The header frozen beside the bootstrap.** `update-zig1` copies `lib/zig.h` to `stage1/zig.h`;
  `CMakeLists.txt` builds `zig1` from the wasm2c output and `stage1/wasi.c`, runs it as `build-exe
  -ofmt=c -lc -OReleaseSmall --name zig2`, and compiles `zig2.c` with
  `target_include_directories(zig2 PUBLIC stage1)`, no `lib` include
  (https://raw.githubusercontent.com/ziglang/zig/master/CMakeLists.txt). So a change to the live
  `lib/zig.h` does not reach stages 1 and 2. **verified**
- **Audited by reproduction.** Motiejus Jakštys, 2024-11-12: `zig1.wasm` of 0.13.0 rebuilt without
  the checked-in binaries (from the last C++ commit forward, and through Guix) matched SHA-256
  `127909fb…dbb27` (https://m.jakstys.lt/2024/zig-reproduced-without-binaries/). **verified** (the
  post; it is the author's report of their own run)
- In the main repository's history: yes. Line directives: no C is committed.

### OCaml: committed bytecode, refreshed when the runtime moves, in a commit of its own

- `boot/ocamlc` 3,785,529 bytes, `boot/ocamllex` 451,828 bytes, in the main repository
  (https://api.github.com/repos/ocaml/ocaml/contents/boot). **verified**
- `BOOTSTRAP.adoc`: a bootstrap is required when *"something changes in the runtime system"*
  (magic numbers, instruction format, primitives, `.cmi` format); `make bootstrap` builds the new
  `boot/`; upstream, *"indicate in the message of the commit that the changes need a bootstrap"* and
  *"commit the result of the bootstrap separately, after that commit"*; *"It is desirable for
  bootstraps to be easily repeatable"* (https://github.com/ocaml/ocaml/blob/trunk/BOOTSTRAP.adoc).
  **verified**
- How often: 15 commits touched `boot/ocamlc` from 2025-10-17 to 2026-10-17 by committer date, 12
  by author date, most titled *Bootstrap*
  (https://api.github.com/repos/ocaml/ocaml/commits?path=boot/ocamlc&since=2025-10-17T00:00:00Z).
  **verified**
- Line directives: not applicable, bytecode.

### Go: nothing committed, a downloaded toolchain of a fixed older version

- *"1.5 <= Go <= 1.19: a Go 1.4 compiler. 1.20 <= Go <= 1.21: a Go 1.17 compiler. 1.22 <= Go <=
  1.23: a Go 1.20 compiler."* and *"Go version 1.N will require a Go 1.M compiler, where M is N-2
  rounded down to an even number"*; Go 1.4 *"the last Go release with a compiler written in C"*;
  `GOROOT_BOOTSTRAP` (https://go.dev/doc/install/source). **verified**
- The frozen 1.4 bootstrap was cut at least three times: `go1.4-bootstrap-20170518`, `-20170531`
  and `-20171003` (https://github.com/golang/go/issues/45563, opened 2021-04-14). **verified**

### Rust: nothing committed but a pin, bumped at every release

- stage0 is *"the very recent beta"*, downloaded by bootstrap
  (https://rustc-dev-guide.rust-lang.org/building/bootstrapping/what-bootstrapping-does.html).
  **verified**
- `src/stage0` is generated by `./x.py run src/tools/bump-stage0`; today `compiler_date` 2026-09-29,
  `compiler_version` beta, `dist_server` https://static.rust-lang.org
  (https://raw.githubusercontent.com/rust-lang/rust/master/src/stage0). **verified**
- The release process: *"Run this to update the bootstrap compiler to the beta you created
  yesterday"* (Tuesday), and a Friday PR updating the beta branch's stage0
  (https://forge.rust-lang.org/release/process.html); *"Rust releases every six weeks, like
  clockwork"* (https://doc.rust-lang.org/book/appendix-07-nightly-rust.html); 1.35.0's
  `src/stage0.txt`: *"Currently Rust always bootstraps from the previous stable release"*
  (https://fuchsia.googlesource.com/third_party/rust/+/refs/tags/1.35.0/src/stage0.txt). **verified**

### GHC: nothing committed, the two previous major releases

- The two-release window explained on ghc-devs by Matthias Kilian, 2016-12-13, as easing
  distribution packagers, while saying he cannot speak for those who decided
  (https://mail.haskell.org/pipermail/ghc-devs/2016-December/013410.html). **verified**
- Its origin in a 2013 post by Simon Marlow, and the commit *"Require GHC 8.10 as the minimum
  compiler for bootstrapping"*: **unverified** (search summaries only).

### Nim: emitted C in a separate repository, one regeneration per generation, no `#line`

- **Four generations, each a fresh repository** (GitHub API, `size` being GitHub's packed size):
  `nim-lang/csources` created 2013-07-19, last pushed 2019-10-24, archived, 30 commits, 50,702 KB;
  `csources_v1` created 2020-12-07, 10 commits (*"based on Nim version 1.0.11"* 2020-12-21, then
  platform fixes to 2021-10-07), 3,453 KB; `csources_v2` created 2022-12-15, 9 commits, the last
  *"build with 7f6681b4…"* 2023-01-02, 20,730 KB; `csources_v3` created 2025-11-09, 4 commits, one
  of them the build (*"build with 92468e99…"*), 6,553 KB
  (https://api.github.com/repos/nim-lang/csources, `…/csources_v1`, `…/csources_v2`,
  `…/csources_v3`, and their `/commits`; https://github.com/nim-lang/csources for the 30). **verified**
- **Pinned by hash from the main repository**: `config/build_config.txt` names `csources_v3` and
  `nim_csourcesHash` `eeab3ac46e93f10efda8e58c4db02b9438319d71`
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/config/build_config.txt); `ci/funs.sh`
  clones it `git clone -q --depth 1` and checks out the hash, then `make`
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/ci/funs.sh). **verified**
- **The profile**: `koch`'s help, *"csource -d:danger builds the C sources for installation"*; its
  `csource` proc passes no line flag of its own
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/koch.nim). **verified**
- **Size**: `c_code/` holds 60 OS and CPU folders; one of them, `2_2`, about 196 files summing to
  about 33.7 MB, its largest `@mcgen.nim.c` 3,922,745 bytes and `@msem.nim.c` 3,709,519
  (https://api.github.com/repos/nim-lang/csources_v3/contents/c_code/2_2). **verified, sum
  approximate**
- **Line directives**: none in `@msaturate.nim.c` whole and none in the first 100,000 characters of
  `@msigmatch.nim.c` (1,059,481 bytes); no `nimln_` or `nimfr_` frame macro either; banner
  *"Generated by Nim Compiler v2.3.1"*
  (https://raw.githubusercontent.com/nim-lang/csources_v3/master/c_code/2_2/@msaturate.nim.c and
  `…/@msigmatch.nim.c`). **verified for those two files only**
- **The emitted-C compiler is a stage, never the result**: `build_all.sh` then runs
  `./koch boot -d:release --skipUserCfg --skipParentCfg --hints:off`
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/build_all.sh). **verified**
- In the main repository's history: no.

### V: emitted C in a separate repository, refreshed per upstream commit

- `vlang/vc`, *"V compiler's source translated from V to C"*, README *"This repository is
  generated automatically"*; created 2019-06-24, last pushed 2026-10-16, **1,937,894 KB**; `v.c`
  30,306,605 bytes, `v_win.c` 30,966,390 bytes; 27 of its latest 30 commits by `vlang-bot`, each
  titled `[v:master] <upstream hash>`, on 19 distinct days from 2026-09-20 to 2026-10-16
  (https://api.github.com/repos/vlang/vc, `…/contents/`, `…/commits`,
  https://raw.githubusercontent.com/vlang/vc/master/README.md). **verified**
- V's `GNUmakefile` fetches it with `git clone --filter=blob:none`
  (https://raw.githubusercontent.com/vlang/v/master/GNUmakefile). **verified**
- Line directives in `v.c`: **unverified**, the file exceeds the fetch tool's 10 MB limit. V's
  documentation says that under `-g` *"Generated C uses `#line` directives so debuggers and panic
  stacktraces resolve positions to the original .v files"* (https://docs.vlang.io/debugging.html,
  **verified**); that `v.c` is generated with `-os cross` and without `-g` rests on a mirror of
  `gen_vc.yml`, **unverified**.

### Gambit Scheme: emitted C in the main repository, refreshed at each release

- Commit `ad8d249a`, 2012-04-16: *"Make repo contain the generated .c files so that a clone of the
  repo is sufficient to build the system."* (the path history of `gsc/_front.c`,
  https://api.github.com/repos/gambit/gambit/commits?path=gsc/_front.c&per_page=100). **verified**
- Today: `gsc/` 36 generated `_*.c` files, 34,466,403 bytes; `lib/` 25 `.c` files, 29,140,223
  bytes, generated `_*.c` and hand-written `os_*.c`, `mem.c` and others together;
  `lib/_kernel.c` *"produced by Gambit v4.9.8"*; the repository 145,099 KB, created 2012-03-13
  (https://api.github.com/repos/gambit/gambit/contents/gsc, `…/contents/lib`,
  https://api.github.com/repos/gambit/gambit). **verified**
- How often: about 34 commits touched `gsc/_front.c` from 2012-04-16 to 2026-08-09, almost all
  titled *"[RUNTIME CHANGES NEEDED FOR v4.x.y] Changed version of runtime using misc/changev"*, one
  per release, beside *"Select files generated by bootstrap to be pushed to repo"* (2018-09-03).
  **verified, count approximate** (read in two parts).
- `INSTALL.txt`: the source *"contains some stale .c files"*, generated at the latest release;
  `make from-scratch` *"Builds the system from source where the .c files which are generated from
  .scm files are stale. This is normally the case after cloning"*
  (https://raw.githubusercontent.com/gambit/gambit/master/INSTALL.txt). **verified**
- Line directives: none in the first 100,000 characters of `lib/_kernel.c` (1,278,981 bytes).
  **verified for that portion only**

### CHICKEN Scheme and cfront, the lineage

- CHICKEN: release tarballs carry the generated C; a git checkout needs an installed `chicken`, and
  `scripts/bootstrap.sh` fetches a development snapshot. **unverified** (search summaries; the
  README page did not render through the fetch tool).
- cfront: Stroustrup, *A History of C++: 1979-1991* (HOPL-II), § 3.2, p. 19: cfront generated C
  *"because I needed extreme portability"*, and *"the C compiler is used as a code generator only"*
  (https://www.stroustrup.com/hopl2.pdf). **verified**. That cfront was ported by shipping its own
  C output: **unverified** (Wikipedia and a search summary of the Release 1.0 makefile, which
  returned 404); Release 3.0.3's source has a `scratch/` folder
  (https://softwarepreservation.computerhistory.org/c_plus_plus/cfront/release_3.0.3/source/),
  **verified**, its role **unverified**.

## The reading, shape by shape

**Shape A, no `#line` (26,803,431 bytes by the brief): approve.** Every committed emitted C whose
text this seat could open carries no source-line directive: Nim's two files, built `-d:danger`
with no frame macros either, and the head of Gambit's `_kernel.c`. None was found that keeps them;
what was searched is those three files, and V's `v.c` could not be opened. What the brief asks it
costs, a crash in the seed-built compiler naming C lines: Nim and Zig treat that compiler as a
stage only (`koch boot`; `zig1`, then `zig2`, then the compiler), which is the answer part 2's
two-stage build gives; the compiler anybody runs is the one built from `selfhost/`, emitted with
its `#line`.

**Shape B, `#line N` alone (32,868,681 bytes): object.** No precedent found. It keeps about 6 MB
over shape A, and, by inference and unrun, most of the churn the milestone measured (62% of the
lines changed between `v0.3.0` and `v0.4.0` were `#line` lines), since the numbers still move when
a `.hero` line moves.

**Shape C, the bootstrap root without `mutate`, `probe`, `measure`: approve on the brief's own
condition**, that the compiler it builds builds `selfhost/main.hero` whole. Zig's `.bootstrap` env
is the precedent: a compiler cut to six features whose one job is to build the next stage, and
`zig1` building `zig2` from the full source is the shape of that condition met.

**How it is asked for.** In every precedent read, the bootstrap profile is fixed by one named
regeneration step and is not a knob of the shipped compiler: Zig's `update-zig1` hard-codes
`dev = .bootstrap`, Nim's `koch csource -d:danger`, OCaml's `make bootstrap`. That supports
CLAUDE.md § 10's refusal of `--no-line` and favours the regeneration's own command or a subcommand
whose profile cannot be tuned. Which of those two Heroes forms is the compiler-engineer's call.

## The reading, rhythm by rhythm

**Refreshed only when the two-stage build needs it: approve.** It is Zig's stated policy (53
updates in about three years, 9 to 25 a year) and OCaml's (15 in the last year, each a commit of
its own). The milestone page counts 62 Heroes regenerations in the two weeks to 2026-10-14, V's
pattern.

**At each release: approve.** Gambit has kept exactly this since 2012: emitted C committed in the
main history, refreshed once per release, stale between releases and rebuilt by `make
from-scratch`. Rust bumps its stage0 at every release.

**The warning, V.** Emitted C of about 30 MB regenerated per upstream commit gave a 1,937,894 KB
repository, which V's own makefile works around with a partial clone; Nim's first `csources`
reached 50,702 KB in 30 commits. Measurement 045 found the Heroes seed 87% of the pack a clone
receives at 301 versions.

**What the precedents disagree on, for the sitting to choose knowingly.** Zig froze the C header
its bootstrap compiles against beside the bootstrap (`stage1/zig.h`, `stage1/wasi.c`), so a change
to the live `lib/zig.h` never turns stages 1 and 2 red. OCaml accepts the opposite coupling: a
runtime change is what requires a bootstrap. The Heroes seed compiles against the live `runtime/`
(`seed/README.md`), and measurement 045 found 2,279 of 3,619 commits carrying a seed whose runtime
interface their tree no longer has; so under *refreshed only when red*, every
`HERO_RUNTIME_ABI` move would be a refresh (inference, unrun). Either is precedented.

## argument

Precedent splits on where the bootstrap lives (Zig, OCaml and Gambit in the main history; Nim and
V apart; Rust, Go and GHC downloaded) but agrees on two things this sitting proposes. The bootstrap
is emitted in a profile of its own, fixed by one regeneration command: Zig's `dev = .bootstrap`
with six features, Nim's `csource -d:danger`, and no source-line directive in any committed C read.
And it is refreshed when the build needs it or at a release, never per commit: Zig 9 to 25 times a
year, OCaml 15, Gambit once per release since 2012. The one project refreshing per commit, V,
carries a 1.9 GB repository. `#line N` alone has no precedent found.

## condition

My reading changes if a self-hosting compiler that commits emitted C is found keeping `#line` in it
and reporting that it needed them to diagnose its bootstrap-built stage (shape A would then cost
something precedented); or if a project that moved from per-commit to when-needed refresh reports
the stale bootstrap breaking its builds or `git bisect` more than the history saved (rhythm 1). And
on the runtime coupling: if the seed stays bound to the live `runtime/`, a count of
`HERO_RUNTIME_ABI` moves per month decides whether *only when red* is Zig's rare event or OCaml's
routine one.
