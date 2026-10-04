- [ ] **M-scalars-run.1** | First C. `print((2 + 3) * 4)` becomes how many lines of C, counting neither declarations nor `#line` directives — and which of them can stop the program?

    **Where to look:** tests/golden/emit/scalars.expected · archive/bootstrap-rs/heroes/src/emit/inst.rs
    **Why it matters:** the emitter is a printer, and reading its output is reading what clang will see
