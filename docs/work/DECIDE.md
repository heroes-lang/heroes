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

- [ ] **panel 145** | M-handle-verdict's sitting: ratify **029 ENTERS** as a fieldless `extern` record that is a handle, spelled as the header's own type name starred; **030 REFUSED as a form**, its home the Part 6 borrow-checker row, zero words added; **031 a WART adopted with the defect held open** until the consume-mark prototype is measured — and rule on the one question the sitting was reserved and did not decide, **the type's name** | `docs/panel/145-a-handle-is-a-pointer-with-a-name-and-the-compiler-already-reads-the-name.md` · design.md Part 6 borrow-checker row, Part 8 wart 20 · `spec/heroes-spec.md` § 13 · `docs/work/DEFECTS.md` 029, 031

    **Origin:** 2026-09-13, M-handle-verdict step 2, the sitting the milestone
    was scheduled for. **The default while this is open**: `ptr` one type for
    every C pointer, `sqlite3_step(db)` building at zero diagnostics, and steps 3
    to 5 proceeding on the provisional resolution — the `swap-ptr` operator, then
    the form, then the consume-mark prototype. **The finding**: with a nominal
    parameter type the checker **already** refuses the swap (`type_mismatch:
    expected Stmt, found Db`, no new rule), and what fails is the emitter
    spelling a group record as a struct by value; in C the header's own pointer
    type makes clang refuse the swap too under a flag already carried. **The
    critic corrected the form's spelling** — every seat built `struct <tag> *`,
    which reaches one of three handle types in the tree (`CURL` is `typedef void`)
    — and the coordinator's census four ways. **Conservative was recorded**: the
    ledger's hand-written idiom written into § 13 as a WART at +27, and 031 closed
    tonight on `--sanitize` alone; refused because §1.12 is a goal of the language
    and a guard this cheap is built, not argued. **Recommendation: ratify as
    adopted.**

    **The undecided question, put rather than defaulted.** Panel 139 reserved for
    this sitting the type's name — `unsafe_ptr`, or a family — and the sitting
    handed it to no seat. **Recommendation: keep `ptr`, and treat a rename as a
    sitting of its own if wanted**, because it is a spec-wide surface change
    touching every `extern` line in `examples/` (27 name a `ptr`), and the handle
    form removes most reasons to write a bare `ptr` at all; the historian's
    regret about Swift's `OpaquePointer` was about typing, not naming.

*******************************************************************************
