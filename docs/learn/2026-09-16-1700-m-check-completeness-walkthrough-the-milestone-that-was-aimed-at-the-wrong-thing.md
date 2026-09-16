- [ ] **M-check-completeness walkthrough** | This milestone is named *what `heroes check` accepts, `heroes build` compiles — through a generic too*, and it spent five panel seats on generics. **Before reading anything: open `docs/work/milestones/M-check-completeness.md` and, for each of its four named faces, write down what `check` and `build` each return.** Then check them against the measurements. | `docs/work/milestones/M-check-completeness.md:10-19` · `docs/panel/155-the-hole-was-never-made-by-the-generic.md`

    **Where to look, in this order:** the milestone file's opening, which states
    the four faces; then panel 155's § *THE FINDING* and its R4; then defect 046
    in `docs/records/done/`.

    **Why it matters.** Only ONE of the four was ever a `check`/`build` gap, and
    it had been closed for a month by a sitting held the same day as the one that
    named it. For the two the milestone actually worked on, `check` and `build`
    both return **0** — they agree — and the divergence is at *run*. So the
    milestone's own name did not describe the work it was doing, and nobody
    noticed until a seat read the file it had been briefed from.

    **The question to carry away** is not about generics. Both surviving faces
    turned out to be reachable with **no generic anywhere**, by nesting records
    sixteen deep, because two static walks abandoned past a bound. Ask what made
    a whole milestone aim at generics: the milestone file names the generic in
    every one of its four sentences, and every one of those sentences is a true
    statement about a witness. Then ask what the file would have had to say
    instead for the bound to be the obvious suspect — and whether *the shape that
    provoked the repair* is ever the right place to stop looking.
