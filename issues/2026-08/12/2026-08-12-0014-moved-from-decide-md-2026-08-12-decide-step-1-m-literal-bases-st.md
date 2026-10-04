- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-literal-bases step 1 | Count, then explain: run `heroes mutate` over a directory holding `constant MASK: int` / `0xff` and `constant WIDE: int` / `0b1010`. How many `typo-digit` mutants, and how many would the pre-milestone `neighbouring_digit` have produced? Then say which *character* decided the difference, and why nothing in the test suite would have gone red

    **Where to look:** archive/bootstrap-rs/heroes/src/mutate/edits.rs (`neighbouring_digit`) · docs/records/journal/014-literal-bases.md § What surprised
    **Why it matters:** this is CLAUDE.md §11's expired premise caught one milestone before it expired, which is the only time the fix is cheap
