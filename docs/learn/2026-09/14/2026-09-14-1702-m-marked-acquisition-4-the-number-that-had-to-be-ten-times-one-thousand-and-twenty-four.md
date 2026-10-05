- [ ] **M-marked-acquisition 4** | The spec ceiling moved to "10k" on 2026-09-14 by your own decision. It landed as **10240** and not **10000**. **Before looking: say which files would have to agree for either number to work, and which one would break.** Then find the line that settles it.

    **Where to look:** `site/src/lib/claims.ts`, the function `ceilingK`;
    `site/src/lib/tables.ts`, where the site gets its keyword set;
    `tests/harness/suite_spec.hero`'s `constant CEILING`.

    **Why it matters:** the site does **not** restate the ceiling. It reads
    `constant CEILING: i64` out of the test suite and renders it in K, and
    `ceilingK` throws if the number is not a whole multiple of 1024 — *a claim
    that rounds would be a claim the tree does not make*. So 10000 does not
    produce a slightly-wrong page; it **stops the site's build**, by design,
    written by somebody who decided that a number a reader sees must be a number
    the tree actually holds.

    **The general shape.** There are two ways to keep a number consistent across
    a repository: repeat it and check the copies, or **derive it and have no
    copies**. This tree does both, in different places, and the difference
    showed today: the ceiling moved in six documents by hand — the design
    document, the README, a subagent's definition — and in the site by nothing
    at all, because the site had no copy to move.

    **The question to carry away**, and it is a design question rather than a
    trivia one: `SPEC_TOKENS` and `SPEC_REAL_TOKENS` are deliberately kept in
    **two** places each, one printed and one checked, with a suite that fails
    when they disagree. The ceiling could have been derived everywhere instead.
    **Say what the repeated-and-checked arrangement buys that deriving does
    not** — the answer is in `defect 017`'s class, and in what a number is FOR
    when a human is about to amend the document it governs.
