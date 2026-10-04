---
kind: decision
area: none
milestone: none
filed: 2026-08-11
commit: 1fef2d8c2713c0bf650abaa9d84fb24aabec6320
github: none
---

2026-08-11 | **The library and the author's file share one `Source` and cannot shadow each other.** Two collisions, both found by the R5 guard rather than by a test: the library's `fold` has a local `total`, and a user program with a top-level `total` was told its own declaration was shadowed by a line it cannot open; and the library's `map<A, B>` collided with a user's `record A`. The rule is one line in two places — a name declared in one region never shadows one declared in the other — and only that direction needs it, because a user local shadowing a library *declaration* is caught earlier and more precisely by `builtin_name_taken` | the guard panel 028 R5 asked for as insurance turned out to be the thing that found the defect, twice, on its first exposure to library code that has locals | §1.11, §4.4 | 028, 029 |
