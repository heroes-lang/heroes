- [x] **M-check-completeness** | the sortable obligation: `heroes check` accepts `first([P(x: 1)])` at exit 0 and `heroes build` refuses it | **CLOSED 2026-09-16**, found closed at the milestone's opening and pinned the same day | `tests/golden/check/sort-through-a-type-parameter.hero` · `docs/panel/084` R1 · `selfhost/check/ordering.hero:45`

    **The measurement, run before the sentence was written.** On a compiler built
    from the seed, `first([Point(x: 2, y: 1), Point(x: 1, y: 2)])` through a
    `function first<A>(xs: [A])` whose body calls `sort(xs)` is **`check` 1 and
    `build` 1**. The milestone's opening sentence said `check` 0 and `build` 1,
    and that was true when panel 082 R3 wrote it on 2026-08-16. Panel 084 R1
    closed it later that same day by taking `generic` OFF `is_refusable`'s exempt
    list, and no living document noticed.

    **Why 084's shape worked here and nowhere else**, which is the part worth
    keeping: `sort`'s domain is twelve types — the eight integer widths, the two
    floats, `str` and `bool` — so refusing it on `[A]` deletes no working program,
    because nothing a call could choose both sorts and is still a type parameter
    in the body. The two rules beside it have no such restriction. A map keyed on
    `K` is legal at `str` and every integer; `==` is legal on almost everything.
    A body refusal there would delete programs that run today, which is why they
    are still open and why they need the call site.

    **What was found in the closing rather than in the gap.** The rule that closed
    this had **no golden case**. Its only witness was a unit test in
    `selfhost/check/ordering.hero:186-188`, which interns a `.generic` type id and
    asserts `is_refusable` on it — a fact about a table entry, not about anything
    anybody would write. So a compiler that passed every suite could have stopped
    refusing the program while the assertion stayed green, and the check/build gap
    would have reopened in silence. The shape is `.claude/rules/module-shape.md`'s
    one level up: **a unit test can only watch what it can name, and this one
    named a table row where the rule is about a program.**

    `tests/golden/check/sort-through-a-type-parameter.hero` is that case. It pins
    both spellings, `sort(xs)` and `xs.sort()`, because UFCS makes them one rule
    and a case reaching one of them would be half a case; it carries a control
    generic that does not sort, so a widening shows up as a new diagnostic; and it
    instantiates the refused generic at `i64`, which sorts and is refused anyway,
    so the body-decides property is written as a program rather than as a
    sentence. `check` moved 120 to 121 and `annotations` 157 to 158, with
    `canonical`, `fixes`, `records`, the compiler's own 653 and the net's own 154
    all green.

    **What this does NOT close**, stated so a later session does not read the tick
    as more than it is: the llm-ergonomist's veto at panel 082 stands untouched.
    The line `sort(xs)` is still undecidable from the line plus its signature, and
    lifting that needs constraints on generics, which is the author's trade.
