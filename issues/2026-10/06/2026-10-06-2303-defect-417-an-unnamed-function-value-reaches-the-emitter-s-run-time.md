---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: 502170c35ec9e1823905f24575647db3d8fe718a
github: none
---

- [ ] **417 — an unnamed function value reaches the emitter's run-time fallback instead of a build-time refusal** | the emitter's fallback for a function value it cannot name is `hero_unreachable`, which aborts at run time; a check in the IR verifier would refuse the program at build time | `selfhost/emit/func_ref.hero`, the IR verifier · **class: improvement**

    **Origin:** lane b13-gen402, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): hardening, no program measured reaching it.

    Repaired at `502170c3`, 2026-10-07 (lane b14-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The verifier refuses, after monomorphisation and after the ownership pass, a call or a function value whose Heroes callee is no function of the program at the type arguments the instruction names, by the emitter's own lookup (`ir/instances.hero`) read from an index built once per verification, and a built-in named as a value at every phase; 17 shapes beside defect 402's were sought first and none reached either fallback, `run/fixedbugs-417-every-function-value-is-named-before-c-is-written` pins eight of the twelve that built (the other four, a qualified name, a value at the top level, one in a test and a function named as a method, were probed and not pinned), and `--dump-ir` over the compiler's own source reads 146.44 and 147.99 billion instructions before and after (+1.1%).
