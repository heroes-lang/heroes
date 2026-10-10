# Panel 207, historian's brief

Read `00-shared.md` first. Precedent, sourced and dated, for evaluating a
constant's body exactly at compile time with loops: Zig's comptime and its
`@setEvalBranchQuota` (its default, its message, its history), C++'s
`constexpr` evaluation limits (`-fconstexpr-steps`, `-fconstexpr-depth`, the
standard's implementation-defined limits), Rust's const evaluation and its
`const_eval_limit` (added and removed, and why), D's CTFE, Nim's `static:`
blocks and their limits; what each does at the bound, and what complaints
each drew.
