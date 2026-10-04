- [x] **054 — the specification states a balance the runtime stopped keeping** | § 13 says a handle consumed twice hides one never consumed; the counter became a set on 2026-09-15 and the program aborts naming the stray FIRST | `spec/heroes-spec.md:378-379` · `runtime/parts/alloc.c` · `tests/golden/run/abort-handle-given-back-twice.expected`

    **Origin:** 2026-09-17, panel 160's ffi-pragmatist, measured on real SQLite
    while answering a question about something else. The tenth correction to a
    coordinator's briefs across six sittings, and the first that is a sentence in
    the specification rather than a number in a brief.

    **The false sentence**, verbatim: *"The owing is counted, so a handle
    consumed twice hides one never consumed."* It entered on 2026-09-14. On
    2026-09-15 commit `2e7d221c` — *"the counter became a set"*, its own
    subject — replaced the count with a set of live addresses in
    `runtime/parts/alloc.c`, and the specification was not touched.

    **Measured (the seat's P3b, two platforms):** two successful opens, `dbs[0]`
    closed TWICE through two copies, `dbs[1]` never closed. Exit **134**, and the
    message is the SET's — *1 C handle(s) given back that were never taken … the
    set of live handles did not hold that address* — with the stray reported
    BEFORE the leak. `tests/golden/run/abort-handle-given-back-twice.expected`
    asserts exactly that, so the instrument already knows what the document does
    not.

    **What is owed.** The sentence, corrected to what the set does. The seat
    priced the merge that replaces it at **−11 vendored**; the real number is
    UNRUN and the rule of 2026-09-16 says a vendored delta is not a price. That
    removal would then be available to pay for a later addition.

    **CLOSED 2026-09-17.** The sentence now reads *"The live handles are a set,
    so giving one back twice aborts on its own."* Reproduced first, because the
    correction had to say what the runtime does: two connections, the first
    closed twice through two copies and the second never — exit **134**, the
    set's message, the double release named FIRST and the leak not reached.

    **+4 vendored and −2 real** (5993 to 5997; 7986 to **7984**; digest
    `6bdb9b497a141864`), and that is the row worth keeping: **the two
    instruments disagree on the SIGN**, which has not happened in seventy-nine
    ledger rows. A session that priced this offline would have written *costs
    four* of a change that gives two back, and no factor can bridge that.
    Ledger row 80 carries it.
