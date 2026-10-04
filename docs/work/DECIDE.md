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

- [ ] **panel 190** | ratify, amend or overturn R1 to R12 (route A-star: one exit only where a function has two or more ways out, made by a pass after lowering; the return slot borrowing, a slot kind and a verifier check at landing; the merge total or loud; the return-type check restored over the exit; coalescing slots and a liveness-pruned sweep refused; design.md Part 5's sentence; defect 231 made `blocking` on the Windows box's measurement) | `docs/panel/190-a-function-with-two-or-more-ways-out-leaves-by-one-exit-that-sweeps-once-its-return-slot-borrowing.md` § The resolution

    **Origin:** panel 190's synthesis, 2026-10-04 from 13:57, on the seats'
    archive of `703af779`. Until it is answered defect 231 stays open, and,
    `blocking` by R9, it lands in the next batch on the provisional
    resolution (CLAUDE.md § 4, *the panel never blocks*).

    **Recommendation: ratify R1 to R12**, on what was built and run:
    - A, G and A-star built, each fixpoint by `cmp`; A-star's own tests 1,158
      with one shape assertion owed;
    - every route's boundary columns the trunk's on this Mac and on Linux arm64
      under two clangs, Valgrind's count included;
    - A-star's IR and the compiler's own memory on the 800 shape linear (16 MB
      against 450 MB), the seed 14.5% smaller and 21.8% cheaper for clang at
      `-O2`;
    - on the Windows box, the trunk's 800 shape does not build and every exit
      route's does.

    **The price A-star pays**: 3.5 to 4.1% of the interpreter's `-O2` depth,
    and a fifth to a third more clang memory at `-O2 -g` on the extreme shape
    on Apple clang.

    **The conservative alternative, yours to choose instead**: route G. It
    costs no depth, but leaves the IR and the compiler's own memory quadratic,
    and needs `.claude/rules/generated-c.md`'s one-label rule and panel 021's
    R1 and R8 amended.

*******************************************************************************
