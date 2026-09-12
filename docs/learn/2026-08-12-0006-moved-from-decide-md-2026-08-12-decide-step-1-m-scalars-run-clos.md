- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-scalars-run close | Milestone debrief offers, all optional: walkthrough (one function from `.hero` through the IR to the C to the binary, following the `#line` directives and the copy-out) · ratify the 5 adversarial `run/` cases (overflow-aborts, division-edges, short-circuit, copy-out-edges, join-slot) · mutation drill (delete the `preds.is_empty()` test in `emit/decls.rs::reachable` and predict which of the 55 goldens fail — the answer is interesting) · exit-quiz (hand-emit the C for a `while` loop, then diff against `--emit-c`)

    **Where to look:** /learn
    **Why it matters:** the author's call, when and how much
