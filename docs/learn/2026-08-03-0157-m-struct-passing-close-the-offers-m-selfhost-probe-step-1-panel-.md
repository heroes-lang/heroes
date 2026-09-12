- [ ] **M-struct-passing close — the offers** | M-selfhost-probe step 1 (panel 065) | The double-emit determinism test emits the same program twice with one binary and diffs the two files. Which of these regressions does it catch? (a) the emitter starts ordering `_desc` definitions by insertion instead of TyId, deterministically; (b) the emitted C mentions the output path; (c) a map with a random seed enters the emitter. One of the three passes it silently

    **Where to look:** archive/bootstrap-rs/heroes-cli/tests/golden.rs (the two tests at :520 and :555) · docs/panel/065
    **Why it matters:** the difference between *stability* and *order* is what the port's 13 sort obligations rest on, and the sitting found two orders no artifact pinned
