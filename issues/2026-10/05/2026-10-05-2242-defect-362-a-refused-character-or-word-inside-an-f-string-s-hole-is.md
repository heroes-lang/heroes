---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: 7101a5e65d55bf37c81448c1979637f6c978d89b
github: none
---

- [ ] **362 — a refused character or word inside an f-string's hole is dropped in silence, so `f"{a; b}"` prints `1`** | on the base, `x = f"{a; b}"` with `b` used elsewhere checks clean and `run` prints `1`, the `; b` gone; `x = f"{a + int}"` checks clean at exit 0 and `run` stops at *internal error: the lowered program is not well formed* at exit 2 (measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `fs2`, `fs3`) | `selfhost/lex_interp.hero`, the hole's reader, and the parser's reading of a hole · lane b12-parse12's `7101a5e6` · **class: blocking**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0 and an exit 2 where the author can be told, a refused character and a reserved word accepted in silence.

    Repaired at `7101a5e6`, 2026-10-05, inside lane b12-parse12's defect 312 (the hole's reader reports what it refused), gated by that repair's cases and the compiler's own tests; the net is owed at the batch's close.
