# 038 — the copy costs thirty-three nanoseconds, and the mark costs nothing

2026-09-20, M-declared-extents step 21. **The measurement the author made a
condition**, in their words: *if this choice makes every call into C cost many
more CPU cycles and slows programs down a lot, we measure it; otherwise we go
straight to the pessimistic assumption, the most robust one.*

## What is being priced, said before the number

The pessimistic default assumes a C function that takes a pointer **keeps** it.
A program then has two ways to hand C a string:

1. **Declare the function as one that does not keep.** The lend stays a lend,
   `hero_str_cstr` in the emitted C, which is a field read. **Zero cost by
   construction**: the machine code is the same as today's.
2. **Do not declare it, and pass a copy.** `hero_str_held` allocates and copies,
   the call reads, `hero_held_release` frees. **This is the case that costs, and
   it is the worst case**: it is what a program pays where its author marks
   nothing.

So the measurement is of case 2, and case 1 is proved by the emitted C rather than
timed.

## The instrument

Darwin arm64, this Mac, 2026-09-20. `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` at HEAD `62a7ed6a`. Two Heroes programs that differ
in three lines, both calling one C function **twenty million times**:

    static inline int64_t sum_bytes(const char *s) {
        int64_t t = 0;
        while (*s) t += (unsigned char)*s++;
        return t;
    }

It reads every byte of a **56-byte** string, so the copy cannot be optimised
away, and it keeps nothing.

The emitted call sites, so the two programs are known to be the two cases:

    lend:   t9 = hero_str_cstr(t8);           t10 = sum_bytes(hero_cstr_nonnull(t9));
    lease:  t8 = hero_str_held(t7);  …  t11 = sum_bytes(hero_cstr_nonnull(t10));  …  hero_held_release(&h3_c);

`/usr/bin/time -p`, each binary three times, **sequentially, with nothing else
running** (CLAUDE.md § Verification: while a clock runs the machine stays still).

## The numbers

| run | lend `real` / `user` | lease `real` / `user` |
|---|---|---|
| 1 | 1.37 / 0.79 | 1.81 / 1.45 |
| 2 | **0.80 / 0.80** | **1.47 / 1.47** |
| 3 | **0.80 / 0.80** | **1.48 / 1.47** |

**Run 1 of each is discarded**, and the reason is written down as the rule asks:
`real` far above `user` plus `sys` means the run was waiting — a cold binary
paging in — and a number taken while waiting is not a measurement.

| | total, 20,000,000 calls | per call |
|---|---|---|
| lend, the loop and the callee's 56-byte read included | 0.80 s | **40 ns** |
| lease, the same plus one `malloc`, one 56-byte copy and one `free` | 1.47 s | **73.5 ns** |
| **the copy alone** | 0.67 s | **33.5 ns** |

## What it means for a program somebody writes

**A program whose author marks nothing and copies at every call pays 33
nanoseconds per call into C.** A `sqlite3_step` takes thousands of those; the
whole `examples/ledger` finishes in **0.00 s of user time** and 0.42 s of wall
time today, its four lends running a handful of times each — the difference is
below what `/usr/bin/time` can see.

**A program whose author marks the functions that do not keep pays nothing**,
and that is every function the census could classify: 977 functions across
SQLite, raylib and curl, **zero** retaining (panel 170's ffi seat, clang's own
AST as the ruler).

For scale, and cited as that sitting's number rather than re-run here: panel
167 timed SQLite's own copy of the same bytes, `SQLITE_TRANSIENT`, at **15.1 ns**.
The library already charges for a copy and expects the caller to take it.

## The verdict the author asked for

**Not many more cycles, and zero where the mark is written.** The condition is
met, and the author's instruction takes effect: the pessimistic assumption, the
most robust one.

## Unrun, and named

Linux and Windows. `.claude/rules/platforms.md`: a platform fact run on one
platform is an inference about the other two. The allocator is what is being
timed, and allocators differ.
