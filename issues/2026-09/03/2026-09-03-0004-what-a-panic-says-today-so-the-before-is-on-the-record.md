---
kind: task
area: runtime
milestone: M-panic-location
filed: 2026-09-03
commit: none
github: none
---

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
