# Panel 251 (sitting A of M-rebel-library), historian

Written 2026-11-19 by the historian seat, advisory, under `docs/panel/251-briefs/historian.md` and
`00-shared.md` (both read whole: the shared brief in two parts, lines 1-575 and 575-716). Every
precedent below is a page fetched on 2026-11-19, its URL beside it, and says only what that page
states; the quotes are in § Log. A line marked `unverified` rests on no page read today and is not
evidence. Earlier historians' pages (246, 248, 249) are cited from their reports as *carried*, not
re-read, unless said. This seat has no shell (its tools are Read, Write, WebSearch, WebFetch): it ran no
`grep` of the tree, built nothing, started no paid run, printed no credential and wrote to GitHub
nothing. *Heroes of code* was not consulted, neither as a route nor as a source.

Status: **complete**, 2026-11-19. Corrected by this seat before hand-over, the same day: three
sentences of the first draft said more than their pages (a line on Boehm's collector citing a 249 sweep
this seat never read, *"for years"* of a TSan file whose age was not read, and *"repairs at iOS 14.5"*
for fishhook, whose 14.5 repair was a user's own), one prior-art line named a URL this seat had not
fetched, and one word count was wrong; each is mended below.

**Where a fetched page landed outside the repository's root** (the brief's rule, as panel 247's
historian did): the fetch of P0660R0 (a PDF) saved the binary to
`/Users/joseph/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang/9d1670cb-db5d-4975-9d5b-1db104217e45/tool-results/webfetch-1795079363441-h9atof.pdf`
(198.7 KB), the tool's own act; this seat read it there with Read (pages 1 to 3) and wrote nothing
there. It is left for the author: this seat has no shell to remove it. No other fetch returned a saved
path; every other page came back as text, the long ones read by offset. The one file this seat wrote is
this report.

---

## Where the brief is wrong, or says more than its pages, first

The brief's list was written from memory and said so. These are the lines a page read today does not
carry, or carries differently.

1. **Go's cgo documentation says nothing of a Go function called from a C signal handler.**
   `src/cmd/cgo/doc.go` at go1.27.3 contains none of *signal*, *handler*, *async*. The rule lives on the
   tracker: issue 35814 (2019-11-25), Ian Lance Taylor, *"I'm sorry, this will never work."*, *"A signal
   handler can only call async-signal-safe functions."*, *"Go code will never be async-signal-safe."*
   (247's reading, re-read today). So the answer to *refused, unsupported, or undefined?* is: none of
   the three on a page of Go's documentation; *will never work* on its tracker.
2. **Rust's `JoinHandle` is not `#[must_use]`**, and was refused it: issue 48820 and PR 48830 (2018),
   *"It was decided that `JoinHandle` should not be `#[must_use]`"*. The source carries
   `#[cfg_attr(target_os = "teeos", must_use)]`, one target only.
3. **The C++ change of course is older than P0660, and is a reversal**: P0660R0 (2017-06-19) records
   that an auto-joining thread was voted for C++17 at Jacksonville in 2016 and *"reverted later in Oulu
   in a smaller group"*; `jthread` reached C++20.
4. **Python's page does not say the interpreter joins non-daemon threads**; it says *"The entire Python
   program exits when no alive non-daemon threads are left."* The recent change it records is 3.14's:
   `join()` *"May raise PythonFinalizationError"* on a running daemon thread late in finalization.
5. **Java's structured concurrency is still a preview API in Java SE 25** (*"Since: 21"*); the JEP chain
   the search names (525, 533, 543 final in JDK 28) is `unverified`: `openjdk.org/jeps/525` and `/505`
   answered 403.
6. **Dart does not copy everything**: *"Objects that are identified as immutable (e.g. strings) will be
   shared whereas all other objects will be copied."* (`SendPort.send`). Beside Erlang's refc binaries,
   a second copying system that shares what cannot change.
7. **Erlang has no close of a mailbox, and a send never fails on a pid**: *"Sending a message to a pid
   never fails, even if the pid identifies a non-existing process."* The brief's *"what each says happens
   to a send after the close"* has no Erlang answer but this one.
8. **Ubuntu 26.04 answers 249's open inference**: dpkg 1.23.7ubuntu1 (resolute, 2026-03-31) carries
   *"Turn on the use of frame pointers by default on 64bit architectures."*; glibc 2.43-2ubuntu2.7
   (resolute-security, 2026-11-05) carries no reversal for amd64 or arm64 in the part of its changelog
   read. The build log of that glibc was found and not read (§ Q1 search).
9. **A runtime that records the setjmp family's frames exists and is not a language's**: ThreadSanitizer
   records each buffer's stack pointer and the thread's signal-handler depth, and restores the depth at
   a longjmp. Where it finds no buffer it **aborts** (*"ThreadSanitizer: can't find longjmp buf"*,
   `CHECK(0)`), the opposite of route J's *"keeps every entry"*. ASan intercepts the jumps only, not
   `setjmp`.
10. **"fishhook on current dyld"**: Apple documents nothing about rewriting an image's symbol pointers in
    any page read. What the pages do show is fishhook's own log of repairs for iOS 13 and iOS 15, a user
    finding iOS 14.5 moving the pointers it rewrites, and dyld-1378 (tagged 2026-06-05) mapping read-only
    data with `VM_PROT_TPRO` when the kernel passes `dyld_hw_tpro_pagers`. Which macOS ships dyld-1378 is
    not on the page.

---

## Question 1: a function of the program reached from a signal handler (defect 2522)

### verdict

**approve route J (advisory)**, its one departure from its precedent named; **object (advisory) to
D-rebind's forward compatibility on Darwin being taken as given**; route K stays as the author read it,
no precedent read moves it.

| Route | Precedent on the pages read | Reading, advisory |
|---|---|---|
| **J**, the runtime's save points record each buffer's caller frame; a jump drops deeper entries | **ThreadSanitizer**: `SetJmp` records `buf->sp`, `shadow_stack_pos` and `in_signal_handler`; `LongJmp` finds the buffer *"with matching sp"*, unwinds the shadow stack, restores `in_signal_handler`, garbage-collects buffers below; the setjmp interceptors are assembly that pass the caller's sp and *"tail jump to libc setjmp"*; the real pointers set by `InitializeSetjmpInterceptors()` | **approve**: J's shape ships in a sanitizer at llvm-project's `main` (when it entered is not read). **Departure, to be deliberate**: TSan aborts on a buffer it never saw (*"can't find longjmp buf"*, `CHECK(0)`); J keeps every entry and errs toward R1's told refusal. TSan matches by sp alone; J by frame depth |
| J's hole, the context family | ASan's `swapcontext` interceptor: *"ASan doesn't fully support makecontext/swapcontext functions and may produce false positives in some cases!"* | the one sanitizer read that touches the context family says it is partial |
| J's hole, a jump with no record | TSan refuses (aborts); ASan only calls `__asan_handle_no_return()` before each of `longjmp`, `_longjmp`, `__longjmp_chk`, `siglongjmp` | no precedent read keeps entries on a miss: J's choice is its own |
| **The rule itself** (no Heroes code in a C handler) | POSIX: undefined behaviour for a handler calling a function not on the list; Go *"will never work"* (tracker); Swift *"Neither the Swift runtime nor the code generated by the Swift compiler promise to be signal-safe"* (Greg Parker, 2015) and *"It's not safe to write a signal handler in Swift"* (Quinn, DTS, 2020); signal-hook-registry `register` is `unsafe`, *"Calling any OS functions that are not async-signal-safe ... is not allowed"* | **approve**: every runtime read forbids it; **none detects it** (one search per language, § search). Heroes' R1 refusal at run time has no ancestor among the pages read |
| The alternative the others took | Go: a pipe (*"open a pipe ... in the signal handler write a byte"*); Swift: a dispatch source whose handler *"does not interrupt the current thread of execution"*; Erlang: `os:set_signal` to `erl_signal_server` (246's, *carried*); Python `PyErr_SetInterruptEx` (249's, *carried*) | the routes that keep program code out of the handler entirely are the norm; Heroes' doors already are one (spec § 13's stop) |
| **C**, frame records in the C library (J leans on them) | **Ubuntu 26.04** dpkg 1.23.7ubuntu1: *"Turn on the use of frame pointers by default on 64bit architectures."*, ppc64el off, s390x `-mbackchain`; glibc's changelog, no reversal for amd64 or arm64 since 2.39-0ubuntu1 in the part read | **approve**: 249's inference for 26.04 now rests on dpkg's page; libc's own build flags on 26.04 stay owed (its build log found, not read) |
| **D-rebind on Darwin** (J's rewritten slots) | fishhook: *"Also rebind symbols in __DATA_CONST segment"*, *"If hooking in __DATA_CONST, make writable before trying to write"*, *"Properly restore protections for iOS 13"*, *"Fixed the crash on iOS 15 perfectly"*, newest mirrored commit 5 years old; iOS 14.5 moved `__cxa_throw`'s pointer to `__AUTH_CONST` (a user's answer, of a user's code); chained fixups from macOS 12, *"there are no more lazy binds"*; macOS 13 beta's `__DATA_CONST` read-only after fixups without the flag (LLVM D133010); **dyld-1378** maps read-only data `VM_PROT_TPRO` when `dyld_hw_tpro_pagers` is passed, and gates interposing on AMFI; Go left its own Darwin trampoline for libSystem in 1.12 *"ensuring forward-compatibility"* (249's, *carried*) | **object (advisory)**: the one technique of its kind read carries repairs for iOS 13 and iOS 15 in its own log, a user met iOS 14.5 moving its pointers, Apple documents none of it, and the newest dyld read has a page protection those repairs never met. A run on `macos-26` and on this Mac (macOS 27.2) is the only answer |
| **K**, the residue told in spec § 13 | the rule is in C's standard (249's, *carried*); every runtime read leaves it to a library page or a tracker | no precedent of a language **specification** telling a residue of its own runtime; K stays the author's *no, for now* |

