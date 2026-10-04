- [ ] **M-module-namespace defects** | **`at_source` and `at_library` were the right split for two files and the wrong one for four.** In the split calculator the emitted `#line` reached 410 while `main.hero` is 78 lines. Task: say what M-ffi-ladder loses when §4.19's guarantee is delivered to a file that does not contain the declaration, then count how many callers `Source::locate` has now and what each was doing before it existed

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/writer.rs · archive/bootstrap-rs/heroes/src/source/mod.rs
    **Why it matters:** the third caller in one milestone to need the same three values, and the first two were found by running rather than by testing
