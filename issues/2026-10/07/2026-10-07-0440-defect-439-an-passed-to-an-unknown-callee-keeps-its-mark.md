---
kind: defect
area: resolve
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **439 — an `@` passed to an unknown callee keeps its mark** | an `@` passed to a call whose callee name is unknown, or past the last parameter, keeps its mark, so with a name bound by `=` it can be told `not_mutable` beside `unknown_name` or the arity error | `selfhost/resolve/built_marks.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 04:40 on 2026-10-07, from lane b13-zero401's report (*found beside*); the lane's reading, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a second message beside a mistake already told, at an edge.