### precedents

- **ThreadSanitizer** (`tsan_interceptors_posix.cpp` and `tsan_rtl_amd64.S`, llvm-project `main`):
  records per thread, at every `setjmp`, `_setjmp`, `sigsetjmp`, `__sigsetjmp`, the caller's stack
  pointer, the shadow-stack position, the signal-handler depth and the mask stack's depth; at `longjmp`
  and `siglongjmp` unwinds to the matching record and restores them; aborts on no match. `verified`.
- **AddressSanitizer** (`asan_interceptors.cpp`, `main`): no `setjmp` interceptor; each jump calls
  `__asan_handle_no_return()`; `swapcontext` partial by its own warning. `verified`.
- **POSIX.1-2024, 2.4.3** (XSH chapter 2, offset 60000): *"If the process is multi-threaded, or if the
  process is single-threaded and a signal handler is executed"* other than as a result of the listed
  calls, *"the behavior is undefined"* if *"The signal handler calls any function or function-like
  macro defined in this standard other than one of the functions and macros specified below as being
  async-signal-safe."* `verified`. **Linux `signal-safety(7)`** (man-pages 6.19): `longjmp` and
  `siglongjmp` are on the list, *"Added in POSIX.1-2008 TC2"*; *"If a signal handler interrupts the
  execution of an unsafe function, and the handler terminates via a call to longjmp(3)"* ... *"then the
  behavior of the program is undefined."* `verified`.
- **Go**: issue 35814 above; `cgo` documentation silent. `verified`.
- **Swift**: swift-users, Greg Parker, 2015-12-22; Apple forums thread 132556, Quinn, May 2020;
  libdispatch's `dispatch_source_create(3)`, `DISPATCH_SOURCE_TYPE_SIGNAL`: *"Unlike signal handlers
  specified via sigaction, the execution of the event handler block does not interrupt the current
  thread of execution; therefore the handler block is not limited to the use of signal safe interfaces
  defined in sigaction."*, *"a dispatch source does not install a signal handler or otherwise alter the
  behavior of signal delivery."* `verified`.
- **Rust signal-hook-registry 1.4.8, `register`**: *"This function is unsafe, because the `action` is run
  inside a signal handler."*; *"Calling any OS functions that are not async-signal-safe as specified as
  POSIX is not allowed."*; *"the passed handler must not panic."* `verified`. signal-hook 0.4.6's
  `low_level::register`: 249's, *carried*.
- **Erlang `os:set_signal/2`**: 246's (`docs/panel/246-reports/historian.md:448-452`), *carried*, not
  re-read.
- **Ubuntu 26.04**: dpkg 1.23.7ubuntu1 changelog; glibc 2.43-2ubuntu2.7 changelog (first 100,000 of
  1,068,154 characters). `verified`. Ubuntu 24.04 and Debian's refusal: 249's, *carried*.
- **fishhook, dyld and Apple**: § Log. `verified` for each page; *which macOS ships dyld-1378*,
  *whether `dyld_hw_tpro_pagers` is passed on this Mac or on `macos-26`* and *whether a program can make
  a TPRO page writable* are `unverified` by any page read: questions for the ffi-pragmatist's runs.
- **Boehm's collector**: not read today; `unverified` as a precedent for telling a jump out of a handler.

### argument

Route J has a working ancestor: ThreadSanitizer records, at each `setjmp`, the stack pointer, its
shadow-stack depth and its signal-handler depth, in an assembly interceptor that tail-jumps to the C
library's own, and a `longjmp` restores them. J departs in one deliberate place: where TSan aborts on a
buffer it never saw, J keeps every entry and refuses later. Go and Swift forbid their code inside a C
handler in words, signal-hook marks it `unsafe`; none detects it. Ubuntu 26.04's dpkg keeps frame
pointers on 64-bit. On Darwin, fishhook's log carries repairs for iOS 13 and 15, Apple documents none of
it, and dyld-1378 can map read-only data as TPRO. (110 words, counted by hand)

### condition

- **A runtime that keeps its records on a jump it cannot match, and says why**, would make J's fallback
  a precedent rather than a departure. TSan's page says the opposite.
- **Apple documenting that an image's own symbol pointers may be rewritten at run time**, or a run on
  `macos-26` and on macOS 27.2 showing D-rebind's write landing on a slot dyld-1378 protects, would lift
  my objection to D-rebind. A run showing `mprotect` refused on such a slot would harden it into a
  finding.
- **Ubuntu 26.04's glibc built without `-fno-omit-frame-pointer` on amd64 or arm64** (its build log,
  below) would overturn the dpkg reading for the one library J most needs.
- **A language runtime that detects its own code running inside a C handler and refuses it** would give
  R1 an ancestor; none read does.

### search

**Queries (WebSearch)**: *golang issue cgo "signal handler" call Go exported function from C signal
handler not supported Ian Lance Taylor*; *Swift code inside signal handler sigaction async-signal-safe
"Swift runtime" not safe forums.swift.org*; *Ubuntu 26.04 frame pointers glibc -fno-omit-frame-pointer
resolute*; *dpkg-buildflags Ubuntu frame pointers "-fno-omit-frame-pointer" "mno-omit-leaf-frame-pointer"
2025 OR 2026 ubuntu dpkg changelog*; *fishhook chained fixups __DATA_CONST iOS 15 macOS 12 broken crash
issue facebook/fishhook*; *github facebook/fishhook issues arm64e "chained fixups" OR "__auth_got" OR
"macOS 13" OR "macOS 14" rebind_symbols not working*; *Apple dyld interposing "__interpose" documentation
hardened runtime DYLD_INSERT_LIBRARIES library validation macOS 26*; *apple-oss-distributions dyld
"__DATA_CONST" read-only after fixups mprotect dyld4 Loader.cpp makeSegmentsReadOnly*; *dyld TPRO
"enableTproDataConst" __DATA_CONST trusted path read-only Apple macOS*; *"enableTproDataConst" dyld
ProcessConfig*.

**Sources swept**: POSIX.1-2024 XSH 2.4 (offset 60000 to 160000); `signal-safety(7)`; Go `cmd/cgo/doc.go`
at go1.27.3 and `pkg.go.dev/cmd/cgo`, issue 35814; signal-hook-registry 1.4.8; libdispatch
`dispatch_source_create.3` at `main`; swift-users 2015-12; Apple forums 132556 and 683638; compiler-rt
`tsan_interceptors_posix.cpp` (first 100,000 of 113,959 characters), `tsan_rtl_amd64.S`,
`asan_interceptors.cpp` at `main`; fishhook's README and its Chromium mirror's log; Emerge Tools' iOS 15
post; LLVM D133010; dyld tags, `include/mach-o/dyld-interposing.h`, `dyld/Loader.cpp` (0 to 200,000 of
203,893) and `dyld/DyldProcessConfig.cpp` (0 to 100,000 of 122,646) at dyld-1378 (one more path,
`common/DyldDelegates.cpp`, 404); Apple's Hardened Runtime page (its JSON); Launchpad's glibc and dpkg
for resolute and their changelogs, and the amd64 build page of glibc 2.43-2ubuntu2.7.

**Not read, and why**: the glibc amd64 build log
(`https://launchpad.net/~ubuntu-security-proposed/+archive/ubuntu/ppa/+build/33922230/+files/buildlog_ubuntu-resolute-amd64.glibc_2.43-2ubuntu2.7_BUILDING.txt.gz`),
a compressed file this seat's fetch would have saved outside the root; the arm64 log likewise. The
fishhook GitHub issues tab itself (only a mirror of issue 89 surfaced, not opened). TSan's arm64
assembly; `tsan_interceptors_posix.cpp`'s last 13,959 characters; the remaining part of the dpkg and
glibc changelogs; Apple's `dyld(1)` manual; which macOS each dyld tag ships in; Boehm's collector;
Oberon, Pascal-P4, Cyclone, Hylo, Idris, K/APL, cfront, Zig, Nim for this mechanism (none known to own
one; an assumption, not a finding). **One search per language** for a runtime that detects its own code
inside a C handler (Go, Swift); Rust's, Python's and Java's are 247's to 249's, *carried*.

### candidate prior-art lines

Each applies only if the sitting adopts the route it names, the page is re-read by the session filing it,
and a link to another project's issue goes through `redirect.github.com`.

**Route J**

- `- **Reached first by ThreadSanitizer (LLVM)**: [tsan_interceptors_posix.cpp](https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/tsan/rtl/tsan_interceptors_posix.cpp): at each setjmp the runtime remembers the stack pointer, the shadow stack's position and the thread's signal-handler depth; a longjmp finds the remembered buffer whose stack pointer matches, unwinds the shadow stack to it and restores that depth, and reports that it cannot find the buffer when none matches.`
- `- **Reached first by ThreadSanitizer (LLVM)**: [tsan_rtl_amd64.S](https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/tsan/rtl/tsan_rtl_amd64.S): the setjmp, _setjmp, sigsetjmp and __sigsetjmp interceptors are written in assembly, pass the caller's stack pointer to __tsan_setjmp and tail-jump to the C library's own function.`

**The rule J enforces**

- `- **Reached first by Go**: [issue 35814](https://redirect.github.com/golang/go/issues/35814): a Go function called from a signal handler that non-Go code installed will never work, since a signal handler can only call async-signal-safe functions and Go code will never be async-signal-safe; a pipe written by the handler is recommended.`
- `- **Reached first by libdispatch**: [dispatch_source_create(3)](https://raw.githubusercontent.com/apple-oss-distributions/libdispatch/main/man/dispatch_source_create.3): a signal source's handler does not interrupt the current thread, so it is not limited to signal-safe interfaces, and the source installs no signal handler.`

