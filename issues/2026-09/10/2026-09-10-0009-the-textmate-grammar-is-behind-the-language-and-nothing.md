- [ ] **M-vscode-extension** | the TextMate grammar is behind the language, and nothing judges either highlighter | `editors/vscode/syntaxes/heroes.tmLanguage.json` · `site/src/lib/highlight.ts` · `.claude/rules/diagnostics-and-goldens.md` § A new surface form

    **Origin:** author instruction 2026-09-10 — a step will be needed at some
    point to improve the syntax colouring, in VS Code and in editors generally
    but on the site too, because introducing new things means the colouring has
    to know about them. That is CL-036's walk asked about a tool the walk did not
    name. The rule was widened the same day; this is the repair it now demands.

    **Three gaps, measured, and the asymmetry is the finding.**
    `site/src/lib/highlight.ts` reads a string with holes as
    `selfhost/lex_interp.hero` reads it and cites that module by name.
    `editors/vscode/syntaxes/heroes.tmLanguage.json` has **no `f"…"` rule**, so a
    hole is painted as ordinary string text; admits **four escapes** where
    `spec:76` gives five in a string, so a legal `\r` is painted
    `invalid.illegal` and a correct program shows as an error; and knows **none**
    of `owned`, `tag`, `partial`, `link`, `package`, `as`. One of the two kept up
    because somebody remembered.

    **The check is the durable half**, and its seam exists: `suite_spec.hero:49`
    already reads `spec/reserved-words.md`, so a row that compares each
    highlighter's word list against `selfhost/keywords.hero`'s **21** keywords and
    `selfhost/inventory.hero`'s **39** built-ins has somewhere to live. **Nothing
    judged either file before** — `grep tmLanguage tests/harness/` was empty, and
    `suite_records.hero` reads `editors/vscode/icons` for the SVG-path rule alone —
    which is how the grammar rotted in silence.

    **Not filed as a defect, deliberately** (author decision the same day): the
    list would go from 0 to 1 and block the next tag, and the repair touches no
    compiler line and no spec token, so it can ride any commit.
