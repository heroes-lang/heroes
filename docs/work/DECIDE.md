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
**OPEN: 1**

- [ ] **panel 125** | the lease clause says a forgotten lease is *counted* where every other runtime stop in the document *aborts*, and the word never says the program dies: does §4.19's fourth case take the document's own verb, at +0 or +4 vendored tokens? | `spec/heroes-spec.md:246`, the verb at `spec:71` and its five other uses

    **Origin:** the site panel's languages seat, 2026-09-10, on the sentence the
    author ratified hours earlier. It is the same sitting's fourth point read
    once more, so it is filed under it rather than as a new number.

    **What is measured.** *Abort* is defined at `spec:71`, *an abort ends the
    program, saying why*, and the document uses it for overflow, an index, a
    slice that splits a character, division by zero, recursion too deep, a
    `nan` in an ordering or a `sort`, a shift count outside 0..63, and an
    argument that is not UTF-8. The lease is the one runtime stop that does not
    say it, and the program does die: exit 134, run twice. Two wordings priced
    on a scratch copy with the vendored instrument: *aborts when `main`
    returns, saying how many* is **4210**, +4; *aborts when `main` returns* is
    **4206**, +0, and drops the count the message actually prints.

    **The recommendation is the +4 wording**, because the count is what the
    message gives and a reader who is told only *aborts* will look for a name.
    **It is a language document, so CLAUDE.md §4 sends it to the panel** rather
    than to a session's judgement, and the author may take it directly instead.
    A prediction is available and pays for it either way: **no golden covers a
    forgotten lease leaving through `exit()`**, measured over every
    `tests/golden/run/*.hero` (four name `lease(`, none of those names `exit(`),
    so the bypass the site now asserts rests on one scratch run. The golden is
    owed with the amendment.

*******************************************************************************
