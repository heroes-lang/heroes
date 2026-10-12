---
kind: task
area: records
milestone: none
filed: 2026-08-12
commit: ec5558bef7a0e7934ec69fce6dab1b6d45dcb362
github: https://github.com/heroes-lang/heroes/issues/342
---

- [x] Covered | sweep 001 | **CLOSED — `line_col` is `pub(crate)` and `line_of` answers the question twenty callers actually had, so the CLI can no longer assemble a location by hand.** **The repair `source/mod.rs` actually needs**: `line_col` returning a concatenated-text line has no legitimate consumer outside `locate` and the printers' relative arithmetic, yet eight of the twenty defects are callers that used it anyway — under a doc comment that has said "there is one function and no caller assembles the triple itself" since M8a. Decide: make it private, or accept that the rule is advisory | crates/heroes/src/source/mod.rs:229 · docs/defects/001 § Shape B | an invariant enforced by convention is enforced by nothing, demonstrated eight times

## What was done

`line_col` became `pub(crate)` and `line_of` answers the question twenty callers actually had, so the CLI can no longer assemble a location by hand; closed under sweep 001. Landed at `ec5558be`, "Panel 035: thirteen silences closed, and the spec got smaller".
