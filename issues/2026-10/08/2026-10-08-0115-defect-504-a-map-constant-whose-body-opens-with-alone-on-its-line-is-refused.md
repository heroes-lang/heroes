---
kind: defect
area: parse
milestone: none
filed: 2026-10-08
commit: 25d458aa520756810270827e5a58759ee81545ad
github: none
---

- [ ] **504 — a map constant whose body opens with `{` alone on its line is refused three times** | `constant AGES: {str: i64}` with the body `{` on its own line, `"ziggy": 42, "mars": 7` below it and `}` under it: `check` exit 1 with `indentation_jump`, `missing_body` (*found `{`*) and `expected_end_of_line`; the same layout with an array, `[` alone on its line, checks at exit 0, and the spec's grammar takes a map literal as a value (`spec/heroes-spec.md:217`) (run by the coordinator before 01:15 on 2026-10-08 on the trunk's compiler, `<scratchpad>/batch15/fmt503/a.hero` and `c.hero`) | the parser's reading of a constant's body that begins with `{` · **class: blocking**

    **Origin:** filed by the coordinator at 01:15 on 2026-10-08, found beside defect 503 while reducing it, and run before filing.

    **Class: blocking**, 2026-10-08 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, with three messages for it.

    Repaired at `25d458aa`, 2026-10-08 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A `{` alone on its line begins a value where it stands where a statement begins and its value begins below it, under a `constant`'s head or over a map's entry (`map_alone.hero`), the lexer and the parser asking it alike; Allman's, GNU's and Whitesmiths' braces read as before; lane print's 31 shapes each `fmt` exit 0 at its fixpoint, defect 503's reproducer among them, and `heroes probe` over defect 394's file refuses 0 variants where the base refused 51 bracket and 36 paren; a parenthesised key's comment moved by `fmt` is defect 506, apart.
