---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: 23d00fbe0cc3777821c7850142cf805d48ce73b2
github: none
---

- [ ] **415 — a map literal's entries never take the type its annotation asks for** | `m: {str: u8} = {"a": 1}` is refused `type_mismatch`, *expected `{str: u8}`, found `{str: i64}`*, where `xs: [u8] = [1, 2]` checks clean; spec § 2 says a literal takes the type its context asks for (the coordinator's re-run on round b13's compiler at `5fc393e3`, 23:03); measured by the lane, not re-run: `{"a": .plus}` is `cannot_infer`, and a generic function value inside a map literal is refused for the same reason | the checker's typing of a map literal against an expected type, `selfhost/check/` · **class: blocking**

    **Origin:** lane b13-gen402, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; reproduced by the coordinator.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, the spec's own rule (§ 2, line 43) not applied to a map's entries.

    Repaired at `23d00fbe`, 2026-10-07 (lane b13-gen402): a map literal's keys and values checked against the key and value types its context asks for, as an array literal's elements are; cases `run/` and `check/fixedbugs-415-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; merged into round b13 by the coordinator, the card filled by the coordinator since the lane was told not to edit it.
