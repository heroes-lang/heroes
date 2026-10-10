# Panel 206, historian's brief

Read `00-shared.md` first. Precedent, sourced and dated: how languages with
contextual literal typing treat arithmetic on literals that overflows its
type at compile time (Rust's `arithmetic_overflow` deny-by-default lint and
its history, Go's untyped constant arithmetic and its *constant overflows*
error, Swift's literal overflow diagnostics, Zig's comptime integer
overflow, C's constant expressions and `-Woverflow`, Ada's static
expressions and `Constraint_Error`), where each stops (a constant through
another, a call, a variable operand), and what false positives or user
complaints each refusal has drawn.
