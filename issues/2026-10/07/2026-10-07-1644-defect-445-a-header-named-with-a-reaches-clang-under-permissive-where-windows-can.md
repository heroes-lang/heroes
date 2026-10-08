---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: 47c96f539fd01c9d2170321750380564d994da78
github: none
---

- [ ] **445 — a header named with a `"` reaches clang under `--permissive`, where Windows can hold no such file** | `undefined_header_name` is a thesis rule, so `check --permissive` and a permissive build let a header name holding `"` through to clang (lane b14-box, 2026-10-07) | `selfhost/head_names.hero`, `selfhost/head_windows.hero` · defect 252 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-box's final report (*found beside* 2).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an edge under `--permissive`, where the thesis rules step aside by design; whether this one should not is a sitting's.

    **2026-10-08, lane b15-box**: Repaired at `c9a22fa4`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Read by design.md Part 11 (the control arm drops the thesis checks alone) and the author's 1a of 2026-10-03, which extended panel 188's R2 to the names no NTFS file holds and moved `e/*b.h` to `unwritable_name` at defect 235: a `"` in any group head's string is now `unwritable_name`, told before any thesis rule (`head_windows.reserved_byte` holds `*`, `<`, `>`, `"`, `?` and `|`), and C's quotes carried in whole keep their certain fix under that code; `check --permissive` refuses the five headers of `permissive/fixedbugs-445-the-control-arm-refuses-a-header-no-windows-file-holds` (red on the base, 0 and 1) and drops the `'` beside them, three `check` cases moved with dated notes, and the compiler's 1,432 tests passed.

    **2026-10-08, lane b15-box**: Repaired at `47c96f53` as well, its shape beside with its own cause, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Measured on the Windows box that morning (Windows 10.0.26100, `CreateFileW`): no NTFS name holds a byte from U+0001 to U+001F, Win32 error 123 for 0x01, 0x09, 0x0B, 0x0C, 0x1B and 0x1F, while DEL, U+0080, U+009F, U+200B and U+2028 each stood in a file's name; so such a byte, panel 188's `unshowable_name` until then, is `unwritable_name` in all three strings, told before any thesis rule and named by its code, and `check --permissive` refuses the four of `permissive/fixedbugs-445-the-control-arm-refuses-a-control-byte-no-windows-file-holds` (red before it, 1 and 1) and drops DEL and U+0080 beside them; four `check` cases moved with dated notes, and the compiler's 1,434 tests passed.
