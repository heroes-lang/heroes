# Panel 256, historian (advisory, no veto)

Read `00-shared.md` first, whole. You build nothing and run no compiler. Your report:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p256/docs/panel/256-reports/historian.md`,
written as you go. **Every claim carries a source you read today (2026-11-22)**, its URL and the page's own
words quoted; anything you could not confirm says *unverified*. A language this sitting merely resembles is
not prior art; what a page states, and nothing of what Heroes might do about it.

## Q1: one wait for everything a program waits on

Who joined a message queue and the system's readiness in one wait, and what happened:

- **Erlang**: a process's one `receive`, and a port or a socket in active mode delivering data as messages
  (`gen_tcp`'s `{active, once}`), the flow-control lesson that made `once` and `N` exist;
- **Go**: `select` over channels, the network poller under goroutines, `context.Context` cancellation and
  `http.Server.Shutdown`;
- **Rust**: `mio::Waker`, `tokio::sync::Notify`, `tokio_util::sync::CancellationToken`, `tokio::select!`;
- **libuv**: `uv_async_send` (coalescing); **GLib**: `GSource`, `g_main_context_wakeup`, `g_unix_fd_add`;
  **Qt**: `QSocketNotifier`, `QCoreApplication::postEvent`; **AppKit/CoreFoundation**:
  `CFFileDescriptor`, `CFRunLoopWakeUp`, `NSApplication.postEvent(_:atStart:)`; **Win32**:
  `PostMessageW`, `MsgWaitForMultipleObjectsEx`, `WSAEventSelect`;
- **Python**: `selectors` beside a self-pipe, `signal.set_wakeup_fd`, `asyncio`'s `call_soon_threadsafe`;
- **the lost wake-up**: where a project documented a ring forgotten or put before the state it announces,
  and the level-triggered answer (a ready predicate on the queue itself);
- **a queue that hands out a descriptor** (eventfd-backed queues, Linux's `signalfd`, a self-pipe per
  queue), against a ring called back into the poller;
- **a scoped stop**: Go's `context`, .NET's `CancellationToken`, Java's `Thread.interrupt` and what it
  taught, Rust's `CancellationToken`; which of them a call blocked in the system can honour, and how.

## Q2: a rule on what a type holds, asked of a generic function

- **Rust**: `Send` as an auto trait, inferred from a type's fields, and a generic that must write `T: Send`
  to send a `T`; what an error says and where;
- **Swift**: `Sendable`, its checking of generic parameters, and the migration it cost;
- **C++**: templates checked at instantiation, the error messages that made concepts (C++20);
- **Standard ML**: equality type variables (`''a`), a restricted letter the compiler infers; **Haskell**,
  **OCaml**, **Pony** (reference capabilities, `iso` and `val`, sendable generics), **Ada** (generic formal
  types and their contract);
- **Go**: the `comparable` constraint and the history of `==` on a type parameter;
- what each found about **where the error is told**: in the body, at the call, or in a signature.

For each, one line in the form the decision issue will use, `- **Taken from <language>**:
[<reference>](<url>): <what the page states>` or `- **Reached first by <language>**: ...`, and say which of
the sitting's routes (`00-shared.md` § 3.3 and § 4.5) it bears on. End with the verdict structure of your
agent file, then one line per question: the route the precedent favours, and the one finding that would
change it.
