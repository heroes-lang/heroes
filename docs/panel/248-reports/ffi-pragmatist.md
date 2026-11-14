# Panel 248, the ffi-pragmatist's report

Written as the work goes, 2026-11-14. Folder: `.claude/worktrees/p248-scratch/248-ffi-pragmatist/`
(`F/` below): `F/tree/` the base, `F/c/` every probe, `F/logs/` every log, `F/linux/` the Linux arm64
script and its log.

## 0. The base, and where this brief is wrong

- `git rev-parse 'df7612ad3^{tree}'`: `f72f48da250e919a68ebc6a98be0facccc29a052`. The seed and stage 2
  built in `F/tree` exactly as 00-shared.md says (`F/logs/build.log`): `seed exit 0`, `stage2 exit 0`,
  `heroes 0.6.0`, `sources 4c9a6a9800095eef`, `ok   runtime      runtime`. On the base.
- `uptime` at 10:06: load averages *242.38 165.13 77.91*. No duration below is a measurement; counts
  are instructions retired (`/usr/bin/time -l`) or event counts.
- **The Windows box was switched on at 10:31** (the coordinator's message), so every Windows arm
  below is **run on the box**, clang 23.1.1 with its MSVC target, not compiled for MinGW and owed.
- **Where the briefs are wrong or incomplete, by what was run**: (1) route I-onstack's cost is not
  only *"it changes what a C handler's stack is"*: it makes a collector's suspend handler record a
  stack pointer outside its thread's stack (§ 1.9), the reason for my veto; (2) Darwin's ten
  installers all end in libsystem_kernel's `__sigaction` through binds that cross images, so **one
  interposed name sees all ten** (§ 1.2), which no route in 00-shared.md uses; (3) the two unrun
  Darwin rows of 00-shared.md are run: dyld **ignores** an executable's `__interpose` section, and a
  dylib itself linked flat **does** reach the executable's definition (§ 1.2); (4) **route I by name
  cannot link on Windows** (`duplicate symbol: signal` against UCRT, both linkers, both CRTs, § 1.7);
  (5) 247's critic's three shapes do not compile on Linux (glibc declares no `bsd_signal` or `sigset`
  under a Heroes program's macros, § 1.6); (6) I-mask, a route none of the briefs lists, is built
  and run (§ 1.10); (7) the compat `sigvec` of glibc is reachable by an object built against an
  older glibc **and** by a new static object linked to a definition of the runtime's: a definition
  makes an object link that fails today (reading, unrun).

## 1. Question 1, defect 2522: the C library's installers

### 1.1 R3's two cases at the base, Darwin arm64 (reproduced)

Built as 00-shared.md says, my compiler, the library a static archive `-O2 -fomit-frame-pointer`
(`F/h/`, `F/logs/b-*.log`, both builds exit 0). The frameless handler saves x29/x30 and never sets
x29 (`objdump -d lib-nofp/sigcb.o`: `stp x29, x30, [sp, #-0x10]!` then `blr x8`, no `mov x29, sp`),
which is why the walk climbs past it.

- `fp-flib-nofp`, 20 runs under `timeout -s KILL 20` (`F/logs/base-flib-nofp-darwin.txt`): **20 of 20
  exit 0, stdout `0 | answer ran for 30 | 0 | 60`, stderr empty**: the Heroes function ran inside the
  signal.
- `fp-fliblocale`, `python3 -I storm.py ./fp-fliblocale SIGUSR1 4 0.3` (then `0.5`), 7 storms at
  10:13:47 to 10:13:55 (`F/logs/base-fliblocale-darwin.txt`): one unarmed (`exit -30`, not counted),
  **6 of 6 armed `exit -9`** after 16,726 to 18,566 signals, stderr empty; `ips.py ... fp-fliblocale`:
  **6 reports, `('fp-fliblocale', 'SIGKILL', 'lock') 6`**, their pids 66844, 66847, 66850, 66852,
  66878, 66880, the storms' six; stack `_os_unfair_lock_recursive_abort < _os_unfair_lock_lock_slow <
  localeconv_l < h_fliblocale_answer < h_0cb_fliblocale_answer < sigcb_handler < localeconv_l <
  __vfprintf < ... < hero_int_to_str < h_fliblocale_round`.

### 1.2 How Darwin resolves the name: run (`F/c/dy/`)

| shape | the two-level dylib's `sigaction` reaches the definition | the executable's own call |
|---|---|---|
| `__interpose` section in the **executable** (`fp-ipexe`, and `fp-ipexe-rw` with a writable section) | **0**; `DYLD_PRINT_INTERPOSING=1` prints nothing for it | 0 |
| executable defining `sigaction`, dylib two-level (`fp-exedef-2lvl`) | **0** | (its own) |
| executable defining `sigaction`, dylib **itself linked `-flat_namespace`** (`fp-exedef-flat`) | **1** | |
| **I-dylib**: the hooks in a dylib of their own, `__interpose` section, linked as a dependent (`fp-ipdylib`, either link order) | **1** (`signal` 1 too) | 1 |
| I-dylib, a dylib `dlopen`ed after start (`fp-dlidylib`) | **1** | |
| I-dylib, executable signed ad-hoc with the hardened runtime (`codesign -s - -o runtime`) | dyld refuses the hooks' dylib: *"mapping process and mapped file (non-platform) have different Team IDs"*, exit 134 | |

So dyld honours an `__interpose` section in a dependent dylib and **ignores one in the executable**
(Darwin 27.2's dyld); a dylib built flat reaches the executable's definition, but no third party
builds its dylib flat; the hardened runtime's library validation refuses a dylib not signed by the
program's Team ID, as it refuses every non-platform dylib (a Homebrew one too).

**The manuals, read**: `man ld`'s *Two-level namespace* (its lines 66-73): *"By default all references
resolved to a dynamic library record the library to which they were resolved. At runtime, dyld uses
that information to directly resolve symbols"*, and of `-flat_namespace` (618-628): *"at runtime dyld
again searches all images and uses the first definition it finds"*, which is the flat row above.
**`man dyld` (224 lines) says nothing of interposing** (`grep -i interpos`: 0 lines); its
`DYLD_INSERT_LIBRARIES` (116-120) loads *"additional dynamic libraries ... before the ones specified
in the program"*. So the `__interpose` section rests on these runs and on the address sanitizer's
own dylib shipping it (00-shared.md: a `__DATA,__interpose` of `0x1300` bytes), not on a documented
promise: **a landing of I-dylib leans on an undocumented dyld mechanism Apple's own toolchain uses**,
and owes a test on each macOS the CI runs.

**Darwin's installers end in one system call through binds that cross images**, found by building
I-dylib: libc's `sigaction` is one branch to libsystem_platform's `__platform_sigaction` (`lldb
disassemble`: `libsystem_c.dylib\`sigaction: b 0x18806d6a0`), which rejects SIGKILL and SIGSTOP and
hands `_sigtramp` and the flags (`| 0x400`) to libsystem_kernel's `__sigaction`. My first I-dylib
interposed `__platform_sigaction` and called `sigaction` from it, and recursed to a stack fault
(exit 139; `lldb`: `hero_i_sigaction + 300` calling `hero_i_sigaction`): dyld interposes libSystem's
own cross-image binds. So **an I-dylib interposing the one name `__sigaction` sees every installer
libSystem has, from any image, with the C library's own code doing all of each installer's meaning.**

### 1.3 Route I-onstack, written in C and held to each C library (`F/c/onstack/`)

**The mechanism.** Every claimed POSIX thread already holds a 256 KiB alternate signal stack of the
runtime's (`runtime/parts/stack.c:994-1015`, `hero_stack_guard_enter`, called from `hero_args_set`
and `hero_spawn_enter`), and ASan's own under `--sanitize` (`stack.c:567-620`: the guard yields to
ASan, *"POSIX's ASan runs on an alternate stack of its own"*). So a definition of an installer need
not wrap the handler at all: it installs **exactly what the C library's own installs, with
`SA_ONSTACK` added where a function is installed**. A handler a library installs then runs on the
thread's alternate stack; a Heroes function it calls is refused by the thunk's **existing second
question** (`sigguard.c:330-334`, `SS_ONSTACK`, the kernel's answer), with or without frame records;
the delivery still calls the handler from the trampoline, so the exact test is untouched; no thread
state is touched inside a signal; a `siglongjmp` out of the handler is the kernel's to forget. No
change to the thunk.

**What each C library's installers do, read back from the kernel** (`read.c`, `fp-read`; Darwin
`F/logs/installers-read-darwin.txt`, Linux `F/linux/log-read.txt`):

| installer | Darwin 27.2 | glibc 2.41 |
|---|---|---|
| `signal` | `SA_RESTART` unless `siginterrupt` asked, empty mask | `SA_RESTART` unless `siginterrupt` asked, **the signal in its own mask** |
| `bsd_signal` | `SA_RESTART` **always**, the signal in its mask | as `signal` |
| `sigset` | as `signal`; `SIG_HOLD` blocks and answers the old handler, then `SIG_HOLD` | flags **0**, empty mask; the same `SIG_HOLD` answers |
| `ssignal` | not exported | as `signal` |
| `sysv_signal`, `__sysv_signal` | not exported | `SA_RESETHAND \| SA_NODEFER`, empty mask |
| `_signal_nobind` | as `signal` | not exported |
| `signal(SIGKILL)`, `signal(0)`, `signal(NSIG)` | `SIG_ERR` | `SIG_ERR` |

