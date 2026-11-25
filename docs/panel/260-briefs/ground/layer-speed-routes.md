# The platform layer's cost: the routes measured (lane layer-speed, 2026-11-25)

The defect: `coord/issues/d-layer-every-build.md`. Every number below was run
by the lane on 2026-11-25, on this Mac and in the Linux arm64 container, the
machine NOT still (other lanes running; the load is said beside each table), so
what stands is the ratio between compilers measured in the same minutes,
interleaved configuration by configuration; the latency compare on a still
machine stays owed to the round's close. Scratch: `.claude/worktrees/lane-layer-speed-scratch/`
(`table-mac-1.txt`, `table-mac-2.txt`, `linux/results.txt`, `linux/probe.txt`,
`linux/ui.txt`; `timing.sh`, `table.sh`, `linux/run.sh`, `py/summarize.py`).

**For the soundness lane, in one paragraph.** What lands under panel 234's R6
(every member in the archive, the linker choosing) buys about a third of the
warm cost on macOS and nothing on Linux arm64, the leg that passed the
360-minute ceiling: there the 43 members' preprocessing IS the cost, asked in
one clang run or in 43. Only a build that asks and compiles what the program
reaches gets back to 17c50eba6's numbers, warm and cold: the prototype of that
route (a) does for `print(1)`, the one program 17c50eba6 builds, and takes 60
to 82% off the clock's and the UI's warm builds on macOS; but the closure it takes from the
runtime's text is short of the members' real calls (9 on macOS, 107 on Linux,
every UI program on Linux refused), so it is not landable as prototyped.
Recommendation, a question for the sitting rather than a premise: route (a)
with its closure held to the archive's own member-to-member calls on every leg
(a compiler test that reads them with `nm`, as `layer.hero`'s tests already
read the archive), and a link that fails on a layer name retried with the whole
archive, so a closure that is short costs time and never a refused program;
route (d) is the R6-keeping alternative, unbuilt.

## Where the net's builds put their `build/`

The compiler keeps its cache in `build/` under the folder it starts in
(`toolchain.locate_toolchain`, `build: "build"`), and the harness starts every
compiler it runs in its own working folder, the repository's root
(`shell.run`, `folder: ""`), every suite and case alike, one with `links` or an
`environment` included (`case_setting.ran`). **So the whole net shares ONE
`build/`, the root's**, and pays one cold archive per key: per level and
sanitizer set, per set of Swift members, per reach (a case whose `environment`
names `CPATH`, `C_INCLUDE_PATH` or `CCC_OVERRIDE_OPTIONS`, 21 of the 45
`environment` files, keys apart). The exceptions, read from the tree: the 3
`unsupported` cases holding a `directory` file, which start in a copy under the
harness's scratch with a `build/` of their own; `fetch`'s 7 commands written
`cd <dir> && heroes ...`, all `add` and `check`, which link nothing; and the
compiler's own tests, whose layer tests make about ten fresh
`build/selftest-*` folders, each a cold archive. **What the net pays is the
warm cost**, at every build of every case: the CI's `run` went from 4.6 s a
case to 9.1, about two warm builds a case.

## The compilers measured

- `old`: the seed of 17c50eba6 (`rebel-scratch/perf/old`, clang -O2), beside
  its runtime. It builds `print(1)`; the clock and UI programs are refused
  (`ffi_missing_link` for `hero_time_monotonic_ns`: the layer had no archive
  then), so those rows have no `old`.
- `new` (first Mac table and Linux only): the seed of 0521cd903
  (`rebel-scratch/perf/new`), the coordinator's. A seed is refreshed only at a
  round's gate, so it is not 0521cd903's own `selfhost/`; its cold builds are
  2 to 3 times `main`'s.
- `main`: main at 849bb174a, its `selfhost/` emitted (`--emit-c`) and built
  with clang -O2 as the seeds are, beside its runtime.
- `c1`: the lane's commits, 184a and 184b (`0097b3f26`), built the same way;
  R6 kept.
- `a`: route (a)'s prototype on `0097b3f26`, uncommitted, built the same way.

Each cell: the best `real` of the kept runs, seconds, and that run's
`user`+`sys` in brackets; warm 4 runs in one folder, cold 3 runs each in a
fresh folder, the first of each set discarded. `exit N`: the build refused.
Programs: `print(1)` (`examples/gallery/00-first.hero`), the clock
(`tests/golden/run/the-platform-layer-clock-never-goes-back.hero`), the UI (a
four-line program binding `hero_ui_native_here` from `hero_ui.h`).

