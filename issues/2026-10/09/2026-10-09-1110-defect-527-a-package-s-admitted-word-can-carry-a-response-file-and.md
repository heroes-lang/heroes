---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **527 — a package's admitted word can carry a response file, and clang or the linker reads options out of it** | the value of an admitted word is checked for a leading `-` and a comma and never for `@`: a `.pc` answering `-Wl,-framework,@rsp`, with `rsp` holding `X --ld-path=<program>`, runs that program as the linker during `heroes build` at exit 0 (macOS); `-Wl,-rpath,@rsp` and `-framework @rsp` make clang read `rsp` as options (a `-map` file written, macOS and Debian 13); `-I@rsp` joined is not expanded. The same list admits `-l:<file>` through the `-l` prefix (`libuv-static`'s `-l:libuv.a`), a word that names a file. This falsifies `docs/design.md`'s sentence that panel 050's allow-list holds against a `.pc` answering `@…`; the frozen compiler of panel 198 and its route K alike | `selfhost/cli/libraries.hero` (`clean_tail`, `valued`, `allowed_prefixed`, the `-framework` next word) · panel 050 · panel 055 · panel 198 · **class: blocking**

    **Origin:** filed by the coordinator at 11:10 on 2026-10-09 from panel 198's completeness critic, second pass (its report goes into the sitting's record with the synthesis, its probes are under `.claude/worktrees/scratch-b15/198-critic/probe/`, ignored by git); the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a package file running a program of its choosing during the build, the class the list exists to refuse (Go's CVE-2018-6574).
