- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-syntax-tree.1 | A `#` comment, then a blank line, then a declaration: does the comment become documentation — and which two comparisons decide it? **Answered in the code, so this is a reading and not a decision**: `take_docs` breaks on `line + 1 != wanted || col != decl_col`, so a blank line ends the run and the comment documents nothing

    **Where to look:** syntax/cursor.rs take_docs:206-232 (§4.1)
    **Why it matters:** Go's adjacency rule is two integer comparisons, not a parser mode
