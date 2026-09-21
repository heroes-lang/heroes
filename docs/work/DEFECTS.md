# DEFECTS — the compiler defects that are still open

Every item is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. **Only open defects live
here**: a repaired one is ticked, gains a *The repair* section with the
measurements that prove it, and moves to `docs/records/done/`. A repair is owed
at the class and not at the witness, with a `tests/golden/fixedbugs/` case per
shape.

**The shape** is `.claude/rules/records.md` § The lists, and § A live list is a
preamble, a count and its items is why this preamble is fifteen lines. **The
next number is READ, never remembered** — `records/numbering` takes one above
the highest issued across this file and `docs/records/done/`. Who issued which
number since 2026-09-08, and why 014 exists twice, is
`docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **070 — a lease handed to a C function that frees it dies with an empty stderr and an unstable exit code** | `check` 0, `build` 0, and the program aborts saying nothing at all, 133 nine times and 134 once in ten runs | `spec § 13`'s lease sentences, design.md §4.17

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
    what is pre-empted is only the exit sweep's own `panic: 1 lease(s) never
    ended`. Third, the class is **seven shapes and not four**: a callback C
    invokes with the leased pointer, a LATER call that frees what an earlier one
    stashed, and an `@out: cstr` C fills and frees are all `check` 0 and
    133/0, and the second has **no pointer parameter at all**, so no word
    written on a parameter can ever reach it. The compiler-engineer built the
    `consumes` route and measured that the reproducer above, whose callee
    carries no word, is `check` 0 before and after it. **What closes this
    entry** is the sitting's resolution: the runtime names the live lease from
    the signal handler it already installs, 0 to 250 bytes on the reproducer,
    landing after panel 173 judges the fifty lines in the soundness lane.
    `docs/panel/172-the-report-comes-from-the-crash-and-not-from-the-declaration.md`.

*******************************************************************************
