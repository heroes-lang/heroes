# Panel 156 — historian brief

Read `docs/panel/156-briefs/00-shared.md` first.

You judge precedent. **Verify every claim by web search and give a source URL
and the date you checked it.** An unsourced claim is inadmissible — write it
down as unsourced rather than dropping or softening it. You have no veto.

**Your first attempt at panel 155 was killed by the watchdog at 600 seconds
while batching searches.** Work in small steps, do at most six searches, and
report early. A short sourced report beats a long one that never arrives.

## The question, in language-design terms

A compiled language with no runtime of its own beyond a small C library lets a
program call C directly. The C library dereferences a null pointer the program
handed it. The process faults. The language installs a signal handler (POSIX) or
a vectored exception handler (Windows) so that the program **says what happened**
instead of dying silently.

Two questions follow, and they are what the panel is deciding.

**A. What should the message NAME?** The fault is inside the C function. The
frame that called it is the author's own. Naming the C function is easy and
always available; naming the author's function needs a frame walk that can fail.
This language's handler currently names whichever it can find, so it names the
author's function on one platform and the C function on another.

**B. Must the program's own output survive?** The program printed a line before
it faulted. On one platform that line reaches the terminal, on another it is
lost in the buffer when the handler calls `abort()`.

## What to find, in priority order

1. **What do comparable languages NAME in a crash that happens inside foreign
   code?** Go panics with a full goroutine stack that spans the cgo boundary;
   Rust's `panic` names a source location and a backtrace behind an environment
   variable. The interesting case is what each does when the fault is inside C
   rather than inside its own code — Go's `SIGSEGV` traceback in cgo, and Rust's
   behaviour on a segfault in an `extern "C"` call, which is NOT a panic. Find
   the primary documents.
2. **Is naming the FOREIGN function alone ever the chosen design?** That is what
   this language does on its green platform, by accident. Find whether any
   language deliberately names only the foreign frame, or whether every one that
   can names its own.
3. **Flushing on abort.** Is there a documented, portable position on whether a
   crashing program should flush buffered output? C's own `abort()` is the
   primary source: what does C99/C11/C17 actually say about whether streams are
   flushed, and what do glibc and Apple's libc each do? This is the one item
   where the standard's own words may settle a panel question, so get the clause
   number.
4. **Async-signal-safety.** POSIX defines a list of functions safe to call from
   a signal handler. Is `fflush` on it? Is `write`? Cite the POSIX page. If
   flushing on the abort path is not safe, the panel needs to know that from the
   standard rather than from folklore.
5. **Windows vectored exception handlers**: what does Microsoft's own
   documentation say about `EXCEPTION_ACCESS_VIOLATION`'s
   `ExceptionInformation` array, and is reading `[1]` the documented way to get
   the address touched? Cite the MSDN/learn.microsoft.com page.
6. Only if you have room: **has any language shipped a crash handler that worked
   on two platforms and silently did nothing on a third**, and what did they say
   when they found out? The value here is the failure mode, not the fix.

## What your report must not do

Recommend a design. Smooth a finding that runs for or against any option. If the
precedent is thin, say it is thin.

## Process

The repository is FROZEN and you have no write tool, which is expected.

**RETURN YOUR REPORT AS YOUR FINAL MESSAGE, in full.** The coordinator writes it
to `docs/panel/156-reports/historian.md` verbatim and hands it to the
completeness critic the same way, so anything not in that message does not reach
the sitting. Include what you found per item, every source with its URL and the
date checked, what you could NOT verify, a falsifiable prediction, and the
condition under which you would change your reading.
