---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: 488de2cb6873f12d2c3cc09bbe61a95ec6d93a82
github: none
---

- [ ] **446 — a rooted `link` and a header named `nul:x.h` reach the tools under `--permissive`** | `machine_locked_path` is a thesis rule, so `--permissive` hands a rooted `link` and `nul:x.h` to the linker and to clang (lane b14-box, 2026-10-07); what Windows does with `nul:x.h` is unmeasured | `selfhost/head_windows.hero` · defects 252 and 253 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-box's final report (*found beside* 3).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an edge under `--permissive`; `nul:x.h` a measurement owed on the box.

    **2026-10-08, lane b15-box**: Repaired at `05c65754`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Its rooted `link`: measured that day, ld64 and GNU ld 2.44 read `-l/opt/q/m446` as `<dir>/lib/opt/q/m446.a`, `-lC:/side/m` as `<dir>/libC:/side/m.a` and GNU's `-l:/w/abs/libm446.a` as `<dir>/w/abs/libm446.a`, below each directory they search and not at the path written, and lld-link 22 in the Linux container read `/out:pwn.lib`, which clang hands it for `-l/out:pwn`, as its own `/out:` option and wrote the program there; so defect 252's `/` and `\` reasons reach a rooted `link` and a rooted package's `\`, `unwritable_name` told before the thesis rule with panel 055's route, and `check --permissive` refuses the six of `permissive/fixedbugs-446-the-control-arm-refuses-a-rooted-library-no-linker-reads-as-written` (red on the base, 0 and 1), while `C:x`, a rooted header and a `.pc` package keep the thesis rule; three `check` cases moved with dated notes, and the compiler's 1,433 tests passed. **`nul:x.h` is unrun**: the Windows box answered no SSH connection from 23:43 on 2026-10-07 to this line, so what Windows opens for it is still the measurement this item owes.

    **2026-10-08, lane b15-box**: Repaired at `488de2cb`, its second half, which closes what the line above left unrun, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Measured on the Windows box that morning (Windows 10.0.26100, `CreateFileW`, `GetFullPathNameW`, `RtlIsDosDeviceName_U`, clang 23.1.1's `#include`): `nul:x.h` is no device, NTFS storing the stream `x.h` of an ordinary file `nul` and clang opening it at exit 0 as it opens `ab:c.h`, so its `:` stays `machine_locked_path`, defect 234's thesis rule, the name being one a Windows file can hold; a part that ENDS in `:` is the device (`NUL:`, `nul :`, `NUL::`, `sub\NUL:`, `COM1:`, `aux:`, `CONOUT$:`) or no file (`ab:`, `nulx:`, `COM0:` and `nul.h:` are Win32 error 123), and is `unwritable_name` now, told before the thesis rule, with a lone `C:` and `:` left to `rooted`; `permissive/fixedbugs-446-the-control-arm-refuses-a-name-ending-in-a-colon-and-keeps-a-stream` pins both (red on the compiler before it, 0 and 1), `check` whole read 612 and 0, and the compiler's 1,436 tests passed.
