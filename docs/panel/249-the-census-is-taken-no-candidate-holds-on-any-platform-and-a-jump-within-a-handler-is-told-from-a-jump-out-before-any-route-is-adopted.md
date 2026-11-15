# 249: the census is taken, no candidate holds on any platform, and a jump within a handler is told from a jump out before any route is adopted

**Status** the soundness lane and the historian, convened 2026-11-14 by the coordinator of M-core-packages
as **the follow-up sitting panel 248's R1 owes on defect 2522**, once 248's ratification was read
(`249-briefs/00-shared.md:3-12`; decision 2527 `CLOSED 2026-11-14T20:56:59Z` by `gh issue view 2527`), on
three questions: the census (what a route may change in what a program's own C observes, and which
libraries a Heroes program plausibly binds depend on it), the candidates run against the census's first
entries, and what closes 2522 if no candidate is sound everywhere (`00-shared.md:33-39`). **Split by the
author's answer of 2026-11-15 at about 02:03** (§ The author's answer that frames it). **No paid run** was
named or made (`00-shared.md:11-12`, `:166-168`). The resolution is
`provisional — author ratification pending` (§ Author's verdict). Panel 247's R3 is written *247's R3* throughout; R1 to R4 are this sitting's.
**Seats** compiler-engineer, ffi-pragmatist, historian (advisory). **What the lane gave up**: the
spec-warden's real token counts and the blind seat's reading (`00-shared.md:47-49`), with an escape that
sends a route needing a spec sentence, a new or widened diagnostic, new refusal words or a change a
binding's author must learn to the full panel before it is adopted (`:51-61`). The completeness critic read
the briefs before any seat (25 items, `249-reports/critic-briefs.md:396-534`; the briefs repaired on them,
the first versions kept as `249-briefs/*.before-critic.md`, `00-shared.md:18-20`) and the reports after (17
items, `249-reports/critic-reports.md:385-437`), the second pass from a brief written after the seats had
finished (`249-briefs/critic-reports.md:3-4`).
**Base** `722ae76bf693bebdb7a5cca37052ed92646753c0` (tree `ce81c0b18b1f...`), *"Panel 248 ratified, R1 to
R6, and the two choices of R5's landing kept"*, holding batch 36's landing of 248's R2 to R5; the signal
guard's nine paths unchanged since 248's base (`00-shared.md:63-78`). Stage 2 prints `sources
2ab7ca0a644692a1` on this Mac, in Linux arm64 and in the emulated x86-64 image (`:92-97`), and in each
building seat's copy (`compiler-engineer.md:24-26`, `ffi-pragmatist.md:9-13`) and the critic's
(`critic-reports.md:51-54`).
**Platforms** Darwin arm64 (macOS 27.2, Apple clang 21.0.0), Linux arm64 (`heroes-linux-arm64:latest`,
Debian glibc 2.41, Debian clang 22.1.8), and Linux x86-64 **emulated** (`heroes-linux:latest`, Debian, not
the CI's Ubuntu leg) (`00-shared.md:230-276`). **The Windows box was off** when the briefs were written
(`ssh ... win` *"Operation timed out"* at 22:11, `:277-286`; again at 23:07, `critic-briefs.md:329-331`);
nothing of this sitting ran there.
**Briefs** `docs/panel/249-briefs/`, **reports** `docs/panel/249-reports/`.

## The proposal

Verbatim in `249-briefs/00-shared.md`, § Question 1 to § Question 3 (lines 288 to 807); in short:

- **Q1, the census**: eight things a route may change in what a program's own C observes (the handler's
  stack; the thread's mask; the signal numbers it may use; the frames its handler finds; the action a
  query answers and the order of installs; the context a handler reads and writes; `errno` and
  `SA_RESTART`; which thread a process-directed signal reaches, `00-shared.md:290-300`), and for each
  library which it depends on, measured. First entries: bdwgc, a Go `c-archive`, ART (read only), the
  users of SIGURG and of the real-time signals, libuv, OpenSSL, SDL, sqlite, and the tree's own ncurses and
  curl (`:390-401`).
- **Q2, the candidates**, each run against the census on Darwin and Linux arm64: **A**, a marker signal the
  runtime owns (a real-time signal on Linux; on Darwin, none exists); **B'**, a wrapper handler with a
  per-thread depth (248's route I); **C**, frame records kept in every unit the compiler builds
  (`-fno-omit-frame-pointer`); **D-tramp**, Darwin's trampoline (248's one-name I-dylib with I's depth);
  and **D**, routes nobody listed (a Darwin sweep, the C library's own `__in_sigtramp`, D-imports at build,
  a refusal at a library's first install, the kernel's facilities for a raw system call, the shadow stack,
  combinations by platform) (`:554-767`). What a route must close: on Darwin a refusal with R1's words
  before the callback's first C call; on Linux no Heroes code inside the signal (`:342-346`, `:556-559`).
- **Q3**: which class of programs the runtime or the compiler refuses or tells, on which platform, with
  which words, so that no program of 2522's shape runs on in silence or dies in silence, and whether that
  is a closing of 2522 under 247's R3 or a change to it, which is the author's (route K) (`:769-807`).

## The author's answer that frames it

**2026-11-15 at about 02:03**, on the coordinator's question put the evening before (the critic's first-pass
item 20, the lane, carried to the author, `00-shared.md:19-20`, `:880-881`; `critic-briefs.md:496-506`), the
coordinator's recommendation **c** confirmed in these words, *"I confirm your recommendations"*
(`249-briefs/critic-reports.md:11-17`): **the sitting is split**. This synthesis rules question 1 and
question 2 now; **question 3 and the blind seat run only if no candidate holds** on a platform. No candidate
holds on any POSIX platform (§ Does a candidate hold), so by that answer both are owed (R3).

## The verdicts

| seat | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | Q1 **object** to the premise that the runtime's own handlers are neutral to a bound library's; Q2 C **approve** as x86-64's complement for the units this compiler builds, **not** as a closing of 2522; **object** to B' as built and to B'' as a Linux route, **approve** B''-style confirmation as Darwin's depth paired with a reach into dylibs; **object** to A, to D-tramp, and to D-rebind as specified pending `injump`, **approve** D-rebind's reach paired with B''-style confirmation; Q3 **object** to any claim that a candidate built closes 2522, **object** to D-imports, *"closing 2522 needs the author's change to R3 (route K)"*; **no veto** (`compiler-engineer.md:650-752`) | design.md §1.12, §1.11, §1.1; CLAUDE.md § 10 | B' `runtime/parts/siginstall.c` 311 lines new (B'' 339), `sigguard.c` +17 -4, `runtime.c` +6, **0 compiler lines**, about 4.0 instructions a callback; C `flags.hero` 584 to 602, `hero_compiler.h` 109 to 116, `sigguard.c` 364 to 371; D-imports unbuilt, estimated 150 to 300 lines (`:701-703`, `:679-681`, `:741-742`) | `q1/gocensus.hero` exits 134 at `go_nilpanic` until `stack.c`'s null arm passes on a fault outside the program's images; with C landed the CI's Linux x86-64 job reads `run: 815 passed, 0 failed`; B'' landed, `kmiss` exits 0 on the Linux legs and 134 on Darwin; `injump` against D-rebind as specified exits 0; no route proposed refuses both `kmiss` and the twelve depth programs correctly on Linux x86-64 (`:669-671`, `:688-690`, `:711-712`, `:730-731`, `:749-750`) | C: a CI x86-64 run under the word with any of the three red; B'' on Linux: a run refusing `kmiss` without refusing a depth program; D-rebind: refusing `injump` 3 of 3 as it stands; Q3: a Linux route refusing `kmiss`, `injump`, 247's two cases and a `dlopen`ed library with no false refusal (`:691-692`, `:713-714`, `:732`, `:751-752`) |
| ffi-pragmatist | Q1 **approve** the census; **the I-onstack veto stands and widens** to Debian's `libgc1`; no other candidate refused by a first entry; Q2 Darwin **approve D-rebind** (a route nobody listed), **object** to D-tramp, to A on Darwin and to the sweep; Linux **approve B' only with the C library's exits dropping the depth, or A with its marker taken by glibc's `__libc_allocate_rtsig(0)`**, each with its residue told; **veto A with `SIGRTMAX` visible**; **object** to C as a route; Q3 **veto D-imports**, **object** to a refusal at a library's first install, closing needs *"the author's change to R3"* (`ffi-pragmatist.md:530-625`) | §1.11, §4.19, §1.12 for the vetoes | D-rebind about 120 lines of C linked as a static archive, one file; D-tramp's thunk question about 8 instructions; A's mask question 1,346; the sweep 41,107 a sweep; D-tramp and A's Darwin arm a file beside every program (`:348-368`, `:380`) | Debian's SDL3 3.2.10 on a `kmsdrm` console takes signal 36, never 64; a program binding Go 1.27.3's archive dies at its first recovered nil dereference until the null arm changes; 247's `fliblocale` against a Homebrew-shaped dylib refused 20 of 20 under D-rebind on each macOS the CI runs; B' with the exits at most 5 instructions a callback on Linux arm64; no census library installs by a raw `rt_sigaction` or a `dlsym`ed `sigaction` (`:555-558`, `:587-590`, `:615-617`) | Q1: a library a binding plausibly links measured to block, wait for or send 64, or to install a calling-back handler past the C library's names; Q2: for D-rebind a library reaching `sigaction` by `dlsym` or an Apple framework calling a program's function back, for B' a correct program refused, for A a library caching `SIGRTMAX` first; Q3: a route seeing a `dlsym` installer and a raw system call without leaving every child a seccomp filter (`:559-561`, `:591-594`, `:618-619`) |
| historian | **approve (advisory)** A on Linux with its collisions named; **object (advisory)** to A on Darwin; **approve** B' as a shape (*"Departure"*: ART meets a handler that may not return by declaration, never by detection); **approve** C; **object (advisory)** to D-tramp and the sweep; D-imports *"no objection, no precedent"*; Q3 **approve words that name the doors**, which *"the escape binds"* (`historian.md:67-79`) | precedent | | | a classic Darwin signal no runtime a program binds claims; a runtime using a blocked bit as a mark; a runtime detecting a handler left by `siglongjmp`; Apple documenting `__sigaction` and `sa_tramp`; Ubuntu 26.04's glibc packaging; where SDL's `sigtimedwait` comes from; a toolchain telling at build (`:232-248`) |

The historian's report ended at 00:00:56, before D-rebind existed (`stat`, the critic's item 9,
`critic-reports.md:417-419`), so no precedent was read for it.

