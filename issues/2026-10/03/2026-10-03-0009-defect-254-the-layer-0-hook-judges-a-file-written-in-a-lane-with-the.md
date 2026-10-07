---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: c821ef3832b8aecdb039058568a6123deaa6843f
github: none
---

- [ ] **254 — the layer-0 hook judges a file written in a lane with the compiler of the session's directory, and skips the whole compiler's check there** | `.claude/hooks/fmt_check.py` takes the compiler as `<cwd>/heroes` and the file's path relative to `<cwd>`: a `selfhost/` module written under `.claude/worktrees/<lane>/` by a session whose directory is the trunk is formatted by the trunk's compiler and reads as `.claude/worktrees/<lane>/selfhost/...`, which is not `selfhost/`, so `heroes check selfhost/main.hero` never runs for it (read by the coordinator, 2026-10-04; batch 8's FFI lane, 2026-10-03, its report's finding 7) | `.claude/hooks/fmt_check.py:75` to `:119` · `.claude/rules/verification.md` § A suite is the last judge, layer 0 · **class: improvement**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 7); the hook read by the coordinator, 2026-10-04.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach; no program moves.

    Repaired at `c821ef38`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests, and `records`; the net is owed at the batch's close. `fmt_check.py` judges a written file in the tree it stands in, the nearest directory above it holding `seed/heroes.c` and a `.git` entry (`.claude/hooks/trees.py`), with that tree's compiler and its paths read from that tree's root; `.claude/hooks/test_hooks.py`, the hooks' first tests, holds seven cases, four red on the base and seven green, and a lane module calling an unknown name, written from a scratch trunk, read exit 0 on the base hook and exit 2 *does not check* on the repaired one.
