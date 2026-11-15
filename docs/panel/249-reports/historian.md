# Panel 249, historian

Written 2026-11-14 by the historian seat, advisory, under `docs/panel/249-briefs/historian.md` and
`00-shared.md` (both read whole, frozen). Every precedent below is a page fetched on 2026-11-14, its URL
beside it in § Log, and says only what that page states; the quotes are in § Log. A line marked
`unverified` rests on no page read today and is not evidence. Panel 247's and 248's historians' pages
are cited from their reports as *247's* and *248's*, not re-read, except ART's `sigchain.cc` and Oracle's
Java SE 26 troubleshooting page, re-read because this sitting leans on sentences 248's report did not
quote (the per-thread mark; HotSpot's own signal table), which is said where they are used. No build, no
run, no paid call, no GitHub write. This seat has no shell (its tools are Read, Write, WebSearch,
WebFetch), so it ran no `grep` of the tree and no `nm`. *Heroes of code* was not consulted, neither as a
route nor as a source.

Status: complete. **No file was written outside the repository's root that this seat saw**: no fetch
returned a saved-file path, every fetch returned text (the long ones read by offset), and the one file
written is this report.

---

## Where the brief is wrong, or says more than its pages, first

The coordinator's list was written from memory and said so. These are the lines a page read today does
not carry, or carries differently.

1. **Go's documentation does not say a c-archive installs SIGURG.** `os/signal/doc.go` at go1.27.3 says
   that for c-archive and c-shared *"For the synchronous signals and SIGPIPE, the Go runtime will install
   a signal handler"* and that such code *"will not install any other signal handlers by default"*.
   `runtime/signal_unix.go` at the same tag installs *"for synchronous signals and SIGPIPE and
   sigPreempt"*, and `sigPreempt = _SIGURG`. The brief's sentence (`00-shared.md:478-479`) rests on the
   source and is right. The page lags it, by my reading of the two; no page says why.
2. **Go issue 39343 does not name SIGSYNCCALL, and musl's author disputes the fix.** The issue says musl
   *"is using signal 34 to implement setgid"*. Rich Felker wrote *"musl does not guarantee that 32-34 are
   reserved for impl-internal use"*. musl's `pthread_impl.h` at `master` does define `SIGSYNCCALL 34`
   (and `SIGTIMER 32`, `SIGCANCEL 33`).
3. **.NET's real-time signal is an "activation" signal on the page read, not a suspension.**
   `signal.cpp` defines `INJECT_ACTIVATION_SIGNAL` as `SIGRTMIN`, and as `SIGUSR1` where there is no
   `SIGRTMIN` (macOS). It is guarded by `FEATURE_HIJACK`. That it suspends threads is `unverified` by
   that file.
4. **SDL's real-time signal is found; its `sigtimedwait` is not.** `SDL_evdev_kbd.c` at release-3.2.10
   takes, for console VT switching, the first number from `SIGRTMIN + 2` to `SIGRTMAX` whose action is
   the default, then SIGUSR1, then SIGUSR2. `sigtimedwait` is in none of the four SDL files read (§ Log),
   so where Debian's `libSDL3.so.0` imports it from stays a question.
5. **OpenSSL's CPU probe takes no SIGILL on Darwin or on glibc arm64.** At openssl-3.5 and openssl-4.1.0
   the SIGILL probe compiles only `#if !defined(__APPLE__) && !defined(OSSL_IMPLEMENT_GETAUXVAL)`. The
   `sigaction` import both platforms show has another source in libcrypto. One is `ui_openssl.c`'s
   password prompt, which replaces the handler of every signal from 1 to `NX509_SIG - 1` but SIGUSR1,
   SIGUSR2 and SIGKILL for the prompt's length. Other sources are not excluded.
6. **Candidate C's Ubuntu inference now has a page, for 24.04's glibc.** Ubuntu's `glibc 2.39-0ubuntu1`
   (2024-02-01): *"Drop the -prof variant to instead use frame pointers on all 64-bit architectures by
   default to match the rest of the distro"*. The CI runs `ubuntu-26.04`, whose glibc was not read, so
   *"Ubuntu's libc keeps the chain"* holds by page for 24.04 and by inference for 26.04. **Debian refuses
   for glibc**: its maintainer, 2020, *"it has a performance impact, so we do not want to do that by
   default"* (bug 767756, wishlist, reported 2014).
7. **D-tramp has a precedent the brief did not name, and the sixth argument has a name.** Go 1.10 on
   Darwin handed the kernel its own trampoline (`sa.sa_tramp = unsafe.Pointer(funcPC(sigtramp))`). Go
   1.12 moved Darwin's system calls to libSystem, *"ensuring forward-compatibility"*. Apple's own
   `_sigtramp` (libplatform `main`) names its sixth parameter `uintptr_t token`, passes it to
   `__sigreturn(uctx, ctxstyle, token)`, and sets `__in_sigtramp = sig` around the handler. That
   confirms the critic's disassembly from Apple's published source. xnu (`main`) makes that token random
   per delivery and refuses a mismatched one with `EINVAL` once validation is on, and validation is fixed
   at a process image's first install. libplatform's `sigaction` always asks for it.
8. **The sanitizer runtimes other than ASan were not read today.** 248's reading of the sanitizer common
   flags (`handle_segv` and its kin) stands, *carried*.

---

## verdict

**approve (advisory), candidate by candidate, with three objections:**

| Candidate | Precedent on the pages read | Reading, advisory |
|---|---|---|
| **A on Linux**, a real-time marker | Runtimes do reserve signals: glibc 32 and 33, made unblockable by its own `sigprocmask`/`pthread_sigmask`; Go SIGURG; .NET `SIGRTMIN`; bdwgc SIGPWR and SIGXCPU (or `SIGRTMIN + 5`, `+ 6`); HotSpot SIGUSR2 | **approve, with its collisions named**: .NET takes `SIGRTMIN` itself (34 under glibc). SDL takes the first number from `SIGRTMIN + 2` **whose action is the default**, so a marker that is only a blocked bit with no handler reads as free to it (my reading of `setup_vt_signal`). bdwgc takes `+ 5`/`+ 6` where built so. musl keeps 32 to 34 *"not guaranteed"*. **Departure, to be deliberate**: no page read uses a signal's blocked bit as a mark (one search, § search) |
| **A on Darwin**, a classic marker | Go took SIGURG because *"platforms without real-time signals (like macOS)"* are *"out"*; .NET took SIGUSR1; HotSpot SIGUSR2 *"on Linux and macOS"*; bdwgc avoids signals on Darwin (Mach `thread_suspend`, Mach exceptions); OpenSSL's prompt overwrites 1 to `NX509_SIG - 1` but USR1, USR2, KILL | **object (advisory)**: the three classic signals precedent recommends are each taken by a runtime a program may bind (SIGURG by Go, SIGUSR1 by .NET, SIGUSR2 by HotSpot). No page read names a free one |
| **B'**, a wrapper with a per-thread depth (248's route I) | ART's `ScopedHandlingSignal`, a per-thread per-signal bitmap around the handlers the chain calls; OpenJDK's `libjsig` and compiler-rt (248's) | **approve as a shape.** **Departure**: ART's mark is around **its own** special handlers, not around the user's. A handler that may not return is met **by declaration** (`SIGCHAIN_ALLOW_NORETURN`: *"Avoid setting the thread local flag in this case, since we'll never get a chance to restore it"*), never by detection. A C library cannot declare, and two census libraries leave by `siglongjmp` (curl's `alarmfunc`, OpenSSL's `ill_handler`) |
| **C**, frame records kept | Fedora 38 (accepted, page of 2023-01-06); Ubuntu 24.04 (2023-12-13), its glibc included (2024-02-01); Apple arm64: *"x29 must always address a valid frame record"* | **approve**: precedented, with measured costs (Fedora 2 to 2.4 %, CPython 1 to 10 %; Ubuntu *"between 1-2% in most cases"*). The residue the brief names is a distribution's choice on the pages: Debian's glibc maintainer refuses |
| **D-tramp** and **the sweep** | Go ≤ 1.10 (read) handed Darwin its own `sa_tramp`; Go 1.12 left raw system calls for libSystem; Apple's `_sigtramp` carries a token xnu validates | **object (advisory)**: the one runtime found doing it abandoned it for *"forward-compatibility"*. The token check (`EINVAL` on mismatch, the state fixed at the first install) is a kernel rule no header states, and a trampoline of the runtime's must honour it |
| **D-imports** | none found (one search) | no objection, no precedent; its holes are the brief's |
| **Question 3**, who tells, in which words | Go throws at the first delivery, naming the fault and no door; HotSpot under CheckJNI prints *"Warning: %s handler modified!"* then *"Consider using jsig library."*; Python's C API names the door (`PyErr_SetInterruptEx`, *"async-signal-safe ... from a C signal handler"*; `PyOS_setsig`, *"Do not call those functions directly!"*); Perl 5.8.0 and CPython defer user code out of the handler | **approve words that name the doors**, HotSpot's shape. **This seat's reading of question 3 favours new words of a runtime refusal a reader must learn**, so the escape binds it: the synthesis convenes the spec-warden, and the blind seat where a reader could get it wrong, before adopting it |

---

## precedents

### Question 1: what each census library documents of its own signal use

Every row is from a page or source file at the version named, read 2026-11-14; § Log holds the quotes.
"Calls back" means the handler itself calls code the program handed the library.

| Library (page, version) | Signals it takes | Flags, stack | Calls back from the handler? | Switch | Marked |
|---|---|---|---|---|---|
| **Go c-archive** (`signal_unix.go`, `os/signal/doc.go`, go1.27.3) | synchronous signals, SIGPIPE, **SIGURG** (`sigPreempt`); every earlier handler saved | turns on `SA_ONSTACK` in a handler it finds; throws at a delivery off the signal stack | no (Go code runs in Go's handler) | `os/signal.Reset`; the doc names no switch for SIGURG | verified |
| **bdwgc** (8.2.8: `gc_priv.h`, `pthread_stop_world.c`, `gc.h`, `os_dep.c`) | Linux: SIGPWR suspend, SIGXCPU restart; `SIGRTMIN + 6`/`+ 5` under `GC_USESIGRT_SIGNALS`; SIGSEGV (and SIGBUS) in incremental mode, chaining to the old handler; Darwin: no stop signal, Mach exceptions for faults | `SA_RESTART`, `SA_SIGINFO`; **no `SA_ONSTACK`** | no | `GC_set_suspend_signal`, `GC_set_thr_restart_signal` before `GC_init`; `GC_USESIGRT_SIGNALS` undocumented in `doc/README.macros` | verified |
| **Android ART** (`sigchain.cc`, `fault_handler.cc`, `main`) | SIGSEGV; SIGBUS under userfaultfd; wraps `signal`, `sigaction`, `sigprocmask` (248's) | `SA_ONSTACK` always (248's); per-thread mark around its special handlers | its special handlers, then the user's | `SkipAddSignalHandler` | verified |
| **glibc NPTL** (`nptl(7)`, `signal(7)`, man-pages 6.19) | 32 and 33 internal; `SIGRTMIN` 34 | its mask calls *"silently ignore attempts to block these two signals"* | n/a | none | verified |
| **musl** (`pthread_impl.h`, `master`; Go issue 39343) | 32 `SIGTIMER`, 33 `SIGCANCEL`, 34 `SIGSYNCCALL`; *"not guarantee[d]"* reserved | n/a | n/a | none | verified |
| **.NET** (`signal.cpp`, `main`) | SIGILL, SIGFPE, SIGBUS, SIGABRT, SIGINT, SIGQUIT, SIGSEGV, SIGTRAP, SIGTERM; activation `SIGRTMIN`, or SIGUSR1 without `SIGRTMIN` | `SA_RESTART`, `SA_SIGINFO`; `SA_ONSTACK` on SIGSEGV alone | chains to `g_previous_*` | `PAL_INITIALIZE_*` flags | verified |
| **SDL** (release-3.2.10: `SDL_quit.c`, `SDL_evdev_kbd.c`, `SDL_systhread.c`, `SDL_posixprocess.c`) | SIGINT, SIGTERM only if their action is the default; on a console, a VT pair from `SIGRTMIN + 2` (else USR1, USR2) and keyboard-restoring fatal handlers; SIGPIPE ignored by its process API; **masks SIGHUP, SIGINT, SIGQUIT, SIGPIPE, SIGALRM, SIGTERM, SIGCHLD, SIGWINCH, SIGVTALRM, SIGPROF in every thread it creates** | VT: `SA_RESTART`; quit: none | **no**: flags only, the program's VT callbacks run later from `kbd_vt_update` | `SDL_HINT_NO_SIGNAL_HANDLERS` | verified |
| **libuv** (`src/unix/signal.c` v1.53.0; `uv_signal_t` page) | the signals a program watches | `SA_RESTART` (+`SA_RESETHAND` oneshot), full mask, **no `SA_ONSTACK`** (*"XXX use a separate signal stack?"*); blocks every signal on its thread around its lock | **no**: writes to a pipe, keeps `errno`; callback from the loop | not installing a watcher; warns off 32 and 33 | verified |
| **OpenSSL** (3.5, 4.1.0: `armcap.c`; 3.5: `ui_openssl.c`) | SIGILL probe only where neither Apple nor `getauxval`, left by `siglongjmp`; a password prompt replaces 1 to `NX509_SIG - 1` but USR1, USR2, KILL, then restores | no flags | no: `recsig` stores the number | `OPENSSL_armcap` skips the probe | verified |
| **curl** (`CURLOPT_NOSIGNAL`; 8.14.1 `hostip.c`, `sigpipe.h`; 8.23.0 `sigpipe.h`) | SIGPIPE ignored around calls (8.14.1: with OpenSSL, mbedTLS or wolfSSL; 8.23.0: unless `USE_SO_NOSIGPIPE`); SIGALRM for a synchronous resolver's timeout, **left by `siglongjmp`**, *"the remainder of the application to run within a signal handler"* | SIGALRM with `SA_RESTART` cleared | no | `CURLOPT_NOSIGNAL` (since 7.10); a threaded or c-ares resolver | verified |
| **ncurses** (`curs_initscr(3X)`, 6.6, 2026-08-22) | SIGINT, SIGTERM (screen clean-up, *"unsafe"*), SIGTSTP, SIGWINCH (a flag), at the first `initscr`/`newterm` | not stated | no | install one's own after initialising the screen | verified |
| **sqlite** | no page found | | | | unverified (question) |
| **Abseil failure handler** (`failure_signal_handler.h`, `master`) | SIGSEGV, SIGILL, SIGFPE, SIGABRT, SIGTERM, SIGBUS, SIGTRAP | alternate stack by default | **yes**: `writerfn`, *"Since this function runs within a signal handler, it should be async-signal-safe if possible"* | `call_previous_handler`, `use_alternate_stack` | verified |
| **Breakpad** (Linux `exception_handler.cc`, `main`) | SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS, SIGTRAP | `SA_ONSTACK \| SA_SIGINFO`, own alternate stack | **yes**: `FilterCallback` and the crash handler, *"This function runs in a compromised context"*; the dump is written by a cloned process | restores the old handlers | verified |
| **GLib** (`g_unix_signal_source_new`) | SIGHUP, SIGINT, SIGTERM; SIGUSR1, SIGUSR2 (2.36); SIGWINCH (2.54) | not stated | **no**: dispatched as a source; *"It is not safe to do any of this from a regular UNIX signal handler"* | not creating the source | verified |
| **CPython** (`signal`) / **Perl** (`perlipc`) | the program's choice | n/a | **no**: a flag, the handler later (CPython: main thread; Perl 5.8.0 on, *"safe" points*) | Perl: `PERL_SIGNALS=unsafe` (5.8.1) | verified |
| **HotSpot** (Java SE 26 troubleshooting; `signals_posix.cpp`, `master`) | SIGSEGV, SIGBUS, SIGFPE, SIGPIPE, SIGILL; SIGQUIT, SIGTERM, SIGINT, SIGHUP (optional); **SIGUSR2** suspend/resume *"on Linux and macOS"* | not stated | n/a | `-Xrs`; `_JAVA_SR_SIGNUM` (above SIGSEGV and SIGBUS); `libjsig` (248's) | verified |
| **SIGURG's owners** | Go (above); the kernel sends it for *"Urgent data"* to the owner set by `SIOCSPGRP`/`F_SETOWN` (`socket(7)`, 6.19, 2026-06-05) | | | | verified |
| UBSan, HWASan, libevent, Crashpad, POSIX timers' `SIGEV_SIGNAL` | not read today | | | | unverified |

### Question 2: precedents for each candidate

- **A, a signal the runtime owns.** Go's four heuristics (debugger pass-through, not used inside libc,
  spurious delivery harmless, *"platforms without real-time signals (like macOS)"*) are the only stated
  choice of a reserved signal found. They appear word for word in proposal 24543 (Austin Clements, last
  updated 2019-01-18) and in `signal_unix.go` at go1.27.3. bdwgc states the opposite route: *"Ideally we
  would allocate a (real-time?) signal using the standard mechanism. unfortunately, there is no standard
  mechanism"*. It hard-codes its numbers and lets the program change them before `GC_init`. HotSpot lets
  an environment variable move its signal; .NET's file offers no override. **glibc is the one owner
  read that keeps its own signals unblockable through its own mask calls**, A's mechanism turned on
  itself. No page read uses a blocked bit as a mark (§ search). **On Darwin**, Go took SIGURG, .NET
  SIGUSR1, HotSpot SIGUSR2, and bdwgc took no signal (Mach). All `verified`.
- **B', a wrapper with a per-thread depth.** ART: a per-thread, per-signal bitmap in `pthread_key_t`s,
  relying on *"bionic's implementation of pthread_(get/set)specific being async-signal safe"*, set by an
  RAII guard around each special handler. A handler declared `SIGCHAIN_ALLOW_NORETURN` does not set it.
  For a claimed signal, a query answers the saved user action, and `SA_RESETHAND` is *"unimplemented"*.
  OpenJDK's interposed `sigaction` *"return[s] the saved signal handlers"* (248's). compiler-rt
  *"cloak[s]"* its handlers by flag (248's). **No page read detects a handler left by `siglongjmp`**: ART
  avoids the case by declaration, and its file names neither `siglongjmp` nor `longjmp`. `verified`.
- **C, frame records kept.** Fedora's change: *"Fedora will add -fno-omit-frame-pointer and
  -mno-omit-leaf-frame-pointer to the default C/C++ compilation flags"*. Target F38, page of 2023-01-06,
  opt-out `%undefine _include_frame_pointers`, kernels excluded, costs measured on Fedora 37. Ubuntu: GCC
  *"will enable frame pointers by default for 64-bit platforms"* from 24.04, Python excepted, *"between
  1-2% in most cases"*; Ubuntu's glibc on all 64-bit architectures from 2.39-0ubuntu1. Debian's glibc
  refuses (bug 767756). GCC: `-fomit-frame-pointer` *"enabled by default at -O1 and higher"*, and
  *"-fno-omit-frame-pointer doesn't guarantee the frame pointer is used in all functions"*. Apple arm64:
  *"The frame pointer register (x29) must always address a valid frame record. Some functions — such as
  leaf functions or tail calls — may opt not to create an entry in this list."* All `verified`.
- **D-tramp.** Go 1.10's `setsig`, `setsigstack`: `sa.sa_tramp = unsafe.Pointer(funcPC(sigtramp)) //
  runtime·sigtramp's job is to call into real handler`. Go 1.12: *"libSystem is now used when making
  syscalls on Darwin, ensuring forward-compatibility with future versions of macOS and iOS."* Apple's
  `_sigtramp(__sigaction_u, sigstyle, sig, sinfo, uctx, token)`; `__in_sigtramp` is `OS_NOEXPORT`, set to
  the signal number before the handler and to 0 before `__sigreturn(uctx, ctxstyle, token)`.
  libplatform's `sigaction`, *"Intercept the sigaction syscall and use our signal trampoline"*, always
  ORs `SA_VALIDATE_SIGRETURN_FROM_SIGTRAMP` (0x0400). xnu `sendsig` puts `token = uctx ^
  uu_sigreturn_token` (random, *"used to validate sigreturn arguments"*) in `x[5]`. `sigreturn` returns
  `EINVAL` on a mismatch unless validation is disabled. `sigaction` fixes the validation state once, at
  the first install with a new action, reset only by `execsigs`. `sa_tramp` is stored per signal
  (`proc_set_sigact_trampact`) after `machine_thread_function_pointers_convert_from_user`. Which xnu
  release added the token: not read, `unverified`. All other lines `verified`.
- **The kernel's facilities** (questions, not routes). Syscall user dispatch: *"Compatibility layers like
  Wine need a way to efficiently emulate system calls of only a part of their process"*, *"Any fork or
  exec of the existing process resets the mechanism to PR_SYS_DISPATCH_OFF"*, *"not a mechanism for
  sandboxing system calls"*. seccomp's `SECCOMP_RET_TRAP` sends *"a thread-directed SIGSYS"*, filters
  are *"preserved across a call to execve(2)"*, and setting one needs `CAP_SYS_ADMIN` or
  `no_new_privs`. x86 shadow stack: enabled by the loader per feature, *"On exec, shadow stack features
  are disabled by the kernel"*, a signal pushes the old SSP token and *"the normal restorer address"*.
  arm64 GCS: per-thread by `prctl`, *"a signal handling specific GCS cap token"* on delivery, *"GCS is
  disabled on exec()"*. All `verified`; which target kernels and CPUs enable either stack: not read.
- **D-imports.** No toolchain or language found that refuses or tells, at build, a program whose bound
  library installs a handler (one search, § search). The nearest is runtime telling: HotSpot's CheckJNI
  check (question 3).

### Question 3: who tells a program, and in which words

- **Go** tells at the first delivery: `"signal", sig, "received but handler not on signal stack"`, the two
  stacks, then `throw("non-Go code set up signal handler without SA_ONSTACK flag")`. It names the fault,
  not a remedy. Issue 35814's *"this will never work"*, a pipe recommended, is 247's, *carried*.
  `verified`.
- **HotSpot** tells periodically under `CheckJNICalls` (*"a periodic task to check for misbehaving JNI
  applications"*). It checks SIGSEGV, SIGILL, SIGFPE, SIGBUS, the shutdown and break signals and
  `SR_signum`, then prints *"Warning: %s handler modified!"*, every handler, and **"Consider using jsig
  library."**: a runtime's words naming the door. The check is off when `libjsig` is loaded (*"we trust
  ourselves"*) or under `AllowUserSignalHandlers`. `verified`.
- **Python** names the door in its C-API reference: `PyErr_SetInterruptEx` (3.10), *"This function is
  async-signal-safe. It can be called without an attached thread state and from a C signal handler"*,
  running the Python handler at the next `PyErr_CheckSignals()`; `PyOS_setsig`, *"Do not call those
  functions directly!"*. `verified`.
- **Rust's signal-hook** 0.4.6: the action *"runs inside a signal handler"*; *"Calling any OS functions
  that are not async-signal-safe as specified as POSIX is not allowed"*; it panics on SIGKILL, SIGSTOP,
  SIGILL, SIGFPE, SIGSEGV. `verified`.
- **Abseil** puts the burden on the caller's callback (*"should be async-signal-safe if possible"*);
  **Breakpad** says its callbacks run *"in a compromised context"*. `verified`.

### Where the rule is written

- **In the language's standard**: C, 7.14.1.1 *The signal function* in C89 (as 4.7.1.1), C99, C11, C17
  and C23, by cppreference's references. Its rule: behaviour is undefined if an asynchronous handler
  calls any standard function but `abort`, `_Exit`, `quick_exit`, `signal` for its own signal and
  lock-free atomics, or refers to a non-atomic static object other than a `volatile sig_atomic_t`.
  `verified` (cppreference, a secondary page citing the standard's clauses; the standard itself not read).
- **On a package's or library's page**: Go's rule is in `os/signal`'s package documentation. Go's
  specification at *"Language version go1.27 (May 26, 2026)"* contains none of *signal*, *SIGURG*,
  *interrupt*, *cgo* (all three parts read by offset). Python's is in its library and C-API references,
  Perl's in `perlipc`, Rust's on the crate's page, Abseil's in its header, and HotSpot's in its
  troubleshooting guide and source. **The JNI specification's design page** (Java SE 21) contains
  neither *signal* nor *handler*. `verified` for each page named, an absence in that page only.
- So Heroes' spec naming the rule (`spec/heroes-spec.md:403-404`, per the brief) puts it where C puts
  it, in the language's reference. Every runtime read beside C leaves it to a library's page.

### What each corrected

| Who | When | Change | Page that says why |
|---|---|---|---|
| Perl | 5.8.0 (safe signals); 5.8.1 (`PERL_SIGNALS`) | user handlers deferred to *"safe" points* instead of running in the C handler | `perlipc`: the old behaviour risked *"possible memory corruption"* |
| Go | 1.12 | Darwin system calls through libSystem; Go 1.10's own `sa_tramp` gone with the raw calls (1.11 not read) | go1.12 notes: *"forward-compatibility with future versions of macOS and iOS"* |
| Go | 1.14 | asynchronous preemption by SIGURG; *"more slow system calls fail with EINTR"* | go1.14 notes; proposal 24543 for the signal's choice |
| Go | 2020-10-28 (commit 49b017f) | signal 34 handled for musl's `setgid` | issue 39343; Felker: *"The fix here was not correct"* |
| GLib | 2.36, 2.54 | SIGUSR1, SIGUSR2; then SIGWINCH added to the watchable list | its page (no reason given) |
| Python | 3.10 | `PyErr_SetInterruptEx`, async-signal-safe, for C code with its own handling | its page |
| curl | between 8.14.1 and 8.23.0 | the SIGPIPE `sigaction` compiled out where `USE_SO_NOSIGPIPE` | the two `sigpipe.h` (no reason; the commit not read) |
| Fedora | 38 (page of 2023-01-06) | frame pointers by default | the change page (profiling; costs measured) |
| Ubuntu | 24.04 (post 2023-12-13); glibc 2.39-0ubuntu1 (2024-02-01) | frame pointers on 64-bit, glibc's included | the blog post and the changelog (*"to match the rest of the distro"*) |
| Debian glibc | 2020-02-16 reply on bug 767756 | frame pointers refused by default | *"it has a performance impact"* |
| OpenJDK | Java 16 | `libjsig`'s `signal`, `sigset` deprecated | 248's (the issue, 403) |

---

## argument

Every candidate has an ancestor; one was abandoned. Runtimes do reserve signals: glibc 32 and 33, Go
SIGURG, .NET `SIGRTMIN`, HotSpot SIGUSR2, bdwgc SIGPWR. But on Darwin each classic candidate is already
some runtime's, and on Linux .NET and SDL take real-time numbers at run time. No page shows a blocked bit
used as a mark. ART keeps a per-thread mark around the handlers it calls and meets one that may not
return by declaration, which a C library cannot make. Fedora, Ubuntu (its glibc included) and Apple keep
frame records; Debian's glibc refuses. Go handed Darwin its own `sa_tramp` and left for libSystem in 1.12,
and xnu validates the trampoline's token. HotSpot's warning names its door. (112 words, counted by hand)

---

## condition

- **A classic Darwin signal that no runtime a program plausibly binds claims**, shown by pages (Go's,
  .NET's, HotSpot's, SDL's and OpenSSL's read here) would lift my objection to A on Darwin.
- **A runtime that uses a blocked bit as a mark and documents why** would make A's mechanism a
  precedent instead of a departure.
- **A runtime that detects a handler left by `siglongjmp`** (a stack pointer kept with the mark, or
  another test) would make B''s depth precedented. ART's declaration is the only answer read.
- **Apple documenting `__sigaction` and `sa_tramp`, or a runtime shipping its own `sa_tramp` on a current
  macOS**, would weaken my objection to D-tramp and the sweep. So would xnu's release note on when token
  validation began, for older systems.
- **Ubuntu 26.04's glibc packaging** (the CI's leg) keeping or dropping frame pointers would confirm or
  overturn the inference this report leaves for 26.04. Debian reversing on bug 767756 would shrink C's
  residue.
- **Where SDL's `sigtimedwait` comes from**: a real-time wait there could add a collision to A on Linux.
- **A toolchain that tells at build that a bound library installs a handler** would make D-imports
  precedented.

---

## search

**How**: WebSearch to find a primary page, WebFetch to read it; a search result's summary was never
cited. Every page was fetched on 2026-11-14 and is quoted in § Log, read whole or at the offsets named.
Pinned versions were fetched by tag where the census names one (go1.27.3, go1.10, bdwgc v8.2.8, libuv
v1.53.0, openssl-3.5, openssl-4.1.0, curl-8_14_1 and curl-8_23_0, SDL release-3.2.10). Everything else
was read at `main` or `master`, which is said beside each.

**Queries (WebSearch)**, in order: *golang issue SIGURG preemption c-archive C library blocks SIGURG
conflict*; *SDL SIGRTMIN sigtimedwait VT switch evdev_kbd source*; *libsdl-org SDL "sigtimedwait" src*;
*sqlite library installs signal handler sigaction os_unix.c "signal" sqlite forum*; *Ubuntu 24.04 frame
pointers enabled by default all packages glibc exception announcement*; *Ubuntu glibc libc6 built with
frame pointers noble launchpad bug "frame pointer" glibc 2.39*; *apple-oss-distributions libplatform
_sigtramp __in_sigtramp source*; *xnu sigreturn token validation ps_sigreturn_token sa_tramp kern_sig.c*;
*linker or compiler warns at build time when a linked library installs a signal handler sigaction
imports check*; *runtime uses blocked signal mask bit as per-thread flag "in signal handler" detect
pthread_sigmask marker async-signal-safe thread local*.

**Projects and sources swept**: Go (`runtime/signal_unix.go` and `os/signal/doc.go` at go1.27.3,
`runtime/os_darwin.go` at go1.10, proposal 24543, go1.12 and go1.14 notes, the specification at go1.27,
issue 39343); bdwgc 8.2.8 (`gc_priv.h`, `pthread_stop_world.c`, `gc.h`, `os_dep.c`, `doc/README.macros`);
Debian `libgc` (`debian/rules`, `master`); Android ART (`sigchain.cc`, `fault_handler.cc`); Linux
man-pages 6.19 (`nptl(7)`, `signal(7)`, `socket(7)`, `seccomp(2)`); musl (`pthread_impl.h`); .NET
(`signal.cpp`); SDL 3.2.10 (`SDL_quit.c`, `SDL_evdev_kbd.c`, `SDL_systhread.c`, `SDL_posixprocess.c`,
and one commit page); libuv (`signal.c` v1.53.0, `uv_signal_t`); OpenSSL (`armcap.c` 3.5 and 4.1.0,
`ui_openssl.c` 3.5); curl (`CURLOPT_NOSIGNAL`, `hostip.c` and `sigpipe.h`); ncurses 6.6
(`curs_initscr(3X)`, `ncurses(3X)`); Abseil (`failure_signal_handler.h`); Breakpad (Linux
`exception_handler.cc`); GLib (`g_unix_signal_source_new`); CPython (`signal`, C-API exceptions and
system pages); Perl (`perlipc`); HotSpot (Java SE 26 troubleshooting, `signals_posix.cpp`); Fedora
(Changes/fno-omit-frame-pointer); Ubuntu (the 2023-12-13 post, glibc 2.39-0ubuntu1 on Launchpad);
Debian (bug 767756 and its debian-glibc message of 2024-03-19); GCC (Optimize Options); Apple (*Writing
ARM64 code for Apple platforms*, as its JSON); Apple libplatform (`sigtramp.c`, `sigaction.c`, `main`);
xnu (`bsd/dev/arm/unix_signal.c`, `bsd/kern/kern_sig.c`, `main`); the kernel's documents
(syscall user dispatch, x86 shadow stack, arm64 GCS); Rust signal-hook 0.4.6; cppreference's C
`signal`; the JNI specification's design page (Java SE 21).

**Pages that failed, and what replaced them**: bdwgc `docs/README.macros` and `include/gc/gc.h` at
v8.2.8 (404; `doc/` and `include/gc.h` read); GLib's `glib/func.unix_signal_source_new.html` (404;
`glib-unix/func.signal_source_new.html` read); Apple's HTML page for arm64 (no body; its JSON read);
Ubuntu's `wiki.ubuntu.com/ToolChain/CompilerFlags` (404; the glibc changelog read instead). Read in part:
`gc.h`'s last 23,509 characters, `ncurses.3x`'s last 1,052 and GCC's Optimize Options' last 30,664 were
not read.

**Not searched or not read**: libevent; UBSan's and HWASan's own pages (248's common-flags wiki
*carried*); Crashpad; Arch Linux's frame-pointer decision; Clang's documentation of its default; POSIX's
pages for SIGURG, `timer_create` and AIO; `fcntl(2)`'s `F_SETOWN`; Go's tracker beyond issue 39343 and one
search; Go 1.11's Darwin source; the commit that changed curl's `sigpipe.h`; Ubuntu 26.04's glibc; .NET's
documentation beyond `signal.cpp`; the C standard itself (cppreference read); the Rust Reference and
Python's language reference; musl at a release tag; bdwgc's `darwin_stop_world.c` (the coordinator's
reading stands); sqlite's `os_unix.c`; which xnu release introduced sigreturn tokens; Oberon, Pascal-P4,
Cyclone, Hylo, Idris, K/APL, cfront, Zig, Nim and Erlang for these mechanisms (none known to this seat to
own one; an assumption, not a finding).

**Not run**: a `grep` of `docs/panel/` for earlier historian lines on these names (no shell). Before any
line below is filed, the synthesis runs `grep -n -i
'sigPreempt\|24543\|39343\|SIGSYNCCALL\|INJECT_ACTIVATION\|find_free_signal\|ScopedHandlingSignal\|ALLOW_NORETURN\|USESIGRT\|NOSIGNAL\|failure_signal_handler\|sa_tramp\|sigreturn_token\|767756\|fno-omit-frame-pointer\|jsig library\|PyErr_SetInterruptEx'
docs/panel/*.md`.

---

## Candidate prior-art lines, per route

In `.claude/rules/records.md`'s two forms. Each applies only if the sitting adopts the route it names,
the historian marked the page `verified` above, and the session filing it re-reads the page. A link to
another project's issue goes through `redirect.github.com`.

**A, a signal the runtime owns (either platform)**

- `- **Reached first by Go**: [runtime/signal_unix.go at go1.27.3](https://raw.githubusercontent.com/golang/go/go1.27.3/src/runtime/signal_unix.go): the preemption signal is SIGURG, chosen as passed through by debuggers, not used internally by libc, harmless when spurious, and available on platforms without real-time signals such as macOS.`
- `- **Reached first by glibc**: [nptl(7)](https://man7.org/linux/man-pages/man7/nptl.7.html): NPTL uses the first two real-time signals internally, SIGRTMIN is 34, and sigprocmask and pthread_sigmask silently ignore attempts to block those two signals.`
- `- **Reached first by .NET**: [signal.cpp](https://raw.githubusercontent.com/dotnet/runtime/main/src/coreclr/pal/src/exception/signal.cpp): the runtime's activation signal is SIGRTMIN where it is defined and SIGUSR1 otherwise.`
- `- **Reached first by bdwgc**: [gc_priv.h at v8.2.8](https://raw.githubusercontent.com/ivmai/bdwgc/v8.2.8/include/private/gc_priv.h): with no standard mechanism to allocate a real-time signal, the collector keeps hard-coded signals, SIGPWR to suspend threads on Linux, and gc.h lets the program choose others before GC_init.`

**B', a wrapper with a per-thread depth**

- `- **Reached first by Android ART**: [sigchain.cc](https://android.googlesource.com/platform/art/+/refs/heads/main/sigchainlib/sigchain.cc): a per-thread bitmap of the signals being handled is set around each special handler and restored after it, and is not set for a handler declared as possibly not returning, since it could never be restored.`
- 248's three lines for route I (OpenJDK, Android ART, compiler-rt; `docs/panel/248-reports/historian.md`
  § Candidate prior-art lines, Q1, route I), *carried*.

**C, frame records kept**

- `- **Reached first by Fedora**: [Changes/fno-omit-frame-pointer](https://fedoraproject.org/wiki/Changes/fno-omit-frame-pointer): Fedora Linux 38 adds -fno-omit-frame-pointer and -mno-omit-leaf-frame-pointer to the default C and C++ flags, with a macro for packages to opt out and measured costs of about 2 to 2.4 % on two workloads.`
- `- **Reached first by Ubuntu**: [frame pointers by default in Ubuntu 24.04 LTS](https://ubuntu.com/blog/ubuntu-performance-engineering-with-frame-pointers-by-default): GCC enables frame pointers by default on 64-bit platforms and almost every package is rebuilt with them, at a penalty of 1 to 2 % in most cases.`
- `- **Reached first by Ubuntu**: [glibc 2.39-0ubuntu1](https://code.launchpad.net/ubuntu/+source/glibc/2.39-0ubuntu1): the C library drops its -prof variant to use frame pointers on all 64-bit architectures by default.`
- `- **Reached first by Apple**: [Writing ARM64 code for Apple platforms](https://developer.apple.com/documentation/xcode/writing-arm64-code-for-apple-platforms): the frame pointer register x29 must always address a valid frame record, leaf functions and tail calls excepted.`

**D-tramp (only if adopted)**

- `- **Reached first by Go**: [runtime/os_darwin.go at go1.10](https://raw.githubusercontent.com/golang/go/go1.10/src/runtime/os_darwin.go): on Darwin the runtime installed its handlers with sa_tramp set to its own sigtramp, whose job is to call into the real handler.`
- `- **Reached first by Apple libplatform**: [sigtramp.c](https://raw.githubusercontent.com/apple-oss-distributions/libplatform/main/src/setjmp/generic/sigtramp.c): _sigtramp takes a sixth argument, a token it hands to __sigreturn, and sets __in_sigtramp to the signal number around the handler.`

**Question 3, the words**

- `- **Reached first by OpenJDK**: [signals_posix.cpp](https://raw.githubusercontent.com/openjdk/jdk/master/src/hotspot/os/posix/signals_posix.cpp): under CheckJNI a periodic check of the VM's signal handlers prints that a handler was modified, every handler, and "Consider using jsig library."`
- `- **Reached first by Python**: [PyErr_SetInterruptEx](https://docs.python.org/3/c-api/exceptions.html): an async-signal-safe call, since 3.10, that C code with its own signal handling makes from its handler so that the Python handler runs later.`
- `- **Reached first by Perl**: [perlipc, Deferred Signals](https://perldoc.perl.org/perlipc): since Perl 5.8.0 the C-level handler sets a flag and returns, and the Perl handler runs at a safe point of the interpreter.`

**Not candidate lines** (for the sitting's file only): Go's `os/signal` lag (a finding, not a decision);
Abseil's and Breakpad's callbacks (the risk documented, not a route); the kernel's facilities (questions);
Debian's refusal (a residue, not a precedent for Heroes' route); xnu's token check (a cost of D-tramp,
not another project's decision about it).

---

## Log, as I went (every page read 2026-11-14)

### Question 1: Go

- **Go `runtime/signal_unix.go` at tag go1.27.3**,
  https://raw.githubusercontent.com/golang/go/go1.27.3/src/runtime/signal_unix.go: the comment above
  `const sigPreempt = _SIGURG`: *"There's no good way to choose this signal, but there are some
  heuristics:"*; *"1. It should be a signal that's passed-through by debuggers by default. On Linux, this
  is SIGALRM, SIGURG, SIGCHLD, SIGIO, SIGVTALRM, SIGPROF, and SIGWINCH, plus some glibc-internal
  signals."*; *"2. It shouldn't be used internally by libc in mixed Go/C binaries because libc may assume
  it's the only thing that can handle these signals. For example SIGCANCEL or SIGSETXID."*; *"3. It
  should be a signal that can happen spuriously without consequences. ... SIGUSR1 and SIGUSR2 are also
  bad because those are often used in meaningful ways by applications."*; *"4. We need to deal with
  platforms without real-time signals (like macOS), so those are out."*; *"We use SIGURG because it meets
  all of these criteria, is extremely unlikely to be used by an application for its "real" meaning (both
  because out-of-band data is basically unused and because SIGURG doesn't report which socket has the
  condition, making it pretty useless), and even if it is, the application has to be ready for spurious
  SIGURG. SIGIO wouldn't be a bad choice either, but is more likely to be used for real."* In
  `sigInstallGoHandler`: *"When built using c-archive or c-shared, only install signal handlers for
  synchronous signals and SIGPIPE and sigPreempt."* `sigNotOnStack`: *"This can only happen if non-Go
  code called sigaction without setting the SS_ONSTACK flag."*, then `println("signal", sig, "received
  but handler not on signal stack")`, the two stacks' bounds and `sp`, and
  `throw("non-Go code set up signal handler without SA_ONSTACK flag")`. In `blockableSig`: *"We never
  want to block the preemption signal if it is being used."* In `sigtrampgo`: *"We got past sigfwdgo, so
  we know that there is no non-Go signal handler for sigPreempt."* **No comment on the page says what
  happens when non-Go code blocks SIGURG or uses it for its own purpose** (the fetch's search of the
  file for those cases). `verified`.
- **Go proposal 24543, *Proposal: Non-cooperative goroutine preemption***,
  https://raw.githubusercontent.com/golang/proposal/master/design/24543-non-cooperative-preemption.md:
  *"Author(s): Austin Clements"*, *"Last updated: 2019-01-18"*; *"We have to choose a signal that is
  unlikely to interfere with existing uses of signals or with debuggers. There are no perfect choices,
  but there are some heuristics."*, then the same four criteria and the same SIGURG paragraph as the
  runtime comment, word for word. Nothing on non-Go code blocking the signal, on cgo or on C threads.
  `verified`.
- **Go `os/signal/doc.go` at tag go1.27.3**,
  https://raw.githubusercontent.com/golang/go/go1.27.3/src/os/signal/doc.go: under *Go programs that use
  cgo or SWIG*: *"If the non-Go code installs any signal handlers, it must use the SA_ONSTACK flag with
  sigaction. Failing to do so is likely to cause the program to crash if the signal is received."*;
  *"The non-Go code should not change the signal mask on any threads created by the Go runtime. If the
  non-Go code starts new threads itself, those threads may set the signal mask as they please."*; a
  non-Go thread that calls Go gets *"the synchronous signals, SIGILL, SIGTRAP, SIGSTKFLT, SIGCHLD,
  SIGPROF, SIGCANCEL, and SIGSETXID"* unblocked, its mask restored on return. Under *Non-Go programs that
  call Go code*: *"For -buildmode=c-archive the Go runtime will initialize signals at global constructor
  time."*; *"For the synchronous signals and SIGPIPE, the Go runtime will install a signal handler. It
  will save any existing signal handler."*; *"Go code built with -buildmode=c-archive or
  -buildmode=c-shared will not install any other signal handlers by default. If there is an existing
  signal handler, the Go runtime will turn on the SA_ONSTACK flag and otherwise keep the signal
  handler."* The word *preemption* is not on the page; SIGURG appears once, in a list of signals
  `Notify` can catch. `verified`.
- **Go 1.14 release notes**, https://go.dev/doc/go1.14: *"Goroutines are now asynchronously
  preemptible. ... This is supported on all platforms except windows/arm, darwin/arm, js/wasm, and
  plan9/*."*; *"A consequence of the implementation of preemption is that on Unix systems, including
  Linux and macOS systems, programs built with Go 1.14 will receive more signals than programs built with
  earlier releases. This means that programs that use packages like syscall or golang.org/x/sys/unix will
  see more slow system calls fail with EINTR errors."* SIGURG and cgo are not named; no date on the page.
  `verified`.
- **Go issue 39343**, https://github.com/golang/go/issues/39343: *"runtime: musl uses signal 34 for
  setgid"*, opened by nmeum on Jun 1, 2020, closed by commit 49b017f on Oct 28, 2020. Ian Lance Taylor:
  *"Looks like the C library is using signal 34 to implement setgid, perhaps in the way that glibc uses
  signal 33."*; the fix, CL 236518, *"runtime: handle signal 34 for musl setgid"*. Rich Felker: *"The fix
  here was not correct and will likely break when musl changes/reduces implementation-internal use of
  signals."* and *"musl does not guarantee that 32-34 are reserved for impl-internal use."* `verified`.

### Question 1: bdwgc 8.2.8

- **`include/private/gc_priv.h` at tag v8.2.8**,
  https://raw.githubusercontent.com/ivmai/bdwgc/v8.2.8/include/private/gc_priv.h (from offset 100000):
  *"We define the thread suspension signal here, so that we can refer to it in the dirty bit
  implementation, if necessary. Ideally we would allocate a (real-time?) signal using the standard
  mechanism. unfortunately, there is no standard mechanism. (There is one in Linux glibc, but it's not
  exported.) Thus we continue to use the same hard-coded signals we've always used."*; under
  `THREAD_SANITIZER`, `SIGSYS`, *"use some synchronous one instead (which is again unlikely to be used by
  clients directly)"*; Linux without `GC_USESIGRT_SIGNALS`, *"Linuxthreads itself uses SIGUSR1 and
  SIGUSR2."*, `# define SIG_SUSPEND SIGPWR` (SIGLOST on SPARC); FreeBSD with glibc `(32+6)`; FreeBSD,
  HURD, RTEMS `SIGUSR1`; OpenBSD `SIGXFSZ`; otherwise `_SIGRTMIN + 6` or `SIGRTMIN + 6`. The block
  excludes `GC_DARWIN_THREADS`. No comment warns of a collision. `verified`.
- **`pthread_stop_world.c` at tag v8.2.8**,
  https://raw.githubusercontent.com/ivmai/bdwgc/v8.2.8/pthread_stop_world.c: *"Since SIG_SUSPEND and/or
  SIG_THR_RESTART could represent a non-constant expression (e.g., in case of SIGRTMIN), actual signal
  numbers are determined by GC_stop_init() unless manually set (before GC initialization)."*;
  `SIG_THR_RESTART` branches `_SIGRTMIN + 5`, `(32+5)`, `SIGUSR2`, `SIGXCPU`, no comment on why; suspend
  handler with `SA_RESTART`, plus `SA_SIGINFO` unless `NO_SA_SIGACTION`; `SA_ONSTACK` absent. `verified`.
- **`include/gc.h` at tag v8.2.8**, https://raw.githubusercontent.com/ivmai/bdwgc/v8.2.8/include/gc.h
  (first 100,000 characters): `GC_set_suspend_signal`, *"Suggest the GC to use the specific signal to
  suspend threads. Has no effect after GC_init and on non-POSIX systems."*; `GC_set_thr_restart_signal`
  likewise; `GC_get_suspend_signal`, *"Return the signal number (constant after initialization) used by
  the GC to suspend threads on POSIX systems. Return -1 otherwise."* `verified`.
- **`os_dep.c` at tag v8.2.8**, https://raw.githubusercontent.com/ivmai/bdwgc/v8.2.8/os_dep.c (both
  halves): `GC_dirty_init` installs `GC_write_fault_handler` for SIGSEGV (SIGBUS on HPUX, Linux, HURD,
  FreeBSD) with `SA_RESTART | SA_SIGINFO`; `GC_old_segv_handler = oldact.sa_sigaction`, called, *"FIXME:
  This code should probably check if the old signal handler used the traditional style and if so call it
  using that style."*; under `#ifndef DARWIN`; *"All this SIGBUS code shouldn't be necessary. All
  protection faults should be going through the mach exception handler."* No reason for the Mach route.
  Whether incremental mode is off unless asked: not in the part read, `unverified`. `verified`.
- **`doc/README.macros` at tag v8.2.8**, https://raw.githubusercontent.com/ivmai/bdwgc/v8.2.8/doc/README.macros:
  `SIGRT`, `USESIG` and `SIGNAL` absent (the fetch's search of the whole file); only `SUNOS5SIGS`.
  `verified`, an absence in that file only.
- **Debian `libgc`, `debian/rules` on `master`**, https://salsa.debian.org/debian/libgc/-/raw/master/debian/rules:
  `--enable-cplusplus`, `--enable-docs`, `--with-libatomic-ops=none`, the paths, `LDFLAGS += -pthread`;
  nothing on signals. `verified` for `master`, not tied to `1:8.2.8-1` by the page.

### Question 1: Android ART (re-read for the per-thread mark)

- **`sigchainlib/sigchain.cc` at `main`**,
  https://android.googlesource.com/platform/art/+/refs/heads/main/sigchainlib/sigchain.cc (`?format=TEXT`,
  base64 decoded by the fetch, twice): `GetHandlingSignalKey`, *"We rely on bionic's implementation of
  pthread_(get/set)specific being async-signal safe."*; *"Use a bitmap to indicate which signal is being
  handled so that other non-blocked signals are allowed to be handled, if raised."*; `ScopedHandlingSignal`'s
  destructor `SetHandlingSignal(signo_, original_value_)`; `SignalChain::Handler`: *"Try the special
  handlers first. If one of them crashes, we'll reenter this handler and pass that crash onto the user
  handler."*, `if (!GetHandlingSignal(signo)) {`; *"The native bridge signal handler might not return.
  Avoid setting the thread local flag in this case, since we'll never get a chance to restore it."*,
  `bool handler_noreturn = (handler.sc_flags & SIGCHAIN_ALLOW_NORETURN);`, `ScopedHandlingSignal
  restorer(signo, !handler_noreturn);`; the user handler under the ucontext mask ORed with its `sa_mask`,
  its signal added unless `SA_NODEFER`; *"SA_RESETHAND: unimplemented, but we can probably do this?"*; a
  claimed signal's query answers the saved user action (code). No `siglongjmp` or `longjmp`. `verified`.
- **`runtime/fault_handler.cc` at `main`**,
  https://android.googlesource.com/platform/art/+/refs/heads/main/runtime/fault_handler.cc (`?format=TEXT`):
  `AddSpecialSignalHandlerFn(SIGSEGV, &sa)`, `.sc_flags = 0UL`, a mask of all but SIGABRT, SIGBUS, SIGFPE,
  SIGILL, SIGSEGV; `if (gUseUserfaultfd)` SIGBUS too; without the chain, `sigaction(SIGBUS, ...)` with
  `SA_SIGINFO | SA_RESTART`. `verified`.

### Question 1: the real-time signals' owners

- **`nptl(7)`**, https://man7.org/linux/man-pages/man7/nptl.7.html (man-pages 6.19, 2026-02-08): *"NPTL
  makes internal use of the first two real-time signals (signal numbers 32 and 33)."*; one for
  *"thread cancelation and POSIX timers"*, the other for UIDs and GIDs; *"SIGRTMIN is defined with the
  value 34 (rather than 32)."*; *"The sigprocmask(2) and pthread_sigmask(3) interfaces silently ignore
  attempts to block these two signals."* `verified`.
- **`signal(7)`**, https://man7.org/linux/man-pages/man7/signal.7.html (6.19, 2026-02-08): *"The Linux
  kernel supports a range of 33 different real-time signals, numbered 32 to 64."*; glibc *"adjusts the
  value of SIGRTMIN suitably (to 34 or 35)"*; *"programs should never refer to real-time signals using
  hard-coded numbers"*; SIGURG *"Ign"*, *"Urgent condition on socket (4.2BSD)"*; SIGPWR *"Term"*.
  `verified`.
- **musl `src/internal/pthread_impl.h` at `master`**,
  https://git.musl-libc.org/cgit/musl/plain/src/internal/pthread_impl.h: `#define SIGTIMER 32`, `#define
  SIGCANCEL 33`, `#define SIGSYNCCALL 34`, no comment. `verified`.
- **.NET `signal.cpp` at `main`**,
  https://raw.githubusercontent.com/dotnet/runtime/main/src/coreclr/pal/src/exception/signal.cpp:
  `#ifdef SIGRTMIN` / `#define INJECT_ACTIVATION_SIGNAL SIGRTMIN` / `#else` / `#define
  INJECT_ACTIVATION_SIGNAL SIGUSR1`; `#error FEATURE_HIJACK requires INJECT_ACTIVATION_SIGNAL to be
  defined`; handlers and flags as in the table; *"Call the original handler when it is not ignored or
  default (terminate)."*; *"A handler that runs on a separate stack should not be interrupted by the
  activation signal until it switches back to the regular stack"*; *"On macOS older than 14.4,
  pthread_kill is not allowed to sent a signal to dispatch queue threads"*. `verified`.
- **SDL `SDL_evdev_kbd.c` at release-3.2.10**,
  https://raw.githubusercontent.com/libsdl-org/SDL/release-3.2.10/src/core/linux/SDL_evdev_kbd.c (read
  twice): `for (i = SIGRTMIN + 2; i <= SIGRTMAX; ++i) { if (setup_vt_signal(i, handler)) {`, then SIGUSR1,
  SIGUSR2; `setup_vt_signal` with `SA_RESTART`, refusing a number whose previous action was not the
  default, *"This signal is already in use"*; `mode.relsig`, `mode.acqsig` to `VT_SETMODE`;
  `kbd_vt_release_signal_action` only `SDL_SetAtomicInt(&vt_signal_pending, VT_SIGNAL_RELEASE)`; fatal
  signals SIGHUP, SIGQUIT, SIGILL, SIGABRT, SIGFPE, SIGSEGV, SIGPIPE, SIGBUS, SIGSYS, the saved action put
  back and the signal re-raised; under `SDL_HINT_NO_SIGNAL_HANDLERS` none. `verified`.
- **SDL's `sigtimedwait`**: absent from `SDL_evdev_kbd.c`, `SDL_posixprocess.c`
  (https://raw.githubusercontent.com/libsdl-org/SDL/release-3.2.10/src/process/posix/SDL_posixprocess.c,
  SIGPIPE ignored only), `SDL_systhread.c` and `SDL_quit.c` (below); commit a559864 (Cameron Gutman,
  2021-10-30, *"x11/wayland: Fix signal handling while blocking in WaitEventTimeout()"*,
  https://skia.googlesource.com/external/github.com/libsdl-org/SDL/+/a55986496891f4a285512a49e9b853c195e4e175)
  showed no diff text. A question.

### Question 1: libuv, OpenSSL, SDL, curl, ncurses, sqlite, HotSpot, SIGURG

- **libuv `src/unix/signal.c` at v1.53.0**, https://raw.githubusercontent.com/libuv/libuv/v1.53.0/src/unix/signal.c:
  `sa.sa_flags = SA_RESTART;`, `SA_RESETHAND` if oneshot, `sigfillset(&sa.sa_mask)`, *"XXX use a separate
  signal stack?"*; the handler saves `errno`, `write(handle->loop->signal_pipefd[1], ...)`, restores it;
  `uv__signal_block_and_lock`: `sigfillset`, `pthread_sigmask(SIG_SETMASK, ...)`. `verified`.
- **libuv `uv_signal_t`**, https://docs.libuv.org/en/v1.x/signal.html: *"SIGKILL and SIGSTOP are impossible
  to catch."*; *"On Linux SIGRT0 and SIGRT1 (signals 32 and 33) are used by the NPTL pthreads library to
  manage threads. Installing watchers for those signals will lead to unpredictable behavior and is
  strongly discouraged. Future versions of libuv may simply reject them."* `verified`.
- **OpenSSL `crypto/armcap.c`**, https://raw.githubusercontent.com/openssl/openssl/openssl-3.5/crypto/armcap.c
  and https://raw.githubusercontent.com/openssl/openssl/openssl-4.1.0/crypto/armcap.c: `#if
  !defined(__APPLE__) && !defined(OSSL_IMPLEMENT_GETAUXVAL)` around `ill_handler`; *"If all else fails, do
  brute force SIGILL-based feature detection"*; `sigaction(SIGILL, &ill_act, &ill_oact)`; `sigsetjmp(ill_jmp,
  1)`, `siglongjmp(ill_jmp, sig)`; *"Capability probing by catching SIGILL appears to be problematic on
  iOS."*; `getenv("OPENSSL_armcap")`; 4.1.0 *"Copyright 2011-2026"*. `verified`.
- **OpenSSL `crypto/ui/ui_openssl.c` at openssl-3.5**,
  https://raw.githubusercontent.com/openssl/openssl/openssl-3.5/crypto/ui/ui_openssl.c: `pushsig` loops
  from 1 to `NX509_SIG - 1`, skipping SIGUSR1, SIGUSR2 and SIGKILL (*"We can't make any action on that."*),
  `sa.sa_handler = recsig;`, `sigaction(i, &sa, &savsig[i]);`, `signal(SIGWINCH, SIG_DFL);`; `recsig`:
  `intr_signal = i;`; `popsig` restores. `verified`.
- **SDL `SDL_quit.c` at release-3.2.10**, https://raw.githubusercontent.com/libsdl-org/SDL/release-3.2.10/src/events/SDL_quit.c:
  `if (action.sa_handler == SIG_DFL ...)`; *"Both SIGINT and SIGTERM are translated into quit
  interrupts"*; *"We can't send it in signal handler; SDL_malloc() might be interrupted!"*;
  `SDL_HINT_NO_SIGNAL_HANDLERS`. `verified`.
- **SDL `SDL_systhread.c` at release-3.2.10**, https://raw.githubusercontent.com/libsdl-org/SDL/release-3.2.10/src/thread/pthread/SDL_systhread.c:
  *"List of signals to mask in the subthreads"*, the ten signals, *"Mask asynchronous signals for this
  thread"*, `pthread_sigmask(SIG_BLOCK, &mask, 0);`. `verified`.
- **curl `CURLOPT_NOSIGNAL`**, https://curl.se/libcurl/c/CURLOPT_NOSIGNAL.html: quoted in the table and
  in question 1's notes above; *"Added in curl 7.10"*. `verified`.
- **curl `lib/hostip.c` at curl-8_14_1**, https://raw.githubusercontent.com/curl/curl/curl-8_14_1/lib/hostip.c:
  *"alarm-based timeouts can only be used with all the dependencies satisfied"*; *"Beware this is a global
  and unique instance. ... This is not thread-safe stuff."*; *"This signal handler jumps back into the main
  libcurl code and continues execution. This effectively causes the remainder of the application to run
  within a signal handler which is nonportable and could lead to problems."*; `sigact.sa_flags &=
  ~SA_RESTART;`. `verified`.
- **curl `lib/sigpipe.h`**, https://raw.githubusercontent.com/curl/curl/curl-8_14_1/lib/sigpipe.h and
  https://raw.githubusercontent.com/curl/curl/curl-8_23_0/lib/sigpipe.h: 8.14.1, `#if
  defined(HAVE_SIGACTION) && (defined(USE_OPENSSL) || defined(USE_MBEDTLS) || defined(USE_WOLFSSL))`,
  *"sigpipe_ignore() makes sure we ignore SIGPIPE while running libcurl internals"*; 8.23.0, `#if
  defined(HAVE_SIGACTION) && !defined(USE_SO_NOSIGPIPE)`. `verified`.
- **ncurses `curs_initscr(3X)`**, https://invisible-island.net/ncurses/man/curs_initscr.3x.html (6.6,
  2026-08-22): quoted in the table; and `ncurses(3X)`,
  https://invisible-island.net/ncurses/man/ncurses.3x.html (6.6, patch 20261010, page 2026-09-19), whose
  first 100,000 characters name no handler. `verified`.
- **HotSpot, Java SE 26 *Handle Signals and Exceptions***,
  https://docs.oracle.com/en/java/javase/26/troubleshoot/handle-signals-exceptions.html (re-read for the
  signal table 248 did not quote): Table 8-2 as in the census table; *"SIGUSR2 is used to implement,
  suspend, and resume on Linux and macOS. However, it is possible to specify an alternative signal ... by
  specifying the _JAVA_SR_SIGNUM environment variable. ... it must be set to a value larger than the
  maximum of SIGSEGV and SIGBUS."*; `-Xrs`: *"fewer signals are used, although the VM installs its own
  signal handler for essential signals such as SIGSEGV"*. `verified`.
- **HotSpot `signals_posix.cpp` at `master`**,
  https://raw.githubusercontent.com/openjdk/jdk/master/src/hotspot/os/posix/signals_posix.cpp: *"This
  method is a periodic task to check for misbehaving JNI applications under CheckJNI"*; the checks listed
  in question 3; *"As we ignore SIGPIPE and SIGXFSZ, and expect other code to potentially install handlers
  for them, we don't bother checking them here."*; `tty->print_cr("Warning: %s handler modified!", ...)`;
  `st->print_cr("Consider using jsig library.");`; *"We don't activate signal checker if libjsig is in
  place, we trust ourselves and if UserSignalHandler is installed all bets are off."*; `int
  PosixSignals::SR_signum = SIGUSR2;`, *"do not use any signal number less than SIGSEGV, see 4355769"*.
  `verified`.
- **`socket(7)`**, https://man7.org/linux/man-pages/man7/socket.7.html (6.19, 2026-06-05): *"Urgent data
  arrived. SIGURG is sent then."*; `SIOCSPGRP`, *"Set the process or process group that is to receive
  SIGIO or SIGURG signals when I/O becomes possible or urgent data is available."*, *"see the description
  of F_SETOWN in fcntl(2)"*. `verified`.
- **sqlite**: one web search (§ search), which returned only `sigaction` manuals; no page read.

### Question 1: crash reporters, event loops, interpreters

- **Abseil `failure_signal_handler.h` at `master`**,
  https://raw.githubusercontent.com/abseil/abseil-cpp/master/absl/debugging/failure_signal_handler.h:
  quoted in the table. `verified`.
- **Breakpad Linux `exception_handler.cc` at `main`**,
  https://raw.githubusercontent.com/google/breakpad/main/src/client/linux/handler/exception_handler.cc:
  `kExceptionSignals[] = { SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS, SIGTRAP }`; `kSigStackSize =
  std::max<unsigned>(16384, SIGSTKSZ)`; `sa.sa_flags = SA_ONSTACK | SA_SIGINFO;`; *"This function runs in a
  compromised context: see the top of the file."*; *"Sometimes, Breakpad runs inside a process where some
  other buggy code saves and restores signal handlers temporarily with 'signal' instead of 'sigaction'.
  This loses the SA_SIGINFO flag"*. `verified`.
- **GLib `g_unix_signal_source_new`**, https://docs.gtk.org/glib-unix/func.signal_source_new.html: quoted
  in the table; *"Available since: 2.30"*. `verified`.
- **CPython `signal`**, https://docs.python.org/3/library/signal.html: quoted in the table. `verified`.
- **Perl `perlipc`**, https://perldoc.perl.org/perlipc: quoted in the table and § What each corrected.
  `verified`.

### Question 2

- **Fedora**, https://fedoraproject.org/wiki/Changes/fno-omit-frame-pointer: owners Daan De Meyer, Davide
  Cavalca, Andrii Nakryiko; *"Fedora Linux 38"*, to be re-evaluated *"by Fedora Linux 40"*; *"Compiling
  the kernel with GCC is 2.4% slower with frame pointers"*, *"Running Blender to render a frame is 2%
  slower"*, *"The impact on CPython benchmarks can be anywhere from 1-10%"*; `%undefine
  _include_frame_pointers`; *"Any kernel packages"* excluded; architectures not named; *"Last updated:
  2023-01-06"*, `ChangeAcceptedF38`. `verified`.
- **Ubuntu**, https://ubuntu.com/blog/ubuntu-performance-engineering-with-frame-pointers-by-default:
  Oliver Smith with Jon Seager, *"13 December 2023"*; *"beginning with Ubuntu 24.04 LTS our GNU Compiler
  Collection (GCC) package will enable frame pointers by default for 64-bit platforms"*; *"All packages in
  Ubuntu, with very few exceptions, will be rebuilt with frame pointers enabled"*; *"We will not make this
  change on 32-bit architectures"*; Python excepted; *"between 1-2% in most cases"*. `verified`.
- **Ubuntu glibc 2.39-0ubuntu1**, https://code.launchpad.net/ubuntu/+source/glibc/2.39-0ubuntu1: *"Drop
  the -prof variant to instead use frame pointers on all 64-bit architectures by default to match the rest
  of the distro"*, LP #2042790, Simon Chopin, *"Thu, 01 Feb 2024 09:44:24 +0100"*. `verified`.
- **Debian bug 767756**, https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=767756: *"glibc: Consider
  providing a libc build compiled with -fno-omit-frame-pointer to help with profiling"*, src:glibc,
  reported 2 Nov 2014 by Andres Freund, wishlist; Aurelien Jarno, 16 Feb 2020: *"I understand the need
  for -fno-omit-frame-pointer, however it has a performance impact, so we do not want to do that by
  default."*, *"Someone has to come with a patch that is well tested."*; open or closed not stated on the
  page. `verified`. And its message of 2024-03-19 (Ralf Jung),
  https://lists.debian.org/debian-glibc/2024/03/msg00027.html: *"Fedora and Ubuntu [1] have now enabled
  frame pointers on amd64 by default"*. `verified`.
- **GCC Optimize Options**, https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html (first 100,000
  characters, version not stated): *"Omit the frame pointer in functions that don't need one."*;
  *"-fno-omit-frame-pointer doesn't guarantee the frame pointer is used in all functions"*; enabled at
  `-O1` and higher. `verified`.
- **Apple, *Writing ARM64 code for Apple platforms***,
  https://developer.apple.com/tutorials/data/documentation/xcode/writing-arm64-code-for-apple-platforms.json
  (the page's JSON): *"The frame pointer register (x29) must always address a valid frame record. Some
  functions — such as leaf functions or tail calls — may opt not to create an entry in this list. As a
  result, stack traces are always meaningful, even without debug information."*; *"The platforms reserve
  register x18."* `verified`.
- **Go 1.10 `runtime/os_darwin.go`**, https://raw.githubusercontent.com/golang/go/go1.10/src/runtime/os_darwin.go:
  `setsig`: `sa.sa_flags = _SA_SIGINFO | _SA_ONSTACK | _SA_RESTART`, `sa.sa_mask = ^uint32(0)`,
  `sa.sa_tramp = unsafe.Pointer(funcPC(sigtramp)) // runtime·sigtramp's job is to call into real
  handler`; `setsigstack` the same `sa_tramp`. `verified`.
- **Go 1.12 release notes**, https://go.dev/doc/go1.12: *"libSystem is now used when making syscalls on
  Darwin, ensuring forward-compatibility with future versions of macOS and iOS. The switch to libSystem
  triggered additional App Store checks for private API usage."* `verified`.
- **Apple libplatform `sigtramp.c` at `main`**,
  https://raw.githubusercontent.com/apple-oss-distributions/libplatform/main/src/setjmp/generic/sigtramp.c
  (read twice): `#if defined(__DYNAMIC__) && !defined(__i386__)` / `OS_NOEXPORT int __in_sigtramp;`;
  `extern int __sigreturn(ucontext_t *, int, uintptr_t);`; *"On these architectures, _sigtramp is
  implemented in assembly to ensure it matches its DWARF unwind information."* (i386, x86_64);
  `_sigtramp(union __sigaction_u __sigaction_u, int sigstyle, int sig, siginfo_t *sinfo, ucontext_t
  *uctx, uintptr_t token)`; `__in_sigtramp = sig;` ... `__in_sigtramp = 0; __sigreturn (uctx, ctxstyle,
  token); __builtin_trap(); /* __sigreturn returning is a fatal error */`; no comment on the token.
  `verified`.
- **Apple libplatform `sigaction.c` at `main`**,
  https://raw.githubusercontent.com/apple-oss-distributions/libplatform/main/src/setjmp/generic/sigaction.c:
  *"Intercept the sigaction syscall and use our signal trampoline"*; *"keep in sync with
  BSD_KERNEL_PRIVATE value in xnu/bsd/sys/signal.h"*, `#define SA_VALIDATE_SIGRETURN_FROM_SIGTRAMP
  0x0400`; `sa.sa_tramp = _sigtramp;`; `sa.sa_flags = nsv->sa_flags |
  SA_VALIDATE_SIGRETURN_FROM_SIGTRAMP;`; `ret = __sigaction(sig, sap, osv);`. `verified`.
- **xnu `bsd/dev/arm/unix_signal.c` at `main`**,
  https://raw.githubusercontent.com/apple-oss-distributions/xnu/main/bsd/dev/arm/unix_signal.c: *"Generate
  random token value used to validate sigreturn arguments"*, `read_random(&ut->uu_sigreturn_token, ...)`;
  `token = (user64_addr_t)token_uctx ^ (user64_addr_t)ut->uu_sigreturn_token;`; `regs->x[5] = token;`;
  `trampact = SIGTRAMP(p, sig);`, `regs->pc = trampact;`; in `sigreturn`, `if ((user64_addr_t)uap->token
  != token) { ... if (sigreturn_validation != PS_SIGRETURN_VALIDATION_DISABLED) { return EINVAL; } }`.
  `verified`.
- **xnu `bsd/kern/kern_sig.c` at `main`**,
  https://raw.githubusercontent.com/apple-oss-distributions/xnu/main/bsd/kern/kern_sig.c: `uint32_t
  sigreturn_validation = PS_SIGRETURN_VALIDATION_DEFAULT;`; `sigreturn_validation = (__vec.sa_flags &
  SA_VALIDATE_SIGRETURN_FROM_SIGTRAMP) ? PS_SIGRETURN_VALIDATION_ENABLED :
  PS_SIGRETURN_VALIDATION_DISABLED;`; stored by compare-and-swap only while the state is still DEFAULT;
  reset in `execsigs()`; `out->sa_tramp = (user_addr_t)in->sa_tramp;`,
  `machine_thread_function_pointers_convert_from_user(current_thread(), &out->sa_tramp, 1);`,
  `proc_set_sigact_trampact(p, signum, sa->sa_handler, sa->sa_tramp);`. `verified` (that the first install
  fixes the state is the fetch's reading of the compare-and-swap, which the code quoted carries).
- **Syscall user dispatch**, https://docs.kernel.org/admin-guide/syscall-user-dispatch.html: quoted in
  § precedents. `verified`.
- **`seccomp(2)`**, https://man7.org/linux/man-pages/man2/seccomp.2.html (6.19, 2026-06-05): quoted in
  § precedents. `verified`.
- **x86 shadow stack**, https://docs.kernel.org/arch/x86/shstk.html, and **arm64 GCS**,
  https://docs.kernel.org/arch/arm64/gcs.html: quoted in § precedents. `verified`.

### Question 3

- **Python C-API, exceptions**, https://docs.python.org/3/c-api/exceptions.html: `PyErr_SetInterrupt`,
  *"Simulate the effect of a SIGINT signal arriving."*; `PyErr_SetInterruptEx`, *"This function can be
  called by C code that sets up its own signal handling and wants Python signal handlers to be invoked as
  expected when an interruption is requested"*, *"This function is async-signal-safe. It can be called
  without an attached thread state and from a C signal handler."*, *"Added in version 3.10."*;
  `PyErr_CheckSignals`, *"Signal handlers are only run in the main thread of the main interpreter."*
  `verified`.
- **Python C-API, system**, https://docs.python.org/3/c-api/sys.html: `PyOS_setsig`, *"Set the signal
  handler for signal i to be h; return the old signal handler. This is a thin wrapper around either
  sigaction() or signal(). Do not call those functions directly!"* `verified`.
- **signal-hook 0.4.6, `low_level::register`**,
  https://docs.rs/signal-hook/latest/signal_hook/low_level/fn.register.html: quoted in § precedents; *"If
  there was a previous signal handler for the given signal, it is chained"*. `verified`.
- **cppreference, C `signal`**, https://en.cppreference.com/w/c/program/signal: the *Signal handler*
  section and references as in § Where the rule is written. `verified`.
- **Go specification**, https://go.dev/ref/spec (*"Language version go1.27 (May 26, 2026)"*), read at
  offsets 0, 100000 and 200000: none of *signal*, *SIGURG*, *interrupt*, *cgo*. `verified`, an absence in
  that page.
- **JNI specification, *JNI Design Overview*, Java SE 21**,
  https://docs.oracle.com/en/java/javase/21/docs/specs/jni/design.html: neither *signal* nor *handler*
  nor *interrupt*; *asynchronous* only of exceptions (`Thread.stop()`). `verified`, an absence in that
  page.
