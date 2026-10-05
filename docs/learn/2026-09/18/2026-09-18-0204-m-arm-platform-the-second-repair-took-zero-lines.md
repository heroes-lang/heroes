- [ ] **M-arm-platform exit quiz** | Two defects were filed apart because their repairs looked different: one a language question, one a table row in `emit/c_spellings.hero`. **Before reading: predict how many lines the second repair took.** | `docs/records/done/2026-09-18-0130-defects-058-and-059-close-on-one-string.md`

    Defect 059 was this, on the machine where `char` is unsigned:

    ```
    error[ffi_parameter_type]: ...
      note: ... Declare it `i8`, and convert at the call where the
            value is known to fit
    ```

    The compiler refused `i8` and its own note told the author to write `i8`.

    **Where to look after answering:** `selfhost/emit/c_spellings.hero`'s header
    comment, which records having learned this exact lesson one type over: *"A
    width is not a constant to be tabled; it is a question about the target."*

    **Why it matters.** The obvious repair was to make the `char` row ask the
    target for the sign, exactly as the file already asks for `long`'s width.
    The answer is **zero lines**: the row was never wrong code, it was a sentence
    that was false on one leg, and the first defect's flag makes it true on four.

    **The question to carry away.** Both entries described their own defect
    accurately and both were wrong about where it lived. Ask what that says about
    filing two defects rather than one, and what would have had to be written in
    either entry for the single repair to have been visible from it.
