---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **583 — on Windows the compiler cannot link its own per-module build, its command line too long** | on the Windows box (clang 23.1.1) the link of `selfhost/main.hero`'s per-module build fails with OS error 87, 558 objects and 32,243 characters of object paths on one command line, past Windows' limit; it happens on the trunk too, before batch 18; the compiler's own fused C builds and passes there | the link step's command, `selfhost/cli/` (the link words, `link_flags.hero` in batch 18's round): a response file for the link on every platform, or where the line would pass the limit · **class: blocking**

    **Origin:** found by lane b18-guard beside panel 205's landing (its final reply and notes, `.claude/worktrees/scratch-b15/b18-guard/notes.txt`, ignored by git), filed by the coordinator at 13:56 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): the compiler unable to build itself per module on a supported platform.
