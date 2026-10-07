---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **500 — fourteen readers still build the table of built-ins where defect 264 left a constant** | `inventory.table()` is still read in `resolve/` (offered, built_marks), `ir/` (flatten, place_store, print) and `emit/` (ops, gate, unread): the last 18,030 table builds on the emission of a 2,000-long built-in chain; each a one-line switch to `inventory.name_of` or `NAMES`, after which `table()` may go (lane b14-check) | `selfhost/inventory.hero` and its fourteen readers · defect 264 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-check's final reports; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost per lookup.
