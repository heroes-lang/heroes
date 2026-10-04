---
kind: defect
area: runtime
milestone: none
filed: 2026-09-23
commit: 01bd4acd1986aafe53a1e1a5556f040aa472bda6
github: none
---

# Defect 086 closed: an empty set holds nothing, so a give-back it never saw is a stray there too

2026-09-24, M-agreed-retention step 5, `e2e13f04`. Found by panel 176's
compiler-engineer (its § 9), reproduced by the coordinator before filing.

- [x] **086 — a handle given back twice before the program has acquired any dies at 133 with nothing on stderr** | while the live set is still empty, `hero_handle_consumed` counts the stray and returns, so the double release reaches C, where after one acquisition the same program is stopped before C with the runtime's line | **closed 2026-09-24**, M-agreed-retention step 5 (`e2e13f04`) | `runtime/parts/alloc.c:428`

    **Origin:** panel 176's compiler-engineer, 2026-09-23 (its § 9); reproduced
    by the coordinator the same day before filing.

    **The reproducer.** A producer marked `borrows`, so nothing is ever
    acquired, and its handle given to a consuming `g_close` twice:
    `a = g_open()`, `g_close(x: a)`, `g_close(x: a)`. **133, zero bytes, three
    of three**, Darwin arm64. The same double release after one acquisition has
    allocated the set (`cap1.hero`): **134, 396 bytes, three of three**, before C.

    **Why it is a defect.** Defect 071 moved the stray report before the C call
    so that a double release is stopped before it happens; that promise now
    depends on the program's history. The seat measured the one-line repair —
    report instead of return at `alloc.c:432` — at 134 and 396 bytes for both.

## The repair

`runtime/parts/alloc.c`, `hero_handle_consumed`: the branch for a set that has
never been allocated counted the stray and returned, which was right before
defect 071 moved the stray report to the call and wrong after it — a program
whose every producer is `borrows`, so that nothing was ever acquired, reached C
with its double release (133, zero bytes) while the same program after one
acquisition was stopped before C with the line. The branch now reports like a
full set does. One line changed, one comment added.

## The measurements

The golden `tests/golden/run/fixedbugs-a-double-release-before-any-acquisition-is-stopped-before-c.hero`
prints `opened` and aborts at the first give-back with *1 C handle(s) given
back that were never taken*, before C:

| leg | plain | `--sanitize` |
|---|---|---|
| Darwin arm64 | 134, three of three | 134, three of three, the runtime's line and not ASan's |
| Linux arm64 (docker) | run suite 140/0 | in the suite |
| Linux x86-64 (docker, Rosetta) | run suite 140/0 | in the suite |
| Windows x86-64 (the box) | 127 (abort), three of three | 127, three of three |

On the runtime before the repair (`13fcd61d`, Darwin): exit 133 with zero
bytes on stderr, three of three, from the binary run directly.

Darwin gate after the edit: run 144/0, corpus 55/0, lines 145/0, runtime 8/0,
determinism 174/0, canonical 2/0, emission 500/0, the net's own tests 165/165,
the compiler's 675/675 — **and `warnings` 204 passed, 1 failed**, which
`e2e13f04`'s body reports as 204/0. That sentence is false, corrected here on
the day it was written: the golden's header spelled its two C helpers `static`
and the tag probe's unit, which includes the header and calls nothing, warned
`unused function` twice. The header now spells them `static inline`, as every
run golden with a `tag` record does, and the suite is rerun in the commit that
carries this record.

**Why the golden aborts at the FIRST give-back and not the second.** The
producer is `borrows`, so the set never held the handle: the first `g_close` is
already a give-back of something never taken, which is the same line
`abort-handle-borrows-that-gives-away` pins for a mark that lies. The defect was
never which call is refused; it was that with an empty set none was.