## What each seat measured, in short

- **compiler-engineer** (`compiler-engineer.md`): **B'** built into the runtime (`:44-62`), refusing 247's
  two cases 20 of 20 on Darwin for a static archive, the storms' 20 reports all SIGABRT, none `lock`
  (`:71-81`), and 20 of 20 in Linux arm64, the shared library and the storms included (`:220-227`); **and
  refusing a correct program** that calls back from deeper than an abandoned wrapper after `siglongjmp`,
  `longjmp` or `_longjmp`, on Darwin, Linux arm64 and the emulated x86-64 image (`:91-105`, `:137`,
  `:231-233`). **B''**, the depth confirmed by the frame records, refuses none of the twelve depth programs
  (`:116`) and **misses `kmiss`**, a frameless handler whose frame register holds an address in the signal
  frame, on both Linux legs (`:119-141`, `:245`), Apple's arm64 target never allocating `x29` (`:120-121`).
  Neither reaches a Darwin dylib, 9 of 9 inside the signal (`:145-153`). Five of the fifteen 2494 programs
  move under each (`:195-210`). A handler that jumps **within itself** and then calls back (`injump`): base
  3 of 3 inside, B' and B'' 3 of 3 refused (`:174-186`). **C** built (`:336-359`): in the emulated image the
  `calls` case turns green at `-O2` and the `atexit` and `fork` cases stay red at `-O0` and `-O2`, at the
  base and under the word, through Debian's glibc (`:309-322`, `:393-405`); on Darwin and Linux arm64 no
  object byte moves (`:363-367`, `:431-432`). The CI at this base read `run: 812 passed, 3 failed`, the three
  at `-O2` (`:324-329`). The runtime's null arm kills a Go archive's recovered nil panic (`:10-15`,
  `:494-513`). D-imports would reach one program of the tree, and that one is correct (`:585-594`).
- **ffi-pragmatist** (`ffi-pragmatist.md`): **the census** by two tracers (Darwin, an `__interpose` dylib
  over libsystem_kernel's entry points; Linux, `LD_PRELOAD` over glibc's installers, waits, senders and
  `syscall`, `:50-66`), on both POSIX legs (`:68-149`). **A** built into the Linux runtime with the marker
  `SIGRTMAX`: 247's two cases 20 of 20, storms 20 of 20, `--sanitize` at both levels (`:173-183`); **a correct
  program blocking `SIGRTMAX` and waiting for it killed, exit 192, in silence** (`:199`, its veto). A on
  Darwin with SIGEMT (`:225-245`). **D-tramp** built and run (`:256-368`): libSystem installs with flag
  `0x400` and the kernel then checks the token `_sigtramp` hands on (`:268-272`). **The sweep** (`:363-368`).
  **D-rebind**, a route nobody listed (`:370-407`): the executable rewrites, in every loaded image outside
  the shared cache, the slots for `sigaction`, `signal` and the jumps, and hands the kernel D-tramp's
  trampoline: 247's two cases 20 of 20 through a static archive **and a two-level dylib**, storms 20 of 20
  and 5 of 5, every crash report joined to its storm, one file. **B'** as a C model with the exits
  dropping the depth (`:411-450`). D-imports' holes built and run, a `dlopen`ed library among them
  (`:483-499`).
- **historian** (`historian.md`): each census library's documented signal use at a named version
  (`:85-110`), among them two crash reporters whose handlers call back (`:104-105`); the precedents per
  candidate (`:112-163`): Go's four heuristics for SIGURG, glibc's own unblockable signals, ART's per-thread
  bitmap met by declaration, Fedora's, Ubuntu's (its glibc included) and Apple's frame records, Debian's
  glibc refusing them, Go 1.10's own `sa_tramp` left in Go 1.12, xnu's per-delivery token; who tells a
  program and in which words (`:165-200`).
