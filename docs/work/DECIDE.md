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

- [ ] **panel 117** | Does `assert` owe both sides for an aggregate, and does the sitting's own sixth option, a leaf walk that names the differing field instead of rendering the value, replace the four the brief carried? | `docs/panel/117-both-sides-of-an-assert-when-a-side-is-an-aggregate.md` · `docs/work/DEFECTS.md` 016

    **Origin:** panel 117, 2026-09-07, convened on open defect 016 by author
    instruction (*"when in doubt, panelize"*). Five seats, two vetoes that
    disagree with each other, and a resolution that needs neither overridden.

    **What is adopted provisionally, and what it costs.** The blind instrument
    is repaired now: the check that watches `assert`'s output pinned the scalar
    case and had no aggregate row, so it could never have seen this defect. The
    spec sentence is rewritten by reference to `print`'s own set rather than by
    listing types, measured at net **+3** tokens against a named **−16**
    removal, and it carries its restoration condition, which is what makes a
    narrowing read as honesty rather than retreat. The mechanism itself, the
    leaf walk, is **scheduled and not landed**: it is unrun, and two vetoes rest
    on measurements it must be tested against first.

    **What the conservative resolution would have been**: land nothing and
    leave the defect gated. It was refused because the spec would go on
    promising something no mechanism can deliver for `{K: V}` at all, measured
    in the sitting, and because the blind check would stay blind.

    **The default the compiler runs on while this is open**: an `assert` whose
    sides are aggregates prints the source expression alone, with no note that
    the sides were withheld.

*******************************************************************************
