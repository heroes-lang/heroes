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

- [ ] **Site refresh 2026-09-17** | design.md Part 7 items 2 (file I/O) and 3 (command-line arguments) carry no status line, while spec § 11 provides `read_file`, `write_file`, `args`, `args_checked` and `exit`, so the ledger reads as deferring what has shipped: do the two items get an ENTERS date and the milestone that landed them, or a note saying why they stay? Default until answered: the site's refusals page lists neither among the postponed items | `docs/design/design.md` Part 7 · `spec/heroes-spec.md` § 11 · `site/src/html/why/not.html`

*******************************************************************************
