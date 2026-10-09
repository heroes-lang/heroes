---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 912b2df39d5f814da4a12ee56d10404ed62f571d
github: none
---

- [ ] **520 — mutual recursion and a self-call through a function value pass `check`** | panel 199's R4 refuses a function every path of which calls itself, and lets through `ping` calling `pong` calling `ping` and `f = go` then `f(n)`; since defect 508's repair both abort 134 at every level (panel 199's R7, route (I)) | `selfhost/check/self_call.hero` · panel 199 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from panel 199's R7 and lane b16-land199's final report.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a refusal a sitting filed apart, owing a sitting of its own.

    Repaired at `912b2df3`, 2026-10-09 (lane b17-check, built with a helper from panel 201's compiler-engineer's prototype), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 201's R3, its four conditions met: the solo walk records a flat call graph, an iterative Tarjan (`check/endless_parts.hero`) leaves only components of two or more, a worklist re-walks a function only when a member its last walk counted drops, and the whole-program may-end pass runs once on the members left (`check/endless_cycle.hero`); `check selfhost/main.hero` 73.124e9 to 73.188e9 instructions (+0.087%, three interleaved runs), chains of 2,000 callers first 0.39 s against the prototype's 4.09 s; the headline names each member's targets, *every path through `pong` calls `ping` or `pang`*, true on the critic's c1 and c8, *call each other for ever* only where every member calls back and *never return* otherwise; the two witnesses `run/fixedbugs-520-…` replace defect 508's two `run` goldens, moved to `check/` as refusals with their correction appended; the 457 golden's correction appended. Whole: `check` 643, `run` 421, `emission` 1,076, `determinism` 459, `corpus` 55, `lines` 422, `warnings` 485, and the compiler's 1,537 tests, 0 failed. Beside it: defect 547's first shape, `selfloop_first`, is refused by this landing (`` `f` and `g` call each other for ever: every path through `f` calls `f` or `g` ``); its second, a self-call through a parameter, still passes.
