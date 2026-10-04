---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: none
github: none
---

- [ ] **253 — the Windows device names written with a superscript digit, `COM¹` to `COM³` and `LPT¹` to `LPT³`, and `CONIN$` and `CONOUT$`, are outside the device rule, and what NTFS does with them is unmeasured** | `unwritable` refuses CON, PRN, AUX, NUL and COM0 to COM9, LPT0 to LPT9 by an ASCII digit (`selfhost/head_windows.hero:82` to `:92`), so `COM¹.h` (`43 4f 4d c2 b9`) passes on every platform; Microsoft's list of reserved names, which batch 8's FFI lane recalled and did not read, adds the superscript forms and the console's two (its report's finding 5, 2026-10-03) | `selfhost/head_windows.hero` (the device names) · the Windows box · **class: improvement**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 5, a question).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed on the box; no program measured wrong.
