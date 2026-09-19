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

- [ ] **panel 165** | route 6 is refused on two vetoes and the sitting adopts the repair of three defects instead — ratify, or take the conservative resolution the file records | `docs/panel/165-the-check-it-would-add-was-not-one-and-the-route-that-shipped-had-none.md`

    **Origin:** panel 165, 2026-09-19, at M-declared-extents step 1. The
    sentence route 6 rested on — *"the only route where the compiler CHECKS the
    extent"* — was falsified three times with three instruments: C erases a
    parameter's extent, so the check is author-against-author while route 4's
    field check is against the header. **The default the compiler runs on while
    this is open** is the refusal plus the three defect repairs, which is what
    CLAUDE.md § 4 asks for: the most robust resolution, not the cheapest.

    Conservative, recorded in the file so it can be taken: leave route 6 queued
    and file only the defects. It is cheaper by the whole of route 14 and it
    leaves defect 063 — memory corruption at exit 0 — open, which is why rank 3
    decides it.

    Two routes nobody had listed are recorded there with their measurements:
    **route 12**, a header `constant` as the extent, which is portable AND
    header-checked and is the form route 6 should have been; and **route 13**,
    deriving the extent from the field.

*******************************************************************************
