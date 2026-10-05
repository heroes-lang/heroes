- [ ] **M-module-namespace step 2** | **Two lexer tests failed on a newline nobody typed.** Normalising every file to end in `\n` moved EOF from `1:6` to `2:1` on a file written without a trailing newline. Question: why does the separator have to be closed *before* the next file rather than after the previous one, and which file in a compilation is the only one that may keep exactly what was written?

    **Where to look:** archive/bootstrap-rs/heroes/src/source/mod.rs (`Source::of`) · archive/bootstrap-rs/heroes/src/lexer/tests/end_to_end.rs
    **Why it matters:** the fix is one line moved, and the test that caught it is about a language with no modules