**The differential test** (`diff.c`, `diff2.c`): every definition of `inst.c` against the C library's
own (`dlsym(RTLD_NEXT, name)`, glibc's compat `sigvec` by `dlvsym(..., "GLIBC_2.17")`), over 12
signal numbers (8 valid, 0, `NSIG`, SIGKILL, SIGSTOP), `SIG_DFL`, `SIG_IGN` and a function, with and
without `siginterrupt`, four `SIG_HOLD` sequences, five `sigaction` flag sets and the query after each
install; equal means the same return value, errno, handler, flags (`SA_ONSTACK` aside), mask and
thread mask, and `SA_ONSTACK` present exactly where a function was installed.

- Darwin: `diff` **1,284 rows, 0 mismatches**, `SA_ONSTACK` added in the 224 rows that install a
  function; `diff2` (`sigvec`, `_sigvec_nobind`, the kernel's `__sigaction` with its trampoline
  field) **123 rows, 0 mismatches**. Linux arm64: `diff` **1,128 rows, 0 mismatches**, 224
  (`F/linux/log-q1.txt`). **A planted mistake** (`bsd_signal` without the signal in its mask,
  `inst_mut.c`) read **48 mismatches, exit 1** (`F/logs/diff-darwin-mutant.txt`): the test can fail.
- **What the documentation does not say and the test found**: Darwin's own `sigvec` leaves the
  caller's struct with `SV_INTERRUPT` flipped and never restores it (`caller struct after: real f
  0x2 ... ours f 0x0`, 60 mismatches until `inst.c` flipped it too). *Whole meaning* is a quirk list
  only a test against the C library can keep.

**R4's rows, Darwin** (`rows.c`: `sigguard.c`'s three questions copied, the claim as `stack.c` makes
it; a static archive and a dylib built `-O2 -fomit-frame-pointer`; `F/logs/rows-*-darwin.txt`):

| row | base | route I in the executable (`inst.c`) | **I-dylib, one name** (`libherok`) |
|---|---|---|---|
| static archive, frameless: `sigaction`, `signal`, `bsd_signal`, `sigset`, `sigvec`, `_sigaction_nobind`, `_signal_nobind`, `_sigvec_nobind`, `__platform_sigaction` (9 rows) | missed 9 of 9 | **refused 9 of 9** (on its alternate stack) | **refused 9 of 9** |
| two-level dylib, frameless: `sigaction`, `signal` | missed 2 | **missed 2** | **refused 2 of 2** |
| the program's own `signal`, `bsd_signal`, `sigaction`, the function the handler | refused 3 (exact test) | refused 3 (exact test) | refused 3 (exact test) |
| a correct callback, C calling it, no signal | not refused | not refused | not refused |
| `siglongjmp` out of a handler: `SS_ONSTACK` after; a correct callback; another delivery | 0; not refused; missed | **0; not refused; refused** | **0; not refused; refused** |
| a claimed worker, the library's handler (`pthread_kill`) | missed | refused | refused |
| a worker nothing claimed | refused (2492) | refused (2492) | refused (2492) |
| `sigaction(sig, NULL, &old)` after a library's install | its handler | its handler, `SA_ONSTACK` shown | its handler, `SA_ONSTACK` shown |

**I-dylib in its smallest form** (`ipose_kernel.c`, 30 lines): a dylib whose `__interpose` section
names **one** symbol, libsystem_kernel's `__sigaction`, adding `SA_ONSTACK` where a function is
installed. Every installer libSystem has reaches it through a bind that crosses images, so every
installer's meaning is the C library's own code: `read.c` linked with it differs from `read.c`
without it **only by `SA_ONSTACK` (bit 0x1) where a function is installed**, every return value,
mask, `SIG_HOLD` answer and `siginterrupt` state identical (`F/logs/installers-read-darwin*.txt`,
`diff`). No reimplementation, no list to keep, no differential test to maintain on Darwin.

### 1.4 The route, built and run on R3's two cases, Darwin arm64 (`F/h/`)

The runtime is **unchanged** on Darwin: its second question does the work. The program links the
one-name dylib (`k-*.hero`: R3's programs with one group `extern "herok.h" link "herok"` added and
`hero_k_present()` called once; the dylib's install name absolute, as Homebrew's are, because
`heroes build` gives a program no `LC_RPATH` and an `@rpath` name failed, *"no LC_RPATH's found"*,
exit 134).

- **`fp-k-flib-nofp`, 20 runs: 20 of 20 exit 134**, stdout `0`, stderr *"panic: kflib.answer was
  called from inside a signal handler: C called it while handling a signal ..."* with R9's remedy
  (`F/logs/route-flib-nofp-darwin.txt`). The base: 20 of 20 exit 0, the function inside the signal.
- **`fp-k-fliblocale-nofp`, 20 storms at 10:28:53 to 10:29:04: 20 armed, 20 `exit -6`, 20 refusal
  lines on stderr**; `ips.py`: `('fp-k-fliblocale-nofp', 'SIGABRT', 'abort') 20`, **the 20 reports'
  pids the 20 storms' pids (`join`), no SIGKILL**; stack `__pthread_kill < pthread_kill < raise <
  hero_signal_stop < hero_signal_refuse < hero_callback_entered < h_0cb_kfliblocale_answer <
  sigcb_handler < hero_release_block < ...`: refused at the thunk's first line, before `localeconv`.
  The base: 6 of 6 armed SIGKILL in the lock, in silence.
- **247's critic's shapes**: `direct` (a Heroes function handed to `bsd_signal`) base and route both
  exit 134, *"ran as a signal handler"* (the exact test still sees the delivery); `hold`
  (`sigset(SIG_HOLD)`) base and route both *"pending after the hold: 1"*, exit 0; `dlocale` storms:
  base 1 armed of 3 refused, route **3 of 3 armed refused** (`F/logs/critic-shapes-darwin.txt`).
- **Under `--sanitize`**: `DYLD_PRINT_INTERPOSING` shows ASan's dylib interposing `_signal` and
  `_sigaction` and mine `___sigaction`, side by side; `fp-s-flib` (base) 3 of 3 exit 0 inside the
  signal, **`fp-ks-flib` 3 of 3 exit 134 refused**; `fp-ks-fliblocale` **3 of 3 armed storms `exit
  -6`, 3 refusal lines, 3 SIGABRT reports** (on ASan's own alternate stack).
- **The C twin with its handler in a dylib** (`F/c/ctwin/`, no Heroes code): 3 of 3 armed storms
  `exit -9`, `('fp-ctwin-so', 'SIGKILL', 'lock') 3`. What the route turns into words is the Heroes
  function's re-entry; plain C dying in libc is C's own, as panel 247 found.

### 1.5 The alternatives, written and run (`F/c/alt/`, Darwin; Linux below)

- **B, asynchronous signals blocked on every claimed thread** (`b.c`, main blocking 13 asynchronous
  signals, an open thread made first; against its control): **`raise(SIGUSR1)` ran its handler 0 times
  before returning (control 1)**, so R3's `flib`, whose library raises from `main`, would print `0`
  and refuse nothing; **`alarm(1)` then `read()` was not interrupted: it returned only when my 4 s
  watchdog wrote** (control: `-1` after 1.01 s, `Interrupted system call`), so a C library's
  alarm-bounded read hangs; **SIGPROF while main computed 1 s: 0 on main, 7 on the open thread**
  (control 24 on main), so a sampling profiler samples the idle thread. B changes what C a program
  binds does; it does not close R3's `flib` case at all.
- **T, a bounded scan above the thunk for the trampoline's return address** (`t.c`): **a correct
  program is refused at `-O0` and `-O2`**: after ten signals handled and returned at `main`'s depth
  by a handler that calls a function (as R3's does, keeping its return into `_sigtramp` on the
  stack), a function with a 16 KiB buffer it never wrote calls back from C with no signal, and the
  scan finds the stale return address at word 1,903 (1,901 at `-O2`). And unbounded by the stack's
  top it read past it: exit 139. A false refusal is a correct program killed with a false message.
- **U, the unwinder** (`u.c`: `_Unwind_Backtrace` from the thunk, looking for a frame whose IP is the
  learned delivery address): **sees the frameless handler when the library has unwind tables, misses
  it built `-fno-asynchronous-unwind-tables -fno-unwind-tables`** (`objdump --unwind-info`: empty);
  **cost 134,700 instructions per correct callback** (`/usr/bin/time -l`: 200,000 callbacks
  26,968,999,599, 100,000 callbacks 13,495,318,678, against 15,196,474 for 100,000 that ask nothing;
  `F/logs/u-cost-darwin.txt`), about 9,000 times a callback asking nothing; **4 storms of 2.4 to 3.9
  million signals while `main` loaded and unloaded a dylib 20,000 times: all exit 0, no report**, so
  no unsafety of Darwin's unwinder was found by this probe.

### 1.6 Linux arm64: the base, and the route built into the runtime (`F/linux/log-q1.txt`, `log-q1r.txt`)

Two containers, `fp-248-3` (10:31:31 to 10:37:45) and `fp-248-4` (10:40:14 to 10:43:15), each `docker
exit 0`, `docker ps` after each empty; the base's seed and stage 2 exit 0, `sources
4c9a6a9800095eef`.

**The base, reproduced**: `fp-flib-nofp` **20 of 20 exit 0** (`0 | answer ran for 10 | 0 | 20`), its
shared form (`libsigcb.so`, `-O2 -fomit-frame-pointer`) **5 of 5 exit 0**; `fp-fliblocale-nofp` **5 of
5 armed storms exit 0, `built 20000000 words`, after 798,450 to 1,395,072 signals**, its shared form
**3 of 3**: the Heroes function ran inside the signal every time. **247's critic's three shapes do not
compile on Linux at the base**: glibc's `signal.h`, as a Heroes program includes it, declares no
`bsd_signal` (`error[ffi_unknown_name]`, *"fix (guess): the header declares `ssignal`"*) and no `sigset`
(`hold.h`: *"call to undeclared function 'sigset'"*), the importc check doing its job; their Linux
variants use `signal` and a `holdl.h` that declares `sigset` and `SIG_HOLD` itself.

**The route on Linux** is the definitions by name, `runtime/parts/siginstall.c` in my tree (170
lines, glibc's eight linkable handler takers, the compat `sigvec`, and `siginterrupt` mirrored), the
probes of `sigguard.c` switched to glibc's own `sigaction` found by `dlsym(RTLD_NEXT)`. It compiles
for `aarch64-linux-gnu.2.41` under the runtime's own `-Werror=sign-conversion` and the rest of
`flags.hero`'s list (`zig cc -target aarch64-linux-gnu.2.41 -c ... runtime/runtime.c`, exit 0) after
one repair the container found (glibc's `SA_RESETHAND` is unsigned; first build `internal error: the
runtime did not compile`). Built with the base's own compiler, the runtime recompiled:

- `nm fr-flib-nofp`: the executable defines `__sigaction __sysv_signal bsd_signal sigaction
  siginterrupt signal sigset sigvec ssignal sysv_signal` (T); `nm -D fr-flib-so` exports `sigaction`,
  `signal`, `__sigaction` dynamically, so the program's shared library reaches them.
- **`fr-flib-nofp` 20 of 20 exit 134**, *"panic: flib.answer was called from inside a signal
  handler ..."*; **its shared form 20 of 20**; **`--sanitize` 5 of 5**.
- **`fr-fliblocale-nofp`: 20 storms, 20 armed, 20 `exit -6`, 20 refusal lines**; shared form **5 of 5
  armed refused**; `--sanitize` **3 of 3 armed refused**.
- The critic's shapes: `direct-l` exit 134 (*"ran as a signal handler"*, the exact test), `hold-l`
  *"pending after the hold: 1"* exit 0, `dlocale-l` 3 of 3 armed refused.
- C rows (`rows.c`): route **refuses all 8 static rows** (`sigaction signal bsd_signal sigset ssignal
  sysv_signal __sysv_signal __sigaction`) **and both shared-library rows**, `siglongjmp` then a
  correct callback not refused and the next delivery refused, a claimed worker refused; the base
  misses all ten. **Under ASan the same**; `nm`: my strong `sigaction` (T) stands beside ASan's weak
  `__interceptor_sigaction` (W), so the link takes mine and **ASan's interception of `sigaction` is
  bypassed** (its own handler install goes by `___interceptor_sigaction`'s internals): a residue to
  state, not a failure measured.
- **A library loaded by `dlopen` after start reaches the definitions** (`dlrow.c`: *"handler on the
  alternate stack (reached the definition)"* for `sigaction` and `signal`); the executable exports
  them in `.dynsym` without `-rdynamic` (`nm -D`), and `-Wl,--export-dynamic-symbol=` changes nothing.
- `diff` 1,128 rows 0 mismatches; **`diff2` on glibc's compat `sigvec`: 30 mismatches of 60 rows**,
  the flags and the first mask word equal, the cause not found in this sitting: **the definition of
  glibc's compat `sigvec` is not shown equal**; it is reachable only from an object built against an
  older glibc, and a landing either repairs it under `diff2` or leaves `sigvec` undefined on Linux
  (then an old object's `sigvec` links to glibc's compat symbol and is the route's residue).

**The alternatives on Linux**: **B** the same as Darwin (`raise` ran the handler 0 times, `alarm`'s
read returned only at the 4 s watchdog, SIGPROF 0 on main and 498 on the open thread, against 1,
EINTR at 1.01 s and 499); **T** refused a correct callback at `-O0` and `-O2`, the stale word at 1,455,
**even before any signal but the runtime's own probe** had been handled; **U** sees the frameless
handler with unwind tables and misses it without; `objdump` of glibc's `dl_iterate_phdr`, which
libgcc's unwinder calls from `_Unwind_Find_FDE`, shows `bl __pthread_mutex_lock` (`13f750`) and
`__pthread_mutex_unlock`, a call POSIX does not list as async-signal-safe; its cost on Linux is
**unrun** (`perf: command not found` in the image) and its storm sent 0 signals (the loop ended before
the arm delay), so **unrun**; **W** (`-Wl,--wrap=sigaction`): *"the runtime object's own call wrapped:
1"*, *"the static archive's call wrapped: 1"*; **R** (`objcopy --redefine-sym sigaction=hero_r_sigaction`
on the static archive): the renamed `sigaction` row refused, the archive's `__sigaction` and the
shared library missed: R reaches only the references it renamed in static archives, and Xcode ships
no `objcopy` (00-shared.md).

### 1.7 Windows, run on the box (on from 10:31, `/c/w/p248-fp/`; clang 23.1.1, target `x86_64-pc-windows-msvc`)

- **An executable cannot define `signal` beside UCRT** (`F/win/q1exe.c`): lld-link, static CRT,
  *"duplicate symbol: signal ... defined at libucrt.lib(signal.obj)"*; lld-link, `-fms-runtime-lib=dll`,
  the same *"at ucrt.lib(api-ms-win-crt-runtime-l1-1-0.dll)"*; MSVC's `link.exe` (`-fuse-ld=link`), both
  CRTs, *"LNK1169: one or more multiply defined symbols found"*. So route I by name has **no Windows
  form** with either linker the project uses, and a DLL's `signal` binds to `ucrtbase.dll` by name
  anyway (unrun, since nothing links).
- **What UCRT exports that installs a handler** (`llvm-readobj --coff-exports
  C:/Windows/System32/ucrtbase.dll`, 2,484 names, filtered for `sig|raise|xcpt|abort|invalid|ctrl`):
  `signal` and `raise`; besides them `_set_invalid_parameter_handler` and `_set_abort_behavior` (and
  their `_o_` forms) and `__pxcptinfoptrs`, CRT callbacks called on the calling thread, not signals.
  The Win32 installers (`AddVectoredExceptionHandler`, `SetUnhandledExceptionFilter`,
  `SetConsoleCtrlHandler`) are kernel32's, called by the runtime itself (00-shared.md), and none is
  a route's concern: a console handler runs on a thread Windows makes (below), a fault's on the
  faulting thread.
- **Where a C library's handler runs** (`F/win/q1thr.c`, run detached so it has a console):
  *"raise(SIGTERM): handler ran on main's thread, synchronously"*;
  *"GenerateConsoleCtrlEvent(CTRL_C_EVENT) sent: SIGINT handler ran on a thread Windows made, not
  main"* (under ssh, with no console, *"not delivered"*). So Windows has no asynchronous delivery
  onto a claimed thread for a console signal, and 2492's guard already refuses a Heroes function on
  the thread Windows makes; a fault's handler runs on the faulting thread, where D stands (R10).
  **2522 needs nothing new on Windows**, and none of route I's machinery would build there.

### 1.8 What it breaks, and panel 239's member

- **The runtime's own installs**: on Linux every one of its 27 `sigaction`/`signal` lines now passes
  through the definitions, which add `SA_ONSTACK` to its handlers (the stack guard's already asks
  it, `stack.c:1052`; the stop's handler `hero_net.c:584-599` and the run's would run on the
  alternate stack, C only), and its `signal(SIGABRT, SIG_DFL)` inside the refusal (`sigguard.c:139`)
  and panic (`panic.c:54`) paths reaches `hero_install_bsd`, which calls glibc's `sigaction` found
  before `main`: async-signal-safe, no allocation, no lock. The probes go to glibc's own. Its 16 mask
  calls are untouched (no mask is interposed). On Darwin the runtime is untouched.
- **A library of the program that defines `sigaction`** (a crash reporter's shape): the runtime
  **calls** `sigaction` and `signal` today, so panel 239's member (`c-boundary.md:186-220`, *"a C
  name ... the runtime calls or defines"*) already tells such a library on its group at the base; for
  those two names nothing widens. **It widens** for the names the runtime would newly define and
  does not call: on Linux `bsd_signal`, `sigset`, `ssignal`, `sysv_signal`, `__sysv_signal`,
  `__sigaction`, `siginterrupt`, `sigvec`; a library defining one of those is refused where it
  linked yesterday. **That is a widened diagnostic, in those words** (00-shared.md § The lane), and
  R4's *"that rule amended in the same commit"*. On Darwin the I-dylib defines no name of the C
  library at all (it interposes by tuple), so `ffi_defined_elsewhere` is not widened there; what a
  library of the program that itself carries an `__interpose` section for `__sigaction` would meet
  is dyld's own order (both tuples apply), unrun.
- **The address sanitizer**: measured above on both legs: it links and refuses; on Linux my strong
  `sigaction` takes the place of ASan's weak interceptor (ASan's own install goes by its internal
  path), on Darwin the two interposers stand side by side.
- **Static linking**: a fully static executable is no mode of `heroes build` (00-shared.md's search);
  there `dlsym(RTLD_NEXT)` answers nothing and the definitions would need the C library's private
  name, a question for the day such a mode exists.

### 1.9 What forcing `SA_ONSTACK` breaks: a collector's suspend handler (found after 1.3 to 1.8)

Route I-onstack changes one thing about a library's handler: the stack it runs on. **A handler that
takes its own frame's address as the interrupted thread's stack pointer then records the alternate
stack instead.** That is not a contrived shape:

- **bdwgc** (the Boehm collector), its source fetched (`F/pages/bdwgc-pthread_stop_world.c`, master
  `a2cb16122afd` of 2026-11-10; and the 8.2.8 release tarball, `F/linux/dl/gc-8.2.8.tar.gz`, sha256
  `7649020621cb2632...`): the suspend handler stores **`GC_approx_sp()`**, its own frame, as the
  thread's stack pointer (`GC_store_stack_ptr`, master `:337`, 8.2.8 `:323`); `GC_push_all_stacks`
  scans from it to the thread's `stack_end` (`:873`, `:910`); a thread that registered its alternate
  stack with `GC_register_altstack` has **only that alternate stack** scanned, *"FIXME: Need to scan
  the normal stack too, but how?"* (`:923-930`; 8.2.8 `:870`); and it installs the handler with
  **`SA_RESTART` and `SA_SIGINFO`, no `SA_ONSTACK`** (`:1501-1513`; 8.2.8 `:1408`). Under route
  I-onstack on Linux my definitions would add `SA_ONSTACK` to it.
- **The shape, run** (`F/c/onstack/gcshape.c`: the thread claimed with stack.c's 256 KiB alternate
  stack, a "collector library" installing its suspend handler with `sigaction`, a collector thread
  suspending it and computing the range it would scan): **base**: *"installed with SA_ONSTACK: 0 ...
  the handler's own frame lies inside the thread's stack; the range ... covers the thread's stack"*;
  **route I-onstack (definitions, Darwin)**: *"installed with SA_ONSTACK: 1 ... the handler's own frame
  lies OUTSIDE the thread's stack; the range it would scan, [0x1006abb6f, 0x16f7b8000), reaches MORE
  THAN A MiB outside the thread's stack"*; **I-dylib one-name (Darwin)**: the same, `[0x104743b6f,
  0x16b730000)`. The `ucontext`'s stack pointer stays inside the thread's stack in every row, so a
  handler that reads it is unaffected; one that reads its own frame scans another mapping or faults.

So route I-onstack would make the runtime corrupt or crash a correct C library a binding links, a
collector, which design.md §1.12 (*"a Heroes program must not segfault and must not corrupt memory"*)
forbids and which is this seat's veto (a change to how a C library's own code behaves at the
boundary). The real bdwgc under I-onstack is run in Linux below.

### 1.10 Route I-mask: the kernel holds the marker, the handler keeps its stack (built, run)

The definitions (Darwin: the I-dylib's tuples; Linux: by name) **add a marker signal (here SIGURG)
to the mask of every handler a function is installed with**, and change nothing else, so the
kernel blocks the marker exactly while such a handler runs, on the stack the handler asked for, and
gives the mask back at the handler's return or at a `siglongjmp` (which restores the mask saved by
`sigsetjmp`). The mask calls (`sigprocmask`, and `pthread_sigmask`, on Darwin its system call
`__pthread_sigmask`, which `pthread_sigmask` in libsystem_pthread reaches across images) **let the
marker go from blocked to unblocked and never from unblocked to blocked**, so a program blocking
every signal outside a handler is never taken for one. The thunk asks one more question after the
claimed-thread check: is the marker blocked on this thread (`pthread_sigmask(SIG_BLOCK, NULL,
&cur)`)? Darwin's arm interposes three names of libsystem_kernel (`ipose_mask.c`, 40 lines), Linux's
defines glibc's eight installers, the compat `sigvec`, `siginterrupt`, `sigprocmask` and
`pthread_sigmask` (`F/linux/c/route/siginstall_mask.c`, compiled for `aarch64-linux-gnu.2.41` under
the runtime's `-Werror` flags, zig, exit 0).

**Darwin, run** (`F/logs/rows-mask-darwin.txt`): all 9 frameless static rows and **both two-level
dylib rows refused** (*"the marker is blocked"*); the program's own handlers by the exact test;
after `siglongjmp` a correct callback **not refused** and the next delivery refused; a claimed worker
refused; **every signal blocked by the program (`pthread_sigmask(SIG_SETMASK, all)` and
`sigprocmask(SIG_BLOCK, all)`), then a correct callback: not refused**; the query answers the
library's handler, its flags untouched. **The collector shape: the handler's own frame inside the
thread's stack, its range covering it** (*"installed with SA_ONSTACK: 0"*).

**R3's two cases under I-mask, Darwin** (the runtime's thunk given the mask question for the build
and then restored to the base, `cmp` against `df7612ad3`; the programs linked with the I-mask dylib,
`F/h/libm/`): **`fp-m-flib` 20 of 20 exit 134** with R1's words; **`fp-m-fliblocale` 20 storms, 20
armed, 20 `exit -6`, 20 refusal lines, 20 SIGABRT reports whose pids are the storms' (`join`)**, stack
`... hero_signal_refuse < hero_callback_entered < h_0cb_kfliblocale_answer < sigcb_handler <
hero_array_push_owned < ...`; `direct` refused by the exact test; `hold` *"pending after the hold:
1"*; `--sanitize` `flib` 3 of 3 refused.

**Its cost, one mask query a callback** (`F/c/alt/maskcost.c`, Darwin, `/usr/bin/time -l`): 2,000,000
queries 2,702,454,238 instructions, 1,000,000 1,356,354,428, so **1,346 instructions a query**; on
247's `qsortcost` ladder (121,205 comparator calls) that is about 163 million instructions over D's
1.51 billion, under prediction (2)'s 2,032,000,000: arithmetic on two measurements, unrun on the
ladder itself.

**Linux arm64, I-mask in C** (`F/linux/log-mask.txt`, container `fp-248-6`): the definitions held to
glibc by `diff.c` with the marker aside: **1,128 rows, 0 mismatches**, the marker present in the 240
rows that install a function; the rows: **all 8 static rows and both shared-library rows refused**,
`siglongjmp` then a correct callback not refused and the next delivery refused, a worker refused, a
program blocking every signal not taken for a handler; one mask query about **0.14 µs of process
CPU** (2,000,000 queries: 0.078 s user, 0.203 s sys; `perf` absent). **The collector shape on
Linux**: base and I-mask inside the thread's stack, I-onstack outside it (`[0xffffba73ed9f,
0xffffff998000)`). **The compat `sigvec` is equal after all**: `diff2`'s 30 "mismatches" were my
test's `memcmp` over glibc's 128-byte `sigset_t`, whose words past the kernel's 64 bits glibc leaves
as they were; compared by `sigismember` over 1 to 63, **60 rows, 0 mismatches** (`log-mask.txt`), and
Darwin's 123 rows still 0.

**A false refusal found, and repaired in the route**: on glibc **a plain `longjmp` out of a handler
gives no mask back** (*"after plain longjmp out of a handler: SIGUSR1 still blocked 1"*), so the
marker stayed blocked and **the next correct callback was refused** (*"refused (the marker is
blocked)"*); on Darwin the same row reads *"still blocked 0"* and is not refused (BSD's `longjmp`
gives the mask back). So the Linux arm also defines `longjmp`, `_longjmp` and `__longjmp_chk` (what
`_FORTIFY_SOURCE` makes of `longjmp`), each letting the marker go before the C library's own jump;
a jump that stays inside a handler then loses the marker, the safe direction (the walk and D stand
under it). Compiled for `aarch64-linux-gnu.2.41` under the runtime's flags (zig, exit 0); its run is
§ 1.12.

### 1.11 The golden `run` cases under a route, Darwin (`F/logs/net-darwin-idylib*.txt`)

Run with the base compiler and `DYLD_INSERT_LIBRARIES` of the one-name I-onstack dylib and a canary
dylib that writes one line per process it loads into (**390 processes** wrote one), the copy given a
`.git` file naming no repository so the provenance hook judges the copy, not the trunk:
`signal` **exit 0**, `callback` **14 passed, 0 failed**; `2494` 13 passed, 2 failed; `stop` 17 and 2;
`handler` 10 and 2: **the same two cases each time**,
`fixedbugs-2494-a-handler-that-allocates-inside-the-allocator-stops-its-program-by-name` and
`...-lends-inside-the-lend-tracking-...`, at `-O0`, which must abort with D's words and now abort
earlier, at the thunk's first line, with R1's (*"... on_signal was called from inside a signal
handler"*). They exist to test D with a handler the walk cannot climb; under any route I a handler
installed through the C library is refused before D is reached, so **a landing re-blesses their
words and gives D a case of its own that no definition reaches** (a handler installed by a raw
system call on Linux). Under I-mask the same two would move the same way (unrun on Darwin's net).

### 1.12 The real collector, and I-mask built into the Linux runtime (`F/linux/log-gc.txt`, `log-mask.txt`)

**bdwgc 8.2.8**, built from its release tarball's all-in-one `extra/gc.c` (`-DGC_THREADS
-DGC_BUILTIN_ATOMIC`: the image has no libatomic_ops; the first attempt failed on *"'atomic_ops.h'
file not found"*), linked statically into `gcreal.c`: a list of 1,000 nodes rooted only on `main`'s
stack, `main` holding a 256 KiB alternate stack as `stack.c`'s claim gives it, another thread forcing
collections, `main` checking the list 300 times; five runs each under `timeout -s KILL 60`:

| build | the collector's suspend handler (SIGPWR) | result, 5 runs |
|---|---|---|
| base | `SA_ONSTACK: 0` | **5 of 5 exit 0, *"the list intact after 301 collections"*** |
| route I-onstack (`inst.c`) | `SA_ONSTACK: 1` | **5 of 5 `Segmentation fault`, exit 139** |
| route I-mask (`inst_mask.c`) | `SA_ONSTACK: 0` | **5 of 5 exit 0, intact after 301 collections** |

**This is the program my veto names**: a Heroes program on Linux that links bdwgc and whose main
thread the runtime has claimed dies by a memory fault under I-onstack, where it ran at the base.

**The `longjmp` repair, run**: with `longjmp`, `_longjmp` and `__longjmp_chk` defined, *"after plain
longjmp out of a handler: SIGUSR1 still blocked 1"* (glibc's own behaviour, untouched) and *"then a
correct callback, no signal: not refused"*; the same with the library built `-D_FORTIFY_SOURCE=2`,
whose object calls `__longjmp_chk` (`nm`: `U __longjmp_chk`); every other row as before.

**I-mask built into the Linux runtime** (the part `siginstall_mask.c` and the thunk's question in
`sigguard_mask.c`, `F/linux/c/route/`, applied to the base and the cases rebuilt with the base's
compiler): **`fm-flib-nofp` 20 of 20 exit 134**, its shared form **20 of 20**, `--sanitize` **3 of
3**; **`fm-fliblocale-nofp` 20 storms, 20 armed, 20 `exit -6`, 20 refusal lines**; sanitized **3 of 3
armed refused**; `direct-l` refused by the exact test, `hold-l` *"pending after the hold: 1"*.
**The golden `run` cases under it**, the `run` form narrowed, in the container's `git archive` copy
(which has no `.git`; the `run` form needs none): `2494` 13 passed 2 failed, `signal` 10 and 0, `stop`
17 and 2, `handler` 10 and 2, `callback` 14 and 0: **the same two D cases every time** (§ 1.11), and
no other case moved. (The `longjmp` definitions were added to the part after this run; the rows
above ran them in C.)

## 2. Question 2, defect 2502: the waits

### 2.1 Each wake, written and run (`F/c/q2/wake.c`; Darwin by `/usr/bin/time -l`, `F/logs/q2-wake-darwin.txt`)

100 threads each waiting on a quiet connected socket of its own, then each closed, the close's
latency kept from the closer's mark to the waiter's return:

| shape | instructions, 100 waiting 1 s / 5 s / 10 s | a close's latency, median (max) |
|---|---|---|
| slices of 50 ms (the base's shape) | 123,212,576 / 227,234,879 / 397,831,358 | 24,401 µs (53,204) |
| a pipe per waiting thread | — / 25,053,489 / — | 18 µs (80) |
| a kqueue `EVFILT_USER` per waiting thread | 25,649,237 / 25,545,557 / 25,268,694 | 5 µs (25) |
| one poller thread for the process (kqueue), waiters on condition variables | 48,959,302 / 82,345,853 / 52,524,281 | 24 µs (134) |
| a thread-directed signal (`pthread_kill`) | — / 25,629,745 / — | 26 µs (79) |

The slices grow about 30 million instructions a second for 100 waiters; every wake stays flat.
**The thread-directed signal has no correct Darwin arm**: closing its lost-wakeup window (the signal
landing between the waiter's check and its `poll`) needs `ppoll`, which **Darwin's SDK does not
declare** (`grep -rln ppoll $SDK/usr/include`: 0 files); and the race it leaves is a C library's
non-restartable call on that thread answering `EINTR` (my probe waits for a `waiting` flag set
before the loop, so it does not reproduce the window).

### 2.2 Server-shaped, under a soft limit of 256 (`srv.c`, Darwin, `F/logs/q2-srv-darwin.txt`)

A server under `setrlimit(RLIMIT_NOFILE)` soft 256 (as `ulimit -n 256`) accepts TCP connections from a
peer process holding up to 300, one waiting thread each, with the stop's pipe made as
`hero_net_catch_stop` makes it:

| shape | descriptors at N = 100 (own count; `lsof -p` lines) | the largest N held | the first call that failed |
|---|---|---|---|
| base (slices) | 106; 110 | **250** | `accept`, *Too many open files* |
| a pipe per waiting thread | (never reached 100) | **83** | `pipe`, *Too many open files* |
| a kqueue per waiting thread | 206; 210 | **125** | `accept` |
| one shared pipe | 108; 112 | 248 | `accept` |
| one poller (kqueue) | 107; 111 | 249 | `accept` |

The critic's arithmetic (N+6, 3N+6, 2N+6, N+8, N+7), now run. A descriptor per waiter is taken from
the same budget every C library of the program spends (SQLite's files, curl's sockets): a pipe per
waiter leaves a third of the room.

**Linux arm64** (`F/linux/log-q25.txt`, container `fp-248-5`; no `perf` in the image, so process CPU
by `getrusage`): slices **0.098 / 0.143 / 0.252 s** at 1 / 5 / 10 s (35 involuntary switches at 5 s),
a close at **51,360 µs** median; a pipe per thread 0.013 s, 25 µs; an `eventfd` per thread 0.011 s
(0.014 at 1 s, 0.010 at 10 s), 25 µs; one poller (`epoll`) 0.016 s, 24 µs; a thread signal (with
`ppoll`) 0.012 s, 26 µs. **Server-shaped under 256: the same five rows as Darwin** (base 250, pipe per
thread 83, `eventfd` per thread 125, shared 248, poller 249; descriptors at 100: 106, —, 206, 108,
107).

### 2.3 Windows, run on the box (`F/win/q2win.c`)

- `WSAPoll` handed an event's handle: **-1, `WSAENOTSOCK` (10038)**; Microsoft's `WSAPoll` page holds
  no sentence naming it (`F/pages/wsapoll.html`, searched), the box's answer is the measurement.
- `MAXIMUM_WAIT_OBJECTS` **64**; `WaitForMultipleObjects` on 65 handles: **`WAIT_FAILED`, error 87**;
  Microsoft: *"The maximum number of object handles is MAXIMUM_WAIT_OBJECTS"*, and to wait on more,
  *"Create a thread to wait on MAXIMUM_WAIT_OBJECTS handles, then wait on that thread plus the other
  handles"* (`F/pages/wfmo.html`). `WSAEventSelect`: *"automatically sets socket s to nonblocking
  mode"* and *"cancels any previous WSAAsyncSelect or WSAEventSelect for the same socket"*
  (`wses.html`), so two threads waiting on one socket (a `recv` and a `send`) share one selection.
- 100 waiting 5 s (`QueryProcessCycleTime`, `GetProcessHandleCount`): **slices 395,168,250 cycles**,
  5.0 handles a waiter, a close at 31,852 µs median (64,692 max); **a loopback socket pair as the wake
  934,390 cycles**, 7.0 handles a waiter, 19 µs (574); **`WSAEventSelect` with a wake event
  219,760 cycles**, 5.0 handles a waiter by the count (two events made a waiter; the count did not
  move, unexplained), 4 µs (16).

- **One poller for the process on Windows** (`F/win/q2poll.c`: one thread on `WSAPoll` over every
  waiting socket and a loopback socket pair's reading end as its wake, each waiter on a condition
  variable): 100 waiting 5 s **1,732,250 cycles**, handles 86 to 391 (about 3 a waiter: both ends of
  each probe connection and its thread; the wake pair's 2 once), a close at **27 µs** median (1,652
  max). `WSAPoll` took 101 descriptors in one call here, past the 64 handles
  `WaitForMultipleObjects` takes; its own bound, if any, is unread and unrun.

### 2.4 The stop's pipe beside it

Read (`hero_net.c:549-669`): written once by the first signal, never read, both ends close-on-exec.
With one poller, the stop's pipe is one more descriptor that poller watches, and a waiter holds
none; with a wake per waiter, every wait watches it beside its own wake, as today. A `fork` between
`hero_net_catch_stop` and `exec`: the pipe's ends are inherited until `exec` closes them
(`O_CLOEXEC`, 620-631), the handler ends a forked child by the signal's default
(`hero_net_stop_owner`, 553-556); a kqueue **is not inherited** by a `fork`ed child at all (Darwin's
`man 2 kqueue`, line 74: *"The queue is not inherited by a child created with fork(2)."*), and an
`epoll` descriptor made with `EPOLL_CLOEXEC` ends at `exec`; the poller thread does not exist in the
child, which runs no Heroes code before `exec`. Nothing a child does before `exec` reaches a waiter.

## 3. Question 3, defect 2490: a child's working folder

### 3.1 What the child already calls, against POSIX's list (`run.c:1333-1477`, 814-826, 1141-1160, 1213-1240)

Every call `grep -c -x`'d against the 231 names of `briefs/fix/web/async-safe.txt`: **on it** `write
_exit open dup2 close setpgid raise getppid sigemptyset sigaction sigprocmask memset getpid
tcsetpgrp`; **off it** `getrlimit`, `setrlimit`, `execvp` / `execvpe` / `execvP`, `getenv` (named by
the comment at 1453 for glibc's `execvpe` search), and Linux's `prctl`. Read on Darwin by disassembly
(`lldb disassemble -n`): `getrlimit` and `setrlimit` are bare system calls (`svc #0x80`, then
`cerror_nocancel`), no lock, no allocation; `execvp` calls `getenv` and `execvPe`, whose callees go
through stubs I did not resolve, so for `execvp` the evidence is the storm below, a sample and not a
proof. glibc's are read in the Linux log below.

