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

- [ ] **panel 167** | the lend's lifetime: a field lease that copies is adopted, two type rules are widened from `cstr` to `ptr`, and routes B, C and D are refused — ratify, or take the conservative resolution the sitting recorded | `docs/panel/167-nobody-checks-the-callee-and-the-lease-we-would-copy-is-open.md`

    **Origin:** panel 167, 2026-09-20, on defect 066, which panel 166 filed and
    declined to price. **The default while this is open** is the adopted
    resolution: the field lease is built, defect 067 is repaired first because
    the lease rests on its mechanism, and defects 066 and 068 stay open until it
    lands.

    **What conservative would have been**, recorded so it can be taken: route
    C's absorbing sentence alone, **+13 real**, which documents the hole, closes
    nothing, and leaves a corruption reachable from ordinary-looking code with
    no instrument anywhere. Two seats vetoed it — on locality and on the
    absence of any instrument — so taking it overrides two vetoes rather than
    choosing a price.

*******************************************************************************
