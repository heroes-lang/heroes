---
kind: defect
area: cli
milestone: none
filed: 2026-10-05
commit: 8800ce9ccd02bb0fb477b8c113ae0e89f24c5e14
github: none
---

- [ ] **359 — on Windows `build -o name` writes `name` with no `.exe`, which `cmd.exe` and PowerShell cannot start** | on the Windows box, `heroes.exe build hi.hero -o name` answers *wrote name* at exit 0 and the file is a PE32+ console executable that Git Bash runs and `cmd //c name` refuses, *is not recognized as an internal or external command*, as PowerShell's `& .\name` does, *The operation attempted is not supported*; `build hi.hero` with no `-o` writes `build/<key>/hi.exe`, with the suffix, and `-o named.exe` writes `named.exe`, which `cmd //c named` starts (batch 11's coordinator, 2026-10-05, on `4c3524fb`'s compiler, found by defect 231's fair pair, whose `-o p231/astar800` wrote one) | `selfhost/cli/link.hero:49` (the default name takes `process.exe_suffix()`) and `:58` (`binary @ output.must()`, the author's path taken as written) · **class: adjacent**

    **Origin:** batch 11's coordinator, 2026-10-05, on the Windows box, running panel 190's R10 pair: the script's `[ -f p231/astar800.exe ]` found nothing, and the binary was `p231/astar800`.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): found beside the work; nothing said is false, *wrote name* names the file written, and the binary is right, but on Windows a build whose path has no extension leaves a program the platform's own shells cannot start by that path. **Unrun**: whether `heroes run` and `heroes test`, which name their own binaries, ever take an author's path; what clang or MinGW's gcc do for the same `-o`, the precedent a repair would weigh.

    Repaired at `e46b0adf`, 2026-10-05 (lane cli12), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The precedent, measured before choosing: clang 23.1.1 on the box writes `-o name` as `name` (lld-link's `-out:name`), and GCC's driver appends `.exe` to an `-o` whose last part holds no `.` for MinGW and Cygwin (`gcc.cc`'s `convert_filename`, read 2026-10-05); gcc's rule is taken, a device left as written. `heroes run -o` takes the author's path too and gets the same suffix. **Unrun**: the repair on the Windows box, offline since about 19:40; it is the push's Windows leg's.

    Repaired at `8800ce9c` as well, 2026-10-05: on the box at `e87deed4` the compiler's own tests read 1242 and 2 failed, two expectations pinning `cannot write .../out` and `.../app` where the build now names `out.exe` and `app.exe`; they take the platform's suffix now. Run on the Windows box under clang 23.1.1, a fresh tree of `8800ce9c` in `/c/w/b12-cli12-8800ce9c-32882`, the compiler built from the seed and then from `selfhost/`: the compiler's own tests 1242, all passed; `build -o h359/name` wrote `name.exe` and said `wrote h359/name.exe`, and `cmd.exe` and PowerShell each started it by `h359\name` (42); `run -o h359/ran` left `ran.exe`; `-o h359/kept.bin` kept its suffix; `-o NUL` wrote nothing and made no `NUL.exe`; `-o h359\sub.d\inner` wrote `inner.exe`, which `cmd.exe` started.
