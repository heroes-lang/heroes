- [ ] **M-ir-lowering.2** | The verifier had 257 lines of checks and no test that any of them fired. Fifteen tests now break one invariant each. Pick two and say what the *wrong* IR would have done at M-scalars-run, in C

    **Where to look:** ir/tests/verify.rs · docs/records/journal/005-lowering.md §3
    **Why it matters:** LLVM keeps a directory for exactly this; a safety net nobody has fallen into is indistinguishable from no net
