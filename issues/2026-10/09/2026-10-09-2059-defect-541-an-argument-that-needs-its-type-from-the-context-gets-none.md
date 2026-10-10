---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: dca963a1cea1acf14a8237aab0d8e09c1e27b127
github: none
---

- [ ] **541 — an argument that needs its type from the context gets none from a generic function's parameter** | panel 105's rule refuses fourteen shapes, a generic function value, `ok(...)`, `[]`, a literal at a narrower width, a result-only call, a concrete parameter of a generic function among them; an inference giving context to every argument that needs it would make spec § 9 true without panel 201's sentence N1f; route I's prototype (`.claude/worktrees/scratch-b15/201-compiler-engineer/tree/`, ignored by git) is its first step, a sitting's | `selfhost/check/` · spec § 9 · design.md §4.12 · panel 201 R1 · defect 488 · **class: improvement**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a language widening that waits on a sitting.

    Repaired at `dca963a1`, 2026-10-10 (lane b18-infer, panel 203 R1 and R4), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A generic callee's argument with no type of its own waits while the call's others settle its letters and is checked against its parameter; what nothing else settles goes by form, never by position: the context where a literal it settles can take it, the argument owing least to a literal's default, a generic function's name, the forms that refuse themselves last and quiet on a letter a mistake poisoned; the receiver of `x.f()` is typed as `f`'s first argument where its form can hold no field, and spec § 9 and § 6 say so (V3T, +42 real). Panel 203's fourteen, `literal2`, `m1`, `m3` and § 9's `fold` over a `[u8]` are accepted and run; the critic's pairs and the compiler-engineer's eight hold in both orders; `genT2` and f4 are told once, f1's double message is gone with the refusal. Cases `run/fixedbugs-541-…` (48 values), `check/` and `full/fixedbugs-541-…`, seven goldens corrected beneath; `check` 651, `full` 33, `permissive` 16, `annotations` 942, `fixes` 929, the compiler's 1,546 tests, 0 failed; the census of 3,200 tracked files moves 18, none to refused, and the IR of the 1,636 both accept is identical.
