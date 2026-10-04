- [ ] **before M-ir-lowering** | Hand-desugar `for x in xs`, `?`, and one UFCS chain from the appendix calculator — **against the IR text** (panel 019 re-specified this: there is no desugared tree to compare against, so the exercise's answer key is `heroes build --dump-ir`)

    **Where to look:** design.md Part 5 · tests/golden/ir/
    **Why it matters:** the desugarer landed in M-ir-lowering and the dump is the only evidence the table was honoured
