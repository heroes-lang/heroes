---
kind: defect
area: cli
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **621 — `check --apply --in-place` says no certain fix applies where its one is in another module, and counts that module's diagnostics as the root's** | a root `main.hero` with no diagnostic of its own, `use lib/geom`, and `lib/geom.hero` holding `y @= x` that nothing re-binds: `heroes check main.hero --apply --in-place` prints `note: 1 certain fix(es) are in another module and were not applied: lib/geom.hero:2` and then that `main.hero` is unchanged, *no certain fix applies to its 1 diagnostic(s)* (the word `certain` in backticks), exit 0; the diagnostic is `lib/geom.hero`'s, not the root's, and a certain fix does apply to it, the note above says so. With an `unknown_name` added to the root, the line says `its 2 diagnostic(s)` of a root that has one. The second line contradicts the first, and the count is the stage's over every module, read as the root's | `selfhost/cli/apply_answer.hero` `hand_back` (the `unchanged` line, whose `told` is `report`'s `kept.len()`, every module's), `selfhost/cli/check.hero` `report` and `settle` (the `elsewhere` list the line could read); the shapes beside: the root's own fix applied beside one elsewhere (`rewrote main.hero`, true), `--apply` without `--in-place`, `--json` · defect 607 · **class: blocking**

    **Origin:** found by lane b20-tools at 01:13 on 2026-10-11, measuring defect 607 in a scratch tree (`.claude/worktrees/scratch-b15/b20-tools/m607c/`, ignored by git) with the lane's compiler built from the trunk at `42656199`.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a false message, `no certain fix applies` of a diagnostic whose certain fix the line above names, and a count of diagnostics the file it names does not hold. Found beside 607 with another cause: 607 is what `--in-place` writes, this is what it says when it writes nothing.
