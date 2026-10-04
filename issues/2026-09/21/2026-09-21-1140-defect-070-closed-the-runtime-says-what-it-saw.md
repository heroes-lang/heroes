- [x] **070 — a lease handed to a C function that frees it dies with an empty stderr and an unstable exit code** | **closed 2026-09-21**, M-declared-extents step 28 | **The runtime names the live leases from the signal handler it already installs**, so the program that died saying nothing now says what was live and in which function; no word could reach the class, and panels 172 and 173 measured why | `runtime/parts/os.c` · `runtime/parts/panic.c` · `tests/golden/run/c-frees-a-lease-and-the-runtime-names-it.hero` | 172, 173

    **Origin:** panel 168's compiler-engineer, 2026-09-20, reproduced by the
    coordinator before filing.

    **Reproducer**, six lines, and the C side is one:

        extern "giveaway.h"                  # static inline void eat(const char *s)
            function eat(s: cstr)            # { free((void *)(uintptr_t)s); }

        function main()
            x = "payload"
            c: cstr @ x.lease()
            eat(s: c)
            end_lease(@c)

    `check` **exit 0**, `build` **exit 0**, ten runs: **133 133 133 133 133 133
    133 133 133 134**, and **stderr is empty every time**. Under `--sanitize`:
    `bad-free`, named at the program's own C line.

    **What is owed.** design.md §4.17 asks a diagnostic to carry everything needed
    to fix the program; this carries nothing, not even a stable exit code. The
    repair is not a header layout — panel 168 measured that both layouts abort and
    that the trailing one produces a message blaming the compiler. It is §4.19's
    third reserved case, *a buffer that C takes ownership of*, or a rule about the
    call. **Neither exists**, and panel 168's ffi seat measured that the give-away
    needs the library's own **allocator** rather than any pointer the Heroes
    runtime can hand out.

    **A FOURTH SHAPE, panel 170's compiler-engineer, 2026-09-20**: a plain
    `.cstr()` lend into a freeing callee, with no lease anywhere — `check` 0,
    exit **134**, stderr **0 bytes**. It is the same class and it is named here
    rather than given a number, because a repair is owed at the class.

    **AND THE EMPTY STDERR IS THE LANGUAGE'S OWN REPORT BEING PRE-EMPTED**, which
    the same seat measured against a control: the control prints
    `panic: 1 lease(s) never ended`, **111 bytes**, and this prints zero because
    C's `free` aborts before `main` returns. **The run-time instrument works and
    the corruption outruns it.**

    **What panel 170 settled about the repair.** No mark can carry it together
    with retention: **retention must ADMIT a lease and give-away must REFUSE
    one**, which is the compiler-engineer's veto ground. The route named for the
    next sitting is a MEANING for a word that already parses — `borrows` and
    `consumes` both parse on a `cstr` today and are thrown away by
    `check/marks.hero`'s handle-only sweep.

    **CORRECTED THREE WAYS BY PANEL 172, 2026-09-21, and the route named above
    was built and measured not to close it.** First, the title's *unstable exit
    code* is two platform paths and not randomness: 133 is SIGTRAP and 134 is
    SIGABRT, the critic measured on five bad-free shapes in plain C. Second, the
    *empty stderr* is not the language's report being pre-empted by an
    allocator message: on Darwin 25.6.0 the allocator writes **nothing** for any
    bad free, under every `Malloc*` knob, and no crash report is written either;

    ## The repair

    **No word could close it, and two sittings measured that rather than
    arguing it.** Panel 172's compiler-engineer BUILT the route the brief
    proposed — `consumes` given a meaning on a pointer parameter, 96 lines,
    every suite green — and measured that the reproducer above, whose callee
    carries no word at all, is `check` 0 before and after. Its completeness
    critic then found three more shapes, none of them in the four recorded
    here, each `check` 0 under both compilers: C invokes a callback with the
    leased pointer, C frees it on a LATER call that takes no pointer, an
    `@out: cstr` C fills. **`later_free()` has no parameter to mark**, so a
    vocabulary on declarations cannot reach the class at all, and panel 150's
    Part 6 row refusing a mark where no handle is reached stands untouched.

    **What closes it is the runtime.** `runtime/parts/stack.c` has installed a
    signal handler since M-thread-stacks and writes from inside it with
    `write(2)`; `runtime/parts/alloc.c` has kept `hero_live_held`, the live
    lease balance, since M-held-bytes. Panel 172's critic wired the two
    together in fifty lines; panel 173 judged the mechanism in the soundness
    lane and both seats vetoed its SENTENCE, which asserted that a C function
    had freed the bytes — **false on six of nine measured paths**, a C
    library's own `abort()` and its failed `assert` among them, with
    `siginfo_t` measured unable to tell any of them apart on either platform.
    The landed line states the two facts the runtime holds and names the C
    free as a condition the reader checks.

    ## The measurements

    | | before | after |
    |---|---|---|
    | the reproducer, ten runs | exit 133/134, **0 bytes** of stderr every time | 133/134, **272 bytes**: `panic: the process died with 1 lease(s) still live, in lease070.main` |
    | C frees it through a callback | `check` 0, 133, 0 bytes | 133, **273 bytes**, named |
    | C frees it on a later call with no pointer parameter | `check` 0, 133, 0 bytes | 133/134, **270 bytes**, named |
    | a pointer C made, freed twice by C — no lease live | 133, 0 bytes | **unchanged and silent**, correctly: the class boundary, and a golden says so |
    | a Heroes panic with a live lease | 134, 32 bytes | **32 bytes, one true line**: the `hero_abort()` funnel, fifteen sites |
    | a C library's own `abort()` with a live lease | 134, 0 bytes | 275 bytes, and the sentence is TRUE: it says what was live, not what freed |
    | a C library that installs its own SIGABRT handler from a constructor | exit 77, its handler runs | **exit 77, its handler runs** — the handler chains to the disposition it found, ten runs of ten on two platforms |
    | a lease nobody ends, C freeing nothing | 134, 111 bytes | **111 bytes, unchanged** |

    **The exit code was never unstable.** Panel 172's critic measured that 133
    is SIGTRAP and 134 is SIGABRT, two allocator paths on Darwin, and that on
    Linux the allocator raises SIGABRT alone. **And the stderr was never
    emptied by anything**: on Darwin 25.6.0 the allocator writes nothing for
    any bad free, under every `Malloc*` knob, so there was no message to
    recover — the title's premise was wrong twice and the entry is corrected
    above rather than rewritten.

    ## What the language still does not do, written down

    `spec § 13` now carries the limit and its falsifier: *No word says C frees
    what it is handed; one that frees a lease kills the process, naming the
    leases live. Bytes C owns come from its own allocator, with their
    disposer.* The falsifier CL-005 requires is panel 173's ffi seat's own
    question — *does the documentation name an ARGUMENT that disposes of this
    pointer?* — and the day one parameter can truthfully carry the word, that
    sentence is wrong.

    **And the direct spelling cannot be written at all**, which the ffi seat
    measured against real libraries: five attempts to hand a lease to `free`,
    to `sqlite3_free` and to a `char *` taker are five refusals,
    `type_mismatch` or `ffi_writable_parameter`. The defect reaches a real
    library only by the indirect route, which is the one no declaration sees.

    ## The gates

    `runtime` 8 passed 0 failed (three `SHARED_BY_DECISION` entries and the
    floor raised 39 to 49, which the suite grades in both directions); `run`
    136 passed 0 failed with four new cases, one of them the silent boundary;
    the net's own tests 161 passed, with three new ones for the expectation
    form the cases needed. The rest are in the landing commit's body.