### 3.2 Run, Darwin (`F/c/q3/q3.c`, `F/logs/q3-stress-darwin.txt`)

A parent with four threads allocating, formatting and writing forks, the child does the step and
`execve`s `/usr/bin/true` (`execvp` for that shape), each child bounded to 10 s, at most 10,000 forks
under `timeout -s KILL 120` (each run reached its bound on this loaded Mac, so the count is how far
it got): **`chdir` 7,500 forks: hung 0, failed 0; `fchdir` (a folder the parent opened `O_DIRECTORY
| O_CLOEXEC`) 8,000: 0, 0; `execvp` after `chdir` 7,500: 0, 0; `getrlimit` and `setrlimit` then
`execve` 7,500: 0, 0**; under ASan `chdir` 3,000: 0, 0; `fchdir` 5,500: 0, 0; `execvp` 4,500: 0, 0; `rlimit` 5,500: 0, 0.
No hang in 49,000 children of a threaded parent; a sample, which proves no absence.

**The three errors** (`q3 errors`), the child's `chdir` errno down the report pipe and the parent's
`open(O_DIRECTORY)`, the same each time: a missing folder **`ENOENT`**, an unreadable one (`chmod
000`, a user that is not root) **`EACCES`**, a file in its place **`ENOTDIR`**. So the `fchdir` shape
tells the program the same three numbers **before any child exists**.

