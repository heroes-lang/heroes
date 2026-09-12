# 035 — Every thread's stack, and what happens when it runs out

`M-thread-stacks`, opened and closed 2026-09-06. Panel 115. Four steps.

## The goal

Panel 107 adopted one thing on 2026-09-04 and could not build it: the stack
guard's bounds and alternate stack, per thread. What it recorded as the unsolved
half was that the handler cannot run on an exhausted stack without a
`sigaltstack` installed **on that thread**, and Heroes does not create the thread
— a C library does, so *"there is no point in a library's own thread where Heroes
code runs first"*.

The author moved this milestone ahead of M-declared-freer the hour
M-isolated-threads' step 6 landed, and gave the reason with it: that step is what
made the defect reachable. Until then no Heroes function ran on any thread but
`main`, so nobody could meet it.

## What surprised

**The door was written by the milestone before this one, and the sitting could
not have known.** `hero_spawn_enter` runs on a new thread before the Heroes body
does and already claims that thread for `parts/thread.c`. One call on the line
below it is the whole of panel 107's unsolved half **for every thread this
runtime starts**. The sitting had priced `pthread_introspection_hook_np` on
Darwin, an interposed `pthread_create` on glibc through `dlsym(RTLD_NEXT)`, and
an unexamined Windows. None of the three was needed.

**And the tidier version of that story is false, which a panel seat proved by
building the counterfactual.** Panel 115's ffi seat took the emitted C of a
program whose callback recurses, deleted the single line
`hero_thread_guard(...)`, and rebuilt against this milestone's own runtime:
**exit 132, empty stderr**. So the per-thread guard does nothing whatever for a
thread a C library made — `hero_stack_guard_enter()` has exactly two callers and
a foreign thread reaches neither. What saves that program is panel 111 R9's
refusal, from M-isolated-threads. The comment in `runtime/parts/thread.c` that
claimed the hole for this milestone was corrected in the same commit, because a
reader who later narrows the callback set would reopen a silent 132 while the
comment told them it belonged to somebody else.

