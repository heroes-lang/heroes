- [ ] **M-declared-extents walkthrough** | Defect 065: a `=` binding's field was written by C, and the repair added **no rule to the checker** about who may write. Before reading: the lend now crosses as `const void *` from a `=` binding and `void *` from a `@` cell, and the compiler ships `-Werror=incompatible-pointer-types-discards-qualifiers`. **Which of the four C crossings is a hard error under that flag — `const void *` into `void *`, `void *` into `const void *`, `void *` into `void *`, `const void *` into `const void *`?** | `selfhost/emit/field_lend.hero` § WHAT THE CAST IS

    **Where to look after answering:** exactly one — `const void *` into
    `void *`. The other three compile clean, which is what lets a `=` binding
    still be lent to a function that only reads, and a `@` cell to anything.
    The question the language cannot answer from the binding (*does C write?*)
    is answered by clang from the header, per call, and no checker rule learns
    it.

    **The question to carry away.** The route was implemented once and
    reverted the same afternoon, on a line no seat had named. Ask WHERE the
    qualifier died on that first landing — and why declaring the temporary by
    its Heroes type was enough to make the honest program the refused one.
