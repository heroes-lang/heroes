# Panel 251, historian

Read `00-shared.md` first, whole: the frozen commit (`3a5ee269117a31780ef1f96d0305452630ac4d83`), the three questions, the proposals
and their conservative alternatives, the record of each, and Rebel's witnesses. You judge precedent,
advisory: who decided each question before, what it cost them, and what changed their mind. **Your
report** is `.claude/worktrees/rebel-round/docs/panel/251-reports/historian.md`, **written by you, as
you go** (`SKILL.md:278-283`), in `.claude/agents/historian.md:40-49`'s structure (`verdict`,
`precedents` each marked `verified` or `unverified`, `argument` of at most 120 words, `condition`,
`search`), one block per question. You build nothing, start no paid run and print no credential.

**Every precedent is a page you read today**, its URL and the day beside it, and says only what the page
states: a comparison is not an adoption, a mechanism the page does not describe is not cited, a page you
cannot read is not cited (`.claude/rules/records.md` § And the record says whose idea it was: of 90
proposed prior-art lines, 33 said more than their page did). End each block with candidate prior-art
lines in the two forms that section allows, `- **Taken from <language or project>**: [<reference>](<url>):
<what the page states>` or `- **Reached first by <language or project>**: ...`. Your `search` field names
the queries and the sources swept, so a sentence about something not found says what was looked for and
where. **Where a fetched page lands**: if your tool saves a file outside
`/Users/joseph/Temp/heroes/heroes-lang`, say where in your report, as panel 247's historian did
(`docs/panel/247-reports/historian.md:510`, carried).

**Read before you search**, so you do not read again what a sitting already read: panel 249's
historian on D-rebind's forward compatibility and the glibc packaging
(`docs/panel/249-reports/historian.md`, its prior-art candidates at `:312-352`, carried); panel 248's on a
join that does not wait, Rust's `is_finished`, Java's `isAlive` and `join(Duration)`, glibc's
`pthread_tryjoin_np` (`docs/panel/248-reports/historian.md:65-82`); panel 246's on Java's shutdown rule
(`docs/panel/246-reports/historian.md:422`); design.md's appendix on Erlang's refc binaries
(`docs/design.md:4980-4991`, citing erlang.org's efficiency guide and binary handling pages). Re-read any
of them you lean on; a line copied from them is carried.

**Every item below was written by the brief-writer from memory, unverified**: each is a question, and a
language that does not do what its line says is a finding.

## Question 1: a function of the program reached from a signal handler

- **POSIX async-signal safety**: the list of functions a handler may call (XSH 2.4.3, *Signal
  Actions*), and what POSIX says of a handler calling a function not on it (undefined behaviour?); the
  Linux `signal-safety(7)` page.
- **Go**: `os/signal` (signals delivered on a channel, Go code never running in the handler?); and cgo's
  rule for a Go function called back from a C signal handler (the `cgo` command's documentation, the
  `runtime/cgo` pages): refused, unsupported, or undefined?
- **Rust**: `signal-hook`'s `low_level::register` and its safety contract; `signal-hook-registry`; the
  crate's pages on what a handler may do.
- **Swift**: `DispatchSource.makeSignalSource` (the handler run on a queue, not in the signal); what
  Swift says of Swift code inside a C signal handler.
- **Erlang**: `os:set_signal/2` and signals delivered as messages to `erl_signal_server`.
- **Interposing the C library's names**: fishhook's technique (rebinding a Mach-O image's symbol
  pointers) on current dyld, chained fixups and `__DATA_CONST` included, and Apple's `dyld`
  interposing (`__DATA,__interpose`); what Apple documents of either for the next macOS and for
  `macos-26`, the CI's runner (249's critic named you for this, `docs/panel/249-reports/critic-reports.md:372-376`,
  carried).
- **Ubuntu 26.04's glibc packaging**: whether its `libc.so.6` and its toolchain keep frame pointers
  (Ubuntu's 24.04 change to `-fno-omit-frame-pointer` by default, and whether 26.04 keeps it), which
  route C (frame records in every unit the compiler builds) rests on; 249's historian read 24.04 only
  (`docs/panel/249-*.md:242`, carried).
- **A jump out of a handler told from a jump within it**: any runtime or sanitizer that records the
  setjmp family's frames to tell the two (ASan's and TSan's interceptors of `setjmp`/`longjmp`, Boehm's
  collector, a language runtime); what their pages say.

## Question 2: the end of `main` with threads still running

