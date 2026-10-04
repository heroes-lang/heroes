- [x] **076 — a runtime panic blames a fabricated `str` for a heap a C function corrupted** | the magic check on a string block named ONE cause, and on a C double free that cause was false while the sentence read as certain | **closed 2026-09-23**, M-agreed-retention, repaired in a lane while panel 175 sat | `runtime/parts/str.c` · `tests/golden/run/fixedbugs-c-writes-over-a-strings-header.hero` · `tests/golden/run/fixedbugs-c-writes-over-a-leases-header.hero`

    **Origin:** 2026-09-23, M-agreed-retention step 1, measuring the milestone's
    first item at the shape beside panel 172's `b_out`: a result mark that says
    the program frees what C has already freed.

    ## The repair, and the shape that made it a class

    **The shape filed as UNRUN was run first, and it was the same defect.** The
    entry named `str.c`'s held-block check as the shape beside the string's,
    *the same claim of one cause*, with no probe written. A C function handed a
    lease that writes over the sixteen bytes before its pointer reaches that
    check on every run, and the line said *the checker admits `end_lease` only
    on a lease cell, so this is a compiler bug*, while the runtime's own comment
    above it said the check was *never the guard against a program*. So the
    class is two sites, both measured false, and both are repaired.

    **The witness was replaced by an instrument.** The filed reproducer, a
    false `owned` on a cell C had already freed, depends on the heap: 8 runs in
    10 on Darwin, 5 in 5 on Linux x86-64, glibc's own line on Linux arm64. A C
    writer over the eight bytes before a lent `.cstr()`, and over the sixteen
    before a lease, reaches each check on every run, at `-O0` and `-O2` and under
    AddressSanitizer, because the write stays inside the runtime's own block.

    **What landed.** Both lines now state what the runtime SAW, that the block
    has lost its mark, and name the causes as causes, worst first: C writing or
    freeing memory the program still holds, which a mark untrue of its function
    lets happen; for the string, a `HeroStr` built by hand in C; and a compiler
    bug. Panel 173 R1's rule is the standard, *no path prints a sentence measured
    false*. `hero_str_from_bytes` stays in the string's line only as the name of
    the right way in C, not as the advice.

    **Measured, the two new cases, three runs each:**

    | | Darwin arm64 | Linux arm64 | Linux x86-64 | Windows x86-64 |
    |---|---|---|---|---|
    | string header, plain | the new line, 134 | the new line, 134 | the new line, 134 | the new line |
    | string header, `--sanitize` | the new line | the new line | the new line | not built |
    | lease header, plain | the new line, 134 | the new line, 134 | the new line, 134 | the new line |
    | lease header, `--sanitize` | the new line | the new line | the new line | not built |

    Against the runtime at `64c92654` the same two binaries print the two old
    sentences, so each case goes red if its line goes back. The filed witness
    prints the new string line five runs of five on Darwin.

    **Gates, in the lane at `64c92654` plus this repair:** `runtime` 8 passed;
    `layout` 2; `canonical` 2; `lines` 139; `warnings` 199; `determinism` 168;
    `run` 138, 0 failed; the net's own tests 162; the compiler's own 675.
