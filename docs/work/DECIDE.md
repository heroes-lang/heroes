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

- [ ] **panel 130** | ratify the naming mandate on a callback: a function handed to a callback type that names its parameters names them the same way, at 7 of 7 against 2 of 7 | `docs/panel/130-the-name-a-generic-callback-could-not-say.md` · `docs/measurements/010-spec-budget-ledger.md` row 67

    **Origin:** panel 130, 2026-09-11, on the route you asked for at panel 129's
    ratification. You have already chosen the rule itself — the sitting offered
    the cheaper 2-of-7 reading and you took agreement with both numbers in front
    of you — so this item is the sitting's own verdict on what that produced,
    which is not the same thing.

    **What is new since you chose.** Two of the three seats sent the work back
    after you decided: the spec-warden found that a written type naming a
    position nothing can confuse was uninhabited by any function in the language,
    and that a sentence I had put in design.md was false; the compiler seat found
    a diagnostic stacking onto three others, a golden deleted where the records
    are append-only, and two ceilings. All are repaired and each is in the
    sitting's file.

    **And the ergonomist's finding, which is the best argument for the rule and
    was not available when you chose it.** Reading the specification alone it
    wrote `acc, item` three times across three different tasks without making a
    naming decision, where left to itself it would have written `total`/`n`,
    `so_far`/`word` and `best`/`x` — **three vocabularies for one built-in**. And
    it reports the whole signature-order class closed: a swapped `fold` callback
    cannot compile in either direction, by name where the two share a type and by
    the ordinary type rule where they do not.

    **What a yes settles**: the mandate, the document at −1 real, and the
    milestone's tag. **What it does not**: a callback whose own two parameters are
    different types, and a user's own generic higher-order function. Both are in
    the sitting under *What this sitting did not close*.

*******************************************************************************
