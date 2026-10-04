- [ ] **M-deferral-ledger 3** | A generic function in Heroes can already compare two values of its type parameter with `==`, and can already use one as a map key — but it cannot write `a < b` and cannot hand one to `print`. Say what makes the first two work with no trait and no constraint, name the pass that decides it, and say why exactly those two operations are in and the other two are out.

    **Where to look:** design.md Part 5's type-descriptor pass and what it
    generates per reachable type; `selfhost/emit/desc*` and `runtime/parts/desc.c`;
    the compiler seat's eight compiled cases in
    `docs/panel/137-the-hole-was-two-operations-wide-and-the-answer-was-a-library-function.md`;
    and spec § 11's sentence about `sort`.

    **Why it matters:** the whole trait question turned on this. A feature that
    looked like it was missing from the language was missing from one table
    instead — the descriptor set has `copy`, `drop`, `eq` and `hash`, and simply
    has no `ord` and no `show`. Seeing that the boundary between *the language
    cannot express this* and *one pass does not generate this* runs right through
    the middle of a proposal is what turned a 900-line feature into a 20-line
    library function.
