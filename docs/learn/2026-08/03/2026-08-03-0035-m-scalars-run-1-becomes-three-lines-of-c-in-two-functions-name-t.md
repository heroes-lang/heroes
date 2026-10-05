- [ ] **M-scalars-run.1** | `bump(@v)` becomes three lines of C in two functions. Name them, and say which one runs on the error side of a `?`

    **Where to look:** emit/decls.rs (prologue) · emit/inst.rs (CopyOut) · design.md §3.1's `@` bullet
    **Why it matters:** §4.8's "copy-out happens always" is now a calling convention, and M-strings-ownership/M-value-aggregates/M-generics-library inherit it