## macOS, first table (07:21 to 08:41, load 11 to 138)

| program | flags | build | old | new | main | c1 | a |
|---|---|---|---|---|---|---|---|
| print | -O0 | warm | 0.37 (0.28) | 4.08 (2.95) | 3.45 (2.54) | 2.21 (1.78) | 0.45 (0.35) |
| print | -O0 | cold | 1.00 (0.75) | 31.28 (27.87) | 11.19 (8.60) | 10.79 (8.22) | 1.24 (0.95) |
| print | -O2 | warm | 0.42 (0.28) | 3.86 (3.00) | 3.41 (2.60) | 2.44 (1.88) | 0.46 (0.35) |
| print | -O2 | cold | 2.26 (1.74) | 58.09 (48.85) | 18.13 (14.93) | 17.27 (14.36) | 2.52 (2.31) |
| print | --sanitize | warm | 0.33 (0.30) | 3.71 (2.99) | 2.95 (2.50) | 1.87 (1.74) | 0.38 (0.35) |
| print | --sanitize | cold | 1.31 (1.13) | 33.04 (30.79) | 16.37 (11.54) | 17.05 (10.60) | 2.40 (1.71) |
| clock | -O0 | warm | exit 1 | 4.93 (3.09) | 4.32 (2.67) | 3.03 (2.00) | 0.79 (0.48) |
| clock | -O0 | cold | exit 1 | 39.65 (29.38) | 13.07 (9.34) | 12.28 (8.57) | 2.21 (1.39) |
| clock | -O2 | warm | exit 1 | 4.11 (3.07) | 4.44 (2.67) | 2.74 (1.90) | 0.80 (0.47) |
| clock | -O2 | cold | exit 1 | 67.40 (51.71) | 21.97 (16.08) | 20.64 (15.60) | 3.96 (3.07) |
| clock | --sanitize | warm | exit 1 | 4.66 (3.04) | 4.13 (2.70) | 2.87 (2.03) | 0.76 (0.48) |
| clock | --sanitize | cold | exit 1 | 47.16 (32.15) | 16.95 (11.51) | 15.97 (10.53) | 3.01 (2.08) |
| ui | -O0 | warm | n/a | 4.72 (3.53) | 4.65 (3.56) | 3.55 (2.90) | 1.80 (1.61) |
| ui | -O0 | cold | n/a | 37.43 (27.96) | 33.44 (28.30) | 32.19 (27.76) | 26.55 (21.92) |
| ui | -O2 | warm | n/a | 4.34 (3.64) | 4.39 (3.61) | 3.11 (2.89) | 1.75 (1.58) |
| ui | -O2 | cold | n/a | 64.65 (49.51) | 97.56 (49.13) | 119.32 (46.90) | 53.15 (34.82) |
| ui | --sanitize | warm | n/a | 3.89 (3.44) | 3.99 (3.51) | 2.75 (2.77) | 1.36 (1.57) |
| ui | --sanitize | cold | n/a | 31.33 (29.38) | 28.82 (28.05) | 26.20 (26.19) | 20.75 (20.78) |

The `ui -O2 cold` row of `main` and `c1` read `real` twice its `user`+`sys`:
waiting, at a load past 100; the bracket is the reading there.

## macOS, second table (09:17 to 09:45, load 7 to 142; stopped by the lane after the `ui -O0 warm` row, the first table holding the rest, so the Mac was given back to the other lanes)

