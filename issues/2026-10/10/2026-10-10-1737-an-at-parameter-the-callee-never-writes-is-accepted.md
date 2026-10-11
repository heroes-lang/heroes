---
kind: decision
area: resolve
milestone: none
filed: 2026-10-10
commit: self
github: none
---

- [x] **an `@` parameter the callee never writes** | should a Heroes function's `@` parameter that the body never writes (by `@` to the name, a field or an element, or through an `@` argument) be a compile error with a certain fix removing the `@` at the parameter and at every call site, as panel 209's never-re-bound rule refuses a cell nothing re-binds one level down? | `docs/panel/209-reports/completeness-critic-pass2.md` § 3, its probe `at_param_never_written.hero` (accepted today and under `heroes-r1b`, prints 2); spec § 5 `:138` (*a write is not a use, except through an `@` parameter*); spec § 9 (`@` parameters)

    **Origin:** panel 209's completeness critic, second pass, 2026-10-10 17:18 to 17:30: *B2's class one level up: an `@` parameter the callee never writes is accepted today and under r1b. One rule, two shapes.*

    **Default while open:** accepted, as today.

    **Recommendation:** refuse it under the same rule and in the same milestone as the cell, once the landing lane has counted the sites in `selfhost/`, `examples/` and `tests/` (unrun: no seat counted them) and read whether any is a parameter written only by a C call through an extern group, which the rule must count as a write as it counts an `@` argument. A parameter marked `@` that nobody writes tells a caller to hold a cell for nothing, which is the same false promise as a cell nothing re-binds; and the fix is multi-site (the signature and every caller), so it is `certain` only where every caller is in the compiled file set, `guess` otherwise.

    **Verdict, 2026-10-11:** carried to panel 210 (`docs/panel/210-an-at-parameter-nothing-writes-or-ends-is-refused-with-a-guess-fix-the-spec-sentence-waiting-for-its-refresh.md`), convened on the author's choice at about 02:07 in the question widget; its resolution, provisional, refuses an `@` parameter nothing writes or ends with `guess` fixes and waits for the author's ratification and the spec's `--refresh`; that ratification is `issues/2026-10/11/2026-10-11-0305-panel-210-ratify-amend-or-overturn-r1-to-r5-an-at-parameter.md`.
