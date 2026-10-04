---
kind: decision
area: none
milestone: none
filed: 2026-08-04
commit: ba81c224df610382690b119dea4188aab9c8e22c
github: none
---

2026-08-04 | The verifier's value invariant is **dominance**, not block-locality. The first wording ("a temporary is read only in the block that defines it") was falsified by the `assert` lowering the hour it landed: both sides of the comparison are computed in the test block and read in the abort block, which the test block dominates. `verify.rs` split at the seam that appeared — a block's *structure* stays there, a value's *lifetime* moves to `ir/values.rs` | with every temporary hoisted to the C prologue and uninitialised, a use its definition does not dominate is a read of an uninitialised local — caught by `-Werror=uninitialized` as a compiler bug wearing a user diagnostic (§8's wart 13) | Part 10 step 6 | 020 |
