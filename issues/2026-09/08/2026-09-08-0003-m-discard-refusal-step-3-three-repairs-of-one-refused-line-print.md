- [ ] **M-discard-refusal step 3** | Three repairs of one refused line print three different answers. Predict them before you look, then say which of the three a `certain` fix could ever have been | `docs/panel/118` R3 · `selfhost/value_errors.hero` `discarded_failure` | this is the whole argument for shipping no automatic repair, and it is two commands long

    **Origin:** M-discard-refusal step 3, 2026-09-08. The program is four lines:
    `risky` fails on a negative, `run()` returns `i64?` and contains the
    refused line, `main` prints `run().default(1)`. Write the three repairs —
    `_ = risky(0 - 1)?`, `_ = risky(0 - 1).is_err()`, `_ = risky(0 - 1).must()`
    — and predict each printed answer. Then run them.
    **The question after:** one of the three preserves the program's meaning
    exactly. It is also the one that preserves the bug. Say what that means for
    what `heroes check --apply` is allowed to do.
