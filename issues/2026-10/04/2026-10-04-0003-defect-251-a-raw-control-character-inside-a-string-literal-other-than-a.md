---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: 462b4a2d8ceb154e3c45283ad0dfe4ffa49286c0
github: none
---

- [ ] **251 — a raw control character inside a string literal, other than a carriage return, is accepted in silence, invisible in the source and lost when the line is retyped** | `print("a<X>")` with X the raw byte 0x00, 0x01, 0x09, 0x1B or 0x7F: `check` exit 0 for each, where 0x0D alone is refused, `raw_carriage_return` (batch 8's round compiler at `1eb854c3`, 2026-10-04, `<scratchpad>/filings-b8/probe/raw000.hero` to `raw177.hero`); panel 066 measured the trap for CR, *compiles silently, survives `fmt` byte for byte, renders invisibly; retyping the visible text drops the byte and still compiles*, and refused CR alone | the lexer's string literal (`selfhost/lexer.hero`, `raw_carriage_return`) · `docs/panel/066-the-sixth-escape.md` · design.md §4.3's escape freeze · the NUL among these bytes is defect 245's · **class: adjacent**

    **Origin:** the coordinator, 2026-10-04, beside defect 245.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a program that differs from what it shows, panel 066's trap for every byte but one; its repair widens a refusal, which CLAUDE.md § 4 sends to a sitting, the one defect 245 owes.

    Repaired at `aa150c8c` (the escape by code, panel 192's R5, the spelling the refusal's fix writes) and at `462b4a2d` (the refusal, R2 to R4), 2026-10-05, gated by their cases and the compiler's own tests; the net is owed at the batch's close, and the seed's two generations with it.