- **The critic's pass over the reports** (`critic-reports.md`): the compiler-engineer's `injump` run against
  D-rebind as the ffi-pragmatist built it (its object `cmp`-equal to the seat's, its thunk instruction for
  instruction the seat's, `:28-68`): **missed 20 of 20, and three shapes beside it 20 of 20 each**
  (`:70-92`); **D-rebind as built kills every `--sanitize` program before `main`** (`:99-116`); on Linux,
  B' with the exits misses every in-handler jump shape and A the shapes that restore no mask (`:123-184`);
  D-rebind moves two 2494 goldens (`:186-204`); **no candidate holds on any POSIX platform** (`:206-225`);
  route K's sentence (`:310-337`); the route nobody asked (`:360-371`).

## The candidates graded

Every route the reports name, with the evidence and the seat that measured it. *Holds* means: refuses 247's
two cases and leaves no run residue of 2522's class (`critic-reports.md:208-210`).

| route | built, by whom | measured | the seats | holds |
|---|---|---|---|---|
| **I-onstack** (248's, vetoed) | 248's; its veto program re-run by the ffi-pragmatist | bdwgc's list check `Segmentation fault`, exit 139, **5 of 5 with Debian's own `libgc1`** and 5 of 5 with the tarball (`ffi-pragmatist.md:211-212`) | ffi-pragmatist's veto stands and widens (`:539-541`, `:595-596`) | no |
| **I-mask with SIGURG** (248's, its own condition met) | 248's Darwin dylib, re-run with SIGEMT | after `_longjmp` out of a handler a correct callback refused; under a parent that blocked the marker every callback refused (`ffi-pragmatist.md:37-40`, `:233-237`) | refused, carried | no |
| **A, Linux, `SIGRTMAX`** | built into the runtime, ffi-pragmatist | 247's two cases 20 of 20 static and shared, storms 20 of 20, `--sanitize` 3 of 3 at each level; `callback` 14 passed, `2494` 13 and 2 (`ffi-pragmatist.md:173-183`); **`swait_rt.c` killed, exit 192, in silence**; `hunblock` missed; a raw `rt_sigaction` and `dlsym(RTLD_NEXT)` missed (`:194-209`); **in-handler `longjmp`, plain and fortified, 5 of 5 each missed** in the runtime build (`critic-reports.md:157-168`) | ffi-pragmatist **vetoes** (`:595-598`); compiler-engineer objects (`compiler-engineer.md:718-719`); historian approves A on Linux with collisions named (`historian.md:73`) | no |
| **A, Linux, `__libc_allocate_rtsig(0)`** | **C model only** (`inst_A.c`; the runtime build holds no such switch, `critic-reports.md:217`, `:237`, `:409-412`) | `swait` with `SIGRTMAX` taken (glibc's `SIGRTMAX` moved to 63); a program naming 64 by number killed, exit 192 (`ffi-pragmatist.md:199-200`); the C model misses the in-handler jumps that save no mask (`critic-reports.md:145-153`) | ffi-pragmatist approves (`:570-571`) | not run as a Heroes program |
| **A, Darwin, SIGEMT** | 248's three-name dylib with SIGEMT, ffi-pragmatist | frameless rows refused; two false refusals (above); a program blocking SIGEMT and waiting for it killed, *"EMT trap: 7"*, exit 135; a file beside every program (`ffi-pragmatist.md:227-245`) | ffi-pragmatist objects (`:567-568`); historian objects: each classic signal precedent recommends is a runtime's (`historian.md:74`) | no |
| **B'** (248's route I, the depth kept across a jump) | built into the runtime, compiler-engineer | 247's two cases refused on Darwin (static) and Linux arm64 (static, shared, storms) (`compiler-engineer.md:71-81`, `:220-227`); **three correct programs refused after a jump out, on three images** (`:91-105`, `:137`, `:231-233`); every Darwin dylib missed, 9 of 9 (`:145-153`); five 2494 goldens move (`:195-208`); bdwgc intact 5 of 5 (`:260-263`); Go's preemption kept (`:264-268`); `runtime` suite one floor outgrown, 137 against 109 (`:272-277`); widens `ffi_defined_elsewhere` (`:482-488`); about 4.0 instructions a callback (`:285-288`) | compiler-engineer objects (`:696`); historian approves the shape (`historian.md:75`) | no |
| **B''** (the depth confirmed by frame records) | built into the runtime, compiler-engineer (+28 lines) | the twelve depth programs 12 of 12 exit 0; 247's `flib` 20 of 20 refused (`compiler-engineer.md:116-117`); **`kmiss` missed** on emulated x86-64 and Linux arm64 (`:136`, `:245`); every Darwin dylib missed (`:147`); the `runtime` suite not run for it (`:277`; `critic-reports.md:275-277`) | compiler-engineer: object as a Linux route, approve as Darwin's depth paired with a dylib reach (`:696-698`) | no |
| **B' with the exits dropping the depth** | **C model only** (`inst_B2.c`), ffi-pragmatist | C rows refused, no false refusal after a jump or `setcontext` (`ffi-pragmatist.md:442-447`); never run as a Heroes program (`critic-reports.md:218`, `:241`); **every in-handler jump shape missed, 6 of 6** (`:145-155`) | ffi-pragmatist approves on Linux (`:568-570`) | no |
| **C**, `-fno-omit-frame-pointer` asked of the machine | built in `flags.hero`, compiler-engineer | emulated x86-64: `calls` green at `-O2` under the word, `atexit` and `fork` red at `-O0` and `-O2` with and without it; `run` form 13 and 2 against the base's 12 and 3 (`compiler-engineer.md:393-405`); no object byte moved on Darwin or Linux arm64 (`:363-367`, `:431-432`); 247's two cases still inside under it (`:381-384`, `:436-437`); a library twelve frames deep still inside (`:371-375`); compiler tests the same 8 failures as the control, `cache` 9 and 0, `runtime` 10 and 0 (`:441-450`) | compiler-engineer approves as x86-64's complement, not a closing (`:676`); ffi-pragmatist objects as a route, approves as the complement (`:573-574`); historian approves (`historian.md:76`) | no (not a closing; below) |
| **D-tramp** | built as a dylib, ffi-pragmatist | the kernel accepts it; 247's two cases 20 of 20 static and two-level dylib, storms 20 of 20, every report SIGABRT, none SIGKILL; `--sanitize` 3 of 3 (`ffi-pragmatist.md:293-334`); **a program copied without its dylib dies before `main`**, exit 134 (`:352-362`); its source zeroes the depth at every jump (`ipose_tramp.c:52-62`, read by the critic, unrun against `injump`, `critic-reports.md:91-92`) | objected by both building seats (`ffi-pragmatist.md:566-567`, `compiler-engineer.md:720`) and the historian: Go left its own `sa_tramp` in 1.12 *"ensuring forward-compatibility"*, and xnu validates a token (`historian.md:77`) | no |
| **the sweep** (Darwin) | built, ffi-pragmatist | a library installing in a later call missed; one re-installed through libc missed; 41,107 instructions a sweep (`ffi-pragmatist.md:363-368`) | ffi-pragmatist objects (`:568`); historian objects (`historian.md:77`) | no |
| **D-rebind** (a route nobody listed) | built as a static archive in a program's group, ffi-pragmatist; inside a runtime copy by the critic | 247's two cases 20 of 20 static and two-level dylib, storms 20 of 20 and 5 of 5; hardened runtime passes; one file; a `dlopen`ed library and SDL2 reaching SDL3 reached; `dlsym(RTLD_DEFAULT)` missed (`ffi-pragmatist.md:382-407`); **`injump` missed 20 of 20, and plain `longjmp`, `_longjmp` and a nested handler's `siglongjmp` within the running handler 20 of 20 each** (`critic-reports.md:75-92`); **two `--sanitize` programs dead before `main`, 3 of 3 each, a C probe the same** (`:101-116`); 2494 programs 26 of 30, D's two cases now R1's words (`:197-204`) | ffi-pragmatist approves (`:565`); compiler-engineer objects as specified, predicting the `injump` miss (`compiler-engineer.md:720-731`), which held | no |
| **D-rebind with B''-style confirmation** | **unbuilt** (*"unbuilt as a pair"*, `compiler-engineer.md:648`) | none | compiler-engineer approves (`:647-648`, `:721`) | unbuilt |
| **D-imports** (a reading of a bound library's imports at build) | unbuilt | a bound library importing `dlopen dlsym raise` loads one whose frameless handler calls back: inside the signal 3 of 3 at the base, D-imports passes it (`ffi-pragmatist.md:484-488`); one program of the tree reached, a correct one (`compiler-engineer.md:585-594`); a new diagnostic (`:583`) | ffi-pragmatist **vetoes** (`:496-499`); compiler-engineer objects (`:736-737`); historian no precedent (`historian.md:78`) | no |
| **a refusal at a library's first install** | unbuilt | the census's start-up installs (Go, SDL, ncurses, bdwgc, libuv, GLib) would refuse correct programs (`ffi-pragmatist.md:500-505`) | ffi-pragmatist objects | no |
| **`__in_sigtramp` as a mark** (Darwin) | probe | one int for the process, and `dlsym` returns NULL (`ffi-pragmatist.md:242-243`, `:273-275`) | cannot serve | no |
| **seccomp's trap on `rt_sigaction`** | read | a filter is preserved across `execve` and needs `no_new_privs` or `CAP_SYS_ADMIN` (`ffi-pragmatist.md:427-436`; `historian.md:152-160`) | a question | not a route |
| **syscall user dispatch** | read by the historian only | *"Any fork or exec of the existing process resets the mechanism to PR_SYS_DISPATCH_OFF"*, *"not a mechanism for sandboxing system calls"* (`historian.md:152-160`); its manual page unread by the ffi-pragmatist (`ffi-pragmatist.md:435-436`) | a question | not a route |
| **the shadow stack** (CET, GCS) | read and probed | neither Linux image offers one (`compiler-engineer.md:31-33`, `:554-561`) | a question | not a route |
| **Linux's `sa_restorer`** (posed, `00-shared.md:619-623`) | none | no report weighs it | | not run |
| **248's B, T, U, W, R, I-dylib; 247's A by name, E, F, S, H, I** | refused, carried (`00-shared.md:556-566`) | no seat names a measurement that changes a refusal; D-tramp and D-rebind are I-dylib's descendants | | refused |
| **A jump within a handler told from a jump out, by the setjmp family's frame** (the critic's item 17, unlisted) | **unbuilt** | none (`critic-reports.md:362-371`, `:435-437`) | nobody | unbuilt |
| **route K** | the author's | none | both building seats say closing 2522 needs it (`compiler-engineer.md:736-739`, `ffi-pragmatist.md:518-528`) | R4 |

## Does a candidate hold, platform by platform

From the critic's table (`critic-reports.md:212-221`), every cell a run named there:

- **Darwin arm64**: none. D-rebind, the nearest, misses the in-handler jump in four shapes (20 of 20 each),
  an installer found by `dlsym(RTLD_DEFAULT)`, and kills `--sanitize` programs before `main`; shared-cache
  images and a raw `__sigaction` naming its own trampoline are its holes by reading. B'' misses every dylib.
- **Linux arm64**: none. A with `SIGRTMAX` is vetoed by its builder and misses the in-handler `longjmp`
  shapes, an unblocking handler, a raw `rt_sigaction` and `dlsym(RTLD_NEXT)`; A by glibc's allocator and B'
  with the exits were never run as Heroes programs, and their C models miss in-handler jumps (A the three
  that restore no mask; B' with the exits all six); B' refuses three correct programs; B'' misses `kmiss`.
- **Linux x86-64**: none, measured emulated only. C leaves 247's two cases and Debian's `exit` and `fork`
  frames; B' and B'' there refuse `flib`, B' with three false refusals, B'' missing `kmiss`
  (`compiler-engineer.md:130-141`).
- **Windows**: no POSIX signal, no walk and no probe (247's R10), no candidate built; nothing of 2522's shape
  measured there tonight; owed to the CI's leg (`ffi-pragmatist.md:532-535`).

## The resolution adopted

`provisional — author ratification pending`. Robust over conservative wherever the two differ, the
conservative form recorded in § Conservative alternative.

**R1, question 1, the census: approved**, by every seat that judged it (`ffi-pragmatist.md:539-541`;
`compiler-engineer.md:656-672` objects to a premise of it, not to the census; `historian.md:85-110`).

- **The first entries install handlers, and none calls the program back from inside one**: seven first
  entries install, all through the C library's own names, traced on both legs (*"Seven first entries install
  handlers; none calls the program back from one; all install through the C library's names"*,
  `ffi-pragmatist.md:549-550`; the rows `:72-86`, `:110-122`). libuv's handler writes a pipe and its loop
  calls back (`:74`, `:112`); SDL posts a quit event, and `SDL_HINT_NO_SIGNAL_HANDLERS` turns it off (`:76-79`,
  `:114`); sdl2-compat installs SDL3's handlers through `dlopen` on this Mac (`:78`); ncurses installs only
  over a default action (`:81`, `:116`); OpenSSL installs nothing at start on either leg (`:75`, `:113`);
  curl 8.14.1 sets SIGPIPE to `SIG_IGN` around each transfer and Homebrew's 8.23.0 toggles nothing (`:82`,
  `:117`); sqlite installs nothing, a callback through `sqlite3_exec` included (`:80`, `:115`). A Go 1.24.4
  archive's masks, alternate stacks and `tgkill` are raw system calls of its own, which the Linux tracer
  does not see (`:120`); its installs go through libc's `sigaction`.
- **Beyond the first entries the pages say otherwise for crash reporters**: Abseil's failure handler calls
  `writerfn` from its handler (*"Since this function runs within a signal handler, it should be
  async-signal-safe if possible"*) and Breakpad calls its `FilterCallback` and crash handler in *"a
  compromised context"* (`historian.md:104-105`), read and not run. So *none calls back* holds of the first
  entries measured, not of every library a binding plausibly links.
- **bdwgc on Darwin takes no signal**, plain or incremental, its list check intact after 15,208 and about
  34,000 collections (`ffi-pragmatist.md:85`); on Linux Debian's build takes SIGPWR and SIGXCPU, incremental
  mode adding SIGSEGV and SIGBUS (`:119`). **The I-onstack veto stands on Debian's own `libgc1`**, 5 of 5
  exit 139 (`:211-212`), so its program is Linux's.
- **Who owns SIGURG**: a Go archive's constructor installs it before `main` on both legs (16 on Darwin, 23
  on Linux); the runtime's probe borrows it for two raises at the first crossing and gives Go's handler back,
  and Go's preemption works before and after (`ffi-pragmatist.md:151-157`, `:91-94`;
  `compiler-engineer.md:536-542`). For the instant of the two raises a SIGURG aimed at the process runs the
  probe, a reading (`compiler-engineer.md:540-543`), unmeasured (§ Found beside the question).
- **The four points the critic added**, read off the runs (`ffi-pragmatist.md:130-149`): Go installs before
  `main` and the runtime's guard chains to it; Go edits the context to preempt and to make a panic; libuv and
  GLib read `errno` in their handlers; libuv, GLib and Go start their threads with every signal blocked.
- **The runtime's half** (`compiler-engineer.md:515-535`): the runtime's 27 install lines and 18 mask lines
  each name the call, so a definitions route reaches each; R2's poller thread would be born with every
  signal blocked but A's marker. **The runtime's null arm kills a Go archive's recovered nil panic**, exit
  134, blaming a `nullptr` handed to C, on both legs at the base and under every candidate
  (`compiler-engineer.md:494-513`, `ffi-pragmatist.md:20-27`): defect 2558, filed (§ Found beside the
  question).
- **What the census does not reach**: an import is not a behaviour (`00-shared.md:468-469`); a library
  reached by `dlopen`, an installer reached by `dlsym`, a raw system call, and an inline `svc`, which the
  Linux tracer does not see (`ffi-pragmatist.md:65-66`). **Whether any census library jumps within a handler
  and then calls back was never asked** (`critic-reports.md:278-281`).

**R2, question 2, the candidates: none holds on Darwin, Linux arm64 or Linux x86-64; no route is adopted;
defect 2522 stays open and `blocking`**, panel 247's R1 and R2 and D standing beneath it as 248's R1 left
them (`docs/panel/248-*.md:224-226`). `gh issue view 2522` at the time of writing: `OPEN`, `kind:defect`,
`class:blocking`, `area:runtime`, M-core-packages.

- **The approvals do not stand as built**. The ffi-pragmatist's Darwin approval, D-rebind, misses the
  compiler-engineer's `injump` 20 of 20 and three shapes beside it, as the compiler-engineer predicted from
  its design (`critic-reports.md:75-92`; `compiler-engineer.md:180-186`, `:730-731`), and kills `--sanitize`
  programs before `main` (`critic-reports.md:101-116`). Its two Linux approvals rest on C models alone, never
  run as Heroes programs, stormed, sanitized or held to a golden (`critic-reports.md:256-259`, `:409-412`), and
  those models miss in-handler jumps (`:145-155`). The compiler-engineer's Darwin approval, D-rebind with
  B''-style confirmation, is unbuilt (`compiler-engineer.md:648`). `/panel` 3c: a route that does not build
  is not adopted (`.claude/skills/panel/SKILL.md:274-277`).
- **The depth's three ways each pay one side, run** (`critic-reports.md:178-184`): kept across a jump
  (B'), a correct program refused after a jump out; dropped at the jump (D-rebind, D-tramp by source, B' with
  the exits, A's `longjmp` release), a Heroes function inside the signal after a jump within; confirmed by
  frame records (B''), exact on Darwin and a miss on Linux.
- **C stands apart.** The compiler-engineer approves it as an x86-64 fix for the tree's own units, *"not as
  a closing of 2522"* (`compiler-engineer.md:676`): under it the emulated image reads the `calls` case green
  at `-O2`, and it leaves 247's two cases, a library deeper than eight frames, Windows, and Debian's
  x86-64 `exit` and `fork` frames, red at every level there (`:456-470`). **What the reports leave unmeasured
  of it**: no instruction counted on x86-64, the one machine where it changes code (no counter in the image,
  `:407`, `:421-422`); its text cost is `size`'s column of two objects, once each, at `-O2` (the runtime's
  +0.69 %, one linked program +0.62 %, the program's unit +6.3 %; the seed's C 7.5 % smaller, measured once,
  why unread, `:411-416`; `critic-reports.md:269-274`); **`run` whole unrun**, *"Owed, before a landing"*
  (`compiler-engineer.md:451-454`); the CI prediction (`run: 815 passed, 0 failed`) unrun, resting on Ubuntu
  26.04's glibc keeping frame records, which the historian read for 24.04 only (`historian.md:48-53`); the
  CI's clang 21.1.8 emission of the three handlers unread (`00-shared.md:656-657`); the Windows leg's object
  owed (`compiler-engineer.md:342-346`). C is not adopted here; it is a candidate, and R3's order binds it.
- **Windows**: nothing new, by 247's R10 and 248's rows, carried; no candidate builds there.
- **The escape binds no adoption**, since none is made. For the record: B' and B'' widen
  `ffi_defined_elsewhere` (run, `compiler-engineer.md:482-488`); A does too by construction, unrun for A's
  names (`critic-reports.md:292-295`), needs a reserved signal and on Darwin a file beside every program
  (`ffi-pragmatist.md:247-254`); D-tramp a file beside every program; D-imports a new diagnostic; D-rebind
  none of the four by its refusals and names (`critic-reports.md:285-291`), though it moves two goldens and
  rests on interfaces no header declares; C none.

**R3, the follow-up, in this order.**

1. **The route nobody listed is built first, by a seat with Bash, before any candidate is adopted**: tell a
   jump out of a handler from a jump within it, by recording the setjmp family's frame (the critic's item 17,
   `critic-reports.md:362-371`, `:435-437`): the definitions (Linux) or the rewritten slots (Darwin) of
   `setjmp`, `_setjmp`, `sigsetjmp` and `__sigsetjmp` record each buffer's caller frame and tail-call the C
   library's own, and a jump drops only the depth entries whose wrapper or trampoline frame lies deeper than
   its target's. Who, by the critic: the ffi-pragmatist with the compiler-engineer. Built into D-rebind and
   into the compiler-engineer's B' (and A's Linux definitions where A is still a candidate), run on `injump`
   and the three shapes beside it, the twelve depth programs, `kmiss`, 247's two cases static and through a
   dylib, on Darwin, Linux arm64 and the emulated x86-64 image, its own holes named (a buffer copied by
   `memcpy`, a `setjmp` the C library makes inside itself). It is the one route measured tonight to be needed
   by every candidate that keeps a depth or a marker (`critic-reports.md:435-437`).
2. **Each candidate's owed runs** (§ What the follow-up owes), D-rebind's `--sanitize` loop repaired first.
3. **Question 3 and the blind seat, owed by the author's answer** (no candidate holds), **convened after the
   author's reading of route K (R4)**, since the words question 3 weighs depend on whether the residue
   becomes a known cost or stays refused. Question 3 is the escape's: any answer short of *nothing changes*
   is a refusal class or its words with spec § 13's sentence behind it (`critic-briefs.md:114-120`; the
   historian's reading favours new words, `historian.md:79`), so the spec-warden sits with the blind seat
   (`00-shared.md:51-61`).
4. **The blind seat's size**, which no brief of this sitting named (`00-shared.md:11-12`): **recommended at
   most 10 USD for all its paid sessions together**, the ceiling the author gave panel 250's blind seat the
   same night (*"the blind seat the author approved on 2026-11-15 at about 02:03 with a ceiling of 10 USD"*,
   `.claude/worktrees/p250/docs/panel/250-briefs/critic-reports.md:54-55`, as the coordinator relays it,
   uncommitted at this writing); panel 250's shape, 24 sessions capped at 0.40 USD each and a runner that
   stops at 9.51 spent (`250-briefs/llm-ergonomist.md:161-167`), is one that fits. **A recommendation
   awaiting the author**: a paid run is the author's every time (`.claude/rules/verification.md` § The
   batch). The spec-warden's `heroes measure --refresh` calls are on the sitting's *no paid run* list
   (`00-shared.md:166-168`; `critic-briefs.md:131-136`) and are named with their count in its brief and asked
   with the blind seat.

**R4, route K, put to the author as a decision.**

- **247's R3 as it stands** (`docs/panel/247-*.md:191-195`): *"What R1 and R2 leave is a defect of its own,
  `blocking`, filed in the hour 2494 closes: a Heroes function a C handler calls through frames the walk
  cannot read (a library built without frame records, `flib-nofp`), which allocates nothing and so passes D:
  `c-fliblocale` dies by SIGKILL in silence on Darwin and runs inside the signal on Linux arm64 (the
  ffi-pragmatist's § 4). Its cases are those two. Never a known cost: it blocks M-core-packages' tag like any
  `blocking` defect."* *"Its cases are those two"* names the falsifiers, not the class, so a residue a route
  leaves stays `blocking` under it whichever route lands (`critic-reports.md:316-318`).
- **The change both building seats ask** (`compiler-engineer.md:736-739`; `ffi-pragmatist.md:518-528`), as
  the critic puts it for a yes or a no (`critic-reports.md:320-323`): *"May defect 2522 close with a named
  residue told rather than refused (on each platform, the programs whose library's handler calls a Heroes
  function in a way the adopted route cannot see), so that the residue no longer holds M-core-packages'
  tag, R3 amended to say so and the residue written where spec § 13 states the rule?"* The ffi-pragmatist's
  narrower residue, *"the class of a library that installs a handler past the C library's installers"*, is
  too narrow for what was run: the in-handler jump installs through `sigaction` and jumps through the C
  library's own `siglongjmp` (`critic-reports.md:323-326`, `:87-91`).
- **The residue each candidate leaves under it, run** (`critic-reports.md:328-337`, with the seats'
  rows): **D-rebind** (Darwin) the in-handler jump in four shapes, a `dlsym` installer, shared-cache images,
  a raw `__sigaction` naming its own trampoline, and until repaired every `--sanitize` program; **B''**
  (Darwin) every dylib; **A** (Linux) the in-handler jumps that restore no mask, a handler that unblocks
  first, a raw `rt_sigaction`, `dlsym(RTLD_NEXT)`, and a program using the marker dies; **B' with the exits**
  (Linux) every in-handler jump, a raw `rt_sigaction`, `dlsym(RTLD_NEXT)`; **B''** (Linux) `kmiss`, a raw
  call, `dlsym(RTLD_NEXT)`; **C** (x86-64) libraries built apart (247's two cases), frames deeper than eight,
  a libc without records (Debian's `exit` and `fork`); **Windows** no walk and no probe, nothing of 2522's
  shape measured.
- **The recommendation: no, tonight.** 247's R3 stands as written while the follow-up builds R3's first
  item, and the sentence is put again with the residue that route leaves. The reason, measured: the one
  residue run tonight inside libraries that install and jump through the C library's own names, the
  in-handler jump (D-rebind 4 shapes of 20 of 20; B' with the exits 6 of 6; A the three shapes that restore
  no mask), is the class R3's first item is built to close, and a yes tonight makes it a known cost before
  that route runs. The residues past the C library's names are not shown closed to every route either: the
  historian's page says syscall user dispatch resets at fork and exec (`historian.md:152-160`), the property
  the ffi-pragmatist's condition asks of a facility that sees a raw call (`ffi-pragmatist.md:618-619`), and
  whether a `dlsym` the runtime defines (Linux) or rewrites (Darwin) closes the `dlsym` hole was asked by no
  seat: two questions, unrun (§ Left open). The decision stays the author's.

**Refused**: I-onstack (vetoed, its program now Debian's own collector); I-mask with SIGURG (its condition
met, carried); A with `SIGRTMAX` visible (vetoed); A on Darwin, D-tramp and the sweep (objected); B' as
built (objected, three false refusals); B'' as a Linux route (objected, `kmiss`); D-imports (vetoed); a
refusal at a library's first install (objected); `__in_sigtramp` as a mark (cannot serve). **Not adopted,
not refused**: D-rebind and the two Linux approvals, each pending the follow-up's runs; D-rebind with
B''-style confirmation (unbuilt); C (apart, R2); route K (R4).

## What a veto compels

- **The ffi-pragmatist's veto of I-onstack**, on bdwgc's list check faulting with Debian's own `libgc1` and
  with the tarball, 5 of 5 each (`ffi-pragmatist.md:211-212`, `:595-596`): I-onstack stays refused. On Darwin
  bdwgc takes no signal (`:85`), so the veto's program is Linux's.
- **The ffi-pragmatist's veto of A with `SIGRTMAX` visible**, on `swait_rt.c x 0`, a correct C program
  blocking `SIGRTMAX` and waiting for it, killed by signal 64, exit 192, saying nothing (`:596-598`): A in
  that form is refused.
- **The ffi-pragmatist's veto of D-imports**, on a bound library that `dlopen`s one whose frameless handler
  calls back, the Heroes function inside the signal 3 of 3 while D-imports' reading passes it
  (`:496-499`, `:606-609`): D-imports is refused, 247's veto of a name list reaching it in both building
  seats' reading (`compiler-engineer.md:597-599`).
- **The compiler-engineer cast none** (`compiler-engineer.md:652-654`). The historian is advisory.

## Disagreements, not smoothed

- **D-rebind.** The ffi-pragmatist approves it on Darwin (`ffi-pragmatist.md:565`); the compiler-engineer
  objects as specified, predicting from its design that `injump` runs inside the signal (`compiler-engineer.md:643-648`,
  `:730-731`). **Settled by a run**: the critic built D-rebind byte for byte as the seat did and `injump`
  ran inside the signal 20 of 20 (`critic-reports.md:28-92`).
- **The depth on Linux.** The ffi-pragmatist approves B' with the exits dropping the depth, on a C model
  (`ffi-pragmatist.md:568-570`, `:437-447`); the compiler-engineer's B' keeps it and refuses correct programs,
  his B'' confirms it and misses `kmiss` (`compiler-engineer.md:696-698`). The critic ran the trade: dropping
  misses every in-handler jump (`critic-reports.md:145-155`). Neither side holds, by the runs.
- **A on Linux.** The ffi-pragmatist approves the allocator form and vetoes the `SIGRTMAX` form; the
  compiler-engineer objects to A, citing that veto (`compiler-engineer.md:718-719`); the historian approves
  A on Linux with its collisions named, and records that no page read uses a blocked bit as a mark
  (`historian.md:73`). The allocator form never ran as a Heroes program.
- **C.** Approved by the compiler-engineer as a complement, not a closing; objected by the ffi-pragmatist as
  a route and approved as the same complement (`ffi-pragmatist.md:573-574`); approved by the historian,
  advisory, the residue *"a distribution's choice on the pages"* (`historian.md:76`). They agree it is no
  closing of 2522.
- **Question 3's words.** The historian's reading favours new words that name the doors, HotSpot's shape,
  which the escape binds (`historian.md:79`); the compiler-engineer finds no route built that needs new
  words, every refusal being R1's (`compiler-engineer.md:603-609`). They differ on question 3, not on a
  route (`critic-reports.md:298-301`); question 3 now runs (R3).
- **Route K's residue.** Both building seats ask the author's change to 247's R3; the ffi-pragmatist words
  the residue as installs past the C library's names, which the critic's in-handler runs show too narrow
  (`critic-reports.md:323-326`).

## Conservative alternative, recorded

- **Route K answered yes tonight, in the critic's sentence, and the follow-up adopting the nearest
  measured candidate per platform with its residue told, R3's first item not built first**: on Darwin
  D-rebind once its `--sanitize` loop is repaired and run, on Linux B'' (no false refusal on the twelve
  depth programs, `kmiss` missed), on x86-64 C with it; 2522 closing on the told residue. Its residue is
  R4's list for those three, the in-handler jump on Darwin and `kmiss` on Linux among them. Narrower still,
  route K answered yes and no route landed at all: 2522 closes with today's whole residue told, 247's two
  cases still dying by SIGKILL in silence on Darwin and running inside the signal on Linux arm64
  (`00-shared.md:318-340`), and the CI's Linux x86-64 leg still red at `-O2` (`compiler-engineer.md:324-329`).

## Prior art, per resolution

None. No resolution of this sitting adopts a route, and none rests on a precedent the historian sourced:
R1 rests on the seats' runs, R2 on the runs and the critic's, R3's first item on the critic's reading of the
runs, and R4 is the author's. The historian's candidate lines stand in `historian.md:312-352` for the
sitting that adopts a route, each to be re-read then; D-rebind has no precedent reading at all
(`critic-reports.md:417-419`).

## Found beside the question

For the coordinator to file or fold into 2522, each with its reproducer.

- **Defect 2558, filed**: *"the runtime's null arm kills a fault a library's own handler would recover"*,
  `OPEN`, `class:blocking`, `area:runtime`, created 2026-11-14T23:34:34Z (`gh issue view 2558`). Reproducers:
  `.claude/worktrees/p249-scratch/249-compiler-engineer/go/census/` and `q1/gocensus.hero`
  (`compiler-engineer.md:494-513`); `249-ffi-pragmatist/c/go/gohero.hero`, its log
  `logs/go-hero-darwin.txt` (`ffi-pragmatist.md:20-27`, `:88-101`).
- **Unfiled: the tree's own `atexit` and `fork` cases run the Heroes function inside the signal on Debian
  x86-64 at every level** (`compiler-engineer.md:309-322`, emulated; at `-O0` the `atexit` case exits 0 and
  prints `the atexit function ran: 42` where its `.expected` holds R1's refusal, `amd64/base.log:9-19`).
  Reproducer: `249-compiler-engineer/amd64/run.sh` run as `git archive 722ae76bf | timeout -s KILL 5400 docker
  run -i --rm --name ce-249-1 -e TAG=base -v <folder>/amd64:/out heroes-linux:latest bash /out/run.sh`
  (`compiler-engineer.md:302-303`); the critic's C model of the walk, `critic/glibc/walk.c`, its logs
  `walk-x86_64.log` and `walk-aarch64.log` (`critic-briefs.md:66-103`). Searched by the critic, *Debian atexit
  fork frame*, *x86-64 glibc frame records walk*: nothing (`critic-reports.md:343-346`). It is 2522's class
  (a Heroes function a C handler reaches through frames with no record), reached through the C library's own
  frames (`00-shared.md:777-779`), and no CI leg runs Debian x86-64. **Recommended: fold into 2522** as a
  comment naming the reproducer.
- **Unfiled, and not yet a defect: the SIGURG probe's window** (`compiler-engineer.md:540-547`, a reading):
  for the instant of the probe's two raises, a SIGURG aimed at the process, a Go preemption included, runs
  the probe instead of its owner's handler. Unmeasured; searched, *SIGURG probe borrowed*, *probe SIGURG
  lost*: none (`critic-reports.md:347-350`). What would settle it: a program counting its own SIGURGs, a
  sender thread and a first crossing. No reproducer exists; the follow-up's to run, since A on Linux would
  move the probe to the marker (`compiler-engineer.md:543-546`).
- **The compiler's own tests fail 8 in a copy made by the brief's recipe**: `git ls-remote` inside the
  package-manager tests climbs to the copy's `.git` file, *"fatal: gitfile does not point to a valid
  repository: .../tree/.git"* (`249-compiler-engineer/ctests-base.txt:299`, `:1541`; `2160 tests, 8 failed`
  at `:2434`; the same eight in `ctests-c.txt`, `compiler-engineer.md:441-447`). Measured in the
  compiler-engineer's copy alone, at the base and under C; the recipe's, not the tree's; whether tests that
  git can carry outside their fixture are hermetic enough is the coordinator's to judge
  (`critic-reports.md:351-354`).
- **The routes' own, not the tree's**: D-rebind's ASan loop (`critic-reports.md:99-116`,
  `critic/reports/asanprobe/q.c`); B''s `runtime` floor, 137 against 109, its 28 objects' thread audit undone
  (`compiler-engineer.md:272-277`); 248's Darwin I-mask dylib's two false refusals (`ffi-pragmatist.md:37-40`).
- **Facts of the instruments, for the next seat**: glibc 2.41's query writes past the kernel's 64 bits of
  `sa_mask`, so masks compare by `sigismember` (`ffi-pragmatist.md:28-31`); bdwgc 8.2.8 aborts at start on
  Apple silicon unless built `-DNO_EXECUTE_PERMISSION` (`:32-34`); Go refuses cgo in the Linux image unless
  `CC=clang` (`:35-36`); 248's critic's `swait.c` hangs on Darwin at the base, the kernel discarding a blocked
  SIGURG whose action is the default (`:41-42`); the first line of `logs/rebind-darwin.txt`, *"__in_sigtramp
  found by dlsym: 6"*, is a D-tramp label printed over D-rebind's six rewritten slots (`critic-reports.md:356-358`).

## What the follow-up owes

- **R3's first item**, built and run as R3 says.
- **D-rebind**: its `--sanitize` loop repaired (take the real `sigaction` from libSystem by a handle, or leave
  ASan's image unrewritten, both unbuilt, `critic-reports.md:114-116`) and the `run` form's `--sanitize` arms
  run under it; the census's libraries under it on Darwin (libuv, ncurses' handlers that call `sigaction` and
  `kill` from inside themselves, GLib, OpenSSL's prompt, sqlite as the control, `:379-380`); beside other
  interposers (TSan, a `DYLD_INSERT_LIBRARIES` tool, a second rebinder, `:377-378`); on each macOS the CI's
  Darwin legs run; and a reading of its forward compatibility, the historian's D-tramp objection reaching its
  trampoline and fishhook's technique on current dyld read by nobody (`:372-376`). Its two moved goldens said
  in its landing (`:197-204`).
- **A by glibc's allocator and B' with the exits**, if still candidates after R3's first item: built into the
  runtime and run on 247's two cases, storms, `--sanitize`, the 2494 goldens and the census inside a Heroes
  program (`critic-reports.md:237`, `:241`); A's widened diagnostic run for A's names (`:292-295`); whether a
  library caches `SIGRTMAX` before the allocator moves it (`ffi-pragmatist.md:594`).
- **B''**: the `runtime` suite under it, the floor's 28 objects audited (`critic-reports.md:275-277`); `injump`
  and rows (4), (5), (7) on Linux (`:240`).
- **C**: `run` whole; instructions on an x86-64 counter; Ubuntu 26.04's glibc packaging read; the CI's clang
  21.1.8 emission read; the Windows leg's object (R2).
- **Whether B' or A turns the tree's `atexit` and `fork` cases green on Debian x86-64**, the program's own
  install going through the definitions (`critic-reports.md:381-383`).
- **Question 3 with the spec-warden and the blind seat**, after the author's reading of route K, the blind
  seat's paid sessions within the ceiling the author gives (R3's item 4).

## Left open, as questions

- Whether a jump out can be told from a jump within by its target's frame, on all three POSIX images (R3).
- Whether syscall user dispatch sees a library's raw `rt_sigaction` at a cost a program bears: the
  historian's page says it resets at fork and exec (`historian.md:152-160`), which answers the ffi-pragmatist's
  objection to seccomp's filter in every child (`ffi-pragmatist.md:427-436`); this synthesis's reading of
  the two reports, unrun, and its manual page unread by the seat that would build it.
- Whether a `dlsym` the runtime defines (Linux) or rewrites in every image (Darwin) closes the `dlsym`
  installer hole every route left (`ffi-pragmatist.md:209`, `:390`, `:489-490`): asked by no seat, unrun.
- Whether D-rebind's trampoline survives the next macOS: Go left its own `sa_tramp` in Go 1.12, and xnu fixes
  sigreturn's token validation at a process image's first install (`historian.md:54-61`, `:140-151`).
- Whether any library a binding plausibly links jumps within a handler and then calls back
  (`critic-reports.md:278-281`).
- Which signal Debian's SDL3 takes on a `kmsdrm` console (the ffi-pragmatist's prediction, unrun and
  conditional, `critic-reports.md:260-268`).
- The SIGURG probe's window (§ Found beside the question).
- What Darwin's shared-cache images and Apple's frameworks install, which no rewrite reaches
  (`ffi-pragmatist.md:400-402`).

## Predictions to score

- compiler-engineer: `injump` against D-rebind as specified exits 0, `answer ran for 30` (missed)
  (`compiler-engineer.md:730-731`): **held**, 20 of 20 by the critic (`critic-reports.md:78`, `:83-86`); its
  condition, *"D-rebind refusing `injump` 3 of 3 as it stands"*, not met. Against D-rebind with B''-style
  confirmation, exit 134: unscored, unbuilt. `q1/gocensus.hero` exits 134 at `go_nilpanic` on both legs until
  `stack.c`'s null arm passes on a fault outside the program's images: at M-core-packages' close, with 2558.
  With C landed, the CI's Linux x86-64 job reads `run: 815 passed, 0 failed`: unscored, C not adopted. B''
  landed, `kmiss` exit 0 on the Linux legs and 134 on Darwin: unscored, B'' not adopted. No route proposed to
  this sitting refuses both `kmiss` and the twelve depth programs correctly on Linux x86-64, by
  M-core-packages' close: open, R3's first item its falsifier.
- ffi-pragmatist: Debian's SDL3 on a `kmsdrm` console takes 36, never 64: unscored, no console here. A
  program binding Go 1.27.3's archive dies at its first recovered nil dereference until the null arm changes:
  with 2558. 247's `fliblocale` against a Homebrew-shaped dylib refused 20 of 20 under D-rebind on each macOS
  the CI runs: unscored, D-rebind not adopted. B' with the exits at most 5 instructions a callback on Linux
  arm64: unscored. No census library installs by a raw `rt_sigaction` or a `dlsym`ed `sigaction`: open, the
  next census's.
- historian: conditions, no predictions (`historian.md:232-248`).

## Process notes

- **The lane**: the soundness lane and the historian, the critic's first-pass item 20 (escalate now, or say
  why question 3 and D-imports wait) put to the author by the coordinator (`00-shared.md:19-20`); the author's
  answer of about 02:03 split the sitting (§ The author's answer).
- **The two building seats stopped on the API's limit**, by the coordinator's account at about 00:40, and
  resumed at about 01:50 from their own reports. The compiler-engineer's report records it: *"Stopped by the
  API's rate limit at about 00:40 and resumed at 01:47; every container's log was read before going on"*
  (`compiler-engineer.md:6`). **The ffi-pragmatist's report records no stop**: it ends *"Finished 2026-11-15
  00:33 CET"* (`ffi-pragmatist.md:627`), its file last modified 00:33:25 (`stat`).
- **The coordinator's session stood idle from about 02:24 to 07:50 on 2026-11-15**, by the coordinator's
  account; no report records it. The second pass's brief says it was written *"on 2026-11-15 at about 02:40,
  after the three seats had finished"* and that *"no second-pass brief was written with the others"*
  (`249-briefs/critic-reports.md:3-4`); its file was last modified at 07:59:22 (`stat`); the second pass ran
  from about 07:58 to 08:43:02 (`critic-reports.md:3`, `:439`).
- **A file written outside the repository's root**: the first-pass critic's bare `go version` at about 22:54,
  `HOME` unmoved, wrote three files under `~/Library/Application Support/go/telemetry/local/`, born
  22:54:53 (a `go@go1.27.3-...-2026-11-14.v1.count`, `local.2026-11-13.json`, `upload.token`), reported for
  the author and deleted by nobody then (`critic-briefs.md:52-64`); the brief's rule was then made to bind
  every `go` command (`00-shared.md:177-196`). **Deleted on the author's answer *a*, by literal path**, by the
  coordinator's account; this synthesis's `ls` and `stat` of that folder find neither the
  `-2026-11-14.v1.count` file nor `local.2026-11-13.json`.
- **The system's crash reports** for every seat's aborting run were written by macOS's crash reporter into
  `~/Library/Logs/DiagnosticReports`, outside the root, by the system and not by a seat, and read there as
  00-shared.md's rule asks (`critic-reports.md:442-443`; `00-shared.md:200-202`).
- **The compiler's own tests** fail 8 in a copy made by the brief's recipe, its `.git` file the cause
  (§ Found beside the question).
- **The compiler-engineer changed the brief's order**: B' was built and run on Darwin first, its one container
  slot held by the emulated runs (`compiler-engineer.md:39-42`).
- **The critic recovered what the ffi-pragmatist's report left out**: D-rebind's object is `clang -O1 -Wall`
  (byte-identical to the seat's), and the thunk's patch was read back from the seat's binary
  (`critic-reports.md:30-47`).
- **Defect 2533 landed during the sitting**: `CLOSED 2026-11-15T06:57:05Z`, *"Fixed by 0652048d2"*, an ancestor
  of `main` (`critic-reports.md:302-308`; `gh issue view 2533`). So the order the briefs asked this synthesis
  to state (`00-shared.md:797-800`) is done, 2533 first, as the compiler-engineer recommended
  (`compiler-engineer.md:616-625`); every refusal in every seat's run prints the old remedy, the base
  predating `0652048d2`; `hero_callback_entered`, where every route patches, is unchanged on `main`.
- **The Windows box**, off at 22:11 and 23:07, *"is on tonight but is not this sitting's"* by the second
  pass's brief (`249-briefs/critic-reports.md:69-70`); nothing of this sitting ran there.
- **The machine** stood at load averages from 14.84 to 428.22 through the sitting (`00-shared.md:98-100`,
  `ffi-pragmatist.md:14-15`, `critic-reports.md:11-13`), so every count is instructions, exits or bytes, and
  every emulated duration indicative.
- **No paid run**, no GitHub write and no commit by any seat; the historian saw no file written outside the
  root by its fetches (`historian.md:14-16`).
- **The evidence archive** (`249-reports/evidence.tar.xz`, 1,748,096 bytes, sha256 `877ee1445d651338`, and
  `evidence.md`, `/panel` 3c) is packed in this synthesis's commit: 3,231 files read back byte for byte, no
  credential found by its scan, the folder `.claude/worktrees/p249-scratch/` the author's to remove.

## Author's verdict

Pending: the author's reading of R1 to R4, asked by the decision issue drafted beside this sitting and not
yet filed. A yes settles R1 and R2 as adopted (the census approved; no route adopted, 2522 open and
`blocking`), R3's order (R3's first item built before any candidate is adopted; question 3 and the blind
seat after the reading of route K) and R4's answer as the author gives it. It does not settle the blind
seat's ceiling until the author names it, nor any route, nor the predictions, on their own clocks.
