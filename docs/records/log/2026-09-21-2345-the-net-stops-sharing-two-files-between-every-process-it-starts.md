# The net stops sharing two files between every process it starts

2026-09-21 | every spawn gets its own redirect files, clang stops inheriting
the harness's capture, the process tree is killed rather than the process, and
the `FILE_SHARE_WRITE` bit is vetoed | one holder of one shared handle refused
120 spawns in CI, and widening the share mode would have traded that loud
refusal for a silent wrong answer | design.md §1.1's fourth force, on §1.12 |
**panel 174**, soundness lane

## What it settles

`runtime/parts/run.c` opened the harness's two redirect files `FILE_SHARE_READ`
and handed them to every child inheritable, and `tests/harness/shell.hero`
gave every one of the net's spawns **the same two paths**. A single lingering
process holding either handle therefore refused every later spawn.

That is no longer a hypothesis. It reproduced in CI during the sitting — run
**35651518558**, Windows job **106504808513** — and the instrument repaired
hours earlier printed `the operating system's own reason is 32` **120 times,
one number and no other**.

## Why the obvious repair is refused

Widening the share mode makes the reopen succeed. Both seats measured what it
costs. `CREATE_ALWAYS` truncates the file while the orphan keeps its own file
offset, so the orphan's later bytes land in the capture the harness is judging
a **different** case on: 150 bytes where 17 were owed, its own line, a NUL
hole, then another program's output — **identically on Windows, Darwin and
Linux**.

**So the hazard is not created by widening; it is already live on three
platforms and only Windows refuses loudly enough to be noticed.** Route A
deletes error 32, which is the one thing making it visible. §1.12's *"a
guarantee that ends quietly is not one"* decides it.

**The trap belongs to one bit.** `FILE_SHARE_READ | FILE_SHARE_DELETE` still
refuses the reopen *and* lets the scratch be cleaned; only `FILE_SHARE_WRITE`
opens the interleaving. The ffi seat vetoed that bit alone, and the veto binds.

## What the sitting changed its mind about

The brief accused the **watchdog** of creating the orphan: `TerminateProcess`
kills one process and not its descendants, so a terminated `heroes.exe` leaves
its `clang.exe` holding the inherited handle. That mechanism is real — the
critic reproduced the whole chain, grandchild `STILL_ACTIVE`, narrow reopen
answering 32.

**It is not what happened.** From the failing job's own timestamps, the `run`
suite finished **65 s** after `unsupported` did, and `WATCHDOG_SECONDS` is
**120**, per process. No process reached the limit. The watchdog route is
**excluded** for that run, not merely unrun.

So route C closes a **different** defect — one measured on Darwin at HEAD,
where the watchdog answers 124 while the descendant it was meant to stop is
still alive — and route B, a path per spawn, is what closes 074. The two are
recorded separately rather than one wearing the other's justification.

## What the critic falsified, and what it got wrong

It falsified the compiler-engineer's route C on POSIX: that patch sweeps the
process group only inside the timeout branch, and **both of that seat's own
probes arm a 2 s watchdog and hang the child**, so its instrument could not see
the gap it left. Against the same patched tree with the child exiting normally,
the capture still carried another case's bytes.

And it accused the brief of a false count that is not false: it measured 19
walked directories where the brief said 10, without asking how many of them
**report**. Ten carry `#~` markers; `deep` and `deepthread` carry zero and
cannot fail. The five failures are positions 1 to 5 of the ten that report. The
correction is recorded in the synthesis in both directions, because a record
that keeps only the flattering half is worth less than none.

## The source of the holder, found by a seat and not by the brief

`selfhost/cli/toolchain.hero:79` passes `out: ""` to clang, and
`runtime/parts/run.c:131-133` reads an empty path as **inherit**. So clang
receives the harness's own capture handle, which is exactly the holder recorded
at `tests/harness/shell.hero:414-417` in August. One word closes it at the
source, and it lands with the rest.

## What is refused, each with a measurement

`PROC_THREAD_ATTRIBUTE_HANDLE_LIST`: the handle reaches the child with
`HANDLE_FLAG_INHERIT` set and travels on to the grandchild, and excluding it
produces **0-byte captures silently**. A bounded retry: it tolerates rather
than removes, and its policy rests on a holder nobody has identified.

A pipe the parent owns was found by the critic, priced, and **deferred rather
than forgotten**: it removes all three hazards and deadlocks above the pipe
buffer, where the largest expectation already in the tree is 38,145 bytes.