- **C and POSIX**: returning from `main` against `exit` (C11 or C17 5.1.2.2.3); whether `exit` waits for
  other threads; POSIX's process termination and the other threads; `pthread_exit` called from `main`;
  C11's `<threads.h>` on a thread neither joined nor detached; `quick_exit` and `_Exit`.
- **Go**: the specification's *Program execution* (its words on `main` returning and goroutines still
  running); Uber's `goleak` as a check at a test's end.
- **Rust**: `std::thread::spawn`'s words on a spawned thread when the main thread ends; what dropping a
  `JoinHandle` does; whether `JoinHandle` is `#[must_use]`; `std::thread::scope`, the version that
  stabilised it.
- **C++**: `std::thread`'s destructor on a joinable thread (`std::terminate`?); `std::jthread` (C++20)
  joining in its destructor, and the paper that changed course (P0660?).
- **Java**: the JVM's exit when only daemon threads remain; structured concurrency
  (`StructuredTaskScope`, which JEPs and status).
- **Python**: non-daemon threads joined at interpreter shutdown, daemon threads stopped, and any recent
  change to daemon threads at finalization.
- **Swift** (a task group awaiting its children; a detached task at the end of `main`), **Zig**
  (`std.Thread`: joined or detached, enforced?), **Ada** (a master waits for its dependent tasks),
  **Erlang** (processes when the VM halts), **Trio** (nurseries, Nathaniel J. Smith's essay).
- **Refused when compiled**: a language or checker that refuses, at compile time, a spawned thread no
  path joins (linear or affine handles, Rust's `#[must_use]` as a warning, a lint, a structured design
  that makes it inexpressible); name what you searched if you find none.
- **Leak checkers and a thread still running at exit**: LeakSanitizer (does it stop the other threads
  and scan their stacks as roots?), Valgrind's memcheck; any runtime with per-thread heaps checking a
  heap at its thread's end.

## Question 3: threads that share nothing, against shared immutable values or channels of values

- **Go**: goroutines sharing memory, channels as the idiom (*"Do not communicate by sharing memory;
  instead, share memory by communicating"*, its source), `sync.Mutex`, the race detector; what a value
  sent on a channel is (a copy of the value, pointers shared?).
- **Rust**: `Send` and `Sync`; `std::sync::mpsc` and moving a value into a channel; `Arc` for a shared
  immutable value; what `Send` forbids crossing (a raw pointer, `Rc`).
- **Erlang**: per-process heaps, a message copied into the receiver's heap, the exceptions (refc binaries
  over 64 bytes, literals), and what the efficiency guide says a copy costs; a port or a resource
  (a C handle) moving between processes.
- **Swift**: `Sendable`, actors, value types crossing concurrency domains, region-based isolation
  (SE-0414) and `sending` (SE-0430): what may cross, copied or moved.
- **Beside the four, where a page says so**: Dart's isolates (messages copied, `Isolate.exit` handing a
  value over without a copy), JavaScript's workers (the structured clone, transferable objects whose
  sender loses them), Pony's reference capabilities (`iso` moved, `val` shared immutable).
- **What each paid**: a copy's cost reported by the language's own pages (Erlang's guide, Dart's), a
  data race reported despite the rule (Go's race detector's page), a reversal or a deprecation.
- **The handle that moves**: which of them lets a resource owned by C (a socket, a database connection)
  cross by giving it up in the sender, and in what words (Rust's move of a `Send` value, JavaScript's
  transfer, Erlang's `controlling_process` of a port or socket).
- **How a mailbox is addressed, and what a copy of the address is** (the critic's finding 7; the
  proposal's mailbox is a handle whose copies reach one queue, not design.md 7.13's queue per thread):
  Erlang's pid, copyable, naming a process's own mailbox; Go's channel, a reference shared by every copy,
  closed once; Rust's `mpsc::Sender` cloned and its `Receiver` held by one; Swift's `AsyncStream` and
  its continuation. What each says happens to a send after the close, and to a receive waiting when the
  close comes.
- **Spelled as a function or after a dot** (the critic's finding 2): Go's `ch <- v` and `close(ch)`,
  Rust's `tx.send(v)` and `JoinHandle::join`, Erlang's `!` and `receive`: whether any language that
  offers both a free function and a method form for its thread or channel operations says why, and
  whether any reserves the operation's name for every program (Go's predeclared `close`, shadowable?).

## What you are asked to say

Your definition's structure per question; per route of `00-shared.md` (Q1's J and route K; Q2's R, E and
J; Q3's any value copied with a mailbox that is a handle, spelled B or M, design.md 7.13's queue per
thread, the rung as it stands, plain data parallelism), the languages
that took it, verified with the page and the day; what each paid; and the candidate prior-art lines.
