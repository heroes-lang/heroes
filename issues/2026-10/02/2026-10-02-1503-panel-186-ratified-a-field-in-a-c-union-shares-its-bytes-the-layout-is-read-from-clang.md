# Panel 186 ratified: a field in a C union shares its bytes, the layout is read from clang, a bit-field is refused

2026-10-02 at 15:03, on the author's answer to the recommendations the
coordinator put to them with the synthesis, each resolution item resting on
the one route built (the compiler-engineer's, in its copy of `779139d0`),
read on five clangs, and on two blind readings.

**The author's words**, meant as: *1a 2a 3a*: ratify R1 to R10 as written;
R7's home (a); and the push of `2bb45a96` once its Linux arm64 and Windows
legs are green. **Recorded as a reading**, CLAUDE.md § 4's default; not `by
delegation`.

- [x] **panel 186** | ratify, amend or overturn R1 to R10 (a field in a C union shares its bytes, the layout is read from clang, a bit-field is refused), and choose R7's home for the construction-arity form: (a) `check` stops refusing an omitted union sibling and `build` judges it, recommended; (b) a marker on the record, a sitting's; (c) `check` asks clang | `docs/panel/186-a-field-in-a-c-union-shares-its-bytes-the-layout-is-clangs-and-a-bit-field-is-refused.md` § The resolution · **ratified 2026-10-02**
    **Origin:** panel 186's synthesis, 2026-10-02 at 14:00, on the trunk
    frozen at `779139d0`. Until it is answered the compiler behaves as
    today: defects 150, 151 and 156 stay open, and their landing (a lane
    porting the compiler-engineer's built route) waits on R3's one rule and
    R1's dump bound being ratified. R3 changes panel 077's ratified
    any-arity rule for a union type (two goldens move); its conservative
    form keeps it. The second blind reading objected to R6's text and
    approved R7's.

    **Recommendation, put with its reasons**: ratify as written, because R1
    is the only route built, identical on Ubuntu clang 18.1.3 (the CI's),
    Debian 18.1.8 and 20.1.8, Apple 21 and Homebrew 22.1.8, with 397 of 447
    files identical and the author-facing initializer warnings 15 to 0, and
    R3 is one rule where the seat built two; and R7 (a), because the second
    blind reading approved the text that needs it and it adds no surface,
    `build` already judging every header fact; its cost said in so many
    words, a forgotten field of an `extern` record told by `build` rather
    than `check`.

    **Verdict, 2026-10-02: ratified, R7's home (a).** The landing proceeds:
    lane land186's batch, then R7 prototyped and read blind before the spec
    takes O_cover.
