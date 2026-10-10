---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **592 — a header's `#pragma redefine_extname` of a name the unit's own C calls renames the compiler's call, a link failure at exit 2** | a header of the program's own writing `#pragma redefine_extname hero_print_int hdr_print_int`, binding `labs`, builds at exit 2 with *internal error: linking failed*, *Undefined symbols: "hdr_print_int"*: the pragma attaches its label to the runtime's declaration read before the groups, so every `print` of an `i64` the compiler writes calls a name nobody defines; the guard of defect 361 gives back a macro of such a name, and a label attached by this pragma it cannot, since a second label does not replace the first; measured on this Mac (Apple clang 21) by lane b19-pack at 16:00 | `selfhost/emit/macro_guard.hero`'s guard, or the round's reading of an undefined runtime name after a header renamed it · **class: blocking**

    **Origin:** found by lane b19-pack attacking the shapes beside defect 582 (`.claude/worktrees/scratch-b15/b19-pack/shapes/`, ignored by git), filed by the lane at 16:03 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.
