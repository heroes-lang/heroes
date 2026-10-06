---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **398 — a function with an `@` parameter used as a value stops the build with an internal error** | `function bump(@n: i64)` taken as a value, `_ = bump`: `check` 0, `run` 2 with *internal error: compiling the generated C failed*, clang refusing to assign `void (int64_t *)` to the value's `void (*)(long long)` (the coordinator's re-run on the frozen tree, `docs/panel/194-evidence/new-defects/inout-value/`) | a function value's type for an `@` parameter, `selfhost/check/` and `selfhost/emit/` · spec § 9 (function values) · **class: blocking**

    **Origin:** panel 194's compiler-engineer, 2026-10-06 (*found beside* 3); reproduced by the critic's second pass and the coordinator.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told; § 9's function values and its `@` parameters meet, and `check` accepts what the build cannot make.
