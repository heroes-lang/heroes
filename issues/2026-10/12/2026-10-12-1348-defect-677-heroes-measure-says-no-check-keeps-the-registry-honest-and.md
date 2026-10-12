---
kind: defect
area: cli
milestone: none
filed: 2026-10-12
commit: self
github: https://github.com/heroes-lang/heroes/issues/2026
---

- [x] **677 — `heroes measure` says no check keeps the registry honest, and two checks do** | `heroes measure spec/reserved-words.md` printed *No ceiling judges `spec/reserved-words.md` and no check keeps it honest (panel 123 R5)*, and `--refresh` on it refused with the same clause, where `suite_spec`'s `REGISTRY_TOKENS` pins the file's count and, since defect 676, its `registered` check holds every row to the compiler's own message | `selfhost/cli/measure.hero` (the branch for a file no ceiling judges) · `selfhost/measure/pinned.hero` (`run_refresh`) · **class: blocking**

    **Origin:** found 2026-10-12 by the lane that repaired defect 676, which ran `heroes measure spec/reserved-words.md` to re-pin the registry's count and read the clause; read again by the coordinator the same day, at `3fe09293`.

    **Class: blocking**, 2026-10-12: a false message. The clause was false of the registry from the day `REGISTRY_TOKENS` began to pin it, before defect 676 added the second check; it is true of a file nothing judges, a draft or `design.md`, which is why the repair drops the clause rather than naming the registry.

## The repair

Both sentences drop *and no check keeps it honest*; what stays, *No ceiling
judges `<path>` (panel 123 R5), so this is a count and not a verdict*, is true
of every file the branch takes, the registry among them. The tests of
`pinned.hero` assert the refusal's exit code, 2, on `design.md`,
`docs/ROADMAP.md` and the registry, and read no text, so none moves. The seed is
regenerated over two generations in the same commit, the two sentences being
literals of `seed/heroes.c`.

**Closed 2026-10-12** at the commit that writes this section.
