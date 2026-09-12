# M-panic-location — a panic names its line


**Scheduled by author instruction 2026-09-03**, the cheapest half of what the
author called *"a strong runtime"*, and its warrant is **§1.12**: a program that
stops must say where.

**Measured 2026-09-03.** `hero_panic` flushes stdout, prints `panic: <msg>` and
calls `abort()` (`runtime/parts/panic.c:21-25`); an out-of-range index, an
overflow, a `.must()` on an error and a slice that splits a character all funnel
through it, and none names a `.hero` file, a line or a function. The one abort
that does is the stack guard: `runtime/parts/stack.c:202-213` walks back with
`dladdr` to the first Heroes frame on POSIX, and `stack.c:292` says Windows names
the failure and not the function until dbghelp and a PDB are measured on the box.
The emitted C carries `#line` (CLAUDE.md §7), so `__FILE__` and `__LINE__` at
every runtime call that can abort already resolve to the `.hero` position — the
information is in the binary and the runtime is not told.

**What it delivers.** Every panic names the `.hero` file, the line and the Heroes
function, on the three platforms, measured on each before its commit (CLAUDE.md
§ Commands); `HERO_RUNTIME_ABI` +1 with the two-phase edit (`seed/README.md`); a
`fixedbugs` case per abort class (CLAUDE.md §9). **Soundness lane**: no surface,
no diagnostic class, no spec token. What it must not do is slow a program's happy
path or the compiler — the location is passed, never computed, and the corpus
leg's time before and after is the measurement.

**Why here, and why it may move.** It touches the runtime, which
M-isolated-threads holds until it closes; nothing else depends on it, so it may
be taken the day the threads land. Its witnesses are the corpus under
`--sanitize` and Part 11's metric 4, whose turns-to-green a panic that names its
line shortens.

*******************************************************************************
**OPEN: 2**

- [ ] **M-panic-location** | what a panic says today, so the "before" is on the record | `runtime/parts/panic.c:21-25` · `runtime/parts/stack.c:202-213`, `:292`

    **Origin:** measured 2026-09-03, scheduled `DESIGN-LOG.md:539`.

    **`hero_panic` flushes stdout, prints `panic: <msg>` and calls `abort()`,
    and no abort in the language names a `.hero` file, line or function**
    (`runtime/parts/panic.c:21-25`; the out-of-range index, overflow, `.must()`
    and the character-splitting slice all funnel through it). The one exception
    is the stack guard, which walks back with `dladdr` to the first Heroes frame
    on POSIX (`runtime/parts/stack.c:202-213`) and on Windows names the failure
    and not the function until dbghelp is measured (`stack.c:292`). The
    generated C carries `#line` (CLAUDE.md §7), so `__FILE__`/`__LINE__` at each
    aborting runtime call already resolve to the `.hero` position.

    Owed: the location passed to every abort, `HERO_RUNTIME_ABI` +1 with the
    two-phase edit (`seed/README.md`), one `fixedbugs` case per abort class, the
    three platforms before the commit, and the corpus leg's time before and
    after — the location is passed, never computed.

    **Where to look also:** `seed/README.md` · `CLAUDE.md` §7, §9.
    **Why it matters:** a program that stops without saying where is §1.12's
    goal met halfway.

    **Re-verified 2026-09-10: STILL OPEN, and the behaviour is identical.**
    `runtime/parts/panic.c:21-25` is still `fflush(stdout)`, `fprintf(stderr,
    "panic: %s\n", msg)`, `abort()`, and `hero_panic` still takes only a message, so
    nothing passes a location; `HERO_RUNTIME_ABI` is **22**. **Two pointers moved
    inside one file**: the `dladdr` walk is `hero_stack_blame` at
    `runtime/parts/stack.c:237-249` (`:202-213` is now `hero_stack_regs`), and the
    Windows note is at `:426-433`, not `:292`.

- [ ] **M-panic-location** | the Windows quoting round trip has no test, and the comment claimed one | `runtime/parts/run.c` § quoted · `tests/harness/suite_records.hero` § citations

    **Origin:** measured 2026-09-06, found by the citation check the same day it
    learned to read the compiler's own comments — a claimed test is a citation
    like any other, and this is the class that check exists for, one level up
    from a path that merely moved. Its home since 2026-09-07: M-declared-freer
    closed without touching `runtime/parts/run.c`, which was always the item's
    real condition rather than that milestone's name. M-panic-location is the
    next milestone whose work IS the runtime — every abort gains a location,
    `HERO_RUNTIME_ABI` moves with the two-phase edit, and its own scheduling
    item names the Windows half by file, `runtime/parts/stack.c:292`, where the
    platform names the failure and not the function. A test about Windows
    quoting rides a milestone that is already on the Windows box; it rides a
    step rather than convening anything.

    `runtime/parts/run.c`'s `quoted` builds the Windows command line — a
    backslash before a quote is doubled, `C:\dir\` at the end of a quoted word
    needs `C:\dir\\` — and it REPLACED a function that had a test,
    `cli_shell.hero`'s `sq`. Its comment claimed a test of its own until
    2026-09-06 — `tests/golden/run/win-quote-round-trip.hero` (2026-09-06),
    *"asserts the round trip on the words that break naive implementations"*.
    Measured 2026-09-06: `git log --all --diff-filter=A --` over
    `tests/golden/run/win-quote-round-trip.hero` (2026-09-06) finds **zero**
    commits, and `ls tests/golden/run/ | grep -ci quote` is **0**. The
    file was never written; the comment was corrected to say what is true, which
    is that the test is owed.

    **Where to look also:** the Windows box,
    `docs/environment/windows/WINDOWS-MACHINE.md`.
    **Why it matters:** a replacement that loses its predecessor's test is a
    regression nobody can see, and the comment that says otherwise is what stops
    anybody looking.

    **Re-verified 2026-09-10: STILL OPEN.** Its own command still answers
    zero: `ls tests/golden/run/ | grep -ci quote` is **0**, and `grep -rn win_quote
    tests/` finds nothing. The comment half IS repaired — `runtime/parts/run.c:145-153`
    now says in the file itself that the function *"HAS NONE OF ITS OWN"* test and
    that the round trip is owed to this list, and adds a *"NOT VERIFIED on a real
    Windows CRT"* line; `hero_run_win_quote` is at `:154`. The test is still
    owed.

*******************************************************************************
