- [ ] **290 — `lex --dump-tokens` and `parse --dump-ast` write a string's control characters raw, so a file's escape sequences reach the terminal** | a string literal holding ESC and `[2Jboom`: the two dumps write the bytes to stdout as they are, a terminal's clear-screen among them; their `--json` forms escape it (lane b9-notext's compiler, 2026-10-04) | `selfhost/cli/lex.hero`, `selfhost/print/dump.hero` · defect 244, the same bytes through a diagnostic's excerpt, repaired in batch 9 · **class: blocking**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 1).

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): 244's reason, what a terminal shows is no longer what the compiler wrote; the lane left the class to the coordinator, a dump being an artifact as well as a message.

    Repaired at `28cef56d`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
