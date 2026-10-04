- [ ] **panel 139** | Eight lines: make a record holding a pointer, copy it, free through the copy, write through the original's own field. Say what the compiler says, what the program prints, what the sanitizer says, and then find the two functions in the shipped SQLite example that are exactly this shape. Then answer the harder half: the function that frees takes its argument WITHOUT the mutable marker — what does the specification promise about that, and who is the promise made to?

    **Where to look:** `docs/work/DEFECTS.md` item 031 and its reproducer;
    `examples/ledger/db/sqlite.hero:225` and `:270`; `spec/heroes-spec.md` § 9 on
    mutable parameters and § 13's *"Unmarked pointers are never freed"*; and
    design.md's borrow-checker row with the correction landed beneath it.

    **Why it matters:** the language's argument against a borrow checker was that
    value semantics removes aliasing, so there is nothing to check. That is true
    of everything Heroes owns and false of every address it borrows, and the
    second half is where every real program lives, because there is no standard
    library. The row still stands — but on cost and on the founding constraint,
    not on *there is nothing to check*, and the difference is what a reader has to
    understand before trusting a signature.
