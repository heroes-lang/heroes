- [ ] **M-value-aggregates step 1** | The first **graph algorithm** in this compiler. `types/sized.rs` colours nodes white/grey/black instead of carrying a `visited` flag, and the difference IS the algorithm: grey means "on the current path", which is what makes a back edge a cycle rather than a diamond. Question: for `record Point/record Rect { a: Point, b: Point }`, how many times does the walk enter `Point`, and what would a `visited` flag get wrong that colours get right — and vice versa?

    **Where to look:** archive/bootstrap-rs/heroes/src/types/sized.rs (walk) · docs/panel/023 R9
    **Why it matters:** the historian predicted the first defect here would be a false positive, and this is the mechanism that avoids it
