- [ ] **M-deferral-ledger 2** | The whole repository contains exactly one markdown fence inside a comment, at `selfhost/parse/group.hero:33-36`. Open it and say why it could never be a doctest, using two different rules: one from the specification's grammar, and one from the module it sits in. Then say what that single instance tells you about what fences in THIS project are for, and check the claim against the fences in the specification itself.

    **Where to look:** `selfhost/parse/group.hero:33-36` and the declaration it
    documents at `:52`; `spec/heroes-spec.md` § 4's `Declaration` production and
    § 5's `Statement` production; the first line of every fenced block in
    `spec/heroes-spec.md`; and design.md §4.19's own illustration.

    **Why it matters:** three seats of panel 136 reached this file independently,
    by three routes — one ran the fence's body through the compiler and read the
    diagnostic, one ran it and then found a second failure one pass later, and one
    never ran anything and derived it from the grammar. A proposal was refuted by
    the single instance of the thing it proposed to govern, which is what
    CLAUDE.md § RUN IT means by attacking a repair at the shapes next to the one
    that provoked it.
