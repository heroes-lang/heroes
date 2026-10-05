- [ ] **M-declared-freer step 5** | The release for a marked `@` cell is paid by `call_or_release` and NOT by `emit_call`, which pays the ordinary write-backs. `sqlite3_exec` returns an `i32` and carries no mark on its result. Say what would have been skipped if the cell release had been hung off the result mark's branch, and name the program it would have been skipped for

    **Where to look:** selfhost/ir/owned_release.hero § call_or_release, release_cells
    **Why it matters:** the two marks are independent, and the milestone's own witness is the case that separates them
