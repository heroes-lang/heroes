---
kind: defect
area: runtime
milestone: none
filed: 2026-09-24
commit: 5767900515f50b59ccfd27ab3ca7e919b55081cd
github: none
---


# Defect 090 closed: a read near zero says what it saw and names both things that put a null there

2026-09-24, M-agreed-retention step 8, in lane `5d2639c0`, merged `748c8f10`. Found
by panel 177's ffi-pragmatist over real OpenSSL (its *Found in what ships*,
item 1), reproduced by the coordinator on Darwin and both Linux legs before
filing.

- [x] **090 — a use-after-free inside C, reached through a stale copy of a handle, is reported as a null handle reaching C** | the stack guard reads any fault below its null window as *a handle or `ptr` holding `nullptr` reached C*, and a freed object whose field C reads as null faults there too | **closed 2026-09-24**, M-agreed-retention step 8 (`5d2639c0`, merged `748c8f10`) | `runtime/parts/stack.c:502`

    **Origin:** panel 177's ffi-pragmatist, 2026-09-24 (its *Found in what
    ships*, item 1); reproduced by the coordinator the same day before filing,
    from the seat's `ossl_alias_null.hero` copied out of its directory.

    **The reproducer**, against Homebrew's OpenSSL 3.6.4 on Darwin and Debian's
    3.5.7 on Linux: `bio = BIO_new(…)`, a copy `mine = bio`, then
    `SSL_set0_rbio(s: ssl, rbio: bio)` hands the BIO to the connection and
    `SSL_free(ssl)` frees both; the program then prints `mine == nullptr` and
    writes through `mine`. **Darwin arm64: 134, three of three**, after
    printing `mine is null: false`, with *panic: a null pointer was read
    through — a handle or `ptr` holding `nullptr` reached C where C
    dereferences it, at offset 0x210, called from osslaliasnull.main* (0x240 on
    one run). **Linux arm64 and x86-64: 139, zero bytes, three of three.**
    Under `--sanitize`: *SEGV on unknown address* inside `libcrypto` on Darwin
    (134) and Linux arm64 (1), never a use-after-free, because the library is
    not instrumented.

    **Why it is a defect.** The line states a cause, and the cause is false: the
    program had just printed that the handle was not null. Panel 173's rule for
    the lease line — *it says what it saw and not why* — is the rule this line
    breaks. The shape that produces the fault is design.md Part 8 wart 20's
    class, a copy made before the call; the defect is the message, which sends
    the reader to look for a `nullptr` that is not there. **Unrun:** Windows.

## The repair

`runtime/parts/stack.c`'s report for a fault below the null window named one
cause — *a handle or `ptr` holding `nullptr` reached C* — and it was false under
a program that had just printed that its handle was not null: a C object given
back, cleansed to zero as OpenSSL cleanses, and read again through a copy of its
handle the program kept faults at the same small offset a null handle would,
since its fields now read as zero. Panel 173's rule for the lease line — it
says what it saw and not why — binds this line too. It now says what was seen,
the offset and the frame, and names the two things that put a null where C read
it, one string for the POSIX and Windows arms so they cannot drift
(`HERO_NULL_READ_TAIL`, beside `HERO_NULL_WINDOW`).

The golden is a header of its own rather than OpenSSL, so it fires on every
platform: `tests/golden/run/fixedbugs-a-freed-object-read-through-a-stale-copy-is-not-a-null-handle`,
which prints `null: false` and then dies with the new line; under `--sanitize`
the runtime is silent and ASan's `heap-use-after-free` is pinned instead. The
`nullread` fixture and `suite_surface.hero`'s row for defect 045 keep their two
assertions — *a null pointer was read through* and *at offset 0x0* — because
both are still what the runtime saw.

## The measurements

| leg | plain | `--sanitize` | `nullread` fixture |
|---|---|---|---|
| Darwin arm64 | 134, three of three, the new line | 134, ASan's report, zero runtime lines | 134, `7`, the line with `at offset 0x0` |
| Linux arm64 (docker) | 134, three of three, the new line | 1, ASan's `SEGV on unknown address`, zero runtime lines | 134, `7`, the line |
| Linux x86-64 (docker, Rosetta) | 134, three of three | 1, ASan's report, zero runtime lines | 134, `7`, the line |
| Windows x86-64 (the box) | 127 (abort), three of three, the new line | 1, ASan's `access-violation on unknown address`, zero runtime lines | 127, `7`, the line with its tail |

**The golden's first shape was over `calloc` and `free`, and it was not a
witness**: on Linux x86-64, Linux arm64 and Windows the freed block was handed
out again before the stale read, and the case printed a heap address at exit 0,
three of three each; only Darwin's allocator left the cleansed object zero long
enough to fault. The header says so, and the case is a pool that is cleansed
and never freed, so the null is read on every platform; under `--sanitize` the
fault is ASan's to report, pinned with the three words the platforms share.

Lane gate, Darwin: run 145/0, lines 146/0, runtime 8/0, determinism 175/0,
warnings 206/0, canonical 2/0, emission 502/0, surface 111/0, the net's own
tests 167/167, the compiler's 676/676. On the merged trunk, compiler rebuilt
with the merged runtime: run 146/0, lines 147/0, runtime 8/0, warnings 207/0,
emission 506/0, surface 111/0, determinism 176/0, canonical 2/0, records 24/0.
