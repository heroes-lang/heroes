---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: eae2f659cb3a55373b0105db24c6007115bcd8d3
github: none
---

- [ ] **453 — the C `--emit-c` writes is compiled by no clang run** | `write_c` runs only for `--emit-c`, whose program build compiles module by module, and only `heroes test` compiles a fused unit, so no clang reads the artifact; clang found nothing in the artifacts of all 372 run goldens, and an `-fsyntax-only` pass would cost about 0.21 billion instructions on a small program and 20.9 billion on the seed (lane b14-cli's measurements, not re-run by the coordinator) | `selfhost/cli/artifact.hero` · defect 434 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-cli's final report (*decisions* 1, the lane's recommendation: ask clang `-fsyntax-only` on the artifact before writing it).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an artifact nobody has checked, every one clean today; it changes what the tool does, so a sitting's question.

    **Widened 2026-10-10** by panel 202 (R3, ratified that day; `docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`): since per-module units, the one file can fail to compile where every unit compiles (`fp`), or compile into another program (`macro3`, `cfg`), so it is refused at exit 1 with a true message where it cannot mean the program the units build.

    Repaired at `eae2f659cb3a55373b0105db24c6007115bcd8d3`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Where a module's unit reads other headers than the one file, clang is asked whether the file compiles and, of both header lists, each binding's canonical C type, its last `#define` and its declaration's dump (`cli/one_file.hero`, `cli/dump_meaning.hero`); a refusal is `ffi_one_file` at exit 1, nothing written. Cases `surface-fixtures/onefile453*` (3) with four `surface` rows; a census of `--emit-c` over the 1,359 tracked programs with a group refused 9 more, each a file clang refuses or a measured other program, their seven blessed emissions retired; `selfhost/`'s `--emit-c` byte-identical, 443.6e9 to 454.3e9 instructions; emission 1107 and 0, surface 406 and 0, annotations 960 and 0, the compiler's own tests 1,555 passed and the net's 326.

    Its suites widened at `77afdb56900146dca20b3e349a8cacfe3fd661e0`, 2026-10-10, after the coordinator's trial net on batch 18's round read `determinism` 498 and 7 and `lines` 460 and 7 on the seven run goldens whose one file `--emit-c` refuses: such a program is judged on the units `build` writes for it, found as the new `build/tu-…` directories of a build with an `--include` of an empty directory of the run's own (`tests/harness/one_file_units.hero`), `determinism` asking them twice alike and `lines` walking each one's `#line` claims; determinism 505 and 0, lines 467 and 0, the net's own tests 332 passed, five of them new (the commit's body says four).
