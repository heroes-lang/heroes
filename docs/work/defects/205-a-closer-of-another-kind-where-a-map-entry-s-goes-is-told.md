- [ ] **205 — a closer of another kind where a map entry's `:` goes is told without the `{` or its closer** | `m = {1: 2` over `print(x) )`: `expected_map_entry_colon` at the `)`, *expected `:` between a map's key and its value, found `)`*, naming no `{` and no `}`; `{1: 2` over `y]` the same | `selfhost/grammar_expr.hero:567` (`map_literal`'s `line_end.expect_after` for the `:`) · defect 203's message, the separator's, which names them (`selfhost/parse/list_line.hero`, `another_kind`) · **class: adjacent**

    **Origin:** lane rec187's first pass beside defect 203, 2026-10-03, on the head's compiler and on the lane's (`scratchpad/lane-rec187/pass1/r6/r04_map_paren.hero` and `r12_map_bracket.hero`, 2026-10-03). The same reading as 203's at another site: the line is read as the map's next key, and the closer stands where its `:` goes.

    **Why it is a defect.** As 203: one reading served, the other's repair a run more (design.md §4.17's measure).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.
