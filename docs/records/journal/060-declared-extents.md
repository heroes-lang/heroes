# 060 — M-declared-extents: the number a lend crosses with, the word it needs, and the report no word could carry

## Goal

Panel 164 had opened a route nobody had priced: a record's field crosses to C by
**address**, `f.ptr()`, so C may write back into it. The milestone's question was
the one that route leaves open — **how far may C read or write through that
address, and who says so** — and it was asked because defect 063 had been
measured: `sl_fill(p: s.name.ptr(), n: 64)` on an eight-byte field moved the
sibling to `0x41414141`, at exit 0, with every suite green, and every overshoot
from nine to 4096 bytes was as silent.

It ended somewhere else. The extent was declared and checked in the first four
steps; the milestone then spent twenty-five more on the question the extent
exposed — **what becomes of a pointer after the call** — and closed with a
default flipped by the author, a word in the language, and a report from the
runtime for the half no word can reach.

## What surprised

**Four of the seven defects this milestone closed did not exist when it
opened.** They were found by measuring the thing just landed, at the shapes
beside it: 069, 071, 072 and 073 were all written on days when the plan said
something else was next. A milestone's defect list is not a queue it works
through; it is what looking produces.

**A route can be adopted, built, and close zero of what it was adopted for.**
Panel 167 adopted a field lease. Panel 168 measured that it closes **zero of
two** — its own spec-warden had predicted exactly that, in those words, and the
sitting scored the prediction rather than waiting for the close. Then panel 172
adopted the word `consumes`; its compiler-engineer **built it**, 96 lines with
every suite green, and measured that it moves one of defect 070's four shapes,
the one that already failed to compile. Twice in one milestone, the thing that
settled a route was somebody building it and running the reproducer, not
somebody arguing about it.

**The corpus is not the world, and a sitting can only price what is in it.**
Panel 170 declined the default flip because *no measurement exists of what the
ninth refusal costs*: the corpus is `examples/` and `tests/golden/`, and the
author's own programs are not in it. The author then flipped the default anyway,
on a different ground — the language is young, nobody depends on it yet, and
robustness is a principle — with one condition, that the cost be measured. It
was: 33 nanoseconds a call at worst, zero where the mark is written.

**A word on a declaration cannot reach a function with no parameter.** The
finding that decided the last sitting is nine lines of Heroes: C stashes a
leased pointer on one call and frees it on a later one that takes no pointer at
all. Three of defect 070's seven measured shapes are like that — a callback
handed as a value, a later call, an out-cell — and every vocabulary anybody
proposed is a word written on a parameter. What could reach them was the
runtime, which already had a signal handler and already kept the count.

**The instrument that refuses a defect can be the one that hides it.** The net
fails any case whose sanitiser speaks, which is right for a correct program and
exactly wrong for a case whose subject is a program C corrupts: three cases that
close defect 070 could not enter the corpus at all. The repair was not to relax
the rule but to let a case **ask** for the sanitiser and assert what it must say,
so a case that stops provoking it goes red.

**Two seats measuring two different builds can both be right and neither
complete.** The ffi seat measured that the mechanism's prototype destroys a C
library's SIGABRT handler, ten runs of ten on two platforms. The engineer's build
already chained to that handler and had marked the branch unrun, having written
no such library. Nobody had run the combination, and the combination is what
ships.

**The document already said the thing the sitting had to be told.** Panel 172's
spec-warden found that both routes on the table were a row in design.md Part 6,
*rejected permanently*, ratified six days earlier; and panel 173's ffi seat found
that the runtime's own `stack.c` had settled the signal-chaining question years
of commits ago, with its reason in a comment. Neither was in a brief. A brief is
the one document in a sitting nobody is assigned to check.

**And a rule can name the trap and be fallen into anyway.** CLAUDE.md § 4 says a
ratification is recorded as a reading, and then, in the same paragraph, *CL-058
is about the record being TRUE, not about which claim is humbler*. The
coordinator wrote *not as a reading* on all three ratifications, and the author
corrected it within the hour: they read them.

## What broke and why