| program | flags | build | old | main | c1 | a |
|---|---|---|---|---|---|---|
| print | -O0 | warm | 0.23 (0.23) | 3.27 (2.28) | 1.97 (1.58) | 0.42 (0.30) |
| print | -O0 | cold | 0.91 (0.64) | 11.16 (8.20) | 10.00 (7.61) | 1.09 (0.86) |
| print | -O2 | warm | 0.34 (0.26) | 3.41 (2.42) | 2.29 (1.70) | 0.43 (0.32) |
| print | -O2 | cold | 4.44 (1.77) | 17.66 (12.80) | 18.39 (13.14) | 3.29 (2.44) |
| print | --sanitize | warm | 0.42 (0.27) | 3.37 (2.39) | 2.26 (1.71) | 0.50 (0.34) |
| print | --sanitize | cold | 1.58 (1.09) | 14.87 (9.84) | 14.81 (9.19) | 2.33 (1.48) |
| clock | -O0 | warm | exit 1 | 3.32 (2.31) | 2.39 (1.69) | 0.62 (0.41) |
| clock | -O0 | cold | exit 1 | 12.53 (7.95) | 12.68 (7.34) | 2.09 (1.21) |
| clock | -O2 | warm | exit 1 | 3.87 (2.44) | 2.71 (1.83) | 0.71 (0.43) |
| clock | -O2 | cold | exit 1 | 26.54 (14.76) | 24.84 (14.26) | 4.50 (2.74) |
| clock | --sanitize | warm | exit 1 | 3.66 (2.50) | 2.92 (1.91) | 1.21 (0.54) |
| clock | --sanitize | cold | exit 1 | 16.66 (10.77) | 17.06 (10.01) | 4.05 (1.97) |
| ui | -O0 | warm | n/a | 4.68 (3.36) | 3.99 (2.61) | 2.10 (1.56) |

## Linux arm64 (`heroes-linux-arm64:latest`, Debian clang 22.1.8, 8 cores, container load 1.4 to 7)

| program | flags | build | old | new | main | c1 | a |
|---|---|---|---|---|---|---|---|
| print | -O0 | warm | 0.20 (0.19) | 1.47 (1.46) | 1.42 (1.40) | 1.36 (1.28) | 0.23 (0.22) |
| print | -O0 | cold | 0.81 (0.78) | 7.25 (7.20) | 7.28 (7.18) | 8.88 (8.63) | 1.29 (1.17) |
| print | -O2 | warm | 0.26 (0.23) | 1.45 (1.43) | 2.00 (1.82) | 2.60 (2.02) | 0.39 (0.32) |
| print | -O2 | cold | 4.21 (3.79) | 28.10 (27.52) | 26.09 (25.76) | 23.12 (22.65) | 3.84 (3.74) |
| print | --sanitize | warm | 0.35 (0.33) | 2.00 (1.94) | 2.21 (2.15) | 2.31 (2.00) | 0.50 (0.42) |
| print | --sanitize | cold | 2.10 (1.89) | 15.13 (14.88) | 24.94 (23.96) | 22.85 (21.70) | 3.67 (3.37) |
| clock | -O0 | warm | exit 1 | 2.72 (2.52) | 2.39 (2.19) | 3.13 (2.47) | 0.56 (0.46) |
| clock | -O0 | cold | exit 1 | 9.74 (9.31) | 10.12 (9.53) | 8.71 (8.15) | 1.64 (1.40) |
| clock | -O2 | warm | exit 1 | 1.76 (1.65) | 1.88 (1.77) | 1.71 (1.51) | 0.35 (0.31) |
| clock | -O2 | cold | exit 1 | 28.53 (27.71) | 30.79 (30.12) | 31.40 (30.31) | 5.01 (4.72) |
| clock | --sanitize | warm | exit 1 | 2.64 (2.50) | 2.63 (2.51) | 2.80 (2.38) | 0.73 (0.65) |
| clock | --sanitize | cold | exit 1 | 17.56 (17.09) | 17.31 (16.81) | 20.55 (19.66) | 4.33 (3.79) |
| ui | -O0 | warm | n/a | exit 2 | exit 2 | exit 2 | exit 2 |
| ui | -O0 | cold | n/a | exit 2 | exit 2 | exit 2 | exit 2 |
| ui | -O2 | warm | n/a | exit 2 | exit 2 | exit 2 | exit 2 |
| ui | -O2 | cold | n/a | exit 2 | exit 2 | exit 2 | exit 2 |
| ui | --sanitize | warm | n/a | 2.15 (2.09) | 2.44 (2.30) | 2.36 (2.07) | exit 2 |
| ui | --sanitize | cold | n/a | 15.65 (15.37) | 17.81 (17.54) | 19.08 (18.37) | exit 2 |

The `ui` rows: every compiler exits 2 at `-O0` and `-O2`, *internal error:
linking failed*, `hero_ui_parts_gtk.o` calling `nextafter` with no `-lm` on
the link, and links under `--sanitize`, whose runtime brings libm: a defect of
`main` found beside (`coord/issues/d-lane-layer-speed-2.md`, class blocking).
Route `a` exits 2 on every `ui` row, `--sanitize` included: its closure leaves
the GTK arm out (below).

