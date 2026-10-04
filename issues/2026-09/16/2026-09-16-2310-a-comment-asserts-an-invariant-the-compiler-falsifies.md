- [x] **051 — a comment asserts an invariant the compiler falsifies, and a design document cites it as live** | `selfhost/check/table.hero:66-67` said a fallible's argument is never itself a fallible; `--dump-ir` names `i64???` at 27 sites | closed 2026-09-16, M-check-completeness

    **Origin:** panel 158, found independently by its compiler-engineer and its
    spec-warden, and narrowed by its completeness critic.

    **The false sentence**, verbatim: *"`T?` — never nested: `T??` is rejected by
    the parser, so this node's argument is never itself a fallible."* The premise
    is true — the parser does reject the written form — and **the conclusion does
    not follow**, because the checker builds the node from a container's element
    type without any written form passing through. Measured false three ways at
    exit 0 in one session: the map route, the `find` route, and `wrap<T>` applied
    twice, the last naming `i64???` at 27 sites under `--dump-ir`.

    **It is `.claude/rules/module-shape.md` § *a premise about the world expires
    silently* exactly**: the argument stayed valid and only the premise died, so
    the comment went on reading as correct.

    **THE ENUMERATION IT OWED, RUN 2026-09-16.** The open question was: *is any
    `.fallible` CONSUMER non-recursive?* Three were proved recursive by two seats
    and the rest were unchecked. Measured:

    `grep -rn "\.fallible" selfhost/` reads **238** lines in **63** files, but
    most are match arms lumping the kind with others to say *not this one*, which
    assume nothing. `grep -rn "\.fallible [a-z_]* *=>"` isolates **65** arms that
    name it. Of those, **fifteen recurse** on the payload, **four** push it onto a
    worklist the loop drains, and **six peel exactly one level without
    recursing**:

        check/access.hero:272      `?` itself
        check/builtins.hero:58     `.default(v)`'s argument type
        check/builtins.hero:346    `.must()`
        check/discard.hero:57      whether the payload is unit
        check/walk.hero:856        the map read — the route that BUILDS the
                                   nested value the false comment denied
        check/walk.hero:1973       `ok`/`fail` against an expected type

    **Four of the six were RUN, not read**, which is the difference between an
    audit and a claim, because one level is CORRECT for every one of them and a
    reading could only assert it. On a `{str: i64?}`, so that `m["a"]` is `i64??`:

        m["a"].default(1)        check 1   expected `i64?`, found `i64`
        m["a"].default(ok(1))    check 0
        v: i64 @ m["a"].must()   check 1   expected `i64`, found `i64?`
        v: i64 @ m["a"]?         check 1   expected `i64`, found `i64?`

    Each peels exactly one level and each names the right type in its own
    diagnostic. **So the answer is: six consumers are non-recursive and not one
    of them is wrong.** One level is the right depth for `?`, `.must()`,
    `.default()`, the map read and the two constructors. The comment was false
    and nothing rested on it, **which is a result and not an absence of one** —
    it is what turns 051 from a repair into a correction.

    What it does NOT cover is defect 050, where peeling one level is correct and
    the program still reads as asking about the other layer.

    **A SECOND WITNESS, found by panel 159 and closed here.** The same class in a
    shipped header: `runtime/heroes_runtime.h` said `sort` *"Works on `[i64]`,
    `[f64]` and `[str]`, which is exactly the set with an order"* and
    `runtime/parts/sort.c` said *"for the three element types that have an
    order"*, while `hero_cmp_for` dispatches **twelve** — every integer width,
    both floats, `str` and `bool`. False since M-sized-integers and panel 068.
    `tests/golden/run/builtins.hero` records that the EMITTER's copy of the same
    list was found stale and fixed; these two were not looked at. Found by panel
    159's compiler-engineer, and the critic added the line that made it matter:
    **one line above the rot, the same header says `ascending`** — so the fact
    panel 159 convened to price was already published to a C author and not to
    the Heroes author reading the specification.

    **The repair, at the class and not at the witness.** All three comments are
    corrected: `check/table.hero` says which half is true and what the audit
    found, `sort.c` and `heroes_runtime.h` name the PROPERTY instead of counting
    the types, because a count rots and `hero_cmp_for` is the only honest census.
    `docs/design/design.md`'s citation of those lines as a live invariant is
    corrected underneath with its date, the document being append-only.
