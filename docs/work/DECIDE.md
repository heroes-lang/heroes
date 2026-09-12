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

- [ ] **M-rotated-records step 7** | may § The chain carry more than one `**OPEN**` row, so that two lanes can each open their milestone? | `site/src/lib/chain.ts:92-94` · `.claude/rules/records.md` § Working in lanes

    **Origin:** 2026-09-12, found while writing the lane protocol this milestone
    delivers. `site/src/lib/chain.ts` throws on a second `**OPEN**` row, so the
    lane protocol is one lane short of working: two sessions can hold two
    milestones, but only one may say so in the table. The check is not wrong
    today — the chain HAS taken one at a time — it is a rule written before
    lanes existed.

    **Why it is the author's**: the file is `site/`, so changing it is an
    outward-facing act, and the floor it sits beside (`FLOOR_ROWS`,
    `FLOOR_DONE`) exists because a parse that falls apart renders an empty
    table. The cheap route is to allow N and keep the floors. The conservative
    route is to leave it and let a second lane work with its row `scheduled`,
    which costs a reader of the site the truth about what is being worked on.

    **Recommendation:** allow more than one, and say in the message how many.
    The reason is measured rather than felt: `where_we_are` already states the
    current milestone in prose and § Where we are is held to 15 lines, so the
    table is the only place a second open milestone could ever be visible.

*******************************************************************************
