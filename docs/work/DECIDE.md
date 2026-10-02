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

- [ ] **panel 186** | ratify, amend or overturn R1 to R10 (a field in a C union shares its bytes, the layout is read from clang, a bit-field is refused), and choose R7's home for the construction-arity form: (a) `check` stops refusing an omitted union sibling and `build` judges it, recommended; (b) a marker on the record, a sitting's; (c) `check` asks clang | `docs/panel/186-a-field-in-a-c-union-shares-its-bytes-the-layout-is-clangs-and-a-bit-field-is-refused.md` § The resolution
    **Origin:** panel 186's synthesis, 2026-10-02 at 14:00, on the trunk
    frozen at `779139d0`. Until it is answered the compiler behaves as
    today: defects 150, 151 and 156 stay open, and their landing (a lane
    porting the compiler-engineer's built route) waits on R3's one rule and
    R1's dump bound being ratified. R3 changes panel 077's ratified
    any-arity rule for a union type (two goldens move); its conservative
    form keeps it. The second blind reading objected to R6's text and
    approved R7's.

*******************************************************************************