**C, frame records (where J leans on them)**

- `- **Reached first by Ubuntu**: [dpkg 1.23.7ubuntu1 changelog](https://changelogs.ubuntu.com/changelogs/pool/main/d/dpkg/dpkg_1.23.7ubuntu1/changelog): Ubuntu 26.04's build flags turn on frame pointers by default on 64-bit architectures, ppc64el excepted and s390x left with -mbackchain.`

**Not candidate lines**: ASan's partial `swapcontext` (a residue, not a route); fishhook and dyld-1378
(a cost of D-rebind, not a decision Heroes takes from them); Swift's forum answers (a refusal in words,
already Heroes' rule).

---

## Question 2: the end of `main` with threads still running (defect 2552)

### verdict

**approve R (advisory)**, as the one route with a direct ancestor and no ancestor's failure. **E is
everybody's silent default with a sentence added**; I do not object to it, and I name what its
ancestors paid.

| Route | Precedent on the pages read | Reading, advisory |
|---|---|---|
| **R**, an unjoined thread aborts at `main`'s return, saying how many | **C++**: `~thread` on a joinable thread calls `std::terminate()`; **Zig**: the caller *"must eventually either call `join()` ... or call `detach()"*, not enforced; **goleak**: a test fails on *"extra goroutines"* at its end; Heroes' own lease and handle rule | **approve**: C++ ships the refusal at run time. **Departure**: C++ refuses at the handle's end, R at `main`'s return, counting; no runtime read counts at the end of `main` |
| R's gate reading no balance while a thread runs | **LeakSanitizer** halts the process and reads *"stacks of running threads"* and their registers as roots; **Memcheck** reads *"general purpose registers of all threads"* and stacks | **approve**: both established checkers treat a running thread's memory as reachable; neither accuses it. Heroes' gate, which counts and traces nothing, is the departure, and R repairs it in their direction |
| **E**, end them with the process and say how many | Go: *"It does not wait for other (non-`main`) goroutines to complete."*; Rust: *"the entire program shuts down, even if other threads are still running, and any destructors on the remaining threads' stacks may not be executed"*; C11's `exit` names no thread; Python's daemons *"abruptly stopped at shutdown"*, resources *"may not be released properly"* | no objection: E is the silent rule of Go, Rust and C, plus the count none of them prints |
| **J** (sitting 252's join at the end) | Java: shutdown *"begins when all started non-daemon threads have terminated"*; Python: exits *"when no alive non-daemon threads are left"*; Ada: a master *"is blocked until all the dependents have terminated"*; C++20 `jthread` joins in its destructor; Rust `scope` (1.63) joins before returning | precedented, and the brief does not propose it: **a third route with the longest ancestry**, the one every language with a joining rule took |
| refused when compiled | **Mojo** 0.26.1 (2026-01-29): explicitly destroyed types, *"Otherwise, the compiler reports an error"*; a thread type on them `unverified` (slides not opened) | no mainstream language read refuses an unjoined thread at compile time; Rust declined even a warning (`#[must_use]`, 2018) |

### precedents

- **C++** (cppreference): `std::thread::~thread`, *"If `*this` has an associated thread (`joinable() ==
  true`), `std::terminate()` is called."*; `std::jthread` since C++20, *"automatically rejoins on
  destruction"*. **P0660R0** (Josuttis, 2017-06-19): an auto-joining thread voted at Jacksonville 2016
  (*"10 11 1 2 0"*, and for C++17 *"9 5 8 2 0"*), *"reverted later in Oulu in a smaller group"*; Herb
  Sutter's table rates killing a thread *"Avoid, almost certain to corrupt transaction(s)"*. `verified`.
- **Rust**: `std::thread` (main's end); `spawn` and `JoinHandle` (drop detaches; not `#[must_use]`);
  issue 48820 and PR 48830 (2018, refused for *"a lot of warnings on stable"*, the libs team liking
  *"the approach of a separate function"*); issue 24292 (2015, `JoinGuard` unsound by a reference
  cycle, `thread::scoped` de-stabilised for 1.0); `scope` since 1.63.0. `verified`.
- **Go**: the specification's *Program execution*; Uber's `goleak` v1.3.0. `verified`.
- **C11 and POSIX.1-2024**: 5.1.2.2.3, 7.22.4.4, 7.22.4.7, 7.26.5.3, 7.26.5.5, 7.26.5.6; `exit`, `_Exit`
  (*"Threads terminated by a call to _Exit() or _exit() shall not invoke their cancellation cleanup
  handlers or per-thread data destructors."*), `pthread_exit` (*"The process shall exit with an exit
  status of 0 after the last thread has been terminated."*). `verified`.
- **Java**: `Thread` (Java SE 25), `StructuredTaskScope` (preview, since 21). `verified`. JEPs
  `unverified` (403).
- **Python**: `threading` (3.15.2). `verified`.
- **Swift**: TSPL's Concurrency chapter, *"In a parent task, you can't forget to wait for its child
  tasks to complete."*; the end of the program is not addressed. `verified`.
- **Zig** 0.15.1 `std/Thread.zig`; **Ada** 2022 RM 9.3; **Erlang** `halt/2` and `init:stop/1` (OTP
  29.1.2); **Trio**'s essay (2018-04-25). `verified`.
- **Mojo** 0.26.1 and the nightly manual. `verified`; the thread example `unverified`.
- **LeakSanitizer** design document; clang's page; **Valgrind** Memcheck manual. `verified`.

### argument

Every language read either waits for the threads (Java's non-daemon threads, Python's, Ada's masters) or
ends them in silence (Go, Rust, C, Python's daemons); none tells, at `main`'s return, how many it ended
or left. C++ is R's nearest ancestor and its warning: an unjoined `std::thread` calls `std::terminate`,
and WG21 voted an auto-joining thread for C++17 in 2016, lost it in Oulu, and shipped `jthread` in
C++20. Rust refused `#[must_use]` on `JoinHandle` for its warnings, de-stabilised `thread::scoped` before
1.0 when a leaked guard proved unsound, and added `scope` in 1.63. LeakSanitizer and Memcheck read
running threads' stacks as roots, so neither accuses a running thread's blocks. (106 words, counted by
hand)

### condition

- **A language that aborts at the end of `main` over an unjoined thread and later withdrew it**, with
  the reason on a page, would turn my approval of R into an objection.
- **WG21's reason for the Oulu reversal**, if a page gives it (P0660R0 gives only the fact), would say
  whether C++ left `std::terminate` for a cost R would also pay.
- **The environment task's rule in Ada's RM 10.2** (not read) would say whether Ada's main program
  waits for library-level tasks, which would strengthen J as a third route.
- **A compile-time refusal of an unjoined thread in a shipped language** (Mojo's thread type, Vale)
  would make a Principle-0 question of whether R's check belongs in the checker.

### search

**Queries (WebSearch)**: *P0660 "Stop Token and Joining Thread" Josuttis jthread paper*; *openjdk JEP
structured concurrency StructuredTaskScope JDK 26 preview status*; *Ada reference manual 9.3 "Task
Dependence - Termination of Tasks" master waits dependent tasks*; *rust-lang issue "JoinHandle"
"must_use" thread spawn detach lint*; *linear types thread handle must be joined compile error language
"must be joined" type system refuses unjoined thread*; *Mojo "@explicit_destroy" linear types docs
"must" destroy compile error*.

**Sources swept**: Go's specification (offsets 0, 160000, 230000); Rust's `thread`, `spawn`,
`JoinHandle`, `scope` (std 1.100.0) and `join_handle.rs` at `master`; issues 48820, 24292 and PR 48830;
cppreference's `~thread`, `jthread` and C `thrd_exit`; P0660R0 (pages 1 to 3); N1570 (offsets 0, 600000,
740000, 832000); POSIX.1-2024 `exit`, `_Exit`, `pthread_exit`; Java SE 25 `Thread` and
`StructuredTaskScope`; Python 3.15.2 `threading`; Ada 2022 RM 9.3; Erlang `erlang` (offsets 0, 185000,
200000) and `init`; Zig 0.15.1 `Thread.zig` (the rendered std page empty); TSPL's Concurrency source
(the rendered docs.swift.org page empty); vorpus.org's essay; goleak; Mojo 0.26.1 notes and the nightly
manual (two other Mojo URLs 404); the LSan design document and clang's page; Valgrind's Memcheck manual
(first 100,000 of 107,113).

**Not read**: openjdk.org's JEPs (403); the adopted P0660 revision; the C++ standard itself; Ada RM 10.2;
Evan Ovadia's LLVM 2024 slides (a PDF, left unopened to write nothing more outside the root); Vale's and
Austral's pages; Swift's behaviour of a detached task at the end of top-level code (no search made);
Python's interpreter source for its join of non-daemon threads; any per-thread-heap runtime that checks
a heap at its thread's end (no search made: Erlang's efficiency guide, read for Q3, names no such check).

### candidate prior-art lines

**R**

- `- **Reached first by C++**: [std::thread::~thread](https://en.cppreference.com/w/cpp/thread/thread/~thread): destroying a thread object that still has an associated thread calls std::terminate.`
- `- **Reached first by Uber's goleak**: [goleak](https://pkg.go.dev/go.uber.org/goleak): VerifyNone marks a test failed when extra goroutines are found, and VerifyTestMain verifies that a package's tests leave no goroutine leaks.`
- `- **Reached first by LeakSanitizer**: [design document](https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizerDesignDocument): the leak check runs at the very end of the process, halts it first, and counts the stacks and general-purpose registers of running threads among its roots.`
- `- **Reached first by Valgrind**: [Memcheck manual](https://valgrind.org/docs/manual/mc-manual.html): the leak search's root set is the general-purpose registers of all threads and the accessible memory, stacks included.`

**E** (only if adopted)

- `- **Reached first by Go**: [The Go Programming Language Specification, Program execution](https://go.dev/ref/spec#Program_execution): when main returns the program exits, and it does not wait for other goroutines to complete.`
- `- **Reached first by Rust**: [std::thread](https://doc.rust-lang.org/std/thread/index.html): when the main thread terminates, the entire program shuts down even if other threads are still running, and destructors on their stacks may not run.`

**J** (only if a join at the end is adopted)

- `- **Reached first by Java**: [Thread](https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Thread.html): the shutdown sequence begins when all started non-daemon threads have terminated.`
- `- **Reached first by C++**: [std::jthread](https://en.cppreference.com/w/cpp/thread/jthread): since C++20 a jthread requests a stop and joins in its destructor if it is still joinable.`

**Not candidate lines**: Rust's refused `#[must_use]` and `thread::scoped` (history of others' costs,
for the sitting's file); P0660R0's vote (the same); Mojo (a compile-time route not proposed).

---

## Question 3: threads that share nothing (decision 32)

### verdict

**approve (advisory) any value copied whole, with a mailbox that is a handle**, three departures named;
**no precedent read decides B against M**, and one read names a spelling neither arm states.

| Route or piece | Precedent on the pages read | Reading, advisory |
|---|---|---|
| **Every value crosses whole, as a copy** | **Erlang**: *"All data in messages sent between Erlang processes is copied, except for refc binaries and literals on the same Erlang node."*, sharing lost (22 words to 4094 in its example); **Dart**: *"may have a linear time cost to copy the transitive object graph"*, *"Objects that are identified as immutable (e.g. strings) will be shared"* | **approve**. **Departure, to be deliberate**: both systems read share what cannot change (refc binaries; immutable objects), and Heroes would copy a `str` too. design.md 7.13 already names Erlang's half of this (`docs/design.md:4142-4148`); Dart's is new |
| what copying cost them | **Dart 2.15** (2021-12-08): the result *"needed to be deep-copied, which could itself cause UI jank if the copy took longer than the frame budget"*; `Isolate.exit` now passes it *"without copying"*, received *"in constant time"* | a precedent's reversal at exactly the `wait`/result boundary: Q3's `wait` copies the answer back, the shape Dart stopped copying |
| **the mailbox, a handle whose copies reach one queue** | **Go**: a channel *"acts as a reference to an underlying data structure"*, `close` once, *"A send on a closed channel proceeds by causing a run-time panic"*, a receive after `close` *"will return the zero value ... without blocking"*; **Rust**: `Sender` *"clone-able"*, `Receiver` *"can only be owned by one thread"*, a dropped half gives `Err`; **Swift** `AsyncStream`: after `finish()`, yields *"have no effect"*, return `.terminated` | **approve**: it is Go's channel shape, the critic's finding 7 answered by precedent: **the proposal departs from Erlang (7.13) toward Go**. Its send after the close aborts, as Go's panics; **what a receive waiting at the close does is not in the brief's text**, where Go answers *"zero value ... without blocking"* and Rust *"Err"* |
| **7.13's own mailbox**, a queue per thread | **Erlang**: a pid; *"Sending a message to a pid never fails, even if the pid identifies a non-existing process"*; `receive` searches the process's own *"message queue"*; no close | precedented by the language 7.13 names; a dead receiver's mail is lost in silence there |
| **a `ptr` or `cstr` never crosses** | **Rust**: *"raw pointers are neither Send nor Sync (because they have no safety guards)"*, *"more of a lint"*; **Dart**: *"Objects with native resources"* cannot be sent, *"A Socket object for example"* | **approve**: Rust and Dart both refuse the address |
| **another handle only into `spawn` or a send, its life ending in the sender** | **JavaScript** transfer: *"Following a transfer, the original object is no longer usable"*; **Swift** SE-0430: *"the caller cannot use the argument value again after the callee returns"*; **Pony** `iso`, *"give it to another actor"*; **Erlang** `controlling_process`, only the owner (`{error, not_owner}`) | **approve**: the move is precedented four times. **Departure**: Dart refuses a native-resource object outright, where Heroes would move it |
| **share and pay a detector** (Go's route, not proposed) | Go allows sharing, advises against it (*"Do not communicate by sharing memory"*, 2010; *"This approach can be taken too far"*), pays with a race detector that *"only finds races that happen at runtime"* at *"5-10x"* memory and *"2-20x"* time; the memory model: a race on a multiword value *"can in turn lead to arbitrary memory corruption"* | the cost of the route Heroes refuses, on Go's own pages |
| **the rung as it stands** | none needed: Rebel's ledger is its record | no reading |
| **plain data parallelism** (7.13's *"first and probably only rung"*) | Rust `scope` (1.63), Java `StructuredTaskScope` (preview), Trio nurseries (2018), Swift task groups | precedented as structured concurrency; not proposed |
| **B against M** | Go: `ch <- v` an operator, `close(ch)` a predeclared function; Rust: methods; Erlang: `!` and `receive` | **no page read gives a reason** for its spelling, and none offers both. **Go reserves `close` and lets a program shadow it** (*"The scope of a predeclared identifier is the universe block"*; *"An identifier declared in a block may be redeclared in an inner block"*): a third spelling, a built-in a declaration may shadow, that neither B (refused everywhere) nor M (after a dot only) states |

### precedents

- **Go**: the blog post (Andrew Gerrand, 2010-07-13), Effective Go, the memory model (2022-06-06), the
  race detector's page, the specification's sends, `close` and predeclared names. `verified`.
- **Rust**: the Rustonomicon's *Send and Sync*, `std::sync::mpsc`, `Arc`. `verified`.
- **Erlang** (OTP 29.1.2): the efficiency guide's *Processes*, the processes reference, *Expressions*'
  send and receive, `gen_tcp:controlling_process/2`. `verified`. Refc binaries' 64-byte threshold:
  design.md's citation of the binary handling page, *carried*, not re-read.
- **Swift**: TSPL's *Sendable*; SE-0414 and SE-0430 (*"Implemented (Swift 6.0)"*); SE-0314 (*"Implemented
  (Swift 5.5)"*). `verified`.
- **Dart**: *Isolates*, `SendPort.send`, `Isolate.exit`, the Dart 2.15 announcement (Michael Thomsen,
  2021-12-08, read on dart.cn, the Medium original not read). `verified`.
- **JavaScript**: MDN's *Transferable objects* and `SharedArrayBuffer` (*"Shared memory and high-resolution
  timers were effectively disabled at the start of 2018 in light of Spectre. In 2020, a new, secure
  approach has been standardized to re-enable shared memory."*: a reversal on sharing between workers,
  read on MDN, the HTML standard not read). `verified`.
- **Pony**: the tutorial's reference capabilities. `verified`.

### argument

Copying every value across is Erlang's and Dart's rule, each with an exception its own page names:
Erlang shares refc binaries and literals, Dart shares immutable objects and since 2.15 hands
`Isolate.exit`'s result over uncopied, since a deep copy could cause jank. Go allows sharing, advises
against it, and pays with a run-time race detector. The proposal's mailbox is Go's channel, not Erlang's
pid: a reference every copy shares, closed once, a send after the close a panic; Erlang's send to a dead
pid never fails. A handle leaving its sender is JavaScript's transfer and Swift's `sending`; Dart refuses
a socket outright. Go reserves `close` yet lets a program shadow it, a third spelling beside B and M.
(118 words, counted by hand)

### condition

- **A share-nothing system that copied immutable large values too and kept it**, on a page, would remove
  the departure I name; Erlang and Dart both share them.
- **A measured copy cost at `wait` comparable to Dart's** (the compiler-engineer's per-byte instructions)
  would make Dart 2.15's transfer at exit a route the sitting owes a look, not a footnote.
- **A language that says why it spells its channel or thread operations as functions or as methods**
  would bear on B against M; none read does.
- **Go's experience with shadowed predeclared names** (a page on bugs from shadowing `close`, `len`)
  would weigh the third spelling either way; not searched.

### search

**Queries (WebSearch)**: *Dart 2.15 isolate groups Isolate.exit "without copying" announcement medium
dart blog*. The rest were direct fetches of the pages the brief names.

**Sources swept**: Go's blog *Share Memory By Communicating*, Effective Go (offset 60000), the memory
model, the race detector article, the specification (offsets 0 and 160000); the Rustonomicon's *Send
and Sync*, `std::sync::mpsc`, `Arc` (first 100,000 of 210,443); Erlang's efficiency guide, processes
reference, expressions (offsets 14000 and 20000), `gen_tcp`; Swift's TSPL Concurrency source,
SE-0414, SE-0430, SE-0314; Dart's *Isolates*, `SendPort.send`, `Isolate.exit`, the 2.15 announcement on
dart.cn; MDN's *Transferable objects* and `SharedArrayBuffer`; Pony's tutorial; Nim's manual (offset
200000, where its *Method call syntax* section was not).

**Not read**: the binary handling page (design.md's citation stands); the HTML standard's structured
clone; Swift's `Sendable` reference page and `AsyncStream` API page (the proposal read instead); Nim's
*Method call syntax* (the part read did not hold it); D's UFCS; any page on Go's shadowed predeclared
names; Pony's `tag` beyond the tutorial; Cyclone, Hylo, Oberon and Idris for message passing (none known
to this seat to own the route; an assumption, not a finding).

### candidate prior-art lines

**Any value copied**

- `- **Reached first by Erlang**: [Efficiency Guide, Processes](https://www.erlang.org/doc/system/eff_guide_processes.html): all data in messages sent between processes is copied, except refc binaries and literals on the same node, and shared subterms are not preserved when a term is sent or passed as a spawn argument.`
- `- **Reached first by Dart**: [SendPort.send](https://api.dart.dev/stable/dart-isolate/SendPort/send.html): a send may have a linear time cost to copy the transitive object graph; immutable objects such as strings are shared and all others copied; objects with native resources, a Socket for example, cannot be sent.`

**The mailbox as a handle**

- `- **Reached first by Go**: [Effective Go, Channels](https://go.dev/doc/effective_go#channels): a channel allocated with make acts as a reference to an underlying data structure.`
- `- **Reached first by Go**: [The Go Programming Language Specification, Close](https://go.dev/ref/spec#Close): close records that no more values will be sent; sending to or closing a closed channel causes a run-time panic; after the close and the values already sent, receives return the zero value without blocking.`

**A handle that moves**

- `- **Reached first by JavaScript (MDN)**: [Transferable objects](https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Transferable_objects): a transferred object's resources are available in one context at a time, and after a transfer the original object is no longer usable.`
- `- **Reached first by Swift**: [SE-0430, sending parameter and result values](https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0430-transferring-parameters-and-results.md): a sending parameter must be disconnected at the function boundary, and the caller cannot use the argument value again after the callee returns.`

**A `ptr` never crosses**

- `- **Reached first by Rust**: [The Rustonomicon, Send and Sync](https://doc.rust-lang.org/nomicon/send-and-sync.html): raw pointers are neither Send nor Sync, because they have no safety guards.`

**Not candidate lines**: Dart 2.15's transfer at exit, Go's race detector and the memory model, MDN's
`SharedArrayBuffer` history (others' costs, for the sitting's file); Pony's capabilities and Erlang's
`controlling_process` (a comparison, unless the sitting adopts an owner-only transfer).

---

## Log, as I went (every page read 2026-11-19)

### Question 1

- **POSIX.1-2024, XSH 2.4**, https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html
  (offset 60000 to 160000): *"If the process is multi-threaded, or if the process is single-threaded and a
  signal handler is executed"* ... *"The signal handler calls any function or function-like macro defined
  in this standard other than one of the functions and macros specified below as being
  async-signal-safe."* ... *"The signal handler refers to any object other than errno with static or
  thread storage duration other than by assigning a value to an object declared as volatile
  sig_atomic_t"*; *"when a signal interrupts an unsafe function or function-like macro, or equivalent"*,
  *"the signal-catching function calls an unsafe function or function-like macro, the behavior is
  undefined."* `verified`.
- **`signal-safety(7)`**, https://man7.org/linux/man-pages/man7/signal-safety.7.html (man-pages 6.19,
  2026-06-05): *"An async-signal-safe function is one that can be safely called from within a signal
  handler."*; `longjmp` and `siglongjmp` listed, *"Added in POSIX.1-2008 TC2; see notes below."*; *"If a
  signal handler interrupts the execution of an unsafe function, and the handler terminates via a call to
  longjmp(3)"* ... *"then the behavior of the program is undefined."* `verified`.
- **Go `cmd/cgo/doc.go` at go1.27.3**, https://raw.githubusercontent.com/golang/go/go1.27.3/src/cmd/cgo/doc.go:
  none of *signal*, *handler*, *async* (the fetch's search of the whole text); the export section's only
  restriction is the preamble's (*"it must not contain any definitions, only declarations"*). The
  rendered `https://pkg.go.dev/cmd/cgo` likewise held no sentence on signal handlers. `verified`, an
  absence in those pages.
- **Go issue 35814**, https://github.com/golang/go/issues/35814: *"runtime: "fatal: morestack on g0" and
  blocking forever when go export functions is called in any signal handlers registered by the non-Go
  code"*, searKing, 2019-11-25; ianlancetaylor the same day: *"I'm sorry, this will never work."*, *"A
  signal handler can only call async-signal-safe functions."*, *"Go code will never be
  async-signal-safe."*, *"I recommend that you open a pipe, read from the pipe on the Go side, and in the
  signal handler write a byte to the pipe."*; closed 2019-11-25, locked 2020-11-24. `verified`.
- **signal-hook-registry 1.4.8, `register`**, https://docs.rs/signal-hook-registry/latest/signal_hook_registry/fn.register.html:
  *"This function is unsafe, because the `action` is run inside a signal handler."*; *"Calling any OS
  functions that are not async-signal-safe as specified as POSIX is not allowed."*; *"Accessing globals
  or thread-locals without synchronization is not allowed"*; *"the passed handler must not panic."*
  `verified`.
- **libdispatch `dispatch_source_create.3` at `main`**,
  https://raw.githubusercontent.com/apple-oss-distributions/libdispatch/main/man/dispatch_source_create.3:
  quoted in § Q1 precedents; *"Furthermore, multiple observers of a given signal are supported; thus
  allowing applications and libraries to cooperate safely."*; *"Therefore, applications must ignore or at
  least catch any signal that terminates a process by default."* `verified`.
- **swift-users**, https://lists.swift.org/pipermail/swift-users/Week-of-Mon-20151221/000586.html, Greg
  Parker, Tue Dec 22 2015: *"Neither the Swift runtime nor the code generated by the Swift compiler promise
  to be signal-safe."*; *"You should use the C code. Signal handlers are allowed to do almost nothing."*
  `verified`.
- **Apple Developer Forums 132556**, https://developer.apple.com/forums/thread/132556, Quinn "The
  Eskimo!", DTS, May 2020: *"It's not safe to write a signal handler in Swift. That's because signal
  handlers must be async signal safe [1], which means that they're not allowed to allocate memory, and
  Swift does not let you prevent such allocations [2]."* `verified`.
- **ThreadSanitizer `tsan_interceptors_posix.cpp` at `main`**,
  https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/tsan/rtl/tsan_interceptors_posix.cpp
  (0 to 100,000 of 113,959): `extern "C" void __tsan_setjmp(uptr sp) { SetJmp(cur_thread_init(), sp); }`;
  `SetJmp`: `// Cleanup old bufs.`, `JmpBufGarbageCollect(thr, sp);`, `buf->sp = sp;`,
  `buf->shadow_stack_pos = thr->shadow_stack_pos;`, `buf->int_signal_send`, `buf->oldset_stack_size`,
  `buf->in_blocking_func`, `buf->in_signal_handler = atomic_load(&thr->in_signal_handler, ...)`;
  `LongJmp`: `uptr sp = ExtractLongJmpSp(env);`, `// Find the saved buf with matching sp.`,
  `// Unwind the stack.`, `while (thr->shadow_stack_pos > buf->shadow_stack_pos) FuncExit(thr);`, the
  mask stack popped to its saved size, `atomic_store(&thr->in_signal_handler, buf->in_signal_handler,
  ...)`, `JmpBufGarbageCollect(thr, buf->sp - 1);  // do not collect buf->sp`; no match:
  `Printf("ThreadSanitizer: can't find longjmp buf\n"); CHECK(0);`; the `longjmp` interceptor: *"Note: if
  we call REAL(longjmp) in the context of ScopedInterceptor, bad things will happen."*; *"The real
  interceptor for setjmp is special, and implemented in pure asm. We just need to initialize the REAL
  functions so that they can be used in asm."*, `InitializeSetjmpInterceptors()` registering `setjmp`,
  `_setjmp`, `sigsetjmp`, `__sigsetjmp`. `verified`.
- **ThreadSanitizer `tsan_rtl_amd64.S` at `main`**,
  https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/tsan/rtl/tsan_rtl_amd64.S: each
  of the four interceptors, `// obtain SP, store in %rdi, first argument to void __tsan_setjmp(uptr sp)`
  (`lea 16(%rsp), %rdi` for `setjmp` on Linux and Apple), then `// tail jump to libc setjmp` (or
  `sigsetjmp`), `jmp *(%rdx)` through the `real_*` pointer (Apple: `jmp ASM_SYMBOL(setjmp)`); no comment
  says why assembly. `verified`.
- **AddressSanitizer `asan_interceptors.cpp` at `main`**,
  https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/asan/asan_interceptors.cpp:
  `INTERCEPTOR(void, longjmp, void* env, int val) { __asan_handle_no_return(); REAL(longjmp)(env, val);`,
  likewise `_longjmp`, `__longjmp_chk`, `siglongjmp`; no `setjmp`; `swapcontext`: *"WARNING: ASan doesn't
  fully support makecontext/swapcontext functions and may produce false positives in some cases!"*;
  *"Clear shadow memory for new context (it may share stack with current context)."*; *"Only clear if we
  know the stack. This should be true only for contexts created with makecontext()."* `verified`.
- **fishhook README**, https://raw.githubusercontent.com/facebook/fishhook/main/README.md: *"enables
  dynamically rebinding symbols in Mach-O binaries running on iOS in the simulator and on device"*; it
  *"binds lazy and non-lazy symbols by updating pointers in particular sections of the `__DATA`
  segment"*; no limitation listed. `verified`.
- **fishhook's log (Chromium mirror)**,
  https://chromium.googlesource.com/external/github.com/facebook/fishhook/+log/refs/heads/main (relative
  ages only): aadc161 *"Merge pull request #87 from haolianfu/perfect_fix_crash_on_iOS15"* (5 years);
  6da9db8 *"Fixed the crash on iOS 15 perfectly: changing vm prot according to whether having
  VM_PROT_WRITE rather than being const seg."* (5 years); 4eeb18c *"Properly restore protections for iOS
  13"* (7 years); cf847c2 *"If hooking in __DATA_CONST, make writable before trying to write"* (7 years);
  8327b28 *"Also rebind symbols in __DATA_CONST segment"* (11 years). Newest: aadc161. `verified` for the
  mirror; that GitHub's `main` holds nothing newer is `unverified`.
- **Apple Developer Forums 683638**, https://developer.apple.com/forums/thread/683638, June 2021, the
  asker's own accepted answer (not Apple's), of their fishhook-based code: *"I found that the symbol
  `__cxa_throw` has been moved to `__AUTH_CONST` and it has found multiple non-lazy pointer symbol tables
  (`__auth_got` and `__got`)."*, of iOS 14.5. `verified` (a user's reading, not Apple's documentation).
