# Panel 249, the completeness critic's second pass, over the reports

Started 2026-11-15 at about 07:58 CET (`date`), written as it goes. Brief:
`docs/panel/249-briefs/critic-reports.md`, read whole. Read whole before any run: `00-shared.md`,
`compiler-engineer.md`, `ffi-pragmatist.md`, `historian.md` and `critic-briefs.md` in `249-briefs/`; the
three seats' reports and my own first pass (`critic-briefs.md`) in `249-reports/`. The four
`*.before-critic.md` files are the texts my first pass read before the coordinator repaired them. My folder
for this pass is `.claude/worktrees/p249-scratch/critic/reports/` (called `R/` below). I give no verdict on
the routes.

Load on this Mac: `uptime` read *172.00 45.86 18.27* at 08:02 and *428.22 390.37 235.25* at 08:12,
another session's work. So **no duration below is a measurement**; what I count is exits, outputs and
bytes.

## 1. Item 2 of my brief: the compiler-engineer's `injump/` against the ffi-pragmatist's D-rebind

### 1.1 What I copied, and proof that the copies are the seats' own

Copied, never built or run in a seat's folder (`shasum -a 256`, first 16 hex digits):

- From `249-compiler-engineer/injump/`: `ij.hero` (`24374aa277a83647`), `lib/ij.c` (`a44017d7e4a35cb8`),
  `lib/ij.h` (`848f23e9722f86d7`), into `R/ij/src/`; `cmp` against the originals printed nothing.
