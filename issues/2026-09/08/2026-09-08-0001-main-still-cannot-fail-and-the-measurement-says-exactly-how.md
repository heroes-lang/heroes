---
kind: feature
area: spec
milestone: M-publication-gate
filed: 2026-09-08
commit: none
github: none
---

- [ ] **M-publication-gate** | `main` still cannot fail, and the measurement says exactly how far `exit(code)` got | `docs/panel/030-the-build-order-revised.md:225` · `spec/heroes-spec.md:190-194` · `examples/`

    **Origin:** M-open-repository, 2026-09-08. The gate's own checklist names
    this as *"one defect that shows up on the second page of any tour"*, queued
    from panel 030, and says it must not be true on the day the examples go up.

    **Measured this session, on a rebuilt compiler**, three shapes rather than
    the one the gate remembered. A program that receives a `fail`, matches it
    and prints it exits **0** — panel 030's sentence, still exactly true. A
    program that calls the built-in `exit(1)` exits **1**, so the escape hatch
    the spec gained is real and works. An out-of-bounds index exits **134**, so
    the guards are unaffected. **11 of the 54 example programs call `exit(`**
    and none of the gallery's 12 do, which is the corpus half of the same
    question: the ones that can fail mostly remember, and nothing makes them.

    So the finding is narrower than *"main cannot fail"* and worse than
    *"solved"*: **the language has a way to report failure and no way to oblige
    it**, and the default for a program that handles its own error is to tell
    the shell it succeeded. Changing what `main` returns is a language change
    and owes a panel; that is why this is filed and not fixed here.

    **Where to look also:** `docs/ROADMAP.md` § M-publication-gate.
    **Why it matters:** a script that calls a Heroes program cannot tell whether
    it worked, which is the one thing an exit code is for.

    **Re-verified 2026-09-10: STILL OPEN, the finding intact, one exit code
    UNPINNED.** `main` still may not declare a result — `selfhost/check/decls.hero:79-81`
    raises `main_returns`, tested at `:218-220` — so *the language has a way to report
    failure and no way to oblige it* stands. `exit(code)` is asserted end to end:
    `tests/golden/run/exit-status.expected` demands `!exit: 3` and
    `tests/harness/expectation.hero:55-56`, `:84-88` make that a demand on the shell
    status. **The abort's 134 is pinned nowhere**: the abort goldens demand only the
    message and a non-zero code (`expectation.hero:89-96`), so *"an out-of-range index
    is 134"* is **UNSETTLED** as a number and settled as non-zero-with-a-message.
    Counts: **11 of 54** program directories call `exit(`, unchanged; the gallery is
    **14** files, not twelve, and still none of them calls it. One pointer moved:
    `exit(code: i64)` is `spec/heroes-spec.md:203`, not `spec:190-194`. **And nothing
    was decided elsewhere**: `docs/work/DECIDE.md` is empty and no sitting on what
    `main` returns has ever been queued.
