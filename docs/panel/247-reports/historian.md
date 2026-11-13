# Panel 247, historian

Written 2026-11-13 by the historian seat, advisory, under `docs/panel/247-briefs/historian.md`
and `00-shared.md` (the plain `.md` files; the `.before-critic.md` copies were not read). Every
precedent below is a page fetched on 2026-11-13, its URL beside it, and says only what that page
states; the quotes are in § Log. A line marked `unverified` rests on no page read today and is not
evidence. Panel 246's historian's pages (`docs/panel/246-reports/historian.md`, line 312 and lines
340 to 407, read 2026-11-12) are cited from that report as *246's*, not re-read, except `os/signal`
and POSIX 2.4.3, re-read for sentences 246 did not quote. No build, no run, no paid call, no
GitHub write. This seat has no shell, so no `grep` of the tree was run by it. *Heroes of code* was
not consulted, neither as a route nor as a source.

Status: complete. One file was written outside the repository's root by the fetching tool itself;
it is reported in its own section below and left in place. Corrected before hand-over, the same
day: three lines on ThreadSanitizer said *"every handler"* where its source, as fetched, says the
handler is replaced *"when needed"*; one line of Q4 mixed whose handler Go asks to use
`SA_ONSTACK`. Both are rewritten below.

---

## Where the brief is wrong, or says more than its pages, first

The coordinator's list was written from memory and said so; these are the lines a page read today
does not carry, or carries differently.

1. **Microsoft's `signal` page does not say on which thread SIGABRT, SIGFPE, SIGILL or SIGSEGV
   runs, nor that any setting is per thread.** Read whole
   (https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/signal, 2026-11-13), it names
   the new thread for Ctrl-C alone. 00-shared.md question 9's *"SIGABRT, SIGFPE, SIGILL and SIGSEGV
   on the faulting thread (Microsoft's `signal` page, to read)"* has no sentence behind it there:
   `unverified`.
2. **Java's handler thread**: the page read says *"the VM signal dispatch thread"* in a code
   comment; the name *Signal Dispatcher* is not on it, `unverified`. **No deprecation of
   `sun.misc.Signal`** is in its source (`@since 1.2`, no `@Deprecated`, in the `jdk.unsupported`
   module by its path); JEP 260 answered 403 and is not cited.
3. **Erlang's `os:set_signal`, listed under S**: by 246's verified page it *"will notify
   erl_signal_server"*, a process of the runtime, which is E's shape (a runtime-owned executor), not
   S's; and it takes no SIGINT (246's finding).
4. **Ruby refuses more than a Mutex**: its 2.0.0 NEWS refuses five Mutex methods in a trap handler
   (`ThreadError`) **and** refuses to trap SEGV, BUS, ILL, FPE and VTALRM at all (`ArgumentError`);
   and the Mutex refusal is contested, bug 19473 open since 2023-03-02.
5. **Swift, Kotlin/Native, Zig**: no page read states a rule for a handler. Kotlin's C-interop page
   says nothing of signals; Zig's search returned forum posts only (not read, not cited); Swift's
   one thread read is individuals' posts (not cited, § search). Swift's precedent here is Apple's
   Dispatch, which is libdispatch's documentation, not Swift's.
6. **jemalloc and glibc as detectors of re-entrance**: no page read says either detects a call
   from a handler. glibc's manual says `malloc` is not reentrant and marks `snprintf` *"AS-Unsafe
   heap"*; jemalloc's search returned results about internal re-entrance (mirrors of `tsd.c`, not
   read), nothing on signals. Both: `unverified` as detectors.
7. **OCaml's "what OCaml 5 changed"**: the trunk runtime says *"We interrupt all domains when a
   signal arrives"*; no page read dates that to 5.0. The polling history read is 4.10, 4.12 and
   4.13 (§ precedents).
