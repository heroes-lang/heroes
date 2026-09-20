# The property is not the base, it is the heap

2026-09-20. M-declared-extents step 10, panel 168, the soundness lane.

## The decision

| | |
|---|---|
| date | 2026-09-20 |
| decision | **the lease allocation keeps the LEADING header that ships**; panel 167 clause 1's trailing-header half is struck on the compiler-engineer's veto, clause 2 is **suspended** on three measurements, and **route A is measured to close zero of two defects**, so M-declared-extents cannot close on it |
| reason | the give-away case needs the library's own **allocator**, not the allocation base: with `sqlite3_config(SQLITE_CONFIG_MALLOC)` replaced the trailing header gives `134 134 134 133 133`. And at 64 KiB and 1 MiB the magic word survives C's free, so the release frees a block it no longer owns — the corruption class §1.12 forbids, created by the layout adopted to prevent an abort |
| design.md § | §1.12, §4.19:2332-2338, §1.1/§1.7 |
| panel | 168, provisional |

## The sitting answered what it was asked and the critic found the larger thing

Both seats approved the proposal. Neither measured whether route A does the
thing the milestone needs done, because the coordinator's brief scoped it out in
one sentence and both seats accepted the scope. The completeness critic did
measure it, on the half of the language that already shipped.

**The `cstr` side IS the world route A plus clause 2 would create for `ptr`**: a
lease that copies, both type rules, the position rule, and defect 067's repair.
If a copy with an owed release closed this class, the class would be closed
there. It is not, and the cheapest demonstration is this repository's own shipped
example with one line moved: `examples/gallery/13-lease.hero` with the print
after `end_lease` instead of before it is `check` 0, `run` 0 five of five, prints
**0 where 13 is honest**, and `--sanitize` says `heap-use-after-free`.

**A copy moves the moment the bytes die from *the frame returns* to *`end_lease`
runs*. Both are Heroes-side events, and C's retention is unrelated to either.**

## Two sentences in this repository could not both be true

Panel 167's spec-warden registered *"routes A, B and C each close zero of two
reproductions when landed alone"*, and defect 066's entry said *"panel 167 adopts
route A … this entry closes when that lands"*. The prediction was checkable the
day it was written and nothing read it. It is **scored CORRECT here** rather than
at the close, and 066's entry is corrected in place for the second time in one
day: from *field lend* to *lend* this morning, and now to **lend AND lease**.

## A seat withdrew its own prediction, on its own measurement

The ffi-pragmatist supplied panel 167's ground for the trailing header and
overturned it here, having asked the question that sitting did not: **is
`sqlite3_free` `free`?** The real header documents its own answer at
`:1879-1891`. That is recorded as the seat's act, not as anybody's correction of
it.

## What the sitting did not ask, and is carried rather than lost

Four routes nobody listed, the nearest of which is almost embarrassing: **the
language already ships a retention vocabulary.** `borrows` says a call keeps what
it is handed, and `acquires` carries a pointer-keyed live set that aborts — the
run-time instrument two sittings said did not exist. And **defect 068's
corrupting write is a Heroes statement**, caller-side, where panel 167's own
historian found static enforcement exists in every ecosystem it surveyed. It was
filed as unsolvable on a survey about the other layer.

## And three defects were filed, two of them new

066's class widened, **069** — a C function name passed as a callback is `check`
0, `build` 0, `run` 134 blaming the compiler, which is load-bearing because a
destructor callback is how every real library expresses retention — and **070** —
a lease handed to a C function that frees it dies with an empty stderr and an
exit code that is 133 nine times in ten.
