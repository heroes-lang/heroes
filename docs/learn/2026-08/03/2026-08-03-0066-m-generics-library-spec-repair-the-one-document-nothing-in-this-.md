- [ ] **M-generics-library spec repair** | **The one document nothing in this project reads.** Symptom: a program written from the committed spec answered `error[unknown_name]: nothing named `has` is in scope`, exit 1, four days after `has` was struck. Task: say which of the three artifacts (spec, design.md, the compiler) was wrong under CLAUDE.md §12's precedence rule — and why the answer is not the one the rule's first sentence gives

    **Where to look:** spec line 128 · docs/panel/026 R7 · archive/bootstrap-rs/heroes/src/resolve/tests/spec.rs
    **Why it matters:** the removal measured −17 and shipped −4, and the missing 13 were a sentence in a different section
