# A value block that leaves on every path: the question goes to panel 185

2026-10-01, evening, `/decide` on the author's answer. The item below was
verified at `b39ae3d5`, after lane flow's batch had landed, before its verdict
was written: `a55`, `a69` and `a73` are still refused with `no_value`, *every
branch jumps, so there is nothing to bind*, at the inner statement.

**The author's words**: *1a 2a 3b 4a 5a 6a*, the evening's answer to six
recommendations the coordinator put to them at 22:06 with their reasons and
measurements, each item verified against the trunk at `b39ae3d5` first.
**Recorded as a reading**, CLAUDE.md § 4's default; not `by delegation`. The
other answers are in the decision log entry of the same evening.

- [x] **lane flow** | a value block whose last statement is an `if` or a `match` that leaves on every path is refused at that statement: accept it, since the block leaves, or keep the refusal | design.md §4.7 (`:1243-1255`, panel 017 R1) · `scratchpad/p185/inline/a55-value-arm-block-inner-returns.hero`, `a69-value-arm-block-if-returns.hero`, `a73-value-arm-block-ends-match-stmt-then-nothing.hero` · **sent to panel 185, 2026-10-01**

    Until it is answered, the compiler goes on as today: `x = match k`
    with an arm whose block ends in a `match` or an `if` whose every branch
    `return`s is refused with `no_value`, *every branch jumps, so there is
    nothing to bind*, at the inner statement (the three programs above,
    checked by the coordinator at 12:35 on 2026-10-01 on `e252fda4`),
    though the arm itself leaves and design.md §4.7 lets a value arm leave
    (`.eof => break`). Panel 017 R1 made an all-leaving `match` legal only
    in statement position; whether a block's last line is one, when the
    block leaves, is the question. Found by lane flow's first pass.

    **Recommendation: a sitting, with defects 143 and 147, once this
    round's gates are through.** The reason: the program's meaning is not
    in doubt (every path of the arm returns, measured by the three
    programs), and the rule that refuses it was written for a `match` used
    as a value, which this one is not; conservative is today's refusal.

    **Verdict, 2026-10-01: a sitting, meant as *2a*.** The author convenes
    panel 185 once the round's gates are through, with four questions: defect
    143 (a macro-only C name), defect 147 (spec § 8's `Inline` production),
    this one, and the premise of design.md §4.15's certain `-` in a pattern,
    which lane 135c measured false on bulleted arms. Until the sitting rules,
    the refusal stands, and the sitting's own item will queue its verdict.
