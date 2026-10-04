- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-sized-integers step 8 | `fit_i8(-129)` was accepted, and the five adversarial cases found it. Read the repaired test construction in `emit/ops.rs` (the `from_low < low` / `from_high > high` pair). Question: state, in one sentence, what the ORIGINAL version tested and why it was right for `fit_u8` and wrong for `fit_i8`. Then: why is a bound *omitted* rather than emitted as a tautology — name the compiler flag that decides it

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/ops.rs (the `fit_` arm) · tests/golden/run/sized-integers.hero § "a narrowing can fail and says so"
    **Why it matters:** a silent truncation inside the function built to prevent silent truncations
