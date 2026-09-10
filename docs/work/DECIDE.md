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

- [ ] **panel 126** | ratify the specification's new shape as adopted: thirteen numbered sections in reference order with Literals before Types, one home per rule, the numbers as the citation anchors, and the two worked examples kept on the decision of 2026-09-10 rather than on a measurement | `docs/panel/126-the-document-nobody-had-tidied.md` · `docs/measurements/010-spec-budget-ledger.md` row 63 · `docs/measurements/028-six-blind-readers-and-what-one-text-says-that-the-other-does-not.md`

    **Origin:** the sitting of 2026-09-10 into 2026-09-11, five seats, on the
    author's own decision to re-shape the document. **The default the compiler
    is running on is the adopted text**, which landed with this milestone, so
    what a ratification settles is whether it stays as it stands.

    **Two seats objected and neither has a veto here.** The spec-warden objects
    to the two worked examples, the Failure one and the containers one, as
    additions nothing pays for: about 150 real tokens, 77% of what the additions
    cost, with no Part 1 argument and no instrument to score them until Part 11's
    metric 2 runs. Its veto attaches only to a budget breach, which does not
    happen, or to a ledger row claiming the merges paid for the examples, which
    row 63 does not claim. The compiler seat's objection was instrumental and was
    met inside the sitting: its six probes repaired the `anchors` check, the
    fences are skipped, the abort clause was reworded, and its three citation
    tables were applied.

    **What the author is choosing between.** Keep the text as adopted; or cut
    the two examples, which the warden predicts lands the document at 5412 ± 12
    real tokens and which the ergonomist's blind A/B is the evidence for or
    against; or keep them and ask that the ledger row say more than it says.
    **The recommendation is to keep them**, because the one experiment that
    exists ran in this sitting and the author's decision of 2026-09-10 already
    weighed the unmeasured effect; the conservative route is recorded beside it
    rather than argued away.

*******************************************************************************
