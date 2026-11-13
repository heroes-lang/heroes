# Panel 247, historian

Read `00-shared.md` first, whole. You judge precedent, advisory as always: who else let a program
written in a language with a runtime hand the operating system one of its own functions as a signal
handler, what each runs where, what each refuses, and what each had to correct. Your report is
`docs/panel/247-reports/historian.md` in `.claude/worktrees/p247`, in your definition's output
structure, written as you go. You build nothing and run no paid call.

**Every precedent is a page you read today**, its URL and the day beside it, and says only what the
page states: a comparison is not an adoption, a mechanism the page does not describe is not cited, a
page you cannot read is not cited (`.claude/rules/records.md` § And the record says whose idea it
was: of 90 proposed prior-art lines, 33 said more than their page did). Mark each `verified` or
`unverified`. End with candidate prior-art lines per route in the two forms that section allows,
`Taken from` or `Reached first by`. Your `search` field names the queries and sources swept, so a
sentence about something not found says what was looked for and where.

**Already read by panel 246's historian, and not to be repeated** (`docs/panel/246-reports/historian.md`,
its lines 51 to 97 and 347 to 392): Rust `signal-hook`'s `flag` module, PEP 475 on `EINTR`, Python's
`signal.set_wakeup_fd`, Microsoft's HandlerRoutine page, Go's `os/signal` package page. Re-read one
only where this sitting leans on a sentence that report did not quote, and say so.

The coordinator's list below is where to look, written from memory and **unverified**: a project
named here that does not do what the line says is a finding, not an embarrassment.

## The routes, and the precedent each would rest on

- **E, the program's function run later, never inside the signal.** Python's `signal` module (the
  handler said to run in the main thread, between the interpreter's instructions, the C-level handler
  only setting a flag); Go's `os/signal` (signals delivered on a channel, Go code never run in the
  handler; and what its cgo notes say of a signal that lands on a thread C made); Java's
  `sun.misc.Signal` and `jdk.internal.misc.Signal` (a handler run on a thread of the virtual
  machine's, and the *Signal Dispatcher*, if that is its name); .NET's `PosixSignalRegistration` and
  `Console.CancelKeyPress` (on which thread the handler runs); Node's `process.on('SIGINT')` (the
  event loop, and libuv's `uv_signal_t`); Ruby's `Signal.trap` (and what it refuses inside a trap,
  for example taking a `Mutex`); OCaml's `Sys.signal` (at the next polling point); Haskell's
  `installHandler` (a new thread); Erlang's `os:set_signal`.
- **C and A, a refusal.** Who refuses a managed function as a raw handler, at build or at run time,
  and with what words: Rust's `signal-hook` `low_level::register` and its `unsafe` contract; Swift,
  Kotlin/Native, Zig, Nim's `setControlCHook` and what its documentation says runs inside the
  handler; Go's `runtime/cgo` and a C handler calling back into Go.
- **D, a guard in the allocator.** Who detects re-entrance into their own allocator from a handler:
  glibc's `malloc` and its `sigaction`-time warnings, Darwin's `libmalloc`, jemalloc, and the notes of
  any runtime that names the case.
- **The standard**: POSIX's definition of async-signal-safe and its list (the page, today), what it
  says of a handler that calls a function not on it, and which thread a process-directed signal goes
  to; Microsoft's `signal` page for UCRT (on which thread SIGINT's handler runs, and SIGABRT's, SIGFPE's,
  SIGILL's and SIGSEGV's).

## What each corrected

For every runtime above that changed its rule, the version, the date, the change and the page that
says why: Python's move of handlers to the main thread and PEP 475's retries; Ruby's trap
restrictions; Go's handling of a signal on a non-Go thread; Java's deprecation of `sun.misc.Signal`
if it happened; .NET's arrival of `PosixSignalRegistration` and why. A precedent that began by
running the program's code inside the handler and moved away is the evidence this sitting needs most;
one that refuses at build is the second.

## Where the rule is written

For each: does the language's specification, or only its library's documentation, say what a handler
may do? The spec here names no signal (`grep -c -i 'signal\|handler' spec/heroes-spec.md` at the
base: 0), and the spec-warden asks whether a sentence is owed; say which precedents put the rule in
the language's own reference and which leave it to the library's page.