- **Emerge Tools, *How iOS 15 makes your app launch faster***, https://www.emergetools.com/blog/posts/iOS15LaunchTime,
  Noah Martin, 2021-06-23: *"All programs and dylibs built with a deployment target of macOS 12 or iOS
  15 or later now use the chained fixups format."*; *"(there are no more lazy binds)"*; *"The real change
  is in LC_DYLD_CHAINED_FIXUPS."* `verified` (a third party's reading of Apple's tools).
- **LLVM D133010**, https://reviews.llvm.org/D133010, BertalanD, 2022-08-31: *"This flag instructs dyld to
  mark the segment read-only after fixups have been performed."*; *"on macOS 13 beta at least,"*
  *"__DATA_CONST is read-only even without this flag, but ld64 sets it as well."* `verified`.
- **dyld tags**, https://github.com/apple-oss-distributions/dyld/tags: dyld-1378 2026-06-05, 1376.6
  2026-04-13, 1340 2026-02-19, 1335 2026-01-05, 1330 2025-11-18, 1323.3 2025-10-04, 1286.10 2025-08-15.
  The page maps no tag to a macOS release. `verified`.
- **dyld-1378 `include/mach-o/dyld-interposing.h`**,
  https://raw.githubusercontent.com/apple-oss-distributions/dyld/dyld-1378/include/mach-o/dyld-interposing.h:
  the pair placed in `__attribute__ ((section ("__DATA,__interpose,interposing")))`, with
  pointer-authentication schema where available; no sentence on which images may interpose. `verified`.