**Where Linux's warm build spends its time** (`linux/probe.txt`, `clang`
wrapped to time each call, a warm `print(1)`): `main` 3.23 s, 48 clang calls
summing 2.01 s, the 43 members' `-M` about 40 ms each; `c1` 2.86 s, 6 calls,
the one `-M` run of the 43 members 1.62 s alone. Asked by hand in the same
minutes: the 43 one run each 1.76 s, the 43 in one run 1.66 s, 43 rules. **On
Linux the preprocessing is the cost, not the processes**, so route (c) buys
there about what it saves in starts, and nothing that keeps every member asked
reaches `old`'s 0.20 s.

## The routes

| route | panel 234's R6 | guards | state |
|---|---|---|---|
| 184a: `--emit-c` asks the layer nothing | kept | none moved: no link, no archive | committed `a2b288e5d` |
| (c) one clang run for every member's reach | kept | 2289, 443, 134, 2625 as before | committed `0097b3f26`, `dec556d50` |
| (c') the members asked in parallel | kept | as (c) | not built (below) |
| (b) the reach answered from the records | kept | **2289 lost**, measured | not a candidate |
| (a) the archive holds what the program's headers reach | **changed** | 2289, 443, 134, 2625 kept; **correct programs refused**, measured | prototype, uncommitted |
| (d) the link names the members it took | kept | as (c) | a route nobody listed; unbuilt |

### 184a, committed: `--emit-c` asks the layer nothing

The archive is a word of the link line alone (`assemble.linked_round`), and
`--emit-c` links nothing, so `layer_reach.for_build` takes `links` and answers
none for it. Every emission paid the 43 members' reach before: the `emission`
suite's 3,317 cases, 10.4 min on the CI's Linux arm64 leg on 2026-11-22 and
33.5 on the run that timed out.

### (c), committed: one clang run

`clang -M -MT dep m1.c ... m43.c` writes one rule per input on its standard
output, in the inputs' order (under `-MF` a listing of several inputs keeps
only the last's, measured); each rule was byte for byte the listing that
member's own `-MF` run writes, 43 of 43 compared with `cmp` on this Mac. So the
record and the archive's key are the ones the 43 runs made, and a listing that
is not one rule per input, each naming its own, is no answer
(`cli/reached_each.hero`). By hand, the same minutes on this Mac: the 43 runs
2.40 to 2.44 s real (user 1.07 to 1.11, sys 0.84 to 0.87); one run 1.39 s
(0.72, 0.51). On this Mac, `c1` against `main` warm: 15 to 40% less, every row of both
tables;
on Linux no gain the noise lets through (from 10% less to 35% more, row by
row; 2.86 s against 3.23 in the probe below). **It cannot reach the
old cost**: each member's preprocessing is the cost (the costliest alone on
this Mac: `hero_watch.c` 0.11 s; `hero_tls.c`, `hero_proxy.c`,
`hero_keyring.c`, `hero_crypto.c` 0.08 s each, the frameworks they include).

### (c'), the members asked in parallel: not built

The runtime's `hero_run_*` state is `_Thread_local` (`runtime/parts/run.c:65`),
so K runs on K threads would divide the wall time; not the CPU, which is what
the net pays, its suites already running several at a time (four inside the
Linux container, `.claude/rules/platforms.md`). It buys the net nothing and
puts threads into every build's path.

### (b), the reach answered from the records: not a candidate

