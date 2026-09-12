- [ ] **M-generics-library step 4** | **The library is APPENDED, never prepended, and one line of arithmetic is the whole reason.** Every span is a byte offset into one text and every message renders a line number from it. Question: what would prepending have done to the line number in every diagnostic in every program — and what does appending cost instead, at the one boundary where it shows?

    **Where to look:** archive/bootstrap-rs/heroes/src/library/mod.rs · archive/bootstrap-rs/heroes/src/source/mod.rs (`with_library`)
    **Why it matters:** the cheap choice and the correct one differ by which end
