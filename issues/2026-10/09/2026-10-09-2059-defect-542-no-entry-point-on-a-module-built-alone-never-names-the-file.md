---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: cd2355e35f62614901113ef0ac6b4e5cc23eabf0
github: none
---

- [ ] **542 — `no_entry_point` on a module built alone never names the file that uses it** | `heroes build geom.hero` of a module tells *add `function main()`, or use `--emit-c`*, caret on its last token, and never *build the file that uses this one*; measurement 040's readers added a `main` under that note (panel 201's critic) | the `no_entry_point` note · panel 201 R2 · defect 467 · **class: improvement**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `cd2355e3`, 2026-10-09 (lane b17-fix, batch 17), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `no_entry_point` carries a module's route first, in general words, *if another file names this one with `use`, build that file instead: a module is compiled with the file that uses it, and needs no `main` of its own*, then a program's with `--emit-c` kept; the report moved into `selfhost/cli/no_entry.hero`, `cli/compile.hero` standing at its ceiling. Two surface rows red on the base and a unit test; `surface` 401, the net's own tests 326, the compiler's own tests 1,533, all passed.
