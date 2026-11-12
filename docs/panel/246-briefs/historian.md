# Panel 246, historian

Read `00-shared.md` first, whole. You judge **Part B** (advisory, as always); for **Part A** you are
not a seat of its lane (the author's `2a`: compiler-engineer and ffi-pragmatist), and your precedent
on it is input to the synthesis, not a verdict. Your report is `docs/panel/246-reports/historian.md`
in `.claude/worktrees/p246`, in your definition's output structure (Part B first, then Part A's
precedent), written as you go. You build nothing and run no paid call.

**Every precedent is a page you read today**, its URL and the day beside it, and says only what the
page states: a comparison is not an adoption, a mechanism the page does not describe is not cited, and
a page you cannot read is not cited (`.claude/rules/records.md` § And the record says whose idea it
was: of 90 proposed prior-art lines, 33 said more than their page did). Mark each `verified` or
`unverified`. End with candidate prior-art lines per route in the two forms that section allows,
`Taken from` or `Reached first by`. Your `search` field names the queries and sources swept, so a
sentence about something not found says what was looked for and where.

The coordinator's lists below are where to look, written from memory and **unverified**: a project
named here that does not do what the line says is a finding, not an embarrassment.

## Part B

**B1, what crosses into a thread, and where a language states it.** At the base one `i64` passes in
and one out (`runtime/hero_os.h:435-447`); panel 113 had ratified *"the scalars and `str`"* and the
runtime shipped less (00-shared.md § Part B, B1). Who restricts what crosses into a thread or isolate,
and does the rule stand in the **language's specification** or in a library's documentation: Rust
(`Send`, `thread::spawn`'s bounds), Dart (isolates, what a `SendPort` carries, `Isolate.run`), Python's
subinterpreters (PEP 734's shareable types), Ruby (`Ractor`, shareable objects), Erlang (copying
between processes), Tcl (`thread::send`), the Web's workers (structured clone), Swift (`Sendable`),
Pony (reference capabilities), Go and OCaml 5 as systems that share. Which began with a narrower rung
and widened it, and what the widening cost.

**B3's handle: moving a connection to another thread.** A connection here is a handle the platform
layer issues and checks (panel 234's R3; `runtime/hero_net.h:45-46`), and the rung passes no handle.
Who moves an open socket between threads, isolates or processes, and how ownership goes with it:
Erlang's `gen_tcp:controlling_process/2`, Rust's `TcpStream` being `Send`, Dart's sockets and isolates
(whether a socket can be sent at all), Pony's TCP connection actors, Go's `net.Conn` shared freely; and
who numbers a handle so it can cross as an integer (Windows' `HANDLE` duplication, POSIX `SCM_RIGHTS`
between processes), with what each says of a number reused after close.

**B2, a program that stops cleanly.** The shapes priced here: a runtime part owns a flag SIGINT and
SIGTERM set, a console control handler on Windows, and the loop asks it between connections (S1 to
S3); **(a)** the signals blocked and asked with `sigpending`, or taken by `sigwait` on a thread of
their own; **(b)** the stop as a descriptor one `poll` waits on beside the listener; **(c)** the stop
inside the networking layer's own blocking calls; and a program's own C header holding the flag. Who
ships which, and what each does when a loop is blocked in a system call: Go's `os/signal` (`Notify`,
`NotifyContext`), Rust's `signal-hook` (its `flag` and `iterator` modules) and `ctrlc` crates and
`tokio::signal`, Python's `signal` module, `signal.set_wakeup_fd` and PEP 475 on `EINTR`, libuv's
`uv_signal_t`, kqueue's `EVFILT_SIGNAL` and Linux's `signalfd(2)` (their manual pages), Java's shutdown
hooks, .NET's `Console.CancelKeyPress` and `PosixSignalRegistration`, Node's `process.on('SIGINT')`,
Nim's `setControlCHook`, OCaml's `Sys.signal`, Haskell's `installHandler`, Erlang's `os:set_signal`.
Also: the convention that a second Ctrl-C ends the program at once (who documents it); which thread
POSIX delivers a process-directed signal to (the standard's words); Microsoft's
`SetConsoleCtrlHandler` and on which thread its handler runs; POSIX's async-signal-safe list; and
whether a server's clean stop is where any of these languages runs its leak or finaliser checks
(task 1072 here: the runtime's leak check runs only when `main` returns, `runtime/parts/os.c:1005-1006`).

**B3, a uniqueness test and threads.** Copy-on-write here decides *"mutate in place"* by
`refcount == 1`, a test then a mutate (`runtime/parts/cow.c:44`). Panel 113 already cited Lean 4's
`lean_is_exclusive` and CPython 3.13t (`grep -n 'Lean' docs/panel/113-*.md`); do not repeat them,
re-read them only where B3 now leans on them. New: Swift's `isKnownUniquelyReferenced` and what its
documentation says about threads; Rust's `Arc::make_mut` and `Arc::get_mut` (how the uniqueness check
is made sound against another thread); Koka's Perceus and a value shared across threads; PEP 703's
notes on code that tests a reference count of 1.

## Part A's precedent: a lock the operating system releases, a name that stands, a killed run

Defect 2453, `blocking` since 2026-11-12 (`gh issue view 2453 --repo heroes-lang/heroes --comments`;
00-shared.md § Part A): a killed `heroes add` or `fetch` leaves its `.fetching-<pid>-<n>` folder, and a
later run handed the same process number removes it, the author's edit inside. Route A gives each run
a lock its death releases and recovers only the folders whose lock it can take; B reports; C pins; D
never removes a standing staging name, as this project's `heroes new` already rules. Who did what,
read today:

- **Cargo**: its package cache lock (*"Blocking waiting for file lock on package cache"*), its `flock`
  module, and what it does on a file system where locking is not supported.
- **Go**: the module cache, `cmd/go/internal/lockedfile`, and how an interrupted download is found
  and redone.
- **npm** (and `cacache`), **pip** (the `~`-prefixed folders an interrupted install leaves in
  `site-packages`, and its *"Ignoring invalid distribution"* warning, if that is what it is), **uv**
  (its cache locks and what it says while waiting).
- **git**'s `index.lock`, created exclusively, left by a killed process and refused by a later
  command, the opposite of A and the cousin of D; **Nix**'s temporary roots, a per-process file whose
  lock tells a live process from a dead one, if its source says so; **Homebrew**'s lock files.
- **A process number in a file name or a pid file**, and how tools that use one guard against its
  reuse (a daemon's pid file and its staleness check; `mkdtemp(3)`'s and `O_EXCL`'s refusal of a name
  that stands).
- **SQLite**'s documentation on file locking over network file systems, for the NFS and SMB question.
