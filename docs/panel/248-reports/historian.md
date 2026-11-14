# Panel 248, historian

Written 2026-11-14 by the historian seat, advisory, under `docs/panel/248-briefs/historian.md`,
`00-shared.md` and the completeness critic's second pass (`248-reports/critic-briefs.md`, § Second pass,
items 2, 11 and 12, which correct this seat's brief). Every precedent below is a page fetched on
2026-11-14, its URL beside it, and says only what that page states; the quotes are in § Log. A line
marked `unverified` rests on no page read today and is not evidence. Panel 246's and 247's historians'
pages are cited from their reports as *246's* and *247's*, not re-read, except Go's `os/signal`, re-read
for sentences neither quoted, which is said where it is used. No build, no run, no paid call, no GitHub
write; this seat has no shell, so no `grep` of the tree was run by it. *Heroes of code* was not
consulted, neither as a route nor as a source.

Status: complete. Corrected before hand-over, the same day: the Go `os/signal` lines first rested on
the fetching tool's paraphrase and now quote the page, re-fetched for its exact words; the two Node rows
of § What each corrected gave exact days where the pages say *"on, or shortly after"*. **No file was
written outside the repository's root that this seat saw**: no fetch of this sitting returned a
saved-file path (panel 246's and 247's did, a PDF and a binary); every fetch returned text, the long
ones read by offset. Nothing else was written but this report.

---

## Where the brief is wrong, or says more than its pages, first

The coordinator's list was written from memory and said so; these are the lines a page read today does
not carry, or carries differently.

