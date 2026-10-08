---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: 05c65754c5d668fcc76e4c7d1110dbe7129fe2ef
github: none
---

- [ ] **446 — a rooted `link` and a header named `nul:x.h` reach the tools under `--permissive`** | `machine_locked_path` is a thesis rule, so `--permissive` hands a rooted `link` and `nul:x.h` to the linker and to clang (lane b14-box, 2026-10-07); what Windows does with `nul:x.h` is unmeasured | `selfhost/head_windows.hero` · defects 252 and 253 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-box's final report (*found beside* 3).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an edge under `--permissive`; `nul:x.h` a measurement owed on the box.

    **2026-10-08, lane b15-box**: Repaired at `05c65754`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Its rooted `link`: measured that day, ld64 and GNU ld 2.44 read `-l/opt/q/m446` as `<dir>/lib/opt/q/m446.a`, `-lC:/side/m` as `<dir>/libC:/side/m.a` and GNU's `-l:/w/abs/libm446.a` as `<dir>/w/abs/libm446.a`, below each directory they search and not at the path written, and lld-link 22 in the Linux container read `/out:pwn.lib`, which clang hands it for `-l/out:pwn`, as its own `/out:` option and wrote the program there; so defect 252's `/` and `\` reasons reach a rooted `link` and a rooted package's `\`, `unwritable_name` told before the thesis rule with panel 055's route, and `check --permissive` refuses the six of `permissive/fixedbugs-446-the-control-arm-refuses-a-rooted-library-no-linker-reads-as-written` (red on the base, 0 and 1), while `C:x`, a rooted header and a `.pc` package keep the thesis rule; three `check` cases moved with dated notes, and the compiler's 1,433 tests passed. **`nul:x.h` is unrun**: the Windows box answered no SSH connection from 23:43 on 2026-10-07 to this line, so what Windows opens for it is still the measurement this item owes.
