- [ ] **291 — invisible and bidirectional characters stay raw in an excerpt and a message, so U+202E reorders what the terminal shows of the line** | `x: i64 @ "<U+202E>abc"`, a type error on the line: its excerpt carries U+202E (`e2 80 ae`), which reorders what a terminal shows; U+FEFF and U+00A0 stay raw too, where 244's repair writes control characters by their code (lane b9-notext's compiler, 2026-10-04) | `selfhost/shown_char.hero` (`visible`) · `selfhost/diag_render.hero` · defect 283, whether such characters are accepted at all · **class: adjacent**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 2).

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message whose line a terminal shows in another order; the ruling 283 owes decides whether these characters reach an excerpt at all.
