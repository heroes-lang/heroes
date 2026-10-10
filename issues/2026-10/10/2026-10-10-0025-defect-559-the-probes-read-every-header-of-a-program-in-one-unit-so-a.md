---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **559 — the probes read every header of a program in one unit, so a binding is judged against another module's header** | panel 202's cases: `fpat64` (`fill(@x: i64)` over `void fill(int *x)`, beside two headers each defining a `static inline twice`) builds at exit 0 and prints `6 4294967299`, `selfhost/cli/pointee.hero:200-204` returning `ok()` when the dump unit does not compile, while the program without the conflict is refused `ffi_parameter_type`; `fprec` (a group record beside them) is refused at `build`; `fpwide` (`fill(@x: i32)` over `void fill(long *x)`) builds and aborts 134 advising `counted_by` where the repair is `i64`; `skew` (a macro in one module's header choosing another's record layout) gets two self-contradicting messages of four and a `test` verdict that flips with the `use` order; `macro4`'s `test` tells a false `ffi_return_type` | `selfhost/cli/pointee.hero`, the layout probe, `compiling.probe`, `artifact.hero:80` · panel 202 R2's C0 · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 202 (`docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`): the compiler-engineer's cases `fpat64` and `fprec`, the critic's second pass's `fpwide` and `skew`, the ffi-pragmatist's `macro4`, each the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0, a correct program refused, a false message.
