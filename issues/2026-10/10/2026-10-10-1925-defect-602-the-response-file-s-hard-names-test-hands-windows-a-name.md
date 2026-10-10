---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: f7712610e59651c2034289173511966bccf78952
github: none
---

- [ ] **602 — the response file's hard-names test hands Windows a name with `<` and `>`, which no Windows file may have** | batch 19's push (`b98016b5`), run 38069799820: the Windows x86-64 leg's step *The compiler's own tests* reads *1593 tests, 1 failed*, `FAIL "every hard name an object can have is read back from the file as written, and linked (defect 583)"` at `assert made.started && made.code == 0`, the compile of one object under one of `hard_names(windows: true)`; the list Windows is given holds `<CFGDIR>x.o`, and `<` and `>` are reserved in a Windows file name (Microsoft's file-naming rules, not run here: the box did not answer), so the likeliest failing name is that one, the assertion naming none; the link itself ran, the 87 of defect 601 gone | `selfhost/cli/response_file.hero`, `hard_names` and its test; the test should say which name failed · **class: blocking**

    **Origin:** filed by the coordinator at 19:25 on 2026-10-10 from the CI's Windows leg of batch 19's push, its job log read through the API (`.claude/worktrees/scratch-b15/ci-b980/win.txt`, ignored by git), as `.claude/rules/verification.md` § The optimistic chain item 5 asks.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a red CI.

    Repaired at `f7712610e59651c2034289173511966bccf78952`, 2026-10-11 (lane b20-link), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `<CFGDIR>x.o` moved to the POSIX-only list of `hard_names`, with `colon:x.o` and `pipe|ask?.o` added there, so POSIX is handed every byte Microsoft's naming rules reserve but `/` (*Naming Files, Paths, and Namespaces*, learn.microsoft.com, read 2026-10-11); every other name of the Windows list was read against those rules and holds. `windows_refuses` reads a name by them (the nine reserved bytes, a byte below 32, a part ending in a space or a dot, a device's name with or without an extension, the superscript `COM` and `LPT` among them), and a new compiler test holds the Windows list to it on every platform, so a name no Windows file may have is refused on this Mac. The compile loop tries every name before it asserts and shows each one that failed, `left: <name> | <name>` (run on a planted list), and the link asserts the linker's own words. Gate: the compiler's own tests 1,597, all passed; layout narrowed to `response_file` 4 and 0. On Windows unrun: the box did not answer at 00:54, and the CI's Windows leg judges after the push.
