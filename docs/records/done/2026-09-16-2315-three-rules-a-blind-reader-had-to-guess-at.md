- [x] **M-check-completeness** | three rules a blind reader guessed at, and one of the guesses compiles: `sort`'s direction, `xs[i] @ v`, and whether `main` may be fallible | closed 2026-09-16 by panel 159, whose three sentences landed with two compiler repairs the sitting itself produced

    **CLOSED 2026-09-16.** All three are in the document:
    `sort` (ascending: a number by value, a `str` by bytes, `false` before
    `true`; never a type parameter) in § 11; `@` declares a mutable cell and
    re-binds it, **or a field or element inside one** in § 5; and `function
    main()`, **which takes nothing and produces nothing**, in § 1. **+15
    vendored and +24 real**, paid by a −14 removal in § 3 and CLAUDE.md §12.

    **The item's own framing was wrong in two places and the sitting measured
    both.** *"It is one word of the document"*: it is not — `print("a" < "b")`
    is exit 1, so the document defines no ordering relation on `str` while its
    only `sort` example sorts `str`, and the complete sentence cost +11. And
    *"xs[i] @ v is given nowhere"* understated it: the general rule of `@`
    was given nowhere, which is why the whole rule cost **+9** against the
    partial one's +10.

    **And the premise that none of the three was a compiler defect was false
    twice**: defects 052 and 053, both opened and closed the same day, both in
    `docs/records/done/`.

    **Origin:** panel 126's ergonomist seat, 2026-09-11, out of three tasks
    written twice each. **`sort`'s direction is the one that matters**: neither
    version of the specification says ascending, both say only *walks them in
    order*, and a tie-break written on the wrong assumption compiles and prints
    a silently different answer. That is the single silent-error risk the seat
    found in six programs, and it is one word of the document. **`xs[i] @ v` is
    given nowhere** while `m[k] @ v` is given, so every sort the seat wrote
    carried a map of taken keys instead of swapping; measure first whether the
    compiler accepts it, because the seat could not. **Whether `main` may be
    `-> ()?`** decides whether `?` is usable in the one function every program
    has, and the document says a file holds `function main()` and nothing more.
    Each is a spec sentence, so each is the panel's; this row is the home
    because that milestone already asks what `heroes check` accepts.

    **ALL THREE ARE MEASURED, 2026-09-16, and every one has an answer the
    document does not give.** The seat could not run them; its only input is the
    specification, which is the point of that seat and the reason these stayed
    open.

    | the question | the compiler, measured | the document |
    |---|---|---|
    | is `sort` ascending? | **yes** — `1,2,3`; `apple,fig,pear`; `false,true` | says only *walks them in order* |
    | is `xs[i] @ v` accepted? | **yes**, exit 0, prints `99` | gives `m[k] @ v` and nothing for an array |
    | may `main` be `-> ()?` | **no**: `error[main_returns]`, *"a program reports failure by what it prints, not by what it returns"* | says a file holds `function main()` and no more |

    **So none of the three is a compiler defect and all three are silences.**
    The sitting's question is therefore narrower than the item first framed it:
    not *what should the language do* but *what does the document owe*, with the
    behaviour already settled. `sort`'s direction is the one that matters, as
    the seat said — a tie-break written on the wrong assumption compiles and
    prints a silently different answer, and it costs one word.

