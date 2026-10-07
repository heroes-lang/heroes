---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: 368dafda8553a2715967be21bd1d241ad699fab9
github: none
---

- [ ] **474 — a function type nested deep in a signature copies table types as the square of its depth** | `table.Ty` copies 41,050 at 250 and 143,925 at 500, no walk function growing; likely a whole-table copy, not located (lane b14-check) | `selfhost/check/` · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the nesting's square.

    Repaired at `368dafda`, 2026-10-08 (lane b15-check), gated by its cases; the net is owed at the batch's close. Located, and repaired by another lane: the copies are `check/table.intern`'s push through the table's own field, `t.nodes @ t.nodes.push(ty)`, defect 409's shape, repaired in lane b14-p409 at `2a26fde2` (`table.append_node`), carried into this round and not in this lane's base, so this item closes only with that commit merged. `check` of a function type 250 and 500 deep copies 41,050 and 143,925 table types on the base, the numbers above, and 806 and 959 on the base with that commit's tree; at 8,000 deep, sampled there, what still grows is the parse lane's bracket pairing and lexer state. `368dafda` pins the shape at 500.