**Windows was never broken, and three days of the record said it was.** Measured
on the box: before the repair a spawned thread's overflow was already `panic:
stack exhausted` at exit 127 there. The reason is a split nobody had written
down — `AddVectoredExceptionHandler` registers with the **process**, while
`sigaltstack` and the bounds are the **thread's**. Panel 107's *"three
platforms, three states of knowledge"* was two states and one wrong guess.

**The failing exit code was not one number but two.** macOS 132, Linux 139, both
silent. The record had carried 132 as the symptom since panel 107, because that
sitting measured one machine.

**A sitting convened on a number can refuse the number.** Panel 115 was asked for
one stack size on all three platforms and every seat that looked found 8 MiB
indefensible: glibc's default imported, an eightfold **cut** on Windows, and a
Win32 call that sets the commit rather than the reserve — a defect libuv ships
today. What the seats wanted was the **surprise** removed, and the surprise is
not that platforms differ: it is that one machine gives its own two threads
**19 levels against 312**. The floor needs no number, raises one platform and
lowers none.

**A document's silence can convert a loud failure back into a quiet one.** The
ergonomist wrote the eight-thread program from the spec alone and the silence
pushed it into a depth budget, a fallible return type, and a `.default(0)` at the
callback boundary where an error cannot cross — **a wrong sum at exit 0**. It
wrote that program while the compiler was already loud, because a loud
implementation cannot reach the author of a first draft. Its sentence is the one
this milestone is built around: *a loud implementation plus a silent document is
a trap that hides its own evidence*.

**And a wording can be killed by one command.** The warden ran `strip` on the
reproducer: `panic: stack exhausted`, **no function name**, because the name comes
from `dladdr`. Every candidate promising a name — panel 107's own, and the
ergonomist's first amendment — would have been false the day it landed. The
ergonomist then withdrew its own wording on a better argument than tokens: this
document never defines *abort*, it teaches the word by **nine instances**, so a
tenth in the same list inherits the meaning free.

**The record broke its own ceiling and the instrument was blinded by the break.**
§ Where we are held three stacked blocks, 41 lines against 15, and the check that
reads it was green — it counted to the first line **starting with** `---`, and
the mangled table separators `---|---|` start with `---`. It reported 14 and 27
lines were invisible. A self-check reading a record through a loose pattern can
only see what the pattern admits.

## What broke and why

**The runtime stopped compiling under the sanitiser, and only under it.**
`spawn.c` called `sysconf(_SC_PAGESIZE)` while leaning on `<unistd.h>` coming in
from `stack.c` above it in the one translation unit. Under `--sanitize` that
file's whole POSIX half compiles away and the include goes with it: ordinary
build green, `heroes build --sanitize` answering `internal error: the runtime did
not compile`. Caught before the commit by running the configuration rather than
the default. A file that leans on another file's includes works until that file
grows a configuration.

**An item said two documents were wrong and one of them had been right for two
days.** The scheduled item about the Windows `/STACK` divergence claimed that
CLAUDE.md § Commands *and* `seed/README.md` print the plain line; `seed/README.md`
had carried the flag since `8c10d5a2`, filed two days before the item. A premise
taken from a document rather than run.

**A negative claim nearly went into a step of work.** A search for *Chi ha
scritto* in the Italian site page returned nothing and was one sentence away from
being written down as *the Italian edition does not have it*. The page says **Chi
lo ha scritto**, in full, and had since a commit twenty minutes after the item was
filed. A failed search is a fact about the searcher's vocabulary.

**A count was taken from a shallow glob.** The site was about to be recorded at
18 pages; `find` says **46**, 23 English and 23 Italian, which is what every other
record says. The first command that runs is not the measurement.

**And a run was discarded with its reason.** The first full net started at 15:29
and a peer session committed to `docs/ROADMAP.md`, `DESIGN-LOG.md` and
`tests/harness/suite_records.hero` at 15:35 while `records` was reading them. Part
of that run saw one tree and part another, so it was killed and redone. The peer
killed its own run in the same window for the mirror reason, having seen this
session's half-written runtime under it.

## Predictions, scored

**Panel 107's coordinator, due at the milestone that lands the per-thread
guard.** *"With the guard's bounds per thread, overflow inside SDL's audio
callback becomes exit 134 with a named function, and the interpreter's ceilings
in all three configurations move by zero levels."*

- The ceilings half **HOLDS**, and it was binary-searched rather than assumed,
  with a before-compiler and an after-compiler built from the same tree:
  `-O0` **314 → 314**, `-O2` **585 → 585**, `--sanitize` **244 → 244**.
- The callback half **holds in outcome and is wrong in mechanism**, which is the
  honest way to score it. A Heroes callback on a thread a C library made is exit
  134 with a name, but the name is the callback's and the message is
  `parts/thread.c`'s isolation refusal, not the stack guard's. Panel 115's ffi
  seat measured that the guard contributes nothing there.

**Panel 107's ffi-pragmatist, due at the next milestone that touches
`runtime/parts/stack.c`.** Two halves, and they score differently.

- The Linux half — *"every `heroes build examples/sqlite/main.hero` prints the
  ignored-flag warning"* — is **lapsed by refusal, not falsified**. It was
  conditional on shipping `-Wl,-z,stacksize=` to Linux, which that same sitting
  refused permanently. A prediction whose antecedent never happened is not
  scored, and under §1.6 it is not renewed either.
- The Darwin half is **half held and half falsified, and the falsified half was
  right about the mechanism.** *"Still get 536,576 bytes"* **HOLDS**, re-measured
  2026-09-06 with `pthread_get_stacksize_np`. *"And still exit 132 with empty
  stderr"* is **FALSE as an observation** — it is exit 134 with a message — but
  panel 115's ffi seat deleted `hero_thread_guard` from the emitted C and rebuilt
  against this milestone's own runtime and got **exit 132, empty stderr**. So the
  seat's underlying claim, that this milestone does nothing for a foreign thread,
  is exactly right, and only its expectation about a different milestone's guard
  was wrong.

## What the numbers were at the close

Three suites on this Mac: the compiler's own **583**, the net **1578**, and the
net's own tests **113**. The spec **3824** of a hard 4096, headroom **272**. One
blessed emission was re-blessed with its diff read: `examples/threads/main.hero`,
+195/−124, every removal a `#line` directive shifting under two inserted
functions except one statement that moves relative to a directive and is
re-added on the next line.

**And the net's own cost is worth recording, because the contract's number is
the warm one.** With the runtime changed, so that no cached object survives, the
full net ran for **a little over 13 minutes** against the 11m16s the block
prints. That figure is read off the process's own elapsed time and not taken with
`/usr/bin/time -p`, so it is stated as the shape it is: about two minutes longer,
not a measurement anybody should quote to three digits. The reason to write it
down at all is that the printed number comes from a tree whose cache is warm, and
a runtime change is exactly the case where it is not.

**A number that was read off the wrong process for twenty minutes, and it is the
cheapest lesson here.** While that run was in flight its elapsed time was read
from the wrapping shell rather than from `./heroes` itself, so the run looked
like it was at 25 minutes when the compiler had been going for 11. Nothing was
decided on it and nothing broke, and it is written down because the failure is
the one CLAUDE.md § Commands describes about reading: the command ran, the output
was read, and the last step — check that the row is the row you meant — was
skipped because a number was already in view.
