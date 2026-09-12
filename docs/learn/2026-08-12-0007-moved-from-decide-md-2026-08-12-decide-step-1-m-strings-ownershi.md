- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-strings-ownership close | Milestone debrief offers, all optional: walkthrough (one `str` from literal to heap block to sweep, following `--dump-ir` then `--emit-c`) · ratify the 2 new adversarial cases (str-copy-out, str-self-assign) · mutation drill (swap the incref and decref around a store in `own.rs` and predict which of the 68 goldens fail — and which configuration catches it) · exit-quiz (hand-write the refcount instructions for `out @ out + to_str(i)` inside a loop)

    **Where to look:** /learn
    **Why it matters:** the author's call, when and how much