- **dyld-1378 `dyld/Loader.cpp`**, https://raw.githubusercontent.com/apple-oss-distributions/dyld/dyld-1378/dyld/Loader.cpp
  (0 to 200,000 of 203,893): `if (enableTpro && region.readOnlyData) { flags |= MAP_TPRO;`; `#define
  VM_PROT_TPRO 0x200`; `if (enableTpro && segInfo.readOnlyData()) {`, `rangeInfo.region.mwlr_protections
  |= VM_PROT_TPRO;`; `makeSegmentsReadOnly`, `mprotect((void*)start, size, PROT_READ);`; *"// AMFI can ban
  interposing"*, `if ( !state.config.security.allowInterposing )`; *"// Note: DATA_CONST is r/w at this
  point, so objc can do its fixups"*. `verified`.
- **dyld-1378 `dyld/DyldProcessConfig.cpp`**, https://raw.githubusercontent.com/apple-oss-distributions/dyld/dyld-1378/dyld/DyldProcessConfig.cpp
  (0 to 100,000 of 122,646): `this->enableTproDataConst = this->defaultTproDataConst();`; *"// This
  returns true if the process is using TPRO for DATA_CONST."*; `return
  (this->appleParam("dyld_hw_tpro_pagers") != nullptr);`; false under `TARGET_OS_EXCLAVEKIT ||
  TARGET_OS_SIMULATOR`; `this->allowInterposing = (amfiFlags &
  AMFI_DYLD_OUTPUT_ALLOW_LIBRARY_INTERPOSING);`. `verified`. Whether a TPRO page accepts the program's
  own `mprotect` is not on these pages: a question.
- **Apple, Hardened Runtime** (its JSON), https://developer.apple.com/tutorials/data/documentation/security/hardened-runtime.json:
  *"The Hardened Runtime, along with System Integrity Protection (SIP), protects the runtime integrity of
  your software by preventing certain classes of exploits, like code injection, dynamically linked
  library (DLL) hijacking, and process memory space tampering."*; the DYLD-variables entitlement,
  *"whether the app may be affected by dynamic linker environment variables, which you can use to inject
  code into your app's process."* `verified`. **No Apple page read speaks of rewriting an image's own
  symbol pointers.**
- **Launchpad, glibc in resolute**, https://launchpad.net/ubuntu/resolute/+source/glibc: release *"glibc
  2.43-2ubuntu2"*, security and updates *"glibc 2.43-2ubuntu2.7"*. **Its changelog**,
  https://changelogs.ubuntu.com/changelogs/pool/main/g/glibc/glibc_2.43-2ubuntu2.7/changelog (first
  100,000 of 1,068,154 characters): newest *"glibc (2.43-2ubuntu2.7), resolute-security"*, Thu, 05 Nov
  2026; the frame-pointer lines found are 2.39-0ubuntu1's *"Drop the -prof variant to instead use frame
  pointers on all 64-bit architectures by default to match the rest of the distro (LP: #2042790)"* and
  2.39-3.1ubuntu1's *"Re-enable frame-pointer optimization on ppc64el"* and *"Replace frame pointers by
  -mbackchain on s390x"*. `verified` for that part; the rest of the file not read.
- **Launchpad, dpkg in resolute**, https://launchpad.net/ubuntu/resolute/+source/dpkg: *"dpkg
  1.23.7ubuntu1"*, uploaded 2026-03-31. **Its changelog**,
  https://changelogs.ubuntu.com/changelogs/pool/main/d/dpkg/dpkg_1.23.7ubuntu1/changelog (first 100,000
  of 1,141,947): *"dpkg (1.23.7ubuntu1) resolute; urgency=medium"*, Tue, 31 Mar 2026: *"dpkg-buildflags:
  Add a new feature "framepointer" in the "qa" area."*, *"Turn on the use of frame pointers by default on
  64bit architectures."*, *"Disable framepointer on ppc64el."*, *"Disable framepointer on s390x, leaving
  only -mbackchain."*; the same block in every Ubuntu upload back to 1.22.10ubuntu1 (oracular,
  2024-08-01). `verified`.
- **glibc 2.43-2ubuntu2.7's builds**, https://launchpad.net/ubuntu/+source/glibc/2.43-2ubuntu2.7: eight
  architectures built; amd64 build 33922230, *"Successfully built"*, 2026-11-06, builder lcy02-amd64-073.
  Its log not read (§ Q1 search). `verified`.

### Question 2

- **Go specification, *Program execution***, https://go.dev/ref/spec (offset 230000; *"Language version
  go1.27 (May 26, 2026)"*): *"Program execution begins by initializing the program and then invoking the
  function `main` in package `main`. When that function invocation returns, the program exits. It does
  not wait for other (non-`main`) goroutines to complete."* `verified`.
- **Rust `std::thread`**, https://doc.rust-lang.org/std/thread/index.html (std 1.100.0, 2026-11-09): *"When
  the main thread of a Rust program terminates, the entire program shuts down, even if other threads are
  still running, and any destructors on the remaining threads' stacks may not be executed."* `verified`.
- **Rust `spawn`**, https://doc.rust-lang.org/std/thread/fn.spawn.html: *"If the join handle is dropped,
  the spawned thread will implicitly be detached. In this case, the spawned thread may no longer be
  joined. (It is the responsibility of the program to either eventually join threads it creates or detach
  them; otherwise, a resource leak will result.)"*; no `#[must_use]` rendered. `verified`.
