---
kind: decision
area: check
milestone: none
filed: 2026-10-10
commit: self
github: https://github.com/heroes-lang/heroes/issues/1946
---

- [x] **panel 207** | ratify, amend or overturn R1 to R5 (spec § 4's sentence states exactly the class a constant's body refuses, open above: an integer operator that would abort, run or not, is a compile error at least where its operands are known, G3 under route W if it builds clean, G1 otherwise, one `--refresh` at the landing; the diagnostic note says the same; what the class leaves aborts where it runs, one `run` witness per kind; panel 206's R1 and R5 corrected beneath; an exact evaluator with a bound, route A, route C, D1r, N3, G2, G4 and C4a refused) | `docs/panel/207-a-constant-s-body-refuses-at-least-the-integer-steps-whose-operands-are-known-and-the-spec-says-exactly-that.md`

    **Origin:** panel 207's synthesis, 2026-10-10 from 12:29, on the tree frozen at `259de604`.

    **Verdict, 2026-10-10:** **ratified**, R1 to R5, the author answering through the question widget between 12:29 and 12:33 by the clocks read before and after (*Ratifica R1-R5*), and *yes, one* `--refresh`. Recorded as a reading (CLAUDE.md § 4).

## Prior art

Panel 207 (`docs/panel/207-a-constant-s-body-refuses-at-least-the-integer-steps-whose-operands-are-known-and-the-spec-says-exactly-that.md`, `docs/panel/207-reports/historian.md`): each line says what the cited page itself states, the sitting's own file says what was taken from it, and every link was re-read 2026-10-11.

- **Reached first by Rust**: [PR #103877, the const-eval step limit replaced by a lint](https://github.com/rust-lang/rust/pull/103877): the const-eval step limit was replaced by a lint with an exponential backoff warning (PR 103877)
- **Reached first by Zig**: [issue #21324, the comptime branch quota bypassed into a compiler crash](https://github.com/ziglang/zig/issues/21324): non-determinism in whether a given piece of Zig code compiles is not acceptable (issue 21324, the eval branch quota bypassed)
