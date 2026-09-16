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

- [ ] **panel 155** | The call-site obligation pass is refused and `==` on a bare type parameter is refused in the BODY instead, by the language's own written answer; `float_map_key` through a generic waits under Principle 0 — ratify, or take the conservative resolution the sitting records | `docs/panel/155-the-hole-was-never-made-by-the-generic.md`

    **Origin:** panel 155, 2026-09-16, at M-check-completeness. The compiler
    currently behaves as R3 says it should — it accepts both shapes — so the
    default while this is open is **the status quo for the float half**, which
    costs nothing, and **R2 unlanded**, which costs `spec § 13` staying false
    through a generic.

    **What the resolution decided, in one line each.** R1: the call-site pass is
    REFUSED, on the compiler seat's soundness veto — it needs a second span-keyed
    table against a standing rule in `check/state.hero:78-85`, measures 284 code
    lines against panel 082's estimate of 90-110, and reproduces the defect 035
    shape in its own walk. R2: `==` on two values of a bare type parameter is a
    compile error in the BODY, which is `design.md:1721`'s own sentence — *"If an
    operation on `T` is needed, pass it as a parameter"* — and deletes **0 of the
    50** generic functions in this tree, measured. R3: the float half waits. R4:
    the real hole is filed as defect 046 and is not made by generics. R5: `spec
    § 10` owes a sentence naming which types may key a map, which is the
    llm-ergonomist's condition and is owed either way.

    **The conservative resolution, recorded so it can be taken instead**: adopt
    the call-site pass for `ffi_partial_operation` alone, which three seats were
    willing to. It buys the same behaviour as R2 at 284 lines rather than a body
    rule, against a veto, and against four language communities' measured
    experience of that mechanism — C++'s own designer, Go's proposal by name, D,
    and Zig's seven-year closed request.

    **What weakens this sitting, in the coordinator's own voice**: two of the five
    seats were resumed after a watchdog kill with the findings of two that had
    finished already in hand, so the split is **two independent readings plus two
    informed concurrences**, not four of five. Four numbers in the coordinator's
    briefs were carried from documents rather than measured, one of which cut in
    the proposal's favour. Both are written into the sitting's own file.

*******************************************************************************
