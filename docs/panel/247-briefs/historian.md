# Panel 247, historian

Read `00-shared.md` first, whole: **R4 is a ratified rule, no Heroes code runs in a handler, taken
from Microsoft's HandlerRoutine page** (decision 2497's `## Prior art`, `Taken from Microsoft`), **and
this sitting rules how it is enforced or honoured**. You judge precedent, advisory as always: who else
let a program written in a language with a runtime hand the operating system one of its own functions
as a signal handler, what each runs where, what each refuses, and what each had to correct. Your
report is `docs/panel/247-reports/historian.md` in `.claude/worktrees/p247`, in your definition's
output structure, written as you go. You build nothing and run no paid call.

**Every precedent is a page you read today**, its URL and the day beside it, and says only what the
page states: a comparison is not an adoption, a mechanism the page does not describe is not cited, a
page you cannot read is not cited (`.claude/rules/records.md` § And the record says whose idea it
was: of 90 proposed prior-art lines, 33 said more than their page did). Mark each `verified` or
`unverified`. End with candidate prior-art lines per route in the two forms that section allows,
`Taken from` or `Reached first by`. Your `search` field names the queries and sources swept, so a
sentence about something not found says what was looked for and where.

**Already read by panel 246's historian, and not to be repeated**
(`docs/panel/246-reports/historian.md`, its lines 340 to 400, each `verified`, and line 312): Python's
`signal` page (342-350: handlers *"always executed in the main Python thread"*, the low-level handler
setting *"a flag which tells the virtual machine to execute the corresponding Python signal handler at
a later point"*, `set_wakeup_fd`, the caveat on a long C call), PEP 475 (351), libuv's `uv_signal_t`
(356), Go's `os/signal` (362), Rust `signal-hook` 0.4.6's `flag` (371), Linux `signalfd(2)` (377),
FreeBSD `kqueue(2)`'s `EVFILT_SIGNAL` (382), POSIX.1-2024 (Issue 8) § 2.4 (388), and Microsoft's
`HandlerRoutine` (312 and 394). Cite them from that report; re-read one only where this sitting leans
on a sentence that report did not quote, and say so.

The coordinator's list below is where to look, written from memory and **unverified**: a project
named here that does not do what the line says is a finding, not an embarrassment.

## The routes, and the precedent each would rest on

- **S, the program's function run later at a safe point of its own thread, never inside the signal**
  (00-shared.md): Python's, by the page 246 verified (above: the main thread, a flag, *"a later
  point"*); Ruby's `Signal.trap` (where the trap's block runs, and what it refuses inside a trap, for
  example taking a `Mutex`); OCaml's `Sys.signal` (said to run at the next polling point: which
  polling points, since when, and what OCaml 5 changed); Erlang's `os:set_signal`.
- **E, the program's function run later on a thread the runtime owns, never inside the signal**: Go's
  `os/signal` (a channel; 246 verified the package page: re-read only for what its cgo notes say of a
  signal that lands on a thread C made); Java's `sun.misc.Signal` and `jdk.internal.misc.Signal` (on
  which thread the handler runs, and the *Signal Dispatcher*, if that is its name); .NET's
  `PosixSignalRegistration` and `Console.CancelKeyPress` (on which thread the handler runs); Node's
  `process.on('SIGINT')` (the event loop; libuv's `uv_signal_t` is 246's); Haskell's
  `installHandler` (a new thread).
- **C and A, a refusal.** Who refuses a managed function as a raw handler, at build or at run time,
  and with what words: Rust's `signal-hook` `low_level::register` and its `unsafe` contract (its
  `flag` module is 246's); Swift, Kotlin/Native, Zig, Nim's `setControlCHook` and what its
  documentation says runs inside the handler; Go's `runtime/cgo` and a C handler calling back into
  Go.
- **G, a door the runtime owns for any signal, read by the program** (00-shared.md): Go's channel and
  Python's `set_wakeup_fd` are 246's; who else offers a counted or queued signal to a program that
  reads it between steps.
- **D, a guard in the allocator.** Who detects re-entrance into their own allocator from a handler:
  glibc's `malloc` and its checks, Darwin's `libmalloc` (and the *"Trying to recursively lock an
  os_unfair_lock"* abort its libplatform raised in today's storm, 00-shared.md: what Apple's pages say
  of it), jemalloc, and the notes of any runtime that names the case.
- **The standard**: POSIX.1-2024 § 2.4 is 246's (cite it); new here: what it says of a handler that
  calls a function not on the async-signal-safe list *"when the signal interrupts an unsafe
  function"*, if its words are those, and Microsoft's `signal` page for UCRT (on which thread SIGINT's
  handler runs, and SIGABRT's, SIGFPE's, SIGILL's and SIGSEGV's).

## What each corrected

For every runtime above that changed its rule, the version, the date, the change and the page that
says why: Ruby's trap restrictions; OCaml's polling points across versions; Go's handling of a signal
on a non-Go thread; Java's deprecation of `sun.misc.Signal` if it happened; .NET's arrival of
`PosixSignalRegistration` and why. A precedent that began by running the program's code inside the
handler and moved away is the evidence this sitting needs most; one that refuses at build is the
second.

## Where the rule is written

For each: does the language's specification, or only its library's documentation, say what a handler
may do? The spec here names no signal (`grep -c -i 'signal\|handler' spec/heroes-spec.md` at the
base: 0) and R4 stands in a sitting's record and the runtime's messages, and the spec-warden asks
whether a sentence is owed; say which precedents put the rule in the language's own reference and
which leave it to the library's page.
