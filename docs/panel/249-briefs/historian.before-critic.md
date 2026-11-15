# Panel 249, historian

Read `00-shared.md` first, whole: the three questions, the census's four points (the stack a library's
handler runs on, its thread's signal mask, the signal numbers it may use, the frames its handler finds),
the candidates A, B', C and D, and the facts the coordinator read today from Go 1.27.3's and bdwgc 8.2.8's
sources. You judge precedent and the libraries' documented behaviour, advisory as always: who met each
question, what they built, what they refused, what they had to correct, and **what each census library
says of its own signal use**, at a named version and dated. Your report is
`docs/panel/249-reports/historian.md` in `.claude/worktrees/p249` (make the folder if it is absent), in
your definition's output structure (`.claude/agents/historian.md`, its line 40), **written by you, as you
go** (`SKILL.md` 3c). You build nothing, run nothing and start no paid call.

**Every precedent is a page you read today**, its URL and the day beside it, and says only what the
page states: a comparison is not an adoption, a mechanism the page does not describe is not cited, a
page you cannot read is not cited (`.claude/rules/records.md` § And the record says whose idea it was:
of 90 proposed prior-art lines, 33 said more than their page did). Mark each `verified` or `unverified`.
**A sentence that says a library does not use a signal names the page or the source file, at its
version, where you looked**, or goes out as a question. Your `search` field names the queries and
sources swept. End with candidate prior-art lines per candidate route in the two forms that section
allows, `Taken from` or `Reached first by`.

**Where a fetched page lands**: panel 247's historian found its fetching tool had saved one page outside
the repository's root (`docs/panel/247-reports/historian.md` § One thing written outside the repository's
root, carried); panel 248's saw none (`docs/panel/248-reports/historian.md:15-18`, carried). CLAUDE.md
§ Hard stops writes no file outside the root: if your tool saves a file, say where, and if it is outside
`/Users/joseph/Temp/heroes/heroes-lang`, report it for the author.

**Already read by earlier historians, cite them, do not repeat them** (re-read one only where this
sitting leans on a sentence that report did not quote, and say so): panel 248's
(`docs/panel/248-reports/historian.md`, its § search at `:291-342` lists every page): OpenJDK's
`libjsig` (Java SE 21 *Signal Chaining*, Java SE 26 troubleshooting, `jsig.c`), Android ART's
`sigchain.cc` and `Android.bp` at `main` (*"SA_ONSTACK: always silently enable this"*, `:98-104`),
compiler-rt's `interception.h` and `sanitizer_signal_interceptors.inc`, Apple's `dyld-interposing.h`
and `dyld.cpp` at `dyld-655.1`, Go's `os/signal` (re-read there, `:117-121`), `runtime/netpoll*.go`,
libuv's `async` pages; panel 247's (`docs/panel/247-reports/historian.md`): ThreadSanitizer's
interposition, Go's issue 35814, libunwind's `unw_is_signal_frame`.

The coordinator's list below is **where to look, written from memory and unverified**, except where a
line names a file the coordinator read today (00-shared.md): a project named here that does not do what
the line says is a finding, not an embarrassment.

## Question 1: what each census library documents of its own signal use

For each: which signals it installs, blocks, waits for or sends; on which thread; with which flags
(`SA_ONSTACK` above all); whether its handler calls back into code the program handed it, or walks its
own stack; whether a switch turns it off; and the version and date of the page.

- **Go, a `c-archive` or `c-shared` library**: `os/signal`'s *"Non-Go programs that call Go code"*
  (248 quoted its c-archive paragraph; the coordinator read lines 129-133 and 195-200 of Go 1.27.3's
  `doc.go`); **why Go's preemption signal is SIGURG**: the non-cooperative preemption proposal and its
  design notes (Go 1.14), and what they say of a signal *"unlikely to be used"* by other code, if they say
  it; whether Go documents what a C library's own SIGURG use does to it, or a C library that blocks it.
