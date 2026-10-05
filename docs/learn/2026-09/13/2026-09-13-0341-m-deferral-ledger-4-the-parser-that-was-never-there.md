- [ ] **M-deferral-ledger 4** | design.md justified a language feature by saying *"the parser's `term` and `expression` functions are identical except for two names"*. Find those two functions. Say which parser they are in, what the compiler's own expression parser does instead, and — the part worth sitting with — what the timestamps say about when each was written.

    **Where to look:** `examples/calculator/parse.hero:84-107`;
    `selfhost/grammar_expr.hero`'s `binary(@c, @a, text, min_power)` and the
    comment above it; `git log --format='%h %ad %s' --date=iso --reverse --all -S "min_power"`;
    and design.md Part 7 item 9 with the correction beneath it.

    **Why it matters:** the item was written on 2026-08-04 and the parser it
    describes was replaced by precedence climbing at 04:10 that same day, in the
    commit that first built an expression parser at all. So a sentence justifying a
    feature outlived its subject by thirteen months while reading as a
    measurement. Three sittings of this milestone have now found the same shape,
    and the question is not how it happened but what kind of sentence can do it.
