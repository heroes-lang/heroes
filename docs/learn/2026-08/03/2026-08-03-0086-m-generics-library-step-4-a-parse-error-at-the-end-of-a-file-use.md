- [ ] **M-generics-library step 4** | A parse error at the end of a file used to point at EOF; with the library appended that became "the library's first line", and the pipeline refused the whole compilation as a compiler bug. Task: read `Cursor::here_or` and say why the fallback is the declaration's keyword rather than the previous token

    **Where to look:** archive/bootstrap-rs/heroes/src/syntax/cursor.rs · tests/golden/check/missing-body.hero
    **Why it matters:** the guard was right to fire and the diagnostic was wrong to be there
