- [x] **M-interpolated-strings** | `.must()` says "abort" and the spec never says what an abort does — panel 121 R6 lands one definition of *abort* WITH the interpolation clause, at +12, covering all eight sites | `spec:156` · `spec:197-198` · `selfhost/value_errors.hero:145` · `docs/measurements/010-spec-budget-ledger.md`

    **Origin:** the llm-ergonomist, panel 118, 2026-09-08, reported as an
    appetite rather than a request, from the seat that reads the spec and nothing
    else. Filed in `DECIDE.md` and settled by `/decide` 2026-09-08: buy it, and
    home it here because this sitting's agenda already carries a *the spec is
    silent where the runtime is specific* question — the printed-float item above
    — and both are paid from one budget. Naming a sitting whose AGENDA holds the
    question is what this file asks for, rather than *the next sitting that
    touches the spec*.

    **Measured 2026-09-08.** The spec writes `abort` **8 times on 7 lines**
    (`:71`, `:156`, `:162` twice, `:169`, `:171`, `:186`, `:197`) and defines it
    **nowhere**. It says *ends the program* exactly **once** in the whole
    document, wrapped across `:197-198`, and that once is `exit(code: i64)`'s
    parenthetical — so the phrase a reader needs is already in the spec, spent on
    the one operation nobody could misread. `.must()` gets three words, *extract
    or abort*, with no consequence attached, and that is what made it the
    ergonomist's attractive repair in the one situation where it was
    catastrophic.

    **The compiler says it and the spec does not.** `.must()` ends the program
    appears at three message sites in `selfhost/value_errors.hero` (`:145`,
    `:165`, `:187`, plus `:349`, the test that pins the third), and
    `selfhost/ir/verify.hero:223` opens with *an abort ends the program*.
    `spec:3` says *"This document is the whole language"*, so today the
    diagnostic is better documentation than the specification.

    **THE OPTION SET IS WIDER THAN THE BALLOT THIS ARRIVED WITH (CL-057).** It
    was filed as *should `.must()`'s abort say it ends the program*, one clause at
    one site. The measurement says there are two routes and the sitting prices
    both: **(a)** the local clause, `.must()` alone — cheapest, and it leaves
    seven aborts undefined; **(b)** one definition of *abort*, that it ends the
    program, which covers all eight sites and is the only route that also makes
    `:162`'s *recursion too deep* and `:71`'s *overflow aborts at every width*
    legible. Nobody has priced (b). It is dearer than (a) and may be cheaper than
    (a) written eight times, and the spec has **40** free tokens (`./heroes
    measure spec/heroes-spec.md`: 3995 against a green ceiling of 4035), so the
    price is what decides and both are measured before the sitting, not during.

    **Why it matters:** a language whose thesis is that every plausible mistake
    is a compile error tells its reader eight times that something *aborts*, and
    never once what that costs them.

    **LANDED 2026-09-09 at M-interpolated-strings step 1**, route (b), with the
    interpolation clause: one definition at `spec:71`, *an abort ends the
    program, saying why*, at **+14** real tokens against R6's +12, covering all
    eight sites where the document said the word and defined it nowhere. Route
    (a), the local clause on `.must()` alone, would have left seven undefined.