- **bdwgc**: its documentation of `SIG_SUSPEND`, `SIG_THR_RESTART` and `GC_USESIGRT_SIGNALS` (the
  coordinator read `gc_priv.h:3051`, SIGPWR on Linux, and `pthread_stop_world.c:169-183`, SIGXCPU or
  `SIGRTMIN + 5`, in 8.2.8's tarball); **whether Debian builds `libgc1` with `GC_USESIGRT_SIGNALS`** (its
  packaging's rules file at the `1:8.2.8-1` the Linux image carries); and its Darwin path (Mach's
  `thread_suspend`, by the coordinator's reading of `darwin_stop_world.c`).
- **ART's `libsigchain`**, which cannot be built here: whether its wrapper keeps a per-thread mark of a
  handler running (a *"handling signal"* key or flag), what it does on a `siglongjmp` out, and which
  signals ART claims; a precedent for candidate B' if its source says so.
- **The real-time signals' users**: glibc's own reservation (the man page `signal(7)` or `nptl(7)`, and
  `__libc_current_sigrtmin`; the coordinator ran `SIGRTMIN 34 SIGRTMAX 64` on glibc 2.41); musl's (Go's
  `sigtab_linux_generic.go:44` says *"musl SIGSYNCCALL; see issue 39343"*, read today; musl's own source
  is unread); **.NET on Linux and its use of a real-time signal to suspend threads**, if its runtime's
  source or documentation says so; POSIX timers and AIO (`timer_create`'s `SIGEV_SIGNAL`); any library
  documented to take `SIGRTMIN + n`.
- **SIGURG's users**: Go (above); out-of-band TCP data (`fcntl(F_SETOWN)`, POSIX's or a manual's words);
  any other runtime that takes it.
- **libuv**: `uv_signal_t`'s page and its source at a named release (the Mac has 1.53.0, the image
  1.50.0): the flags its handler is installed with, whether it blocks signals in its own threads, and
  what it says of signals it does not support.
- **OpenSSL**: what its CPU-capability probe does on each platform (a `SIGILL` handler left by
  `siglongjmp`, or `getauxval`, or `sysctl`), at 3.5 and 4.1, from its source; when it installs anything
  else.
- **SDL**: SDL3's and SDL2's handlers for SIGINT and SIGTERM, and `SDL_HINT_NO_SIGNAL_HANDLERS`, from its
  documentation or source at 3.4 and 3.2; and **which real-time signal it takes on Linux and why**: the
  coordinator's `nm -D --undefined-only` of Debian's `libSDL3.so.0` (3.2.10) and `libSDL2-2.0.so.0`
  (2.32.4) printed `__libc_current_sigrtmin`, `__libc_current_sigrtmax` and `sigtimedwait` among their
  imports (00-shared.md).
- **sqlite**: whether the library (not its shell) installs any handler, from its source or documentation.
- **Crash reporters** (Breakpad, Crashpad, or another): the stack their handler runs on, whether they
  walk the interrupted stack and how, and how they chain to the handler they replaced: the census's
  points (1) and (4).
- **The JVM beside native code**: which signals HotSpot takes for itself on Linux and macOS (its
  troubleshooting guide), and what it documents of a native library's handler (248 read `libjsig`; cite
  it).

## Question 2: precedents for each candidate

- **A, a marker the runtime owns**: any runtime that reserves a signal for itself and says how it chose
  it (glibc's NPTL signals, Go's SIGURG, .NET's, bdwgc's), and any that uses a signal's **blocked bit** as
  a mark rather than delivering it. On Darwin, any runtime that faced 31 signals and no real-time range,
  and what it chose.
- **B', a wrapper with a per-thread depth**: ART's `libsigchain` (above), OpenJDK's signal chaining
  (248's; cite), compiler-rt's sanitizers' handler wrappers; how each answers `sigaction(sig, NULL,
  &old)`, `SA_RESETHAND`, and a handler left by `siglongjmp`.
- **C, frame records kept**: which distributions or toolchains turned `-fno-omit-frame-pointer` on by
  default, when and why (Fedora, Ubuntu, others, each with the page of the decision and its date), what
  they measured it cost, and what the x86-64 default is in GCC's and clang's documentation; Apple's arm64
  ABI on frame records (its page), which the coordinator's `objdump` agrees with on arm64 Linux (00-shared.md).
- **D-tramp**: any runtime or tool that hands Darwin's kernel a trampoline of its own through
  `__sigaction`'s `sa_tramp`, or documents that field; Apple's word on `__sigaction`, if any.
- **D-imports**: any toolchain or language that refuses or tells, at build, a program whose bound library
  installs a signal handler; a negative answer says what was searched and where.

## Question 3: who tells a program, and in which words

Who refuses or tells a program whose bound library's handler may run its code in a signal, and how:
Go's `throw("non-Go code set up signal handler without SA_ONSTACK flag")` (the coordinator read it at
Go 1.27.3's `runtime/signal_unix.go:1124`) is a runtime that tells at the first delivery, in words; the
JNI specification or the JVM's documentation on native handlers calling back; Python's
`PyOS_setsig` and `PyErr_SetInterrupt`; Rust's `signal-hook` documentation on what a handler may do; and
**where each rule is written**, the language's reference or the library's page. The spec here names the
rule at its lines 403-404 (00-shared.md): say which precedents put such a rule in the language's
reference and which leave it to a library's page.

## What each corrected

For every precedent above that changed its rule (a signal moved, a switch added, a default turned), the
version, the date, the change and the page that says why.
