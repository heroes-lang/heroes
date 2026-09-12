- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-program-corpus | **Exit quiz**, four closed questions: (a) `n + left & 1` — what does it parse as, and is that the same as C? (b) `mask & bit != 0` — same two questions. (c) A `[str]` in a `for` loop accumulated with `push`: what is its complexity, and what does the spec say about it? (d) Which of the nine corpus programs never calls `exit`, and what does that buy on Darwin arm64?

    **Where to look:** examples/logs/mask.hero · examples/adventure/ · spec § Strings, arrays, maps
    **Why it matters:** two of these were the corpus's own failing assertions, and the language was right both times