**The order, measured** (`q3 order`): after the child's `chdir("sub")`, **`execvp("./x")` runs
`sub/x`** (*"child's x ran in sub"*); a stream path opened **before** the `chdir` lands in the parent's
folder, **after** it in `sub/`.

**Linux arm64** (`log-q25.txt`): the same order rows; the errors `ENOENT`, `ENOTDIR`, and `EACCES` run
as the user `nobody` (`setpriv --reuid=65534`; as root the `chmod 000` folder opens, *"ok"*); **10,000
forks each from the threaded parent, under the 120 s bound: `chdir` hung 0 failed 0, `fchdir` 0 0,
`execvp` 0 0, `rlimit` 0 0**; under ASan, each stopped by its bound (`log-mask.txt`): `chdir` 7,000,
`fchdir` 8,000, `execvp` 8,500, `rlimit` 8,000, hung 0 and failed 0 each (the first ASan pass's counts
were lost to my log keeping only the kill line, `log-q25.txt`). **glibc read by `objdump`**: `execvp`
calls `getenv`, `__libc_alloca_cutoff`, `strchr`, `strchrnul`, `strnlen`, `__memcpy_chk`,
`__mempcpy_chk` and `execve`, **no allocator** (its path buffer is on the stack); `setrlimit` is one
`svc #0x0`; `getrlimit`'s body holds neither a call nor a system call of its own (a branch this
reading did not follow). glibc 2.41 declares **only** `posix_spawn_file_actions_addchdir_np` and
`addfchdir_np` (`/usr/include/spawn.h:206`, `:214`), not POSIX 2024's names.

