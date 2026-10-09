---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **533 — a package answering `-framework` on Linux makes `build` exit 2** | GNU ld reads `-framework` as its own `-f` option and stops, *-f may not be used without -shared*, so a package whose answer carries `-framework X` (admitted for macOS) ends the build at exit 2 on Debian 13, GNU ld 2.44; reproducer `clang m.c -framework HeroesAbsent528` (lane b16-land198). The lane recommends reading that linker line on a refused link that carried a package's `-framework`, never a platform question, which `selfhost/cli/flags.hero:162-167` refuses | the refused link's reading, `selfhost/cli/package_missing.hero`, `selfhost/cli/whose.hero` · defect 528 · panel 198 · **class: blocking**

    **Origin:** filed by the coordinator at 12:54 on 2026-10-09 from lane b16-land198's final report (its reproducer `.claude/worktrees/scratch-b15/land198/beside/fwlinux/`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.
