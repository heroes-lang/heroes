---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: c522515d58c2356c6c4e3011843b0d89e91ddf27
github: none
---

- [ ] **500 — fourteen readers still build the table of built-ins where defect 264 left a constant** | `inventory.table()` is still read in `resolve/` (offered, built_marks), `ir/` (flatten, place_store, print) and `emit/` (ops, gate, unread): the last 18,030 table builds on the emission of a 2,000-long built-in chain; each a one-line switch to `inventory.name_of` or `NAMES`, after which `table()` may go (lane b14-check) | `selfhost/inventory.hero` and its fourteen readers · defect 264 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-check's final reports; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost per lookup.

    Repaired at `c522515d`, 2026-10-09 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The fifteen readers found, eleven wanting one name, two the list of names, two counts in tests and one test helper an index, read `inventory.name_of`, `NAMES` and `index_of`: over a main of 4,000 lines of built-in calls the emission retires 18.963 billion instructions against 19.909, 4.8% fewer, the C byte-identical, and the compiler checking itself is unchanged, 71.067 billion before and after. `table()` stays, its doc saying why: the test holding `NAMES` to it, and `tests/harness/suite_spec.hero`, which reads its `Builtin(name: ` lines as text.
