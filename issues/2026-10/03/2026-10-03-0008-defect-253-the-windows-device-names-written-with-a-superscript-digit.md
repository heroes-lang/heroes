---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: aac84d00583ed90939f30e0460e0f5612407a504
github: none
---

- [ ] **253 — the Windows device names written with a superscript digit, `COM¹` to `COM³` and `LPT¹` to `LPT³`, and `CONIN$` and `CONOUT$`, are outside the device rule, and what NTFS does with them is unmeasured** | `unwritable` refuses CON, PRN, AUX, NUL and COM0 to COM9, LPT0 to LPT9 by an ASCII digit (`selfhost/head_windows.hero:82` to `:92`), so `COM¹.h` (`43 4f 4d c2 b9`) passes on every platform; Microsoft's list of reserved names, which batch 8's FFI lane recalled and did not read, adds the superscript forms and the console's two (its report's finding 5, 2026-10-03) | `selfhost/head_windows.hero` (the device names) · the Windows box · **class: improvement**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 5, a question).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed on the box; no program measured wrong.

    **2026-10-07, lane b14-box**: Repaired at `aac84d00`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The device rule refuses `COM¹` to `COM³`, `LPT¹` to `LPT³`, `CONIN$` and `CONOUT$` in any ASCII case, on the part before its first `.` with its ending spaces dropped, and admits `COM0` and `LPT0`, as Microsoft's *Naming Files, Paths, and Namespaces* (read that day) and the Windows box (10.0.26100, each bare name the device by `RtlIsDosDeviceName_U` and `GetFullPathNameW`, `COM0` and `LPT0` files) read them; its case `fixedbugs-253-a-device-named-by-a-superscript-digit-or-the-console` holds ten refusals and five passes, and the compiler's 1,358 tests passed.
