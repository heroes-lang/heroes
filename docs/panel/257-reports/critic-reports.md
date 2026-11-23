# Panel 257, the completeness critic: SECOND READING (the reports, before the synthesis)

Written as I go from 2026-11-23 16:1x by this Mac's clock (`date`). A fresh agent: I read `critic-second.md`, `critic.md`, `00-shared.md`,
`resume.md`, the warden's, the engineer's and the ffi-pragmatist's reports with `ce-clauses.md` and `ffi-clauses.md`, `spec-w*.txt`, `spec-x1.txt`,
`spec-x2.txt` (and their diffs), and my colleague's first reading `critic-briefs.md` (its table and repair list; I re-ran only what I doubted).
My copies: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/critic/` (`828567a30`, frozen; seed `./heroes`, stage 2 `./heroes-next` built
by the coordinator) and `.../critic-landed/` (`523d35537`; I built its seed compiler there). The Linux container is `p257-critic-1`
(`heroes-linux-arm64:latest`: `uname -m` aarch64, `ldd --version` Debian GLIBC 2.41-12+deb13u3, Debian clang 22.1.8). Scripts and my own programs:
`257-reports/programs/critic/`. No paid run, no credential printed, nothing outward, no file outside the repository root (my scratch is under
`p257-scratch/critic/cw/`, `TMPDIR` set there for every runner: the engineer's `run.sh` would otherwise have written to `/tmp`).
Every cell is `3 of 3` runs, `timeout 20`, both streams through `head -c 100000`, unless it says otherwise.

(sections are appended as each question is run)

## 0. What I did, in the order I did it (each by its command; scripts are `programs/critic/scripts/`, my programs `programs/critic/mine/`)

1. Read the brief, `critic.md`, `00-shared.md`, `resume.md`, then every report whole except `critic-briefs.md` (my colleague's: its table and its repair list
   read, nothing re-run that the seats' cells already re-ran) and the arms' older diffs. Applied and measured the two candidates (§ 8).
2. **The landing check on Linux, mine** (§ 1): the landed tree's seed compiler built in the container, a no-UI program built with it.
3. **The cross-run** (§ 2): the engineer's programs, with the runner she wrote (`programs/compiler-engineer/run.sh`, portable, her own), on **this Mac**
   against three builds (today = the frozen runtime; C4r = panel 255's Darwin route, the two files of the evidence archive over `parts/`, `md5` equal to
   the shared brief's table; landed = `523d35537`), and the ffi-pragmatist's programs (her `run.sh`, `depth/sweep.sh`, `libs.sh`, all hers, `libs.sh` already
   has a Linux arm) in the **container** against three builds (today; C2679 = the evidence archive's `sigjump.c` over `parts/`, `md5` `ba8487af...`; landed
   with ONE initializer, § 1). Every cell `-O0` plain, 3 runs, `timeout 20`, both streams `head -c 100000` (the runners do it), the exit from `PIPESTATUS`.
   The programs the other platform could not build are named with the reason (§ 2.3).
4. **My own shapes** (§ 3): `n1` to `n6`, six, each beside a clause of x1 or x2; which of them are defined C, and which is not.
5. The words (§ 4), the clauses (§ 5), scope (§ 6), contradictions between seats and what the sitting did not ask (§ 7), the candidates' numbers (§ 8).

**Compilers and builds.** Darwin: `critic/heroes-next` (stage 2 of the frozen tree, the coordinator's) for `today` and `c4r`, and `critic-landed/heroes` (the seed
compiler I built there: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `./heroes --version` printed `sources 05186f2b38e77a97`, the frozen seed's key:
`git diff --stat 828567a30 523d35537 -- seed` is the seeds' sameness, the engineer's and the ffi-pragmatist's command, not mine) for `landed`. Linux: ONE compiler,
the seed compiler built in the container from the landed tree's seed (`/w/landed/heroes`, the same key), with `HEROES_RUNTIME` naming `/w/rt/today`, `/w/rt/c2679`
or `/w/rt/landed`. **A compiler other than the build's own stage 2** is what the ffi-pragmatist did for `landed` too; no cell below shows it to matter (the
engineer's landed cells, built by the landed stage 2, equal C2679's, and so do mine).

## 1. The landing check the engineer reports: **it is so**, on the landed tree and on `rebel-round`'s head

- `git -C /Users/joseph/Temp/heroes/heroes-lang log --oneline -1 rebel-round` printed **`cab9f3cf4`** (*"M-rebel-library step 1092: pdf embeds TrueType fonts ..."*), five
  commits past `523d35537` (`git log --oneline -3` shows `b9129d41b`, `523d35537` below it).
- `git show rebel-round:runtime/platform/hero_ui_print_gtk.c | sed -n 609p` printed `    hero_ui_print_settings chosen;` (uninitialised), and
  `git diff --stat 523d35537 rebel-round -- runtime/platform/hero_ui_print_gtk.c` printed **nothing**: the file at `rebel-round`'s head is the landed tree's.
  `git log --oneline -3 -- runtime/platform/hero_ui_print_gtk.c` printed `2e8b81933` (105p7) and `2d8a98396` (105p6), both UI steps.
- **My run** (`programs/critic/scripts/build-landed.sh`, `hello2.sh`, container `p257-critic-1`: `uname -m` aarch64, `ldd --version` Debian GLIBC 2.41-12+deb13u3, `clang --version`
  Debian clang 22.1.8): the landed tree's seed compiler built (`real 0m27.367s`, `heroes --version` `sources 05186f2b38e77a97`), then `heroes build hello.hero -o hello-landed`
  (a four-line program with no UI: `function main()` / `print("hi")`) **failed**: `internal error: the platform layer did not compile`, `compiling the platform layer's
  hero_ui_print_gtk failed: /w/landed/runtime/platform/hero_ui_print_gtk.c:631:28: error: variable 'chosen' may be uninitialized when used here
  [-Werror,-Wconditional-uninitialized]` with the note at `:609:5`, `error: no platform layer`. With ONE initializer (`hero_ui_print_settings chosen = {0};`, `sed` in
  `scripts/lsetup.sh`, `diff -r` of the two runtimes printing that one hunk) every build below succeeded. The frozen runtime built the same program in the same image
  (all my `today` Linux cells).
- **So the engineer's L1 is true, and it is the coordinator's to report** (the brief's wording): `rebel-round`'s head builds no Heroes program in that Linux image. **What I did
  not measure**: whether the CI's `ubuntu-26.04` clang (the workflow's comment, `.github/workflows/ci.yml:355`, says 21.1.8) refuses the line, and whether the CI
  installs what makes the file compile at all (the apt line at `ci.yml:404` is `clang lldb pkg-config libsqlite3-dev libcurl4-openssl-dev`; no GTK): *unrun*, a question, not a premise.
  The sitting's engineer's condition (e) (*"the landed tree builds in a Linux image the CI uses"*) is therefore **not met by any image this sitting has**.

## 2. The cross-run: C10 measured, not read

**What ran where.** *Darwin* (this Mac, Darwin arm64, Apple clang 21.0.0) ran the engineer's programs (`s-mine` = her `blind/` p1 to p10 and `mine/`, `s-ce` = her panel-255 shapes, plus the
ffi-pragmatist's shapes `s-ffi` and the critic's `g2_1`), and the ffi-pragmatist's real-library programs rebuilt in my copy (`ffi-dar`). *Linux* (container, Debian, glibc 2.41) ran
the same source folders and the ffi-pragmatist's real-library programs (`ffi-lin`, `LD_LIBRARY_PATH` set: her `run.sh` does not set it on Linux, my first try read `exit=127`
for every library program, `scripts/lffi.sh`, and `scripts/lffi2.sh` is the corrected one) and her depth sweep `depth/sweep.sh`. Builds: Darwin today / C4r / landed; Linux today / C2679 / landed.
Everything `-O0` plain, 3 of 3 runs (every cell below had its three runs alike; the table prints `x(n)` where they were not, and prints none; `scripts/table.py`).
My cells **reproduce every cell of the seats' tables that I re-ran** (the engineer's Linux today and C2679 cells p1 to p10, `p4s p4sd p4x p4a p4b`, `b1 b2 b3`, `cs2`, `g_3_0`, `x_partial`; the ffi-pragmatist's
Darwin today / C4r / landed cells `p7 g1_0 g1_1 g2 g3 fp3 as_0 as_1 cs_1 sched watchdog-b`, and her depth sweep at -1024, 0, 4096 and 65536): no cell I re-ran read differently from the seat's own; where a seat's cell had a feature mine could not reproduce (the `ORDER` relation on Darwin, § 2.1; the first stderr line cut at 150 characters by the engineer's runner on Linux, § 4), the section says so.

### 2.1 The table (`scripts/table.py cw > logs-table.md`; 70 rows; `run` = exit 0 and the program's own lines; `-` = not run; `BUILD` = did not build on that platform, § 2.3)

| program | Darwin today | Darwin c4r | Darwin landed | Linux today | Linux c2679 | Linux landed |
|---|---|---|---|---|---|---|
| p1 | run | run | run | run | run | run |
| p2 | run | run | run | run | run | run |
| p3 | U | run | run | U | run | run |
| p4 | S | S | S | S | S | S |
| p5 | run | run | run | run | run | run |
| p6 | S | S | S | S | S | S |
| p7 | run/run | S/S | SW/SW | run/run | run/run | run/run |
| p8 | S | S | S | S | S | S |
| p9 | run | run | run | U | run | run |
| p10 | U | run | run | U | run | run |
| p1l | run | run | run | run | run | run |
| p1u | run | run | run | run | run | run |
| p1f | BUILD(1) | BUILD(1) | BUILD(1) | run | run | run |
| p7s | run | run | run | run | run | run |
| p4s | S | S | S | run | run | run |
| p4sd | S | S | S | run | run | run |
| p4x | S | S | S | run | run | run |
| p4a | S | S | S | run | run | run |
| p4b | S | S | S | S | S | S |
| p4c1 | S | S | S | S | S | S |
| p4c2 | S | S | S | S | S | S |
| p4n | run | run | run | run | run | run |
| b1 | run | run | run | run | run | run |
| b2 | S | S | S | S | S | S |
| b3 | T | T | T | T | T | T |
| p1n | run | run | run | run | run | run |
| p10a | U | run | run | run | run | run |
| p10c | U | run | run | U | run | run |
| p10d | U | run | run | U | run | run |
| p10h | U | run | run | U | run | run |
| ORDER=0 HOW=jmp cs2 | run | run | run | run | run | run |
| ORDER=0 HOW=ctx cs2 | K | S | SW | run | run | run |
| ORDER=1 HOW=jmp cs2 | run | run | run | S | S | S |
| ORDER=1 HOW=ctx cs2 | K | S | SW | S | S | S |
| ORDER=0 tal | run | run | run | run | run | run |
| ORDER=1 tal | run | run | run | run | run | run |
| PAD=0 p4d | S | S | S | run | run | run |
| PAD=1024 p4d | S | S | S | run | run | run |
| PAD=4096 p4d | S | S | S | run | run | run |
| PAD=6144 p4d | S | S | S | S | S | S |
| PAD=16384 p4d | S | S | S | S | S | S |
| b_0 | run | run | run | run | run | run |
| b_1 | run | run | run | run | run | run |
| g_1_0 | BUILD(1) | BUILD(1) | BUILD(1) | run | run | run |
| g_3_0 | BUILD(1) | BUILD(1) | BUILD(1) | run | S | S |
| g_3_1 | BUILD(1) | BUILD(1) | BUILD(1) | run | run | run |
| gcopy | U | run | run | U | run | run |
| grefill | U | S | S | U | S | S |
| x_heap | BUILD(1) | BUILD(1) | BUILD(1) | run | run | run |
| x_tls | BUILD(1) | BUILD(1) | BUILD(1) | run | run | run |
| x_dlopen | BUILD(1) | BUILD(1) | BUILD(1) | run | run | run |
| x_partial | BUILD(1) | BUILD(1) | BUILD(1) | U | S | S |
| x_unseen0 | BUILD(1) | BUILD(1) | BUILD(1) | run | S | S |
| acc | BUILD(1) | BUILD(1) | BUILD(1) | BUILD(1) | run | BUILD(1) |
| fixedbugs-2679-the-runtime-says-which-route-decides-a-jump | BUILD(1) | run | run | BUILD(1) | BUILD(1) | run |
| fp3 | run | S | S | run | run | run |
| as_0 | run | S | S | run | run | run |
| as_1 | K | U | U | K | K | K |
| g1_0 | run | S | SW | run | run | run |
| g1_1 | run | S | SW | run | run | run |
| g2 | run | S | SW | run | run | run |
| g3 | U | S | SW | run | S | S |
| cs_1 | K | S | SW | run | run | run |
| c_1 | run | run | run | run | run | run |
| cs_0 | run | run | run | run | run | run |
| c_0 | run | run | run | run | run | run |
| nn | run | S | SW | run | run | run |
| heap | run | run | run | run | run | run |
| mmap | other | run | run | other | run | run |
| g2_1 | run | S | SW | run | run | run |

(`ORDER=` and `PAD=` rows are one program under an environment variable. **`ORDER` names the program's mapping order and not an address relation**: on Darwin `cs2` printed *"the coroutine's stack is BELOW the alternate
stack"* for ORDER=0 and *"ABOVE"* for ORDER=1, on Linux the reverse (the logs print it), so the Linux row `ORDER=1` (coroutine BELOW) is **Darwin's `ORDER=0`**; read by the printed relation, Darwin runs `cs2`
`HOW=jmp` in both relations and Linux aborts when the coroutine's stack is BELOW. `p7` appears twice, once in each source folder, with the same cells.)

### 2.2 Where the two platforms disagree on the same C (`scripts/diffcount.py logs-table.md`: **rows 70; comparable, same-named build: today 59, adopted 59, landed 60; differ: 14, 19, 21**)

For the **landed** column, the 21: `p4s p4sd p4x p4a p4d(PAD 0, 1024, 4096)` (Darwin S, Linux **runs**), `p7 g1_0 g1_1 g2 g2_1 nn cs_1 ORDER=0-ctx` (Darwin SW, Linux runs), `fp3 as_0` (Darwin S, Linux runs),
`cs2 ORDER=1 HOW=jmp` (Darwin runs, Linux S), and **three where both refuse in different words**: `g3` (SW | S), `cs2 ORDER=1 HOW=ctx` (SW | S), `as_1` (**U | K**). Each is a row the engineer or the ffi-pragmatist
had on one platform only; each is stated in a seat's report for ITS platform and is true of it. **No seat's report is wrong; four claims of the sitting are one platform's and have been read as the sitting's**:

1. **The depth test is Linux's.** `p4s p4sd p4x p4a` (an inline jump out of a handler, then a callback from where the signal was raised; a shallow then a deep one; a later handler between; a Heroes `atexit`
   function at `main`'s natural end) **run on Linux on every build and abort (S) on Darwin on every build**; `p4d` with `PAD` 0, 1024, 4096 runs on Linux and aborts on Darwin, and 6144 and 16384 abort on both.
   The ffi-pragmatist's sweep **reproduced by me on Darwin** (`logs-D/ffi-{today,c4r,landed}.log`): `p4` aborts at `D=-1024` (the callback called 960 bytes **above** the handler's frame, `measured=-960`), 0, 4096 and 65536; on **Linux
   `p4` at `D=-1024` runs** (`measured=-976`) and aborts from `D=0` (`logs-L/ffi-today.log`, `ffi-landed.log`). The engineer's *"a callback called from the raise site runs, one about 5 KiB deeper aborts"* is Linux; the
   ffi-pragmatist's *"every refusal holds at every depth"* is Darwin. **Both are measured and neither contradicts the other: they are two platforms' runtimes** (`runtime/parts/siginstall.c`'s prune by frame on Linux; `rebind.c`'s *"no prune
   by the thunk's own frame"* on Darwin, the ffi-pragmatist read it before she measured it).
   **And the Linux half is not stable under the lane's own plan**: `gh issue view 2702` (read only) says R3's design is *"the rules Darwin's C4r carries ... no prune by a trampoline's, wrapper's or callback's own frame"*, carried to Linux:
   when 2702 lands, Linux's `p4s` aborts as Darwin's does and the depth clause of w26 and w27 becomes false on **both** platforms. Even today the landed Linux runtime's own new refusal has a depth test: **`g3` at `D=-1024` runs and aborts
   from `D=0`** (`logs-L/ffi-landed.log`: `landed g3 o0 D=-1024 x3 exit=0 ... D=0 x3 exit=134 ... S`), where Darwin's `g3` (landed, SW) aborts at all four depths.
2. **A switch of context is refused on Darwin and mostly run on Linux, and the green-thread scheduler splits three ways.** `sched` (the ffi-pragmatist's SIGALRM handler that `swapcontext`s to the next green thread, every thread switched back
   to, the three sums right; my run on Linux today: `preempted true`, `thread 0 5000000050000000`, `thread 1 11250000075000000`, `thread 2 20000000100000000`, 3 of 3) is **Linux today: runs; Linux C2679 and landed: S; Darwin today: U; C4r: S; landed: SW** (`logs-L/ffi2-*.log`,
   `logs-D/ffi-*.log`). The engineer's report counts the Linux build as refusing *"2 of the 12 G programs"* (`g_3_0`, `g3`); **`sched` is a third and it is the one a real program looks like**, refused on every build that has the new decode on both platforms. `watchdog_b` (a `swapcontext` away, never back): Darwin today runs, C4r S, landed SW;
   **Linux runs on all three** (the engineer's *"the clause waits for 2702"*, measured on the real-library program as well).
3. **Plain `siglongjmp` onto a stack the program made, and `setcontext` into one, depend on the platform and the address order**: `cs2`: Linux aborts when the coroutine's stack lies BELOW the alternate stack, for both `HOW`
   (the engineer's § 10, reproduced: today, C2679 and landed alike), **Darwin runs `HOW=jmp` in both relations** and answers `HOW=ctx` with K (today), S (C4r) or SW (landed) in both; `cs_1` (`setcontext` into a coroutine whose
   stack is `malloc`'d) is Darwin K / S / SW and Linux (Debian) runs.
4. **The words of one program differ by platform**: `as_1` is **K on all three Linux builds and U on Darwin C4r and landed** (K on Darwin today): § 4.

### 2.3 What did not run on the other platform, and why (each a command's answer, not a guess)

- **Darwin, the engineer's `g_1_0 g_3_0 g_3_1`**: `BUILD-FAILED`, `compiling the generated C failed: <inline asm>:4:1: error: unknown directive .type xu_switch, %function`: her `xn.h` switch is **ELF assembly** (`.type`, `.size`).
  The ffi-pragmatist's `r2new.h` has the Mach-O twin (`au_switch`), and `g1_0 g1_1 g2 g3 g2_1 nn` are her/critic's forms of the same shapes and ran on Darwin.
- **Darwin, `x_heap x_tls x_dlopen x_partial x_unseen0`**: `BUILD-FAILED`, `error[ffi_header_refused]: extra.h ... does not compile: clang refuses <ucontext.h> ... The deprecated ucontext routines require _XOPEN_SOURCE to be defined`
  (`extra.h` opens with `_GNU_SOURCE`). **I did not patch it**; the ffi-pragmatist's `x_heap x_tls x_dlopen jo_*` are the same C classes and ran on Darwin (her table); the cross-run of the *engineer's own* five is therefore not measured.
- **Darwin, `p1f`**: `error[ffi_missing_link]: ... its body calls __longjmp_chk`: glibc's fortified `longjmp`, not on Darwin. **Darwin, `acc`**: the accessor `hero_signal_jump_decoding` exists only in panel 255's route files; on the landed runtime its
  successor is the golden `fixedbugs-2679-the-runtime-says-which-route-decides-a-jump`, which printed **`route 3`** on Darwin C4r and landed and on Linux landed (3 of 3 each, `logs-*.log`); `acc` printed `decoding: 3` on Linux C2679.
- **Linux, the ffi-pragmatist's `bctx_plain bctx_signal bmon`** (Boost: `libs.sh` skips it, `/opt/homebrew/opt/boost` is Darwin's), **`pngdemo pngcb pngread`** (the image has no libpng: the shared brief's own `pkg-config` answer), **`uvsig sqlint ossl rl gsrun asyncjob`**
  (libuv, SQLite, OpenSSL, readline, Ghostscript: not looked for in the image, so *unrun*, not *absent*), `cxxguard` (built, not run: her finding is that on Darwin no correct program exists, and the engineer's `c5` is the Linux form).
  Ran on Linux: `sched watchdog_a watchdog_b watchdog_c probe lazy watchdog_h1 watchdog_h2 abandon` and the eleven `dsweep` forms.
- **Not run on either**: Linux x86-64, Windows, `macos-26`, the CI's glibc 2.43 (the engineer's: `ubuntu:26.04` exists as a local image; I did not repeat her Ubuntu rows).

### 2.4 What reproduces, and what does not read as a seat wrote it

- **Reproduced (all `3 of 3`)**: the engineer's `p4s` runs / `p4d` runs to 4096 and aborts from 6144 / `p4x` runs / `b1` runs, `b2` S, `b3` T / `cs2` ORDER=1 S / `g_3_0` runs today and S on C2679 and landed; the ffi-pragmatist's `p7`, `g1_0`, `g1_1`, `g2`, `g3`, `nn`, `watchdog-b`,
  `sched`, `fp3`, `as_0` at every depth she swept (my four depths on Darwin: `logs-D/ffi-*.log`), `sc1`: Darwin today U, C4r S, landed SW, 4 depths, 3 of 3.
- **`p4d`'s abort starts between 4096 and 6144 on Linux**, where the engineer reports 4608 / 5120: the same boundary (my PADs are the brief's: 0, 1024, 4096, 6144, 16384); I did not repeat `p4e`.
- **`fp3` and `as_0` on Darwin, four cells each** (`logs-D/fp3.log`, `scripts` `dr.sh` with `LEVELS="-O0 -O2" MODES="plain sanitize"`, 3 of 3, **C4r and landed alike**): `fp3` **S** at `-O0` plain and `-O0 --sanitize`, **runs** at `-O2` plain and `-O2 --sanitize`; `as_0` **S** in all four cells.
- **The engineer's `K` for `as_1`**: reproduced, Linux today, C2679 and landed: K (`logs-L/*.log`); **Darwin: K today, U on C4r and landed** (the ffi-pragmatist's *"C4r U"*): the same C, different words (§ 4).
- **`abandon` and `watchdog_h2`** (a `longjmp` that abandons the frames of a Heroes callback): `exit=134`, *`panic: N heap blocks still live at exit (a missing decref) - this is a compiler bug`*, on **Linux today, C2679 and landed and on Darwin today, C4r and landed** (3 of 3 each:
  `logs-L/ffi2-*.log`, `logs-D/ffi-*.log`, `logs-L/ffi-*.log`; the callbacks ran and printed first). **The ffi-pragmatist's finding 3e/3h holds on both platforms and on all builds: it is not a sentence of this sitting and it is a correct C program told as a compiler bug.**

## 3. My own shapes (CL-061), six, each beside a clause of x1 or x2 (`programs/critic/mine/n*.h` and `.hero`, built by the engineer's portable runner from `s-mine`)

`-O0` and `-O2`, plain, 3 of 3 runs a cell, on **all six builds** (Darwin today / C4r / landed; Linux today / C2679 / landed), unless a row says otherwise. *Defined* means the premise of p1 to p10 (the program raises the signal itself); no seat
read the standard (the ffi-pragmatist's § 4 says her C11 and POSIX clauses are *recalled*), so I do not claim more.

| shape | what it does | defined in C? | the clause it was written against | Darwin (3 builds) | Linux (3 builds) |
|---|---|---|---|---|---|
| **n1**, **n1s** | a **nested handler**: `h1` saves a point inside itself and raises SIGUSR2; `h2` `siglongjmp`s to that point (out of `h2`, **into `h1`**); `h1` returns; then the callback from 64 KiB deeper (n1) or from `run_case`'s own frame (n1s) | yes (the target frame is live) | *"returns"*, *"siglongjmp ... onto the thread's own stack"*, *"a jump that lands within it"* | **runs**, every cell: 4 programs (n1 n1s n2 n4) x 2 levels x 3 builds = 24 cells, 3 of 3 each (`logs-D/n-*.log`: `grep -c exit=0` printed 10 per log, n3 included) | **runs**, the same 24 cells (`logs-L/n-*.log`, 10 per log) |
| **n2** | `setcontext` out of a handler to a context `getcontext` took in a **different function's live frame** (`inner`, the signal raised from a function below it) | yes | *"a `setcontext` to a context `getcontext` took"* | **runs** | **runs** (also on the forced route A: § 6) |
| **n3** | the handler `siglongjmp`s to a buffer saved in a frame that **has since been popped and reused** | **no: undefined** (C11 7.13.2.1, from memory) | none (a probe) | every build: exit 0 and `result 1` (the program **ran past the stale frame and returned a garbage value**; no refusal, no abort) | the same |
| **n4** | the handler `swapcontext`s **away** to a coroutine on its own stack, the coroutine `swapcontext`s **back**, the handler **returns**; the callback is called only **after** (never while switched away): the design of `sched`, with no callback during the switch | yes | *"returns"* **and** *"a `swapcontext` away from it"* (both apply) | **runs** (landed, C4r, today) | **runs** |
| **n5** (Linux only) | the stack has **grown** to `DEPTH_MB` (4, 24, 40) mebibytes by `setrlimit(RLIMIT_STACK)` and recursion; a `siglongjmp` out of a handler onto that stack; the callback | yes | *"onto the thread's own stack"* (grown past the size it had at start-up) | not run: the shape needs the main thread's stack to grow past its start-up size, which I did not try on Darwin (*unrun*, not *refused*) | **runs** at 4, 24 and 40 MiB, today and landed (`logs-L/n5-{today,landed}.log`); I looked for an own-stack bound the runtime keeps from start-up and found none violated |
| **n6** | **the mundane `fp3`**: a helper runs a `makecontext` task on a **local array** of its own frame, the task returns through `uc_link`, the helper returns; later, in frames that occupy that memory, the same thread saves a point with `sigsetjmp` (10 KiB below `run_case`), raises SIGUSR1 **`DOWN` frames of 2 KiB below the save point**, the handler `siglongjmp`s to it, the callback runs | yes | *"`siglongjmp` ... onto the thread's own stack ... and a callback then runs"* (x1, x2, w26, w27) | **today runs** (all `DOWN`); **C4r and landed: `DOWN` 0, 20, 60 (signal 0, 40, 120 KiB below the save point) run, `DOWN` 130 and 150 (260 and 300 KiB below) abort S, at `-O0` and at `-O2`** (`logs-D/n6b-*.log`: C4r runs=6 aborts=4, landed runs=6 aborts=4, today runs=10) | **runs at every `DOWN`, on all three builds** (`logs-L/n6b-*.log`: runs=10 aborts=0 each) |

**What the six say.** (a) **I could not falsify the positive clause with a nested handler (n1), with a `setcontext` into another function's frame (n2), with a swap-away-and-back (n4), or with a stack that grew (n5)**: they are the shapes the
brief suggested, and they run on every build of both platforms. (b) **n3 is not a correct program and nothing refuses it**: *undefined* is not *refused*; no sentence owes it, and the runtime's silence is the runtime's. (c) **n6 is a falsifier
of the positive clause on Darwin that is neither `-O`-dependent nor the ffi-pragmatist's one byte of frame layout**: `fp3`'s refusal at `-O0` and run at `-O2` read, in her § 7, as *"one byte of frame layout"*; my `n6`
refuses at `-O0` **and** `-O2` as soon as the handler runs more than 256 KiB below a save point whose frame lies in a dead `makecontext` array, and runs below that: the trigger is *where the signal was raised relative to
the dead range*, which a program's author cannot see in the program's text. (d) **The brief's own example** (*a handler on an alternate stack that leaves to the thread's own stack on a thread the program started*) is the engineer's `tal`: I re-ran it on Darwin,
**runs on all three builds**, but Darwin printed *"the thread's stack is ABOVE the alternate stack"* for **both** `ORDER=0` and `ORDER=1` (`logs-D/*.log`, `# ORDER=` blocks), so on Darwin **one address relation was exercised and the other was not**; Linux's two relations are the engineer's.

## 4. The words (item 3): what each build prints, and whether the candidates' sentences about it are true of it

**Source** (read in `critic-landed/runtime/parts/sigguard.c`, lines 405 to 500, the landed file): `hero_signal_refuse` joins four-and-a-half texts, each ending with `HERO_SIGNAL_REMEDY`: **D** *"... ran as a signal handler: the system called it inside a signal ..."*;
**S** (`HERO_SIGNAL_STANDING`) *"... was called while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that handler, or C left the handler in a way this runtime does not follow
(an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library). A handler not seen to end is held to be running ..."*; **SW** (`HERO_SIGNAL_SWITCHED`, `sigguard.c:440`, **Darwin only**: `grep -n HERO_SIG_SWITCHED`'s only setter is the engineer's `rebind_entries.c:297`, which
my Linux cells agree with: no Linux program of mine printed SW) *"... had switched to another context (`swapcontext`, or `setcontext` into a context `makecontext` made) and had not been seen to switch back or to end: either C called it on that other context, or C abandoned the handler by a switch this runtime cannot
tell from one that comes back. A handler not seen to end is held to be running ..."*; **U** *"... was called after C, inside a signal handler, jumped to a buffer this runtime holds no record of as it now stands (copied with `memcpy`, refilled where the runtime does not look, or saved before more save points than it keeps), so whether the jump left
the handler is not known: a handler not seen to end is held to be running ..."*; **K** (`HERO_SIGNAL_CALLED`) *"... was called from inside a signal handler: C called it while handling a signal, ..."*. **Every refusal's tail is `HERO_SIGNAL_REMEDY`, which has no mailbox clause**
(`grep -c mailbox runtime/parts/sigguard.c` in the landed copy: **0**; `sed -n 428p` of `rebel-round`'s spec `| grep -c mailbox`: **1**): the printed tail and the spec's differ at the landed tree too (the red compiler test, defect 2533, is the sitting's known cost; I did not run it).

**What the builds printed** (`scripts/kinds.py`, my main streams, `-O0` plain; the first line of stderr of every refusal is in `logs-D/*.log`, 600 characters, and in `logs-L/*.log`, **150 characters** because the engineer's runner cuts there):

```
Darwin today: U: g3 gcopy grefill p10 p10a p10c p10d p10h p3; S: b2 p4 p4a p4b p4c1 p4c2 p4d p4s p4sd p4x p6 p8; T: b3; K: as_1 cs2 cs_1; other: mmap
Darwin c4r: S: as_0 b2 cs2 cs_1 fp3 g1_0 g1_1 g2 g2_1 g3 grefill nn p4 p4a p4b p4c1 p4c2 p4d p4s p4sd p4x p6 p7 p8; T: b3; U: as_1
Darwin landed: S: as_0 b2 fp3 grefill p4 p4a p4b p4c1 p4c2 p4d p4s p4sd p4x p6 p8; SW: cs2 cs_1 g1_0 g1_1 g2 g2_1 g3 nn p7; T: b3; U: as_1
Linux today: U: gcopy grefill p10 p10c p10d p10h p3 p9 x_partial; S: b2 cs2 p4 p4b p4c1 p4c2 p4d p6 p8; T: b3; K: as_1; other: mmap
Linux c2679: S: b2 cs2 g3 g_3_0 grefill p4 p4b p4c1 p4c2 p4d p6 p8 x_partial x_unseen0; T: b3; K: as_1
Linux landed: S: b2 cs2 g3 g_3_0 grefill p4 p4b p4c1 p4c2 p4d p6 p8 x_partial x_unseen0; T: b3; K: as_1
```

**Is the first line true of the program that provoked it, and does the candidates' wording fit it?** Counts over the Darwin lines I kept whole (600 characters; today, C4r, landed; `grep -c 'seen to end'` and `grep -c 'from inside'` over the lines of each kind): **S 65 lines, all 65 contain both
*seen to end* and *from inside*; SW 11 lines, all contain *seen to end* (in the tail *A handler not seen to end is held to be running*; the head says *seen to switch back or to end*); U 11 lines, all contain *seen to end* (in the tail); K 4 lines, none *seen to end*, all *from inside*; T 3 lines, neither** (the thread rule, defect 2492).
- **x1** (*"saying the handler was not seen to end, or that C called it from inside one"*): literally true of **91 of 91** S, SW, U and K lines on Darwin, and of the Linux S and K lines (cut at 150 characters, both phrases inside). It does **not** claim T or D, and neither arises after a handler exit.
- **x2** (*"saying the handler was not seen to end"*): true of **87 of 91** Darwin lines; **the four K lines** (`cs2` `HOW=ctx` today, twice, `as_1` today, `cs_1` today) do not say it; and **Linux prints K for `as_1` on all three builds** (K appears on Linux in no other program of mine). Whether `as_1` is
  in x2's class is arguable (a handler left by `siglongjmp` and *re-entered* by `setcontext`, the callback called inside the re-entered handler), which is itself the point: **K is the words of a refusal whose cause the reader cannot tell from the class's**. x2 has no K alternative.
- **The same C, different words by platform**: `as_1` is K on Linux (all builds), K on Darwin today, **U on Darwin C4r and landed** (*"jumped to a buffer this runtime holds no record of ... (copied with `memcpy`, refilled ..., or saved before more save points than it keeps)"*: **none of which `as_1` did**: it is a resumption of a handler left by a `siglongjmp`, the ffi-pragmatist's § 6). The words a reader meets for one program depend on the platform.
- **Does the engineer's *"K appears (as_1)"* stand? Yes, reproduced** (Linux today, C2679, landed: `K`). Her Ubuntu 2.43 row (*S in plain, K under `--sanitize`*) I did not repeat.

**The lane's note (that it did not argue S's words true of Darwin `fp3` and `as_0`): is a reader of those words misled, and does the sentence need to say so? Yes to both, by a program that is not the ffi-pragmatist's.** `fp3` and `as_0` (landed, Darwin, S, 3 of 3) print *"either C called it from inside that handler, or C left the handler in a way this runtime does not follow (an inline
`__builtin_longjmp`, a C++ exception, a jump inside a system library)"*. **`n6` (my shape) prints exactly those words for a plain `siglongjmp` out of a handler onto the thread's own stack**, a program with no inline jump, no C++ and no library jump in it, **while x1 says in the same document that such a jump *carries the thread out of
the handler ... and a callback then runs***. The reader holds two statements that cannot both be about their program and the message points at three causes the program does not have. **A sentence that promises *runs* for a `siglongjmp` and does not name the memory-once-given-to-`makecontext` class leaves that reader with no sentence to find**; the ffi-pragmatist's repair
(*"unless the jump lands on another stack that was, now or once, given to `makecontext`"*) names it, and `n6` shows the class is not the stack the program *sees* (the target is an ordinary frame of `deep()`; the memory is a dead array of a helper that returned): a clause in terms of *"a stack given to `makecontext`"* is a clause a reader cannot apply to `n6` either.
The same two sentences also say nothing about `sched` on **Linux**, whose S words (*C called it from inside that handler, or left it in a way ... inline `__builtin_longjmp`, a C++ exception, a jump inside a system library*) name none of a green-thread scheduler's acts (SW names `swapcontext` but only on Darwin).
**The S words' own example does not compile on arm64**: reproduced by the seats on both platforms (the engineer, `clang -c`: *not supported for the current target*; the ffi-pragmatist, Apple clang 21.0.0); x1 and x2 do not repeat it (*"written in assembly, say"*), which is what keeps them true.

## 5. Clause by clause, on both platforms and both builds (C10 for `x1`, `x2`, `w26`, `w27`)

Marks: **T** true of every cell I ran for it; **F** a program falsifies it (named, with the cells); **by *may*** the clause says *may*, so no abort falsifies it, only a run would (and a run is not the clause's claim). "Both builds" = the panel-255 route and the landed runtime
(Darwin C4r and landed; Linux C2679 and landed); today's runtime is shown where it differs, since the sentence will be read against the landed one.

### 5.1 `x1`

| clause of x1 | Darwin | Linux | the program(s) |
|---|---|---|---|
| *A handler of C's is over where it returns, ... and a callback then runs* | **T** | **T** | `p5` run on all six builds; `n4` (swap away, back, return): run |
| *or where `longjmp`, `siglongjmp` or `_longjmp` ... carries the thread out of it onto the thread's own stack, and a callback then runs* | **F** (C4r, landed) | **T** | **Darwin: `fp3` aborts S at `-O0` and `--sanitize` (landed, 3 of 3; `-O2` runs), `n6` aborts S at `-O0` and `-O2` with the signal 260 and 300 KiB below the save point (C4r and landed, 3 of 3 each); Linux: `p1 p1l p1u p1f p2 p3 p9 p10 p10a p10c p10d p10h n1 p1n` all run on C2679 and landed, `n5` (a grown stack) on today and landed.** Today's runtime also refuses `p3 p9 p10 p10c p10d p10h gcopy` (U) on Linux and `p3 p10 p10a p10c p10d p10h gcopy` on Darwin, which the landing moves |
| *... or a `setcontext` to a context `getcontext` took ...* | **T** | **T** | `p7s b_0 b_1 n2 p10c` run on every build (`p10c` today: U) |
| *left any other way (a jump no function of the C library makes, written in assembly say) ... a callback called later on its thread may abort* | **T by *may*** | **T by *may*** | `p4`: S on all six builds; `p4s p4sd p4x p4a`: S on Darwin, **runs on Linux** |
| *... by a `swapcontext` away from it ... may abort* | **T** | **T by *may*** | `p7`, `g1_0`, `g1_1`, `g2`, `g2_1`, `nn`, `watchdog_b`: Darwin C4r S, landed SW; **Linux run** (3 builds); `sched`: **S** on Linux C2679 and landed, SW Darwin landed |
| *... by a jump onto a stack `makecontext` made ... may abort* | **T** | **T by *may*** | `cs2` `HOW=jmp`: Darwin runs both relations, Linux S (coroutine below); `HOW=ctx`: Darwin K / S / SW, Linux runs (above), S (below); `sc1`, `as_0`, `g3`, `cs_1`; **`fp3` and `n6` are not *a stack makecontext made* in any sense of the words**: their target is an ordinary frame (§ 3, § 4), so for them the first positive clause and this one **do not decide which applies** |
| *... or by a jump that lands within it ... may abort* | **T** | **T** | `p8`: S, six builds. **A jump that lands within a handler does not *leave* it**; *"left ... by a jump that lands within it"* is a wording that says the handler was left. The callback of `p8` is called **by the handler** (line 427's rule) |
| *saying the handler was not seen to end, or that C called it from inside one* | **T** (91 of 91) | **T** (S and K) | § 4 |

### 5.2 `x2`

x2 has the same atoms in the other order, and differs in four places: **(i)** *"a handler that returns, or that C leaves by `longjmp`, `siglongjmp`, `_longjmp` or `setcontext` onto the thread's own stack, is over, and its callbacks run"* is the same positive clause, **false of the same programs** (`fp3`, `n6`, Darwin). **(ii)** its words clause is
*"saying the handler was not seen to end"* only: **false of the K lines** (§ 4: Darwin today's `cs2 ctx`, `as_1`, `cs_1`; Linux's `as_1` on every build). **(iii)** *"its callbacks run"*: *its* has no antecedent that is a callback's owner (the handler's callbacks? the callbacks called after it?), and `p8`'s callback, called by a handler
that jumped within itself, is a *callback of a handler* that does not run. **(iv)** *"left by ... a jump that lands within it"* is the same wording as x1's.

### 5.3 `w26` and `w27` (the warden's), for the record the brief asks

| clause | falsified by | cells |
|---|---|---|
| *ends by ... `longjmp`, `siglongjmp` or `_longjmp` out of it* (no stack scoping) | **`cs2` with the coroutine's stack below the alternate stack** (Linux today, C2679, landed, both `HOW`: S, the engineer's § 10, reproduced); **`fp3`, `n6`** (Darwin) | Linux `logs-L/{today,c2679,landed}.log`, `# ORDER=1`; Darwin `logs-D/{c4r,landed}.log`, `logs-D/n6b-*.log` |
| *... or `setcontext` out of it* (no scoping) | **`sc1`** (a one-way `setcontext` onto a `makecontext` stack: Darwin today U, C4r S, landed SW at 4 depths, 3 of 3, my `ffi-dar` sweep; Linux runs), **`cs2 HOW=ctx`** (Darwin K / S / SW, both relations), `cs_1` | `logs-D/ffi-*.log`, `logs-D/*.log` |
| *a callback ... from deeper in the stack than the handler was may abort* | not falsified (*may*), but **it is Linux's rule and misleads on Darwin**: `p4s p4a p4x p4d(PAD 0..4096)` abort on Darwin from the raise site's own level (`p4` at `D=-1024`, `measured=-960`, S); **and it will mislead on Linux the day 2702 lands** (R3: *"no prune by a trampoline's, wrapper's or callback's own frame"*) | § 2.2 |
| *left any other way ... it is still taken to be running* (w26: `swapcontext`, `makecontext`) | **`p7`, `watchdog_b`, `g1_*`, `g2*` on Linux** run: a handler that is *taken to be running* is not one whose callback runs (the clause says *may abort* after it, so it holds by *may*; the first half, a **state** claim, does not) | Linux three builds |
| w27 | the same positive clauses (`cs2` below, `fp3`, `n6`, `sc1`); silent on contexts, so no clause of it is false of `p7` | |

## 6. Scope (item 4): Windows, route A, x86-64: which clause is false there, and does the candidate say so

**Route A, forced** (`HERO_JMP_FORCE_UNCALIBRATED 3` prepended to the landed `runtime.c`, one line, `diff` against the landed file printing nothing else; the accessor golden read `route 3` on the unforced builds; `scripts/routeA.sh`; **a simulation, not a platform**, the
ffi-pragmatist's and the engineer's own words for it). My cells, `-O0`, 3 of 3 (`logs-D/routeA-mine.log`, `logs-L/routeA-mine.log`):

| program | Darwin landed, forced A | Linux landed, forced A |
|---|---|---|
| `p1` `p5` `p7s` `n1` `n2` `n4` `p1n` | run | run |
| `p2` (heap buffer) | **U** | run |
| `p3` (a copy), `p10` (the handler refills and leaves) | **U** | **U** |
| `p9` (40 live saves) | run | **U** |
| `b_0` `b_1` (`setcontext` to a `getcontext` context) | **U** | **run** |
| `gcopy` | **U** | **U** |
| `fp3` `as_0` | run | run |

**So on a platform that falls to route A, the positive clause common to x1, x2, w26 and w27 is false of `p3` and `p10` on both (U), `p2` and `b_0`/`b_1` on Darwin, `p9` on Linux; neither x1 nor x2 says so**, and neither names the accessor
(`hero_signal_jump_route()`, the landed golden). The **words** printed there are U, *"jumped to a buffer this runtime holds no record of"* (§ 4): true of `p3`, `p10`, `gcopy`, and **false of `b_0` and `p2` by the parenthesis's own list** (*copied with `memcpy`, refilled where the runtime does not look, or saved before more save points than it keeps*: `b_0` is a `setcontext` to a saved context and `p2` a heap buffer saved and jumped to at once, which is the reading the ffi-pragmatist applied to `rr_*` and `as_1`). **Which platform reaches route A is not known**: panel 255's account names *Darwin x86-64, glibc's four other
architectures, a future C library*; **none of them is a CI leg except possibly Linux x86-64**, whose calibration the sitting *carried* (the engineer's § 11) and nobody ran. That one leg is the open question: if `hero_signal_jump_route()` reads below 3 on `ubuntu-26.04`, the positive clause is false on a CI leg.
The row's own split between platforms (`b_0`: Darwin U, Linux run) is the forced decode's, and says the two platforms' route A are not one rule.

**Windows** (unrun; no machine, no probe): the landed `sigguard.c` has `hero_callback_entered` empty in its `_WIN32` branch (read: *"Windows: no probe and no walk (panel 247's R10) ... unrun"*), so by the runtime's own comment **no callback is refused there**. Of x1 and x2 **no clause is false there by that reading**: *may abort* is never falsified by a run, *and a callback then runs* is true when nothing is refused,
and the words clause describes a refusal that never prints. **Neither candidate names a platform and neither needs to for that reason**; line 427's *"nor called by one"* is the sentence that is Windows-false, as the warden said and the coordinator knows. **What nobody asked: whether the sentences' vocabulary exists there**: `siglongjmp`, `_longjmp`,
`getcontext`, `setcontext`, `swapcontext`, `makecontext` are POSIX names (recalled, not run: I know of no UCRT `ucontext.h`); a reader on Windows is told about functions the platform may not have. A question, not a finding.

**Linux x86-64** (unrun here): what rests on it, from the reports and my cells: **(a)** the calibration of the setjmp key and the context fields (above, the open question); **(b)** the inline jump's spelling: *"written in assembly, say"* is true on both architectures, while `__builtin_longjmp` compiles on x86-64 only (the ffi-pragmatist, `-target x86_64-apple-macos
-fsyntax-only` printed nothing; the engineer's A12), so **x1 and x2 are the first candidates that survive it by not naming it**; **(c)** the depth test's threshold (arm64's signal frame) is a Linux number, not in x1 or x2; **(d)** `cs2`'s mapping order is an address comparison with no machine-specific part (the engineer's inference, unrun). **The CI's glibc 2.43**: the engineer's rows, not repeated by me.

## 7. Contradictions between the seats, which are checkable, and what the sitting should have asked and did not (item 5)

**Checked contradictions** (each by my cells, § 2):

1. **Depth: Linux's test, Darwin's none.** Both seats are right on their platform (§ 2.2, item 1). The warden's text drew one rule from the first (`from deeper in the stack than the handler was`) and the warden's own condition (1) said a Darwin sweep could change *may* into *aborts*, and the sweep read *no depth on Darwin*, which makes the clause *wrong as a condition*, not *wrong as a may*. **And 2702 makes it wrong on Linux too** (§ 5.3).
2. **`setcontext`.** The engineer's `b_0`/`b_1` run and the ffi-pragmatist's `sc1` is refused: **both true, and different programs** (a `setcontext` to a `getcontext` context, and a one-way `setcontext` onto a `makecontext` stack); w26/w27's *"`setcontext` out of it"* does not say which, x1/x2's *"to a context `getcontext` took ... onto the thread's own stack"* does. The seats' two tables never met until now; **on Linux `sc1` runs on today, C2679 and landed** (4 depths, `logs-L/ffi-*.log`), so *Linux does not refuse the `sc1` class at all and Darwin always does*.
3. **The words.** The engineer found K on Linux for `as_1`; the ffi-pragmatist found U on Darwin for the same program: **one C program, two platforms, two kinds of words** (§ 4); neither seat could see it alone.
4. **The warden's `w27` was called *"true on both builds as they stand"* (*left by an inline jump* alone) and it is not**: its positive clause *"ends by ... `longjmp` ... `setcontext` out of it"* is falsified on Linux by `cs2` below (the engineer's § 10, written in her resumed run, after the warden's report) and on Darwin by `sc1`, `fp3`, `n6`. The warden's condition (2) (*"`b_0`/`b_1` refused on either build in a 257 re-run takes `setcontext` out of the list"*, `spec-warden.md`'s verdict) is **not met on an unforced build** (they run on all six) **and is met on Darwin's forced route A** (`b_0` U).
5. **The engineer's `5b` (*"no correct program that LEFT the handler is refused under the adopted build"*) is one platform's**: on Darwin `fp3` and `n6` are correct programs that leave the handler by a plain `siglongjmp` and are refused on the adopted build and on the landed one.
6. **The engineer's L5 row (*"saying the handler was not seen to end": true of S, false of K*, `as_1`) and the ffi-pragmatist's (*"U still prints on C4r and landed for `rr_0`, `rr_1`, `as_1`: a finding against R2's promise"*)** are the same finding from two platforms; R2's promise (*"the true 'had not been seen to end'"* instead of U) holds on Linux with the decode verified and **does not hold on Darwin for `as_1`**.

**What the sitting should have asked and did not** (each a question; none is a verdict):

- **Which of the two builds' refusals will still stand when 2702 lands?** The sitting was told to write *"where the landed runtime refuses it, until 2702 says otherwise"* and its seats did, but **nobody asked 2702's lane for the words and the depth behaviour its Linux build will have** (R3 carries Darwin's rules to Linux, which includes *no prune by frame*): the one clause of the arms that rests on Linux's current mechanism, the depth, is the one 2702 removes. The question that would have found it: *"what does a callback from the raise site do on each platform, and which of those cells does an open defect change?"*
- **Does the sentence promise a *run*, or only describe a *refusal*?** Every candidate states what *runs* as well as what *may abort*. A promise of *runs* is falsified by one program (`n6`, `fp3`) where a description of *may abort* is falsified by none. The brief's own wording (*"tells a reader of the spec which correct C programs a Heroes program's jump out of a handler refuses, and what the program is told"*) asks only for the refusals. **The positive clause is the one the sitting added and the one the runs cut.**
- **What does a refused reader do instead?** `HERO_SIGNAL_REMEDY` tells a Heroes author how to stop on a signal, which is the *remedy for a handler that calls Heroes*; for a refused `sched` or `fp3` the reader gets a rule about what C may do and no way out. The author's 1a/2a (as the recording session glossed them) ask *what the program is told*, not what it should do; but `spec-shape.md`'s own test is the reader's rewrite, and none of the sentences gives one.
- **Which words will exist when the sentence lands?** The lane has SW on Darwin and not Linux; the spec sentence is written for both. x1's alternatives (*not seen to end*, *from inside*) fit every kind that exists today (§ 4); **a sixth kind, a rewording of U (the lane is "re-wording the very words", the critic's first reading), or SW on Linux at 2702, is not held by anything**: the warden's own *"no instrument holds the sentence's nouns"* (his § 8) stands, and the candidates add none.
- **Does the landed tree build where the sitting says it lands?** (§ 1): the engineer's condition (e) is not met by any image here; the sitting's first act on reading L1 should have been to ask the coordinator to say so to the lane that owns the UI file.
- **The Windows probe**: the warden's condition *"file it"* (a Windows runtime probe) is still a search-only claim (`gh issue list --search` printed four closed issues, none about a probe): the sitting did not ask whether the sentence's vocabulary even exists there (§ 6).
- **The `abandon` finding** (a `longjmp` that abandons the frames of a Heroes callback ends in *"a missing decref - this is a compiler bug"*, reproduced on six builds, both platforms: § 2.4) **bears on the sentence's reader**: *a callback then runs* is true and the program then aborts at exit blaming the compiler. `gh issue list --search 'still live at exit missing decref in:title,body'` (read only) printed defect 2552 (*"a thread still running when main returns is told as a compiler bug"*, a different cause) and nothing for a jump over a Heroes frame. **Nobody asked whether it is filed.**

## 8. The candidates as texts (mechanics, by command)

- **They apply**: `patch --dry-run -p1 < spec-x1.diff` and `spec-x2.diff` in my copy printed `patching file 'spec/heroes-spec.md'`; applied, each leaves **516 lines** (`wc -l`) and **the tail from `No function of the program is a signal handler` to the end of line 428 byte-identical to the frozen file's** (`sed -n 428p | sed 's/.*\(No function ...\)/\1/' | shasum -a 256`: `792d04dbbfea431a` in all three), so the compiler's test of defect 2533 sees the same tail as before (it is red at the frozen tree for the mailbox clause; I did not run it).
- **Size**: `wc -w` of the texts: x1 **106 words** (564 bytes), x2 **80** (436). `./heroes-next measure spec/heroes-spec.md` on each applied copy (the spec restored by `cp` from the saved original, `shasum -a 256` `15804f3fb52a62c208bc59ef` before and after): x1 **claude-legacy 9,727, cl100k_base 9,903**, x2 **9,697 / 9,873**, against 9,585 / 9,761:
  **+142 and +112 vendored**; the warden's `w26` is +101 vendored and **+138 real, measured** (`spec-w-evidence.txt`). **Real counts for x1 and x2 are unpriced** (no `--refresh` is mine to spend): at the warden's own ratios (1.31 to 1.40 real per vendored, four measures) they would read about **+186 to +199** and **+147 to +157**, an inference.
- **The `spec` suite** (`scripts/specsuite.sh`, `./heroes-next run tests/harness/main.hero -- ./heroes-next spec`, the spec swapped in my copy and restored): **x1: `harness: 31 passed, 4 failed`; x2: `harness: 31 passed, 4 failed`**, the four being `spec/budget`, `spec/spendable`, `spec/real`, `spec/ledger` for both (`cw/spec-suite-x1.log`, `-x2.log`): the pins the landing moves, as for every other arm. **`getcontext` is a code span neither `w26` nor the arms had; the suite admits it** (`named`, `rejected`, `inventory`, `anchors`, `offered`, `shape` did not fail).
- **Wording points a reader meets** (not measured, each checkable by reading): *"a jump that lands within it"* listed among the ways a handler is **left** (x1 and x2): a jump within a handler does not leave it; *"a stack `makecontext` made"* (`makecontext` is *given* a stack: the ffi-pragmatist's *"given to"*, and the S/SW words' *"a context `makecontext` made"* are three different phrasings of one noun);
  *"written in assembly say"* (x1) is spoken English in a specification (`spec-shape.md`'s register is not mine to judge); x2's *"its callbacks run"* (above); x1's two long clauses joined by a semicolon carry nine ways in 106 words; **neither** says **`__longjmp_chk`** (glibc's fortified `longjmp`, which libpng imports and the engineer's `p1f` runs on Linux): *"in whichever spelling the library gives them"* was her R-7; the Darwin `p1f` did not build (no such symbol), so the point is Linux's.

## 9. Findings, numbered (CL-061 shapes beside the ones that provoked them; each cites the section that holds its command)

1. **The landed tree, and `rebel-round`'s head `cab9f3cf4`, build no Heroes program in `heroes-linux-arm64:latest`**: `hero_ui_print_gtk.c:609`, `-Werror=conditional-uninitialized` (§ 1). The coordinator's to report; unrun on the CI's clang.
2. **A positive clause is falsified on Darwin by plain C**: `fp3` (landed and C4r: S at `-O0` and `--sanitize`, runs at `-O2`) **and my `n6`** (S at `-O0` **and** `-O2`, signal ≥ 260 KiB below the save point; runs at ≤ 120 KiB; Linux runs at every depth, three builds). Every candidate says *a callback then runs / its callbacks run* after a `siglongjmp` onto the thread's own stack (§ 3, § 5).
3. **Depth is each platform's own, and the Linux half is the part 2702 removes**: `p4s p4sd p4x p4a p4d(PAD <= 4096)` run on Linux and abort on Darwin, every build (§ 2.2); w26/w27's *"from deeper in the stack than the handler was"* is true of neither platform's future. The warden's registered prediction *"goldens ... that show `p4s` running, `p4d` at 5120 aborting"* **cannot be one platform-neutral `run` golden**: `p4s` aborts on Darwin on all three builds.
4. **`cs2` (a handler on an alternate stack leaving for a coroutine's stack) aborts on Linux when the coroutine's stack lies below, for `siglongjmp` and `setcontext`, today, C2679 and landed, and runs on Darwin for `siglongjmp` in both relations** (§ 2.2 item 3); **`sc1` is refused on Darwin on all three builds and runs on Linux** (§ 5.3). w26/w27 say `longjmp`/`siglongjmp`/`setcontext` *out of it* end a handler with no scoping; x1/x2's *onto the thread's own stack* is what survives both.
5. **x2's words clause is false of K**: four Darwin lines (today's `cs2 ctx` twice, `as_1`, `cs_1`) and Linux's `as_1` on all three builds (§ 4). x1's two alternatives are true of 91 of 91 Darwin lines and of every Linux S and K line.
6. **Printed words differ by platform for one program** (`as_1`: K on Linux, U on Darwin C4r and landed), **the S words' three causes are false of `fp3`, `as_0`, my `n6` and `sched`**, the answer to the lane's unargued note is *yes, a reader is misled* (§ 4), and **a sentence cannot repair printed words**: it can only avoid promising what the words contradict.
7. **Route A, forced, falsifies the positive clause on both platforms** (`p3 p10 gcopy` both; `p2 b_0 b_1` Darwin; `p9` Linux), and **the only CI leg whose calibration could read below 3, Linux x86-64, has never been run** (§ 6). No candidate scopes it or names `hero_signal_jump_route()`.
8. **`sched` (a preemptive green-thread scheduler on `swapcontext`) is the third G program the landed Linux build refuses** (S: Linux C2679 and landed; runs today), beside the engineer's `g_3_0` and `g3`: *"2 of the 12 G programs"* is the engineer's count of her programs, not of the shapes a real program has; `watchdog_b` runs on Linux and is refused on Darwin (§ 2.2 item 2).
9. **`abandon` and `watchdog_h2` end in *"N heap blocks still live at exit (a missing decref) - this is a compiler bug"* on all six builds, both platforms**; the callbacks ran and the jump did what C says. No issue found by a title-and-body search for it (§ 2.4, § 7). A correct C program told as a compiler bug.
10. **Shapes searched and not falsifying** (negative results with their searches, § 3): `n1`/`n1s` (a nested handler left into the outer one), `n2` (`setcontext` into another function's live frame), `n4` (swap away, back, return, then call), `n5` (a stack grown to 40 MiB, Linux): every cell runs (`n1 n1s n2 n4`: 24 cells on Darwin and 24 on Linux, 4 programs x 2 levels x 3 builds; `n5`: 6 cells on Linux, 3 depths x 2 builds); `tal` on Darwin exercises **one** address relation only (ORDER=1 did not flip it); `n3` (undefined) is not refused by any build: it returns a garbage value.
11. **Programs that did not run on the other platform, and the reason** (§ 2.3): the engineer's `g_*` (ELF `.type` assembly), `x_*` (`extra.h` needs `_XOPEN_SOURCE`), `p1f` (`__longjmp_chk`) on Darwin; the ffi-pragmatist's Boost, libpng, libuv, SQLite, OpenSSL, readline, Ghostscript programs on Linux (not in the image, or not looked for). **The engineer's own five `x_*` rows are therefore unmeasured on Darwin.**
12. **The candidates' wording** (§ 8): *left ... by a jump that lands within it*; x2's *its callbacks*; *a stack `makecontext` made*; no `__longjmp_chk`; and **`getcontext` is admitted by the `spec` suite** (both: 31 passed, 4 failed, the pins the landing moves).
13. **My own slip, corrected**: one command of mine (a typo of a redirection in a counting pipeline) created an empty file `/tmp/nothing` outside the repository's root, and the same command removed it (`ls /tmp/nothing` read *No such file or directory* in the next command). Nothing else was written outside the root (my scratch is `p257-scratch/critic/cw/`, every runner's `TMPDIR` set there).

## What the synthesis must settle (most important first)

1. **Does the sentence promise a *run* at all?** `n6` (Darwin C4r and landed, `-O0` and `-O2`) and `fp3` (the same builds, `-O0` and `--sanitize`) falsify *"a callback then runs"* / *"its callbacks run"* in **x1, x2, w26 and w27 alike**, and the forced route A falsifies it on both platforms. The only clause no program of mine or the seats' falsifies is a **`may abort`** one. The coordinator chooses between (a) a sentence that says only what *may abort*, (b) the positive clause carrying its exceptions (*a stack once given to `makecontext`*, which `n6` shows a reader cannot apply, and *where the runtime verified the C library's layout*, which is an accessor), (c) a *may not run* hedge. **This is the decision every other is downstream of.**
2. **The words.** x1's *"saying the handler was not seen to end, or that C called it from inside one"* is true of all 91 Darwin S/SW/U/K lines I kept whole and of Linux's S and K; x2's single alternative is false of K. **The printed S words are false of `fp3`, `as_0`, `n6` and `sched`, and U's of `as_1`, `b_0`, `p2` (forced)**: that is the runtime lane's to reword, the sentence's to avoid contradicting. *Settle whether the sitting files a defect for S and U's parentheses, and whether the sentence names K.*
3. **Depth is out**, on both platforms and for 2702's reason (§ 5.3, § 7): w26/w27's clause must not be printed; **and the owed goldens must be shaped by platform**, since `p4s` runs on Linux and aborts on Darwin on every build.
4. **Where is Linux x86-64's calibration read?** It decides whether the positive clause is false on a CI leg (§ 6). One run of `fixedbugs-2679-the-runtime-says-which-route-decides-a-jump` on `ubuntu-26.04` is the instrument; **settle whether the sentence lands before it has been read**.
5. **The landing prerequisites that are not the sentence's**: `hero_ui_print_gtk.c:609` (§ 1); the compiler's test of defect 2533 (red); a Windows probe or a Windows scope (§ 6: the vocabulary's existence there is a question); a defect for `abandon`'s false *compiler bug* abort (§ 9, 9); the engineer's and the warden's owed goldens (`cs2` below, `p4s`/`p4d`, `b_0`, `sc1`, `n6`) as `run` goldens each with the platform in its name.
6. **Wording** (§ 8): the jump-within clause as a way of *leaving*; *its callbacks*; *made* vs *given to*; whether `__longjmp_chk` is one of the three spellings.
7. **What the cross-run did not reach** (so the sitting says it and does not assume it): Linux x86-64, `macos-26`, Windows, the CI's glibc 2.43 for my cells, the engineer's five `x_*` shapes on Darwin, the ffi-pragmatist's Boost/libpng/libuv programs on Linux, `-O2` and `--sanitize` for most of my streams (`-O0` plain, 3 of 3; `n1`-`n6` ran `-O0` and `-O2`).

**Which of `x1`, `x2`, `w26`, `w27` I would not print in the spec, and the program that stops me** (naming programs, giving no verdict on an arm): **none of the four unchanged, because all four carry the positive clause and `n6` (Darwin landed and C4r, S at `-O0` and `-O2`, 3 of 3) and `fp3` (S at `-O0`) falsify it.** Beyond that one: **`w26` and `w27` are also stopped by `cs2` with the coroutine's stack below (Linux today, C2679, landed: S for `siglongjmp` and `setcontext`) and by `sc1` (Darwin, three builds), and by the depth clause (`p4s`, Darwin, three builds; Linux at 2702)**; **`x2` is also stopped by the K lines** (`cs_1` and `as_1` on Darwin today; `as_1` on every Linux build); **`x1` is stopped by the positive clause alone** (and by the forced route A, a simulation). A sentence that keeps x1's refusal side and says nothing positive has no program of mine against it; that is not a verdict, it is the one clause no run has touched.

## Cleanup and what is where

- Containers: `docker rm -f p257-critic-1` (see the last lines of this file for the command's result). `pgrep -f 'build/'` and `docker ps -a | grep p257` are in the last lines too.
- `programs/critic/`: `mine/` (n1 to n6, `.h` and `.hero`), `scripts/` (every script above; `table.py`, `kinds.py`, `diffcount.py`), `logs/` (text logs of every stream: `logs-D`, `logs-L`, `logs-table.md`, `logs-kinds.txt`, the spec-suite logs). No binary, no dylib.

**Cleanup, run after 16:38 by this Mac's clock** (`date` read 16:38 at the last stream): `docker rm -f p257-critic-1` printed `p257-critic-1`; `docker ps -a | grep p257 | wc -l` printed **0**; `pgrep -fl 'build/'` listed processes of other sessions only (`lane-ui-mac2`'s `zsh` wrappers and a `build/<key>/main ./heroes-next rebel...` of another lane; none under `p257-scratch/critic*`);
`rm -rf critic/build critic-landed/build`; the spec in my copy equals the frozen file (`shasum -a 256 spec/heroes-spec.md` begins `15804f3fb52a62c208bc59ef` after both candidates and both suite runs); `git -C critic status --short` and `critic-landed` print only the untracked `cw/` of mine.
No push, no commit, no `git add`, no GitHub write (`gh issue list` and `gh issue view` read only, twice); no paid run; no credential printed (`grep -rIl 'sk-ant\|ANTHROPIC_API_KEY'` over `programs/critic/` printed nothing).
