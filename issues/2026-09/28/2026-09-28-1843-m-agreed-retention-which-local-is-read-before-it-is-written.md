- [ ] **M-agreed-retention exit quiz** | The seed emitted `= {0}` on 106,734 locals; after panel 182 it emits 21,566. The emitted C has two kinds of local: a VALUE, an IR temporary defined once, and a SLOT, a variable a store writes and rewrites. **Before reading: which of the two can be read before its first write, by what operation, and why is the zero on the other kind not a safety net but a blindfold?** | `selfhost/emit/body.hero` (its header) · `selfhost/ir/values.hero` (`in_order`) · design.md Part 5

    **Where to look after answering:** the slot. Every store into a
    refcounted slot loads the old value first, to release it, so the first
    store reads the slot before anything wrote it, and the zero is what that
    read finds. A value's one definition precedes every read on every path,
    which the verifier proves across blocks and, since this milestone, within
    one (`in_order`, with a test that makes it fire). So a zero on a value is
    never read, and it hides the one thing that would matter: with every
    remaining zero stripped, clang's uninitialised-read warnings flag all
    21,566 locals and every one is a slot, while a value zeroed by default
    could never be flagged at all.

    **The question to carry away.** The sitting's critic found the one place
    the emitter wrote a value in parts, `map_get`'s found path, and the suite
    `wholes` now pins zero such writes. Say why a value written field by field
    breaks the rule above even though its first field is written before any
    read.
