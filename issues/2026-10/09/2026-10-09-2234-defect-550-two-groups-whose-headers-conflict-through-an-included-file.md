---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: fd0ebba6f2153eabf393b67dfd31bef47686c5ac
github: none
---

- [ ] **550 — two groups whose headers conflict through an included file are told falsely in one order** | when the conflicting definition sits in a file another group's header includes, clang's note names that file, which no group names, so defect 538's repair tells one `use` order of both headers and the other still with the old false message; telling it order-free needs the include graph (lane b17-emit; reproducer `.claude/worktrees/scratch-b15/b17-emit/inc/`, ignored by git) | `selfhost/cli/headers_together.hero` · defect 538 · panel 200 R1 · **class: blocking**

    **Origin:** filed by the coordinator at 22:34 on 2026-10-09 from lane b17-emit's final report (its notes `.claude/worktrees/scratch-b15/b17-emit/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message, the shape beside defect 538, for the sitting panel 200 R1 names.

    Repaired at `fd0ebba6f2153eabf393b67dfd31bef47686c5ac`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. `-fdiagnostics-show-note-include-stack` joins the compile and probe flags (`cli/flags.hero`, every key moved once), so clang prints a note's own includes in the same run, and the refusal of two headers reads the note through them to the group's header it arrives by (`cli/group_file.hero`): both orders of `inc1` and `inc2` are told the same words, `at `a_impl.h` line 1, which `a.h` includes, and `b.h` line 1`; raylib with X11 in `test` names `X11/X.h` line 100, which `X11/Xlib.h` includes, and `raylib.h` line 322, where the old message said `raylib.h` does not compile. Cases `unsupported/fixedbugs-550-*` (2); unsupported 202 and 0, warnings 494 and 0, cache alone 7 and 0, the compiler's own tests 1,548 passed.
