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

- [ ] **panel 133** | Do the grammar's productions enter `spec/heroes-spec.md` inline, which needs §1.6's 6144 ceiling raised or the production set trimmed, or does a hand-written grammar file ship beside it under eleven seat conditions? | `docs/panel/133-the-half-that-can-be-derived-and-the-half-that-cannot.md`

    **Measured, not estimated.** The productions alone are **1253** vendored
    tokens and `heroes measure` calls that *"a lower bound"*; the spec's real
    headroom is **368** (5716 real + 60 FFI floor against 6144). So they do not
    fit inline under any ratio, and the question is a ceiling, which is yours.

    **The default while this is open**, and it is the sitting's adopted
    resolution: `heroes grammar` prints the derivable half (keywords, token
    spellings, the nine precedence levels with their `ast.BinaryOp` names) and
    **nothing hand-written ships**. Five seats falsified hand-writing the
    productions from five different inputs and none defended it.

    **The conservative answer, recorded so you can take it**: ship the
    hand-written file with five checks and all eleven conditions met. Cheaper,
    no ceiling decision, and it is what Zig built before archiving it.

*******************************************************************************