1. **`pthread_tryjoin_np` is glibc's, not POSIX's** (the critic's item 11, confirmed): the Linux
   man-pages entry gives STANDARDS *"GNU; hence the suffix "_np" (nonportable) in the names"*, HISTORY
   *"glibc 2.3.3"*, and a `#define _GNU_SOURCE` synopsis (https://man7.org/linux/man-pages/man3/pthread_tryjoin_np.3.html).
2. **Java's `libjsig` names three installers on one page and one on another.** The Java SE 21 *Signal
   Chaining* page says `signal()`, `sigset()` and `sigaction()` are intercepted, and that *"As of Java 16
   the use of the `signal` and `sigset` functions are deprecated"*; the Java SE 26 troubleshooting page
   says only *"calls to `sigaction()` are intercepted"*. The source at `master` still defines all three,
   `signal` and `sigset` printing a deprecation warning. The JDK issue behind it, JDK-8257572, answered
   403 and is not cited.
3. **The sanitizer's Darwin interposer is documented as preloaded, and measured as dependent.**
   compiler-rt's `interception.h` says its `__interpose` dylib works *"When this dylib is preloaded before
   an executable using DYLD_INSERT_LIBRARIES"*; 00-shared.md measured it loaded as a dependent dylib with
   no such variable. dyld-655.1's own source carries *"dyld should support interposition even without
   DYLD_INSERT_LIBRARIES"* (rdar 19315404), which agrees with the measurement: the header comment is
   older than dyld's rule, by my reading.
4. **libuv's wake is not "an `eventfd` on Linux, a pipe elsewhere" only**: `src/unix/async.c` uses
   kqueue's `EVFILT_USER` where a runtime detection finds it, a pipe otherwise. Its Windows wake goes
   through a macro, `POST_COMPLETION_FOR_REQ`, whose definition the file read does not hold: that it is
   the loop's IOCP is `unverified`.
5. **No page read says why Go or Rust chose one wake per poller**: Go's `netpoll*.go` and mio's `Waker`
   state the shape (one `eventfd` or `EVFILT_USER` per poller; *"Only a single Waker can be active per
   Poll"*) and give no reason.
6. **glibc's release notes and Apple's manual for `posix_spawn_file_actions_addchdir_np` were not
   read.** What was read: glibc's patch of 2018-10-26 (its NEWS hunk and its `GLIBC_2.29` version node),
   and a comment in Rust's source stating the `_np` form *"was introduced in macOS 10.15"* with a bug for
   relative programs. Darwin's SDK declaring the non-`_np` form is the critic's reading (item 12),
   *carried*, not a page.
7. **Go's deadline is absolute and reaches calls already waiting** (the critic's item 2, confirmed in
   Go's source): *"A deadline is an absolute time after which I/O operations fail instead of
   blocking. The deadline applies to all future and pending I/O"*. **Python's `settimeout` is not that
   shape**: a per-socket duration for each operation (*"if the timeout period value has elapsed before
   the operation has completed"*).
8. **Go 1.1's notes do not name a poller**: they say *"Due to tighter coupling of the run-time and
   network libraries, fewer context switches are required on network operations."* When Go's
   integrated poller arrived stays `unverified` by any page read.

---

## verdict

**approve (advisory)**, question by question, with two objections:

| Q | Route | Precedent on the pages read | Reading, advisory |
|---|---|---|---|
| 1 | I, the runtime defines installers | OpenJDK `libjsig` (3 names), Android ART `libsigchain` (`signal`, `sigaction`, `sigprocmask`, and `sigaction64`, `sigprocmask64`, `bsd_signal` by target), compiler-rt (3 names) | approve that a runtime may: it has shipped for years. **Departure, to be deliberate**: every precedent defines a short list; none read claims every installer its C library exports |
| 1 | I-onstack | ART: *"~SA_ONSTACK: always silently enable this"*; Go turns `SA_ONSTACK` on in a handler that already exists (non-Go programs calling Go) | approve: the one route here two production runtimes already do |
| 1 | I-dylib | compiler-rt's Darwin interposer is exactly this; dyld-655.1 registers a non-cache image's interposing without `DYLD_INSERT_LIBRARIES` | approve as the precedented Darwin shape |
| 1 | I on Darwin from the executable alone, or flat namespace | OpenJDK needs `DYLD_INSERT_LIBRARIES` **and** `DYLD_FORCE_FLAT_NAMESPACE`; compiler-rt calls flat namespace error-prone in Chromium; ART builds a fake on Darwin; dyld-655.1's loops skip `sAllImages[0]` (my reading) | **object (advisory)**: no precedent read reaches a two-level dylib from an executable's own definitions |
| 1 | widening `ffi_defined_elsewhere` | compiler-rt documents a protocol for up to three interceptors of one function to *"co-exist"*; no page read refuses a program for defining an installer | advisory: a refusal of every library that defines `sigaction` refuses the shape compiler-rt asks foreign interceptors to take |
| 2 | (e) one poller, (b) one wake per poller | Go's netpoll (one per process, a waiter is a goroutine pointer, deadlines are timers); libuv (one wake per loop); mio (*"Only a single Waker can be active per Poll"*) | approve |
| 2 | (a) a descriptor per waiting thread | none read, except as libuv's one wake per loop where a loop *"is meant to be tied to a single thread"* | no objection, no precedent |
| 2 | Windows loopback pair | libevent's emulated `socketpair` over loopback, from Tor, *"does not work when localhost is down"* | approve, with its caveat |
| 3 | a folder door | POSIX Issue 8's spawn action from Solaris's `_np`; glibc's `_np` under `GLIBC_2.29`; Go, Python, Rust, Node all offer one | approve; **the order question is where precedents split** (below) |
| 4 | a door that asks the stop and returns | Go issue 39919, *"Shutdown() is synchronous"*, a handler waiting on its own connection; 246's Python `shutdown()` deadlock sentence | approve a door that does not wait |
| 5 | a per-socket absolute deadline; a header deadline in `serve_conn` | Go `SetDeadline` and `ReadHeaderTimeout`/`IdleTimeout` (go1.8); Node `headersTimeout` (CVE-2018-12122) and `requestTimeout` (CVE-2020-8251) | approve both; Node added both after shipping without, as security fixes |
| 5 | a join that does not wait | Rust `is_finished` beside `join`; Java `isAlive` and `join(Duration)` beside `join`; glibc `pthread_tryjoin_np` beside `pthread_join` | approve **a door beside an unchanged join**; **object (advisory)** to `hero_thread_join` itself answering "not yet": no precedent read changes a join's meaning |

---

## precedents

### Question 1: a runtime that defines the C library's installers

- **OpenJDK `libjsig`** (Java SE 21 page and `jsig.c` at `master`): linked (`-ljsig`) or `LD_PRELOAD`ed on
  Linux, *"before the `libc/libthread/libpthread` library"*; on macOS `DYLD_INSERT_LIBRARIES` and
  `DYLD_FORCE_FLAT_NAMESPACE`, *"because of macOS's two-level namespace"*; defines `signal`, `sigset`,
  `sigaction`; *"The interposed ... calls return the saved signal handlers, not the signal handlers
  installed by the HotSpot VM"* (R4's `sigaction(sig, NULL, &old)` condition, reached); `signal` and
  `sigset` deprecated as of Java 16; four signals *"cannot be chained"*; under BSD, `sigset()` exits.
  `verified`. **What it corrected**: Java 16 deprecated two of its three names (21's page); 26's page
  names `sigaction` alone; the source keeps both with a warning. Why: JDK-8257572 not readable (403).
- **Android ART `libsigchain`** (`sigchain.cc` and `Android.bp` at `main`): wraps `signal`, `sigaction`,
  `sigprocmask` (and `sigaction64`, `sigprocmask64` on Bionic, `bsd_signal` on non-LP64); a claimed
  signal's user action is recorded *"but don't pass it on to the kernel"*; real functions by
  `dlopen`/`dlsym` of libc; **`SA_ONSTACK` always enabled**, `SA_NODEFER`, `SA_RESETHAND` and
  `SA_RESTART`'s absence *"unimplemented"*; linked `-z,global` so app libraries in other linker
  namespaces find it *"before libc"*; **Darwin builds `sigchain_fake.cc`**. `verified` (the Darwin line
  is the fetch's reading of `srcs`).
- **compiler-rt** (`interception.h`, `sanitizer_signal_interceptors.inc`, the common-flags wiki): Linux,
  a function of the same name plus `dlsym`, with a weak three-level alias so *"sanitizer interceptors,
  foreign user interceptors, and foreign interceptors of other dynamic analysis runtimes ... may
  co-exist"*; Darwin, a dylib with `__DATA,__interpose`, flat namespace avoided for *"errors loading
  the shared libraries in Chromium"*; signal interceptors **`bsd_signal`, `signal`, `sigaction`** only;
  `handle_segv=2` *"block user from changing it"*. `verified`.
- **Apple dyld** (`dyld-interposing.h` at `main`; `src/dyld.cpp` at tag `dyld-655.1`): the macro puts a
  `{replacement, replacee}` pair in `__DATA,__interpose`; *"only INSERTED libraries can interpose"*,
  then *"<rdar://problem/19315404> dyld should support interposition even without
  DYLD_INSERT_LIBRARIES"*, the loop from `sInsertedDylibCount+1`, skipping shared-cache images.
  `verified`; that index 0, which neither loop visits, is the main executable is **my reading**; dyld4
  not read beyond its design note, which says nothing of interposing.
- **Go** (`os/signal`, re-read for sentences 246 and 247 did not quote): *"If the non-Go code installs
  any signal handlers, it must use the SA_ONSTACK flag with sigaction."*; for non-Go programs that call
  Go code, *"If there is an existing signal handler, the Go runtime will turn on the SA_ONSTACK flag and
  otherwise keep the signal handler."*, and the same for SIGCANCEL and SIGSETXID on Linux. The page says
  nothing of Go defining an installer. `verified`.
- **R3's shape** (a C library's handler calling back): 247's Go issue 35814 (*"this will never work"*,
  a pipe recommended) and Python's `PyErr_SetInterruptEx` stand; Oracle's Java SE 26 troubleshooting page
  holds no sentence on native handlers calling JNI. `.NET`: not searched.

### Question 2: a wait woken without slices

- **Go netpoll** (`netpoll.go`, `netpoll_epoll.go`, `netpoll_kqueue.go`, `netpoll_kqueue_event.go` at
  `master`): one *"Integrated network poller"* with `netpollBreak`, *"Wake up the network poller"*;
  each descriptor's `rg`/`wg` hold the waiting goroutine, *"io notification or timeout/close changes
  the state ... and unparks the goroutine"*; `rd`/`wd` deadlines with `rt`/`wt` timers; *"If we set the
  new deadline in the past, unblock currently pending IO"*; the wake one `eventfd` on Linux, an
  `EVFILT_USER` event on darwin, dragonfly and freebsd, a compare-and-swap avoiding *"duplicate calls"*.
  `verified`. **Route (e), reached; question 5's deadline as a timer in the poller, reached.**
- **libuv** (`async.html`, `design.html`, `src/unix/async.c`, `src/win/async.c`): `uv_async_send` *"safe
  to call ... from any thread"*, *"async-signal-safe"*, coalescing; one wake per loop, a loop *"meant to
  be tied to a single thread"*; `eventfd` on Linux, `EVFILT_USER` where detected, a pipe otherwise;
  *"the timeout of the closest timer is taken"*. `verified`; Windows' target `unverified`.
- **mio 1.2.5 `Waker`**: *"Only a single Waker can be active per Poll"*; `EVFILT_USER` on kqueue,
  `eventfd` on Linux. `verified`.
- **Windows**: `WSAPoll` determines *"status of one or more sockets"*, the array *"must contain at least
  one structure with a valid socket"*; Vista and later; *"As of Windows 10 version 2004, when a TCP socket
  fails to connect, (POLLHUP | POLLERR | POLLWRNORM) is indicated"*. libevent's `evutil_socketpair`: on
  Windows AF_UNIX tried, else an emulation over a loopback listener, *"originally from Tor"*, *"does not
  work when localhost is down"*. Python `socket.socketpair`: *"Changed in version 3.5: Windows support
  added"* (no emulation described). `verified`. Whether `WSAPoll` refuses a non-socket handle: the page
  speaks of sockets alone and states no refusal.
- **The status quo** (slices): 246's Python `serve_forever`, *"Poll for shutdown every poll_interval
  seconds"*, 0.5 by default.
- **DJB's self-pipe page** (https://cr.yp.to/docs/selfpipe.html): the fetch failed on a header error;
  not cited.

### Question 3: a child's working folder

- **POSIX Issue 8** (`posix_spawn_file_actions_addchdir.html`): *"as if chdir(path) had been called"*;
  actions run *"in the same order that they were added"*; *"a relative path passed to posix_spawn() will
  be affected by the last chdir or fchdir action"*; a path that cannot be used at the time of the call is
  not an error then; *"First released in Issue 8. Derived from Solaris
  posix_spawn_file_actions_addchdir_np."* `verified`.
- **glibc** (Florian Weimer's patch, libc-alpha, 2018-10-26): NEWS *"enabling posix_spawn and
  posix_spawnp to run the new process in a different directory"*; `GLIBC_2.29` version node. `verified`.
- **Rust** (`Command` docs; `unix.rs` at `master`): `current_dir` with a relative program is *"platform
  specific and unstable"*, `canonicalize` recommended; the fork path calls `chdir` in the child;
  `posix_spawn` used with `addchdir` only where found (the `_np` name tried first, *"more widely
  available"*, then POSIX's), and **not on Apple for a relative program**: *"There is a bug in macOS
  where a relative executable path like "../myprogram" will cause `posix_spawn` to successfully launch
  the program, but erroneously return ENOENT when used with posix_spawn_file_actions_addchdir_np which
  was introduced in macOS 10.15."* On Windows Rust resolves the program itself before
  `CreateProcessW`. `verified`.
- **Go** (`os/exec`; `syscall/exec_linux.go`): *"If Path is relative, it is evaluated relative to
  Dir"*; the child calls `SYS_CHDIR` by raw system call after `fork` and, failing, writes its errno down
  a pipe and exits 253; the child *"must not acquire any locks ... no malloc calls"*. `verified`.
- **Python** (`subprocess` docs and source): *"On POSIX, the function looks for executable ... relative
  to cwd"*; on Windows *"cwd does not override the current working directory"* in the search; errors
  in the child before exec *"re-raised in the parent"*; `posix_spawn` used only when `cwd is None`,
  among other conditions. `verified`.
- **Node** (`child_process` docs): a `cwd` that does not exist *"emits an ENOENT error"*, and *"ENOENT
  is also emitted when the command does not exist"*. `verified`. **A precedent of one number for two
  failures.**
- **`CreateProcessW`** (re-read): `lpCurrentDirectory` *"The full path"*; a name without a folder is
  searched in *"The directory from which the application loaded"* then *"The current directory for the
  parent process"*; a supplied environment block does not carry the `=C:` drive folders, which *"An
  application must manually pass"*. `verified`. Other runtimes on `=X:`: not searched.

### Questions 4 and 5: a stop the program asks, and a connection that sends nothing

- **Go `Server.Close`** (go1.8): *"immediately closes all active net.Listeners and any connections in state
  StateNew, StateActive, or StateIdle"*; `Shutdown` waits *"indefinitely for connections to return to
  idle"*. **Go issue 39919** (2020-06-29, closed 2020-06-30): a handler calling `Shutdown`; fraenkel,
  *"Shutdown() is synchronous"*, the handler still waiting on a call that cannot finish while its own
  connection is active; with `Shutdown` in its own goroutine the reporter got the expected output.
  `verified`. 246's: Python `shutdown()` *"must be called while serve_forever() is running in a
  different thread otherwise it will deadlock"*.
- **Node `server.close()`**: stops accepting and closes connections *"not sending a request or waiting
  for a response"*, idle ones *"before returning"* since v19.0.0; `closeIdleConnections()` and
  `closeAllConnections()` since v18.2.0. `verified`.
- **Go `net.Conn`** (`net.go` at `master`): the deadline quoted in item 7 above; *"an error that wraps
  os.ErrDeadlineExceeded"*; *"An idle timeout can be implemented by repeatedly extending the
  deadline"*; `SetReadDeadline` *"for future Read calls and any currently-blocked Read call"*; `Close`,
  *"Any blocked Read or Write operations will be unblocked and return errors."* `verified`.
- **Go `http.Server`**: `ReadHeaderTimeout` *"the amount of time allowed to read request headers"*,
  `IdleTimeout` *"to wait for the next request when keep-alives are enabled"*; go1.8 *"adds
  configuration options ReadHeaderTimeout and IdleTimeout and documents WriteTimeout"*; issue 14204
  (opened 2016-02-02 for `IdleTimeout`, closed 2016-10-29, commit c7e0dda of 2016-10-26 *"add
  Server.ReadHeaderTimeout, IdleTimeout"*). The pages give no slow-client reason. `verified`.
- **Node `http.Server`**: `headersTimeout` added v11.3.0 and v10.14.0, *"It must be set to a non-zero
  value ... to protect against potential Denial-of-Service attacks in case the server is deployed without
  a reverse proxy in front"*; its security release (Nov. 2018, CVE-2018-12122, *"sending headers very
  slowly keeping HTTP or HTTPS connections and associated resources alive"*): *"A timeout of 40 seconds
  now applies"*. `requestTimeout` added v14.11.0 for CVE-2020-8251 (*"delayed requests submission which
  can make the server unable to accept new connections"*), *"default value of 0 which means it is
  disabled"*; default 300 s since v18.0.0. `server.timeout` default *"changed from 120s to 0"* in v13.0.0.
  `verified`.
- **Python `socket`**: `settimeout` per socket; *"At the operating system level, sockets in timeout mode
  are internally set in non-blocking mode."* `verified`. **00-shared.md's inference (a deadline as the
  layer's own `poll` bound over non-blocking sockets, no `SO_RCVTIMEO`) is Python's documented
  implementation.**
- **A join that does not wait**: Rust `is_finished`, stable 1.61.0, *"supports implementing a
  non-blocking join operation, by checking `is_finished`, and calling `join` if it returns `true`"*;
  Java `isAlive()` (Thread since 1.0; no *since* on the method), `join(Duration)` since 19, *"just tests
  if the thread has terminated"* at a duration of zero or less; glibc `pthread_tryjoin_np`, EBUSY,
  glibc 2.3.3, GNU. `verified`. **Every one stands beside an unchanged blocking join.**

### What each corrected

| Who | When | Change | Page that says why |
|---|---|---|---|
| Node | v10.14.0 / v11.3.0, Nov. 2018 (releases *"on, or shortly after, November 27th, 2018 UTC"*) | `headersTimeout` added, 40 s | Nov. 2018 security release, CVE-2018-12122 Slowloris |
| Node | v14.11.0, Sep. 2020 (releases *"on or shortly after Tuesday, September 15, 2020"*) | `requestTimeout` added, default 0 | Sep. 2020 security release, CVE-2020-8251 |
| Node | v18.0.0 | `requestTimeout` default 0 to 300 s | the API page's history (no reason given) |
| Node | v19.4.0, v18.14.0 | `headersTimeout` default min(60 s, `requestTimeout`) | the API page's history |
| Node | v13.0.0 | `server.timeout` 120 s to 0 | the API page's history |
| Node | v19.0.0 | `close()` closes idle connections | the API page's history |
| Go | go1.8 (issue closed 2016-10-29) | `ReadHeaderTimeout`, `IdleTimeout`, `Shutdown`, `Close` | go1.8 notes; issue 14204 (no reason beyond limiting keep-alive idle time) |
| OpenJDK | Java 16 | `libjsig`'s `signal` and `sigset` deprecated | Java SE 21 page (the issue, 403) |
| POSIX | Issue 8 | spawn's `chdir`/`fchdir` actions | its page: derived from Solaris's `_np` |
| glibc | `GLIBC_2.29` (patch 2018-10-26) | `posix_spawn_file_actions_addchdir_np` | the patch's NEWS |
| Apple | macOS 10.15 (Rust's comment) | the `_np` form, with a relative-program bug | Rust's `unix.rs` |
| dyld | before `dyld-655.1` | interposing without `DYLD_INSERT_LIBRARIES` | rdar 19315404, a code comment |

### Where the rule is written

Every page read today is **a library's or runtime's documentation or source, or the platform's**:
Go's `pkg.go.dev` and runtime source, Rust's `std` docs and source, Java's API reference and HotSpot
guides, Node's API reference and security blog, Python's library reference and source, POSIX's function
pages, Microsoft's Win32 pages, libuv's, mio's, libevent's, compiler-rt's and dyld's documentation or
source. **No language reference was searched** today (Go's specification, the Rust Reference, the JLS,
Python's language reference, ECMAScript), so no sentence here says they are silent; 247's reading
stands (C and Ada state their handler rules in the language's reference). A deadline, a server's stop, a
child's folder and a non-waiting join are each told on a package's page in every precedent read; Heroes'
spec names the stop doors and the join (00-shared.md), which is a placement the pages read do not show,
neither for nor against.

---

## argument

Every mechanism asked about has shipped elsewhere; the departures are where routes go beyond it.
Runtimes do define the C library's installers (OpenJDK's libjsig, Android's libsigchain, the
sanitizers), each a short list, none complete; on Darwin each needed an inserted or dependent dylib with
`__interpose`, flat namespace by environment, or a fake. ART forces SA_ONSTACK; Go turns it on in
handlers it finds. One poller per process with one wake (Go, mio, libuv per loop) is the norm;
per-waiter descriptors are not. Spawn's folder action came from Solaris to glibc 2.29 and POSIX Issue 8;
Rust refuses to promise a relative program's folder. Node added header and request deadlines as CVE
fixes. Non-waiting joins sit beside join, never replace it. (120 words, counted by hand)

---

## condition

- **dyld honouring an executable's own `__interpose`** (a run on this Mac, or dyld4's source saying
  so) would lift my objection to route I from the executable alone on Darwin; my reading of
  dyld-655.1's loop bounds would then be stale.
- **A runtime that defines every installer its C library exports** (`sysv_signal`, `sigset`,
  `__sigaction`, Darwin's `_nobind` forms) and documents why would make route I's completeness a
  precedent instead of a departure.
- **A runtime or toolchain that refuses a program whose library also defines `sigaction`** would make
  the widened `ffi_defined_elsewhere` precedented; compiler-rt's co-existence protocol is the contrary
  evidence read.
- **A runtime that keeps a wake descriptor per waiting thread and says why** would weigh for route (a)
  over (b) and (e).
- **A thread library that changed its join to answer "not yet"** would remove my objection to the
  00-shared.md shape of question 5's join.
- **A page saying Darwin's `addchdir_np` relative-program bug is fixed** would weaken the caution Rust's
  comment gives for question 3's order on Darwin.
- **Go's or mio's stated reason for one wake per poller** (unread) could add a cost the routes here
  have not priced.

---

## search

**How**: WebSearch to find a primary page, WebFetch to read it; a search result's summary was never
cited. Every page fetched on 2026-11-14; each URL is in § Log, read whole or at the offsets named.

**Queries (WebSearch)**, in order: *Oracle Java troubleshooting guide signal chaining libjsig
LD_PRELOAD sigaction signal sigset*; *android art sigchainlib sigchain.cc sigaction signal sigprocmask
interpose "libsigchain"*; *dyld source interposing "__interpose" only allowed images loaded at launch
main executable dyld4 Loader comment*; *JDK 16 libjsig deprecate signal sigset JBS issue "libjsig"
deprecation warning sigaction only*; *Go 1.1 release notes runtime network poller integrated scheduler
"net" package epoll kqueue*; *glibc 2.29 NEWS "posix_spawn_file_actions_addchdir_np" release
announcement*; *golang issue calling Server.Shutdown from inside handler deadlock waits for active
connections net/http*; *Node.js November 2018 security releases Slowloris HTTP Denial of Service
CVE-2018-12122 headersTimeout*; *golang issue "ReadHeaderTimeout" proposal net/http Server separate
header timeout go1.8 issue 14204 OR 16100*.

**Languages, projects and sources swept**: OpenJDK (`libjsig` source, Java SE 21 signal chaining, Java
SE 26 troubleshooting, Java SE 21 `Thread`); Android ART (`sigchain.cc`, `Android.bp`); LLVM compiler-rt
(`interception.h`, `sanitizer_signal_interceptors.inc`, the common-flags wiki); Apple dyld
(`dyld-interposing.h`, `dyld.cpp` at `dyld-655.1`, `doc/dyld4.md`); Go (`os/signal`, `runtime/netpoll.go`,
`netpoll_epoll.go`, `netpoll_kqueue.go`, `netpoll_kqueue_event.go`, `net/net.go`, `net/http`, `os/exec`,
`syscall/exec_linux.go`, go1.1 and go1.8 notes, issues 14204 and 39919); libuv (`async.html`,
`design.html`, `src/unix/async.c`, `src/win/async.c`); mio (`Waker`); libevent (`evutil.c`); Python
(`socket`, `subprocess` docs, `Lib/subprocess.py`); Rust (`std::process::Command`, `JoinHandle`,
`library/std/src/sys/process/unix/unix.rs`); Node (`child_process`, `http`, security releases of Nov.
2018 and Sep. 2020); POSIX Issue 8 (`posix_spawn_file_actions_addchdir`); glibc (the libc-alpha patch of
2018-10-26); Linux man-pages (`pthread_tryjoin_np(3)`); Microsoft (`WSAPoll`, `CreateProcessW`).

**Pages that failed, and what replaced them**: Apple's archived `dyld(1)` (404); the Darwin-dev answer
on `__interpose` and `dlopen` (502; its search summary seen, not cited); opensource.apple.com's
`dyld-655.1` (404; Apple's GitHub mirror read); JDK-8257572 and its review on `mail.openjdk.org` (403
each); `git.crux.nu`'s glibc commit (403; the libc-alpha patch read); https://cr.yp.to/docs/selfpipe.html
(the fetch's parse error); `docs.oracle.com/en/java/javase/21/troubleshoot/handle-signals-and-exceptions.html`
(returned an index page; the Java SE 26 page read); the first fetch of Python's `socket` page cut the
timeout note mid-word, the second read its two subsections; the first fetch of Go's `os/signal`
paraphrased its c-archive paragraph, the second quoted it.

**Not searched**: .NET's and the JNI specification's word on native handlers calling back; UCRT's
installers and compiler-rt's Windows interception (`interception_win.cpp`); dyld4's source; Go's and
mio's reasons for one wake per poller; libuv's `uv_spawn` and Go's Windows `Dir`; other runtimes'
handling of `=X:` entries; glibc's `addfchdir_np` patch (seen in a search summary only); Solaris's and
Darwin's manual pages for `addchdir_np`; Go's commit c7e0dda's body; Node's pull requests for
`headersTimeout` and `requestTimeout`; the language references named in § Where the rule is written;
Oberon, Pascal-P4, Cyclone, Hylo, Idris, K/APL, cfront, Zig, Nim and Erlang for these five mechanisms
(none known to this seat to own one; an assumption, not a finding).

**Not run**: a grep of `docs/panel/` for earlier historian lines on these names (no shell). Before any
line below is filed, the synthesis runs `grep -n -i
'libjsig\|sigchain\|interception.h\|19315404\|netpollBreak\|uv_async_send\|single Waker\|evutil_socketpair\|addchdir\|39919\|14204\|CVE-2018-12122\|CVE-2020-8251\|is_finished\|tryjoin'
docs/panel/*.md`.

---

## Candidate prior-art lines, per route

In `.claude/rules/records.md`'s two forms, each only if the sitting adopts the route it names, the
historian marked the page `verified` above, and the session filing it re-reads the page. A link to
another project's issue goes through `redirect.github.com`.

**Q1, route I (installers defined by the runtime)**

- `- **Reached first by OpenJDK**: [Signal Chaining, Java SE 21](https://docs.oracle.com/en/java/javase/21/vm/signal-chaining.html): libjsig, linked or preloaded before libc, intercepts signal(), sigset() and sigaction() so that they save a new handler, chained behind the HotSpot VM's, instead of replacing the VM's, and the interposed calls return the saved handlers.`
- `- **Reached first by Android ART**: [sigchain.cc](https://android.googlesource.com/platform/art/+/refs/heads/main/sigchainlib/sigchain.cc): libsigchain implements wrapper functions for signal, sigaction and sigprocmask, and records a user's action for a claimed signal without passing it to the kernel.`
- `- **Reached first by LLVM compiler-rt**: [interception.h](https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/interception/interception.h): on Linux an interceptor is a function of the same name with the real one found by dlsym, aliased weakly so that up to three interceptors of one function may co-exist.`

**Q1, I-onstack**

- `- **Reached first by Android ART**: [sigchain.cc](https://android.googlesource.com/platform/art/+/refs/heads/main/sigchainlib/sigchain.cc): SA_ONSTACK is always silently enabled, and its signal wrapper installs with SA_RESTART | SA_ONSTACK.`
- `- **Reached first by Go**: [os/signal](https://pkg.go.dev/os/signal): non-Go code that installs signal handlers must use the SA_ONSTACK flag with sigaction, and in a non-Go program that calls Go code, where a signal handler already exists the Go runtime turns on the SA_ONSTACK flag and otherwise keeps the handler.`

**Q1, I-dylib**

- `- **Reached first by LLVM compiler-rt**: [interception.h](https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/interception/interception.h): on Mac OS, where the two-level namespace makes replacement functions invisible to other libraries, the sanitizer creates a dylib with a __DATA,__interpose section associating library functions with their wrappers.`
- `- **Reached first by Apple dyld**: [dyld.cpp at dyld-655.1](https://raw.githubusercontent.com/apple-oss-distributions/dyld/dyld-655.1/src/dyld.cpp): interposing is registered for inserted libraries and, under rdar 19315404, for images outside the shared cache without DYLD_INSERT_LIBRARIES.`

**Q2, (e) one poller, (b) one wake per poller**

- `- **Reached first by Go**: [runtime/netpoll.go](https://raw.githubusercontent.com/golang/go/master/src/runtime/netpoll.go): one integrated network poller per platform, each descriptor holding the goroutine waiting to read or write and a read and a write deadline with its timer, a deadline set in the past unblocking pending I/O.`
- `- **Reached first by Go**: [runtime/netpoll_epoll.go](https://raw.githubusercontent.com/golang/go/master/src/runtime/netpoll_epoll.go): the poller is woken by a write to one eventfd, a compare-and-swap avoiding duplicate calls.`
- `- **Reached first by libuv**: [uv_async_t](https://docs.libuv.org/en/v1.x/async.html): uv_async_send wakes the loop from any thread, is async-signal-safe, and coalesces calls.`
- `- **Reached first by mio**: [Waker](https://docs.rs/mio/latest/mio/struct.Waker.html): only a single Waker can be active per Poll; it uses EVFILT_USER on kqueue and eventfd on Linux.`

**Q2, Windows loopback pair**

- `- **Reached first by libevent**: [evutil.c](https://raw.githubusercontent.com/libevent/libevent/master/evutil.c): on Windows an AF_UNIX socket pair is tried, and otherwise a pair is built over a loopback listener, code originally from Tor, which does not work when localhost is down.`

**Q3, the folder door**

- `- **Reached first by POSIX**: [posix_spawn_file_actions_addchdir](https://pubs.opengroup.org/onlinepubs/9799919799/functions/posix_spawn_file_actions_addchdir.html): a chdir action performed in order with the other file actions, a relative path passed to posix_spawn being affected by the last chdir or fchdir action; first released in Issue 8, derived from Solaris posix_spawn_file_actions_addchdir_np.`
- `- **Reached first by glibc**: [posix: New function posix_spawn_file_actions_addchdir_np](https://sourceware.org/legacy-ml/libc-alpha/2018-10/msg00579.html): a function under GLIBC_2.29 enabling posix_spawn and posix_spawnp to run the new process in a different directory.`
- `- **Reached first by Go**: [os/exec](https://pkg.go.dev/os/exec): Cmd.Dir specifies the command's working directory, and a relative Path is evaluated relative to Dir.`
- `- **Reached first by Python**: [subprocess](https://docs.python.org/3/library/subprocess.html): on POSIX the executable is looked for relative to cwd when its path is relative, on Windows cwd does not override the current directory in that search, and exceptions in the child before the program starts are re-raised in the parent.`
- `- **Reached first by Rust**: [Command::current_dir](https://doc.rust-lang.org/std/process/struct.Command.html): whether a relative program path is read against the parent's directory or current_dir is platform specific and unstable, and canonicalize is recommended.`
- `- **Reached first by Rust**: [unix.rs](https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/sys/process/unix/unix.rs): posix_spawn_file_actions_addchdir_np, introduced in macOS 10.15, launches a relative executable such as "../myprogram" but returns ENOENT, so the fork path is taken there.`
- a missing folder told apart: `- **Reached first by Node.js**: [child_process](https://nodejs.org/api/child_process.html): a cwd that does not exist emits ENOENT, which is also emitted when the command does not exist.`

**Q4, a door that asks and does not wait**

- `- **Reached first by Go**: [issue 39919](https://redirect.github.com/golang/go/issues/39919): Shutdown is synchronous, so a handler that calls it waits on a call that cannot finish while its own connection is active; called from a separate goroutine it behaved as expected.`
- `- **Reached first by Go**: [net/http, Server.Close](https://pkg.go.dev/net/http): Close immediately closes all active listeners and connections in state StateNew, StateActive or StateIdle, Shutdown being the graceful form.`

**Q5, deadlines**

- `- **Reached first by Go**: [net.Conn](https://raw.githubusercontent.com/golang/go/master/src/net/net.go): a deadline is an absolute time applying to all future and pending I/O, after which calls return an error wrapping os.ErrDeadlineExceeded, and an idle timeout is a deadline extended after each successful call.`
- `- **Reached first by Go**: [Go 1.8 release notes](https://go.dev/doc/go1.8): the Server adds configuration options ReadHeaderTimeout and IdleTimeout and documents WriteTimeout.`
- `- **Reached first by Node.js**: [November 2018 security releases](https://nodejs.org/en/blog/vulnerability/november-2018-security-releases): a client sending headers very slowly could keep connections and their resources alive (CVE-2018-12122), and a 40-second limit on receiving headers, server.headersTimeout, was added.`
- `- **Reached first by Node.js**: [September 2020 security releases](https://nodejs.org/en/blog/vulnerability/september-2020-security-releases): delayed request submission could leave the server unable to accept new connections (CVE-2020-8251), and an option requestTimeout was added, disabled by default.`
- `- **Reached first by Python**: [socket, Notes on socket timeouts](https://docs.python.org/3/library/socket.html): a socket in timeout mode is internally set in non-blocking mode at the operating system level.`

**Q5, a join that does not wait, beside join**

- `- **Reached first by Rust**: [JoinHandle::is_finished](https://doc.rust-lang.org/std/thread/struct.JoinHandle.html): stable since 1.61.0, it does not block and supports a non-blocking join by checking it and calling join when it returns true.`
- `- **Reached first by glibc**: [pthread_tryjoin_np(3)](https://man7.org/linux/man-pages/man3/pthread_tryjoin_np.3.html): returns EBUSY instead of blocking when the thread has not terminated, since glibc 2.3.3, a GNU extension.`
- `- **Reached first by Java**: [Thread.join(Duration)](https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Thread.html): since 19, a duration of zero or less does not wait and only tests whether the thread has terminated.`

**Not candidate lines** (for the sitting's file only): `CreateProcessW` and `WSAPoll` (platform facts,
not another project's decision); the sanitizer flags wiki (a tool's option, Heroes adopts none of it);
OpenJDK's Java 16 deprecation (its reason unread); Go 1.1's notes (they name no poller).

---

## Log, as I went (every page read 2026-11-14)

### Question 1

- **Oracle, Java SE 21, *Signal Chaining***, https://docs.oracle.com/en/java/javase/21/vm/signal-chaining.html:
  *"Your application can link and load the `libjsig.so` shared library before the
  `libc/libthread/libpthread` library. This library ensures that calls such as `signal()`, `sigset()`,
  and `sigaction()` are intercepted and don't replace the signal handlers that are used by the HotSpot
  VM, if the handlers conflict with the signal handlers that are already installed by HotSpot VM.
  Instead, these calls save the new signal handlers."*; *"The interposed `signal()`, `sigset()`, and
  `sigaction()` calls return the saved signal handlers, not the signal handlers installed by the HotSpot
  VM and are seen by the operating system."*; Linux: `cc -L libjvm.so-directory -ljsig -ljvm
  java_application.c`, or `LD_PRELOAD`; macOS: *"`DYLD_INSERT_LIBRARIES`: Preloads the specified
  libraries instead of the `LD_PRELOAD` environment variable"*, *"`DYLD_FORCE_FLAT_NAMESPACE`: Enables
  functions in the `libjsig` library and replaces the OS implementations, because of macOS's two-level
  namespace (a symbol's fully qualified name includes its library)"*; *"As of Java 16 the use of the
  `signal` and `sigset` functions are deprecated, and support for those functions will be removed in a
  future release. Use the `sigaction` function instead."*; *"The `SIGQUIT`, `SIGTERM`, `SIGINT`, and
  `SIGHUP` signals cannot be chained."* `verified`.
- **Oracle, Java SE 26, *Handle Signals and Exceptions***,
  https://docs.oracle.com/en/java/javase/26/troubleshoot/handle-signals-exceptions.html: *"This library
  ensures that calls to `sigaction()` are intercepted and don't replace the signal handlers that are
  used by the HotSpot VM"*; *"The interposed `sigaction()` call returns the saved signal handlers"*; the
  same macOS variables; **no sentence on native handlers calling JNI or Java**, none on `SA_ONSTACK`.
  `verified`. (The Java SE 21 URL of the same page returned an index; this one was read instead.)
- **OpenJDK `jsig.c`**, https://raw.githubusercontent.com/openjdk/jdk/master/src/java.base/unix/native/libjsig/jsig.c:
  *"should be loaded before libc & libthread to interpose the signal handler installation
  functions"*; defines `signal`, `sigset`, `sigaction`, `JVM_begin_signal_setting`,
  `JVM_end_signal_setting`, `JVM_get_signal_action`; *"sigaction is the only truly supported API"*;
  `signal()` and `sigset()` warn they were *"deprecated in version 16.0"*; under `_ALLBSD_SOURCE`,
  *"sigset() is not supported by BSD"* and exit; macOS: *"On macosx, the OS implementation of signal calls
  sigaction"*, a per-thread flag to *"prevent reentry deadlock"* (JDK-8072147); *"Save the handler.
  Don't really install it."*; *"there is only one modifying sigaction call for a specific signal"*; a
  mutex *"Used to synchronize the installation of signal handlers"*; nothing on `SIG_HOLD`. `verified`.
- **Android ART `sigchain.cc`**, https://android.googlesource.com/platform/art/+/refs/heads/main/sigchainlib/sigchain.cc
  (read twice): *"libsigchain provides an interception layer for signal handlers, to allow ART and others
  to give their signal handlers the first stab at handling signals before passing them on to user code.
  It implements wrapper functions for signal, sigaction, and sigprocmask, and a handler that forwards
  signals appropriately."*; *"In our handler, we start off with all signals blocked, fetch the original
  signal mask from the passed in ucontext, and then adjust our signal mask appropriately for the user
  handler."*; *"SA_NODEFER: unimplemented, we can manually change the signal mask appropriately."*,
  *"~SA_ONSTACK: always silently enable this"*, *"SA_RESETHAND: unimplemented, but we can probably do
  this?"*, *"~SA_RESTART: unimplemented"*; `sigaction` calls `InitializeSignalChain()` then
  `__sigaction(signal, new_action, old_action, linked_sigaction)`; *"If this signal has been claimed as a
  signal chain, record the user's action but don't pass it on to the kernel."*; `signal` builds
  `sa.sa_flags = SA_RESTART | SA_ONSTACK` and, claimed, returns the previous user handler; the real
  functions by `dlopen` of libc and `dlsym`, `RTLD_DEFAULT` as fallback; *"Darwin has an #error when
  ucontext.h is included without _XOPEN_SOURCE defined."* Defined: `sigaction`, `sigaction64` (Bionic),
  `signal`, `bsd_signal` (non-LP64), `sigprocmask`, `sigprocmask64` (Bionic); not `pthread_sigmask`.
  `verified`.
- **Android ART `Android.bp`**, https://android.googlesource.com/platform/art/+/refs/heads/main/sigchainlib/Android.bp:
  *"Make libsigchain symbols global, so that an app library which is loaded in a classloader linker
  namespace looks for libsigchain symbols before libc."*; *"-z,global marks the binary with the
  DF_1_GLOBAL flag"*; *"Create a fake version of libsigchain which expose the necessary symbols but throws
  when called. This can be used to get static binaries which don't need the real functionality of the sig
  chain but need to please the linker."*; per-target sources, `sigchain.cc` on Linux and
  `sigchain_fake.cc` on Darwin (the fetch's reading); visibility `frameworks/base/cmds/app_process`.
  `verified`.
- **compiler-rt `interception.h`**, https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/interception/interception.h:
  *"To replace system functions on Linux we just need to declare functions with the same names in our
  library and then obtain the real function pointers using dlsym()."*; *"a user may also intercept some
  of the functions we intercept. To allow for up to 3 interceptors (including ours) of a given function
  "func", the interceptor implementation is in ___interceptor_func, which is aliased by a weak function
  __interceptor_func, which in turn is aliased (via a trampoline) by weak wrapper function "func"."*; a
  foreign interceptor should *"provide a non-weak function "func" that performs interception"*; *"With
  this protocol, sanitizer interceptors, foreign user interceptors, and foreign interceptors of other
  dynamic analysis runtimes, or any combination thereof, may co-exist simultaneously."*; *"This is not so
  on Mac OS, where the two-level namespace makes our replacement functions invisible to other libraries.
  This may be overcomed using the DYLD_FORCE_FLAT_NAMESPACE, but some errors loading the shared libraries
  in Chromium were noticed when doing so. Instead we create a dylib containing a __DATA,__interpose
  section that associates library functions with their wrappers. When this dylib is preloaded before an
  executable using DYLD_INSERT_LIBRARIES, it routes all the calls to interposed functions done through
  stubs to the wrapper functions."*; *"As it's decided at compile time which functions are to be
  intercepted on Mac, INTERCEPT_FUNCTION() is effectively a no-op on this system."*; Windows:
  *"Implemented in interception_linux.cpp on non-Windows targets and interception_win.cpp on Windows"*,
  no comment on the mechanism. `verified`.
- **compiler-rt `sanitizer_signal_interceptors.inc`**, https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/sanitizer_common/sanitizer_signal_interceptors.inc:
  `INTERCEPTOR(uptr, bsd_signal, ...)`, `INTERCEPTOR(uptr, signal, ...)`, `INTERCEPTOR(int,
  sigaction_symname, ...)`; for `kHandleSignalExclusive`, `signal`: *"The user can neither view nor
  change the signal handler, regardless of the cloak_sanitizer_signal_handlers setting. This differs from
  sigaction()."*; `sigaction`: *"If false, the user can view but not change the signal handler. This
  differs from signal()."*; *"If the user sets a signal handler, it becomes uncloaked, even if they reuse
  a sanitizer's signal handler."* `verified`.
- **Sanitizer common flags**, https://github.com/google/sanitizers/wiki/SanitizerCommonFlags:
  `handle_segv` true, *"Controls custom tool's SEGV handler (0 - do not registers the handler, 1 -
  register the handler and allow user to set own, 2 - registers the handler and block user from changing
  it). Ignored on Windows."*; the same for `handle_sigbus`, `handle_sigill`, `handle_sigfpe` (true) and
  `handle_abort` (false); `use_sigaltstack` true, *"If set, uses alternate stack for signal handling."*
  `verified`.
- **Apple `dyld-interposing.h`**, https://raw.githubusercontent.com/apple-oss-distributions/dyld/main/include/mach-o/dyld-interposing.h:
  `#define DYLD_INTERPOSE(_replacement,_replacee)`, a static `used` pair in
  `__DATA,__interpose,interposing`, `__ptrauth` fields where available; no sentence on which images.
  `verified`.
- **Apple dyld-655.1 `src/dyld.cpp`**, https://raw.githubusercontent.com/apple-oss-distributions/dyld/dyld-655.1/src/dyld.cpp
  (offsets 0, 100000 and 200000): *"// only INSERTED libraries can interpose"*, *"// register interposing
  info after all inserted libraries are bound so chaining works"*, `for(unsigned int i=0; i <
  sInsertedDylibCount; ++i)` with `ImageLoader* image = sAllImages[i+1];`; *"// <rdar://problem/19315404>
  dyld should support interposition even without DYLD_INSERT_LIBRARIES"*, `for (long
  i=sInsertedDylibCount+1; i < sAllImages.size(); ++i)` with `sAllImages[i]` and `if (
  image->inSharedCache() )` skipped; *"// Accelerator tables cannot be used with implicit interposing,
  so relaunch with accelerator tables disabled"*; *"// apply interposing to initial set of images"*;
  `sInsertedDylibCount = sAllImages.size()-1;`, *"so that a flat search will look at inserted libraries,
  then main, then others"*. `verified`.
- **Apple dyld `doc/dyld4.md`**, https://raw.githubusercontent.com/apple-oss-distributions/dyld/main/doc/dyld4.md:
  no sentence holds *interpos*, `DYLD_INSERT_LIBRARIES` or which images may interpose. `verified` as an
  absence in that file only.
- **Go `os/signal`**, https://pkg.go.dev/os/signal (re-read twice for sentences 246 and 247 did not
  quote): *"If the non-Go code installs any signal handlers, it must use the SA_ONSTACK flag with
  sigaction."*; *"Otherwise some Go run-time panics will not occur as expected."*; *"Otherwise, the Go
  handler removes itself, unblocks the signal, and raises it again"*; under *Non-Go programs that call
  Go code*, re-fetched for the exact words: *"If there is an existing signal handler, the Go runtime will
  turn on the SA_ONSTACK flag and otherwise keep the signal handler."* and *"If the Go runtime sees an
  existing signal handler for the SIGCANCEL or SIGSETXID signals (which are used only on Linux), it will
  turn on the SA_ONSTACK flag and otherwise keep the signal handler."* The first fetch's other lines on
  c-archive and c-shared were the tool's paraphrase and are not quoted. `verified`.

### Question 2

- **Go `runtime/netpoll.go`**, https://raw.githubusercontent.com/golang/go/master/src/runtime/netpoll.go:
  *"Integrated network poller (platform-independent part). A particular implementation
  (epoll/kqueue/port/AIX/Windows) must define the following functions"*, including *"func netpollBreak()
  Wake up the network poller, assumed to be blocked in netpoll."*; `rg atomic.Uintptr // pdReady, pdWait,
  G waiting for read or pdNil`; *"G pointer - the goroutine is blocked on the semaphore; io notification
  or timeout/close changes the state to pdReady or pdNil respectively and unparks the goroutine."*; `rd
  int64 // read deadline (a nanotime in the future, -1 when expired)`, `rt timer // read deadline timer`;
  *"If we set the new deadline in the past, unblock currently pending IO if any."*;
  `poll_runtime_pollUnblock` carries no comment. `verified`.
- **Go `netpoll_epoll.go`**, https://raw.githubusercontent.com/golang/go/master/src/runtime/netpoll_epoll.go:
  `efd, errno := linux.Eventfd(0, linux.EFD_CLOEXEC|linux.EFD_NONBLOCK)`; `netpollWakeSig atomic.Uint32
  // used to avoid duplicate calls of netpollBreak`; *"netpollBreak interrupts an epollwait."*, *"Failing
  to cas indicates there is an in-flight wakeup, so we're done here."* `verified`.
- **Go `netpoll_kqueue.go`**, https://raw.githubusercontent.com/golang/go/master/src/runtime/netpoll_kqueue.go:
  `//go:build darwin || dragonfly || freebsd || netbsd || openbsd`; `addWakeupEvent(kq)`; *"netpollBreak
  interrupts a kevent."* `verified`.
- **Go `netpoll_kqueue_event.go`**, https://raw.githubusercontent.com/golang/go/master/src/runtime/netpoll_kqueue_event.go:
  `//go:build darwin || dragonfly || freebsd`; *"Copyright 2024"*; `filter: _EVFILT_USER, flags: _EV_ADD |
  _EV_CLEAR` and `fflags: _NOTE_TRIGGER`; *"Magic number of identifier used for EVFILT_USER."* That
  netbsd and openbsd take a pipe is my reading of the two build lines. `verified`.
- **Go 1.1 release notes**, https://go.dev/doc/go1.1: *"Due to tighter coupling of the run-time and
  network libraries, fewer context switches are required on network operations."*; no poller named, no
  date. `verified`.
- **libuv `uv_async_t`**, https://docs.libuv.org/en/v1.x/async.html: *"Async handles allow the user to
  "wakeup" the event loop and get a callback called from another thread."*; *"libuv will coalesce calls
  to uv_async_send(), that is, not every call to it will yield an execution of the callback."*; *"It's
  safe to call this function from any thread. The callback will be called on the loop thread."*;
  *"uv_async_send() is async-signal-safe. It's safe to call this function from a signal handler."*;
  *"Changed in version 1.53.0: uv_async_send() and uv_async_cb are sequentially consistent."*
  `verified`.
- **libuv design overview**, https://docs.libuv.org/en/v1.x/design.html: *"It establishes the content for
  all I/O operations, and it's meant to be tied to a single thread. One can run multiple event loops as
  long as each runs in a different thread."*; *"epoll on Linux, kqueue on OSX and other BSDs, event
  ports on SunOS and IOCP on Windows"*; *"If none of the above cases matches, the timeout of the closest
  timer is taken, or if there are no active timers, infinity."*; *"network I/O is always performed in a
  single thread, each loop's thread"*. `verified`.
- **libuv `src/unix/async.c`**, https://raw.githubusercontent.com/libuv/libuv/v1.x/src/unix/async.c:
  `#ifdef __linux__ err = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);`; `#elif UV__KQUEUE_EVFILT_USER`, a
  runtime detection, `EV_SET(&ev, err, EVFILT_USER, EV_ADD | EV_CLEAR, 0, 0, 0);`, else
  `uv__make_pipe(pipefd, UV_NONBLOCK_PIPE)`; `#else` a pipe; *"When using EVFILT_USER event to wake up the
  kqueue, this event must be registered beforehand."*; no comment on why `eventfd` over a pipe.
  `verified`.
- **libuv `src/win/async.c`**, https://raw.githubusercontent.com/libuv/libuv/v1.x/src/win/async.c:
  `uv__async_notify` calls `POST_COMPLETION_FOR_REQ(loop, &handle->async_req);`, undefined in the file;
  comments speak of *"an IOCP notification"*. `verified` for the line; the IOCP target `unverified`.
- **mio `Waker`**, https://docs.rs/mio/latest/mio/struct.Waker.html (mio 1.2.5): *"Waker allows
  cross-thread waking of Poll."*; *"Only a single Waker can be active per Poll"*; *"On platforms that
  support kqueue this will use the EVFILT_USER event filter"*; *"On Linux it uses eventfd."* `verified`.
- **Microsoft `WSAPoll`**, https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsapoll
  (`updated_at` 2024-02-22): *"The WSAPoll function determines status of one or more sockets."*; *"An
  array of one or more POLLFD structures specifying the set of sockets for which status is requested.
  The array must contain at least one structure with a valid socket."*; timeout *"Less than zero"*, *"Wait
  indefinitely"*; *"The WSAPoll function is defined on Windows Vista and later."*; *"As of Windows 10
  version 2004, when a TCP socket fails to connect, (POLLHUP | POLLERR | POLLWRNORM) is indicated."*
  `verified`.
- **libevent `evutil.c`**, https://raw.githubusercontent.com/libevent/libevent/master/evutil.c:
  `#if defined(_WIN32)` calls `evutil_win_socketpair`; *"If the AF_UNIX socket works, we will change the
  family to AF_UNIX forcely."*; *"Win10 does not support AF_UNIX socket of a type other than SOCK_STREAM
  still now."*; `evutil_ersatz_socketpair_`: *"This code is originally from Tor. Used with permission."*,
  *"This socketpair does not work when localhost is down. So it's really not the same thing at all. But
  it's close enough for now"*; `htonl(INADDR_LOOPBACK)`, port 0. `verified`.
- **Python `socket`**, https://docs.python.org/3/library/socket.html (offsets 0 and 100000):
  `socketpair`, *"Changed in version 3.5: Windows support added."*; `settimeout`, *"Set a timeout on
  blocking socket operations ... subsequent socket operations will raise a timeout exception if the
  timeout period value has elapsed before the operation has completed"*; *"At the operating system
  level, sockets in timeout mode are internally set in non-blocking mode."*; accept: *"if the listening
  socket is in blocking mode or in timeout mode, the socket returned by accept() is in blocking mode"*.
  `verified`.

### Question 3

- **POSIX Issue 8**, https://pubs.opengroup.org/onlinepubs/9799919799/functions/posix_spawn_file_actions_addchdir.html:
  *"The posix_spawn_file_actions_addchdir() function shall add a chdir action to the object referenced
  by file_actions that shall cause the working directory to be set to path (as if chdir(path) had been
  called)"*; *"A relative path shall be interpreted in relation to the working directory determined by
  any prior actions."*; *"File actions are performed in a new process created by posix_spawn() or
  posix_spawnp() in the same order that they were added to the file actions object. Likewise, a relative
  path passed to posix_spawn() will be affected by the last chdir or fchdir action in the file action
  list."*; *"Changing the working directory of a child process can be useful when invoking utilities such
  as pax."*; *"It shall not be considered an error for the path or fildes argument passed to these
  functions to specify a pathname or file descriptor for which the specified operation could not be
  performed at the time of the call."*; *"First released in Issue 8. Derived from Solaris
  posix_spawn_file_actions_addchdir_np."* `verified`.
- **glibc patch**, https://sourceware.org/legacy-ml/libc-alpha/2018-10/msg00579.html: Florian Weimer,
  Fri, 26 Oct 2018, *"[PATCH] posix: New function posix_spawn_file_actions_addchdir_np [BZ #17405]"*;
  *"I'm not adding documentation in this patch because none exists for the posix_spawn
  functionality."*; *"If we agree that this is the way to go, I will add further such functions (fchdir,
  chroot, perhaps more)."*; NEWS *"The posix_spawn_file_actions_addchdir_np function has been added,
  enabling posix_spawn and posix_spawnp to run the new process in a different directory."*; `+
  GLIBC_2.29 {` and `+GLIBC_2.29 posix_spawn_file_actions_addchdir_np F`. `verified`.
- **Rust `Command`**, https://doc.rust-lang.org/std/process/struct.Command.html: *"If the program path is
  relative (e.g., "./script.sh"), it's ambiguous whether it should be interpreted relative to the
  parent's working directory or relative to current_dir. The behavior in this case is platform specific
  and unstable, and it's recommended to use canonicalize to get an absolute program path instead."*;
  Unix, *"the PATH searched comes from the child's environment"*; Windows, *"Rust resolves the executable
  path before spawning, rather than passing the name to CreateProcessW for resolution"*, its five-step
  order; no sentence on a missing folder. `verified`.
- **Rust `unix.rs`**, https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/sys/process/unix/unix.rs:
  `cvt(libc::chdir(cwd.as_ptr()))?;` in the fork path; the macOS comment quoted in § precedents;
  *"POSIX.1-2024 standardizes this function ... The _np version is more widely available, though, so try
  that first."*; *"Only glibc 2.24+ posix_spawn() supports returning ENOENT directly."* `verified`.
- **Go `os/exec`**, https://pkg.go.dev/os/exec: *"If Path is relative, it is evaluated relative to
  Dir."*; *"Dir specifies the working directory of the command. If Dir is the empty string, Run runs the
  command in the calling process's current directory."*; *"as of Go 1.19, this package will not resolve a
  program using an implicit or explicit path entry relative to the current directory"*; nothing on a
  missing `Dir`. `verified`.
- **Go `syscall/exec_linux.go`**, https://raw.githubusercontent.com/golang/go/master/src/syscall/exec_linux.go:
  `// Chdir` then `RawSyscall(SYS_CHDIR, ...)`, on failure `goto childerror`, which writes the errno to
  `pipe` and exits 253; *"In the child, this function must not acquire any locks, because they might have
  been locked at the time of the fork. This means no rescheduling, no malloc calls, and no new stack
  segments."* `verified`.
- **Python `subprocess`**, https://docs.python.org/3/library/subprocess.html: *"If cwd is not None, the
  function changes the working directory to cwd before executing the child. ... On POSIX, the function
  looks for executable (or for the first item in args) relative to cwd if the executable path is a
  relative path."*; *"For Windows, ... note that when resolving or searching for the executable path with
  shell=False, cwd does not override the current working directory and env cannot override the PATH
  environment variable. Using a full path avoids all of these variations."*; *"Exceptions raised in the
  child process, before the new program has started to execute, will be re-raised in the parent."*; no
  sentence names a missing `cwd`. `verified`.
- **CPython `Lib/subprocess.py`**, https://raw.githubusercontent.com/python/cpython/main/Lib/subprocess.py:
  *"glibc 2.24 has a new Linux posix_spawn implementation using vfork which properly reports errors to
  the parent process."*; the condition list holding `and cwd is None`. `verified`.
- **Node `child_process`**, https://nodejs.org/api/child_process.html: *"Use cwd to specify the working
  directory from which the process is spawned. If not given, the default is to inherit the current
  working directory. If given, but the path does not exist, the child process emits an ENOENT error and
  exits immediately. ENOENT is also emitted when the command does not exist."*; lookup by
  `options.env.PATH` else `process.env.PATH`; no sentence on a relative command against `cwd`.
  `verified`.
- **Microsoft `CreateProcessW`**, https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw
  (`updated_at` 2025-07-01): the sentences 00-shared.md quotes, word for word, and *"(This feature is
  provided primarily for shells that need to start an application and specify its initial drive and
  working directory.)"*; *"One way to obtain the current directory information for a drive X is to make
  the following call: GetFullPathName("X:", ...)"*. `verified`.

### Questions 4 and 5

- **Go `net/http`**, https://pkg.go.dev/net/http (offsets 150000 and 200000): *"ReadTimeout is the
  maximum duration for reading the entire request, including the body."*; *"ReadHeaderTimeout is the
  amount of time allowed to read request headers. If zero, the value of ReadTimeout is used."*;
  *"WriteTimeout ... It is reset whenever a new request's header is read."*; *"IdleTimeout is the maximum
  amount of time to wait for the next request when keep-alives are enabled."*; `Close` (go1.8),
  `Shutdown`'s *"waiting indefinitely for connections to return to idle and then shut down"*;
  `RegisterOnShutdown` (go1.9), *"should not wait for shutdown to complete"*. `verified`.
- **Go 1.8 release notes**, https://go.dev/doc/go1.8: *"The HTTP Server now has support for graceful
  shutdown using the new Server.Shutdown method and abrupt shutdown using the new Server.Close method."*;
  *"The Server adds configuration options ReadHeaderTimeout and IdleTimeout and documents
  WriteTimeout."*; no date (*"six months after Go 1.7"*). `verified`.
- **Go issue 14204**, https://github.com/golang/go/issues/14204: *"net/http: add http1 and http2 Server
  IdleTimeout"*, bradfitz, 2016-02-02, *"Add http.Server.IdleTimeout to limit the keep-alive idle time for
  http1 and http2 servers."*; commit c7e0dda (2016-10-26), *"net/http: add Server.ReadHeaderTimeout,
  IdleTimeout, document WriteTi…"*; milestone Go1.8; closed 2016-10-29. `verified`.
- **Go issue 39919**, https://github.com/golang/go/issues/39919: *"x/net/http2: Shutdown closes active
  HTTP/2 connections"*, nwidger, 2020-06-29; fraenkel, 2020-06-30, *"There is an issue with your test.
  Shutdown() is synchronous."* (the rest paraphrased by the fetch: the handler still waiting on a call
  that cannot finish while its own connection is active); after `Shutdown` moved to a goroutine,
  *"That looks right. We should close this."*; closed 2020-06-30. `verified`.
- **Go `net/net.go`**, https://raw.githubusercontent.com/golang/go/master/src/net/net.go: *"A deadline is
  an absolute time after which I/O operations fail instead of blocking."*; *"The deadline applies to all
  future and pending I/O, not just the immediately following call to Read or Write."*; *"After a
  deadline has been exceeded, the connection can be refreshed by setting a deadline in the future."*;
  *"If the deadline is exceeded a call to Read or Write or to other I/O methods will return an error that
  wraps os.ErrDeadlineExceeded."*; *"An idle timeout can be implemented by repeatedly extending the
  deadline after successful Read or Write calls."*; *"SetReadDeadline sets the deadline for future Read
  calls and any currently-blocked Read call."*; *"Any blocked Read or Write operations will be unblocked
  and return errors."*; *"Even if write times out, it may return n > 0"*. `verified`.
- **Node `http`**, https://nodejs.org/api/http.html (offsets 0 and 100000): `server.close()`, *"Stops the
  server from accepting new connections and closes all connections connected to this server which are
  not sending a request or waiting for a response."*, history *"v19.0.0 The method closes idle
  connections before returning."*; `closeIdleConnections()` and `closeAllConnections()` *"Added in:
  v18.2.0"*; `headersTimeout`, *"Added in: v11.3.0, v10.14.0"*, *"Limit the amount of time the parser
  will wait to receive the complete HTTP headers. If the timeout expires, the server responds with status
  408 without forwarding the request to the request listener and then closes the connection."*, *"It must
  be set to a non-zero value (e.g. 120 seconds) to protect against potential Denial-of-Service attacks in
  case the server is deployed without a reverse proxy in front."*; `requestTimeout`, *"Added in:
  v14.11.0"*, *"v18.0.0 The default request timeout changed from no timeout to 300s (5 minutes)."*;
  `server.timeout`, *"v13.0.0 The default timeout changed from 120s to 0 (no timeout)."*;
  `connectionsCheckingInterval` default 30000; `keepAliveTimeout` added v8.0.0, default 5000.
  `verified`.
- **Node November 2018 security releases**, https://nodejs.org/en/blog/vulnerability/november-2018-security-releases:
  *"An attacker can cause a Denial of Service (DoS) by sending headers very slowly keeping HTTP or HTTPS
  connections and associated resources alive for a long period of time. Attack potential is mitigated
  by the use of a load balancer or other proxy layer."*; *"A timeout of 40 seconds now applies to servers
  receiving HTTP headers. This value can be adjusted with `server.headersTimeout`. Where headers are not
  completely received within this period, the socket is destroyed on the next received chunk."*;
  *"Reported by Jan Maybach (liebdich.com), fixed by Matteo Collina."*; releases *"on, or shortly after,
  November 27th, 2018 UTC"*. `verified`.
- **Node September 2020 security releases**, https://nodejs.org/en/blog/vulnerability/september-2020-security-releases:
  *"Node.js is vulnerable to HTTP denial of service (DOS) attacks based on delayed requests submission
  which can make the server unable to accept new connections. The fix a new http.Server option called
  requestTimeout with a default value of 0 which means it is disabled by default."* (sic); *"All versions
  of the 14.x release line"*; releases *"on or shortly after Tuesday, September 15, 2020"*. `verified`.
- **Rust `JoinHandle`**, https://doc.rust-lang.org/std/thread/struct.JoinHandle.html: `is_finished`
  (1.61.0), *"Checks if the associated thread has finished running its main function. `is_finished`
  supports implementing a non-blocking join operation, by checking `is_finished`, and calling `join` if it
  returns `true`. This function does not block. To block while waiting on the thread to finish, use
  `join`."*; `join`, *"This function may panic on some platforms if a thread attempts to join itself"*.
  `verified`.
- **Java `Thread`**, https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Thread.html:
  `isAlive()`, *"A thread is alive if it has been started and has not yet terminated."*;
  `join(long millis)`, *"A timeout of 0 means to wait forever."*; `join(Duration)`, *"This method does
  not wait if the duration to wait is less than or equal to zero. In this case, the method just tests if
  the thread has terminated."*, Since 19; class Since 1.0. `verified`.
- **Linux man-pages `pthread_tryjoin_np(3)`**, https://man7.org/linux/man-pages/man3/pthread_tryjoin_np.3.html:
  *"If thread has not yet terminated, then instead of blocking, as is done by pthread_join(3), the call
  returns an error."*; EBUSY; `pthread_timedjoin_np`'s `abstime` *"measured against the CLOCK_REALTIME
  clock"*; HISTORY *"glibc 2.3.3"*; STANDARDS *"GNU; hence the suffix "_np" (nonportable) in the
  names"*; BUGS, timedjoin *"measures time by internally calculating a relative sleep interval"*.
  `verified`.
