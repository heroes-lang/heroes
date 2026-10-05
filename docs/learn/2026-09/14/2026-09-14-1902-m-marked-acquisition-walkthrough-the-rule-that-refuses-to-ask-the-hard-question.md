- [ ] **M-marked-acquisition walkthrough** | Read `selfhost/check/acquiring.hero` end to end — it is 126 lines. Then answer, from the code and not from the comments: **what question does this rule NOT ask, and what would it have cost to ask it?**

    **Where to look:** `selfhost/check/acquiring.hero`; then
    `selfhost/check/leasing.hero:29` and `selfhost/check/consuming.hero:22`,
    which both state the same thing about the checker; then
    `runtime/parts/alloc.c`'s fourth exit check.

    **Why it matters:** the obvious rule to want is *did the program release
    this handle on every path out of the scope?* That is a **flow** question,
    and the checker says twice, in two files, that it has no flow analysis. So
    this rule asks a different question that happens to be decidable from
    declarations alone — **is this binding finished?** — and the flow question
    is answered somewhere else entirely, by a counter, at exit.

    **Two instruments, two questions, and neither pretends to be the other.**
    The counter cannot say *you forgot to mark `sqlite3_next_stmt`*; the rule
    cannot say *you leaked one on the error path*. Splitting them is what made
    the compile-time half 126 lines instead of a milestone of its own.

    **The harder half of the walkthrough** is the key. The rule is keyed on the
    handle TYPE, not on the `extern` group it was written in — and panel 148's
    compiler seat priced a group-keyed version at ~90 lines over a key that does
    not exist after the parser, which is true and reproduces. Find
    `check/decls.hero`'s `one_tag_one_type` and read its comment. **Then say why
    the type key works where the group key does not**, and what property of the
    language makes it well defined.
