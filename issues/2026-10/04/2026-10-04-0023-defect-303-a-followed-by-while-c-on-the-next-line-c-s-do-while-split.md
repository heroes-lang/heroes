- [ ] **303 — a `}` followed by `while (c)` on the next line, C's do-while split across lines, costs two messages** | a `}` closing a block, then `while (c)` on the line below: two messages, where defect 201's repair tells `} while (c)` on one line as one habit (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/braced_lines.hero` · defect 201 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 201.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.
