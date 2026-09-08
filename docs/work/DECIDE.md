# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item asks **what should be true**, and until it is
answered the compiler goes on behaving some way by default — so the item names
that default, because it is the cost of leaving the item open.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/work/DONE.md`, the record. Rank by
what an item blocks, never by age, and verify it against the repository before
putting it to the author: asking a settled question is the one cost this list
cannot pay.

**The shape.** One line per item, and an optional body indented four spaces
under it. The first field is the item's **origin**, and where that origin is a
sitting it is spelled `panel NNN`, padded — because
`tests/harness/suite_records.hero`'s `queued` check reads the `- [ ] ` lines
alone and scans them for exactly that, so a citation that slides into the body
makes every pending sitting report as unqueued, and it fails silently. Nothing
lives outside the two banners: `records/lists` is the executor of that.

Format: `- [ ] **<origin>** | <the question, in one line> | <where to look>`

*******************************************************************************
**OPEN: 3**

- [ ] **panel 120** | ratify or overturn the verdict on the spec's higher-order silences: prose bought at +30, the type signatures refused, and the −35 removal priced and not taken | `docs/panel/120-the-signature-cannot-say-which-half-it-keeps.md`

    **Origin:** the sitting of 2026-09-08, opened by author instruction mid-step — four seats, provisional pending this answer.
    **The default the compiler is running on** is the resolution as adopted and
    landed: R1 states *no anonymous functions* in the enumeration of absences at
    `spec:110-111`; R2 puts `filter`'s polarity and `find`'s firstness into the
    `Built-ins:` sentence in prose; R3 names `not_found` beside `missing_key`;
    R4 refuses the type signatures; R5 keeps the mandatory-named-argument
    bullet. Total **+30**, the spec at **3995**, **40** tokens free.
    **The finding that replaced the whole ballot**, the historian's: a signature
    is **byte-identical for keep and for reject**, so every draft on the ballot
    bought the loud class and nothing of the silent one. Prose closes it at less
    than half the price.
    **What conservative would have been**: refuse, the spec stays silent, 70
    tokens free and no ledger row. Two seats argued against it.
    **Recommendation: ratify.** No seat vetoed the adopted resolution; the two
    facts bought are the only two in the sitting whose wrong guess compiles and
    lies; and R1 is what panel 119's refusal of closures owes a reader, since
    that refusal lives in design.md and design.md is not what a reader gets.
    The one part the author may want to overturn is R5: the removal would have
    left the spec smaller than it started, and it was refused on an ergonomic
    argument rather than a measured one.

- [ ] **panel 119** | ratify or overturn the verdict on Part 7 items 1 and 12: closures REFUSED with a Part 6 row, inline blocks EXAMINED AND UNPLACED, and three design.md sentences corrected | `docs/panel/119-the-warning-that-does-not-fit.md`

    **Origin:** the sitting of 2026-09-08, M-closures-verdict step 1 — full panel, five seats, provisional pending this answer.
    **The default the compiler is running on** is the resolution as adopted:
    R1 refuses closures with capture on cost alone, R2 gives the row a falsifier
    that deliberately does NOT rest on capture being hazardous, R4 names the
    capture-free narrowing as the form that returns on a measured Part 11
    effect, R5 puts inline blocks in `comptime`'s *unplaced* shape with three
    joint return conditions, and R6 corrects *"~150 lines"*, *"~60 spec
    tokens"* and *"no refcount interaction"* in design.md.
    **The measurements that decided it**, all 2026-09-08: 282 match arms in 74
    of 190 modules and five DECIDED ceilings breached; **+118** spec tokens
    against **70** free (the sitting's own 71 was corrected by panel 120 the same day), with six removals priced and none admissible; four
    compiled corruption classes, one of them clang-clean under all fourteen
    flags and **exit 139** at run time; and the subtraction test worth **21
    declarations, all in example programs, none in the compiler**.
    **What conservative would have been** is in the file's own section: option
    F for both, a dated deferral, cheaper by two documents and objected to by
    two seats — the warden because a dated condition is the mechanism §1.6 names
    as the one the record shows failing, the ffi seat because it leaves three
    pieces of prose reading as true when they are not.
    **Recommendation: ratify.** Three seats vetoed the feature and the two that
    compile both approved the refusal; the historian found the refusal is a
    mainstream position under this language's exact constraints, four languages
    in writing and one of them self-hosting since 1988. The one thing the author
    may want to overturn is R5: putting inline blocks *unplaced* rather than
    deferring them costs a design.md paragraph that a deferral would not.

- [ ] **panel 118** | ratify or overturn the resolution on `_ =` and a fallible: three positions closed, no `certain` fix, and the deliberate drop paid for by a narrow emitter relaxation rather than a new built-in | `docs/panel/118-the-missing-word-was-a-dead-variable.md`

    **Origin:** the sitting of 2026-09-08, `docs/panel/118-the-missing-word-was-a-dead-variable.md` — full panel, five seats, provisional pending this answer.
    **The default the compiler is running on is the resolution AS LANDED, which
    is not the resolution as adopted** — this item said "as adopted" and named
    three facts the sitting's own § Corrections had already overturned; corrected
    here 2026-09-08 by `/decide`, which read the panel file before putting the
    question, because ratifying a description the record contradicts settles
    nothing. What the author is being asked about is this:
    R1 closes **two** positions, `_ = e` and a `_` parameter — the type parameter
    was **WITHDRAWN** at `docs/panel/118:344` after it was run, because
    `drop(risky(0 - 1))` through `function drop<A>(_: A)` prints and exits 0 with
    no discard statement anywhere, so the refusal relocated the hole rather than
    closing it, and `_ = [x]` transfers it to the position the rule exists to
    protect. R3 ships three `guess` fixes and no `certain` one. **R5 was
    REPLACED** (`:373`): the emitter relaxation for `eq`, `ne` and the three
    bitwise ops was built, measured and thrown away because it closed two of four
    shapes — `_ = a == b` on a `str` and on an array still warned — and what
    shipped declares the temporary `__attribute__((unused))`, the idiom the
    sibling half of this class already used on 2026-08-16, which closes all four
    and keeps every abort check. R6 spends **+62** and not the adopted +49: what
    landed is the boundary-carrying wording, because `args_checked() -> [str?]` is
    the spec's own built-in and one `_ =` line drops every non-UTF-8 failure it
    carries (ledger row 3965).
    **What conservative would have been** is in the file's own section: the
    clause at +15 covering the statement only, `match` with block arms at five
    lines a site and no emitter change, and the `ignore` built-in, which two
    seats wanted and which is blocked by `selfhost/check/builtins.hero` sitting
    at 374 of a DECIDED 374 rather than by any seat's judgement.
    **Recommendation: ratify.** The robust and the cheap disagree here and the
    author's standing instruction of 2026-09-08 is to prefer robustness; the two
    extra positions cost zero migration sites, and the emitter relaxation costs
    ~14 lines in the one nearby file with headroom and moves no golden's bytes.

*******************************************************************************
