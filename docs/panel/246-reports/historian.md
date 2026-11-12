# Panel 246, historian

Written 2026-11-12 by the historian seat, advisory, under `docs/panel/246-briefs/historian.md`
and `00-shared.md`, beside the critic's first pass (`246-reports/critic-briefs.md`, item 17).
Every precedent below is a page read on 2026-11-12, its URL beside it, and says only what that
page states; the quotes are in § Log. A line marked `unverified` rests on no page read today and is
not evidence. No build, no paid run, no GitHub write. *Heroes of code* was not consulted this
sitting, neither as a route nor as a source.

**Where the brief is wrong, first** (the coordinator's lists were written from memory and said so):

- **Erlang's `os:set_signal/2` cannot handle SIGINT**: its list is `sighup | sigquit | sigabrt |
  sigalrm | sigterm | sigusr1 | sigusr2 | sigchld | sigstop | sigtstp | sigcont | sigwinch |
  siginfo`, no `sigint` (https://www.erlang.org/doc/apps/kernel/os.html). It is a SIGTERM precedent,
  not a Ctrl-C one.
- **PEP 703 says nothing about code that tests a reference count of 1**
  (https://peps.python.org/pep-0703/), and neither does
  https://docs.python.org/3/howto/free-threading-extensions.html. The brief's *"PEP 703's notes on
  code that tests a reference count of 1"* has no page behind it here: `unverified`.
- **Nim's `setControlCHook` has no documentation comment** in its source
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/lib/system/excpt.nim); there is nothing to
  cite beyond its signature.
- **git's `lockfile.h` says nothing of `kill -9`**: it says the lock is removed by *"an atexit(3)
  handler and a signal handler"* (https://raw.githubusercontent.com/git/git/master/lockfile.h). That
  a SIGKILL leaves `index.lock` behind is not on any page read: `unverified`.
- **Rust's `Arc::get_mut` page does not say how the check is made sound against another thread**;
  only the nightly `Arc::is_unique` does (https://doc.rust-lang.org/std/sync/struct.Arc.html).
- **uv's cache page names no message printed while waiting** (https://docs.astral.sh/uv/concepts/cache/);
  it names a 5-minute timeout. **pip's *"Ignoring invalid distribution"*** has no primary page
  read (two searches returned blogs only): `unverified`. **Homebrew's lock file** and **Dart's
  widening of what an isolate may send** were not read (a 404; search results only): `unverified`.
- **The Windows console timeouts disagree between pages**: libuv and Node say *"approximately 10
  seconds"* and *"about 10 seconds"* after the console closes; Microsoft's own table gives
  `CTRL_CLOSE_EVENT` *"system parameter SPI_GETHUNGAPPTIMEOUT, 5000ms"*. Microsoft's is the primary.

---

## Part B, the server step (advisory)

### verdict

**object (advisory)**, and narrowly: to S1, S2 or S3 adopted **alone** as the way a server stops,
and to a program's own C header as the stop. The runtime-owned flag of S1 to S3 has broad
precedent and I would approve it paired with (b) or (c). B1's spec placement and narrow rung have
precedent and I raise nothing against them; B3's reading is below.

### precedents

**B2, the stop** (every quote in § Log, *B2*):

- **A flag set by the handler, asked by the loop**: Rust `signal-hook` 0.4.6 `flag`
  (https://docs.rs/signal-hook/latest/signal_hook/flag/index.html), which states *"The flags in this
  module allow for polling ... The do not allow blocking until something arrives"* and *"If one
  iteration could block for arbitrary time, the handling of the signal would be postponed for a long
  time."* CPython's own low-level handler *"sets a flag"* and the Python handler runs later, in the
  main thread (https://docs.python.org/3/library/signal.html), and *"A long-running calculation
  implemented purely in C ... may run uninterrupted ... regardless of any signals received."*
  `verified`.
- **A blocking call retried on `EINTR`**: PEP 475 (Final, 3.5, https://peps.python.org/pep-0475/)
  retries the standard library's wrappers on `EINTR` unless the handler raises, and names two ways
  to interrupt on some signals: a handler that raises, or *"a I/O multiplexing function like select()
  together with Python's signal wakeup file descriptor"*. 00-shared.md reads
  `runtime/platform/hero_net.c:509-512` as the same retry (this seat did not re-read it). `verified`.
- **(b), the stop as a descriptor beside the listener**: Python `signal.set_wakeup_fd` writes the
  signal number *"as a single byte into the fd ... to wakeup a poll or select call"*; libuv's
  `uv_signal_t`, *"per-event loop"* (https://docs.libuv.org/en/v1.x/signal.html); Linux `signalfd(2)`,
  signals blocked with `sigprocmask`, readable when pending, since 2.6.22
  (https://man7.org/linux/man-pages/man2/signalfd.2.html); FreeBSD `kqueue(2)` `EVFILT_SIGNAL`,
  *"lower precedence"* than `sigaction`, records even `SIG_IGN`
  (https://man.freebsd.org/cgi/man.cgi?query=kqueue&sektion=2; Darwin's own page not read).
  `verified`.
- **(c), the server closes its own listener**: Go `net/http` `Server.Shutdown` (go1.8), *"first
  closing all open listeners, then closing all idle connections"*, after which `Serve` *"immediately
  return[s] ErrServerClosed"* (https://pkg.go.dev/net/http); Go `net.Listener.Close`, *"Any blocked
  Accept operations will be unblocked and return errors"* (https://pkg.go.dev/net); Java
  `ServerSocket.close`, *"Any thread currently blocked in accept() will throw a SocketException"*
  (https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/net/ServerSocket.html); Python
  `socketserver.serve_forever` *"Poll[s] for shutdown every poll_interval seconds"*, 0.5 by default
  (https://docs.python.org/3/library/socketserver.html). These are each runtime's own documentation,
  not POSIX `shutdown(2)`'s: whether `hero_net_close` wakes `accept` on each platform stays the
  brief's open question. `verified`.
- **On which thread a handler runs**: Windows, *"the system creates a new thread in the process to
  execute the function"* (https://learn.microsoft.com/en-us/windows/console/handlerroutine); .NET
  `CancelKeyPress`, *"executed on a thread pool thread"*; Haskell `unix` 2.8.8.0, *"in a new
  thread"*; Rust `ctrlc` 3.5.2, *"a new dedicated signal handling thread"*; POSIX.1-2024 2.4.1, a
  process-directed signal goes to *"exactly one"* thread not blocking it or in `sigwait`
  (https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html). `verified`.
- **Once armed, the default stays off**: Tokio 1.54.1 `ctrl_c`, *"for the duration of the entire
  process"*, *"the default platform behavior will NOT be reset"*
  (https://docs.rs/tokio/latest/tokio/signal/fn.ctrl_c.html); Node, a listener removes the default
  (https://nodejs.org/api/process.html); Go `NotifyContext` (go1.16), *"Future interrupts received will
  not trigger the default (exit) behavior until the returned stop function is called"*
  (https://pkg.go.dev/os/signal). `verified`.
- **The second Ctrl-C**: Python `asyncio` runner (3.11), *"the second following Ctrl-C immediately
  raises the KeyboardInterrupt"* (https://docs.python.org/3/library/asyncio-runner.html);
  `signal-hook`'s example *"When terminated by a second term signal, exit with exit code 1"*
  (https://docs.rs/signal-hook/latest/signal_hook/). No page read says the convention is universal.
  `verified` for these two.
- **Which signals**: Go's default exit on *"SIGHUP, SIGINT, or SIGTERM"*; `ctrlc` takes SIGINT and,
  with a feature, SIGTERM and SIGHUP; .NET cancels SIGINT and SIGQUIT everywhere, *"SIGTERM can only
  be canceled on Unix"*; Node: *"'SIGTERM' is not supported on Windows"*; Go maps
  `CTRL_CLOSE_EVENT`, `CTRL_LOGOFF_EVENT`, `CTRL_SHUTDOWN_EVENT` to SIGTERM and *"the process will
  still get terminated unless it exits"*. `verified`.
- **A clean stop and the leak check**: Python `atexit` handlers are *"not called when the program is
  killed by a signal not handled by Python ... or when os._exit() is called"*
  (https://docs.python.org/3/library/atexit.html); LeakSanitizer has an end-of-process phase and
  `__lsan_do_recoverable_leak_check()`, which *"prints a report without terminating the process"*
  (https://clang.llvm.org/docs/LeakSanitizer.html). `verified`.

**B1, what crosses** (§ Log, *B1*):

- **Stated where the checker enforces it**: Rust `thread::spawn`'s `Send + 'static` bounds and
  `Send`, *"Types that can be transferred across thread boundaries"*
  (https://doc.rust-lang.org/std/thread/fn.spawn.html, https://doc.rust-lang.org/std/marker/trait.Send.html);
  Swift SE-0302, *"Implemented (Swift 5.7)"*, *"The compiler rejects any attempts to pass data across
  concurrency domains"* that are not `Sendable`. Dart states its rule in `SendPort.send`'s library
  documentation. `verified`.
- **A narrow first rung, widened**: PEP 554 (2017, Superseded) began with *"A minimal set of simple,
  immutable builtin types"*, `None`, `bool`, `bytes`, `str`, `int`, `float`, and *"Further builtin types
  may be supported later"*; PEP 734 (Final, 3.14) made *"All objects that can be pickled"* shareable,
  most mutable ones *"copied"*. Neither page states what the widening cost. `verified`. Note: PEP
  554's first rung already held `str` and `float`, so T1's `i64` alone is narrower than PEP 554's.
- **Copy, move, or share**: Erlang copies *"All data in messages ... except for refc binaries and
  literals"* (https://www.erlang.org/doc/system/eff_guide_processes.html); Ruby Ractor deep-copies or
  *"moves membership"*, after which *"The sending ractor can not access the moved object"*; Dart shares
  immutable objects and copies the rest; Go's slogan is *"share memory by communicating"*
  (https://go.dev/doc/effective_go); OCaml 5 domains: *"Immutable values may be freely shared"*, a data
  race gives *"no crashes"* (https://ocaml.org/manual/5.2/parallelism.html). `verified`.

**B3, a connection across threads** (§ Log, *B3*):

- **Allowed by type**: Rust lists `impl Send for TcpStream`; Go: *"Multiple goroutines may invoke
  methods on a Conn simultaneously."* `verified`.
- **Moved explicitly**: Tcl `thread::transfer` *"moves the specified channel"* and *"waits until
  the other thread has incorporated the channel"*; Erlang `gen_tcp:controlling_process/2`, callable
  only by the current owner (`{error, not_owner}`); Ruby's move. `verified`.
- **Refused**: Dart, *"A Socket object ... can therefore not be sent."* `verified`.
- **A number for a handle, and its reuse**: POSIX 2.6, functions that open descriptors *"atomically
  allocate the lowest"* unused number; `SCM_RIGHTS` gives the receiver *"a different file descriptor
  number"* (https://man7.org/linux/man-pages/man7/unix.7.html); Microsoft warns against
  `DuplicateHandle` for sockets, which *"interferes with internal reference counting"*, and names
  `WSADuplicateSocket`. `verified`.
- **The `refcount == 1` test under threads**: Swift's `isKnownUniquelyReferenced`, *"If the instance
  passed as object is being accessed by multiple threads simultaneously ... this function may still
  return true. Therefore, you must only call this function from mutating methods with appropriate
  thread synchronization."* (https://raw.githubusercontent.com/swiftlang/swift/main/stdlib/public/core/ManagedBuffer.swift);
  Rust `Arc::get_mut` returns `None` *"because it is not safe to mutate a shared value"*, and nightly
  `is_unique` *"performs an Acquire operation"* and *"requires locking the weak ref count"*; Koka's
  Perceus (MSR-TR-2020-42, § 2.7.2): *"when starting a new thread, the argument passed to the thread
  is marked as thread-shared"*, encoded *"as a negative value"*, with atomic counts on that path
  (https://microsoft.com/en-us/research/uploads/prod/2020/11/perceus-tr-v4.pdf, its header reading
  *"MSR-TR-2020-42, Nov 22, 2020"*). `verified`.

### argument

Every flag-only stop read documents its own limit: `signal-hook` says a flag allows polling, not
blocking; PEP 475 retries `EINTR`, as 00-shared.md reads the layer's `accept` doing, and names a
raising handler or the wakeup descriptor as its ways to interrupt. S1 to S3 alone would rebuild that
known limit in a server blocked in `accept`. The precedents pair the flag with a wakeup:
`set_wakeup_fd`, libuv, `signalfd`, kqueue (b), or the server closing its listener, Go and Java (c).
Windows, .NET, Haskell and `ctrlc` run handlers on threads the program's own code never started, so
no Heroes code belongs in one. B1's narrow rung has PEP 554's later widening; B3's handle has Tcl's
and Ruby's moves, and Dart's refusal. (119 words, counted by hand)

### condition

A precedent where a flag-only stop served a server blocked in a retried `accept`, with no wakeup and
no listener closed, would turn the objection; every page read says the opposite. A Darwin or
Windows page stating that closing or shutting down a listening socket does **not** wake a blocked
`accept` would move my reading from (c) to (b). A page showing a language that began with one
integer crossing and found the rung unworkable would weigh against T1; none was found.

### search

Pages fetched and read, all on 2026-11-12: every URL in § Log. Queries (WebSearch): *nim
setControlCHook documentation*; *Ruby Ractor documentation shareable objects move: true IO*;
*Perceus garbage free reference counting with reuse Koka thread-shared*; *Dart 2.15 isolate groups
send any object SendPort*. Pages that failed or held nothing on the point, and are not cited:
Apple's `Sendable` page (title only), `ruby/ruby` `doc/ractor.md` and `docs.ruby-lang.org/en/master/ractor_md.html`
(404), Nim's rendered `system.html` (three parts read, the procs not in them), PEP 703 and the
free-threading extensions HOWTO (no `refcount == 1` text), Tokio's module page (pointed elsewhere).
**Not searched**: Darwin's own manual pages (kqueue, sigwait, accept), Pony's TCP actors, Go's and
CPython's issue trackers, Windows' `accept` after `closesocket`. **Not run**: a grep of `docs/panel/`
for earlier historian lines on these names (this seat had no shell); the synthesis runs `grep -n -i
'signal-hook\|set_wakeup_fd\|controlling_process\|lockedfile\|temproots' docs/panel/*.md` before any
prior-art line is filed.

---

## Part A's precedent (input to the synthesis; not a seat of the soundness lane)

### verdict

**approve (advisory)** route A **with** route D, as input only: a per-run lock whose acquisition
proves its holder dead has a working precedent in Nix and Go, and the refusal of a standing name has
git's and POSIX's. Routes B and C have none that I found for data a dead run holds alone.

### precedents

- **Nix, a per-process file whose lock tells the dead from the living**
  (https://raw.githubusercontent.com/NixOS/nix/master/src/libstore/gc.cc): *"Read the `temproots'
  directory for per-process temporary root files."*; *"Try to acquire a write lock without blocking.
  This can only succeed if the owning process has died."*, *"In that case we don't care about its
  temporary roots."*, and the code then unlinks the file. Its owner *"Create[s] the temporary roots
  file for this process"* and takes `lockFile(..., ltWrite, true)`. `verified`. **And the shape of
  `staging.hero:48` is Nix's too**: in `createTempRootsFile`, if the file exists, *"It *must* be stale,
  since there can be no two processes with the same pid."*, `tryUnlink`. Nix reasons so of a file
  whose contents it says it does not care about once the owner is dead; `.fetching-<pid>-<n>/replaced-N`
  holds the author's only copy (00-shared.md's measurement). `verified` for Nix's words; the contrast
  is my reading.
- **Go's module cache, recovery only under the lock**
  (https://raw.githubusercontent.com/golang/go/master/src/cmd/go/internal/modfetch/fetch.go): *"To
  prevent other processes from reading the directory if we crash, create a .partial file before
  extracting the directory, and delete the .partial file afterward (all while holding the lock)."*;
  *"Clean up any remaining temporary directories ... as well as partially extracted directories ...
  This is only safe to do because the lock file ensures that their writers are no longer active."*;
  *"Go 1.14.2 and higher respect .partial files. Older versions may use partially extracted
  directories."*; and *"Before Go 1.16, we extracted to a temporary directory with a random name then
  renamed it into place with os.Rename. On Windows, this failed with ERROR_ACCESS_DENIED when another
  process (usually an anti-virus scanner) opened files in the temporary directory."* Its lock is per
  module version (`lockVersion`), a well-known path, not per run. `filelock`
  (https://raw.githubusercontent.com/golang/go/master/src/cmd/go/internal/lockedfile/internal/filelock/filelock.go):
  no other process can lock *"until this process exits, closes f, or calls Unlock on it"*, and
  *"Closing the file may or may not release the lock promptly."* `verified`.
- **git, the refusal of a standing name (route D's cousin)**: `<filename>.lock` created *"with
  O_CREAT|O_EXCL so that we can notice and fail if somebody else has already locked the file"*
  (https://raw.githubusercontent.com/git/git/master/lockfile.h); with `core.lockfilePid`, a PID file
  beside it and the messages *"Lock may be held by process %...; if no git process is running, the
  lock file may be stale (PIDs can be reused)"* and *"Lock was held by process %..., which is no
  longer running; the lock file appears to be stale"*, liveness by `kill(pid, 0)`
  (https://raw.githubusercontent.com/git/git/master/lockfile.c). git tells, and removes nothing.
  `verified`.
- **Cargo, the lock and the file systems without one**
  (https://doc.rust-lang.org/stable/nightly-rustc/src/cargo/util/flock.rs.html): `flock` on Unix
  (*"process-scoped fcntl locks on Solaris"*), *"mandatory using LockFileEx"* on Windows; while
  waiting it prints `Blocking` *"waiting for file lock on"*; on NFS, *"the failure mode [1] for flock on
  NFS is **blocking forever**, even if the "non-blocking" flag is passed!"* so it *"skip[s] all file
  locks entirely on NFS mounts"*, citing cargo issue 2615 (not read here); where the error is
  `ENOTSUP`, `EOPNOTSUPP`, `ENOSYS` or `ERROR_INVALID_FUNCTION` it proceeds as if locked. The package
  cache's three modes (https://doc.rust-lang.org/stable/nightly-rustc/cargo/util/cache_lock/index.html)
  *"coordinate when multiple cargos are running at the same time"*. `verified`.
- **uv** (https://docs.astral.sh/uv/concepts/cache/): *"uv applies a file-based lock to the target
  virtual environment when installing"*; cache commands wait 5 minutes (`UV_LOCK_TIMEOUT`), `--force`
  ignores the lock. **npm `cacache`** (https://github.com/npm/cacache): *"Lockless, high-concurrency
  cache access"*, *"Fault tolerance (immune to corruption, partial writes, process races, etc)"*, and
  cleanup is a separate `verify` that *"Removes the tmp directory"*. **pip**
  (https://raw.githubusercontent.com/pypa/pip/main/src/pip/_internal/utils/temp_dir.py):
  `AdjacentTempDirectory` *"creates a temporary directory adjacent to a real one"*, names from
  `LEADING_CHARS = "-~.=%0123456789"`, and on a failed cleanup *"You can safely remove it manually."*
  `verified`. What pip prints about a leftover: `unverified`.
- **The lock primitives**: `flock(2)` (https://man7.org/linux/man-pages/man2/flock.2.html), locks
  *"associated with an open file description"*, so descriptors from *"fork(2) or dup(2)"* hold the same
  lock; on NFS *"emulating them as fcntl(2) byte-range locks"* since 2.6.12; over SMB *"not propagated"*
  up to Linux 5.4, emulated since 5.5. `LockFileEx`
  (https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfileex): on termination
  *"the locks are unlocked by the operating system. However, the time it takes ... depends upon
  available system resources"*; supported over SMB 3.0. SQLite
  (https://www.sqlite.org/howtocorrupt.html): network file systems' locking *"especially ... NFS"* has
  bugs, and *"the close() system call will cancel all POSIX advisory locks on the same file for all
  threads"*. `verified`.
- **A process number in a name**: `start-stop-daemon(8)`, *"Using this matching option alone might
  cause unintended processes to be acted on, if the old process terminated without being able to
  remove the pidfile."* (https://man7.org/linux/man-pages/man8/start-stop-daemon.8.html). The refusal
  of a name that stands: POSIX `open()`, *"If O_CREAT and O_EXCL are set, open() shall fail if the file
  exists."*, atomically (https://pubs.opengroup.org/onlinepubs/9799919799/functions/open.html);
  `mkdtemp(3)` replaces `XXXXXX` *"with a string that makes the directory name unique"*
  (https://man7.org/linux/man-pages/man3/mkdtemp.3.html). `verified`.

### argument

Route A's lock is Nix's mechanism: a per-process file its owner locks, and a collector whose
non-blocking lock *"can only succeed if the owning process has died"*. Go's cache removes dead
writers' leftovers *"only"* because *"the lock file ensures that their writers are no longer
active"*. Route D is git's: a name created exclusively, a later command refused, and a message that
*"PIDs can be reused"*. And `staging.hero:48`'s premise is Nix's own, *"there can be no two processes
with the same pid"*, applied by Nix to contents it no longer needs; here the folder holds the
author's only copy. Cargo's NFS skip, `flock`'s shared description across `fork`, and `close()`
cancelling POSIX advisory locks name the three shapes A3 asks. (119 words, counted by hand)

### condition

A tool that recovered a dead run's staging by its lock and lost a user's data that way (a lock
released while its owner lived, a network file system, a child holding it) would weigh against A;
none was found among the pages listed. Cargo issue 2615, cited by Cargo's source and not read here:
if it shows `LOCK_NB` blocking on NFS, route A's *leave what it cannot lock* needs Cargo's NFS
check before it asks the lock at all, or a recovering `check` can hang.

### search

Pages fetched and read 2026-11-12: Nix `gc.cc` (twice), Go `fetch.go`, `filelock.go`, the
`lockedfile` page, git `lockfile.h` and `lockfile.c` (twice), Cargo `flock.rs` (rustc docs' source
view; the raw GitHub path is a 404) and `cache_lock`, Cargo's config reference (nothing on locks),
uv's cache page, `npm/cacache`, pip `temp_dir.py`, `flock(2)`, `LockFileEx`, SQLite
`howtocorrupt`, `start-stop-daemon(8)`, POSIX `open()` and 2.6, `mkdtemp(3)`. Queries: *cargo
flock.rs is_on_nfs_mount OR error_unsupported*; *pip "Ignoring invalid distribution" tilde*; *pip
source "Ignoring invalid distribution" site:github.com pypa pip*. Not read: Homebrew's
`lock_file.rb` (404 at `Library/Homebrew/lock_file.rb`, not searched further), cargo issue 2615,
Darwin's `flock(2)`, any Windows page on `LockFileEx` over SMB 2.

---

## Candidate prior-art lines, per route, in `.claude/rules/records.md`'s two forms

Each only if the sitting adopts the route it names and the session filing it re-reads the page.

**Part B**

- (b) adopted: `- **Taken from Python**: [signal.set_wakeup_fd](https://docs.python.org/3/library/signal.html): the signal number is written as a single byte into a non-blocking file descriptor, which can be used to wake a poll or select call.`
- (c) adopted: `- **Taken from Go**: [net/http Server.Shutdown](https://pkg.go.dev/net/http): Shutdown first closes all open listeners, then idle connections, and Serve immediately returns ErrServerClosed.`
- (c) adopted: `- **Reached first by Java**: [ServerSocket.close](https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/net/ServerSocket.html): any thread currently blocked in accept() throws a SocketException.`
- any S with (b) or (c): `- **Reached first by Python**: [PEP 475](https://peps.python.org/pep-0475/): system call wrappers are retried on EINTR unless the signal handler raises, and an application interrupted on some signals uses a raising handler or select() with the signal wakeup file descriptor.`
- the flag kept out of Heroes code: `- **Reached first by Microsoft**: [HandlerRoutine](https://learn.microsoft.com/en-us/windows/console/handlerroutine): when a console control signal is received, the system creates a new thread in the process to execute the handler.`
- a second-Ctrl-C rule adopted: `- **Reached first by Python**: [asyncio Runner](https://docs.python.org/3/library/asyncio-runner.html): a second Ctrl-C immediately raises KeyboardInterrupt without cancelling the main task (added in 3.11).`
- B1's narrow rung adopted: `- **Reached first by Python**: [PEP 554](https://peps.python.org/pep-0554/): a minimal set of simple, immutable builtin types is supported initially, and further builtin types may be supported later.`
- B3 a moving rung adopted: `- **Taken from Tcl**: [thread::transfer](https://www.tcl-lang.org/man/tcl8.6/ThreadCmd/thread.htm): a channel is moved from the current thread to another, the command waiting until the other thread has incorporated it.`
- B3 one thread or a refusal adopted: `- **Reached first by Dart**: [SendPort.send](https://api.dart.dev/stable/dart-isolate/SendPort/send.html): a Socket object refers to native resources and cannot be sent to another isolate.`

**Part A**

- A adopted: `- **Taken from Nix**: [gc.cc](https://raw.githubusercontent.com/NixOS/nix/master/src/libstore/gc.cc): each process write-locks its own temporary roots file, and a non-blocking write lock on another's file can only succeed if the owning process has died.`
- A adopted: `- **Taken from Go**: [modfetch/fetch.go](https://raw.githubusercontent.com/golang/go/master/src/cmd/go/internal/modfetch/fetch.go): partially extracted directories and temporary files of earlier runs are removed only under the lock file, which ensures their writers are no longer active.`
- D adopted: `- **Reached first by git**: [lockfile.h](https://raw.githubusercontent.com/git/git/master/lockfile.h): the lock file is created with O_CREAT|O_EXCL so that a second locker notices and fails.`
- an NFS rule adopted: `- **Taken from Cargo**: [flock.rs](https://doc.rust-lang.org/stable/nightly-rustc/src/cargo/util/flock.rs.html): file locks are skipped on NFS mounts, where flock's failure mode is blocking forever even with the non-blocking flag.`
- Not a candidate line: Nix's *"It *must* be stale, since there can be no two processes with the same pid"* is the precedent of what `staging.hero:48` does, which the repair leaves; it goes in the sitting's file, not in `## Prior art`.

---

## One thing written outside the repository's root, reported

At the Perceus fetch, WebFetch saved the PDF it could not render to
`/Users/joseph/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang/15c2275d-d1e8-47ba-94cd-49bc93c10427/tool-results/webfetch-1794456045919-8mv3we.pdf`
(627.2 KB), by the tool's own behaviour; I read pages 7 to 11 of it there. It is outside the
repository's root (CLAUDE.md § Hard stops), as panel 234 recorded of two coordinator files. I did
not remove it: removal is a destructive act, the coordinator's or the author's to ask.

---

## Log, as I went

### B2, the stop: pages read 2026-11-12 (first batches)

- **Python `signal`**, https://docs.python.org/3/library/signal.html: *"Python signal handlers are
  always executed in the main Python thread of the main interpreter, even if the signal was
  received in another thread"*; *"A Python signal handler does not get executed inside the
  low-level (C) signal handler. Instead, the low-level signal handler sets a flag which tells the
  virtual machine to execute the corresponding Python signal handler at a later point"*;
  `set_wakeup_fd`: *"the signal number is written as a single byte into the fd ... This can be used
  by a library to wakeup a poll or select call"*, the fd *"must be non-blocking"*, `warn_on_full_buffer`
  *"Added"* in 3.7; *"A long-running calculation implemented purely in C ... may run uninterrupted
  for an arbitrary amount of time, regardless of any signals received"*. `verified`.
- **PEP 475**, https://peps.python.org/pep-0475/: Final, Python 3.5, created 29-Jul-2014. *"System
  call wrappers provided in the standard library should be retried automatically when they fail
  with `EINTR`"*; *"If the signal handler raises an exception, the Python wrapper bails out"*; to
  interrupt on some signals only, the PEP names a handler that raises, or *"a I/O multiplexing
  function like `select()` together with Python's signal wakeup file descriptor"*. `verified`.
- **libuv `uv_signal_t`**, https://docs.libuv.org/en/v1.x/signal.html: *"Signal handles implement
  Unix style signal handling on a per-event loop bases"*; on Windows *"SIGINT is normally delivered
  when the user presses CTRL+C"*, *"SIGHUP is generated when the user closes the console window"*,
  *"On SIGHUP the program is given approximately 10 seconds to perform cleanup"*;
  `uv_signal_start_oneshot` *"New in version 1.12.0"*. The page does not say on which thread the
  callback runs (the fetch found no such sentence). `verified` for what is quoted.
- **Go `os/signal`**, https://pkg.go.dev/os/signal: *"A SIGHUP, SIGINT, or SIGTERM signal causes the
  program to exit"*; `Notify` *"will not block sending to c"*, *"a buffer of size 1 is sufficient"*;
  `NotifyContext` *"added in go1.16"*, *"Future interrupts received will not trigger the default
  (exit) behavior until the returned stop function is called"*, *"The stop function unregisters the
  signal behavior, which, like signal.Reset, may restore the default behavior"*; Windows: *"if Notify
  is called, and Windows sends CTRL_CLOSE_EVENT, CTRL_LOGOFF_EVENT or CTRL_SHUTDOWN_EVENT to the
  process, Notify will return syscall.SIGTERM"* and *"the process will still get terminated unless it
  exits"*; non-Go code *"should not change the signal mask on any threads created by the Go
  runtime"*. `verified`.
- **Rust `signal-hook` 0.4.6, `flag`**, https://docs.rs/signal-hook/latest/signal_hook/flag/index.html:
  *"helper functions to set flags whenever a signal happens"*, atomic, `SeqCst`; *"The flags in
  this module allow for polling if a signal arrived since the previous poll. The do not allow
  blocking until something arrives"*; *"If one iteration could block for arbitrary time, the
  handling of the signal would be postponed for a long time."*; `register_conditional_shutdown`
  *"Terminate the application on a signal if the given condition is true."* `verified`.
- **Linux `signalfd(2)`**, https://man7.org/linux/man-pages/man2/signalfd.2.html: *"creates a file
  descriptor that can be used to accept signals targeted at the caller"*; the signals *"should be
  blocked using sigprocmask(2)"*; *"remains open across an execve(2)"*; *"A thread will not be able
  to read signals that are directed to other threads in the process"*; readable *"if one or more of
  the signals in mask is pending for the process"*; Linux 2.6.22, glibc 2.8. `verified`.
- **FreeBSD `kqueue(2)`, `EVFILT_SIGNAL`**, https://man.freebsd.org/cgi/man.cgi?query=kqueue&sektion=2:
  *"will record all attempts to deliver a signal to a process, even if the signal has been marked
  as SIG_IGN"*; *"This coexists with the signal() and sigaction() facilities, and has a lower
  precedence"*; *"data returns the number of times the signal has occurred since the last call to
  kevent()"*; kqueue *"first appeared in FreeBSD 4.1"*. This is FreeBSD's page, not Darwin's;
  Darwin's own manual page is not read here. `verified` for FreeBSD.
- **POSIX.1-2024 (Issue 8), 2.4**, https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html:
  *"Signals generated for the process shall be delivered to exactly one of those threads within
  the process"* which is in `sigwait()` for it or has not blocked it; otherwise *"the signal shall
  remain pending on the process until a thread calls a sigwait() function selecting that signal"*
  (or unblocks it); the async-signal-safe table includes `_Exit()`, `write()`, `sigaction()`; a
  handler may assign *"to an object declared as volatile sig_atomic_t"*. `verified`.
- **Windows `HandlerRoutine`**, https://learn.microsoft.com/en-us/windows/console/handlerroutine:
  *"When the signal is received, the system creates a new thread in the process to execute the
  function."*; *"Because the system creates a new thread in the process to execute the handler
  function, it is possible that the handler function will be terminated by another thread in the
  process"*; the default handler *"calls ExitProcess"*; handlers called *"last-registered,
  first-called"*; timeouts: `CTRL_CLOSE_EVENT` 5000 ms, `CTRL_C`, `CTRL_BREAK` *"no timeout"*;
  *"a third-party library or DLL can install a console control handler for your application"*.
  `verified`.
- **Windows `SetConsoleCtrlHandler`**, https://learn.microsoft.com/en-us/windows/console/setconsolectrlhandler:
  with `NULL` and `TRUE`, *"the calling process ignore CTRL+C input ... This attribute of ignoring or
  processing CTRL+C is inherited by child processes"*; *"When a console window with the keyboard
  focus receives CTRL+C or CTRL+BREAK, the signal is typically passed to all processes sharing that
  console"*; *"Console functions, or any C run-time functions that call console functions, may not
  work reliably during processing of"* the close, logoff and shutdown events. `verified`.

### B2, the stop: second batch, read 2026-11-12

- **.NET `Console.CancelKeyPress`**, https://learn.microsoft.com/en-us/dotnet/api/system.console.cancelkeypress:
  *"By default, the Cancel property is `false`, which causes program execution to terminate when the
  event handler exits. Changing its property to `true` specifies that the application should
  continue to execute."*; *"The event handler for this event is executed on a thread pool thread."*
  `verified`.
- **.NET `PosixSignalRegistration.Create`**, https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.posixsignalregistration.create:
  monikers `net-6.0` onward; *"Default handling of the signal can be canceled through Cancel. SIGINT
  and SIGQUIT can be canceled on both Windows and on Unix platforms; SIGTERM can only be canceled on
  Unix."*; *"In .NET 10 and later versions, the handlers are executed in reverse order of their
  registration."* The page does not say on which thread the handler runs. `verified`.
- **Java `Runtime`**, https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Runtime.html:
  the shutdown sequence starts *"when the number of live non-daemon threads drops to zero"*, on
  `exit`, or *"when some external event occurs, such as an interrupt or a signal is received from
  the operating system"*; *"A shutdown hook is simply an initialized but unstarted thread"*; hooks
  are *"started in some unspecified order. They run concurrently"*; *"should also finish their work
  quickly"*; `halt` *"does not initiate the shutdown sequence"*. `addShutdownHook` since 1.3. The
  page says nothing of SIGKILL. `verified`.
- **Node.js `process`, signal events**, https://nodejs.org/api/process.html: *"'SIGTERM' and 'SIGINT'
  have default handlers on non-Windows platforms that reset the terminal mode before exiting with
  code 128 + signal number. If one of these signals has a listener installed, its default behavior
  will be removed."*; *"'SIGTERM' is not supported on Windows, it can be listened on."*; on Windows
  SIGHUP at console close, *"Node.js will be unconditionally terminated by Windows about 10 seconds
  later"*. `verified`.
- **Nim `setControlCHook`**, source https://raw.githubusercontent.com/nim-lang/Nim/devel/lib/system/excpt.nim:
  `proc setControlCHook(hook: proc () {.noconv.})` and `proc unsetControlCHook()`, neither with a
  doc comment; the default path prints *"SIGINT: Interrupted by Ctrl-C."*. The rendered
  https://nim-lang.org/docs/system.html was read in three parts and the procs' entries were not
  found in what the fetch returned. `verified` for the signatures and the message only.
- **OCaml `Sys`**, https://ocaml.org/manual/5.2/api/Sys.html: *"catch_break governs whether
  interactive interrupt (ctrl-C) terminates the program or raises the Break exception"*; *"Inside
  multi-threaded programs, the Break exception will arise in any one of the active threads"*; *"Use
  signal masks from Thread.sigmask to direct the interrupt towards a specific thread."* The page
  says nothing of when a handler runs. `verified`.
- **Haskell `unix` 2.8.8.0, `installHandler`**, https://hackage-content.haskell.org/package/unix-2.8.8.0/docs/System-Posix-Signals.html
  (redirected from hackage.haskell.org): *"if handler is Catch action, a handler is installed which
  will invoke action in a new thread"* *"when (or shortly after) the signal is received."*
  `verified`.
- **Erlang `os:set_signal/2`**, https://www.erlang.org/doc/apps/kernel/os.html: the signals are
  `sighup | sigquit | sigabrt | sigalrm | sigterm | sigusr1 | sigusr2 | sigchld | sigstop | sigtstp |
  sigcont | sigwinch | siginfo`, **`sigint` not among them**; `handle`: *"This signal will notify
  erl_signal_server when it is received by the Erlang runtime system"*; *"(since OTP 20.0)"*.
  `verified`.
- **Rust `ctrlc` 3.5.2**, https://docs.rs/ctrlc/latest/ctrlc/: *"Setting a handler will start a new
  dedicated signal handling thread."*; *"There can only be one handler"*; *"Handling of SIGTERM and
  SIGHUP can be enabled with termination feature."* `verified`.
- **Tokio 1.54.1 `ctrl_c`**, https://docs.rs/tokio/latest/tokio/signal/fn.ctrl_c.html: *"the first
  time that a Signal instance is registered for a particular signal kind, an OS signal-handler is
  installed which replaces the default platform behavior when that signal is received, for the
  duration of the entire process."*; *"Even if this Signal instance is dropped, subsequent SIGINT
  deliveries will end up captured by Tokio, and the default platform behavior will NOT be reset"*.
  `verified`. The module page, https://docs.rs/tokio/latest/tokio/signal/index.html, points its
  limitations to other pages and is not cited.
- **The second Ctrl-C.** Python `asyncio` runner, https://docs.python.org/3/library/asyncio-runner.html,
  *"Added in version 3.11"*: the first Ctrl-C *"cancels the main task"*; *"A user could write a
  tight loop which cannot be interrupted by asyncio.Task.cancel(), in which case the second
  following Ctrl-C immediately raises the KeyboardInterrupt without cancelling the main task."*
  `signal-hook`'s crate page, https://docs.rs/signal-hook/latest/signal_hook/, carries an example
  commented *"When terminated by a second term signal, exit with exit code 1"*, *"This will do
  nothing the first time"*, and states *"signals will not be delivered if the signal is masked in
  all program's threads"*. Go's `NotifyContext` (above) says its `stop` *"may restore the default
  behavior"*. `verified` for these three; no page read says the convention is universal.
- **Go `net/http` `Server.Shutdown`**, https://pkg.go.dev/net/http (read from offset 200000):
  *"Shutdown gracefully shuts down the server without interrupting any active connections"*, by
  *"first closing all open listeners, then closing all idle connections"*; *"When Shutdown is
  called, Serve, ServeTLS, ListenAndServe, and ListenAndServeTLS immediately return
  ErrServerClosed."*; *"Shutdown does not attempt to close nor wait for hijacked connections such
  as WebSockets."*; added in go1.8. `verified`.
- **Go `net.Listener.Close`**, https://pkg.go.dev/net (read from offset 100000): *"Close closes the
  listener. Any blocked Accept operations will be unblocked and return errors."* `verified`.
- **Java `ServerSocket`**, https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/net/ServerSocket.html:
  `close`, *"Any thread currently blocked in accept() will throw a SocketException."*; `accept`
  *"blocks until a connection is made"*, and is interruptible when a virtual thread accepts.
  `verified`.
- **Python `socketserver`**, https://docs.python.org/3/library/socketserver.html:
  `serve_forever(poll_interval=0.5)`, *"Handle requests until an explicit shutdown() request. Poll
  for shutdown every poll_interval seconds."*; `shutdown()` *"must be called while serve_forever() is
  running in a different thread otherwise it will deadlock."* `verified`.
- **Python `atexit`**, https://docs.python.org/3/library/atexit.html: handlers run *"upon normal
  program termination (for instance, if sys.exit() is called or the main module's execution
  completes)"*; *"Exit handlers are not called when the program is killed by a signal not handled by
  Python, when a Python fatal internal error is detected, or when os._exit() is called."*
  `verified`.
- **Clang LeakSanitizer**, https://clang.llvm.org/docs/LeakSanitizer.html: an *"extra leak detection
  phase"* at the end of the process; `__lsan_do_recoverable_leak_check()` *"prints a report without
  terminating the process or disabling the end-of-process check."* `verified`.

### B1, what crosses: read 2026-11-12

- **Rust `thread::spawn`**, https://doc.rust-lang.org/std/thread/fn.spawn.html: `F: FnOnce() -> T +
  Send + 'static, T: Send + 'static`; *"The Send constraint is because the closure will need to be
  passed by value from the thread where it is spawned to the new thread. Its return value will need
  to be passed from the new thread to the thread where it is joined."* `verified`.
- **Rust `Send`**, https://doc.rust-lang.org/std/marker/trait.Send.html: *"Types that can be
  transferred across thread boundaries."*; *"automatically implemented when the compiler determines
  it's appropriate"*; *"An example of a non-Send type is the reference-counting pointer rc::Rc"*;
  the auto implementors list `impl Send for TcpStream` (and `TcpListener`). `verified`.
- **Swift SE-0302**, https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0302-concurrent-value-and-concurrent-closures.md:
  *"Implemented (Swift 5.7)"*; *"The Sendable protocol models types that are allowed to be safely
  passed across concurrency domains by copying the value."*; *"The compiler rejects any attempts to
  pass data across concurrency domains"* where *"the argument or result of an actor message send or
  structured concurrency call does not conform to the Sendable protocol"*; `@unchecked` overrides
  the check. Apple's own page, https://developer.apple.com/documentation/swift/sendable, rendered
  only its title to the fetch and is not cited. `verified` for SE-0302.
- **Dart `SendPort.send`**, https://api.dart.dev/stable/dart-isolate/SendPort/send.html: between
  isolates that do not share code, *"the transitive object graph of message can only contain"*
  `null`, booleans, `int`, `double`, `String`, collection literals and a few others; between
  isolates that share code, any object except *"Objects with native resources ... A Socket object
  for example refers internally to objects that have native resources attached and can therefore
  not be sent."*, and `ReceivePort`, `DynamicLibrary`, `Finalizer` among others; *"Objects that are
  identified as immutable (e.g. strings) will be shared whereas all other objects will be copied."*
  `verified`.
- **Dart isolates guide**, https://dart.dev/language/isolates: *"Isolates can only communicate via
  message passing"*; `Isolate.run` *"transfers the memory holding the result to the main isolate.
  It does not copy the data."* `verified`. Whether Dart once allowed less and widened (Dart 2.15's
  isolate groups): search results only, no page read, `unverified`.
- **PEP 554 then PEP 734: a narrow first rung, widened.** PEP 554,
  https://peps.python.org/pep-0554/, *"Superseded"*, created 05-Sep-2017: *"A minimal set of simple,
  immutable builtin types will be supported initially, including:"* `None`, `bool`, `bytes`, `str`,
  `int`, `float`, plus `memoryview` and channels; *"Further builtin types may be supported later,
  complex or not."* PEP 734, https://peps.python.org/pep-0734/, Final, Python 3.14, created
  06-Nov-2023: *"All objects that can be pickled are shareable. Thus, nearly every object is
  shareable."*; *"For most mutable objects, when one is sent to another interpreter, it is copied."*;
  *"objects which implement the buffer protocol, like memoryview ... Their underlying Py_buffer is
  actually shared between interpreters."*; file descriptors are among *"process-global state that
  interpreters will always share"*. Neither page says what the widening cost. `verified`.
- **Ruby `Ractor`**, https://docs.ruby-lang.org/en/master/language/ractor_md.html (read in two
  parts): *"Unshareable objects can only be used by the ractor that instantiated them"*; copy
  *"deep-copies the object to the other ractor"*, move *"moves membership to another ractor"*,
  *"The sending ractor can not access the moved object after it moves."*; *"There is a guarantee
  that only one ractor can access an unshareable object at once."*; *"Some T_DATA objects (objects
  whose class is defined in a C extension, such as StringIO) are not supported"*; *"$stdin,
  $stdout and $stderr are local to each ractor"*. The page says nothing of sockets. `verified`.
- **The Web's structured clone**, https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Structured_clone_algorithm:
  *"copies complex JavaScript objects ... to transfer data between Workers via postMessage()"*;
  *"Function objects cannot be duplicated by the structured clone algorithm; attempting to throws a
  DataCloneError exception."* `verified`.
- **Pony**, https://tutorial.ponylang.io/reference-capabilities/reference-capabilities.html: six
  capabilities; of `iso`, *"Since there is only one reference to it, isolated data cannot be shared
  by multiple threads, so there are no concurrency problems"*; of `ref`, *"you can't share it with
  other actors"*. The page does not use the word *sendable*. `verified` for what is quoted.
- **Tcl Thread**, https://www.tcl-lang.org/man/tcl8.6/ThreadCmd/thread.htm: `thread::send` *"passes
  a script to another thread"*; `thread::transfer` *"moves the specified channel from the current
  thread and interpreter to the main interpreter of the thread"* and *"waits until the other thread
  has incorporated the channel"*; *"the channel in question must not be shared among multiple
  interpreters running in the sending thread"*. `verified`.
- **Erlang**, https://www.erlang.org/doc/system/eff_guide_processes.html: *"All data in messages
  sent between Erlang processes is copied, except for refc binaries and literals on the same Erlang
  node."* `verified`.
- **Go**, https://go.dev/doc/effective_go: *"shared values are passed around on channels and, in
  fact, never actively shared by separate threads of execution"*; *"Do not communicate by sharing
  memory; instead, share memory by communicating."*; *"This approach can be taken too far."*
  `verified`.
- **OCaml 5**, https://ocaml.org/manual/5.2/parallelism.html: *"Immutable values may be freely shared
  between multiple domains"*; a data race: *"While the user may observe non sequentially consistent
  behaviours, there are no crashes."* `verified`.

### B3, moving a connection, numbers reused, and the uniqueness test: read 2026-11-12

- **Erlang `gen_tcp:controlling_process/2`**, https://www.erlang.org/doc/apps/kernel/gen_tcp.html:
  *"The controlling process is the process that the socket sends messages to."*; called from any
  other process it returns `{error, not_owner}`; in active mode it *"will transfer any messages from
  the socket in the mailbox of the caller"*; *"If any other process is interacting with the socket
  during the transfer, it may not work correctly"*. The page does not say what the owner's death
  does. Erlang's ports guide, https://www.erlang.org/doc/system/ports.html: *"If the port owner
  terminates, so does the port"*; that page does not mention sockets. `verified`.
- **Go `net.Conn`**, https://pkg.go.dev/net: *"Multiple goroutines may invoke methods on a Conn
  simultaneously."* `verified`.
- **Linux `unix(7)`, `SCM_RIGHTS`**, https://man7.org/linux/man-pages/man7/unix.7.html: *"Send or
  receive a set of open file descriptors from another process"*; *"equivalent to duplicating (dup(2))
  a file descriptor into the file descriptor table of another process"*; *"in the receiving process
  it is likely that a different file descriptor number will be used."* `verified`.
- **Windows `DuplicateHandle`**, https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-duplicatehandle:
  *"The duplicate handle refers to the same object as the original handle."*; *"You should not use
  DuplicateHandle to duplicate handles to ... Sockets. No error is returned, but the duplicate handle
  may not be recognized by Winsock at the target process. Also, using DuplicateHandle interferes with
  internal reference counting on the underlying object. To duplicate a socket handle, use the
  WSADuplicateSocket function."* `verified`.
- **POSIX.1-2024, 2.6 File Descriptor Allocation**, https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html
  (read from offset 100000): *"All functions that open one or more file descriptors shall, unless
  specified otherwise, atomically allocate the lowest"* numbered descriptor not open. `verified`.
  *My inference, not the page's*: a number closed is handed out again by the next allocation
  whenever it is the lowest free one. POSIX `open()`,
  https://pubs.opengroup.org/onlinepubs/9799919799/functions/open.html: *"If O_CREAT and O_EXCL are
  set, open() shall fail if the file exists."*, the check and the creation *"atomic with respect to
  other threads"*. `verified`.
- **Swift `isKnownUniquelyReferenced`**, its doc comment in
  https://raw.githubusercontent.com/swiftlang/swift/main/stdlib/public/core/ManagedBuffer.swift: *"If
  the instance passed as object is being accessed by multiple threads simultaneously"*, *"this
  function may still return true."*, *"Therefore, you must only call this function from mutating
  methods with appropriate thread synchronization."*, *"only returns true when there is really one
  accessor, or when there is a race condition"*, *"which is already undefined behavior."* `verified`.
- **Rust `Arc`**, https://doc.rust-lang.org/std/sync/struct.Arc.html (two parts): `make_mut` *"will
  clone the inner value to a new allocation to ensure unique ownership. This is also referred to as
  clone-on-write."*; `get_mut` (stable since 1.4.0) returns `None` *"because it is not safe to mutate a
  shared value"*; nightly `is_unique`: *"this call performs an Acquire operation on the underlying
  strong and weak ref counts"*, *"this operation requires locking the weak ref count, so concurrent
  calls to downgrade may spin-loop"*. `verified`.
- **Koka's Perceus**, MSR-TR-2020-42, https://microsoft.com/en-us/research/uploads/prod/2020/11/perceus-tr-v4.pdf
  (the PDF's pages 7 and 8, its running header *"MSR-TR-2020-42, Nov 22, 2020"*), § 2.7.2
  *Concurrent Execution*: *"If multiple threads share a reference to a value, the reference count
  needs to be incremented and decremented using atomic operations which can be expensive."*;
  *"Following the solution of Ullrich and de Moura [46], we mark each object with whether it can be
  thread-shared or not"*; *"All objects start out as unshared, and are only marked through explicit
  operations. In particular, when starting a new thread, the argument passed to the thread is marked
  as thread-shared."*; *"In Koka we encode the reference count for thread-shared objects as a negative
  value."* § 2.7.3 describes a race between a read's `dup` and a write's `drop` on a thread-shared
  mutable reference. `verified`. (Ullrich and de Moura is Lean 4's, which panel 113 already cites.)
- **PEP 703**, https://peps.python.org/pep-0703/ (Final, 3.13): biased reference counting, *"Each
  object is associated with an owning thread"*; nothing on a count-of-one test. Python's
  free-threading extension HOWTO, https://docs.python.org/3/howto/free-threading-extensions.html:
  nothing on it either. So the brief's line has no page: `unverified`.

### Part A: read 2026-11-12

The quotes are in § Part A's precedent above, each with its URL; every one was read today. Pages
that returned nothing usable: `https://raw.githubusercontent.com/rust-lang/cargo/master/src/cargo/util/flock.rs`
(404; read instead through the rustc docs' source view),
`https://raw.githubusercontent.com/Homebrew/brew/master/Library/Homebrew/lock_file.rb` (404),
`https://doc.rust-lang.org/cargo/reference/config.html` (nothing on locks), and pip's warning (two
searches, blogs only).
