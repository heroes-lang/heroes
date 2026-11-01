# 074 — M-latency-ceiling: a latency the compiler may not lose without a decision, its instrument, and the 2.0 % it found the day it was built

## Goal

CLAUDE.md § Verification says *never slow the compiler down*, and until this row nothing held it
over months: a timing was taken by hand at a close and written into a commit body, and a slow
creep of one per cent a batch read as nothing at every close. The row, scheduled by the author on
2026-10-14 before the core packages, delivers the instrument and the ceiling it holds:
`tests/harness/latency.hero`, a root of the harness with three words, `time`, `compare` and
`record`; its ledger, `docs/measurements/061-the-latency-ledger.md`, a row per close; the net's
suite `latency`, which holds the newest row to its pins and to the level and times nothing;
decision 2282, the level the newest row may not pass (7.50 s for `heroes check
selfhost/main.hero`, 9.00 s for clang on the seed, 41.00 s for the warm build) and the floor of
effect below which a close's comparison counts no loss (0.5 %); the bullet of `/step`'s close
checklist that runs both at every close; and a sentence in `CONTRIBUTING.md` naming latency as
the one speed that is a reason for a change, since a model writing Heroes pays a check on every
turn. The incremental frontend stays the ceiling's return condition, built by no row until the
ceiling would give way (issue 715, settled at this close).

## What surprised

**The first number the row wrote down was already false in public.** The start page and the home
page, both editions, said the seed builds *under four seconds* on a MacBook Air; the opening
measured 8.29 s by hand and the instrument 8.18 s on that MacBook Air, the seed grown from 22 MB in
August to 28.9 MB. A threshold typed from one reading expires in silence and goes on reading as
true (defect 2281, `blocking`); the four sentences now say *nine*, and the site's claims check
holds them to the ledger's newest row, falsified once before landing.

**A best-of-N comparison missed a real two per cent.** The instrument's first verdict read two
compilers by their bests against the larger of their spreads, and on its first real use it said
*within the floor of 0.22 s* of a sitting whose five clean pairs all put the second compiler
behind, by 0.10 to 0.25 s. A best is one run of each side, and the pairs are what alternating
bought. Read pair by pair, with the order inside each pair alternating and a sign test counted
exactly in integers, the same two compilers read 8 of 8 pairs slower, a median 0.14 s, a tail of
0.4 %. And the spread the old rule called noise was mostly the machine warming: on this fanless
MacBook Air both compilers crept from 6.53 to 6.80 s and from 6.69 to 6.94 s across one sitting,
a drift a pair's two runs share and its difference cancels.

**Then the paired test was too sensitive for a ceiling.** Against the base of task 2279, a build
its bisect found free of the regression read 0.03 s faster, 0.4 %, and the repaired compiler read
0.03 s slower: the test resolves differences that small between any two builds. A ceiling that
counted them would ring at every close, so decision 2282 gives `compare` a floor of effect, 0.5 %,
and the level catches what the floor lets through only by accumulating.

