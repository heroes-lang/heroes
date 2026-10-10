---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: 555eade6e5892213024745d462df9965234b097e
github: none
---

- [ ] **559 — the probes read every header of a program in one unit, so a binding is judged against another module's header** | panel 202's cases: `fpat64` (`fill(@x: i64)` over `void fill(int *x)`, beside two headers each defining a `static inline twice`) builds at exit 0 and prints `6 4294967299`, `selfhost/cli/pointee.hero:200-204` returning `ok()` when the dump unit does not compile, while the program without the conflict is refused `ffi_parameter_type`; `fprec` (a group record beside them) is refused at `build`; `fpwide` (`fill(@x: i32)` over `void fill(long *x)`) builds and aborts 134 advising `counted_by` where the repair is `i64`; `skew` (a macro in one module's header choosing another's record layout) gets two self-contradicting messages of four and a `test` verdict that flips with the `use` order; `macro4`'s `test` tells a false `ffi_return_type` | `selfhost/cli/pointee.hero`, the layout probe, `compiling.probe`, `artifact.hero:80` · panel 202 R2's C0 · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 202 (`docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`): the compiler-engineer's cases `fpat64` and `fprec`, the critic's second pass's `fpwide` and `skew`, the ffi-pragmatist's `macro4`, each the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0, a correct program refused, a false message.

    Repaired at `555eade6e5892213024745d462df9965234b097e`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Each module's unit is planned before any probe and reads its own groups' headers and those of the group records it reaches (`emit/unit_plan.hero`), where every unit read every group record's header; the pointee check, the layout check and a refused round's questions ask each distinct header list once (`cli/probing.hero`); a pointee dump that does not compile is refused after the units compile, never passed. `fpat64` and `fpwide` are refused `ffi_parameter_type` at `build`, `fprec` builds and prints `6 8`, `skew32` builds in both orders and `skew64` is told `int ... not i64` in both, as the critic predicted. Cases `unsupported/fixedbugs-559-*` (4) and `run/fixedbugs-559-*` (4); unsupported 195 and 0, emission 1092 and 0, the compiler's own tests 1,544 passed; a warm self-build 419.2e9 instructions before and 388.1e9 after, the probes' clang runs 2 and 5. `macro4`'s `test` is still one unit's, defect 560's.