- From `249-ffi-pragmatist/c/rebind/`: `rebind.c` (`0281ee1c166636cb`), and from its `h/`: `rebind.h`
  (`60697b8069715646`), `rflib.hero` (`14db92eb6af8ed82`), `rfliblocale.hero` (`81a19826f059f361`),
  `sigcb.h` (`8f94416fb6d4dce3`, the inputs manifest's), into `R/rebind/src/`; and R3's library source,
  `briefs/inputs/ev247/247-ffi-pragmatist/h/lib/sigcb.c` (`d14966c6e36aea9d`, the manifest's).

How each piece was built, recovered rather than guessed, because neither report gives every word:

- **`rebind.o`**: the ffi-pragmatist's report says only *"compiled with `-Wall`"*. I compiled my copy under
  ten flag sets and compared each object with the seat's `c/rebind/rebind.o` by `cmp`: **`clang -O1 -Wall -c
  rebind.c` is byte-identical to the seat's object**; `-Wall` alone, `-O2`, `-Os` and the rest differ. The
  member of the seat's `h/lib-nofp/librebind.a`, extracted by `ar x`, is `cmp`-equal to my `rebind.o`.
- **`libsigcb.a`**: my `clang -std=gnu11 -c -O2 -fomit-frame-pointer -I. sigcb.c` (00-shared.md's words) is
  `cmp`-equal to the member of the seat's `h/lib-nofp/libsigcb.a`.
- **`libij.a`**: my `clang -std=gnu11 -c -O2 -fomit-frame-pointer -I. ij.c` is `cmp`-equal to the
  compiler-engineer's `injump/lib/ij.o`. Its `ij_handler` opens `stp x20, x19` / `stp x29, x30` and never
  sets `x29` (`otool -tV`): the frameless shape R3's library has. `nm -m ij.o` imports `_siglongjmp`,
  `_sigsetjmp`, `_sigaction`, `_raise`.
- **The thunk's depth question**: the ffi-pragmatist's report says *"the thunk given `hero_tramp_inside()` for
  the build and restored, `cmp`-equal"*, *"four lines after `if (plain == 0 || own == NULL) return;`"*; the
  lines are in no file of its folder (`grep -rn 'plain == 0'` outside its `tree/` finds none, and its logs
  print only *"sigguard.c restored (cmp)"*). **Recovered from the seat's own D-rebind binary**: `otool -tV
  249-ffi-pragmatist/c/rebind/h/fp-r-flib-nofp`, read and not run, shows `_hero_callback_entered` testing
  `plain` and `own` and then `bl _hero_tramp_inside`, `cbz w0`, and on nonzero `mov w1, #0x0` / `bl
  _hero_signal_refuse`. So the patch is `if (hero_tramp_inside()) hero_signal_refuse(what, 0);` in that place,
  which is what I apply.

### 1.2 The build, held to the seat's

My compiler: `git archive 722ae76bf | tar -x` into `R/tree`, a `.git` file reading `gitdir:
/nonexistent-critic-reports`, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` exit 0, then
`timeout -s KILL 1800 ./heroes build selfhost/main.hero -o heroes-next` exit 0; `heroes-next --version`:
`heroes 0.6.0`, **`sources 2ab7ca0a644692a1`**, on the base (`R/seed.log`, `R/stage2.log`).

`R/ij/build.sh` (its log `R/ij/build.log`): the base's `sigguard.c` checked `cmp`-equal to `git show
722ae76bf:runtime/parts/sigguard.c`; the base programs built; the patch applied by a script that asserts its
anchor once (`diff`: 4 lines added after line 326); the D-rebind programs built; `sigguard.c` restored and
**`cmp`-equal to `722ae76bf`'s** again. Every build exit 0 once the headers stood beside the programs (my first
pass at it put them only in the library folders and read `ffi_missing_header`, as the seat's `h/` shows they
must stand beside the `.hero`). Then, before any run:

- **The thunk of my D-rebind build is the seat's, instruction for instruction**: `otool -tV` of
  `_hero_callback_entered` in the seat's `fp-r-flib-nofp` and in my `cr-rflib`, addresses stripped, 151 lines
  each, `diff` printed nothing; my `cr-rebind-0` (injump) the same 151 lines.
- `nm -m cr-rebind-0`: `_hero_tramp_inside` defined in `__TEXT`, `_r_siglongjmp`, `_r_longjmp`, `_r__longjmp`
  present, and **`_siglongjmp` and `_sigsetjmp` imported from libSystem**: the slots D-rebind rewrites.
  `cr-base-0` holds no `hero_tramp_inside` (`nm | grep -c`: 0).

### 1.3 What it prints, Darwin arm64 (`R/ij/run.sh`, its log `R/ij/run.log`)

Every run under `timeout -s KILL 20`, stdout and stderr through `head -c`; 20 runs each, as R3 asks and as the
ffi-pragmatist ran D-rebind's (the compiler-engineer ran `injump` 3 times per runtime):

| program | base runtime | D-rebind as the ffi-pragmatist built it |
|---|---|---|
| **R3's `flib`** (`rflib.hero`, `libsigcb.a`), the control that my D-rebind is the seat's | (00-shared.md: 3 of 3 exit 0) | **20 of 20 exit 134**, stdout `0\|`, stderr *"panic: rflib.answer was called from inside a signal handler ..."*: the seat's own 20 of 20 |
| **`injump`, the compiler-engineer's `ij.c` unchanged** (`sigsetjmp(env, 1)` in the handler, a helper's `siglongjmp` back into it, then the callback) | **20 of 20 exit 0**, `0\|answer ran for 30\|0\|60\|` | **20 of 20 exit 0, `0\|answer ran for 30\|0\|60\|`: the Heroes function ran inside the signal, MISSED** |
| shape 1: `setjmp` / plain `longjmp` within the handler | 20 of 20 exit 0, inside | **20 of 20 exit 0, inside: MISSED** |
| shape 2: `_setjmp` / `_longjmp` within the handler | 20 of 20 exit 0, inside | **20 of 20 exit 0, inside: MISSED** |
| shape 3: a nested signal: the SIGUSR1 handler `sigsetjmp`s, raises SIGUSR2, whose handler `siglongjmp`s back into the still-running SIGUSR1 handler, which calls back | 20 of 20 exit 0, inside | **20 of 20 exit 0, inside: MISSED** |

**So the compiler-engineer's prediction holds, run** (`compiler-engineer.md` § 8, *"`injump/ij.hero` against
D-rebind as specified exits 0, `answer ran for 30` (missed)"*): D-rebind as built gives the in-handler jump
exactly the base's answer, the Heroes function inside the signal, in silence, 20 of 20, and so does every
shape beside it I tried. Its own condition, *"D-rebind refusing `injump` 3 of 3 as it stands"*, is not met.
The library in each row imports nothing but the C library's own names (`sigaction`, `sigsetjmp`/`setjmp`,
`siglongjmp`/`longjmp`, `raise`, by `nm -m`), so **this residue is not the one the ffi-pragmatist's
route-K sentence names** (*"the class of a library that installs a handler past the C library's
installers"*, its § 5.3): it is a library that installs through `sigaction` and jumps through the C library's
own `siglongjmp`. **D-tramp carries the same rule by its source** (`249-ffi-pragmatist/c/tramp/ipose_tramp.c:52-62`,
each jump and `setcontext`/`swapcontext` setting the depth to zero first): a reading, unrun by me.

Crash reports, as 00-shared.md's rule asks (`python3 -I ips.py ~/Library/Logs/DiagnosticReports` over my binary
names, a sample): `('cr-rflib', 'SIGABRT', 'abort') 12` for its 20 deaths, the ASan rows below 4, 3 and 1, all
`abort`; **none for any `injump` binary**, which exited 0; none `SIGKILL`, none `lock`. `pgrep -fl` after:
nothing of mine running.

### 1.4 Found running the control: D-rebind as built kills every `--sanitize` program before `main`

The ffi-pragmatist ran `--sanitize` at `-O2` and `-O0` for D-tramp (3 of 3 each refused with R1's words) and
**not for D-rebind** (its § 3.7 names none). Run here, `rflib.hero` under D-rebind built `--sanitize` and
`--sanitize -O0`: **3 of 3 and 3 of 3 exit 134 with `AddressSanitizer:DEADLYSIGNAL`**, *"ERROR:
AddressSanitizer: stack-overflow"*, frame `#0 sigaction+0x4 (libclang_rt.asan_osx_dynamic.dylib)`, frames
`#1` to `#255` all `r_sigaction+0x24` (`R/rebind/asan-e.txt`, 20,140 bytes, one more run). **The cause, run
apart in C** (`R/asanprobe/q.c`, whose `main` first does `write(1, "main reached\n", 13)` and then one
`sigaction(SIGUSR1, NULL, &q)`): built `-fsanitize=address` alone, exit 0 and both lines; linked with
`rebind.o` alone, exit 0 and both lines; **both together, exit 134, the same stack overflow, and `main
reached` never printed**. D-rebind's constructor takes the real `sigaction` from `dlsym(RTLD_DEFAULT,
"sigaction")`, which under ASan answers ASan's interceptor, then rewrites the `_sigaction` slot of every image
outside the shared cache, ASan's runtime among them, so ASan's call to the C library's `sigaction` comes back
into `r_sigaction`: a loop before `main`. **So under D-rebind as built every program built `--sanitize` dies
before `main` with a false message**, and the `run` form judges every case under `--sanitize` and
`--sanitize -O0` (`suite_run.hero:238`). A repair is easy to name (take the real `sigaction` from libSystem by
a handle, or leave ASan's image unrewritten) and is unbuilt; until it is built and run, D-rebind's approval
rests on no `--sanitize` run.

**Guard Malloc is not ASan's case**: the `run` form runs each `-O0` binary again under
`DYLD_INSERT_LIBRARIES=/usr/lib/libgmalloc.dylib` (`tests/harness/freed_bytes.hero:57`, `:90`); under it, 3 runs
each (`R/ij/gmalloc.log`): `cr-rflib` (D-rebind) 3 of 3 exit 134 with R1's words, `cr-rebind-0` 3 of 3 exit 0
inside, `cr-base-0` 3 of 3 exit 0 inside: the same as without it.

### 1.5 The same trade on Linux arm64: yes, the reset opens the in-handler miss there too

The brief's second question. The ffi-pragmatist approves B' on Linux *"only with the C library's exits (the
jumps, `setcontext`, `swapcontext`) dropping the depth"*, a variant that exists only as its C model
`linux/c/inst_B2.c` (each of `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk`, `setcontext`, `swapcontext`
setting `hero_b_depth = 0`, read); and A, whose runtime build `linux/c/route/siginstall.c:104-130` defines
`longjmp`, `_longjmp` and `__longjmp_chk`, each unblocking the marker before the jump, and **not** `siglongjmp`
(read, and `nm` below). Copied into `R/linux/c/` (`inst_A.c` `ae30e8152925b5fa`, `inst_B2.c` `90c2064c59e0feaa`,
`route/siginstall.c` `a5bcf903232e3283`, `route/sigguard.c` `5d2e79952d7f27d9`, `route/runtime.c`
`b77368da02ea5399`; `diff` against the base's: the probes through the C library's own `sigaction`, the marker
question placed where D-rebind's is, the part included). Two containers of `heroes-linux-arm64:latest`, each
`git archive 722ae76bf | timeout -s KILL 3600 docker run -i --rm --name critic-249-r<n> -v R/linux:/out ...`,
`docker exit 0` each, `docker ps` empty after each; inside: *Debian GLIBC 2.41-12+deb13u3*, *Debian clang
22.1.8*.

**Part 1, the C models** (`R/linux/run.sh`, `R/linux/log.txt`, `critic-249-r1`): each library built apart
`-O2 -fomit-frame-pointer` (its handler `stp x30, x19` and no frame record, `objdump`), linked into
`R/linux/c/ijdrv.c`, whose callback asks the route's own question `hero_mask_inside()` as the seat's
`setctx_l.c` does. `nm -u` of each object names its jump: `j0` (the compiler-engineer's `ij.c`) `__sigsetjmp
siglongjmp`; `j0f` (the same, `-D_FORTIFY_SOURCE=2`) `__longjmp_chk __sigsetjmp`; `j1` `_setjmp longjmp`; `j1f`
`__longjmp_chk _setjmp`; `j2` `_longjmp _setjmp`; `j3` (the nested shape) `__sigsetjmp siglongjmp`; `jn` none.

| the library's handler, then a callback while it still runs | A, `SIGRTMAX` | A, `__libc_allocate_rtsig(0)` | B' with the exits dropping the depth |
|---|---|---|---|
| `jn`, no jump (the control) | INSIDE | INSIDE | INSIDE |
| `j0`, `sigsetjmp(env, 1)` / `siglongjmp` | INSIDE | INSIDE | **MISSED** |
| `j0f`, the same fortified (`__longjmp_chk`) | INSIDE | INSIDE | **MISSED** |
| `j1`, `setjmp` / `longjmp` | **MISSED** | **MISSED** | **MISSED** |
| `j1f`, the same fortified (`__longjmp_chk`) | **MISSED** | **MISSED** | **MISSED** |
| `j2`, `_setjmp` / `_longjmp` | **MISSED** | **MISSED** | **MISSED** |
| `j3`, a nested handler's `siglongjmp` into the running one | INSIDE | INSIDE | **MISSED** |

Every row's second line, *"then a correct callback, no signal"*, read `not inside`: no false refusal anywhere.

**Part 2, Heroes, A built into the runtime** (`R/linux/run2.sh`, `R/linux/log2.txt`, `critic-249-r2`; seed exit
0, stage 2 exit 0, `sources 2ab7ca0a644692a1`; `ij.hero` with the header beside it, each library above): the
A program's `nm` defines `T sigaction`, `T longjmp`, `T _longjmp`, `T __longjmp_chk`, no `siglongjmp`.

| library | base runtime, 3 runs | A built into the runtime |
|---|---|---|
| `jn` | exit 0, `answer ran for 10` (inside) | 5 of 5 exit 134, R1's words |
| **`j0`, the compiler-engineer's `ij.c`** | exit 0, inside | **20 of 20 exit 134, R1's words** |
| `j0f` | exit 0, inside | 5 of 5 exit 134 |
| **`j1`, `setjmp` / `longjmp`** | exit 0, inside | **5 of 5 exit 0, `answer ran for 10`: MISSED** |
| **`j1f`, fortified** | exit 0, inside | **5 of 5 exit 0, `answer ran for 10`: MISSED** |
| `j3` | exit 0, inside | 5 of 5 exit 134 |

So, run: **B' with the exits dropping the depth misses every in-handler jump shape** (6 of 6, in the only build
of that variant there is, the seat's C model); **A misses the in-handler jumps that save no mask** (plain
`longjmp`, `_longjmp`, and `longjmp` fortified into `__longjmp_chk`), because its own definitions unblock the
marker before the jump, and refuses those that restore a saved mask (glibc's `siglongjmp`, which A does not
define, gives back the mask `sigsetjmp(env, 1)` saved inside the handler, the marker blocked in it: my reading of
why, consistent with the runs). The compiler-engineer's own `injump` therefore reads refused under A and missed
under the seat's B'; the shape beside it, one word changed (`setjmp`/`longjmp`), is missed by both.

**Where this leaves the depth's three ways, each now run on its shapes**: keep the depth across a jump (the
compiler-engineer's B'): a correct program refused after a jump out (its § 2.3, three images); drop it at the
jump (D-rebind, D-tramp by source, the seat's B', A's `longjmp` release): a Heroes function run inside the
signal after a jump within (above); confirm it by the frame records (B''): exact on Darwin, a miss on Linux
where the frame register holds a signal-frame address (`kmiss`, its § 2.3b). No variant built is free of a
run miss or a run false refusal on Linux; on Darwin B'' is, and it is built only with the executable's
definitions, which miss every dylib (its § 2.3c, 9 of 9).

### 1.6 The 2494 programs under D-rebind, which no seat ran

A and B' each had the fifteen 2494 programs run (A: the `run` form narrowed, 13 and 2; B': five move, the
compiler-engineer's § 2.4); D-rebind none. Run here: D-rebind put **inside a copy of the runtime**, where a
landing would put it (`R/rt-rebind/runtime`: the seat's `rebind.c` as `parts/rebind.c`, its one
`#define _DARWIN_C_SOURCE` dropped, included after `sigguard.c`, the thunk patched as in § 1.1; `diff -r`
against the base shows those three changes; `clang -std=gnu11 -Wall -fsyntax-only` exit 0), handed to my stage
2 by `HEROES_RUNTIME`; each program built at `-O0` and `-O2`, run under `timeout -s KILL 30`, stdout then `!`
and stderr compared with its `.expected` by `cmp` (the compiler-engineer's `judge.sh`, copied; `R/g2494/all.sh`,
`R/g2494/verdicts.txt`). `nm` of a D-rebind binary holds `hero_tramp_inside` (1).

