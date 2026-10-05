- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-sized-integers step 3 | Three blanket renames crossed a language boundary in one session: `: int` into English prose, `"int"` into a Rust table key, and Heroes' `int` into C's inside `_Generic`. Question: for each, name the test that caught it and say how long it would have survived without that test. Then the general one: this repository holds four languages in overlapping files — what property would a rename tool need to be safe here, and does `\bint\b` have it

    **Where to look:** docs/records/journal/015 § What broke and why · git show 3280ffe · git show 04fc16a
    **Why it matters:** the method is sound and the tool was not, which is a distinction worth being able to make