**`posix_spawn`'s form**: Darwin's SDK declares POSIX 2024's `posix_spawn_file_actions_addchdir` and
`addfchdir` *"__API_AVAILABLE(macos(26.0))"* (`spawn.h:72-75`) and marks the `_np` forms deprecated,
*"posix_spawn_file_actions_addchdir(3) has replaced posix_spawn_file_actions_addchdir_np(3)"*
(`:184-188`); glibc's below.

### 3.3 Windows, run on the box (`F/win/q3win.c`, launched detached into its own console)

- `lpCurrentDirectory` **relative** (`"sub"`): started; the child's folder `C:\w\p248-fp\q3\sub`,
  resolved against the parent's.
- **missing** (`"missing-2490"`) and **a file** (`"afile"`): **`CreateProcessW` FAILED, `GetLastError`
  267 for both** (`ERROR_DIRECTORY`), where POSIX tells `ENOENT` from `ENOTDIR`.
- **A program named with no folder** (`"x.exe child"`, an `x.exe` in the parent's folder and one in
  `sub\`, `lpCurrentDirectory` `"sub"`): **the parent's `x.exe` ran** (*"child
  C:\w\p248-fp\q3\x.exe: cwd C:\w\p248-fp\q3\sub"*), where POSIX's `execvp("./x")` after `chdir`
  runs the child folder's: the two systems disagree, so a door that leaves the lookup to each gives
  two meanings.
- **The `=X:` entries**: the process held one (`=C:=C:\w\p248-fp\q3`); a block built as
  `hero_run_win_environment` builds it (every entry of `GetEnvironmentStringsW`, then a staged one)
  **keeps `=C:`**, and the child read it; with the current drive's folder moved by
  `lpCurrentDirectory`, `C:x` resolved in the child's folder.
- **clang on Windows splits `C_INCLUDE_PATH` at `;`**, run: `C_INCLUDE_PATH=:` gave one folder named
  `:` (*"ignoring nonexistent directory ":""*), `/nonexistent-2344:` one folder of that name, and
  only `C:/nonexistent-2344;` gave an empty element, which clang made `.`. **So the two
  `fixedbugs-2344` folder cases cannot pass on Windows as written, with the door or without**; the
  fetch case's `cd` lines are the one the door would let run there.

## 4. Question 4: a stop the program asks itself

### 4.1 The door, written against the base's own layer (`F/c/q4/`)

`door.c` includes the base's `hero_net.c` (a copy, `hero_net_door.c`, whose one change is that
`hero_net_stopping` also reads the door's own flag under `-DDOOR_APART`) and adds
`int64_t hero_net_ask_stop(void)`: from any thread, no signal, the counter, and where the stop is
armed one byte down its pipe. Two designs: **shared**, the door adds to the signals' count
(`hero_net_stop_asks`), and **apart**, the door sets a flag of its own that every wait reads and the
signal handler does not count. The probe (`q4.c`, the layer's three runtime calls stubbed as lane
b35's `waiters.c` stubs them): `main` waits in `hero_net_accept`, four workers in `hero_net_recv` on
quiet connections, a fifth asks the stop 300 ms in.

**Darwin** (`F/logs/q4-door-darwin.txt`): both designs, **every wait answered `stopped`**, the
slowest **11.4 ms** (shared) and **6.0 ms** (apart) after the ask unarmed, where the slices hear it,
and **0.0 ms** armed, where the pipe's byte wakes every `poll`; `hero_net_stop_asked()` 1.
**ThreadSanitizer** (`-fsanitize=thread`, `timeout -s KILL 120`, `F/logs/q4-tsan-*-darwin.txt`):
**0 warnings in all four runs** (both designs, armed and unarmed), the slowest wait 42.3, 0.1, 3.0
and 0.1 ms.

**Linux arm64** (`F/linux/log-q25.txt`): both designs, every wait `stopped`: shared **38.7 ms**
unarmed and **0.2 ms** armed, apart **16.6** and **0.4**; **ThreadSanitizer 0 warnings in all four
runs** (33.2, 0.5, 32.1, 0.3 ms).

**Windows, on the box** (`F/win/q4win.c` against the base's Windows layer): both designs, armed and
not, every wait `stopped`, **31 to 63 ms** after the ask (`GetTickCount64`): the slices, Windows'
stop having no pipe (`hero_net.c:68-69`).

### 4.2 The second signal, run

- **Darwin**, armed, after the ask `kill(getpid(), SIGINT)` twice: **shared: the first SIGINT ends
  the process (exit 130)**, since the count already reads 1 and the handler takes it for a second
  signal (`hero_net.c:556-577`); **apart: the first SIGINT goes on** (*"after the ask, a first SIGINT:
  the process goes on"*) **and the second ends it (exit 130)**.
- **Linux arm64**, the same: shared, *"exit 130"* with no line after the ask; apart, *"after the ask,
  a first SIGINT: the process goes on"*, then exit 130.
- **Windows**, each design launched into a console of its own, `GenerateConsoleCtrlEvent` twice after
  the ask: **shared: the first Ctrl-C ends it** (the log stops at the ask's line); **apart: the first
  goes on, the second ends it**. (Run first through one `.cmd`, the console event reached `cmd.exe`
  too and left it at its own prompt; those two `cmd.exe`, mine, were stopped by pid.)

So the shared design turns a user's first Ctrl-C into a kill whenever the program has begun its own
stop, which no reader of line 404 (*"Ctrl-C and SIGTERM only make `hero_net_stop_asked() -> i64`
answer 1"*) would expect; apart keeps the user's one-press-stops, two-presses-end.

## 5. Question 5: a connection that sends nothing

### 5.1 `SO_RCVTIMEO` does not reach the layer's waits, run on three systems (`F/c/q5/q5.c`, `F/win/q5win.c`)

| | non-blocking `recv` with `SO_RCVTIMEO` 200 ms | `poll` / `WSAPoll` 1,000 ms on it | the same socket made blocking |
|---|---|---|---|
| Darwin | `-1`, `EAGAIN`, after 0.0 ms | 0 after 1,002.1 ms | `-1` `EAGAIN` after 202.0 ms |
| Linux arm64 | `-1`, `EAGAIN`, after 0.0 ms | 0 after 1,007.2 ms | `-1` `EAGAIN` after 211.1 ms |
| Windows (box) | `-1`, `WSAEWOULDBLOCK`, after 0.0 ms | 0 after 1,008.1 ms | `-1` `WSAETIMEDOUT` after 220.2 ms |

The layer's sockets are non-blocking (`hero_net.c:46-48`) and every wait is its own `poll`, so a
deadline lives in that wait's timeout: the brief's inference, now run (Linux below).

### 5.2 The deadline at the wait, run (a quiet peer in another process, bounded by `alarm(10)`)

The wait as the layer's: `poll` on the socket and the stop's pipe in 50 ms slices, each slice capped
by the time left to an absolute deadline on `CLOCK_MONOTONIC` (`GetTickCount64` on Windows):

- **Darwin**: quiet peer, deadline 500 ms, 20 trials: **timed out 20, data 0, late by 1.89 ms on
  average and 2.76 at worst**; a peer sending one byte at 450 ms: **data 20, timed out 0**.
- **Linux arm64**: quiet peer: **timed out 20, data 0, late by 4.86 ms on average and 10.91 at
  worst**; the byte at 450 ms: **data 20, timed out 0**.
- **Windows**: quiet peer: **timed out 20, late 2.61 ms on average, 16.83 at worst**; the early byte:
  **data 20, timed out 0**. The worst case is the clock's: Microsoft's `GetTickCount64` page, *"The
  resolution of the GetTickCount64 function is limited to the resolution of the system timer, which
  is typically in the range of 10 milliseconds to 16 milliseconds"*; `QueryPerformanceCounter` is
  *"a high resolution (<1µs) time stamp"* (`F/pages/gtc64.html`, `qpc.html`). So the Windows arm
  clocks a deadline with `QueryPerformanceCounter`.

**The clock that never goes back, read**: Darwin's `man clock_gettime`: `CLOCK_MONOTONIC` *"will
continue to increment while the system is asleep"*; `CLOCK_UPTIME_RAW` *"does not increment while the
system is asleep. The returned value is identical to the result of mach_absolute_time()"*. A
deadline that counts a laptop's sleep (`CLOCK_MONOTONIC`) ends a quiet connection on waking, which is
what a server wants; Linux's own (`CLOCK_MONOTONIC`, not counting suspend; `CLOCK_BOOTTIME`, counting
it) is read in the Linux log below only if the image carries the manual.

## 6. Verdicts, in my definition's structure (`.claude/agents/ffi-pragmatist.md:33`)

### Question 1, defect 2522

- `verdict`: **approve route I-mask** (Darwin: an interposing dylib of three names, defining no name
  of the C library; Linux: the runtime defines glibc's installers and mask calls by name; Windows:
  nothing new); **veto route I-onstack** (forcing `SA_ONSTACK`), **veto B**, **veto T**; object to U,
  W and R as routes for 2522; K stays forbidden by R3's own words.
- `section`: design.md §1.11 (everything a program needs comes from C libraries, so the runtime must
  not change what a library's own handler does) and §4.19 (the FFI); §1.12 (*"must not segfault and
  must not corrupt memory"*) for the vetoes.
- `experiment`: §§ 1.1 to 1.11 above, every row compiled and run on Darwin arm64 and Linux arm64,
  and the Windows rows on the box. The decisive ones: I-onstack makes a collector's suspend handler
  record a stack pointer outside its thread's stack (`gcshape.c`, both legs; bdwgc's own code reads
  `GC_approx_sp()`; the real bdwgc under it, § 1.12 below); B stops `raise` delivering, an `alarm`
  interrupting a `read`, and SIGPROF sampling the running thread (both legs); T refuses a correct
  program (both legs, `-O0` and `-O2`); I-mask refuses R3's two cases 20 of 20 on Darwin and Linux
  (§ 1.10, § 1.12), the two-level dylib on Darwin included, leaves a handler's stack and flags
  untouched, and costs 1,346 instructions a callback on Darwin.
- `argument`: R3's cases are a library's handler calling a Heroes function with no frame record. The
  kernel knows a handler is running; the question is how to ask it without changing the library.
  `SA_ONSTACK` changes the library (bdwgc scans the wrong stack: memory corrupted or a fault). A mask
  bit changes nothing the library sees but one blocked signal it never named, and `siglongjmp` puts
  it back. On Darwin one dylib reaches every image; on Linux the executable's definitions reach
  static archives, shared libraries and `dlopen`. Windows has no asynchronous delivery onto a claimed
  thread, and cannot link a definition of `signal` anyway.
- `prediction`: (1) **already run** (§ 1.12): bdwgc 8.2.8 with the runtime's alternate stack, 5 of 5
  intact under I-mask, 5 of 5 memory faults under I-onstack; a second collector of the same shape
  (one that reads its own frame in a signal handler to find a thread's stack) fails the same way
  under I-onstack and not under I-mask; (2) under I-mask, R3's
  `fliblocale` built against **a Homebrew-shaped dylib** (two-level, `-O2 -fomit-frame-pointer`) is
  refused 20 of 20 on Darwin, as the two-level dylib rows are; (3) a landing's differential test
  (`diff.c`, `diff2.c`) reads 0 mismatches on glibc 2.41 and Darwin 27.2 and **finds at least one
  mismatch on the first C library it was not written against** (musl, or an older Darwin), which is
  why it must run on every CI leg.
- `condition`: I withdraw approval of I-mask if a C library a binding plausibly links is shown,
  compiled and run, to need the marker signal itself (SIGURG: blocked by it outside a handler through
  a raw system call, or waited for), or if a landing drops the `longjmp` family (glibc's plain
  `longjmp` out of a handler then refuses every later correct callback on that thread, run, § 1.10);
  I would lift the I-onstack veto only for a version that leaves a handler on the stack it asked for
  (none built).

**What I-mask needs, said in the escape's words** (00-shared.md § The lane): **a widened
diagnostic**: on Linux the runtime would newly define `bsd_signal`, `sigset`, `ssignal`,
`sysv_signal`, `__sysv_signal`, `__sigaction`, `siginterrupt` and `sigvec`, so panel 239's
`ffi_defined_elsewhere` refuses a library of the program that defines one of them (it already covers
`sigaction`, `signal`, `sigprocmask` and `pthread_sigmask`, which the runtime calls today); on Darwin
the dylib defines no C library name, so nothing widens there. **No spec sentence** that I can see:
the refusal is R1's, its words R9's; the reserved marker (SIGURG, the probe's already) is a sentence
of `hero_os.h`, the spec-warden's to price. **A link change on Darwin**: programs that hand C a
function link the dylib; `heroes build` gives a program no `LC_RPATH` today (measured), so the dylib
needs an install name a landing chooses (beside the program with `@executable_path`, or the
toolchain's own absolute path) and the hardened runtime needs it signed by the program's Team ID.
**Two goldens' words move** (§ 1.11), and D needs a case of its own no definition reaches.

### Question 2, defect 2502

- `verdict`: **approve one poller for the process** (route e: `kqueue` on Darwin, `epoll` on Linux,
  a `WSAPoll` thread with a loopback socket pair as its wake on Windows); **object** to a descriptor
  per waiting thread (the lane's pipe) and to a thread-directed signal.
- `section`: §1.11: the descriptor table is shared with every C library the program links.
- `experiment`: § 2: a server under the launchd soft limit of 256 holds **250** connections at the
  base, **249** with one poller, **83** with a pipe per waiting thread and **125** with an
  `eventfd`/`kqueue` per thread, on Darwin and Linux alike; every wake is flat while waiting (Darwin
  poller 49 to 53 million instructions for 100 waiters at 1 and 10 s against the slices' 123 to 398
  million) and answers a close in 24 to 27 µs on all three systems; Windows' poller 1,732,250 cycles
  against the slices' 395,168,250. Darwin has no `ppoll`, so a signal wake has a lost-wakeup window
  there.
- `argument`: the slices cost CPU, not correctness; the repair must not cost a program its
  descriptors. A pipe per waiter cuts a server's room by two thirds before SQLite or curl opens
  anything, and the lane's fall-back to slices then returns the defect under load. One poller costs
  one descriptor and one thread, gives question 5 its timer and question 4 its wake, and has a
  Windows form that compiles and runs.
- `prediction`: a server built on the poller holds within 2 of the base's connection count under
  `ulimit -n 256` on both POSIX legs, and its 100 idle waiters cost less than 1 % of the slices'
  instructions over 10 s.
- `condition`: if the poller's hand-off (a condition variable a wait) is shown to cost a `recv` that
  is ready at once more than the slices do, measured on a busy server, I would take a per-socket
  `kqueue`/`eventfd` instead (125 under 256).

`hero_net.h`'s two *"within about 50 ms"* become *"at once"*: a header promise told, not a spec
sentence (CLAUDE.md § 2: no old promise kept beside).

### Question 3, defect 2490

- `verdict`: **approve the door with the `fchdir` shape**: the parent opens the folder
  (`O_DIRECTORY | O_CLOEXEC`) when the child is started, tells `ENOENT`, `EACCES` or `ENOTDIR` before
  any child exists, and the child opens its streams by path, then `fchdir`s, then `execve`s a program
  path **the parent resolved**; on Windows the parent resolves the folder to a full path and asks its
  kind itself before `CreateProcessW`.
- `section`: §1.11 and §4.19's door (`hero_os.h`, decision 2228), and POSIX's fork page.
- `experiment`: § 3: no hang in 49,000 children on Darwin and 71,500 on Linux (40,000 plain, 31,500
  under ASan) from a parent with four allocating threads (`chdir`, `fchdir`, `execvp`, the rlimit calls); the three errors identical in
  the child's `chdir` and the parent's `open` on both legs; `execvp("./x")` after `chdir` runs the
  child folder's `x` on POSIX while `CreateProcessW` runs the **parent's** (box); `CreateProcessW`
  answers 267 for a missing folder and a file alike; the block keeps `=C:`.
- `argument`: the program a child runs must mean one thing on every system, and the two systems
  disagree unless the parent resolves it; resolving it in the parent also takes `execvp`, which POSIX
  does not list as async-signal-safe, out of the child. `fchdir` is on the list; `chdir` is too, but
  `fchdir` moves the error before the `fork` and resolves a relative folder once.
- `prediction`: with the door, `fetch/fixedbugs-2479-...` passes on the Windows CI leg; the two
  `fixedbugs-2344` folder cases do not, with or without it (clang there splits at `;`, run).
- `condition`: if `getrlimit` in glibc 2.41, whose branch I did not follow, is shown to take a lock or
  allocate, the write ceiling moves out of the child (`prlimit` on the child's pid from the parent);
  `setrlimit` is one system call and Darwin's two are bare system calls, read.

**Needs a spec sentence**, in the escape's words: a new door in `hero_os.h`, the language's promised
door (decision 2228), which a reader must be told of; its order (folder after streams, program
resolved in the parent's folder) is part of what it promises.

### Question 4, a stop the program asks itself

- `verdict`: **approve `hero_net_ask_stop()` kept apart from the signals' count**; **object** to the
  door sharing the count.
- `section`: §4.19 (the layer is C a binding calls from any thread).
- `experiment`: § 4: both designs wake every wait on all three systems (0.0 to 0.5 ms armed on
  POSIX, a slice unarmed, 31 to 63 ms on Windows); ThreadSanitizer 0 warnings in 8 runs over both
  POSIX legs; with the count shared, the user's first Ctrl-C after the program asked ends the
  process (exit 130 on POSIX, the process gone on Windows); apart, the first goes on and the second
  ends it, on all three.
- `argument`: the door is a write of an atomic and of one byte, safe from any thread, and the
  measured difference is only in what a later Ctrl-C means. Sharing the count makes a user's single
  Ctrl-C a kill whenever the program happens to be stopping itself.
- `prediction`: `examples/server`'s handler calling the door from a connection's thread stops the
  server with every wait answering `stopped`, and a single Ctrl-C during that stop does not end it.
- `condition`: none from the C side.

**Needs a spec sentence**: line 404's *"Ctrl-C and SIGTERM only make `hero_net_stop_asked() -> i64`
answer 1"* becomes false; and **a card change**: `<core>/net` gains the door and the reader
`stop_asked` in a new version.

### Question 5, a connection that sends nothing

- `verdict`: **approve a per-socket deadline at the layer's own wait** (an absolute time on the
  system's monotonic clock kept in the socket's table entry, answered `HERO_NET_TIMED_OUT`), with
  `QueryPerformanceCounter` as Windows' clock; **object** to leaning on `SO_RCVTIMEO`.
- `section`: §4.19.
- `experiment`: § 5: `SO_RCVTIMEO` reaches neither a non-blocking `recv` nor a `poll` on Darwin, Linux
  or Windows; the deadline at the wait timed out 20 of 20 quiet peers on all three (worst late 2.76,
  10.91 and 16.83 ms) and delivered the byte sent 50 ms before the deadline 20 of 20; the Windows
  worst case is `GetTickCount64`'s 10 to 16 ms step.
- `argument`: the layer's sockets are non-blocking, so the system's timeout cannot help; the wait's
  own timeout is the one place a deadline works the same on three systems. Kept per socket in the
  table, it crosses to a connection's thread with the socket's number and needs no second value.
- `prediction`: a server built like `examples/server` with a 5 s read deadline on each connection
  keeps accepting under 256 quiet connections, each answered timed out within 20 ms of its deadline
  on POSIX.
- `condition`: none from the C side; a non-waiting join (moving line 377) is a different question I
  did not run.

**A card change**: a new function on the `net` package's card (a new `core` version); no spec
sentence by my reading.

## 7. Evidence (all under `.claude/worktrees/p248-scratch/248-ffi-pragmatist/`)

- `c/onstack/`: `inst.c` (I-onstack's definitions), `inst_mask.c` (I-mask's, with the `longjmp`
  family), `ipose_kernel.c` (I-dylib, one name), `ipose_mask.c` (I-mask's Darwin dylib), `read.c`,
  `diff.c`, `diff2.c` (the differential tests), `rows.c` and `frameless.c` (R4's rows), `gcshape.c`.
- `c/dy/` (Darwin's namespace rows), `c/alt/` (`b.c`, `t.c`, `u.c`, `maskcost.c`), `c/ctwin/`,
  `c/q2/` (`wake.c`, `srv.c`), `c/q3/q3.c`, `c/q4/` (`door.c`, `hero_net_door.c`, `q4.c`),
  `c/q5/q5.c`; `win/` (the box's probes and `put.sh`, `run.sh`); `h/` (R3's programs, the `k-` and
  `m-` copies, `libk/` and `libm/` the route dylibs).
- `linux/`: `run.sh`, `phases.sh`, `c/route/` (the runtime parts: `siginstall.c` I-onstack,
  `siginstall_mask.c` I-mask, `sigguard_mask.c`, `runtime_mask.c`), `c/gcreal.c`, `dl/gc-8.2.8.tar.gz`,
  and the logs `log-read.txt`, `log-q1.txt`, `log-q1r.txt`, `log-q25.txt`, `log-mask.txt`,
  `log-gc.txt` (containers `fp-248-1` to `fp-248-7`, each `docker exit 0`, `docker ps` empty after).
- `logs/`: every Darwin run named above; `pages/`: Microsoft's five pages and bdwgc's two sources.
- Every Darwin storm's crash reports read by pid: base `fp-fliblocale` 6 SIGKILL (lock); route
  `fp-k-fliblocale-nofp` 20 SIGABRT, `fp-ks-fliblocale` 3 SIGABRT, `fp-m-fliblocale` 20 SIGABRT;
  `fp-ctwin-so` 3 SIGKILL (lock); `fp-u`/`fp-ustorm` none.
- No paid run, no credential, no git or GitHub write; the copy's runtime is the base again (`cmp`
  against `df7612ad3`), the `.git` file given to it for the provenance hook removed; nothing of mine
  left running on this Mac, in Docker or on the box (`/c/w/p248-fp/` holds the probes' files).
