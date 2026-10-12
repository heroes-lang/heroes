---
kind: decision
area: none
milestone: none
filed: 2026-08-10
commit: 7576d5d005ce196b9a6ae5e8ac9d21131b08ade5
github: https://github.com/heroes-lang/heroes/issues/127
---

2026-08-10 | **Panel 023 — a type with no size.** New diagnostic class `no_size` in the **checker** (a diagnostic `heroes check` cannot see is in the wrong pass — `missing_return`'s own precedent), and the rule is a **property, not a list**: *an edge is a field C lays out by value*. Stated as "a cycle not through `[T]` or `{K: V}`" the proposal was wrong in both directions and both were compiled, not argued: `record Scope { vars: {str: Scope} }` is legal at 8 bytes, and `record R { next: R? }` is `field has incomplete type` at exit 1, because a `T?` is by value and therefore **transparent** — it propagates the edge into `T`. Two judges found the `T?` row independently from opposite ends, one reading only the spec (`next: Node?` is "the most attractive wrong answer" for a list) and one reading only the tree. The property also forced itself: the check and the emitter's `typedef` order are one relation (§3.1:461, ordered since before the backend existed), and two edge sets cannot be one walk — a map imposes **no** ordering constraint on C, measured, three records mutually recursive through arrays compiling in the worst order with zero forward declarations. So `types/sized.rs` publishes the order in `Checked` and `emit/` contains no graph code. The walk is **iterative**: rustc #84611 is a stack overflow inside this very check, Nim #13715 a SIGSEGV, Zig #21436 an undetected cycle plus a crash — the check that prevents a hang is the one that historically hangs | today all four illegal shapes exit **0**, and the day M5c deletes the gate row they become exit 2 with a path to generated C: this compiler telling the author the compiler is broken about the author's mistake | §3.1, §4.17, §4.20 | 023 |

## Prior art

Panel 023 (`docs/panel/023-a-type-with-no-size.md`): each line says what the cited page itself states, the sitting's own file says what was taken from it, and every link was re-read 2026-10-11.

- **Taken from Go**: [internal/types/testdata/check/cycles0.go](https://raw.githubusercontent.com/golang/go/master/src/internal/types/testdata/check/cycles0.go): a struct that contains itself by value, directly or in an array, is an invalid recursive type, while a cycle through a pointer is legal
- **Taken from Go**: [golang/go#41575, invalid recursive type error is not as helpful as go/types](https://github.com/golang/go/issues/41575): the 'invalid recursive type' error named one type where go/types gave the cycle, and was fixed to report the whole type loop
- **Taken from Rust**: [rust-lang/rust#84611, checking for structural recursion can cause a stack overflow](https://github.com/rust-lang/rust/issues/84611): rustc's check for structural recursion overflowed the stack (issue 84611, closed by PR 85012)
