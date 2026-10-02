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

- [ ] **panel 185** | R7, the forgotten `f`: its blind reading objected to (5b)'s wording; land (5b) on the author's ruling that design.md §1.3 speaks of meaning, with the wording widened to the rule as built, or (5a), or (5c) | `docs/panel/185-a-macro-is-named-as-a-macro-an-arm-takes-a-statement-a-leaving-block-leaves-and-a-spaced-sign-has-two-readings.md` § Author's verdict · `docs/panel/185-reports/llm-ergonomist-5b-reading.md`

    Until it is answered, a forgotten `f` compiles and prints its braces,
    told only by panel 184's R2 note once that lands. The reading of
    2026-10-02 objected to K, *not a veto: the meaning is always literal
    text, only whether the program compiles depends on bindings outside the
    line*, and read the short wording as reaching bare names only; it
    approved M, (5a), as every blind reading has. Measured by the
    compiler-engineer on `03e70520`: (5b) 0 false alarms, the 2 true sites,
    0 files stopped; (5a) 32 false alarms, the same 2 true sites, 22 files
    stopped, the compiler among them.

    **Recommendation: (5b), landing after R1, its sentence widened to the
    rule as built (a hole whose names are all bound where the literal
    stands, `{i + 1}` included).** The reason: the reading's one objection is
    a locality of legality, which the author's ruling of 2026-10-01 put
    outside design.md §1.3, and the reading itself says the meaning stays
    local; (5a) buys no site (5b) misses, in the tree as measured, for 32
    false alarms. Conservative is (5c), R2's note alone.

*******************************************************************************
