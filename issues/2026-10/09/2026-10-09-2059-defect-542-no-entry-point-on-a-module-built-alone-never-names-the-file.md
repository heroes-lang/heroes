---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: cd2355e35f62614901113ef0ac6b4e5cc23eabf0
github: none
---

- [x] **542 — `no_entry_point` on a module built alone never names the file that uses it** | `heroes build geom.hero` of a module tells *add `function main()`, or use `--emit-c`*, caret on its last token, and never *build the file that uses this one*; measurement 040's readers added a `main` under that note (panel 201's critic) | the `no_entry_point` note · panel 201 R2 · defect 467 · **class: improvement**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `cd2355e3`, 2026-10-09 (lane b17-fix, batch 17), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `no_entry_point` carries a module's route first, in general words, *if another file names this one with `use`, build that file instead: a module is compiled with the file that uses it, and needs no `main` of its own*, then a program's with `--emit-c` kept; the report moved into `selfhost/cli/no_entry.hero`, `cli/compile.hero` standing at its ceiling. Two surface rows red on the base and a unit test; `surface` 401, the net's own tests 326, the compiler's own tests 1,533, all passed.

## The repair

Repaired at `cd2355e3`, 2026-10-09 (lane b17-fix, batch 17), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `no_entry_point` carries a module's route first, in general words, *if another file names this one with `use`, build that file instead: a module is compiled with the file that uses it, and needs no `main` of its own*, then a program's with `--emit-c` kept; the report moved into `selfhost/cli/no_entry.hero`, `cli/compile.hero` standing at its ceiling. Two surface rows red on the base and a unit test; `surface` 401, the net's own tests 326, the compiler's own tests 1,533, all passed.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
