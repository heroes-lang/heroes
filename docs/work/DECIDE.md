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

- [ ] **panel 125** | ratify the lease's soundness: the cell is the authority and it is nulled on release, a lease's name stands only as an argument of a call and takes no other write, `end_lease` takes only a lease cell, the runtime reads no memory it was not handed in any accepted program, the registry is refused on Part 7.13, and the spelling is `lease`/`end_lease` | `docs/panel/125-the-guard-that-read-freed-memory.md` § The resolution

    **Origin:** the soundness lane of 2026-09-09, two seats, convened by the
    coordinator on its own runtime guard after measuring that a double release
    was caught by luck — `malloc` had left a zeroed magic word readable in a
    freed block.

    **THREE THINGS IN IT ARE THE AUTHOR'S TO OVERTURN.**

    **(i) The registry was approved by one seat and refused by the other, and
    the resolution took the refusal.** The ffi seat prototyped a runtime-owned
    list that catches every bad shape including a pointer laundered through C;
    the compiler seat measured that `hero_grow_kept`'s one-caller assertion
    FIRES on a second buffer and that widening it is design.md Part 7.13's
    second allocation site. The conservative route is the registry, and it is
    recorded so the author can choose it.

    **(ii) The spelling.** `held`/`release`, the coordinator's, cost 29 errors
    across 11 files by the reservation mechanism itself; `lease`/`end_lease`
    cost zero by the same mechanism. Panel 121's precedent gave the author the
    spelling last time, and this one is theirs to rename before it reaches a
    reader.

    **(iii) The exit path is named, not closed.** A lease outstanding when the
    program leaves through `exit(code)` is not accused, because the gate runs
    at the end of `main` and registering it with `atexit` would make the block
    counter accuse the compiler on the same path. The Linux `--sanitize` leg is
    the instrument named instead.

*******************************************************************************
