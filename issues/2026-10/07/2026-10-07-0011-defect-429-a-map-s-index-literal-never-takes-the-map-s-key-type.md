---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: 0bade8bc04dbf9cc38437fbe96d976e5102e2fb3
github: none
---

- [ ] **429 — a map's index literal never takes the map's key type** | `names: {u8: str} = {11: "eleven"}`, then `names[11]`: `type_mismatch`, *expected `u8`, found `i64`* (the coordinator, 00:11); spec § 2 says a literal takes the type its context asks for; lane b13-gen402 reads it reaching `m[k] @ v` the same way, unrun | `access.index_type`, which synthesises the index, `selfhost/check/` · **class: blocking**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-gen402's report of the evening before (*found beside*); reproduced by the coordinator on round b13's compiler.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, § 2's rule not applied to an index.

    Repaired at `0bade8bc`, 2026-10-07 (lane b13-gen402): a map's index with no type of its own (a number literal, a `.case`, an empty literal) is checked against the map's key type, for a read and a store (`check/access.hero`'s `key_asked`, applied by `walk.hero`'s `index_of`); cases `run/` and `check/fixedbugs-429-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; the card filled by the coordinator.
