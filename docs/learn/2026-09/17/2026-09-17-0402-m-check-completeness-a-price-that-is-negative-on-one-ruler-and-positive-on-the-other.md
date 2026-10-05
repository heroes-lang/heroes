- [ ] **M-check-completeness exit quiz** | The specification is measured by two tokenisers. One sentence was replaced by another, and the two rulers disagreed about which direction the document moved. **Before reading: predict the sign on each, then say what that does to any rule of thumb for converting between them.** | `docs/measurements/010-spec-budget-ledger.md` rows 79 and 80

    The sentence that left:

    > The owing is counted, so a handle consumed twice hides one never consumed.

    and the one that replaced it:

    > The live handles are a set, so giving one back twice aborts on its own.

    Seventy-nine ledger rows before this one, the real delta was always LARGER
    than the vendored delta and always in the same direction — ratios from 1.00
    to 1.60.

    **Where to look after answering:** row 80's own paragraph, then row 79 above
    it for the contrast, and then `docs/measurements/010`'s opening, which
    refuses any single conversion factor and had until now only arithmetic to
    argue with.

    **Why it matters.** The measurement is **+4 vendored and −2 real**. A
    session that priced it offline would have written *this costs four* about a
    change that gives two back — and no factor, however carefully fitted to
    seventy-nine rows, produces a negative from a positive. The author's
    instruction of the previous day, *always measure with the real*, stops being
    a caution about magnitude and becomes a rule about direction.

    **The question to carry away.** Ask why the sentence was wrong in the first
    place: the runtime changed its bookkeeping from a count to a set on
    2026-09-15, the golden that asserts the new message was updated the same
    day, and the document was not. Then ask which of the three — runtime, golden,
    document — an instrument was watching, and which one nobody was.
