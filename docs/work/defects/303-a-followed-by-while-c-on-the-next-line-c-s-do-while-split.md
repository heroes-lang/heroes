- [ ] **303 — a `}` followed by `while (c)` on the next line, C's do-while split across lines, costs two messages** | a `}` closing a block, then `while (c)` on the line below: two messages, where defect 201's repair tells `} while (c)` on one line as one habit (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/braced_lines.hero` · defect 201 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 201.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    **Cause found 2026-10-04, lane b11-parse**: a `}` alone on its line is no token, the lexer having laid out the braces of a `{` that ends its line (`brace_layout.hero`, ruling 1; `heroes lex --dump-tokens`: `dedent` then `while`), so `loop_habit.past_a_statement`, which tells the habit where `c.tokens[close + 1]` is the `while`, and `loop_habit.do_tail`, which reads the tail only with its `}` on the `while`'s line, see neither. Both are in `selfhost/parse/loop_habit.hero`, batch 12's file. Not repaired.
