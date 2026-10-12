---
kind: feature
area: records
milestone: none
filed: 2026-08-12
commit: 2b38394542dd16250a4bfa0437e035ff233a8680
github: https://github.com/heroes-lang/heroes/issues/303
---

- [x] The record | **CLOSED — `bound_unit` fires:** `` `x` would hold `()`, which is no value — keep the call and drop the binding ``. Verified 2026-08-12. Was: panel? | A `()` value in *binding* position is unrejected: `x = print("a")` today. Pre-existing, shared by every option panel 014 considered, and it wants its own panel priced on its own | docs/panel/014-match-arm-body.md § Watch list | panel 003 checks non-`()` in statement position, not `()` in value position

## What was done

`bound_unit` fires on a `()` value in binding position, telling the writer that `x` would hold `()`, which is no value, and to keep the call and drop the binding; verified 2026-08-12. Landed at `2b383945`, "Four more queue entries were claims that had outlived their causes".
