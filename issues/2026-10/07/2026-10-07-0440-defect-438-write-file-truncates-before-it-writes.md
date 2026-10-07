---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **438 — `write_file` truncates before it writes** | `write_file` opens its file truncated and then writes, so a concurrent reader can see it empty and a kill between the two leaves it empty, its old contents lost; lane b13-tmpl407's case had to change its witness for it | `runtime/`, `write_file` · **class: improvement**

    **Origin:** filed by the coordinator at 04:40 on 2026-10-07, from lane b13-tmpl407's report (*found beside*); the lane's reading, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): the common behaviour of a write, no program judged wrong; a write through a private name and a rename would keep the old contents until the new are whole.
