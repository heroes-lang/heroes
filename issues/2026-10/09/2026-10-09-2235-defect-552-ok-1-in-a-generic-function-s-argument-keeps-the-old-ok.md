---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: f4e883af6a39d4a0877f8f9186e93fab8644eac4
github: none
---

- [ ] **552 — `[ok(1)]` in a generic function's argument keeps the old `ok` message** | defect 544's repair tells `ok(...)` at a generic function's parameter by its rule, and the same `ok(1)` one level down, inside an array literal argument, keeps the message before it (lane b17-check) | `selfhost/check/generic_argument.hero` · defect 544 · **class: adjacent**

    **Origin:** filed by the coordinator at 22:35 on 2026-10-09 from lane b17-check's final report (its notes `.claude/worktrees/scratch-b15/b17-check/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than its sibling's.

    Repaired at `f4e883af`, 2026-10-09 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A synthesised literal reads its type from its first element, a map's from its first key and value, so `check/generic_argument.hero`'s `places` follows the argument and a UFCS receiver down that chain and tells each form found the rule, the parameter and what it gives its type to (*nor the literal written there, which takes its type from this element*), its guess binding the whole argument first. Cases `check/fixedbugs-552-…` (9 shapes, each red on the base) and `full/fixedbugs-552-…` (2); `check` 648, `full` 31, `permissive` 16, the compiler's 1,544 tests, 0 failed; `check` of the compiler 76.212e9 to 76.170e9 instructions.