8. **One shape the brief does not list, found in a precedent** (CL-057): ThreadSanitizer selects
   handlers by **interposing the registration calls themselves**: its `sigaction_impl` and
   `signal_impl` put a program's handler behind TSan's own `sighandler` (the fetch's words: it
   *"replaces the handler with `sighandler` when needed"*; the source's comment: *"if we call
   REAL(sigaction) directly for any reason without proxying the signal handler through sighandler,
   very bad things will happen."*), neither by the callee's name at build (A) nor by a stack walk at
   run time (C, F). Whether Heroes' runtime could interpose `sigaction` and `signal` for the
   program's own C, which would see `wrap`'s handler, is a question for the ffi-pragmatist and
   compiler-engineer seats; unrun.

---

## verdict

**approve (advisory)** a **deferring** route as the way R4 is honoured, S or E with F's run-time
selection where a name cannot see the handler, **with D kept as the backstop** and a door the
program's own C handler can call; **object (advisory)** to **A as R4's only enforcement**, and to
**D alone, H and I** as resolutions (each leaves Heroes code inside the signal, which changes R4,
the author's call).

| Route | Precedent on the pages read (2026-11-13 unless 246's) | Reading, advisory |
|---|---|---|
| S | Perl moved its handlers out of the signal at 5.8.0; Python (246's), Ruby, Tcl, Lua and OCaml run them at a safe point; TSan queues them per thread | approve: the deepest precedent, and the one with a documented move |
| E | Go, Java, .NET, GNAT, Apple's Dispatch; Haskell, Windows console, .NET `CancelKeyPress` (246's); UCRT's Ctrl-C | approve: the commonest shape among threaded runtimes; it moves the 2496 case off its worker |
| F | ThreadSanitizer puts a program's handlers behind its own, runs synchronous signals at once and defers the rest | approve as the selection that can see `wrap`; TSan selects by interposition, a mechanism 00-shared.md does not list |
| C | Ruby's `ThreadError` for one operation (contested); Go's `"malloc during signal"` assertion; TSan's report; libunwind may answer *"unable to determine"* | approve as backstop and as F's selector, its words written by `write(2)` and ended by `abort` or `_exit` |
| A | clang-tidy and GCC's analyzer select by `signal` and `std::signal`, and neither source matches `sigaction`; Ada refuses a handler's shape at build | object as the only enforcement: the precedents' own hole is `wrap` |
| D | Go (unconditional to `go1.23.0`, compiled out at `go1.24.0` for cost); Darwin's `os_unfair_lock` owner check; SBCL's pseudo-atomic flag | approve as backstop; alone it changes R4 |
| G | Apple's Dispatch counts deliveries; kqueue, `signalfd`, `set_wakeup_fd`, Go's channel (246's); Python's `PyErr_SetInterruptEx` for a C handler | approve as deferral's complement |
| H | clang-tidy and GCC check a handler's body (lints); Ada's bounded error, and its note that blocking *"within a foreign language domain"* is *"unlikely to be detected"* | object as a resolution: changes R4 |
| I | Perl keeps the old way only as `PERL_SIGNALS="unsafe"`, *"despite possible memory corruption"*; no runtime read makes a program's handler safe in place | object: changes R4 |

---

## precedents

### By route

**S, the program's function run later at a safe point of the program's own thread**

- **Perl** (https://perldoc.perl.org/perlipc, 2026-11-13): before 5.8.0 a handler interrupting
  `malloc(3)` or `printf(3)`, or Perl's *"own internal data structures"*, gave *"unpredictable
  behavior--often, a core dump"*; from 5.8.0 *"a flag is set, and the handler returns immediately"*
  and the Perl handler runs *"at strategic "safe" points"*, *"when it is about to execute a new
  opcode"*. Its documented costs: a long opcode delays the handler; *"Perl 5.8.0 and later do _not_
  use SA_RESTART"*, so system calls fail with `EINTR` where they had succeeded; SEGV, ILL, BUS and
  FPE are *"delivered immediately rather than"* deferred; CHLD on some systems *"will not work"*. The
  way back, since 5.8.1: `PERL_SIGNALS="unsafe"`, *"despite possible memory corruption"*. `verified`.
  **This is the precedent the brief says the sitting needs most: a runtime that ran the program's
  code inside the handler and moved it out, for 2494's class (malloc, printf, the runtime's own
  structures).**
- **Python** (246's, https://docs.python.org/3/library/signal.html): handlers *"always executed in
  the main Python thread"*, the C handler *"sets a flag"*, a long C calculation *"may run
  uninterrupted"*. New today, its C API (https://docs.python.org/3/c-api/exceptions.html,
  2026-11-13): `PyErr_CheckSignals` handles interruptions *"whose processing has been delayed until
  it is safe to run Python code"*, *"should be called by long-running C code frequently enough so
  that the response appears immediate to humans"*, and *"Signal handlers are only run in the main
  thread of the main interpreter."* `verified`.
- **Ruby** (https://docs.ruby-lang.org/en/3.4/signals_rdoc.html, 2026-11-13): *"The Ruby VM defers
  Signal.trap callbacks from running until it is safe for its internal data structures,"* but *"it
  does not know when it is safe for data structures in YOUR code."*; the short C handlers *"tell the
  VM to run callbacks registered via Signal.trap"* *"later in the main Ruby Thread."*; *"Mutex#lock,
  Mutex#synchronize and any code using them are explicitly unsafe."*; *"Creating a new Thread via
  Thread.new/Thread.start can used to get around the unusability of Mutexes"* *"inside a signal
  handler."* `verified`. **S's own residue, documented**: deferral protects the runtime and leaves
  the program's own data reentrant, which Ruby answered by refusing the Mutex (below, C).
- **Tcl** (https://www.tcl-lang.org/man/tcl8.6/TclLib/Async.htm, 2026-11-13): the handler should
  *"set a flag indicating that the event occurred, then handle the event later"*; Tcl runs it
  *"later, when the world is in a safe state"*, checking *"after each command is evaluated"*.
  `verified`.
- **Lua's stand-alone interpreter** (https://raw.githubusercontent.com/lua/lua/master/lua.c,
  2026-11-13): *"Because a C signal cannot just change a Lua state (as there is no proper
  synchronization), this function only sets a hook"*. `verified`.
- **OCaml** (https://ocaml.org/manual/4.13/notes/Changes and
  https://raw.githubusercontent.com/ocaml/ocaml/trunk/runtime/signals.c, 2026-11-13): signals
  recorded and processed at polling points; the polls were moved three times (§ What each
  corrected); *"The thread executing a POSIX signal handler is not necessarily the most ready to
  execute the corresponding OCaml signal handler."* `verified`.
- **ThreadSanitizer**, a tool and not a language, the one precedent read that keeps a deferred
  handler on the thread that received the signal (§ F below; that it is that thread is my reading
  of the code, `sctx->pending_signals[sig]` drained by `ProcessPendingSignals(thr)`).

**E, the program's function run on a thread the runtime owns**

- **Go** (https://raw.githubusercontent.com/golang/go/master/src/runtime/sigqueue.go, 2026-11-13):
  the handler *"cannot block, allocate memory, or use locks"*, *"so the handler communicates with a
  processing goroutine"*; pending signals are a bit mask (*"signal already in queue"*), so a
  repeated delivery is not counted (my reading of the code). `verified`. 246's: `os/signal`,
  `Notify` *"will not block sending to c"*.
- **Java** (https://raw.githubusercontent.com/openjdk/jdk/master/src/java.base/share/classes/jdk/internal/misc/Signal.java,
  2026-11-13): `dispatch` starts `new Thread(null, runnable, sig + " handler", 0, false)`; *"There
  is no way to force the Java signal handler to run synchronously before the C signal handler
  returns."* `verified`.
- **.NET** (https://raw.githubusercontent.com/dotnet/runtime/main/src/native/libs/System.Native/pal_signal.c
  and https://raw.githubusercontent.com/dotnet/runtime/main/src/libraries/System.Private.CoreLib/src/System/Runtime/InteropServices/PosixSignalRegistration.Unix.cs,
  2026-11-13): *"We can't do anything interesting in the signal handler, so we instead send a
  message to another thread"*, one byte down a pipe; the managed callback, *"called on the native
  signal handling thread"*, moves again *"so signal handling is not blocked"*, to a background
  thread named `".NET Signal Handler"` for SIGINT, SIGQUIT and SIGTERM *"in case the ThreadPool is
  saturated"*, and to the thread pool for the rest. `verified`. 246's: `CancelKeyPress` *"executed
  on a thread pool thread"*.
- **Ada, GNAT's runtime** (https://raw.githubusercontent.com/gcc-mirror/gcc/master/gcc/ada/libgnarl/s-interr.adb,
  2026-11-13): interrupts *"masked at all times in all tasks/threads except possibly for the
  Interrupt_Manager task"*, each *"caught using "sigwait" by the corresponding Server_Task"*.
  `verified`.
- **Apple's Dispatch** (https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/GCDWorkQueues/GCDWorkQueues.html,
  2026-11-13): signal sources *"executed asynchronously on a dispatch queue"*, *"there are no
  restrictions on the functions you can call"*, at the price of *"some increased latency"*; they
  *"are not a replacement for the synchronous signal handlers you install using the sigaction
  function"* and cannot monitor *"SIGILL, SIGBUS, and SIGSEGV"*. `verified`.
- **Windows**: UCRT, *"When a CTRL+C interrupt occurs, Win32 operating systems generate a new thread
  to specifically handle that interrupt"* (the `signal` page, 2026-11-13, `verified`); console
  handlers on *"a new thread"* (246's `HandlerRoutine`, R4's own prior art). **Haskell** `unix`, *"in
  a new thread"* (246's).

**F and C, a refusal or a deferral chosen at run time**

- **ThreadSanitizer** (https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/tsan/rtl/tsan_interceptors_posix.cpp,
  both parts read, 2026-11-13): the handlers a program installs through `sigaction` or `signal` are
  put behind TSan's own `sighandler` (*"if we call REAL(sigaction) directly for any reason without
  proxying the signal handler through sighandler, very bad things will happen."*); a synchronous
  signal, or one arriving while the thread is in a blocking call, runs the handler at once (*"If we
  are in blocking function, we can safely process it now"*); any other is armed in
  `pending_signals` and run by `ProcessPendingSignals` when the thread leaves an interceptor;
  `is_sync_signal` names `SIGSEGV || SIGBUS || SIGILL || SIGTRAP ||` and more the fetch cut. Its
  test (https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/test/tsan/signal_malloc.cpp)
  pins *"WARNING: ThreadSanitizer: signal-unsafe call inside of a signal"* for `malloc` and `free`
  in a SIGPROF handler. The code that makes that report is not in the file read. `verified` for
  what is quoted.
- **Ruby 2.0.0** (https://raw.githubusercontent.com/ruby/ruby/v2_0_0_0/NEWS, 2026-11-13): *"Mutex#lock,
  Mutex#unlock, Mutex#try_lock, Mutex#synchronize and Mutex#sleep are no longer allowed to be used
  from trap handler and raise a ThreadError in such case."* `verified`. Contested: bug 19473
  (https://bugs.ruby-lang.org/issues/19473), open, its three options (remove the check, a trap-safe
  opt-in, run traps on a separate thread) unchosen on the page. `verified`.
- **Go's own allocator**, refusing a call on the signal stack (§ D).
- **libunwind**, recognising the delivery's frame
  (https://raw.githubusercontent.com/libunwind/libunwind/master/doc/unw_is_signal_frame.tex,
  2026-11-13): `unw_is_signal_frame()` *"returns a positive value if the current frame identified
  by cp is a signal frame,"* *"also known as a signal trampoline"*, frames *"normally created by
  the kernel when delivering a signal"*; or `-UNW_ENOINFO`, *"Libunwind is unable to determine
  whether or not the current frame is a signal frame."* `verified`. **The precedent for C's
  detection documents a "cannot tell" answer.**
- **The failure path inside a handler**: .NET's handler, failing to write its byte, calls `abort()`
  (pal_signal.c); Go's `sigsend`, not sending, *"the caller typically crashes the program"*; GCC's
  analyzer names `_exit` as *"a possible signal-safe alternative"* to `exit`. None of the in-handler
  failure paths read formats a message (pages read, not a survey). `verified` for each.

**A and H, a refusal at build**

- **clang-tidy `bugprone-signal-handler`**
  (https://clang.llvm.org/extra/clang-tidy/checks/bugprone/signal-handler.html and its source
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang-tools-extra/clang-tidy/bugprone/SignalHandlerCheck.cpp,
  2026-11-13): checks a handler's calls against a list (CERT SIG30-C), *"user-defined functions with
  visible definitions are checked recursively"*, unknown external functions assumed unsafe; the
  handler is found by `functionDecl(hasAnyName("::signal", "::std::signal"), parameterCountIs(2),
  isStandard())`, and `sigaction` is matched nowhere in the file. `verified`.
- **GCC `-fanalyzer`** (https://gcc.gnu.org/onlinedocs/gcc/Static-Analyzer-Options.html and
  https://raw.githubusercontent.com/gcc-mirror/gcc/master/gcc/analyzer/sm-signal.cc, 2026-11-13):
  *"call to %qD from within signal handler"*, CWE-479; registration recognised by
  `is_named_call_p (callee_fndecl, "signal", *call, 2)` or its `std` form, `sigaction` nowhere in
  the file; eleven unsafe functions listed under *"TODO: populate this list more fully"*.
  `verified`. **Both shipped tools have route A's selection and route A's hole.**
- **Ada** (https://www.adaic.org/resources/add_content/standards/22rm/html/RM-C-3-1.html,
  RM-9-5-1.html and RM-H-5.html, 2026-11-13): an interrupt handler is a parameterless protected
  procedure at library level (legality, at build; 6.1/3 as the fetch paraphrased it); inside a
  protected action a potentially blocking operation is a bounded error, *"If the bounded error is
  detected, Program_Error is raised"*, and pragma `Detect_Blocking` requires detection; *"An
  operation that causes a task to be blocked within a foreign language domain is not defined to be
  potentially blocking"* *"and is unlikely to be detected."* `verified`.
- **Rust `signal-hook` 0.4.6** (https://docs.rs/signal-hook/latest/signal_hook/low_level/fn.register.html,
  2026-11-13): `pub unsafe fn register`, unsafe *"because the `action` is run inside a signal
  handler"*; the build refuses a call outside `unsafe`, and the body is the programmer's contract.
  `verified`.

**D, a guard in the allocator**

- **Go** (malloc.go at three tags and on `master`, 2026-11-13): `mallocgc` threw `"malloc deadlock"`
  when `mp.mallocing != 0` and `"malloc during signal"` when `mp.gsignal == getg()`,
  unconditionally, at `go1.21.0` and `go1.23.0`; at `go1.24.0` and on `master` the same checks
  stand inside `if doubleCheckMalloc {`, `const doubleCheckMalloc = false`, because *"They account
  for a few % of mallocgc's cost though, which does matter somewhat at scale."* `verified`.
- **Darwin libplatform** (https://raw.githubusercontent.com/apple-oss-distributions/libplatform/main/src/os/lock.c,
  2026-11-13): `_os_unfair_lock_lock_slow` calls `_os_unfair_lock_recursive_abort` when the lock's
  owner is the calling thread, `__LIBPLATFORM_CLIENT_CRASH__(owner, "Trying to recursively lock an
  os_unfair_lock")`: the death the storm met (00-shared.md's `asi`), raised on purpose by this
  check. `verified`.
- **SBCL** (https://sbcl.org/sbcl-internals/The-deferral-mechanism.html, 2026-11-13): allocation in
  *"pseudo atomic sections"* marked by *"a simple flag"*, since blocking signals per allocation
  would cost *"two `sigsetmask` system calls"*; a deferrable signal inside one is stored and *"On
  leaving the pseudo atomic section, the pending handler is run"*. `verified`. **D's deferring twin,
  and S's safe point at the allocator's exit, reached by SBCL.** Where SBCL's Lisp handlers run
  otherwise, its pages do not say.

**G, a door the runtime owns**

- **Apple's Dispatch**: a signal source *"reports how many total signals have been delivered since
  the last invocation of the event handler"* (the guide above). `verified`. 246's: kqueue's
  `EVFILT_SIGNAL` count, `signalfd`, `set_wakeup_fd`, Go's channel, libuv.
- **Python's door for a program's own C handler** (https://docs.python.org/3/c-api/exceptions.html,
  2026-11-13): `PyErr_SetInterruptEx` *"can be called by C code that sets up its own signal handling
  and wants Python signal handlers to be invoked as expected"*; *"This function is async-signal-safe.
  It can be called without an attached thread state and from a C signal handler."*; *"Added in
  version 3.10."* `verified`. **F's case, a Heroes function wanted by the program's own C handler,
  has this precedent: the C handler gets an async-signal-safe door into the deferral, not a call.**

**I, the handler made safe in place**

- **Perl** keeps it only as an opt-out, `PERL_SIGNALS="unsafe"`, *"despite possible memory
  corruption"* (above). **Rust**'s way in is `unsafe fn`. **Go's runtime** runs its own Go code on
  the signal stack under restrictions: *"System and signal stacks cannot grow"*, `//go:nosplit` for
  functions *"entered from C code such as cgo callbacks or the signal handler"*
  (https://raw.githubusercontent.com/golang/go/master/src/runtime/HACKING.md, 2026-11-13); that is
  the runtime's code, not a program's. **SBCL**: *"signal handlers must invoke only reentrant
  functions or async signal safe functions"*
  (https://sbcl.org/sbcl-internals/Programming-with-signal-handling-in-mind.html, 2026-11-13). No
  page read describes a runtime that makes a program's own handler safe in place. `verified` for
  each quote.

**R4 itself, in another language: Go's answer to 2494's exact shape**

- **Go issue 35814** (https://github.com/golang/go/issues/35814, 2026-11-13), opened 2019-11-25,
  Go 1.13.4 darwin/amd64: *"runtime: "fatal: morestack on g0" and blocking forever when go export
  functions is called in any signal handlers registered by the non-Go code"*; Ian Lance Taylor, the
  same day: *"I'm sorry, this will never work. A signal handler can only call async-signal-safe
  functions."*, *"Go code will never be async-signal-safe. That would be nearly impossible to
  implement, and it is not a goal."*, *"I recommend that you open a pipe, read from the pipe on the
  Go side, and in the signal handler write a byte to the pipe."*; closed as completed. No comment
  on the page says a refusal or a document followed. `verified`. **Go ruled R4 and built nothing to
  enforce it; the program that breaks it dies with "morestack on g0" or blocks.**

### What each corrected

- **Perl**: 5.8.0, handlers moved out of the signal; 5.8.1, `PERL_SIGNALS` added to get the old
  behaviour back; SA_RESTART dropped so deferred signals arrive promptly. The page gives no dates.
- **Ruby**: 2.0.0, five Mutex methods refused in a trap (`ThreadError`) and five signals refused to
  `trap` (`ArgumentError`); since 2023-03-02 the Mutex refusal is argued in bug 19473, open, last
  note 2025-07-15.
- **OCaml**: 4.10.0, bytecode polls at every minor allocation; 4.12.0, *"avoid running its OCaml
  handler in the middle of a blocking section"*, a poll removed from `caml_leave_blocking_section`
  and one added to `caml_raise`; 4.13.0, #10039, *"Add poll points to native generated code"*, and
  #9876, *"more reliable, at the cost of a small slowdown."* The changelog gives no dates.
- **Go**: the allocator's signal check unconditional through `go1.23.0`, compiled out from
  `go1.24.0` for cost; issue 35814 (2019) closed with an explanation. The commit is not read.
- **Java**: no deprecation found in the source (above).
- **.NET**: `PosixSignalRegistration` arrived with 6.0; its issue,
  https://github.com/dotnet/runtime/issues/50527 (2026-11-13), opened 2021-03-31 by David Fowler,
  closed by PR 55333 on 2021-07-09, milestone 6.0.0: *"Many systems use SIGTERM as a way to signal a
  graceful shutdown"*, `AppDomain.ProcessExit` *"is the one that fires when it's too late"*, and
  Stephen Toub, *"you can't run arbitrary managed code in signal handiers"* (sic). `verified` as the
  fetch returned it.

### Where the rule is written

On the pages read, **the language's own reference states the rule in two cases**, both languages
that let a program's function be the handler: **ISO C** (N1570 7.14.1.1: *"the behavior is
undefined if the signal handler refers to any object with static or thread storage duration"* ...
*"or the signal handler calls any function in the standard library other than the abort
function"*, `_Exit`, `quick_exit` or `signal`; p7, *"Use of this function in a multi-threaded
program results in undefined behavior."*, https://port70.net/~nsz/c/c11/n1570.html, 2026-11-13) and
**Ada** (Annex C, 9.5.1, H.5, above); and **POSIX** for the system (2.4.3: *"when a signal
interrupts an unsafe function or function-like macro"* and *"the signal-catching function calls an
unsafe function or function-like macro, the behavior is undefined."*, re-read 2026-11-13). **Every
deferring runtime read states it in its library's or runtime's own documentation**: Python's
`signal` page and C API, Ruby's `signals_rdoc` and NEWS, Perl's `perlipc`, Go's `os/signal` and
runtime source, Java's internal class, .NET's API reference and source, OCaml's `Sys` page and
changelog, Tcl's C API manual, Lua's interpreter source, Apple's Dispatch guide. The language
specifications of Go, Java, Python, C# and Rust were **not searched**, so no sentence here says
they are silent. Heroes emits C11, so 7.14.1.1 binds the emitted program whatever the Heroes spec
says.

### The questions of 00-shared.md a page answers

- **Q3, a refusal async-signal-safe end to end**: the failure paths read inside a handler are
  `write` then `abort` (.NET), a crash (Go's `sigsend`), `_exit` (GCC's suggested alternative);
  POSIX's table holds `_exit()`, `abort()`, `write()` and not `snprintf`, `printf`, `malloc`,
  `free`, `exit` or `localeconv` (as the fetch read it); Apple's `sigaction(2)` the same for
  `printf`, `snprintf`, `malloc`, `free`, `exit`
  (https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/sigaction.2.html,
  2026-11-13). Darwin's SIGTRAP from `abort()` on an exiting thread: no page read.
- **Q4, C's detection**: libunwind documents *"unable to determine"*; Go knows it is on the signal
  stack by the goroutine its runtime switched to (`getg()` returns *"gsignal"*, HACKING.md), and
  asks non-Go code that installs a handler to use `SA_ONSTACK` (*"Failing to do so is likely to
  cause the program to crash if the signal is received."*, `os/signal`, re-read); OCaml: *"In mixed
  C/OCaml applications there is no guarantee that the POSIX signal handler runs in an OCaml
  thread"*; TSan interposes the registration (finding 8 above).
- **Q5, deferral's limits**: faults are not deferred by any precedent read (Perl delivers them
  immediately; TSan runs synchronous signals at once; Ruby and `signal-hook` refuse to take them;
  Dispatch cannot monitor three of them; Go turns them into *"a run-time panic"*; C11 p3 makes
  returning from SIGFPE, SIGILL or SIGSEGV undefined); CHLD-like signals that must act before
  returning break deferral (Perl); latency in long C calls (Perl, Python, OCaml's three moves). On
  `SA_SIGINFO`'s two pointers: no page read says what a deferral keeps of them (Java's handler gets
  a `Signal` object, .NET a context object; neither page speaks of `siginfo_t`).
- **Q6, D's words' remedy, a C handler in the program's own header**: C's world advises exactly a
  flag (*"a handler that does nothing but set an external variable that the program checks
  regularly"*, glibc; *"signal handlers should do little more than set a flag"*, Apple), and the
  runtimes that host such a C handler give it a door (Python's `PyErr_SetInterruptEx`) or a pipe
  (Go's maintainer). The two-module trap panel 246 measured is on no page read.
- **Q7, glibc where Darwin kills**: glibc's manual marks `snprintf` *"AS-Unsafe heap"*, *"only as
  safe as"* the `malloc` family, which *"are not reentrant"*
  (https://sourceware.org/glibc/manual/latest/html_node/Formatted-Output-Functions.html,
  Unsafe-Features.html, Nonreentrancy.html, 2026-11-13). So by glibc's own documentation the
  handler's `snprintf` inside `main`'s is not harmless by design; Linux's 0 of 50 is a sample, and
  whether it corrupts in silence stays unrun.
- **Q9, Windows**: Ctrl-C's new thread is on the page; the other four signals' thread is not
  (finding 1).
- **Q10, D's place**: Go removed its equivalent check from the default build, for *"a few %"*, in a
  runtime where no program code runs in a handler; SBCL keeps its flag on every allocation in a
  runtime whose handlers must be *"reentrant"*. My reading, not either page's: where the adopted
  route sees every handler, Go's history prices D as an assertion; where it cannot (A's or F's
  holes), SBCL's and Darwin's keep a cheap per-thread flag.

---

## argument

Of the runtimes read that say where a program's handler runs, none runs it inside the signal by
default; Rust's and Perl's ways in are marked unsafe. Perl is the evidence most wanted: until 5.8.0
it ran Perl code in the handler, and moved to deferral because malloc, printf and its own
structures were interrupted, 2494's class. Python, Ruby, Tcl, Lua and OCaml defer to a safe point;
Go, Java, .NET, GNAT and Dispatch use a thread they own. Go's maintainer answered 2494's exact
shape: *"this will never work"*, use a pipe. Refusal alone is thin: clang-tidy and GCC select by the
name `signal`, missing `sigaction`. Deferral's documented costs: long C calls, faults, and
program-level reentrancy. (116 words, counted by hand)

---

## condition

- A runtime that ran a program's handler inside the signal behind a body check (H) or a made-safe
  runtime (I), and kept it, would move me on H and I. None on the pages read: Perl moved the other
  way and kept the old mode only as an opt-out *"despite possible memory corruption"*.
- A deferring runtime that went back to running handlers in the signal, for latency, faults or
  CHLD, would weigh against S and E. The pages read document those limits (Perl, Python, OCaml) and
  none went back.
- A refusal by name shown to see a handler installed through a wrapper's `sigaction` would lift my
  objection to A alone; both sources read match `signal` only.
- Go's commit that introduced `doubleCheckMalloc` (unread): if it says a cheaper guard replaced the
  check, rather than cost alone, my reading of D's place changes.
- For the 2496 case: a language runtime that runs a deferred handler on the thread the signal was
  sent to would strengthen S on the interrupted thread; the one precedent read is a tool, TSan, and
  my reading of its code, while OCaml says the POSIX handler's thread *"is not necessarily the most
  ready"* and Python and Ruby use the main thread.

---

## search

**How**: WebSearch to find a primary page, WebFetch to read it; a search result's summary was never
cited. Every page fetched on 2026-11-13; the URLs are in § Log, each read whole or at the offsets
named there.

**Queries (WebSearch)**, in order: *Ruby Signal.trap documentation trap context Mutex can't be
called from trap context*; *OCaml 4.14 safepoints #10039 signals polling points loops Changes*;
*OCaml manual Sys.signal handler executed "polling point" OCaml 5 signals domain*; *dotnet runtime
PosixSignalRegistration.Unix.cs OnPosixSignal dedicated thread ".NET Signal Handler" ThreadPool*;
*"Caveats for implementing Signal.trap callbacks" ruby*; *Apple Concurrency Programming Guide
"Monitoring a Signal" dispatch source "SIGILL, SIGBUS, and SIGSEGV"*; *Kotlin/Native signal handler
staticCFunction signal(SIGINT) Kotlin code inside signal handler runtime not initialized*; *SBCL
internals deferrable signals pseudo-atomic without-interrupts signal handler deferred allocation*;
*Zig std.posix.sigaction signal handler callconv async-signal-safe documentation*;
*"ThreadSanitizer: signal-unsafe call inside of a signal" compiler-rt tsan deferred signal handler*;
*Swift forums Swift code in signal handler not async-signal-safe runtime allocation
DispatchSource.makeSignalSource*; *jemalloc reentrancy signal handler malloc called from signal
handler tsd_reentrancy_level*; *golang cgo exported Go function called from C signal handler "signal
handler" cgo callback not allowed*.

**Languages and sources swept**: Perl, Ruby (NEWS 2.0.0, `Signal`, `signals_rdoc`, bug 19473), Tcl,
Lua, OCaml (changelog, runtime), Go (runtime `sigqueue.go`, `malloc.go` at `go1.21.0`, `go1.23.0`,
`go1.24.0` and `master`, `HACKING.md`, `os/signal`, issue 35814), Java (two `Signal` classes), .NET
(native and managed sources, issue 50527), Ada (RM 2022 C.3.1, 9.5.1, H.5; GNAT `s-interr.adb`),
Rust (`signal-hook` `low_level::register`), clang-tidy (page and source), GCC analyzer (page and
source), POSIX.1-2024 2.4.3, C11 N1570 7.14.1.1, Microsoft UCRT `signal`, glibc manual (three
pages), Darwin libplatform `lock.c`, Apple `sigaction(2)`, Apple's Dispatch guide, SBCL internals
(three pages), ThreadSanitizer (interceptors, test), CPython C API, libunwind, Kotlin's C-interop
page.

**Pages that failed, and what replaced them**: Ruby `doc/NEWS/NEWS-2.0.0.md` and
`doc/signals.rdoc` on `master` (404; the `v2_0_0_0` tag's `NEWS` and the 3.4 rendered page read
instead); https://openjdk.org/jeps/260 (403, not cited); source.dot.net's `PosixSignalRegistration`
(404; GitHub's raw file read); http://www.ada-auth.org (connection refused; adaic.org's mirror
read); gnu.org's glibc manual (302 to sourceware.org, read there); Apple's `GCDEventSources` path
(404; the guide found at `GCDWorkQueues`); N1570 at offsets 0 and 330,000 (7.14 not there; found at
560,000); libunwind's nongnu.org page (429 twice; `.man` 404; the `.tex` read).

**Read and not cited**: the Swift Forums thread https://forums.swift.org/t/signal-handling-in-swift/61351
(individuals' posts, one quote returned reading as the opposite of its sentence); SBCL's
`Signal-handling.html` overview (it says nothing of where handlers run).

**Not searched**: the language specifications of Go, Java, Python, C# and Rust; Haskell's GHC RTS
source; Node beyond 246's page; Nim beyond 246's finding; Emacs; Erlang beyond 246's page; Oberon,
Pascal-P4, Cyclone, Hylo, Idris, K/APL and cfront (no signal facility known to this seat, an
assumption, not a finding); Microsoft's UCRT source for per-thread signal actions; glibc's source
for whether `snprintf` allocates for an integer; Go's commit introducing `doubleCheckMalloc`; the
definition of `__LIBPLATFORM_CLIENT_CRASH__`; CERT SIG30-C's own page; the LLVM *SignalSanitizer*
proposal and .NET 10's SIGTERM page (both seen in search results only); POSIX's own `signal()` page.

**Not run**: a grep of `docs/panel/` for earlier historian lines on these names (no shell). Before
any line below is filed, the synthesis runs `grep -n -i
'perlipc\|PERL_SIGNALS\|signals_rdoc\|doubleCheckMalloc\|35814\|Tcl_AsyncMark\|pseudo.atomic\|bugprone-signal-handler\|unsafe-call-within-signal-handler\|PyErr_SetInterruptEx'
docs/panel/*.md`.

---

## Candidate prior-art lines, per route

In `.claude/rules/records.md`'s two forms, each only if the sitting adopts the route it names, the
historian marked the page `verified` above, and the session filing it re-reads the page. A link to
another project's issue goes through `redirect.github.com` (§ The issues).

**S adopted**

- `- **Taken from Perl**: [perlipc, Deferred Signals (Safe Signals)](https://perldoc.perl.org/perlipc): before Perl 5.8.0 a signal handler calling a function such as malloc(3) or printf(3) that the signal had interrupted, or interrupting Perl's own internal data structures, could give unpredictable behaviour, often a core dump; from 5.8.0 the C handler sets a flag and the Perl handler runs at safe points, such as before a new opcode.`
- `- **Reached first by Ruby**: [Caveats for implementing Signal.trap callbacks](https://docs.ruby-lang.org/en/3.4/signals_rdoc.html): short C handlers tell the VM to run Signal.trap callbacks later in the main Ruby Thread, and the VM does not know when the program's own data structures are safe.`
- poll points in loops: `- **Reached first by OCaml**: [Changes](https://ocaml.org/manual/4.13/notes/Changes): OCaml 4.13.0 added poll points to native generated code (#10039), and 4.12.0 avoids running a signal's OCaml handler in the middle of a blocking section.`
- a long C call: `- **Reached first by Python**: [PyErr_CheckSignals](https://docs.python.org/3/c-api/exceptions.html): signal handling delayed until it is safe to run Python code is performed by this function, which long-running C code should call frequently, and signal handlers run only in the main thread of the main interpreter.`
- faults not deferred: `- **Reached first by Perl**: [perlipc](https://perldoc.perl.org/perlipc): SEGV, ILL, BUS and FPE are delivered immediately rather than deferred, there being little a Perl-level handler can do with them.`
- faults refused: `- **Reached first by Ruby**: [NEWS for 2.0.0](https://raw.githubusercontent.com/ruby/ruby/v2_0_0_0/NEWS): Signal.trap raises ArgumentError for SEGV, BUS, ILL, FPE and VTALRM.`

**E adopted**

- `- **Taken from Go**: [runtime/sigqueue.go](https://raw.githubusercontent.com/golang/go/master/src/runtime/sigqueue.go): the signal handler cannot block, allocate memory or use locks, so it communicates with a processing goroutine.`
- `- **Reached first by Java**: [jdk.internal.misc.Signal](https://raw.githubusercontent.com/openjdk/jdk/master/src/java.base/share/classes/jdk/internal/misc/Signal.java): the VM runs a Java signal handler on a new thread named after the signal, and there is no way to force it to run before the C signal handler returns.`
- `- **Reached first by .NET**: [pal_signal.c](https://raw.githubusercontent.com/dotnet/runtime/main/src/native/libs/System.Native/pal_signal.c): handlers whose work is not signal-safe write the signal to a pipe that a runtime thread reads, since nothing interesting can be done in the signal handler.`
- `- **Reached first by GNAT**: [s-interr.adb](https://raw.githubusercontent.com/gcc-mirror/gcc/master/gcc/ada/libgnarl/s-interr.adb): user-handleable interrupts are masked in all tasks and each is caught with sigwait by its own Server_Task.`
- `- **Reached first by Apple**: [Dispatch Sources](https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/GCDWorkQueues/GCDWorkQueues.html): a signal dispatch source's handler runs asynchronously on a dispatch queue with no restriction on the functions it calls, at some cost in latency, and cannot monitor SIGILL, SIGBUS or SIGSEGV.`

**R4 kept, any route** (beside 2497's `Taken from Microsoft`)

- `- **Reached first by Go**: [issue 35814](https://redirect.github.com/golang/go/issues/35814): exported Go functions called from signal handlers installed by non-Go code fail, a signal handler can only call async-signal-safe functions and Go code will never be, and a byte written down a pipe is recommended instead.`

**F or C adopted**

- `- **Reached first by ThreadSanitizer**: [tsan_interceptors_posix.cpp](https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/tsan/rtl/tsan_interceptors_posix.cpp): a handler installed through sigaction or signal is proxied through TSan's own signal handler; a synchronous signal, or one arriving during a blocking call, runs the handler at once, and any other is kept pending and run when the thread leaves an interceptor.`
- `- **Reached first by Ruby**: [NEWS for 2.0.0](https://raw.githubusercontent.com/ruby/ruby/v2_0_0_0/NEWS): Mutex#lock, #unlock, #try_lock, #synchronize and #sleep raise ThreadError when used from a trap handler.`
- C's detection: `- **Reached first by libunwind**: [unw_is_signal_frame](https://raw.githubusercontent.com/libunwind/libunwind/master/doc/unw_is_signal_frame.tex): a frame created by the kernel when delivering a signal, a signal trampoline, is recognised during an unwind, or the call reports that it cannot determine it.`
- a C handler's door: `- **Reached first by Python**: [PyErr_SetInterruptEx](https://docs.python.org/3/c-api/exceptions.html): an async-signal-safe function, added in 3.10, that C code with its own signal handling calls so that the Python handler runs at the next PyErr_CheckSignals.`

**A or H adopted**

- `- **Reached first by clang-tidy**: [bugprone-signal-handler](https://clang.llvm.org/extra/clang-tidy/checks/bugprone/signal-handler.html): a signal handler's calls are checked against a set of asynchronous-safe functions, user functions with visible definitions recursively, after CERT SIG30-C.`
- `- **Reached first by GCC**: [Static Analyzer Options](https://gcc.gnu.org/onlinedocs/gcc/Static-Analyzer-Options.html): -fanalyzer warns for paths on which a function known to be async-signal-unsafe, such as fprintf, is called from a signal handler (CWE-479).`
- `- **Reached first by Ada**: [ARM 2022 C.3.1](https://www.adaic.org/resources/add_content/standards/22rm/html/RM-C-3-1.html): an interrupt handler is a parameterless protected procedure declared at library level, and attaching it to a reserved interrupt raises Program_Error.`

**D kept**

- `- **Reached first by Go**: [runtime/malloc.go at go1.23.0](https://raw.githubusercontent.com/golang/go/go1.23.0/src/runtime/malloc.go): mallocgc throws "malloc deadlock" when re-entered on its thread and "malloc during signal" when called on the signal stack's goroutine.`
- D's cost: `- **Reached first by Go**: [runtime/malloc.go at go1.24.0](https://raw.githubusercontent.com/golang/go/go1.24.0/src/runtime/malloc.go): those checks stand behind doubleCheckMalloc, set to false, as they account for a few percent of mallocgc's cost.`
- `- **Reached first by SBCL**: [The deferral mechanism](https://sbcl.org/sbcl-internals/The-deferral-mechanism.html): allocation runs in pseudo-atomic sections marked by a flag, blocking signals around each allocation costing two sigsetmask calls, and a deferrable signal arriving inside one is stored and its handler run on leaving the section.`
- `- **Reached first by Apple**: [libplatform lock.c](https://raw.githubusercontent.com/apple-oss-distributions/libplatform/main/src/os/lock.c): an os_unfair_lock whose owner is the calling thread aborts the process with "Trying to recursively lock an os_unfair_lock".`

**Not candidate lines** (for the sitting's file only): C11 7.14.1.1 and POSIX 2.4.3, which state the
rule Heroes' emitted C is bound by rather than a decision another project made; glibc's `snprintf`
annotation (Q7's answer); Ruby bug 19473 (a debate, not a decision); Perl's `PERL_SIGNALS`
opt-out, which this seat does not recommend copying.

---

## One thing written outside the repository's root, reported

At the OCaml changelog fetch (https://ocaml.org/manual/4.13/notes/Changes, served as
`application/octet-stream`), WebFetch saved a binary copy of the page, by the tool's own behaviour,
to
`/Users/joseph/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang/15c2275d-d1e8-47ba-94cd-49bc93c10427/tool-results/webfetch-1794541514309-g2tlua.bin`
(467.3 KB). It is outside the repository's root (CLAUDE.md § Hard stops), as panel 246's historian
recorded of a PDF. I did not remove it: removal is a destructive act, the coordinator's or the
author's to ask. After it, this seat fetched text sources (`raw.githubusercontent.com`) wherever one
existed; a session re-reading the OCaml line may prefer the changelog in OCaml's repository (not
read here).

---

## Log, as I went (every page read 2026-11-13)

### First batch: a runtime that ran program code inside the handler, and moved it out

- **Perl, `perlipc`, "Deferred Signals (Safe Signals)"**, https://perldoc.perl.org/perlipc:
  *"Before Perl 5.8.0, installing Perl code to deal with signals exposed you to danger from two
  things. First, few system library functions are re-entrant. If the signal interrupts while Perl is
  executing one function (like malloc(3) or printf(3)), and your signal handler then calls the same
  function again, you could get unpredictable behavior--often, a core dump. Second, Perl isn't
  itself re-entrant at the lowest levels. If the signal interrupts Perl while Perl is changing its
  own internal data structures, similarly unpredictable behavior may result."*; *"The pragmatic
  approach was to say "I know the risks, but prefer the convenience", and to do anything you wanted
  in your signal handler, and be prepared to clean up core dumps now and again."*; *"Perl 5.8.0 and
  later avoid these problems by "deferring" signals. That is, when the signal is delivered to the
  process by the system (to the C code that implements Perl) a flag is set, and the handler returns
  immediately. Then at strategic "safe" points in the Perl interpreter (e.g. when it is about to
  execute a new opcode) the flags are checked and the Perl level handler from %SIG is executed."*;
  *"we know the Perl interpreter is in a safe state, and that we are not in a system library
  function when the handler is called."* Its caveats: *"a signal that arrives during a long-running
  opcode (e.g. a regular expression operation on a very large string) will not be seen until the
  current opcode completes."*; *"In order to deliver deferred signals promptly, Perl 5.8.0 and later
  do _not_ use SA_RESTART. Consequently, restartable system calls can fail (with $! set to `EINTR`)
  in places where they previously would have succeeded."*; faults: *"Certain signals like SEGV, ILL,
  BUS and FPE are generated by virtual memory addressing errors and similar "faults". These are
  normally fatal: there is little a Perl-level handler can do with them. So Perl delivers them
  immediately rather than attempting to defer them."*; CHLD: *"On such systems the deferred signal
  scheme will not work for those signals: it does not do the `wait`."*; the way back: *"If you want
  the old signal behavior back despite possible memory corruption, set the environment variable
  `PERL_SIGNALS` to `"unsafe"`. This feature first appeared in Perl 5.8.1."* `verified`. The page
  gives no release date for 5.8.0.
- **Ruby, NEWS for 2.0.0**, https://raw.githubusercontent.com/ruby/ruby/v2_0_0_0/NEWS (heading *"=
  NEWS for Ruby 2.0.0"*; the path `doc/NEWS/NEWS-2.0.0.md` on `master` is a 404): under `Mutex`,
  *"Mutex#lock, Mutex#unlock, Mutex#try_lock, Mutex#synchronize and Mutex#sleep are no longer
  allowed to be used from trap handler and raise a ThreadError in such case."*; under `Signal`,
  *"Signal.trap raises ArgumentError when :SEGV, :BUS, :ILL, :FPE, :VTALRM"*. `verified`.
- **Ruby, `Signal`**, https://docs.ruby-lang.org/en/master/Signal.html: `Signal.trap`, *"Argument
  `command` or block provided specifies code to be run when the signal is raised."*; the page states
  neither on which thread the block runs nor any restriction on what it may do; the source it shows
  raises *"can't trap reserved signal: SIG%s"*. `verified` for what is quoted.
- **Ruby bug 19473**, https://bugs.ruby-lang.org/issues/19473: *"can't be called from trap context
  (ThreadError) is too limiting"*, filed 2023-03-02 by Benoit Daloze, **Open**, no assignee, last
  note 2025-07-15; Koichi Sasada: *"The current limitation is introduced to protect user from
  dead-lock error"*; Yusuke Endoh: *"`Mutex#lock` is designed to consistently raise an exception
  when called inside a trap handler."*; three options listed by Sasada (remove the check, a trap-safe
  opt-in, or run trap handlers on a separate thread), none chosen on the page. `verified`.
- **Tcl, `Tcl_AsyncCreate(3)`**, https://www.tcl-lang.org/man/tcl8.6/TclLib/Async.htm: when an
  event such as a signal occurs while a script is evaluated, *"it is not safe to take any
  substantive action to process the event"*, nor to evaluate a script *"since the interpreter may
  already be in the middle of evaluating a script"*; the handler should *"set a flag indicating that
  the event occurred, then handle the event later"* by calling `Tcl_AsyncMark`; Tcl calls the proc
  *"later, when the world is in a safe state"*, and *"calls Tcl_AsyncReady after each command is
  evaluated and calls Tcl_AsyncInvoke if needed."*; `Tcl_AsyncMark` *"will only yield the origin
  thread into the asynchronous handler."* `verified`.
- **Lua, the stand-alone interpreter `lua.c`**, https://raw.githubusercontent.com/lua/lua/master/lua.c:
  *"Function to be called at a C signal. Because a C signal cannot just change a Lua state (as
  there is no proper synchronization), this function only sets a hook that, when called, will stop
  the interpreter."*; `laction` restores `SIG_DFL` (*"if another SIGINT happens, terminate
  process"*) and calls `lua_sethook(globalL, lstop, flag, 1)`; `lstop` resets the hook and raises
  `"interrupted!"`. `verified`.
- **OCaml, the changelog**, https://ocaml.org/manual/4.13/notes/Changes: 4.10.0, #8691, #8897,
  #9027 (Jacques-Henri Jourdan): *"in bytecode mode, polling for asynchronous callbacks is performed
  at every minor heap allocation"*, and long-running C functions *"can be replaced with calls to
  caml_process_pending_actions at safe points."*; #8993: the pending actions are *"(requested minor
  and major collections, signal handlers, finalisers, and memprof callbacks)"*; 4.12.0, #1128,
  #7503, #9036, #9722, #10069 (Stephen Dolan): *"When a signal arrives, avoid running its OCaml
  handler in the middle of a blocking section."*, *"A polling point was removed from
  caml_leave_blocking_section, and one added to caml_raise."*; 4.13.0, #10039 *Safepoints* (Sadiq
  Jaffer, Stephen Dolan, Damien Doligez, Xavier Leroy, Anmol Sahoo, Mark Shinwell): *"Add poll
  points to native generated code."*; #9876 (Xavier Leroy): *"making signal handling and minor GC
  triggers more reliable, at the cost of a small slowdown."* `verified`. The fetch returned the
  file truncated; entries before 4.10 were not read. **This fetch wrote a file outside the
  repository's root** (the section above).
- **OCaml runtime, `runtime/signals.c` (trunk)**, https://raw.githubusercontent.com/ocaml/ocaml/trunk/runtime/signals.c:
  `caml_record_signal`, *"Record the delivery of a signal, and arrange for it to be processed as
  soon as possible, by playing with the allocation limit"*; *"We interrupt all domains when a signal
  arrives. This is a strategy that makes as little assumptions as possible about signal-safety,
  threads, and domains."*; actions *"that run OCaml code and may raise OCaml exceptions"* *"can be
  delayed, and do not run during allocations from C."*; *"The thread executing a POSIX signal
  handler is not necessarily the most ready to execute the corresponding OCaml signal handler."*;
  *"In mixed C/OCaml applications there is no guarantee that the POSIX signal handler runs in an
  OCaml thread"*. `verified` (quoted as the fetch returned them, some cut at its limit).

### Second batch: Go's runtime

- **Go, `runtime/sigqueue.go` (master)**, https://raw.githubusercontent.com/golang/go/master/src/runtime/sigqueue.go:
  *"Most synchronization primitives are not available from the signal handler (it cannot block,
  allocate memory, or use locks) so the handler communicates with a processing goroutine via struct
  sig, below."*; `sigsend`: *"sigsend delivers a signal from sighandler to the internal signal
  delivery queue. It reports whether the signal was sent. If not, the caller typically crashes the
  program. It runs from the signal handler, so it's limited in what it can do."* The pending set is
  a bit mask, `mask [(_NSIG + 31) / 32]uint32`, with the inline comment *"signal already in
  queue"*: a signal delivered again before it is received is one bit, not a count (my reading of
  the code the fetch returned). `verified`.
- **Go, `runtime/malloc.go`, four reads**: at `go1.21.0`
  (https://raw.githubusercontent.com/golang/go/go1.21.0/src/runtime/malloc.go) and `go1.23.0`
  (https://raw.githubusercontent.com/golang/go/go1.23.0/src/runtime/malloc.go), `mallocgc` reads,
  unconditionally, `if mp.mallocing != 0 { throw("malloc deadlock") }` and `if mp.gsignal == getg()
  { throw("malloc during signal") }`, then `mp.mallocing = 1`; at `go1.24.0`
  (https://raw.githubusercontent.com/golang/go/go1.24.0/src/runtime/malloc.go) and on `master`
  (https://raw.githubusercontent.com/golang/go/master/src/runtime/malloc.go) the same two checks
  stand in `mallocgcTiny`, `mallocgcSmallNoscan`, `mallocgcSmallScanNoHeader`,
  `mallocgcSmallScanHeader` and `mallocgcLarge`, inside `if doubleCheckMalloc {`, and:
  *"doubleCheckMalloc enables a bunch of extra checks to malloc to double-check that various
  invariants are upheld. We might consider turning these on by default; many of them previously
  were. They account for a few % of mallocgc's cost though, which does matter somewhat at scale."*
  `const doubleCheckMalloc = false`. `verified`. The commit and its reason beyond that comment are
  not read; *"a few %"* is the comment's, for the whole bunch, not a measurement of these two.
- **Go, `runtime/HACKING.md`**, https://raw.githubusercontent.com/golang/go/master/src/runtime/HACKING.md:
  every M has *"on Unix platforms, a *signal stack* (also known as the M's "gsignal" stack)"*;
  *"System and signal stacks cannot grow"*; `getg()` *"when executing on the system or signal
  stacks"* returns *"the current M's "g0" or "gsignal""*; `//go:nosplit` marks functions *"that may
  be entered from C code such as cgo callbacks or the signal handler"*; `go:nowritebarrierrec`
  *"directs the compiler to emit an error if the following function or any function it calls
  recursively, up to a `go:yeswritebarrierrec`, contains a write barrier"*, *"the compiler floods
  the call graph"*. The page does not connect `go:nowritebarrierrec` to signal handlers, and has no
  passage on what a handler may do. `verified` for what is quoted.
- **Go, `os/signal`, re-read** for its cgo sections, which 246 quoted only for the signal mask
  (https://pkg.go.dev/os/signal): *"In general, except as discussed below, Go programs will convert
  a synchronous signal into a run-time panic."*; non-Go handlers *"must use the SA_ONSTACK flag with
  sigaction. Failing to do so is likely to cause the program to crash if the signal is received."*;
  *"If the Go signal handler is invoked on a non-Go thread not running Go code, the handler
  generally forwards the signal to the non-Go code"*: *"the Go handler removes itself, unblocks the
  signal, and raises it again"*, *"to invoke any non-Go handler or default system handler"*; in a
  non-Go program, *"If a synchronous signal arrives while executing non-Go code, the Go runtime will
  invoke the existing signal handler"*. `verified` (quoted as the fetch returned them).

### Third batch: on which thread a managed handler runs (E)

- **Java, `jdk.internal.misc.Signal`**, https://raw.githubusercontent.com/openjdk/jdk/master/src/java.base/share/classes/jdk/internal/misc/Signal.java:
  *"Java code cannot register a handler for signals that are already used by the Java VM
  implementation."*; *"There is no way to force the Java signal handler to run synchronously before
  the C signal handler returns."*; `dispatch`, called by the VM, starts `new Thread(null, runnable,
  sig + " handler", 0, false)`, its comment *"Signal handler will run at maximum priority inherited
  from the VM signal dispatch thread."* `verified`.
- **Java, `sun.misc.Signal`**, https://raw.githubusercontent.com/openjdk/jdk/master/src/jdk.unsupported/share/classes/sun/misc/Signal.java:
  in the `jdk.unsupported` module by its path; `@since 1.2`; no `@Deprecated` in the source the
  fetch returned. JEP 260 (https://openjdk.org/jeps/260) answered 403 and is not cited. `verified`
  for the source.
- **.NET, the native half**, https://raw.githubusercontent.com/dotnet/runtime/main/src/native/libs/System.Native/pal_signal.c:
  *"Entrypoint for the thread that handles signals where our handling isn't signal-safe. Those
  signal handlers write the signal to a pipe, which this loop reads and processes."*; *"We can't do
  anything interesting in the signal handler, so we instead send a message to another thread
  that'll do the handling work."*; the handler's write, `write(g_signalPipe[1], &signalCodeByte, 1)`
  retried on `EINTR`, and `abort()` where it did not write one byte. `verified`.
- **.NET, the managed half**, https://raw.githubusercontent.com/dotnet/runtime/main/src/libraries/System.Private.CoreLib/src/System/Runtime/InteropServices/PosixSignalRegistration.Unix.cs
  (source.dot.net's copy answered 404): `OnPosixSignal`, *"This is called on the native signal
  handling thread. We need to move to another thread so signal handling is not blocked. Otherwise we
  may get deadlocked when the handler depends on work triggered from the signal handling thread."*;
  *"For terminate/interrupt signals we use a dedicated Thread in case the ThreadPool is
  saturated."*: a background thread named `".NET Signal Handler"` for SIGINT, SIGQUIT and SIGTERM,
  `ThreadPool.UnsafeQueueUserWorkItem` for the rest. `verified`.
- **Ada, the reference manual (Ada 2022), C.3.1**, https://www.adaic.org/resources/add_content/standards/22rm/html/RM-C-3-1.html
  (ada-auth.org refused the connection): the `Interrupt_Handler` and `Attach_Handler` aspects are
  for parameterless protected procedures (6.1/3, as the fetch paraphrased it); 7/3, the declaration
  *"shall be a library-level declaration and shall not be declared within a generic body"*; 10/3, at
  attachment *"A check is made that the corresponding interrupt is not reserved."*, *"Program_Error
  is raised if the check fails"*; 13, *"the interrupt is blocked"* during every protected action on
  the object. `verified`.
- **Ada, 9.5.1 and H.5**, https://www.adaic.org/resources/add_content/standards/22rm/html/RM-9-5-1.html,
  https://www.adaic.org/resources/add_content/standards/22rm/html/RM-H-5.html: 8/5, *"During a
  protected action, it is a bounded error to invoke an operation that is potentially blocking (see
  9.5)."*; 17/5, *"If the bounded error is detected, Program_Error is raised."*, else it *"can
  result in deadlock or a (nested) protected action on the same target object."*; H.5 5/5, under
  pragma `Detect_Blocking`, *"An implementation is required to detect a potentially blocking
  operation"* *"and to raise Program_Error"*; 7/5, *"An operation that causes a task to be blocked
  within a foreign language domain is not defined to be potentially blocking"* *"and is unlikely to
  be detected."* `verified`.
- **GNAT, `s-interr.adb`**, https://raw.githubusercontent.com/gcc-mirror/gcc/master/gcc/ada/libgnarl/s-interr.adb:
  *"All user-handleable interrupts are masked at all times in all tasks/threads except possibly for
  the Interrupt_Manager task."*; *"the actual delivery of the Interrupt will be caught using
  "sigwait" by the corresponding Server_Task."*; *"There is no more than one interrupt per
  Server_Task and no more than one Server_Task per interrupt."*; the comments the fetch returned do
  not say why. `verified`.
- **Rust `signal-hook` 0.4.6, `low_level::register`**, https://docs.rs/signal-hook/latest/signal_hook/low_level/fn.register.html:
  `pub unsafe fn register<F>(signal: i32, action: F) -> Result<SigId, Error> where F: Fn() + Sync +
  Send + 'static`; unsafe *"because the `action` is run inside a signal handler"*, with *"similar
  restrictions as specified in C or C++"*; *"the passed handler must not panic"*; *"Note that many
  innocently-looking functions do contain some of the forbidden routines"*; it panics on `FORBIDDEN`,
  SIGKILL, SIGSTOP, SIGILL, SIGFPE and SIGSEGV, the last three *"generally needs very special
  handling to do so correctly"*. `verified` (some sentences as the fetch paraphrased them).

### Fourth batch: a refusal at build, by the callee's name

- **clang-tidy `bugprone-signal-handler`**, https://clang.llvm.org/extra/clang-tidy/checks/bugprone/signal-handler.html:
  *"Finds specific constructs in signal handler functions that can cause undefined behavior."*;
  *"Asynchronous-safety is determined by comparing the function's name against a set of known
  functions."*; *"Any function that cannot be determined to be asynchronous-safe is assumed to be
  non-asynchronous-safe by the check"*; *"Calls to user-defined functions with visible definitions
  are checked recursively."*; it implements CERT SIG30-C; `AsyncSafeFunctionSet` `minimal` or
  `POSIX` (default `POSIX`); *"The check is disabled on C++17 and later."* The page does not say how
  a handler is found. Its source,
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang-tools-extra/clang-tidy/bugprone/SignalHandlerCheck.cpp,
  does: `functionDecl(hasAnyName("::signal", "::std::signal"), parameterCountIs(2), isStandard())`
  and `callExpr(callee(SignalFunction), hasArgument(1, anyOf(HandlerExpr, HandlerLambda)))`;
  `sigaction` is matched nowhere in the file. `verified`.
- **GCC `-fanalyzer`, `-Wanalyzer-unsafe-call-within-signal-handler`**, https://gcc.gnu.org/onlinedocs/gcc/Static-Analyzer-Options.html:
  *"This diagnostic warns for paths through the code in which a function known to be
  async-signal-unsafe (such as `fprintf`) is called from a signal handler."*, CWE-479, enabled by
  `-fanalyzer`. Its source, https://raw.githubusercontent.com/gcc-mirror/gcc/master/gcc/analyzer/sm-signal.cc:
  registration recognised by `is_named_call_p (callee_fndecl, "signal", *call, 2) ||
  is_std_named_call_p (callee_fndecl, "signal", *call, 2)`, the handler an `ADDR_EXPR` of a
  `FUNCTION_DECL`; `sigaction` appears nowhere in the file; the unsafe list is `exit`, `fprintf`,
  `free`, `malloc`, `printf`, `snprintf`, `sprintf`, `vfprintf`, `vprintf`, `vsnprintf`, `vsprintf`,
  under *"TODO: populate this list more fully"*; the message *"call to %qD from within signal
  handler"*, and *"%qs is a possible signal-safe alternative for %qD"* (`_exit` for `exit`).
  `verified`.

### Fifth batch: the standards, and the C libraries' own words

- **POSIX.1-2024, 2.4.3, re-read** for a sentence 246 did not quote
  (https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html, characters 0 to
  100,000): functions behave as defined when called from or interrupted by a handler *"with the
  exception that when a signal interrupts an unsafe function or function-like macro"* and *"the
  signal-catching function calls an unsafe function or function-like macro, the behavior is
  undefined."*; *"All other functions (including generic functions) and function-like macros may be
  unsafe with respect to signals."*; *"It is implementation-defined which additional interfaces, if
  any, are also async-signal-safe."* The fetch found none of `snprintf`, `printf`, `malloc`, `free`,
  `exit`, `localeconv` in the table, and `_exit()`, `abort()` and `quick_exit()` in it. `verified`
  (the first sentence in the fragments the fetch returned).
- **C11, N1570 7.14.1.1** (the HTML rendering at https://port70.net/~nsz/c/c11/n1570.html, read at
  offset 560,000): p3, *"If and when the function returns, if the value of sig is SIGFPE, SIGILL,
  SIGSEGV,"* or another computational exception value, *"the behavior is undefined; otherwise the
  program will resume execution at the point it was interrupted."*; p5, *"the behavior is undefined
  if the signal handler refers to any object with static or thread storage duration"* (that is not
  a lock-free atomic object, other than by assigning to a `volatile sig_atomic_t`) *"or the signal
  handler calls any function in the standard library other than the abort function"*, `_Exit`,
  `quick_exit`, or `signal` for its own signal; p7, *"Use of this function in a multi-threaded
  program results in undefined behavior."*; footnote 252, *"If any signal is generated by an
  asynchronous signal handler, the behavior is undefined."* `verified` for the quoted fragments; the
  words between them, outside the quotes, are the draft's as I recall it and the fetch cut, so
  `unverified` there.
- **Microsoft, `signal` (UCRT)**, https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/signal
  (read whole): *"`SIGINT` is not supported for any Win32 application. When a CTRL+C interrupt
  occurs, Win32 operating systems generate a new thread to specifically handle that interrupt. This
  can cause a single-thread application, such as one in UNIX, to become multithreaded and cause
  unexpected behavior."*; *"your signal-handler function may get control when a run-time operation
  is incomplete and in an unknown state"*; *"Don't issue low-level or STDIO.H I/O routines (for
  example, printf or fread)."*; *"Don't call heap routines or any routine that uses the heap routines
  (for example, malloc, _strdup, or _putenv)."*; *"Don't use any function that generates a system
  call"*; *"Before the specified function is executed, the value of func is set to SIG_DFL."*; *"The
  SIGILL and SIGTERM signals aren't generated under Windows."* The page does not say on which thread
  a SIGABRT, SIGFPE, SIGILL or SIGSEGV handler runs, nor that any setting is per thread. `verified`
  for what is quoted.
- **glibc manual, "Nonreentrancy"**, https://sourceware.org/glibc/manual/latest/html_node/Nonreentrancy.html
  (gnu.org's address redirects there): `malloc` and `free` *"are not reentrant, because they use a
  static data structure which records what memory blocks are free. As a result, no library functions
  that allocate or free memory are reentrant."*; of a handler printing with `fprintf` while the
  program does, the stream *"could be corrupted, because both calls operate on the same data
  structure"*; *"The best practice is to write a handler that does nothing but set an external
  variable that the program checks regularly, and leave all serious work to the program."*
  `verified`.
- **glibc manual, the annotations**: https://sourceware.org/glibc/manual/latest/html_node/Formatted-Output-Functions.html,
  `snprintf` and `sprintf`, *"Preliminary: | MT-Safe locale | AS-Unsafe heap | AC-Unsafe mem |"*;
  `printf` and `fprintf`, *"| MT-Safe locale | AS-Unsafe corrupt heap | AC-Unsafe mem lock corrupt
  |"*. https://sourceware.org/glibc/manual/latest/html_node/Unsafe-Features.html: *"Functions
  marked with `lock` as an AS-Unsafe feature may be interrupted by a signal while holding a
  non-recursive lock."*; *"Functions marked with `heap` may call heap memory management functions
  from the `malloc`/`free` family of functions and are only as safe as those functions."*;
  `corrupt`, *"may corrupt data structures and misbehave when they interrupt, or are interrupted by,
  another such function."* `verified`.
- **Darwin, libplatform `src/os/lock.c`**, https://raw.githubusercontent.com/apple-oss-distributions/libplatform/main/src/os/lock.c:
  `_os_unfair_lock_recursive_abort(os_lock_owner_t owner)` is `__LIBPLATFORM_CLIENT_CRASH__(owner,
  "Trying to recursively lock an " "os_unfair_lock")`, called from `_os_unfair_lock_lock_slow` when
  `OS_ULOCK_IS_OWNER(current, self, allow_anonymous_owner)`, the lock's owner being the calling
  thread; no comment explains it; the recursive variant counts instead of aborting. The macro's
  definition is not in the file. `verified`.
- **Apple, `sigaction(2)`**, https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/sigaction.2.html:
  *"The following functions are either reentrant or not interruptible by signals and are
  async-signal safe."*; *"All functions not in the above lists are considered to be unsafe with
  respect to signals. That is to say, the behaviour of such functions when called from a signal
  handler is undefined. In general though, signal handlers should do little more than set a flag;
  most other actions are not safe."*; `printf`, `snprintf`, `malloc`, `free`, `exit` in no list,
  `_exit()` in Base Interfaces. `verified`.

### Sixth batch: deferral's own words, and its tools

- **Ruby, "Caveats for implementing Signal.trap callbacks"**, https://docs.ruby-lang.org/en/3.4/signals_rdoc.html
  (search *"Caveats for implementing Signal.trap callbacks" ruby*; the `master` raw path is a
  404): *"The Ruby VM defers Signal.trap callbacks from running until it is safe for its internal
  data structures,"* *"it does not know when it is safe for data structures in YOUR code."*; *"These
  short C functions only do enough tell the VM to run callbacks registered via Signal.trap"* *"later
  in the main Ruby Thread."*; *"Mutex#lock, Mutex#synchronize and any code using them are explicitly
  unsafe."*; *"Creating a new Thread via Thread.new/Thread.start can used to get around the
  unusability of Mutexes"* *"inside a signal handler."*; *""thread-safety" does not guarantee
  reentrancy."* `verified` (fragments, verbatim as asked of the fetch).
- **Apple, Concurrency Programming Guide, "Monitoring Signals"**, https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/GCDWorkQueues/GCDWorkQueues.html
  (the `GCDEventSources` path is a 404): *"Signal dispatch sources are not a replacement for the
  synchronous signal handlers you install using the `sigaction` function."*; *"Signal dispatch
  sources allow you to monitor only the arrival of the signal."*; *"Specifically, you cannot use them
  to monitor the `SIGILL`, `SIGBUS`, and `SIGSEGV` signals."*; *"Because signal dispatch sources are
  executed asynchronously on a dispatch queue, they do not suffer from some of the same limitations
  as synchronous signal handlers. For example, there are no restrictions on the functions you can
  call from your signal dispatch source's event handler."*; *"there may be some increased latency
  between the time a signal arrives and the time your dispatch source's event handler is called."*;
  in "About Dispatch Sources", *"a signal-based dispatch source provides information about only the
  most recent signal but also reports how many total signals have been delivered since the last
  invocation of the event handler."* `verified`.
- **SBCL, "The deferral mechanism"**, https://sbcl.org/sbcl-internals/The-deferral-mechanism.html:
  *"Some operations, such as allocation, consist of several steps and temporarily break for
  instance gc invariants."*; *"the overhead of the two `sigsetmask` system calls would be
  enormous."*; *"the pseudo-atomic-interrupted flag is set, the signal and its context are stored,
  and all deferrable signals blocked."*, *"at most one pending handler"*; *"On leaving the pseudo
  atomic section, the pending handler is run and the signals are unblocked."*; `WITHOUT-INTERRUPTS`
  *"defers deferrable signals in its thread until the end of its body"*. `verified`.
- **SBCL, "Programming with signal handling in mind"**, https://sbcl.org/sbcl-internals/Programming-with-signal-handling-in-mind.html:
  *"signal handlers must invoke only reentrant functions or async signal safe functions"*;
  *"Functions passed to `INTERRUPT-THREAD` have the same restrictions and considerations as signal
  handlers."*; *"Recursive locks are not likely to help."*; calling user code from such places is
  *"dangerous and best avoided."* `verified`. SBCL's overview page,
  https://sbcl.org/sbcl-internals/Signal-handling.html, lists four subsections and says nothing of
  where Lisp handlers run.
- **ThreadSanitizer, the test**, https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/test/tsan/signal_malloc.cpp:
  a SIGPROF handler installed with `sigaction` calls `malloc(1)` and `free`; `// CHECK: WARNING:
  ThreadSanitizer: signal-unsafe call inside of a signal`, frames `#0 malloc`, `#0 free`, the
  handler at frame 1 or 2. `verified`.
- **ThreadSanitizer, the interceptors**, https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/tsan/rtl/tsan_interceptors_posix.cpp
  (113,959 characters, read in two parts, 0 to 100,000 and 90,000 to the end): `sigaction_impl`
  and `signal_impl` install a program's handlers, `sigaction_impl` replacing the handler with
  `sighandler` *"when needed"* (the fetch's words, not the source's), and the source's comment *"if
  we call REAL(sigaction) directly for any reason without proxying the signal handler through
  sighandler, very bad things will happen."*; `sighandler`: *"Don't mess with synchronous
  signals."*, `if (sync || atomic_load(&thr->in_blocking_func, ...))`, *"If we are in blocking
  function, we can safely process it now"*, else `sctx->pending_signals[sig]` armed and
  `thr->pending_signals` set; `ProcessPendingSignals(thr)` from the destructors of
  `ScopedInterceptor` and `ScopedSyscall`; `is_sync_signal`: *"If we are sending signal to
  ourselves, we must process it now."*, `sig == SIGSEGV || sig == SIGBUS || sig == SIGILL || sig ==
  SIGTRAP ||` (cut by the fetch); `CallUserSignalHandler` checks errno, *"Ensure that the handler
  does not spoil errno."*; *"When we are in a "blocking call", we process signals asynchronously
  (right when they arrive)."* The `signal-unsafe call` report's code is not in this file as read.
  `verified` for what is quoted.
- **CPython C API**, https://docs.python.org/3/c-api/exceptions.html: `PyErr_CheckSignals`,
  *"Handle external interruptions, such as signals or activating a debugger, whose processing has
  been delayed until it is safe to run Python code and/or raise exceptions."*; *"`PyErr_CheckSignals()`
  should be called by long-running C code frequently enough so that the response appears immediate
  to humans."*; *"Signal handlers are only run in the main thread of the main interpreter."*;
  `PyErr_SetInterruptEx`, *"This function can be called by C code that sets up its own signal
  handling and wants Python signal handlers to be invoked as expected when an interruption is
  requested"*, *"This function is async-signal-safe. It can be called without an attached thread
  state and from a C signal handler."*, *"Added in version 3.10."* `verified`.

### Seventh batch: Go's answer to the exact shape, .NET's reason, and C's detection

- **Go issue 35814**, https://github.com/golang/go/issues/35814: title *"runtime: "fatal: morestack
  on g0" and blocking forever when go export functions is called in any signal handlers registered
  by the non-Go code"*, opened 2019-11-25 by searKing, Go 1.13.4 darwin/amd64, closed as completed the
  same day, locked 2020-11-24, label `FrozenDueToAge`; Ian Lance Taylor, verbatim: *"I'm sorry, this
  will never work. A signal handler can only call async-signal-safe functions."*, *"Go code will
  never be async-signal-safe. That would be nearly impossible to implement, and it is not a goal."*,
  *"I recommend that you open a pipe, read from the pipe on the Go side, and in the signal handler
  write a byte to the pipe."*, *"The write call is async-signal-safe."* No comment says a document
  was updated. `verified`.
- **.NET issue 50527**, https://github.com/dotnet/runtime/issues/50527: *"Make improvements to signal
  handling on .NET applications"*, David Fowler, opened 2021-03-31, closed by PR 55333 on
  2021-07-09, milestone 6.0.0; *"Many systems use SIGTERM as a way to signal a graceful
  shutdown."*; *"To make this work well in container scenarios, we need to start the shutdown
  process and wait for some timeframe."*; `AppDomain.ProcessExit` *"is the one that fires when it's
  too late."*; Stephen Toub: *"There's then a thread processing this queue."*, *"you can't run
  arbitrary managed code in signal handiers"* (sic). `verified` as the fetch returned it.
- **libunwind, `unw_is_signal_frame`**, https://raw.githubusercontent.com/libunwind/libunwind/master/doc/unw_is_signal_frame.tex
  (the nongnu.org page answered 429 twice; `doc/unw_is_signal_frame.man` is a 404):
  `int unw_is_signal_frame(unw_cursor_t *cp);`, *"returns a positive value if the current frame
  identified by cp is a signal frame,"* *"also known as a signal trampoline, and a value of 0
  otherwise."*; *"a signal frame is a frame that was created in response to a potentially
  asynchronous interruption."*; *"For UNIX and UNIX-like platforms, such frames are normally created
  by the kernel when delivering a signal."*; `UNW_ENOINFO`, *"Libunwind is unable to determine
  whether or not the current frame is a signal frame."* `verified`.
- **Kotlin, C interop**, https://kotlinlang.org/docs/native-c-interop.html: *"The function or lambda
  must not capture any values."* (of `staticCFunction`); nothing on signals or on a callback run on
  a thread Kotlin did not create, as the fetch read it. `verified` for the quote; not a precedent.
- **Swift Forums, "Signal handling in Swift"**, https://forums.swift.org/t/signal-handling-in-swift/61351
  (2022-11-10): posts by individuals, one returned as *"There are very few things you can safely do
  in a signal handler, and creating a Task is one of those things."*, which reads as the opposite of
  its own sentence's argument and may be cut; **not cited**.
