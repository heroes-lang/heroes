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

- [ ] **panel 185** | ratify, amend or overturn R1 to R8: a macro-only C name refused and told as a macro with a shim it can use; defect 145's one query; an arm's line holds one statement that names no binding, `_ = e` included; a value block that leaves on every path leaves, R4 kept off c5; R4 landing with its seal; a spaced sign deleted before a string and two guesses otherwise; and for the forgotten `f`, (5b) only once the author's condition is met | `docs/panel/185-a-macro-is-named-as-a-macro-an-arm-takes-a-statement-a-leaving-block-leaves-and-a-spaced-sign-has-two-readings.md` § The resolution

    Until it is answered, the compiler goes on as today: a macro-only name
    is told *declares no* (false), an arm's `_ = 0` is refused with a false
    message, a value block that leaves is refused, `--apply` writes 5 wrong
    programs from the sitting's sign probes, and a forgotten `f` prints its
    braces. For R7 the author chooses: (a) one blind reading of (5b)'s
    wording, one session capped at 3 USD (this sitting's cost 0.61 USD),
    landing (5b) if it approves; or (b) a ruling that the earlier reading's
    objection, a locality of compiling, falls outside the condition, since the
    author's ruling of 2026-10-01 put legality outside design.md §1.3.

    **Recommendation: ratify R1 to R8, and for R7 take (a).** The reason: R1
    to R6 rest on routes built and run (the macro's out-of-bounds read, t7,
    the census of 1,530 files, the depth table), and (a) costs well under a
    dollar and settles the one premise no command has settled, the words.

*******************************************************************************
