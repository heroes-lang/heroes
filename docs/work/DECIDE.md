# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item asks **what should be true**, and until it is
answered the compiler goes on behaving some way by default — so the item names
that default, because it is the cost of leaving the item open.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/records/done/`, the record. Rank by
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

- [ ] **panel 171** | the word is `lent`, in its own slot beside `counted_by`; a lend reaches only a `lent` parameter of an `extern` function; the spec sentence is the warden's merged draft at +13 real and promises nothing about a pointer C hands back; the landing is two commits with 27 marks on 19 functions across three trees — ratify the word, the rule and the landing, or take the conservative sentence the sitting recorded | `docs/panel/171-lent-and-the-pointer-c-hands-back.md`

    **Origin:** panel 171, 2026-09-20, the full panel, on the word and the exact
    rule under the default the author flipped that evening. **The direction is
    not in this item**: it is the author's ruling, recorded in
    `docs/records/log/2026-09-20-1930-heroes-must-be-robust-and-the-default-flips.md`.
    **The default while this is open** is the adopted resolution, and work
    proceeds on it: commit A parses `lent` with the rule unchanged; commit B
    flips the rule, marks 27 parameters, lands the sentence and the fence, and
    moves 35 goldens.

    **What conservative would have been**, recorded so it can be taken: the same
    word and rule with the llm-ergonomist's longer sentence, +42 real, which
    says *takes a lease or a pointer C owns*. It is clearer to a reader and it is
    **false of the pointer C hands back**, which the completeness critic measured
    on five programs. The sitting took the shorter sentence because it is the one
    that is true.

*******************************************************************************
