- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-ir-lowering close | Milestone debrief offers, all optional: walkthrough (one function from source to dump, following the four blocks of a `for` and the copy-out chain) · ratify the 5 adversarial IR goldens (continue-steps, try-copies-out, diverging-arms, nested-shortcircuit, for-evaluates-once) · mutation drill (change `continue_to: step` to `continue_to: test` in control.rs and predict which tests fail — the answer is interesting) · exit-quiz (hand-lower `for x in xs` or `e?` on paper, then diff against `--dump-ir`)

    **Where to look:** /learn
    **Why it matters:** the author's call, when and how much