**The bootstrap, on the first attempt to land the word in one commit.** The seed
compiler that builds the next rung has to parse the marks that rung writes on the
compiler's own bindings: `selfhost/cli/process.hero:42:40`,
`error[expected_params_close]`, no binary. The landing is two commits, and the
sitting that measured it wrote that down before anyone tried again.

**The third tree nobody had counted.** The flip needed 27 marks on 19 functions,
and the brief counted nine functions in one tree. The ffi seat found the embedded
library's two, without which every `heroes check` of every program exits 2; the
engineer found `tests/harness/shell.hero`'s nine, without which no suite of the
net can run.

**A golden that would have been green on the bug it forbids.** The case written
to refuse a false second line of output was judged by containment, and
containment is satisfied by a stderr that carries both lines. Found by the seat
reviewing its own predecessor's work.

**The spec's first complete sentence broke the commit gate**, +55 vendored
against 50, and was compressed three times on the real instrument rather than
split across two commits to get under it.

**And the mechanism did not exist on Windows at all**, which the goldens found:
three of them were red on that leg, asserting a report the runtime says in its
own comment it does not make there. The repair was not to weaken the cases but
to measure the platform — a bad free raises `STATUS_HEAP_CORRUPTION` and a
vectored handler sees it — and write the arm that was missing. A golden that
goes red on one platform of four is the instrument doing its job; the mistake
would have been to make it green by asking it less.

**And on Linux, the mechanism did not compile at all under the sanitiser** —
`unknown type name 'sig_atomic_t'`, one missing include — which CL-055 makes
mandatory on that leg for every program that declares an `extern`, and which
Darwin hides completely. The seat that found it was the one told to run on the
platform rather than reason about it.

## What landed, and what carried forward

**Seven defects, all closed.** 063 the silent overshoot, 065 the write through a
lend the program may not write, 069, 071, 072, 073 — four of which did not exist
when the milestone opened — and then the three the last week was about: **066**
and **068**, closed by the flipped default, and **070**, closed by a report the
runtime makes rather than a refusal the checker writes.

**Ten sittings**, 164 to 173, two of them in the soundness lane. Three ratified
on the last day in one act, read.

**What the language gained**: `counted_by`, which relates the number a lend
crosses with to the field it crosses into, checked in C against the field's own
`sizeof`; `lent`, the word for a C parameter that keeps nothing of what it is
handed, under a default the author flipped so that silence means *keeps*; and
three sentences in `spec § 13`, the last of which is a limit carrying its own
falsifier.

**What the runtime gained**: one `hero_abort()` funnel, so every death the
runtime causes says one true thing; and a handler that names the leases alive
when the process is killed by something else, on the mechanism the stack guard
already installed. **It reaches all three platforms**, each measured on itself:
a signal on Darwin and Linux, and on Windows a vectored handler catching
`STATUS_HEAP_CORRUPTION`, which was a stub with a reason until the author
powered the box on the closing day and a twenty-line probe read the exception
code.

**What the net gained**: a case may now say `!sanitizer:` and assert the
sanitiser's own verdict, which is what let three programs C corrupts become
tests instead of staying anecdotes.

**Carried forward**, in `docs/work/milestones/M-declared-extents.md` and not
lost: two modules may declare one C function with contradictory marks, and the
compiler accepts both; `sqlite3_bind_text`'s three retention modes have no
single Heroes spelling; a pointer C made and freed twice by C is `check` 0 and
nothing in § 13 says a pointer C made is C's to free once; and `/panel`'s rules
still do not say that a seat's tree copy is its own.

**The gates at the close**: the whole net at **1980 passed, 0 failed** over
twenty-four suites, the compiler's own 675 and the net's own 161, on a compiler
built from the regenerated seed; Windows 130 and 0 twice over, its own tests 675;
Linux measured in the container. The net found two things at the close and both
were real: § Where we are had grown eight lines past its ceiling, and this
milestone's file still held four open items after it was closed, which is a
place nobody looks. The second opened **M-agreed-retention**, row 62.

**Four predictions are open with a horizon**, all of the shape *a program will
meet this shape within N milestones*:
`docs/records/done/2026-09-21-1320-the-predictions-of-m-declared-extents-scored.md`.
