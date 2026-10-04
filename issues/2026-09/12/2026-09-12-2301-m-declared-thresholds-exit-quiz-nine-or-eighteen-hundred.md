- [ ] **M-declared-thresholds exit quiz** | `heroes measure CLAUDE.md` printed "Headroom: 9" about a document that was 1806 tokens over its ceiling. Both numbers were computed correctly. Say what each one was measuring, and name the earlier occasion on which this project made the identical mistake.

    **Where to look:** `selfhost/cli/measure.hero`'s `CONTRACT_CEILING` comment,
    `docs/measurements/023-the-instrument-was-not-the-readers.md`, and
    design.md §1.6's paragraph beginning *"What the 4096 was"*.

    **Why it matters:** nothing was broken. The tool did its arithmetic, the
    check passed, and the number printed was true of the thing it was a count of.
    What was wrong is which thing that was — and a wrong instrument is invisible
    precisely because it keeps answering. The second half of the question is the
    half worth sitting with: the repository had already paid 998 tokens for this
    exact lesson, had written the measurement down, and still did not carry it
    across to the second document a ceiling judges.
