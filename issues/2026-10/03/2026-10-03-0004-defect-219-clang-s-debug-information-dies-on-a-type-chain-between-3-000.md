- [ ] **219 — clang's debug information dies on a type chain between 3,000 and 5,000 nested variants on this Mac, by 10,000 in the Linux container** | past panel 184's R6 floor of 2,000: since `6c95f44a` (defect 170) the build says so in the compiler's words at exit 2 and leaves no crash files; clang's own stack raised to 64 MB compiled 10,000 in the lane's measurement, about eight times further, not built | `selfhost/cli/clang_died.hero` (lane depth's, at `6c95f44a`, 2026-10-03), `selfhost/emit/typeorder.hero` · **class: improvement**

    **Origin:** lane depth beside defect 170, 2026-10-03, reported to the coordinator.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): beyond the floor panel 184's R6 sets, and told truly at exit 2 since defect 170's repair; a reach nobody needs to be right today.