The archive's record already vouches for the bytes of every file the members'
compiles read (`deps.vouched`; 871 distinct files, 16.4 MB, on this Mac). What
the reach question adds is **a file that did not exist**: one now found ahead
of a file read, or one a `__has_include` now finds, which no record of what was
read can name (`cli/opening.hero`'s opening says why). The shadow probe
(`build/shadowed/`, `cli/shadowed.hero`) does not cover it: it judges the
runtime's opening, not the members, and the folders the program or the
environment name, not the system's. **Measured in the Linux arm64 container**
with `c1`: `hero_printing.c` probes `__has_include(<cups/cups.h>)` and, where it
finds one, holds its copies of CUPS's records to it by `_Static_assert`. A cold
build of the clock program at exit 0 recorded 3,660 rows, none naming a file
under `/usr/include/cups`; a `cups.h` written there after it (planted to
refuse) made the next warm build exit 2 with clang's words naming
`/usr/include/cups/cups.h:1:2`, as a build in a fresh folder did. A reach
answered from the record would have served the old archive at exit 0: one
machine, two programs by the state of its `build/`, defect 2289's shape. The
other ways to see such a file: a directory's time on every folder of the
search, which needs a stat the runtime does not offer (`hero_os.h` has none)
and rests on every file system updating a directory's time on every entry; or
walking the search's folders whole, 0.38 to 0.52 s on this Mac (40,320 entries
under the SDK's `Frameworks`), more than (c).

### (a), prototype: the archive holds the members the program's headers reach

`py/route_a.py`, applied in the detached worktree
`.claude/worktrees/lane-layer-speed-a` at `0097b3f26`, uncommitted (its diff:
`lane-layer-speed-scratch/route-a.patch`, 105 lines):
`layer_reach.reached_c`, the closure `layer_reach.reached` already computes for
the Swift members (panel 253's R3, defect 2625), taken over the C members;
`layer.archive_of` archives those alone, keyed by their names, and none where
none is reached; `value_run`'s probe link takes the same. Built on this Mac:
`print(1)` links no archive; the clock's archive holds `hero_time.o` alone; the
UI's holds 12 members (`hero_net`, `hero_poll`, `hero_stream`,
`hero_stream_net`, `hero_ui`, the two web arms and the five AppKit members);
all three run. In the tables, `print(1)` under (a) is within 0.15 s of `old` warm and
1.6 s cold, on both machines.

**Its hole, measured**: the closure follows a member's OWN header, and a member
calls another through a header no member owns by name. Read from the whole
archive's symbols (`nm`, every name a member leaves undefined that another
defines; `py/closure.py`): **on macOS, 9 calls outside the closure**, all
`hero_audio` calling `hero_audio_coreaudio`'s `hero_audio_arm_*` through
`platform/hero_audio_arm.h`, so a program binding `hero_audio.h` builds at exit
0 under R6 and stops at exit 2 under (a), *internal error: linking failed*,
`_hero_audio_arm_beep` and eight more undefined (`progs/audio.hero`); **on
Linux arm64, 107**, `hero_audio` calling `hero_audio_pulse`'s nine and
`hero_ui` calling 98 of `hero_ui_gtk`'s `hero_ui_arm_*` through
`platform/hero_ui_arm.h`, so every UI program there is refused. Before (a) can
land it needs one of: the closure widened to the `platform/` headers a member
includes that the runtime itself does not (`*_arm.h` today), held by a compiler
test that reads the whole archive's member-to-member calls on every leg and
fails on one the closure misses; a link that fails on a layer name retried with
the whole archive, R6's link as the fallback, its cost only where the closure is
short; or a closure read from the members' symbols, which needs every member
compiled once a folder, R6's cold cost. The first with the second is the robust
pair: the test keeps the closure whole, the fallback keeps a program from ever
being refused for it.

**And what it closes besides**: under R6 one member that does not compile on a
machine stops every program's build, `print(1)` and the clock included
(`coord/issues/d-lane-layer-speed-1.md`, measured with the planted `cups.h`);
under (a) only the programs that reach it.

### (d), a route nobody listed: the link names the members it took

R6 kept whole: the archive served on its record's bytes, the link asked which
members it took (Apple's `-map`, GNU ld's and lld's `-Map` or `--trace`,
link.exe's `/MAP`, which `cli/supplied.hero` already reads where a program names
a library), and only those members asked what they reach now, the archive
compiled again and the link run again where one moved. Warm, `print(1)` would
ask nothing; cold, it still compiles the 43 members, R6's archive (11 to 22 s on this
Mac, 7 to 31 s on Linux, in `main`'s `print(1)` and clock rows). Unbuilt: four map
readers on every link, and a link that may run twice; a question for the same
sitting.

## The panel line

Route (a), in any form, changes which members a program's link can take, so it
is the soundness lane's (compiler-engineer and ffi-pragmatist), as the brief
says; so is (d), which keeps R6's archive but makes which members are asked
depend on the link's answer. Neither is committed. What is committed keeps R6
whole.
