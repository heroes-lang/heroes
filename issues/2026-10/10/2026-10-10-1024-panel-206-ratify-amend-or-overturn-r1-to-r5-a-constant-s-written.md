---
kind: decision
area: check
milestone: none
filed: 2026-10-10
commit: self
github: https://github.com/heroes-lang/heroes/issues/1942
---

- [x] **panel 206** | ratify, amend or overturn R1 to R5 (a step of a constant's written body that cannot be computed, wherever written and read or not, a compile error, spec § 4's C4b, +13/+13 on the vendored tables, one `--refresh` at the landing; literal arithmetic in a function aborting where it runs, as § 7 says; `repeat`'s computed count aborting at its subtraction ratified as panel 054's path; a group's constant left an abort, its build probe filed as 579; a refusal over function bodies, F2a to F7h, C4a and a final-value semantics refused) | `docs/panel/206-a-constant-s-written-body-that-cannot-be-computed-is-a-compile-error-and-arithmetic-in-a-function-still-aborts-where-it-runs.md`

    **Origin:** panel 206's synthesis, 2026-10-10 from 09:44, on the tree frozen at `e0aeb991`.

    **Verdict, 2026-10-10:** **ratified**, R1 to R5, the author answering through the question widget between 09:44 and 10:22 by the clocks read before and after (*Ratifica R1-R5*), and *yes, one* `--refresh`. Recorded as a reading (CLAUDE.md § 4).

## Prior art

Panel 206 (`docs/panel/206-a-constant-s-written-body-that-cannot-be-computed-is-a-compile-error-and-arithmetic-in-a-function-still-aborts-where-it-runs.md`, `docs/panel/206-reports/historian.md`): each line says what the cited page itself states, the sitting's own file says what was taken from it, and every link was re-read 2026-10-11.

- **Reached first by Go**: [the Go specification, constant expressions](https://go.dev/ref/spec): constant expressions are evaluated exactly, a typed constant must be representable in its type, and a constant division by zero is illegal
- **Reached first by C#**: [checked and unchecked contexts](https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/checked-and-unchecked): constant expressions are evaluated in a checked context by default, and overflow in one is a compile-time error
- **Reached first by C**: [ISO C11 (N1570) 6.6p4](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf): each constant expression shall evaluate to a constant in the range of representable values for its type (6.6p4)
