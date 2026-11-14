# Panel 248, historian

Read `00-shared.md` first, whole: the five questions and the routes each lists. You judge precedent,
advisory as always: who else met each question, what they built, what they refused, and what they had
to correct. Your report is `docs/panel/248-reports/historian.md` in `.claude/worktrees/p248`, in your
definition's output structure, **written by you, as you go** (`SKILL.md` 3c: the historian has `Write`
since 2026-09-29). You build nothing and run no paid call.

**Every precedent is a page you read today**, its URL and the day beside it, and says only what the
page states: a comparison is not an adoption, a mechanism the page does not describe is not cited, a
page you cannot read is not cited (`.claude/rules/records.md` § And the record says whose idea it was:
of 90 proposed prior-art lines, 33 said more than their page did). Mark each `verified` or
`unverified`. End with candidate prior-art lines per route in the two forms that section allows,
`Taken from` or `Reached first by`. Your `search` field names the queries and sources swept, so a
sentence about something not found says what was looked for and where.

**Where a fetched page lands**: panel 247's historian found its fetching tool had saved one page outside
the repository's root (`docs/panel/247-reports/historian.md` § One thing written outside the
repository's root, reported). CLAUDE.md § Hard stops writes no file outside the root: if your tool
saves a file, say where, and if it is outside `/Users/joseph/Temp/heroes/heroes-lang`, report it as 247's
did, for the author.

**Already read by earlier historians, cite them, do not repeat them**: panel 246's
(`docs/panel/246-reports/historian.md`) read Go's `net/http` `Server.Shutdown` (its lines 72 and
472-476, *"first closing all open listeners, then closing all idle connections"*) and Python's
`socketserver.serve_forever` (485-486), and the signal pages its lines 340 to 400 list; panel 247's
(`docs/panel/247-reports/historian.md`) read ThreadSanitizer's interposition of the registration calls
(its lines 53-55, the brief's own correction at 15), Go's issue 35814, libunwind's
`unw_is_signal_frame` and the runtimes its § By route covers. Re-read one only where this sitting leans
on a sentence that report did not quote, and say so.

The coordinator's list below is where to look, written from memory and **unverified**: a project named
here that does not do what the line says is a finding, not an embarrassment.

## Question 1: a runtime that defines the C library's signal installers

- **Java's signal chaining, `libjsig`**: the JVM's library that interposes `signal`, `sigaction` and
  `sigset` so an application's handler chains with the JVM's (Oracle's troubleshooting guide, its
  *signal chaining* page); how it is loaded (`LD_PRELOAD`, `DYLD_INSERT_LIBRARIES`, or linked), which
  installers it defines, and what it says of a library that installs by another path.
- **The sanitizers' interceptors**: compiler-rt's interception layer, on Linux by a definition of the
  same name and on Darwin by dyld's `__interpose` (00-shared.md measured the symbols and the section):
  what its documentation says of `signal` and `sigaction`, and of a program that defines them too.
- **Darwin's two-level namespace and dyld interposing**: Apple's `man dyld` and `man ld` on
  `__DATA,__interpose`, `-flat_namespace` (00-shared.md: `-force_flat_namespace` is refused by
  `ld-27106.1`), and whether interposing works from a program's own image.
- **Who refuses or defers a managed function a C library's own handler calls back** (R3's shape, not
  the direct case 247 covered): Go's cgo notes on a C signal handler calling into Go, the JVM's notes on
  native code calling back from a handler, .NET's.

## Question 2: a wait woken without slices

- **Go's network poller** (`runtime/netpoll`): one poller for the process, how a closed connection wakes
  its waiter, and how a deadline is a timer in it (00-shared.md's route (e)).
- **libuv's wake**: `uv_async_send` and the loop's own wake (an `eventfd` on Linux, a pipe elsewhere, and
  on Windows), from its documentation or design notes.
- **A self-pipe on Windows**: who makes a loopback socket pair `WSAPoll` or `select` can watch
  (libevent's `evutil_socketpair`, Python's `socket.socketpair` on Windows), and Microsoft's page on what
  `WSAPoll` watches.
- **Go's and Rust's choice of descriptor per waiter or per poller**, where their pages say why.

## Question 3: a child's working folder

- **`posix_spawn_file_actions_addchdir_np`** (glibc's release notes for the version that added it,
  Apple's manual) and **POSIX.1-2024's `posix_spawn_file_actions_addchdir`** (the Open Group's page).
- **`CreateProcessW`'s `lpCurrentDirectory`** (00-shared.md quotes the page; cite it, and say what other
  runtimes write about a relative folder or the `=X:` variables there).
- **How other runtimes give a child its folder**: Go's `os/exec` `Cmd.Dir`, Rust's
  `std::process::Command::current_dir`, Python's `subprocess` `cwd`, Node's `child_process` `cwd`: where
  each changes folder (in the child after `fork`, by `posix_spawn`'s action, by `fchdir`), how a missing
  folder is told, and how each finds a program named by a relative path when a folder is given (the
  order question 00-shared.md asks).

## Questions 4 and 5: a server that stops itself, and a connection that sends nothing

- **Go's `http.Server.Shutdown`** is 246's (above); new here: **`Server.Close`**, and how a handler asks
  its own server to stop (Go's documentation or its issues).
- **Go's `net.Conn` `SetReadDeadline`, `SetWriteDeadline`, `SetDeadline`**: what each promises (an
  absolute time, `os.ErrDeadlineExceeded`, a deadline that applies to calls already waiting).
- **Go's `http.Server` `ReadHeaderTimeout`, `ReadTimeout`, `IdleTimeout`**, and the reason Go's pages
  give for them (a client that sends nothing).
- **Node's `http.Server`** `headersTimeout`, `requestTimeout`, `server.close()` and
  `closeIdleConnections()`; **Python's `socket.settimeout`** and `socketserver.shutdown`.
- **A join that does not wait**: who offers one for threads (POSIX's `pthread_tryjoin_np` on glibc,
  Rust's `JoinHandle::is_finished`, Java's `Thread.isAlive`), and since when.

## What each corrected, and where the rule is written

For every precedent above that changed its rule, the version, the date, the change and the page that
says why: a deadline added after a server was found held by idle clients, a poller replacing per-waiter
wakes, a spawn API that gained a folder action. And for each: does the language's own reference say
it, or only its library's documentation? The spec here names the stop doors (`spec/heroes-spec.md:404`)
and the thread join (377) and no deadline, folder or installer (00-shared.md's greps); say which
precedents put such a rule in the language's reference and which leave it to the library's page.
