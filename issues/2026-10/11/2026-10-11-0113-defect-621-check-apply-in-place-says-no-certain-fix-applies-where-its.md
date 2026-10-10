---
kind: defect
area: cli
milestone: none
filed: 2026-10-11
commit: 981e6071f93a4073ec01ca179332726975f3c22e
github: none
---

- [ ] **621 — `check --apply --in-place` says no certain fix applies where its one is in another module, and counts that module's diagnostics as the root's** | a root `main.hero` with no diagnostic of its own, `use lib/geom`, and `lib/geom.hero` holding `y @= x` that nothing re-binds: `heroes check main.hero --apply --in-place` prints `note: 1 certain fix(es) are in another module and were not applied: lib/geom.hero:2` and then that `main.hero` is unchanged, *no certain fix applies to its 1 diagnostic(s)* (the word `certain` in backticks), exit 0; the diagnostic is `lib/geom.hero`'s, not the root's, and a certain fix does apply to it, the note above says so. With an `unknown_name` added to the root, the line says `its 2 diagnostic(s)` of a root that has one. The second line contradicts the first, and the count is the stage's over every module, read as the root's | `selfhost/cli/apply_answer.hero` `hand_back` (the `unchanged` line, whose `told` is `report`'s `kept.len()`, every module's), `selfhost/cli/check.hero` `report` and `settle` (the `elsewhere` list the line could read); the shapes beside: the root's own fix applied beside one elsewhere (`rewrote main.hero`, true), `--apply` without `--in-place`, `--json` · defect 607 · **class: blocking**

    **Origin:** found by lane b20-tools at 01:13 on 2026-10-11, measuring defect 607 in a scratch tree (`.claude/worktrees/scratch-b15/b20-tools/m607c/`, ignored by git) with the lane's compiler built from the trunk at `42656199`.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a false message, `no certain fix applies` of a diagnostic whose certain fix the line above names, and a count of diagnostics the file it names does not hold. Found beside 607 with another cause: 607 is what `--in-place` writes, this is what it says when it writes nothing.

    Repaired at `981e6071`, 2026-10-11 (lane b20-tools), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `apply_answer.hand_back` takes the root's own count, `in_root` asking `source.is_root` of each diagnostic's start, and its `unchanged` line says, where the root holds none, `none of the diagnostics is in it`, and where the note above names certain fixes in another module, that they are there: `applyx/main.hero` now reads `unchanged: none of the diagnostics is in it, and the certain fix(es) named above are in another module`; a root with one unknown name of its own beside such a fix, `no certain fix applies to its 1 diagnostic(s), and the one(s) named above are in another module`. Nothing written moves. Cases: four shapes in `apply_answer.hero`'s test and `surface`'s row on `applyx`; `surface` 408 and 0, `layout` narrowed to `apply_answer` 4 and 0, the compiler's own tests 1597 and 0, the net's own tests 336 and 0.