**A better message for one mistake cost every correct program.** Task 2279's 2.0 % on `heroes check
selfhost/main.hero` bisected to defect 2261, which tells labels written in one another's place as
one mistake: its rule ran at every call and every construction and, before learning that nothing
was moved, built the parameters' names, copied every label and made a map of positions, the walk
handing it the whole tree and source by value, which the emitted C copies as structs. On a program
written right that is every call. Asked first of the bytes, with nothing allocated, the rule gives
back 0.11 s, 1.6 %, behaviour unchanged on 2,570 roots.

**A premise of the repair, measured false.** The residual 0.03 s against the base was put down to
the source record passed by value into the new questions; the questions were given the text alone,
and the residual read the same, 0.03 s. It is left unattributed, inside the band two unrelated
builds differ by, and the commit says so.

**Inside a batch neither the seed nor the trunk builds an old commit's compiler.** The bisect's
points between the runtime's step from ABI 31 to 32 and the seed's refresh held a runtime at 32
and a seed at 31, and the trunk's compiler, too new, compiles in a library calling
`hero_panic_library_at`, which those runtimes lack. What built every point the same way: the base's
compiler building the point's stage 2 against the base's runtime, then that stage 2 building the
point's stage 3 against its own, which is the point's exact compiler.

**The language held its own compiler to its rule.** The repair's `written_as(text: str, label:
token.Span, name: str)` has two `str` parameters, so the compiler refused the five calls that
named neither, with a `certain` fix: swapping the text and the name would have been a silent bug.

## What broke and why

**In the process.** Step 0 committed `ROADMAP.md`, a path the site's build reads, without first
saying how many site commits would travel and waiting for the author's yes, which CLAUDE.md § Hard
stops asks before committing; nothing had been pushed, every other session was offline, and the
author was told the same turn and asked before step 1, whose ledger is a site path too. The
guards refused four commands, each rightly: twice a suite's output cut with `tail` or `head`,
once a harness run on a compiler built from sources older than the tree, once a line of the page
naming the chip `M3`, which `records/numbered` reads as a numbered milestone. `compare` was
repaired twice by its own first uses: its bests-only verdict (above), and mostly tied pairs read
as a machine that was not still, where they are two compilers reading the same. The `canonical`
suite's floor had been outgrown by 25 % since batch 19 and was raised with this row's first gate.

**In the measurements.** The first run of one sitting waited 310 s, `real` 316.66 against `user`
6.63, cause unmeasured; the rule discarded it twice, as the first and as waiting, which is what
both halves of the rule exist for. And the close's comparison of the warm build was disturbed half
way by another application on this Mac, a browser's renderer taking a whole core: two `before` runs
read 45.45 and 51.08 s, `user` itself grown to 41 s on a fanless machine warming, their pairs
dropped, four kept, no verdict; sat again under that load, now steady, it gave seven pairs and a
verdict. A burst breaks pairs where a steady load does not, since both runs of a pair carry it.

## What landed, and what carried forward

**What landed**, every item a commit on `main`, each pushed on the author's yes, given for the
session at step 1 and again as they left it: `ccdf13f53`, step 0, the row opened and its clocks
first run by hand, defect 2281 filed; `4bde900c2`, step 1, the instrument, its ledger and first row
(6.81, 8.18 and 37.25 s), the suite `latency`, the `canonical` floor raised from 1,361 to 1,704;
`d33925de0` and `fc6946023`, step 2, `compare` reading alternated pairs by a sign test and mostly
tied pairs as two compilers reading the same; `fba1101a1`, step 2, task 2279 bisected to defect
2261 and given back through `check/labels_in_place.hero`, the census of `check` over 2,570 roots
moving none; `04e710a86`, defect 2281, the site's four sentences held to the ledger; `fe6903aaf`,
step 3, decision 2282 applied with the close checklist's bullet and `CONTRIBUTING.md`'s sentence.
Issues 2279, 2281 and 2093 closed; 2282 a decision taken with the recommended default while the
author was away, closed for the author to read and turn; 715 settled at this close.

**Re-measured at the close, 2026-11-01**: gate A, the seed of M-compatibility-promise, not
refreshed, building stage 1, stage 1 building stage 2 and stage 2 stage 3, the two emitting
identical bytes (28,897,328); gate B, the full net on stage 2, 35 suites, 10,551 passed and 0
failed; the compiler's own tests 1,790 and the net's own 505; the site's build green. The ledger's
second row, by `record` on stage 2 (`sources 0939902c3b85306c`), seven runs per clock: `check`
6.91 s, the seed 8.18 s, the warm build 37.20 s, each under decision 2282's level. `compare --clock
all` against M-compatibility-promise's compiler (`sources 23825438285ee301`): `check` faster in
every untied pair, a median 0.11 s, 1.6 %; the build, at its second sitting, slower in 2 of 7
untied pairs, the median 0.06 s faster, no loss counted.

**Carried forward**: the tag (below); the incremental frontend, this row's return condition, built
by no row until the level or a close's comparison gives way; the residual 0.03 s against task
2279's base, unattributed; the clock for `heroes check` on a program with packages, a column of the
ledger the day M-core-packages has such a program; tcc as a development build and a third witness
of the seed's C, the page's question, unrun; the CI's legs on the close's push and the site's
deploy, read after it.

## The tag waits, 2026-11-01

Defects 2275, 2277 and 2278 were filed `adjacent` beside M-compatibility-promise's batch on
2026-10-29 and 30 and stood open at its tag, so `m-latency-ceiling` is their second, at which
`records/tagged` counts an `adjacent` defect as `blocking` (`.claude/rules/verification.md`
§ Bounded discovery). Measured, not inferred: the tag placed locally on `fe6903aaf`, `records` read
27 and 1, `records/tagged` naming the three, *open at 2 tags*, and the tag was taken off again,
which is the answer `/step` § 5 gives a refusal. 2275, a construction short of a field told
`cannot_infer` at each value only its field could type, and 2277, a stack exhausted in a callback's
frame at `-O2` told under a module named `0cb`, are repairs of the checker and of the runtime.
2278, a record of records some megabytes deep keeping clang at `-O2` past ten minutes, says itself
that whether such a cost is the compiler's to pay is a sitting's question (CLAUDE.md § 13), and a
panel's gate and its paid seats are the author's. So the close is made and the tag waits on the
three, as `m-panic-location`'s waited on decision 2228: the row says done, and the tags stay 74.

**Later the same day, 2026-11-01**, defects 2275 and 2277 were repaired in one round gated once:
`e1b4d5b0d`, a construction short of a field checking each labelled value against its field, and
`3cb9f7c90`, the runtime telling a callback thunk's marker without; the runtime's key moved every
program's emission by its provenance line, re-blessed in `b80265fe6`, 709 files of one line each,
the two this Mac skips in the Linux arm64 image. Gate A's fixpoint, the compiler's own tests 1,791,
the net's own 505, the full net green once blessed, and the census of `check` over 2,572 roots moving
one, 2275's own case. Both issues closed. The tag waits on 2278 alone, and on the sitting the author
convened on it the same morning, answering decision 2283 *(A)*: the soundness lane, the
compiler-engineer and the ffi-pragmatist, on a measured reproducer.


## The tag, 2026-11-01: panel 233's route landed and defect 2278 closed

The sitting the author convened sat the same day and found the cause where no
guess had put it: not the stack, which defect 2253's guard already watched, but
**clang's SROA**, which splits what a frame holds into its leaves and takes
96.3 % of the `-O2` time doing it. A record of records 18 deep, four mebibytes
of it, cost **314.40 billion instructions and 12.31 GB** of clang; off the
frame it costs **0.398 billion and about 12 MB**, and the same program now runs
at `-O0`, where its frame had aborted.

So a type whose named fields pass 4,096 bytes, or that holds a header's
`partial` record by value, no longer lives on the frame: it takes its room from
a per-thread last-in-first-out room of the runtime, bounded at a gibibyte for
the whole process (`runtime/parts/frame.c`); a large result is written into its
caller's slot, a large plain parameter arrives as `const T *`, and a
construction is written in place where it is going
(`selfhost/emit/large_value.hero`). Every `extern` call and every callback keeps
C's ABI byte for byte.

**The gate rewrote the route, and that is the story of the landing.** The
sitting's resolutions were sound and two of them were short, and neither was
found by reading them.

*The first came from `wholes`*, panel 182's instrument, which refused ten of the
route's own emissions. A construction wrote field by field with nothing writing
the value whole, so the bytes no field names kept what the room last held, and
because the room is given back last-taken-first, *what it last held* is another
value's bytes, not merely indeterminate ones. The store that follows a
construction copies `sizeof` bytes and carries them into the caller, and where
the record is lent to C, panel 215's R1 promises exactly those bytes are zero.
Every construction now opens by writing the whole value zero.

*The second came from asking clang a question nobody had asked it.* Panel 233's
R1 counts a **variant** among the large types, the widest case plus its tag, so
a variant's temporary already lived in the room; R2 wrote in place only a
**record**'s construction, and the compound literal left behind put the whole
value straight back on the frame R1 had moved it off. The same held of a `T?`.
`clang -O2 -Wframe-larger-than=65536` on a 2 MiB payload: **`main`'s frame
4,194,624 bytes**. And then the measurement that settles what kind of thing this
is: one recursion run twice, same depth, same bytes, differing only in whether
the value sits in a case:

```
as a record:          15          exit 0
as a variant case:    panic: stack exhausted      exit 134
```

A correct program died because its large value was in a variant instead of a
record, and nothing in the language says those are different. So the landing
added the two writers R1 had always implied, and after them clang reports no
frame over 64 KiB in that program at all.

Doing it took `emit/large_value.hero` past §11's 300 lines, so it is two
modules now, on a seam that names a concern rather than a line count:
`emit/large_size.hero` answers **how big a type is and which are large**,
`emit/large_value.hero` answers **how a large value is spelled in C**. Neither
knows the other's trade, and the dependency runs one way.

**And the runtime's own two rules, told at the same gate.** `parts/frame.c`
called `free` where design.md Part 7.13 puts every such call in `parts/alloc.c`;
`hero_free_raw` now joins `hero_malloc_raw` there, the pair neither leak counter
weighs, which is what the room needs and what the two counted pairs would get
wrong. `hero_frame_told` is spelled `_Atomic int`. And the runtime's file floor
went 29 to 37 on purpose: eight parts had been added since 2026-10-07 and a
sweep could have missed them.

**What the landing had to read rather than assume.** The runtime moved, so every
program's provenance line moved with it and every emission wanted blessing; the
question was which of them changed for a reason other than that line. Read file
by file at the first bless: **26**, exactly the number the compiler-engineer had
predicted, and inside them **290** changed lines of which **212** are `#line`
directives renumbered and 78 the route's own forms.

