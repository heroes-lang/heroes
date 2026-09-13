- [ ] **M-deferral-ledger 6** | On 2026-08-12 a sitting deferred a feature partly because *"one whole-program translation unit gives visibility no linkage consequence"*. Find the milestone that made that sentence false, say how many days later it landed, and then find the reason the conclusion survives anyway. Finally: compile a two-module program and look at the emitted C for a helper nobody calls outside its own file.

    **Where to look:** `docs/ROADMAP.md`'s chain row for M-separate-compilation;
    `selfhost/emit.hero` around the translation-unit count and `reach_of`;
    `selfhost/emit/decls.hero`'s `HERO_TU_LOCAL`; and the correction beneath
    design.md Part 7 item 14.

    **Why it matters:** the sentence was true when written and false fourteen days
    later, and nothing in the repository noticed for a year. That is the same shape
    as the doctest clock and the `raw` module's preconditions, three times in one
    milestone. The interesting half is the second question: the conclusion was
    right for a reason nobody had stated, which is why re-deriving a verdict is
    not the same as repeating it.
