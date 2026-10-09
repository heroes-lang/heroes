---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: 0ec27fe0bd6aceccd8a4e7b9b8ba5a1e6aca811e
github: none
---

- [ ] **509 — on the CI's Windows leg ASan's report at `--sanitize` names no frame, so defect 322's case is red** | the CI run 37844345400 on `811f8398` (2026-10-08): `run/fixedbugs-322-a-sanitiser-names-the-hero-line-at-every-level` at `--sanitize`, built at `-O2` under `-gline-tables-only`, reports `AddressSanitizer: heap-use-after-free` with every frame an address alone (`fixedbugs322...exe+0x140032d1d`), no file and no line, so the needle `fixedbugs-322-a-sanitiser-names-the-hero-line-at-every-level.hero:21` is absent; clang 20.1.8 on the runner; the same case green on Darwin arm64, Linux arm64 and Linux x86-64; the Windows box (clang 23.1.1) unrun | `selfhost/cli/flags.hero` (`debug_words`: line tables at `-O2`), the link line on Windows (`link.hero`), `tests/golden/run/fixedbugs-322-a-sanitiser-names-the-hero-line-at-every-level.hero` · panel 197's R2 and R6 · defect 322 · **class: blocking**

    **Origin:** filed by the coordinator at 01:35 on 2026-10-09 from the CI's Windows leg on batch 15's push (`811f8398`): the net 6,811 passed and 1 failed, this case; the job stopped there, so the net's own tests did not run on that leg. Panel 197's R6 had asked exactly this of the platforms (*a unit at `-O2` under line tables*), and lane b14-land197 listed it as not run (*the ASan needle's format on Windows*).

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a red CI; the sanitiser's location is what panel 197's R2 kept line tables at `-O2` for.

    Repaired at `0ec27fe0`, 2026-10-09 (lane b15-runtime, batch 16), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Two causes, measured on the Windows box (clang 23.1.1): the binary recorded the PDB's path inside the private link directory the compiler removes, under a name the published PDB does not carry, so no frame was named at either level; and llvm-symbolizer, which ASan asks first, answers the last file block's line for every address of a function whose line table spans the `.hero` and the generated `.c`, so `-O0` named `.c:122` where DbgHelp names `.hero:21`. The link records the published PDB's own name (`/PDBALTPATH`), and `run` and `test` under `--sanitize` on Windows hand the program `external_symbolizer_path=` unless the user named a symbolizer. A binary run by hand at `-O0` keeps llvm-symbolizer's `.c` line; the runner's clang 20 is unrun on the box.
