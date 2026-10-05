- [ ] **M-deferral-ledger 4** | Two ways to choose a variant's constructor at a call site: pass a function that builds it, or `match` on a tag and build it in each arm. Both compile to the same thing. Say which one catches a new variant case being added, which one does not, and why the difference is a property of the language rather than of the programmer's care.

    **Where to look:** the compiled pair in the sitting,
    `docs/panel/138-the-item-named-a-parser-that-never-existed-and-the-route-was-already-in-the-grammar.md`
    § The resolution, point 4; `spec/heroes-spec.md` § 8 on exhaustiveness and the
    forbidden `_` arm on a variant; and the `non_exhaustive` diagnostic, which you
    can provoke in two lines.

    **Why it matters:** the feature under judgement would have made the shorter
    spelling the natural one, and the shorter spelling is the one that goes quiet
    when the variant grows. That is the opposite of what this language is for. It
    is also a reminder that *fewer characters* and *harder to get wrong* are
    different axes, and that a proposal measured only on the first will look good.
