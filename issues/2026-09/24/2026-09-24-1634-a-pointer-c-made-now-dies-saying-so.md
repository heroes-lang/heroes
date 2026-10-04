---
kind: defect
area: none
milestone: M-agreed-retention
filed: 2026-09-21
commit: 43c225d53f89cc98310d34830187c29c0661deeb
github: none
---

# The milestone's first item closed: a pointer C made and gave back twice now dies saying so

2026-09-24, M-agreed-retention step 4, by panel 175's route E (`8a18290a`,
merged `79b55be5`).

- [x] **M-agreed-retention** | a pointer C made, handed twice to a C function that frees it, is `check` 0 and dies at run time saying nothing, and nothing in § 13 says a pointer C made is C's to free once | **closed 2026-09-24**, M-agreed-retention step 4, by panel 175's route E (`8a18290a`, merged `79b55be5`) | panel 172's compiler-engineer and completeness critic, `spec § 13`

    **Origin:** panel 172, 2026-09-21. The compiler-engineer measured it as the
    shape beside the double give-away of a lease (`p10`: `p = make()`,
    `release(p: p)` twice, exit 0 at `check` under both compilers) and the
    critic as `b_out`, an `@out: cstr` C fills and frees and the program then
    frees. **Not this sitting's**, both said, and filed here so it is not
    discovered as new: the runtime's report adopted at 172 covers the LEASE
    class and is silent here, because no lease is live and the pointer is C's.
    Whether the document owes the sentence that a pointer C made is C's to free
    once, whether that is design.md §1.12's business or C's, and whether the
    runtime's handler can reach it, are unmeasured.

    **Measured 2026-09-23, step 1, on all four platforms**, the C side
    `noinline`, five runs at each of `-O0` and `-O2` on Darwin and five at
    `-O0` on the other three:

    | shape | `check` | Darwin arm64 | Linux arm64 and x86-64 | Windows x86-64 |
    |---|---|---|---|---|
    | `p10`: a `ptr` C made, handed twice to a call that frees it | 0 | 133, **zero bytes**, 10 of 10 | 134, glibc's `free(): double free detected in tcache 2` | `0xC0000374`, **zero bytes** |
    | `b_out`, panel 172's: the same with a `cstr` C filled | 0 | the same | the same | the same |
    | the same program over a **handle**, `acquires` and `consumes` | 0 | 134, the runtime names the double release, 396 bytes | 134, 399 bytes | the runtime names it, 401 bytes |

    So the silence is Darwin's and Windows's; on Linux the C library speaks and
    Heroes still does not. **The handle route already catches the identical
    program on every platform**, which makes this item a question about the
    `ptr` that could have been a handle, not about a missing mechanism. The shape
    beside it, a false `owned` on a cell C has already freed, prints a runtime
    sentence that is FALSE and is defect 076. And a probe of this item must keep
    the C side out of the optimiser's sight: a `static inline` malloc and two
    frees are deleted whole at `-O2` and the program exits 0 printing its last
    line, which is what `heroes run`'s default level did to the first probe.

## What closed it

A `ptr` C made and gave back twice died with zero bytes on Darwin (133, measured
on the runtime before the merge, three of three) and on Windows. It now says
*the process is dying of <signal> …, and this runtime did not raise it* on
Darwin and both Linux legs, and *the process is dying of exception 0xC0000374,
the heap manager's report of a corrupted heap, and this runtime did not raise
it* on Windows; under `--sanitize` the report is ASan's own `double-free` with
the runtime's line silent, on all four legs. The golden is
`tests/golden/run/fixedbugs-c-gives-a-pointer-back-twice-and-the-runtime-says-so.hero`.

**What the item also asked**, whether the document owes a sentence that a pointer
C made is C's to free once, is answered by § 13's `tag void` sentence, landed at
`a747e5a2`: *a `void *` C hands out for the program to give back is declared as
one*, and a handle so declared is given back through the live set, which refuses
the second release before C runs.
