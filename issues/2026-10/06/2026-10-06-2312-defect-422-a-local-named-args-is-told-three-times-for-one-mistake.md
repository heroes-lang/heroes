---
kind: defect
area: resolve
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **422 — a local named `args` is told three times for one mistake** | `args: i64 @ 1` then two writes `args @ 2`, `args @ 3`: `check --brief` tells `builtin_name_taken` at the binding and `no_mutable_globals` at each later write, three messages for the one name (the coordinator's re-run on round b13's source at `3bc3b2a5` with lane fixed405 merged, 23:12) | `selfhost/resolve/`, the reading of a binding whose name a built-in holds · **class: adjacent**

    **Origin:** lane b13-fixed405, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:12; reproduced by the coordinator.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): one mistake told as three, each later write read as the built-in's.
