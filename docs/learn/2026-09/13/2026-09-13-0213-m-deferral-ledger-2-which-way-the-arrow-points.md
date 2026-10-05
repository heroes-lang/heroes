- [ ] **M-deferral-ledger 2** | design.md Part 6 refuses code inside a markdown document and says the documentation generator *"goes one way only"*. A doctest puts code inside a comment inside code. Say which direction that is, why the answer is not obvious, and what Go and D do differently that gets the same benefit — documentation that cannot lie — while pointing the arrow the way this project already chose.

    **Where to look:** design.md Part 6's literate-source row; §4.18's doctest
    paragraph and its correction; the historian's row in
    `docs/panel/136-the-item-named-three-ancestors-and-the-two-it-needed-were-elsewhere.md`;
    and the chain row for M-doc-generator, which is where the form that returns
    was homed.

    **Why it matters:** the item had named Rust, Python and Elixir for eleven
    months, and two of those three do not say what it thought they said — Elixir's
    doctests are not a separate mechanism at all. The seat that checked found the
    two languages that solve the problem in this project's own direction, and
    neither was on the list. A precedent nobody verifies is a decision resting on
    a guess, which is why that seat is required to cite a source for every claim.
