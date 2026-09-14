- [ ] **M-cleanup-verdict walkthrough** | Read `selfhost/ir/own.hero`'s six rules, then build any program with a `str?` in it and read the emitted C for one function. Find the `_retain` and the `_release` and say which of the six rules put each one there. Then find the **two** blocks a `?` produces and confirm the release appears in both. **Then do the same for a program holding a C handle, and say what is different — before reading the answer below.**

    **Where to look:** `selfhost/ir/own.hero` lines 10-45 (the six rules, each
    with its compiled counterexample); `./heroes build <file> --emit-c -o out`;
    `docs/measurements/030` § 4.

    **Why it matters:** this is the machinery the whole milestone turned on, and
    it is easier to read than its reputation. Rule 5 is the one to sit with —
    *ownership lives in slots, and cleanup is a walk over a table* — and its
    comment records that the obvious design was tried and refuted **within the
    hour**: releasing at the end of the defining block fails because `.must()`,
    `?`, `&&` and if-as-value all open a block in the **middle** of an
    expression. That failure is why the sweep is uniform, and the uniformity is
    why a `?` cannot leak a `str`.

    **The answer, for after you have looked:** the handle gets `_eq` and `_hash`
    and **nothing else**. No `_retain`, no `_release`. The sweep is not skipping
    it out of caution — it has nothing to call. That single observation is what
    narrowed the sitting's question from *should this language have `defer`* to
    *can a handle be given something for the sweep to call*, and the answer to
    THAT turned out to be no, for a reason the sweep has nothing to do with.

    **The question to carry away:** rule 3 says a store increfs the new value
    **before** decrefing the old, and the comment says `s @ s` otherwise frees
    the buffer and increfs a dead one. **A handle has no count.** So ask: which
    of the six rules still mean anything for a value that cannot be counted, and
    what does that tell you about reusing this machinery for one?
