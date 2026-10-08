---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **509 — on the CI's Windows leg ASan's report at `--sanitize` names no frame, so defect 322's case is red** | the CI run 37844345400 on `811f8398` (2026-10-08): `run/fixedbugs-322-a-sanitiser-names-the-hero-line-at-every-level` at `--sanitize`, built at `-O2` under `-gline-tables-only`, reports `AddressSanitizer: heap-use-after-free` with every frame an address alone (`fixedbugs322...exe+0x140032d1d`), no file and no line, so the needle `fixedbugs-322-a-sanitiser-names-the-hero-line-at-every-level.hero:21` is absent; clang 20.1.8 on the runner; the same case green on Darwin arm64, Linux arm64 and Linux x86-64; the Windows box (clang 23.1.1) unrun | `selfhost/cli/flags.hero` (`debug_words`: line tables at `-O2`), the link line on Windows (`link.hero`), `tests/golden/run/fixedbugs-322-a-sanitiser-names-the-hero-line-at-every-level.hero` · panel 197's R2 and R6 · defect 322 · **class: blocking**

    **Origin:** filed by the coordinator at 01:35 on 2026-10-09 from the CI's Windows leg on batch 15's push (`811f8398`): the net 6,811 passed and 1 failed, this case; the job stopped there, so the net's own tests did not run on that leg. Panel 197's R6 had asked exactly this of the platforms (*a unit at `-O2` under line tables*), and lane b14-land197 listed it as not run (*the ASan needle's format on Windows*).

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a red CI; the sanitiser's location is what panel 197's R2 kept line tables at `-O2` for.
