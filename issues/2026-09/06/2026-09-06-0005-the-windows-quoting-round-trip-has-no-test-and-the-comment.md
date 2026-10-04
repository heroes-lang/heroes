---
kind: task
area: runtime
milestone: M-panic-location
filed: 2026-09-06
commit: none
github: none
---

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
    `docs/ref/environment/windows/WINDOWS-MACHINE.md`.
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
