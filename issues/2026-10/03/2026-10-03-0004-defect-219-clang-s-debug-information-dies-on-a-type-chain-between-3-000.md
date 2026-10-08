---
kind: defect
area: cli
milestone: none
filed: 2026-10-03
commit: 7ab0a4ee06bd3769d09af057d58e8adb977b7fb5
github: none
---

- [ ] **219 — clang's debug information dies on a type chain between 3,000 and 5,000 nested variants on this Mac, by 10,000 in the Linux container** | past panel 184's R6 floor of 2,000: since `6c95f44a` (defect 170) the build says so in the compiler's words at exit 2 and leaves no crash files; clang's own stack raised to 64 MB compiled 10,000 in the lane's measurement, about eight times further, not built | `selfhost/cli/clang_died.hero` (lane depth's, at `6c95f44a`, 2026-10-03), `selfhost/emit/typeorder.hero` · **class: improvement**

    **Origin:** lane depth beside defect 170, 2026-10-03, reported to the coordinator.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): beyond the floor panel 184's R6 sets, and told truly at exit 2 since defect 170's repair; a reach nobody needs to be right today.

    Repaired at `7ab0a4ee`, 2026-10-07 (lane b14-land197), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 197's R1 (ratified 2026-10-07): the touch `emit/typeorder.hero` writes past 32 deep is a cast, so clang's debug-information writer, which retains every cast's type and builds that list before any function, meets the chain from its bottom where the element descriptor's casts had handed it the top; `cli/deep_types.hero`'s test reads the first chain type clang retains, `h_t_R0`, and `h_t_R39` with the touch written as a declaration again. It closes on rows, never on *fixed* (panel 197's R7): on this Mac at `-O0` under `-g`, v5000, v10000, o5000, param-5000, mod-5000 and r-15000 build and print what they print, where the base died at exit 2 on each it was asked (lane b14-land197, 2026-10-07; v10000's base not re-run, the sitting read 2), and o10000 is about an hour of clang's front end, which no debug word reaches (the sitting's compiler-engineer and critic); on Linux arm64 both shapes build to 10,000, and Ubuntu clang 18.1.3, the CI's, builds v10000 where it died stock (the sitting, in a container); on Windows v10000 and o5000 build, stock too, and o10000 dies in clang's front end under every debug word, defect 468.
