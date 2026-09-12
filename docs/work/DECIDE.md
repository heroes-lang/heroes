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

- [ ] **panel 134** | the contract measures 7306 real against a stated 5500, so it has been 1806 over for three days — ratify 8192 written as `7306 + 886`, the delta gate at +50 vendored on both documents, and the refusal of a digest for the contract | `docs/panel/134-the-contract-was-over-its-ceiling-for-three-days.md`

    **Origin:** 2026-09-12, M-declared-thresholds. **The default the compiler
    runs on while this is open**: `heroes measure CLAUDE.md` goes on printing
    `Headroom: 9` against a ceiling on the wrong scale, and the `contract` check
    goes on judging the vendored figure — so the contract can grow another 1800
    tokens with every instrument in the tree reporting green. Conservative was
    costed and recorded rather than adopted: `7306 + 373`, the same margin
    2026-09-07 chose, which is **7679** and about four days of room at the
    measured rate. The author has instructed 8192.
*******************************************************************************