- **Base: 30 of 30 read their `.expected`.**
- **D-rebind: 26 of 30.** The four that move are
  `a-handler-that-allocates-inside-the-allocator-stops-its-program-by-name` and
  `a-handler-that-lends-inside-the-lend-tracking-stops-its-program-by-name`, at both levels: still exit 134,
  now *"... on_signal was called from inside a signal handler ..."* (R1's words) where D's words were expected.
  So under D-rebind, as under A, **D's guard keeps no falsifier among the goldens**, and a landing moves two
  goldens. The three cases the kernel calls directly keep their *"ran as a signal handler"* words (the probes
  learn the trampoline's return, as the seat's `direct` row read under D-tramp), unlike B', which moved them.

## 2. Platform by platform: does a candidate hold, by the reports' measurements and mine

The author's answer splits the sitting: question 3 and the blind seat run only where no candidate holds. Under
R3 as written (*"Never a known cost"*) a candidate holds where it refuses R3's two cases and leaves no run
residue of 2522's class. **By the measurements, none holds on any POSIX platform:**

| platform | nearest candidate | R3's two cases | what is run against it and not refused or wrongly refused |
|---|---|---|---|
| **Darwin arm64** | D-rebind (the ffi-pragmatist's) | refused, 20 of 20 static and two-level dylib, storms 20 of 20 and 5 of 5 (its § 3.7) | **in-handler jumps, 4 shapes, 20 of 20 each MISSED** (§ 1.3, here); an installer found by `dlsym(RTLD_DEFAULT)` MISSED (its § 3.7); **every `--sanitize` program dead before `main`** (§ 1.4, here); shared-cache images and a raw `__sigaction` with its own trampoline (read) |
| Darwin arm64 | B'' (the compiler-engineer's) | refused for a static archive (20 of 20) | every dylib MISSED, 9 of 9 (its § 2.3c): its Darwin arm is the executable's definitions |
| **Linux arm64** | A with `SIGRTMAX` (built into the runtime) | refused 20 of 20, storms 20 of 20 | vetoed by its builder (`swait_rt.c`, exit 192); **in-handler `longjmp`, 5 of 5 and fortified 5 of 5 MISSED** (§ 1.5, here); an unblocking handler (`hunblock` 1, 0, 0), a raw `rt_sigaction`, `dlsym(RTLD_NEXT)` MISSED (its table) |
| Linux arm64 | A with `__libc_allocate_rtsig(0)` (the seat's approval) | **never run**: the variant is the C model's alone (`inst_A.c -DFP_ALLOC_TOP`; `route/siginstall.c` holds no such switch, `grep`) | the C model misses the in-handler `longjmp` shapes (§ 1.5); a program naming 64 by number dies (`hard64.c`) |
| Linux arm64 | B' with the exits dropping (the seat's approval) | **never run**: C model only (`inst_B2.c`) | **every in-handler jump shape MISSED, 6 of 6** (§ 1.5, here); raw `rt_sigaction`, `dlsym(RTLD_NEXT)` |
| Linux arm64 | B' / B'' (the compiler-engineer's) | refused 20 of 20 | B': three correct programs refused after a jump out; B'': `kmiss` MISSED |
| **Linux x86-64** | C, emulated Debian only | R3's library built apart: inside (C reaches none of it) | the tree's `atexit` and `fork` cases red at every level through Debian's libc (its § 3.1, 3.4); on the CI's Ubuntu leg C's effect is a prediction, unrun; B' and B'' there: `flib` refused, B' 3 false refusals, B'' `kmiss` missed (its § 2.3b) |
| **Windows** | none built; no POSIX signal | not applicable (247's R10, 248's rows, carried) | owed to the CI's leg; the box was off |

So, by the author's own split, **question 3 and the blind seat run**. What could still make a candidate hold is
named in item 7 (a jump told apart from a jump out by its target's frame) and in the compiler-engineer's
pairing (D-rebind's reach with B''-style confirmation), both unbuilt.

## 3. The brief's items, answered

### Item 1: routes, and whether each recommended one was built and run on its cases

| route | who recommends | built | run on the cases named | named and not run (here: what I ran) |
|---|---|---|---|---|
| D-rebind | ffi-pragmatist approves (Darwin); compiler-engineer approves its reach only paired with B''-style confirmation | yes, as a static archive in a Heroes program's group (seat); inside the runtime (here) | rows, `dlopen` reach, SDL2→SDL3, Go, R3 20 of 20 static and dylib, storms, hardened runtime | `injump` (here: **missed 20 of 20**); `--sanitize` (here: **dies before `main`**); the 2494 programs (here: 26 of 30, D's two lose their falsifier); libuv, ncurses, GLib, OpenSSL, sqlite under it (by its logs: none); the macOS versions its prediction names; any precedent (the historian's report ended at 00:00:56, before D-rebind existed) |
| D-rebind + B''-style confirmation | compiler-engineer (*"the Darwin arm I would approve"*) | **no** (*"unbuilt as a pair"*) | none | all |
| D-tramp | objected by both building seats | yes (dylib) | rows, R3 20 of 20, storms, `--sanitize`, `direct`/`dlocale`/`hold`, Go | `injump`: its source zeroes the depth at every jump (`ipose_tramp.c:52-62`), so by its rule it misses; unrun |
| A, Linux, `SIGRTMAX` | vetoed by its builder | yes (runtime) | everything its brief named | in-handler jumps (here: `longjmp` missed) |
| **A, Linux, `__libc_allocate_rtsig(0)`** | **ffi-pragmatist approves** | **C model only** | `swait_rt`, `hard64`, `rawrow` (through `sigaction`) | R3's two Heroes cases, storms, `--sanitize`, the `run` form, the census inside a Heroes program; whether a library caches `SIGRTMAX` before the allocator moves it (its own condition) |
| A, Darwin, SIGEMT | objected (builder, historian) | 248's dylib, SIGEMT | its rows | none |
| B' (keep the depth) | objected by its builder | yes (runtime) | all named | none |
| B'' | objected on Linux; approved as Darwin's depth when paired | yes (runtime) | R3, depth 12, `kmiss`, 2494, rows | the `runtime` suite (*"the same statics) not run separately"*); `injump` on Linux; rows (4), (5), (7) (*"(not run)"*) |
| **B' with the exits dropping** | **ffi-pragmatist approves (Linux)** | **C model only** (`inst_B2.c`) | C rows (frameless static and shared, jumps out, `setcontext`) | R3's two Heroes cases, storms, `--sanitize`, 2494, the census inside a Heroes program; in-handler jumps (here: **6 of 6 missed**) |
| C | compiler-engineer approves as a complement; ffi-pragmatist objects as a route; historian approves | yes | three cases emulated, fifteen programs on three images, compiler tests, `cache`, `runtime` | `run` whole (*"Owed, before a landing"*); instructions on x86-64 |
| I-onstack, I-mask | refused (248), veto re-run | 248's | `gcreal` 5 of 5 exit 139 on Debian's `libgc1` too | none |
| D-imports | vetoed (ffi-pragmatist), objected (compiler-engineer) | no | the `dlopen` program at the base | none |
| the sweep | objected | yes (`sweep.c`) | its rows, cost | none |
| a refusal at a library's first install | objected | no | read from the census's start-up installs | none |
| `__in_sigtramp` as a mark | cannot serve (`dlsym` NULL, process-wide) | probe | yes | none |
| seccomp, syscall user dispatch, shadow stacks | questions | no | read; `/proc` in both images (no shadow stack) | none |
| route K | the author's | none | none | none |
| **a jump told apart from a jump out** (unlisted, item 7) | nobody | no | none | none |

### Item 2: answered in § 1.

### Item 3: claims asserted and not measured with the command that settles them

- **The approved candidates' cases** (above): D-rebind's census rows on Darwin are Go and SDL alone; the
  ffi-pragmatist's two Linux approvals (A by the allocator, B' with the exits) never ran R3's two cases as Heroes
  programs, never stormed, never `--sanitize`d, never ran a golden: each rests on a C model. The
  compiler-engineer's approved Darwin pairing is unbuilt.
- **The SDL prediction** (*"takes signal 36 (`SIGRTMIN + 2`) for its VT switch, never 64"*): **unrun**, and its
  *never* is conditional. The historian (`SDL_evdev_kbd.c` at release-3.2.10) and the ffi-pragmatist's own § 2.1
  (*"A leaves the marker's action at default, a library scanning for a default action sees it free"*) agree that
  SDL takes the first number from `SIGRTMIN + 2` whose action is the default: under A with `SIGRTMAX` it takes 64
  whenever 36 to 63 are all taken; under the allocator variant glibc's `SIGRTMAX` itself moves to 63 (its run),
  so SDL's loop to `SIGRTMAX` cannot reach 64, by that reading. What would settle it: a Linux machine with a
  console running `SDL_VIDEODRIVER=kmsdrm`, none here. The historian's open question, *where Debian's SDL3 imports
  `sigtimedwait` from*, the ffi-pragmatist answered from Debian's 3.2.10 source (`SDL_waylanddatamanager.c:79`, a
  pending SIGPIPE drained): a reading, not a run, and not a real-time signal.
- **C's cost**: *"0.6 to 0.7 % of text"* is `size`'s text column for the runtime's object (+0.69 %) and one linked
  program (+0.62 %) at `-O2` in the emulated image, each built once; the program's own unit grew 6.3 %; **no
  instruction was counted on x86-64**, the one machine where the word changes code. The CI prediction (*"815
  passed, 0 failed"*) rests on Ubuntu 26.04's libc keeping records, which the historian read for 24.04 only; the
  CI's own base run, `atexit` and `fork` green at `-O0` (its log, the three FAILs at `-O2` alone), is that leg's
  measurement of the premise; the prediction itself is unrun, and so is `run` whole.
- **B''s floor** (`runtime-bp.txt` in the compiler-engineer's folder, read: *"137 against a floor of 109 ...
  passes a walk that misses 28 of them"*): told under B', **not under B''**, and the 28 objects' thread audit the
  floor asks for is undone.
- **"No first entry of the census falls in that residue"** (ffi-pragmatist § 5.3) is about installers past the C
  library's names. **Whether any census library jumps within a handler and then calls back was never asked**:
  OpenSSL's `ill_handler` and curl's `alarmfunc` leave by `siglongjmp` (out, by the historian's pages); none was
  searched for a jump that stays inside a handler.

### Item 4: the escape, against what each route's refusal prints

- **D-rebind**: its refusals print R1's words (run: `rflib` 20 of 20; the two moved goldens print
  *"was called from inside a signal handler"*), and it defines no C library name (`nm`: external names
  `hero_tramp_inside`, `hero_tramp_through`, `hero_tramp_found_mark` alone). So the ffi-pragmatist's reading,
  *none of the four*, holds for its refusal and its names. What it does need, none of them one of the four:
  two goldens moved (§ 1.6), the ASan loop repaired (§ 1.4), and interfaces no header declares (`__sigaction`,
  `__sigreturn`, flag `0x400`, the token), which is the historian's objection to D-tramp's trampoline,
  D-rebind's own (item 7).
- **A**: a reserved signal (a binding's author must learn it), and a widened `ffi_defined_elsewhere` on Linux
  for the names A defines (`longjmp`, `_longjmp`, `__longjmp_chk`, `bsd_signal`, ...): the mechanism run by the
  compiler-engineer for B''s `siginterrupt`, **not run through the member for A's names**; on Darwin a file
  beside every program (run).
- **B', B''**: a widened diagnostic, run (`siginterrupt` refused, its § 4.1).
- **C**: none, no words.
- **Route K** puts a residue in words somewhere; spec § 13's line (*"No function of the program is a signal
  handler"*) is where the rule lives, so a told residue is likely a spec sentence: the escape. The historian's
  question-3 reading favours new refusal words (the escape); the compiler-engineer's says no route built needs
  new words. The two do not contradict on a route; they differ on question 3, which now runs (§ 2).
- **The words themselves moved tonight**: defect 2533 is **CLOSED**, `2026-11-15T06:57:05Z`, *"Fixed by
  0652048d2"*, an ancestor of `main` (`git merge-base --is-ancestor`); `HERO_SIGNAL_REMEDY` on `main` now reads
  *"No function of the program is a signal handler: ... Ctrl-C, SIGTERM and `hero_net_ask_stop() -> ()` only make
  ..."*, the spec's line. So the order the briefs asked the synthesis to state is already *2533 first*, as the
  compiler-engineer recommended; every refusal in every seat's run (and mine) prints the old, false remedy,
  because the base predates `0652048d2`. `git diff 722ae76bf main -- runtime/parts/sigguard.c` moves the comment
  and the macro alone, so `hero_callback_entered`, where every route patches, is unchanged on `main`.

### Item 5: route K

**R3 as it stands** (`docs/panel/247-*.md:191-195`): *"What R1 and R2 leave is a defect of its own, `blocking`,
filed in the hour 2494 closes: a Heroes function a C handler calls through frames the walk cannot read (a library
built without frame records, `flib-nofp`), which allocates nothing and so passes D: `c-fliblocale` dies by
SIGKILL in silence on Darwin and runs inside the signal on Linux arm64 (the ffi-pragmatist's § 4). Its cases are
those two. Never a known cost: it blocks M-core-packages' tag like any `blocking` defect."* *"Its cases are those
two"* names the falsifiers, not the class: a residue a route leaves is still *"what R1 and R2 leave"*, so it stays
`blocking` under R3 whichever route lands; the seats' reading holds.

**The change both building seats ask**, as a sentence the author can answer yes or no: *"May defect 2522 close
with a named residue told rather than refused (on each platform, the programs whose library's handler calls a
Heroes function in a way the adopted route cannot see), so that the residue no longer holds M-core-packages'
tag, R3 amended to say so and the residue written where spec § 13 states the rule?"* The ffi-pragmatist's own
residue sentence (*"the class of a library that installs a handler past the C library's installers"*) **is too
narrow for what was run**: the in-handler jump installs through `sigaction` and jumps through the C library's own
`siglongjmp`. The residue each candidate leaves, run:

- **D-rebind (Darwin)**: in-handler jumps (four shapes); `dlsym` installers; shared-cache images; a raw
  `__sigaction` with its own trampoline; and, until repaired, every `--sanitize` program.
- **B'' (Darwin)**: every dylib.
- **A (Linux)**: in-handler jumps that restore no mask; a handler that unblocks before calling back; a raw
  `rt_sigaction`; `dlsym(RTLD_NEXT)`; a program using the marker dies.
- **B' with the exits (Linux)**: every in-handler jump; a raw `rt_sigaction`; `dlsym(RTLD_NEXT)`.
- **B'' (Linux)**: a frame register holding a signal-frame address (`kmiss`); raw; `dlsym(RTLD_NEXT)`.
- **C (x86-64)**: libraries built apart (R3's own two cases), frames deeper than eight, a libc without records
  (Debian's `exit` and `fork`).
- **Windows**: no walk and no probe (247's R10); nothing of 2522's shape measured there tonight.

### Item 6: defects found beside the question

- **2558** is filed (`gh issue view 2558`: `OPEN`, `class:blocking`, *"the runtime's null arm kills a fault a
  library's own handler would recover"*).
- **The tree's own `atexit` and `fork` cases run the Heroes function inside the signal on Debian x86-64 at every
  level** (the compiler-engineer's § 3.1, emulated; its `amd64/base.log`). `gh issue list --state all --search`
  for *Debian atexit fork frame* and *x86-64 glibc frame records walk* printed nothing. 00-shared.md counts it in
  2522's class; the synthesis says whether it is 2522's or its own, and no CI leg runs Debian x86-64.
- **The SIGURG probe's window** (the compiler-engineer's § 5, a reading): for the instant of the two raises, a
  SIGURG aimed at the process runs the probe, not Go's handler or a program's own; unmeasured, unfiled (searched
  *SIGURG probe borrowed*, *probe SIGURG lost*: none). A question until a program counting its own SIGURGs, a
  sender thread and a first crossing are run.
- **The compiler's own tests fail 8 in every seat's copy** (the compiler-engineer's `ctests-base.txt`, lines
  301-312, 1543, 2415-2430): `git ls-remote` inside the tests climbs to the copy's `.git` file that 00-shared.md's
  recipe writes (*"fatal: gitfile does not point to a valid repository: .../tree/.git"*). The recipe's, not the
  tree's; whether tests that git can escape are hermetic enough is the coordinator's to judge.
- **D-rebind's ASan loop** (§ 1.4) is the route's, not the tree's.
- **A label to read correctly**: the first line of the ffi-pragmatist's `logs/rebind-darwin.txt`,
  *"libsystem_platform's __in_sigtramp found by dlsym: 6"*, is `trows.c:43`'s D-tramp label printed over
  `rebind.c:86`'s `rebound`, six rewritten slots; it is not `__in_sigtramp`.

### Item 7: the questions the sitting should have asked

1. **Can a jump out of a handler be told from a jump within it?** Every way of handling the depth run so far
   pays one side: keep it (a correct program refused), zero it (a Heroes function inside the signal), confirm it
   by frame records (a miss on Linux). A jump's target is known before it is taken: the definitions (Linux) or
   the rewritten slots (Darwin) of `setjmp`, `_setjmp`, `sigsetjmp`/`__sigsetjmp` could record each buffer's
   caller frame and tail-call the C library's own (clang's `musttail`), and a jump could then drop only the
   depth entries whose wrapper or trampoline frame lies deeper than its target's. Who: the ffi-pragmatist with
   the compiler-engineer. How: build it into D-rebind and into the compiler-engineer's B'; run `injump` and the
   three shapes beside it, the twelve depth programs, `kmiss`, R3's two cases and a dylib, on Darwin, Linux arm64
   and emulated x86-64; its own holes (a buffer copied by `memcpy`, a `setjmp` the C library makes inside itself)
   named.
2. **Is D-rebind's trampoline forward-compatible?** The historian's objection to D-tramp (Go handed Darwin its
   own `sa_tramp` and left it in Go 1.12 *"ensuring forward-compatibility"*; xnu validates a per-delivery token)
   reaches D-rebind, which hands the same trampoline to the same private `__sigaction`; and fishhook's
   slot-rewriting on current dyld was read by nobody. Who: the historian (Apple's and fishhook's pages); the
   ffi-pragmatist (D-rebind on each macOS the CI's Darwin legs run, which `gh api` of the jobs' labels names).
3. **What D-rebind does beside every other interposer** (ASan dies, § 1.4; Guard Malloc passes, § 1.4; TSan,
   `DYLD_INSERT_LIBRARIES` tools, a second rebinder in the program).
4. **Whether the census's libraries keep working under D-rebind on Darwin**: libuv's loop, ncurses' handlers that
   call `sigaction` and `kill` from inside themselves, GLib, OpenSSL's prompt, sqlite as the control.
5. **Whether B' or A turns the tree's `atexit` and `fork` cases green on Debian x86-64** (the program's own
   install goes through the definitions, so the depth or the marker sees it): unrun in the emulated image; the
   compiler-engineer's container would run it.

## 4. The numbered list

1. **D-rebind misses the compiler-engineer's `injump`, 20 of 20.** Command: `R/ij/build.sh` (D-rebind as the
   ffi-pragmatist built it: `rebind.o` byte-identical to its own, by `clang -O1 -Wall`; the thunk instruction for
   instruction its own) and `R/ij/run.sh`. Printed: `cr-rebind-0` 20 of 20 exit 0 `0|answer ran for 30|0|60|`;
   control `cr-rflib` 20 of 20 exit 134 with R1's words. The synthesis must not adopt D-rebind as built, nor say
   it closes 2522's class on Darwin up to `dlsym`, the shared cache and a raw `__sigaction`.
2. **The miss is a class, not one program.** Command: `R/ij/src/ijv.c` shapes 1 to 3, the same scripts.
   Printed: plain `longjmp`, `_longjmp`, and a nested handler's `siglongjmp` into the running one, each 20 of 20
   exit 0 inside under D-rebind. D-tramp carries the same rule by source (`ipose_tramp.c:52-62`), unrun. The
   synthesis must not word the residue as *"a library that installs past the C library's installers"*.
3. **The same trade on Linux, run.** Command: `R/linux/run.sh` and `run2.sh` in `critic-249-r1` and `-r2`
   (`docker exit 0`, none left). Printed: B' with the exits dropping (the seat's `inst_B2.c`) MISSED 6 of 6 jump
   shapes; A (model and runtime) MISSED the shapes that restore no mask (`ij.hero` with `setjmp`/`longjmp` 5 of 5
   exit 0 inside, fortified 5 of 5), refused `ij.c` 20 of 20 and the nested shape. The synthesis must not adopt
   B' with the exits or A without that residue named.
4. **D-rebind as built kills every `--sanitize` program before `main`.** Command: `cr-rflib-asan`,
   `cr-rflib-asan0`, and `R/asanprobe/q.c` with and without `rebind.o` and `-fsanitize=address`. Printed: 3 of 3
   and 3 of 3 exit 134, *"AddressSanitizer: stack-overflow"*, `r_sigaction` 255 frames deep; `main reached` never
   printed with both linked, printed with either alone. The synthesis must not adopt D-rebind before a repair is
   built and the `run` form's `--sanitize` arms run under it.
5. **D-rebind moves two goldens, and D's guard loses its falsifiers.** Command: `R/g2494/all.sh` (D-rebind inside
   the runtime, `HEROES_RUNTIME`). Printed: base 30 of 30, D-rebind 26 of 30, the allocator's and the lend
   tracking's cases at both levels now R1's words. The synthesis must say so for D-rebind as it was said for A.
6. **Two of the ffi-pragmatist's three approvals rest on C models.** Command: `grep -n 'FP_ALLOC'
   linux/c/route/*.c` (nothing) and its § 2.1 table; `inst_B2.c` is the only build of B' with the exits. Printed:
   no runtime build of either, so no R3 Heroes case, storm, `--sanitize` or golden ran under them. The synthesis
   must not adopt either as built.
7. **The compiler-engineer's approved Darwin arm is unbuilt.** Command: its § 7, *"unbuilt as a pair"*. The
   synthesis must not adopt D-rebind with B''-style confirmation until it refuses `injump` and the three shapes
   and refuses none of the twelve depth programs, on this Mac.
8. **No candidate holds on any POSIX platform** (§ 2), so by the author's split question 3 and the blind seat run.
9. **D-rebind has no precedent reading.** Command: `grep -c -i rebind historian.md` (0), `stat` (historian 00:00:56,
   ffi-pragmatist 00:33:25). The historian's D-tramp objection reaches its trampoline. The synthesis must not file
   prior art for D-rebind, nor adopt it without that reading (item 7.2).
10. **Defect 2533 landed first, tonight.** Command: `gh issue view 2533` (`CLOSED 2026-11-15T06:57:05Z`, *"Fixed by
    0652048d2"*), `git merge-base --is-ancestor 0652048d2 main`. The synthesis states the order as done, says the
    seats' refusals print the old remedy, and lands any route on `main`, whose `hero_callback_entered` is the
    base's.
11. **The SDL prediction is unrun and conditional** (item 3); it concerns the vetoed `SIGRTMAX` variant. A
    console with `kmsdrm` would settle it; none here.
12. **C's cost is text size of two objects, once each; no instruction counted on x86-64; `run` whole unrun.** The
    synthesis must not write C's cost as measured in instructions, nor C as landed before `run` whole.
13. **B''s floor was told under B', not B''**, and its 28 new objects' thread audit is undone.
14. **A's widened diagnostic is unrun for A's names** (`longjmp`, `_longjmp`, `__longjmp_chk`, ...); B''s is run.
15. **Route K's sentence**: item 5 gives it in a form the author can answer, with each candidate's run residue,
    the in-handler jump among them.
16. **Unfiled, beside the question**: the tree's `atexit`/`fork` cases on Debian x86-64 (2522's or its own: the
    synthesis says); the SIGURG probe's window (a question, unmeasured); the eight compiler tests a copy's `.git`
    file breaks (the recipe's); the D-rebind log's mislabelled first line. 2558 is filed.
17. **The question not asked**: can a jump out be told from a jump within, by its target's frame (item 7.1);
    who and how are there. It is the one route measured tonight to be needed by every candidate that keeps a
    depth or a marker.

Finished 2026-11-15 at 08:43 CET (`date`: *Sun Nov 15 08:43:02 CET 2026*). Every container of mine ended (`critic-249-r1`,
`critic-249-r2`, `docker exit 0` each, `docker ps` empty after each); `pgrep -fl` printed nothing of mine;
`R/tree/runtime/parts/sigguard.c` is `cmp`-equal to `722ae76bf`'s. No file written outside the repository's root by me:
everything is under `.claude/worktrees/p249-scratch/critic/reports/` and this report; the system's crash reporter wrote its `.ips`
reports for my aborting runs into `~/Library/Logs/DiagnosticReports`, as it does for every seat's, and I read them there. No `go`,
no `lldb`, no paid run, no GitHub write (`gh issue view`, `gh issue list` and `git` reads only).
