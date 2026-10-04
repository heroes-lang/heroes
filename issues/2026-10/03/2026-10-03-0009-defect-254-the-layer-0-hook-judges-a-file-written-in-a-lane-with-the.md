---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: none
github: none
---

- [ ] **254 — the layer-0 hook judges a file written in a lane with the compiler of the session's directory, and skips the whole compiler's check there** | `.claude/hooks/fmt_check.py` takes the compiler as `<cwd>/heroes` and the file's path relative to `<cwd>`: a `selfhost/` module written under `.claude/worktrees/<lane>/` by a session whose directory is the trunk is formatted by the trunk's compiler and reads as `.claude/worktrees/<lane>/selfhost/...`, which is not `selfhost/`, so `heroes check selfhost/main.hero` never runs for it (read by the coordinator, 2026-10-04; batch 8's FFI lane, 2026-10-03, its report's finding 7) | `.claude/hooks/fmt_check.py:75` to `:119` · `.claude/rules/verification.md` § A suite is the last judge, layer 0 · **class: improvement**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 7); the hook read by the coordinator, 2026-10-04.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach; no program moves.
