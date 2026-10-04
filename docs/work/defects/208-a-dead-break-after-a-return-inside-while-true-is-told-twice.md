- [ ] **208 — a dead `break` after a `return` inside `while true` is told twice, `unreachable_statement` and `missing_return`** | `while true` over `if m > 3`, `return m`, `break`, in a `function f(n: i64) -> i64`: `unreachable_statement` at the `break` and `missing_return` on `f`, both gone once the `break` is deleted | `selfhost/check/flow.hero` (panel 184's R4: a `while true` with a `break` of its own does not end a path, read by syntax) · **class: adjacent**

    **Origin:** lane flow4's report, 2026-10-03 (`scratchpad/lane-flow4/w/w16-dead-break-under-return.hero`, 2026-10-03); measured by the coordinator on the trunk at `e5893696` (`scratchpad/file-r5/`, 2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `4d8259cc`, 2026-10-04, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The cause is `selfhost/check/walk.hero`'s `block`, read by `check/path_end.hero`'s `forever`; `check/flow.hero`, named above, is route M's lattice and holds nothing about loops.
