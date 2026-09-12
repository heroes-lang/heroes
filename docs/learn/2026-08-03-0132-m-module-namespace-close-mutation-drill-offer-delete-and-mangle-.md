- [ ] **M-module-namespace close** | **Mutation drill offer**: delete `FileEntry.component` and mangle with `module` again, or make `at_span` use `line_col`, and predict which instrument fires — clang, the leak counter, a golden, a surface test, or nothing. The milestone's own record says the answer differs per line, and for one of the two the answer was *nothing* for a whole day

    **Where to look:** archive/bootstrap-rs/heroes/src/source/files.rs · archive/bootstrap-rs/heroes/src/emit/writer.rs
    **Why it matters:** both were live in shipped code and neither had a test until a judge compiled them
