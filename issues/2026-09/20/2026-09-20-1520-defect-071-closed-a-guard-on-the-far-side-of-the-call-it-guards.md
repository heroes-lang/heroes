- [x] **071 — the `consumes` guard is emitted AFTER the call, so it can never prevent** | **closed 2026-09-20**, M-declared-extents step 13 | **The repair is an ORDER, in two halves, and neither alone is enough.** `consumes` is now emitted BEFORE the call and `acquires` still after, which is a fact rather than a preference: a consumed handle's value exists before the call, while an acquired result does not exist until the call returns and an acquiring `@` out-parameter's cell is not written until then. And `hero_handle_consumed` now ABORTS at the stray instead of counting it for exit, because a program that double-frees never reaches exit | `selfhost/emit/handle_traffic.hero` · `selfhost/emit/ops.hero` · `runtime/parts/alloc.c` · `tests/golden/run/fixedbugs-a-real-deallocator-given-the-same-handle-twice.hero` | 169

    **Origin:** panel 169's ffi-pragmatist, 2026-09-20, as the one further rule
    route R5 needs. Re-run by the coordinator before filing, and found worse than
    the seat described: the runtime never spoke at all.

    ## The measurements

    | | before | after |
    |---|---|---|
    | `heroes check` / `heroes build` | 0 / 0 | 0 / 0 |
    | `heroes run`, three times, against a **real** deallocator | **133 133 133**, **stderr empty** | **134 134 134**, with the runtime's three-sentence stray message |
    | the existing stub-deallocator golden | `7` then the message | **unchanged** |

    ## The comment that justified the wrong order, and why it lost

    `emit/ops.hero` said *"the counter rides the call it belongs to, on the lines
    after it, so the two are one instruction to read and cannot drift apart."*
    That is a readability reason for the emitted C. CLAUDE.md § Precedence puts
    robustness at rank 3 and leaves readability of generated C at no rank at all,
    and CL-012 names it among what robustness beats. **A guard on the far side of
    the call it guards can report and never prevent** — and here it could not even
    report.

    ## The runtime already held the reasoning that condemned its own placement

    `runtime/parts/alloc.c`, above the exit report: *"THE STRAY IS REPORTED
    FIRST, because it is the one that may already have corrupted memory."* True,
    and waiting for exit is how that reasoning loses to a program that never
    reaches exit. The message is now written **once**, in
    `hero_handle_report_stray`, raised at the moment of detection, and the exit
    gate keeps its own call as **defence in depth**: an emitter that stopped
    telling the set before the call would otherwise turn a corruption into
    silence.

    ## AND THE CASE THAT WOULD HAVE CAUGHT IT PREDICTED IT IN PROSE

    `tests/golden/run/abort-handle-given-back-twice.h`, written 2026-09-15, in
    its own words: *"`slot_close` deliberately does nothing, so the second
    release below does not corrupt anything on this machine and the case can
    assert the COUNT rather than a crash. **Against a real allocator the same
    program is a double free.**"* True, written down, and never followed by the
    case that runs it against one.

    That is `.claude/rules/diagnostics-and-goldens.md` § An instrument watches
    the world, in the one place the rule's own question would have caught it:
    *what would still pass if the thing under test were wrong?* The accounting
    would. The new case,
    `fixedbugs-a-real-deallocator-given-the-same-handle-twice`, differs from its
    ancestor in **one line of C** — `blk_close` calls `free` — so a reader can
    diff the two and see that the difference is C's and not Heroes'.

    ## The gates

    `run` **131**, `determinism` **160**, `lines` **132**, `warnings` **191**,
    `canonical` **2**, `layout` **2**, `order` **3**, `runtime` **8**, `corpus`
    **55**, `records` **24**, `check` **129**, `ir` **24**, `descriptors`
    **222**, `emission` **472**, the compiler's own **673**, the net's own
    **158** — all 0 failed. Three of those are suites nobody expected to move and
    they were run anyway (CL-054).

    **`emission` was re-blessed and the diff is quoted**, which that suite's own
    doc requires: **14 files, 46 insertions and 46 deletions**, every file's line
    count identical before and after, and every hunk the same two adjacent lines
    swapped — `hero_handle_consumed(tN);` moving above the call. A pure
    reordering, and the arithmetic is what says so.

    The seed was regenerated in the same commit and the fixpoint verified
    **byte-identical**.
