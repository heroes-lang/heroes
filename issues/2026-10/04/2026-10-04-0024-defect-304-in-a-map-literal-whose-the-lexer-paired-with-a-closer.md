- [ ] **304 — in a map literal whose `{` the lexer paired with a closer further down, a statement line is read as a key, told twice without the `{`** | the shape of defect 204 in a map literal: a binding line below a map left open is read as a key, two messages that name no `{` (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/unclosed.hero`, `selfhost/closers.hero` · defect 204, the list's twin repaired · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 204.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, and neither names its cause.
