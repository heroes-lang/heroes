---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **602 — the response file's hard-names test hands Windows a name with `<` and `>`, which no Windows file may have** | batch 19's push (`b98016b5`), run 38069799820: the Windows x86-64 leg's step *The compiler's own tests* reads *1593 tests, 1 failed*, `FAIL "every hard name an object can have is read back from the file as written, and linked (defect 583)"` at `assert made.started && made.code == 0`, the compile of one object under one of `hard_names(windows: true)`; the list Windows is given holds `<CFGDIR>x.o`, and `<` and `>` are reserved in a Windows file name (Microsoft's file-naming rules, not run here: the box did not answer), so the likeliest failing name is that one, the assertion naming none; the link itself ran, the 87 of defect 601 gone | `selfhost/cli/response_file.hero`, `hard_names` and its test; the test should say which name failed · **class: blocking**

    **Origin:** filed by the coordinator at 19:25 on 2026-10-10 from the CI's Windows leg of batch 19's push, its job log read through the API (`.claude/worktrees/scratch-b15/ci-b980/win.txt`, ignored by git), as `.claude/rules/verification.md` § The optimistic chain item 5 asks.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a red CI.