**What it costs, said plainly.** A binding the author wrote, if its type lives
in the room, is read in the debugger through the name the room gives it:
`hero_big_h5_holder->vfs.f_bsize` where it was `h5_holder.vfs.f_bsize`
(`tests/golden/lldb/formatter-steps-aside.expected`). The pointer is left
visible on purpose, unlike every temporary the lowering invents, because
otherwise the value could not be seen at all.

**The gate.** Gate A on the seed of M-compatibility-promise, not refreshed:
clang built stage 1, stage 1 built stage 2, stage 2 built stage 3, and the two
emitted identical bytes, **29,043,650**, stage 2 keyed `200a93d749fe1443`.
Gate B, the full net on stage 2, **35 suites, 10,644 passed and 0 failed**, no
floor outgrown and its 16 skips this Mac's own; the compiler's own tests
**1,793** and the net's own **505**; the site's build green over 192 pages.
The emissions were blessed here, 710 files moved and 9 new, and the twelve
`tests/golden/emit/` goldens written by the form's own command, `build
--emit-c`, a second register of the same emissions that `UPDATE_EMISSION=1`
does not touch: **24 changed lines and every one of them the provenance
line**, which is R6 read a second way. On Linux arm64 the whole corpus read
2,203 and 2 before its bless and **2,205 and 0** after, the two failures being
the cases this Mac skips, and a `cmp` of every blessed file there against this
Mac's named those same two and no other, each differing by its provenance line
alone.

**What the route does not close**, filed apart: defect 2286, a chain of group
records nested by copy and a `partial` group record returned by value; defect
2287, two threads exhausting their stacks at once writing into each other's
message; defect 2288, the compiler's own work growing toward the square of a
construction's fields; and defect 2284, a bound `partial` header naming many
members at depth. Decision 2285 carries the sitting for the author to ratify or
turn, and an open `panel NNN` ratification is the one decision a tag may stand
over.

## The tag, placed 2026-11-01

`82a9dc6306bed5723c1e21d02c8f6e1086a174b6` was pushed with the author's standing
yes, given as they left, one commit travelling and that one publishing the site;
then defect 2278 was closed with the route's account. **The list read from GitHub
at the tag**: no `blocking` and no `systemic` defect open, four `adjacent` ones,
2284, 2286, 2287 and 2288, every one filed on 2026-11-01 beside this route and so
standing at its first tag; and one decision open, 2285, panel 233's ratification,
which `records/tagged` lets a tag stand over because the author reads every
sitting and a route already landed cannot wait on that reading. `records`
28 and 0 and `unseen` 3 and 0 over this tree.

So the row that said *done, the tag waiting* on 2026-11-01 says *done* with its
tag, and the tags go 74 to 75. What M-latency-ceiling leaves behind is not the
route but the instrument: the ledger's three rows, decision 2282's level over
the newest of them, and the comparison of alternated pairs that a close owes the
previous tag's compiler. Defect 2278's closing is the first change that
instrument judged which moved the whole emission corpus.