- **Rust `JoinHandle`**, https://doc.rust-lang.org/std/thread/struct.JoinHandle.html: *"A `JoinHandle`
  detaches the associated thread when it is dropped"*; *"it is not possible to `Clone` this handle: the
  ability to join a thread is a uniquely-owned permission."* **Its source**,
  https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/thread/join_handle.rs:
  `#[stable(feature = "rust1", since = "1.0.0")]`, `#[cfg_attr(target_os = "teeos", must_use)]` above
  `pub struct JoinHandle<T>`. `verified`.
- **Rust `scope`**, https://doc.rust-lang.org/std/thread/fn.scope.html: since 1.63.0; *"All threads spawned
  within the scope that haven't been manually joined will be automatically joined before this function
  returns."*; *"If any of the automatically joined threads panicked, this function will panic."*
  `verified`. (1.63.0's date is not on the page.)
- **Rust issue 48820**, https://github.com/rust-lang/rust/issues/48820: *"std::thread::JoinHandle should be
  marked with #[must_use]"*, 2018-03-07; *"Even worse - the exit code is 0, Rust doesn't give any
  warnings, and everything looks good, while it really isn't!"*; varkor, 2018-11-03, *"It was decided that
  `JoinHandle` should not be `#[must_use]` here"*. **PR 48830**, https://github.com/rust-lang/rust/pull/48830:
  opened 2018-03-07, closed unmerged 2018-04-30; alexcrichton, *"This seems pretty likely to cause a lot
  of warnings on stable."*; sfackler, 2018-04-19, *"We discussed this in triage and like the approach of a
  separate function"*, *"we're presumably doing this work in a separate thread because we want to do
  other stuff while it's running"*; the author, *"The alternative is to add a new function for spawning
  threads that returns a join-on-drop guard."* `verified`.
- **Rust issue 24292**, https://github.com/rust-lang/rust/issues/24292: *"std::thread::JoinGuard (and
  scoped) are unsound because of reference cycles."*, arielb1, 2015-04-10; *"You can use a reference cycle
  to leak a `JoinGuard` and then the scoped thread can access freed memory."*; nikomatsakis, *"I definitely
  think we need to address this for 1.0."*; aturon's commits *"De-stabilize `thread::scoped` and
  friends"*, 2015-04-13. `verified`.
