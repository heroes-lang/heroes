---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: self
github: none
---

- [x] **279 — eleven living files cite a closed defect as an item of `docs/work/DEFECTS.md`, which since 2026-10-04 holds no item** | `git grep -n 'DEFECTS\.md' -- selfhost docs/learn`: four `selfhost/` comments (`check/decls.hero:423` for 042, `check/reaches.hero:1`, `handles.hero:1` and `mutate/handles.hero:2` for 029) and seven `docs/learn/` notes (029, 030, 031, 129 among them), each closed and its record in `docs/records/done/` (read by the coordinator, 2026-10-04) | those eleven files · the closing records they should name · **class: improvement**

    **Origin:** lane b8-defects, 2026-10-04 (its reply's *found beside*: seven notes and four comments, left as history); counted again by the coordinator.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a citation that resolves to a front page and not to the defect; no program moves.

## The repair

The four `selfhost/` comments and the open notes cite each defect by its number
(`defect 029`, `defects 033 and 035`, `defect 042`), its identifier, and no
longer through a list that holds no item; the code comments that cited a defect
as an entry of `docs/work/DONE.md` were repointed the same way. `git grep -n
'DEFECTS\.md' -- selfhost` finds nothing.

**Closed 2026-10-05** in the commit that ticks this item, which deleted the maps
and merged the roadmap's folders
(`issues/2026-10/05/2026-10-05-0100-no-map-is-kept-and-the-roadmap-holds-the-milestones.md`).

