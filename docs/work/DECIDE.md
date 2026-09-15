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

- [ ] **panel 150** | the counter becomes a SET, a `tag` may name a C type NAME, the mark is refused on a `ptr`, and the spec is corrected on both counts it is false about — ratify, or take the conservative resolution the sitting recorded | `docs/panel/150-the-runtime-holds-a-number-where-it-needs-a-set.md`

    **Origin:** panel 150, 2026-09-15, convened on defects 037 and 038 without
    asking, under CLAUDE.md § 4. Full panel plus the completeness critic. **The
    resolution is not what any seat proposed**, because the critic falsified the
    premise four of them shared.

    **The one fact under every finding**: the runtime holds a number where it
    needs a set. A counter cannot tell a null from an address, a double release
    from an unmarked producer, or an element release from a composite one. An
    address set tells all four, and `runtime/parts/alloc.c` had already named
    that instrument in writing while pricing it at nothing.

    **Why the cheap route is refused.** All five seats converged on rewriting the
    runtime message, nine lines and zero spec tokens. The critic measured that
    **the message is only ever readable on the one program that is memory-safe**,
    while the two that actually corrupt exit 133 saying nothing at all on either
    stream. It would also be the third rewrite of that message in two days.

    **What conservative would have been** (CL-040), recorded so it can be chosen
    instead: the message repair alone, leaving 037 and 038 open and 039, 040 and
    041 unaddressed.

    **The measured cost of the spec half is +7 real**, 7974 to 7981, the
    spec-warden's D1, which repairs two falsehoods and not one: `acquires` is
    said to go after a *handle* result where the compiler asks what a result
    REACHES, and the demand is said to be per *group* where the compiler keys on
    the type program-wide and across modules.

*******************************************************************************
