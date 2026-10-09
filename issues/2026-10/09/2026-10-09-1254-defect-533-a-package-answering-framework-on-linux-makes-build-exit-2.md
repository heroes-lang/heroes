---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: dae7ca34114ef9292cb48157831181eb086d20b4
github: none
---

- [ ] **533 — a package answering `-framework` on Linux makes `build` exit 2** | GNU ld reads `-framework` as its own `-f` option and stops, *-f may not be used without -shared*, so a package whose answer carries `-framework X` (admitted for macOS) ends the build at exit 2 on Debian 13, GNU ld 2.44; reproducer `clang m.c -framework HeroesAbsent528` (lane b16-land198). The lane recommends reading that linker line on a refused link that carried a package's `-framework`, never a platform question, which `selfhost/cli/flags.hero:162-167` refuses | the refused link's reading, `selfhost/cli/package_missing.hero`, `selfhost/cli/whose.hero` · defect 528 · panel 198 · **class: blocking**

    **Origin:** filed by the coordinator at 12:54 on 2026-10-09 from lane b16-land198's final report (its reproducer `.claude/worktrees/scratch-b15/land198/beside/fwlinux/`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.

    Repaired at `dae7ca34`, 2026-10-09 (lane b16-land198), gated by its cases and the compiler's own tests; the net is owed at the batch's close, and as a C-boundary item (a package's answer reaching the linker) its Linux case is owed the push's platform legs. Only ld64 takes `-framework`, so a package answering `-framework X` on Linux or Windows ended the build at exit 2; GNU ld 2.44 reads it as its own `-f` and stops, *-f may not be used without -shared*, and the lld family calls it *unknown argument '-framework'*. `selfhost/cli/package_missing.hero` reads those wordings and, since none names the framework, reads it from the one the link carried, telling it at exit 1 at the package's string as macOS's, no `link` way out, never a platform question. Reproduced in the Debian 13 container with a stand-in `.pc`: `-framework X`, `-Wl,-framework,X` and `-framework` beside a `-lm` each exit 2 on the base and exit 1 now; `ld.lld` (ELF) and `lld-link` (Windows) are not reachable by a package's own words, so their wording is a compiler test only. Cases: a compiler test on the reader over the three wordings, red on the base; the compiler's own tests 1527 passed, the net's own tests 322.
