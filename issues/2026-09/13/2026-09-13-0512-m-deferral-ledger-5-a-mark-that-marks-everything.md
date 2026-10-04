- [ ] **M-deferral-ledger 5** | The proposal was that a file becomes dangerous only by writing `use raw` at the top, so a reader knows from one line whether to be careful. Count how many files in this tree would carry that line, and say what the count does to the claim. Then find the two places where the compiler already marks a dangerous type, look at what they key on instead of an import, and name the one shape they catch that a file-level mark cannot.

    **Where to look:** the counts in
    `docs/panel/140-the-document-had-already-ruled-against-part-9-and-nobody-told-part-9.md`
    § The other three facts; `selfhost/ffi_errors.hero:281` and
    `selfhost/emit/gate.hero:255`; and
    `tests/golden/surface-fixtures/qualifyptr/window.hero:9`, which is a pointer in
    a signature in a file with no `extern` group anywhere in it.

    **Why it matters:** a warning that fires on everything is not a warning. The
    language had already solved this the other way round — by declaration rather
    than by import — and the solution is stronger for a reason nobody set out to
    achieve: a `use` line can be renamed with `as`, and a diagnostic keyed on a
    declaration cannot be renamed at all.
