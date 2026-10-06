---
kind: defect
area: cli
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **371 — `check --apply --json` ignores `--json` in silence, printing the program at exit 0** | on a file whose `x = 1,` takes a `certain` fix, `heroes check --apply --json` and `check --json --apply` write the applied program on stdout at exit 0, the same bytes as `check --apply`, and on a clean file the same; `--apply --brief` likewise (the base's compiler at `00217c39`, measured by the coordinator at 23:13 on 2026-10-05, `<scratchpad>/batch12/filing/ap.hero`; found by panel 193's completeness critic, its briefs pass) | `selfhost/cli/check.hero` (the `--apply` branch answers before the format flags are read) · `.claude/rules/cli-surface.md` (*`--json` says how to print, never what*) · panel 193, which rules what `--apply --json` answers · **class: adjacent**

    **Origin:** panel 193's completeness critic, 2026-10-05, its first pass over the briefs (`docs/panel/193-reports/completeness-critic-briefs.md`); reproduced by the coordinator at 23:13, after a first try whose zsh loop passed `--apply --json` as one word and read exit 2.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a flag the author gave is dropped with no word; no program moves and no message is false.
