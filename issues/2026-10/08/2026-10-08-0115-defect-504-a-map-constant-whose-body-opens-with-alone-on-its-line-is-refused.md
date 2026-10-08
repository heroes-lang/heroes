---
kind: defect
area: parse
milestone: none
filed: 2026-10-08
commit: none
github: none
---

- [ ] **504 — a map constant whose body opens with `{` alone on its line is refused three times** | `constant AGES: {str: i64}` with the body `{` on its own line, `"ziggy": 42, "mars": 7` below it and `}` under it: `check` exit 1 with `indentation_jump`, `missing_body` (*found `{`*) and `expected_end_of_line`; the same layout with an array, `[` alone on its line, checks at exit 0, and the spec's grammar takes a map literal as a value (`spec/heroes-spec.md:217`) (run by the coordinator before 01:15 on 2026-10-08 on the trunk's compiler, `<scratchpad>/batch15/fmt503/a.hero` and `c.hero`) | the parser's reading of a constant's body that begins with `{` · **class: blocking**

    **Origin:** filed by the coordinator at 01:15 on 2026-10-08, found beside defect 503 while reducing it, and run before filing.

    **Class: blocking**, 2026-10-08 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, with three messages for it.