- **C++ `~thread`**, https://en.cppreference.com/w/cpp/thread/thread/~thread, and **`jthread`**,
  https://en.cppreference.com/w/cpp/thread/jthread: quoted in § Q2; `jthread`'s feature macro
  `__cpp_lib_jthread` `201911L`, *"Stop token and joining thread"*. `verified` (cppreference, a secondary
  page; the standard not read).
- **P0660R0**, https://open-std.org/JTC1/sc22/wg21/docs/papers/2017/p0660r0.pdf (read from the saved copy
  named at the top, pages 1 to 3): *"Doc No: WG21 P0660R0"*, *"Date: 2017-06-19"*, *"Reply to: Nicolai
  Josuttis"*; *"In Jacksonville 2016 we had an evening session ... Add an auto-joining (in destructor)
  thread type under the understanding that its name will be changed and there will be LEWG review. SF F
  N A SA 10 11 1 2 0 Include it in C++17 SF F N A SA 9 5 8 2 0"*; *"Unfortunately, this decision was
  reverted later in Oulu in a smaller group so that we didn't get what the majority voted for."*; *"The
  destructor requests for a cooperative end of the started thread if it is still joinable by calling
  interrupt() and then join()."*; Sutter's table, *"1. Kill"*, *"Avoid, almost certain to corrupt
  transaction(s)"*; *"Butenhof's book is very clear that there is no such thing as killing or
  async-cancelling a single thread and these are equivalent to killing the whole process because they
  lead to memory corruption across the process."* `verified`. The adopted revision (R10, by the search's
  summary) not read.
- **N1570**, https://port70.net/~nsz/c/c11/n1570.html (offsets 0, 600000, 740000, 832000; a draft of
  C11): 5.1.2.2.3, *"a return from the initial call to the main function is equivalent to calling the exit
  function"*; 7.22.4.4, *"First, all functions registered by the atexit function are called, in the
  reverse order of their registration"*, *"Next, all open streams with unwritten buffered data are
  flushed, all open streams are closed"*, *"Finally, control is returned to the host environment."*, no
  word on other threads in the paragraphs read; 7.22.4.7, *"No functions registered by the atexit function
  or signal handlers registered by the signal function are called."*; 7.26.5.3, *"The thrd_detach function
  tells the operating system to dispose of any resources allocated to the thread identified by thr when
  that thread terminates."*; 7.26.5.5, *"The program shall terminate normally after the last thread has
  been terminated. The behavior shall be as if the program called the exit function with the status
  EXIT_SUCCESS at thread termination time."*; 7.26.5.6, *"The thread identified by thr shall not have
  been previously detached or joined with another thread."*; 7.21.3p5, *"If the main function returns to
  its original caller, or if the exit function is called, all open files are closed"*. `verified`.
  **cppreference `thrd_exit`**, https://en.cppreference.com/w/c/thread/thrd_exit: *"If the last thread
  in the program is terminated with `thrd_exit`, the entire program terminates as if by calling exit
  with EXIT_SUCCESS"*, citing C11 and C17 7.26.5.5. `verified`.
- **POSIX.1-2024 `exit`**, https://pubs.opengroup.org/onlinepubs/9799919799/functions/exit.html: *"The
  exit() function shall cause normal process termination to occur."*, the consequences those of
  *"Consequences of Process Termination"*; no thread named. **`_Exit`**,
  https://pubs.opengroup.org/onlinepubs/9799919799/functions/_Exit.html: *"Threads terminated by a call to
  _Exit() or _exit() shall not invoke their cancellation cleanup handlers or per-thread data
  destructors."*; *"The _Exit() and _exit() functions shall not call functions registered with atexit()
  nor at_quick_exit()"*; *"Open streams shall not be flushed."* **`pthread_exit`**,
  https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_exit.html: *"The process shall exit
  with an exit status of 0 after the last thread has been terminated. The behavior shall be as if the
  implementation called exit() with a zero argument at thread termination time."*; the implicit call is
  for a thread that *"is not the thread in which main() was first invoked, returns from the start
  routine"*. `verified`.
- **Java `Thread`**, https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Thread.html
  (Java SE 25): *"Platform threads are designated daemon or non-daemon threads. ... The shutdown sequence
  begins when all started non-daemon threads have terminated. Unstarted non-daemon threads do not prevent
  the shutdown sequence from beginning."*; *"Virtual threads are daemon threads and so do not prevent the
  shutdown sequence from beginning."* **`StructuredTaskScope`**,
  https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/StructuredTaskScope.html:
  *"`StructuredTaskScope` is a preview API of the Java platform."*, *"Since: 21"*; *"The `close` method
  always waits for threads executing subtasks to finish, even if the scope is cancelled"*; *"the `fork`,
  `join` and `close` methods may only be invoked by the owner thread ... and the `close` method throws an
  exception after closing if the owner did not invoke the `join` method after forking subtasks."*; *"If a
  thread terminates without first closing scopes that it owns then termination will cause the underlying
  construct of each of its open tasks scopes to be closed."* `verified`. 246's `Runtime` page (*"live
  non-daemon threads"*, `docs/panel/246-reports/historian.md:421-427`), *carried*.
- **Python `threading`**, https://docs.python.org/3/library/threading.html (3.15.2): *"A thread can be
  flagged as a "daemon thread". The significance of this flag is that the entire Python program exits when
  only daemon threads are left."*; *"Daemon threads are abruptly stopped at shutdown. Their resources (such
  as open files, database transactions, etc.) may not be released properly. If you want your threads to
  stop gracefully, make them non-daemonic and use a suitable signalling mechanism such as an Event."*;
  *"The entire Python program exits when no alive non-daemon threads are left."*; under `join()`, *"If an
  attempt is made to join a running daemonic thread in late stages of Python finalization `join()` raises
  a `PythonFinalizationError`"*, *"Changed in version 3.14"*. `verified`.
- **Ada 2022 RM 9.3**, https://docs.adacore.com/live/wave/arm22/html/arm22/RM-9-3.html: paragraph 5, *"The
  first step of finalizing a master (including a task_body) is to wait for the termination of any tasks
  dependent on the master. The task executing the master is blocked until all the dependents have
  terminated."*; paragraph 1, *"Each task (other than an environment task — see 10.2) depends on one or
  more masters"*. `verified`.
