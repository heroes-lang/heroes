# Panel 262, the historian's brief

Read `00-shared.md` first, whole: its head (the repair before any seat), § 1 (the proposal), § 2 (the questions;
question 1, the readings, question 4, the word *may*, and question 9, the aborts beside it), § 3.2 (how the sentence
came), § 3.6 (who in this project weighed it), § 4 (the candidates, (d1) to (d9)). **Your agent file is the base's**
(`.claude/agents/historian.md`): every factual claim carries a source, verified by a web search or a page you read, and
**anything you could not confirm is written `unverified`**. You have no veto. Your report is
`docs/panel/262-reports/historian.md`, written as you go, in the tree the coordinator's prompt names. You read pages;
you start no paid run, download nothing, and save no file but your report. **The ffi-pragmatist builds from your
answers at the same time and has no web** (the critic's C20): where a precedent describes a mechanism (the signal stack
trick above all), say exactly what the page says the code does, step by step, so the seat can name which of its
variants is the real one.

## The question you answer

**How do the documents this specification's tradition is written in state what a callback, or any function, may and
may not survive when C leaves a signal handler, switches stacks, or jumps, and how do they word an outcome that is
permitted but not promised, so that a reader neither widens nor narrows it?** Every precedent below is what the clerk
or the critic recalls, **none verified by this sitting**: each is a question for you, and a precedent you cannot source
is written `unverified` or dropped. For each, say: **(rule)** what it permits, forbids or leaves undefined, quoted;
**(scope)** how it bounds the class (a list of mechanisms, a property, a list of safe functions, a thread); **(word)**
the word it uses for a permitted outcome (*may*, *unspecified*, *undefined*, *implementation-defined*) and whether the
document defines that word; **(history)** when it entered, the bugs and complaints, and any revision, with dates and
versions.

