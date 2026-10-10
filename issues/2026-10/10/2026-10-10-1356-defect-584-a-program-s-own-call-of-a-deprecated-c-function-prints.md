---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **584 — a program's own call of a deprecated C function prints clang's raw warning at exit 0** | after defect 571 quieted a header's `deprecated` attribute on the compiler's own lines, a program's own call of a function its header marks deprecated still prints clang's raw `-Wdeprecated-declarations` text and builds at exit 0; design.md says the language has no warning level (`:3744`, *a diagnostic is exit 1 or nothing*), so it is either a refusal or silence, a diagnostic-class question for a sitting | the emitted call site and the compile words, `selfhost/emit/` and `selfhost/cli/`; for a sitting · **class: blocking**

    **Origin:** found by lane b18-guard beside panel 205's landing (its final reply and notes, `.claude/worktrees/scratch-b15/b18-guard/notes.txt`, ignored by git), filed by the coordinator at 13:56 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.
