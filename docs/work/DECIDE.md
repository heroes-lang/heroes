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
**OPEN: 3**

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

    **PARTLY OVERTAKEN BY PANEL 168, 2026-09-20.** Its clause 1's allocation half
    is struck on a veto, and its clause 2 is suspended on three measurements.
    What is still yours to ratify here is the rest: that a lend gains a sibling
    that copies. Panel 168 measured that the sibling closes **neither** defect,
    so a yes no longer buys what this item said it would.

- [ ] **panel 168** | the lease allocation keeps the leading header that ships, panel 167's clause 1 allocation half is struck on a veto and its clause 2 is suspended, and route A is measured to close zero of two defects — ratify, or take the conservative resolution the sitting recorded | `docs/panel/168-the-property-is-not-the-base-and-route-a-closes-zero-of-two.md`

    **Origin:** panel 168, 2026-09-20, the soundness lane, on panel 167 clause
    1's allocation half. **The default while this is open** is the adopted
    resolution: the runtime's lease allocation does not change, clause 2 is not
    built until it is re-argued, and this milestone's remaining question is
    defects 066 and 068 rather than route A.

    **What conservative would have been**, recorded so it can be taken: adopt the
    leading header and say nothing about route A's efficacy, leaving defect 066's
    entry reading *"this entry closes when that lands"*. It is the cheapest thing
    on the table and it is **false, measured** — it would have the milestone build
    a feature for two more steps and discover at the close that the list is not
    clean.

- [ ] **panel 169** | the lend's extent is stated in the spec before any rule enforces it, the caller-side rule is stated over the IR rather than the AST, defect 066 and defect 068 are two classes, and defect 069 becomes the milestone's blocking item — ratify, or take the conservative resolution the sitting recorded | `docs/panel/169-two-defects-two-classes-and-the-one-that-was-never-searched-where-it-happens.md`

    **Origin:** panel 169, 2026-09-20, the full panel, on what closes defects 066
    and 068. **The default while this is open** is the adopted resolution: nothing
    of the caller-side rule is built until `spec § 13` says how long a lend is
    readable, because the ergonomist measured that the same rule closes defect 068
    under one reading of the extent and closes nothing under the other; defect
    069's six-line repair lands first; and R5, give the ownership away, is the
    route for 066.

    **What conservative would have been**, recorded so it can be taken: adopt the
    caller-side rule as the sitting's own first draft had it, widened to *written*
    and stated over the AST. It is built, it is measured at 0 of 752 files
    refused, and the completeness critic **ran the program that shows it leaves
    defect 068 open one statement kind over** — a re-declaration on a second loop
    turn, `check` 0, zero diagnostics. Taking it closes the witness and ships the
    class for the third time in one milestone.

*******************************************************************************
