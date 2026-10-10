---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **582 — a group header's unclosed `#pragma pack(push, 1)` packs the unit's own records, and a record passed between modules reads a wrong value** | `packed.h` is `#pragma pack(push, 1)` then `#include <stdlib.h>`, named by `main`'s group; `other.hero` declares `record Pair` of `a: u8` and `b: i64`; `main` builds `Pair(a: 1, b: 123456789)` through `other.make` and prints `p.b`, `other.second(p)` and `other.second(Pair(a: 2, b: 987654321))`: the trunk's compiler builds at exit 0 and prints `1513209474796486656`, `6103393301`, `8372224`, one module laying `Pair` out packed and the other not; reproduced by the coordinator at 13:55 (`.claude/worktrees/scratch-b15/r582/`) | the guard's close, `runtime/heroes_guard_close.h` and `selfhost/emit/macro_guard.hero`: `#pragma pack()` and its family reset after the groups, as panel 205's R1 raises the warnings again; the shapes beside: `pack(2)`, `pack(push)` with no pop inside a header of the program's own, MSVC's `#pragma pack`, `__attribute__((packed))` default via a macro · **class: blocking**

    **Origin:** found by lane b18-guard beside panel 205's landing (its final reply and notes, `.claude/worktrees/scratch-b15/b18-guard/notes.txt`, ignored by git), filed by the coordinator at 13:56 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0.
