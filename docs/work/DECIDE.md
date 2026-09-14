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

- [ ] **panel 149** | a handle is what a type REACHES, on the consumer side as well as the producer side, no refusal lands for the multi-handle record, and the releaser a mark names must be checked — ratify, or take the conservative resolution the sitting recorded | `docs/panel/149-the-walk-already-existed-and-the-instrument-was-counting-calls.md`

    **Origin:** panel 149, 2026-09-14, convened by author instruction on
    `docs/work/DEFECTS.md` 033. Full panel, five seats plus the completeness
    critic. **The default already in force** is the sitting's provisional
    resolution, so leaving this open costs nothing beyond the ratification.

    **What is being ratified, in one line each.** The rule asks what a type
    reaches through group-record fields and fixed-array elements to any depth,
    of `consumes` parameters as well as of results and `@` out-parameters,
    because a producer-only rule was measured to stay silent on raylib's `Font`
    while erroring on the contrived case. The diagnostic prints the dotted path.
    **No refusal lands for a record reaching several handles**: all three
    candidates were withdrawn by measurements taken in the sitting, one of them
    by its own author's stated condition firing on `LoadFont` unnoticed. One mark
    is one obligation on the whole value, which is what C ships. The releaser a
    mark names must resolve and must carry `consumes` on a parameter of the
    marked type or a handle it reaches — today `acquires sqlite3_notafunction`
    checks and builds at exit 0.

    **What conservative would have been** (CL-040), recorded so it can be chosen
    instead: R1 producer-side only, the consumer side left at one level, and no
    releaser check. Cheaper, and it buys a rule that does not fire on the one
    shipped library that has the shape.

    **The one number:** +4 real on the specification, 7974 → 7978 against a
    ceiling of 10240, the spec-warden's `dsub` substitution. Adopted over zero
    because the critic showed the zero-token reading rests on an argument the
    document refutes one line above it, where `owned` sits in the identical
    grammatical position and is narrowed in prose.

*******************************************************************************