1. **POSIX** (IEEE Std 1003.1, the 2024 edition if it is current, else 2017): *Signal Concepts* and its list of
   async-signal-safe functions; what a signal handler may call and what follows when it calls another (*"the behavior
   is undefined"*, as recalled); `longjmp` and `siglongjmp` from a handler (whether they are async-signal-safe, and
   what follows a jump out of a handler that interrupted an unsafe function: the case panel 258's refusal stops,
   `00-shared.md` question 9); `sigaltstack`; and **`makecontext`, `swapcontext`, `getcontext`, `setcontext`**: marked
   obsolescent and then removed (as recalled, in the 2004 and 2008 editions), with the reason the standard gave, and
   what it said of calling them from a handler. **And `timer_create` with `SIGEV_THREAD`** (a callback on a thread the
   implementation makes: question 8's shape).
2. **C11 (ISO/IEC 9899:2011) 7.13 and 7.14**: 7.13.2.1's `longjmp`, its sentences on interrupts, signals and *"a
   nested signal handler"* (as recalled: undefined), and what it says of a jump over frames; 7.14.1.1's limits on a
   handler (`volatile sig_atomic_t`, the library functions a handler may call); how C17 and C23 changed either. And
   C's own words for a permitted outcome: **unspecified behaviour** (*"two or more possibilities ... no further
   requirements on which is chosen"*, as recalled, 3.4.4) against **undefined** (3.4.3): which is the spec's *may
   abort* closer to, by the standard's definitions?
3. **Rust**: unwinding out of `extern "C"` (RFC 2945, the `"C-unwind"` ABI, and the release from which a panic
   crossing `extern "C"` aborts, as recalled 1.81); what the reference says of `longjmp` over Rust frames (*"plain old
   frames"*, as recalled from RFC 2945); and what the standard library or the reference says a signal handler may do
   (the `signal-hook` crate's documentation is a crate's, not the language's: say which you quote).
4. **Zig**: what its language reference or standard library says of a signal handler (`std.posix.sigaction`'s
   documentation) and of a function called back from C (`callconv(.c)`); whether Zig has a `setjmp`, and what it says
   of C's.
5. **Go's cgo**: the `cmd/cgo` documentation on calling Go from C (*"C code calling Go"*, threads Go did not create),
   and the `os/signal` documentation on non-Go signal handlers (`SA_ONSTACK`, as recalled); and **golang/go issue
   62440** (cgo callbacks after a coroutine library's stack switching, mended by refreshing the bounds at every
   callback, as panel 259's historian verified it, carried from `docs/panel/259-*.md:39-40`): what Go promises a
   callback after an unseen switch.
6. **The signal stack trick**: Ralf S. Engelschall's *Portable Multithreading: The Signal Stack Trick for User-Space
   Thread Creation* (USENIX, 2000, as recalled) and GNU Pth: a coroutine library that creates its stacks inside a
   signal handler on an alternate stack and jumps back into the handler's frame to start each one
   (`00-shared.md` § 2 question 8). Is it real, still shipped, and what does its paper say about calling code from
   such a frame? **Say, step by step as the paper or the code states it** (the critic's C20): does the handler
   **return** after its save, or **leave by a jump**; does the library **restore the previous alternate stack** after
   the creation, or leave its own? **And libcoro's `CORO_SJLJ`** (Marc Lehmann's, used by Perl's Coro, as the critic
   recalls it, **unverified**): a second library on the trick?
7. **The tradition the spec is written in** (`spec/heroes-spec.md:5`, Wirth's notation; the Algol 60 and Oberon
   reports, `.claude/rules/spec-shape.md`): how the Revised Report on Algol 60 and the Oberon report word an outcome
   left to the implementation (*"undefined"*, *"the effect is undefined"*), and whether either uses *may* for it.
8. **How a standard defines its *may*** (the reading question): ISO/IEC Directives Part 2 (*may* for permission, *can*
   for possibility, as recalled) and RFC 2119 (*MAY*, *"truly optional"*): which reading of *may abort* (permission to
   the implementation, possibility for the program) does each fix, and does either warn that a permission is read as
   a promise?
9. **Added by the repair, from the critic's C23, each the critic's recollection, unverified, a question for you**:
   - **JNI**: its specification has native code on a thread the VM did not start call `AttachCurrentThread` before
     calling Java (the precedent for the thread's refusal, the blind key's `n4`); and HotSpot's documentation of
     **signal chaining** (`libjsig`) for a VM that installs its own handlers beside a native library's (the precedent
     for a runtime with handlers of its own: every plain-built Heroes program installs SIGSEGV and SIGBUS handlers
     before C runs, `00-shared.md` question 2);
   - **Lua**: its manual's rule that a coroutine cannot yield across a C call that does not allow it, and the error
     *"attempt to yield across a C-call boundary"*: a language document wording a switch refused across C frames, the
     nearest thing to (a)'s *coroutine switch* sentence;
   - **Python's `signal` module**: *"a Python signal handler does not get executed inside the low-level (C) signal
     handler"* (as recalled): the remedy's own precedent (count the signal, run the code later), which
     `rebel/library/0.1.0/files/os.ledger:454` already cites by analogy;
   - **libcurl's synchronous resolver timeout**: `alarm` and `siglongjmp` around `getaddrinfo` (as the critic recalls
     it), and what curl's documentation says of `CURLOPT_NOSIGNAL` and threads: the watchdog shape panel 258's refusal
     stops.
10. **The spec's own conditional aborts, for comparison** (`00-shared.md` question 4; quoted there, read by the repair):
    *"running out of stack aborts"* (§ 9, no depth), *"C reaching past them aborts, or fails the call where the kernel
    writes"*, and *"aborts, ... unless C has since handed its address out again or a million other lives have ended"*
    (a stated exception). Does any precedent above word a conditional abort with *unless* rather than *may*, and which?

## What you write

One section per precedent, each ending with a line **`verified`** (a page read, quoted, its URL and the day you read
it) or **`unverified`**; then a table: precedent · what a callback or handler may do · how the class is bounded (a
list of mechanisms, a property, a list of safe calls, a thread) · the word for a permitted outcome · defined in the
document or not. Then which precedent each candidate of `00-shared.md` § 4 rests on (a list of mechanisms is (c)'s
shape, a property (b)'s, a safe list a route of its own, a thread rule (c)'s *"on its thread"*), and **what the history
says went wrong** where a document stated a permitted outcome. **Each line says what the page itself states**, never
what Heroes would do about it (`.claude/rules/records.md` § And the record says whose idea it was: of 90 lines four
researchers once proposed, three re-readers found 33 saying more than their page did).
