- [ ] **M-generics-library step 4** | `library_at` was `text.len()` for a file with no library, and every end-of-file diagnostic moved a line. Question: which span starts exactly at `text.len()`, and why did the sentinel have to become `u32::MAX` rather than the boundary being made exclusive?

    **Where to look:** archive/bootstrap-rs/heroes/src/source/mod.rs (`Source::new`) · the test `an_empty_variant_says_what_is_missing`
    **Why it matters:** an off-by-one in a sentinel, caught by a test about something else entirely
