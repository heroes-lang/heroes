- [ ] **M-cleanup-verdict golden ratification** | Read `docs/measurements/030` down to *What this measurement does NOT say*, and **stop there**. Write down which of its numbers you would check first if you had one hour and had to hand it to five judges as fact. Then read the correction underneath and see whether you picked any of the four.

    **Where to look:** `docs/measurements/030-three-release-obligations-and-only-one-of-them-is-silent.md`, both halves; then
    `docs/panel/147-reports/completeness-critic.md`, which is the audit that
    found them.

    **Why it matters:** that document was written in a morning and handed to
    five judges as the factual base of a sitting, and **nobody audited it** —
    the same shape as the defect it was written alongside, where one golden test
    asserted the single value at which a broken answer and a right one coincide.
    Four numbers were wrong. Three are small: 23 paths in two files is 22 in
    one, twelve pairs is at least nineteen, and *10 of 21* is 8 because 10 + 10
    leaves one slot for three programs.

    **The fourth is the one to sit with.** Every one of those 22 paths ends in
    `exit(1)` or `abort` — the program closes and leaves. So **no shipped
    program leaks a handle and then goes on running**, and the headline *exit 0,
    in silence* was measured on a reduction written for the purpose. The class
    is real; the urgency was not what the headline claimed.

    **Two questions to carry away.** First: the correction weakens the argument
    the document was written to support, and it is written **underneath** rather
    than over it. Say what would be lost if it had been written over it — and
    why this project would rather keep a wrong sentence with a date on it.
    Second, and harder: the enumeration was **short by seven**. Ask what the
    difference is between counting by reading twelve function bodies and
    counting from the tree with a command, and why this project calls the list
    itself a measurement.