- **Erlang `halt/2`**, https://www.erlang.org/doc/apps/erts/erlang.html (OTP 29.1.2, erts 17.1.1; offsets
  0, 185000, 200000): *"Halt the runtime system with status code Status."*; with flushing (the default)
  the runtime flushes output, closes ports, waits for async jobs and calls `atexit`/`on_exit` callbacks
  before terminating (the fetch's summary of the list); without, *"the runtime system will terminate
  immediately"*; nothing on processes. **`init:stop/1`**, https://www.erlang.org/doc/apps/erts/init.html:
  *"All applications are taken down smoothly, all code is unloaded, and all ports are closed"* before
  *"calling `halt(Status)`"*. `verified`.
- **Zig 0.15.1 `std/Thread.zig`**, https://raw.githubusercontent.com/ziglang/zig/0.15.1/lib/std/Thread.zig:
  `spawn`, *"The caller must eventually either call `join()` to wait for the thread to finish and free its
  resources or call `detach()` to excuse the caller from calling `join()` and have the thread clean up its
  resources on completion."*; `join` and `detach`, *"Once called, this consumes the Thread object and
  invoking any other functions on it is considered undefined behavior."* `verified`.
- **TSPL Concurrency source**, https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/Concurrency.md:
  *"Because of the explicit relationship between tasks and task groups, this approach is called
  structured concurrency."*; *"In a parent task, you can't forget to wait for its child tasks to
  complete."*; of unstructured tasks, *"you're also completely responsible for their correctness"*; *"A
  type that can be shared from one concurrency domain to another is known as a sendable type."*; *"The
  type is a value type, and its mutable state is made up of other sendable data"*; tasks at the program's
  end not addressed (the fetch). `verified`.
- **Nathaniel J. Smith**, https://vorpus.org/blog/notes-on-structured-concurrency-or-go-statement-considered-harmful/,
  *"Posted by Nathaniel J. Smith Wed 25 April 2018"*: *"Crucially, the nursery block doesn't exit until
  all the tasks inside it have exited"*; *"go statements are a form of goto statement."*; *"Go statements
  break abstraction."*, *"Go statements break automatic resource cleanup."*, *"Go statements break error
  handling."* `verified`.
- **Mojo**: https://mojolang.org/releases/v0.26.1/, *"January 29, 2026"*, *"Mojo now has first-class
  support for explicitly-destroyed types (sometimes referred to as "linear types")."*;
  https://mojolang.org/nightly/docs/manual/lifecycle/explicit-destroy/, *"Values that require explicit
  destruction must be consumed by a named deinitializer or transferred out of scope."*, *"Otherwise, the
  compiler reports an error."*, the example `struct CustomFileHandle(not Deinitable else "call
  'save_and_close()' or 'discard()'")`; no thread example. `verified`.
- **goleak**, https://pkg.go.dev/go.uber.org/goleak (v1.3.0, 2023-10-24): *"Package goleak is a Goroutine
  leak detector."*; *"VerifyNone marks the given TestingT as failed if any extra goroutines are found by
  Find."*; *"VerifyTestMain can be used in a TestMain function for package tests to verify that there were
  no goroutine leaks."* `verified`.
- **LeakSanitizer**, https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizerDesignDocument:
  *"The leak checker module is inactive until the very end of a process' lifetime."*; *"When invoked, it
  first halts the execution of the process."*; *"we run the leak checker as a separate task that attaches
  to the parent process with ptrace"*; the root set *"must include at least"* *"global variables"*,
  *"stacks of running threads"*, *"general-purpose registers of running threads"*, *"ELF thread-local
  storage and POSIX thread-specific data"*. https://clang.llvm.org/docs/LeakSanitizer.html: *"LSan adds
  almost no performance overhead until the very end of the process."* **Valgrind**,
  https://valgrind.org/docs/manual/mc-manual.html (first 100,000 of 107,113): *"The root-set consists of
  (a) general purpose registers of all threads, and (b) initialised, aligned, pointer-sized data words in
  accessible client memory, including stacks."*; *""Still reachable" blocks are very common and arguably
  not a problem."*; *"definitely lost"*, *"no pointer to the block can be found"*. `verified`.

### Question 3

- **Go blog**, https://go.dev/blog/codelab-share, Andrew Gerrand, 13 July 2010: *"Do not communicate by
  sharing memory; instead, share memory by communicating."*; *"Go encourages the use of channels to pass
  references to data between goroutines."*; *"This approach ensures that only one goroutine has access to
  the data at a given time."* `verified`.
- **Effective Go**, https://go.dev/doc/effective_go (offset 60000): *"Go encourages a different approach in
  which shared values are passed around on channels and, in fact, never actively shared by separate
  threads of execution. Only one goroutine has access to the value at any given time. Data races cannot
  occur, by design."*; *"This approach can be taken too far. Reference counts may be best done by putting
  a mutex around an integer variable, for instance."*; *"Like maps, channels are allocated with `make`, and
  the resulting value acts as a reference to an underlying data structure."* `verified`.
- **The Go memory model**, https://go.dev/ref/mem, *"Version of June 6, 2022"*: *"Programs that modify data
  being simultaneously accessed by multiple goroutines must serialize such access."*; *"Don't be
  clever."*; *"An implementation may always react to a data race by reporting the race and terminating
  the program."*; *"such races can in turn lead to arbitrary memory corruption."* `verified`.
- **Go race detector**, https://go.dev/doc/articles/race_detector: *"Data races are among the most common
  and hardest to debug types of bugs in concurrent systems."*; *"The race detector only finds races that
  happen at runtime, so it can't find races in code paths that are not executed."*; *"memory usage may
  increase by 5-10x and execution time by 2-20x"*; *"unsynchronized send and close operations on the same
  channel can also be a race condition"*; *"Concurrent reads and writes of the same map are not safe"*.
  `verified`.
- **Go specification**, https://go.dev/ref/spec (offsets 0 and 160000): *"Both the channel and the value
  expression are evaluated before communication begins."*; *"A send on a closed channel proceeds by
  causing a run-time panic."*; *"For a channel `ch`, the built-in function `close(ch)` records that no more
  values will be sent on the channel. ... Sending to or closing a closed channel causes a run-time panic.
  Closing the nil channel also causes a run-time panic. After calling `close`, and after any previously
  sent values have been received, receive operations will return the zero value for the channel's type
  without blocking."*; `close` among the predeclared functions; *"The scope of a predeclared identifier is
  the universe block."*; *"An identifier declared in a block may be redeclared in an inner block."*
  `verified`.
- **The Rustonomicon**, https://doc.rust-lang.org/nomicon/send-and-sync.html: *"A type is Send if it is safe
  to send it to another thread."*; *"A type is Sync if it is safe to share between threads (T is Sync if
  and only if `&T` is Send)."*; *"raw pointers are neither Send nor Sync (because they have no safety
  guards)."*; *"`Rc` isn't Send or Sync (because the refcount is shared and unsynchronized)."*; *"raw
  pointers are, strictly speaking, marked as thread-unsafe as more of a lint"*. `verified`.
- **`std::sync::mpsc`**, https://doc.rust-lang.org/std/sync/mpsc/index.html: *"Multi-producer,
  single-consumer FIFO queue communication primitives."*; *"Both senders are clone-able (multi-producer)
  such that many threads can send simultaneously to one receiver (single-consumer)."*; `Receiver`, *"This
  half can only be owned by one thread."*; *"An unsuccessful operation is normally indicative of the other
  half of a channel having "hung up" by being dropped in its corresponding thread."*; *"Once half of a
  channel has been deallocated, most operations can no longer continue to make progress, so Err will be
  returned."* `verified`.
- **`Arc`**, https://doc.rust-lang.org/std/sync/struct.Arc.html (first 100,000 of 210,443): *"A
  thread-safe reference-counting pointer."*; *"atomic operations are more expensive than ordinary memory
  accesses"*; *"`Rc<T>` is a safe default, because the compiler will catch any attempt to send an `Rc<T>`
  between threads."* `verified`.
- **Erlang efficiency guide, *Processes***, https://www.erlang.org/doc/system/eff_guide_processes.html
  (OTP 29.1.2): *"All data in messages sent between Erlang processes is copied, except for refc binaries
  and literals on the same Erlang node."*; *"Shared subterms are not preserved"* when sent, passed as spawn
  arguments or stored in ETS; the example's 22 words shared and 4094 copied, *"Sharing has been lost."*;
  *"A newly spawned Erlang process uses 327 words of memory."* `verified`.
- **Erlang processes reference**, https://www.erlang.org/doc/system/ref_man_processes.html: *"the message is
  added to the message queue"*; *"If the receiver has terminated, the signal does not arrive."*; *"if an
  entity sends multiple signals to the same destination entity, the order is preserved."* **Expressions**,
  https://www.erlang.org/doc/system/expressions.html (offsets 14000, 20000): *"`Expr1` must evaluate to a
  pid, an alias (reference), a port, a registered name (atom), or a tuple `{Name,Node}`."*; *"Sending a
  message to a pid never fails, even if the pid identifies a non-existing process."*; a name not
  registered, *"a `badarg` run-time error"*; *"`receive` never fails."* `verified`.
- **`gen_tcp:controlling_process/2`**, https://www.erlang.org/doc/apps/kernel/gen_tcp.html (OTP 29.1.2,
  kernel 11.0.4): *"Assigns a new controlling process `Pid` to `Socket`."*; *"The controlling process is the
  process that the socket sends messages to."*; *"If this function is called from any other process than
  the current controlling process, `{error, not_owner}` is returned."*; *"If the socket is in active mode,
  this function will transfer any messages from the socket in the mailbox of the caller to the new
  controlling process."* `verified`.
- **SE-0414**, https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0414-region-based-isolation.md:
  *"Implemented (Swift 6.0)"*; *"the language can prove that transferring a non-`Sendable` value over an
  isolation boundary cannot result in races"*, the value *"is not used in the caller after the point of
  transfer."* **SE-0430**, https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0430-transferring-parameters-and-results.md:
  *"Implemented (Swift 6.0)"*; *"A function parameter or result that is annotated with `sending` is
  required to be disconnected at the function boundary"*; *"the caller cannot use the argument value again
  after the callee returns."* **SE-0314**, https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0314-async-stream.md:
  *"Implemented (Swift 5.5)"*; *"Calling a continuation's `finish()` method moves its stream into a
  "terminated" state."*; *"further calls to `finish()` or `yield(_:)` once a stream is in a terminated
  state have no effect"*; *"It is thread safe to yield and finish."* `verified`.
- **Dart**: https://dart.dev/language/isolates, *"Isolates can only communicate via message passing."*,
  *"The worker isolate transfers the memory holding the result to the main isolate."*, *"It does not copy
  the data."*; https://api.dart.dev/stable/dart-isolate/SendPort/send.html, *"The send happens immediately
  and may have a linear time cost to copy the transitive object graph."*, *"Objects that are identified as
  immutable (e.g. strings) will be shared whereas all other objects will be copied."*, *"A Socket object
  for example refers internally to objects that have native resources attached and can therefore not be
  sent."*; https://api.dart.dev/stable/dart-isolate/Isolate/exit.html, *"this final message object graph
  will be reassigned to the receiving isolate without copying."*; https://dart.cn/blog/announcing-dart-2-15
  (Michael Thomsen, 2021-12-08), *"That result needed to be deep-copied, which could itself cause UI jank if
  the copy took longer than the frame budget."*, *"In 2.15 the worker isolate can call Isolate.exit(),
  passing its result as an argument."*, *"It is now more than 100 times faster to start an additional
  isolate in an existing isolate group"*. `verified`.
- **JavaScript (MDN)**: https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Transferable_objects,
  *"Transferable objects are objects that own resources that can be transferred from one context to
  another, ensuring that the resources are only available in one context at a time. Following a transfer,
  the original object is no longer usable"*, the `ArrayBuffer`'s `byteLength` `0` after the transfer;
  https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/SharedArrayBuffer,
  quoted in § Q3, and *"A `SharedArrayBuffer` is not a Transferable Object, unlike an `ArrayBuffer` which is
  transferable."* `verified`.
- **Pony**, https://tutorial.ponylang.io/reference-capabilities/reference-capabilities.html: *"If you have an
  `iso` variable then you know that there are no other variables that can access that data. So you can
  change it however you like and give it to another actor."*; *"If you have a `val` variable then you know
  that no-one can change the data. So you can read it and share it with other actors."*; *"The problem with
  concurrency is shared mutable data."*; *"By sharing only immutable data and exchanging only isolated data
  we can have safe concurrent programs without locks."* `verified`.
- **Nim manual**, https://nim-lang.org/docs/manual.html (offset 200000): its *Method call syntax* section
  not in that part. Not cited.

---

## Before any line above is filed

This seat ran no `grep` of `docs/panel/` (no shell). The session filing the sitting runs, before it
writes a prior-art line, `grep -n -i
'tsan_setjmp\|can.t find longjmp\|35814\|48820\|48830\|24292\|P0660\|jthread\|goleak\|LeakSanitizerDesign\|dyld_hw_tpro\|TPRO\|fishhook\|1.23.7ubuntu1\|SE-0430\|Isolate.exit\|controlling_process\|share memory by communicating'
docs/panel/*.md`, to find a sitting that already read one of these pages.
