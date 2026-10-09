---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: 4b956b613499f5e75bb269f6f22228dcea5479a5
github: none
---

- [ ] **527 — a package's admitted word can carry a response file, and clang or the linker reads options out of it** | the value of an admitted word is checked for a leading `-` and a comma and never for `@`: a `.pc` answering `-Wl,-framework,@rsp`, with `rsp` holding `X --ld-path=<program>`, runs that program as the linker during `heroes build` at exit 0 (macOS); `-Wl,-rpath,@rsp` and `-framework @rsp` make clang read `rsp` as options (a `-map` file written, macOS and Debian 13); `-I@rsp` joined is not expanded. The same list admits `-l:<file>` through the `-l` prefix (`libuv-static`'s `-l:libuv.a`), a word that names a file. This falsifies `docs/design.md`'s sentence that panel 050's allow-list holds against a `.pc` answering `@…`; the frozen compiler of panel 198 and its route K alike | `selfhost/cli/libraries.hero` (`clean_tail`, `valued`, `allowed_prefixed`, the `-framework` next word) · panel 050 · panel 055 · panel 198 · **class: blocking**

    **Origin:** filed by the coordinator at 11:10 on 2026-10-09 from panel 198's completeness critic, second pass (its report goes into the sitting's record with the synthesis, its probes are under `.claude/worktrees/scratch-b15/198-critic/probe/`, ignored by git); the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a package file running a program of its choosing during the build, the class the list exists to refuse (Go's CVE-2018-6574).

    Repaired at `4b956b61`, 2026-10-09 (lane b16-land198, panel 198's R2, ratified by the author that day), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every admitted word's value is judged as a value in every slot, joined or as the next word: not empty, not beginning with `-` or `@`, and for `-l` not beginning with `:`; an `@` inside a path stays a letter of it, and the refusal says which it read. Reproduced first on this Mac with stand-in `pkg-config`s: under the base compiler `-framework @f`, `-Wl,-framework,@f`, `-Wl,-rpath,@f` and `-Wl,-rpath -Wl,@f` each left the linker map its response file asked for, at exit 0; under the repair each is `ffi_package` at exit 1 and leaves no file. `docs/design.md`'s sentence that a `.pc` answering `@...` is `ffi_package` is corrected underneath. Cases: two compiler tests and one `absence.hero` case over seven shapes, red on route K's code; the compiler's own tests 1518 passed, the net's own tests 321.
