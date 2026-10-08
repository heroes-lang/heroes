---
kind: defect
area: harness
milestone: none
filed: 2026-10-07
commit: 486cc7feb10b96405970f952dc9a002f4e5586d3
github: none
---

- [ ] **486 — the map command misses `tests/harness`, which `canonical` walks** | `canonical`'s `SOURCE_DIRS` walks `tests/harness`, the map command does not search for it, so the `tests/harness/**` row runs one suite too few (lane b14-hooks; `grep -n '"tests/harness"' tests/harness/suite_canonical.hero`) | `.claude/rules/verification.md` § The map · defect 288 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-hooks's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a map running one suite too few.

    Repaired at `486cc7fe`, 2026-10-08 (lane b15-hooks), gated by the command run whole under zsh and bash, `records` and `unseen`; the net is owed at the batch's close. Reproduced on the base: `canonical`'s row lacked `"tests/harness"`, its alternation naming the roots by hand. The map's command in `.claude/rules/verification.md` now reads the paths a suite can name from `git ls-files`, every file and directory, and prints each literal of a suite's code naming one, with a second listing for the harness's shared modules and the suites that reach them through `use`; against its old self it moved 11 of 24 suite rows, and the table took them: rows for `tests/**`, `runtime/**`, `editors/**`, `selfhost/diag.hero`, `inventory.hero` and `escape.hero`, the five files `absence.hero`'s `spellings()` reads, and `CLAUDE.md` with `docs/ROADMAP.md` and the spec's ledger, and suites added in eight rows; what it prints that is not a walk is written beneath the table.
