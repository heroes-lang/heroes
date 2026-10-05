- [ ] **M-deferral-ledger 3** | A proposal made `for x in v` call a module's `function next(@it: T) -> E?`. It reads harmlessly. Say which written rule of this language it contradicts, find the diagnostic the compiler already raises for the neighbouring shape, and say what the rule is protecting — then say why that objection outranked a different seat's veto on the same proposal.

    **Where to look:** design.md §4.8's sentence about UFCS and `@`;
    `selfhost/check/walk.hero`'s `ufcs_on_mutable` and the comment above it;
    spec § 9 on mutable parameters being marked at the call site too; and
    § Where they disagree in
    `docs/panel/137-the-hole-was-two-operations-wide-and-the-answer-was-a-library-function.md`.

    **Why it matters:** two seats objected to the same proposal for two different
    reasons, and only one of them could be repaired by adding sentences. A silence
    in a proposal is closed by writing more; a contradiction with an existing rule
    is not, and telling those apart is what decided the verdict. The rule in
    question is small and old and exists so that a reader can see, on the line,
    that a call can change what they passed it.
