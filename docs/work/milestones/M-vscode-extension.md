# M-vscode-extension — the extension, complete


**Scheduled, no warrant.** `editors/vscode/` already ships the TextMate grammar,
the language configuration and the icon theme; this milestone makes it an
extension somebody could install and forget about.

- **The LSP client**, speaking to M-lsp-server's `heroes lsp`: diagnostics as you
  type, hover, go-to-definition, document symbols, formatting through
  `heroes fmt`.
- **Code actions from the fixes that already exist.** §4.17's `Fix`es are tagged
  `certain | guess` and `heroes check --apply` already applies the certain ones;
  the extension surfaces exactly those as quick fixes and never the guesses. This
  costs almost nothing and is the thesis made visible in the editor — the
  likeliest mistake arrives with its repair pre-written.
- **Debugging**, and the honest shape of it first: the emitted C carries `#line`
  back to `.hero` (with `-g` repaired at M-selfhost-port), so the debug info is
  ordinary DWARF pointing at Heroes source. `lldb-dap` therefore composes with
  the generated binary without this project writing a debug adapter — which is
  CLAUDE.md §10's *"nothing if two existing invocations already compose to it"*.
  If a launch configuration cannot be expressed that way, `heroes dap` enters
  under the stopping rule like any other verb, with the reason recorded. **The
  ceiling stated here used to be design.md §2's, and M-typed-inspection is the row
  that removes it** (amended 2026-09-06, in the commit that scheduled that row):
  `p x` showing a mangled C temporary rather than a Heroes value was true of every
  build until then, and the variables pane shows whatever lldb's formatters show,
  so this milestone inherits the answer instead of documenting the ceiling. What is
  unchanged is the ruling: `lldb-dap` composes, this project writes no debug
  adapter, and an editor **consumes** formatters rather than producing them. The
  sentence is corrected rather than deleted, because a bullet that states a limit
  the compiler has already lifted funds the wrong decision at the next sitting.
- **Packaging**: a `.vsix` that installs, with `heroes doctor` as the extension's
  own health check.

*******************************************************************************
**OPEN: 1**

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

*******************************************************************************
