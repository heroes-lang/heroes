- [ ] **M-discard-refusal step 2** | Two repairs for one clang warning: one deletes the assignment, one keeps it and says the silence is deliberate. Both work on the shape that provoked them. Say which shapes separate them | `selfhost/emit/body.hero` · `selfhost/emit/unread.hero` `may_lose_its_destination` | the first was built, measured clean, and thrown away, and the reason is a runtime call carrying a check

    **Origin:** M-discard-refusal step 2, 2026-09-08. `_ = a == b` on two
    integers, on two `str`, and on two arrays. For each, say what the emitter
    prints and whether dropping the assignment would delete anything worth
    keeping. `runtime/parts/array.c`'s `hero_array_require` and
    `selfhost/emit/structural.hero`'s deliberate `hero_panic` are the answer's
    two halves.
