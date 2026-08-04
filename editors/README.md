# editors/ — syntax highlighting for `.hero`

`vscode/` is a complete, minimal VS Code extension: a language declaration,
an editor configuration, and a TextMate grammar (the format VS Code,
Sublime and Zed all read). It is the M1 bonus deliverable from
`docs/ROADMAP.md`.

It highlights what the spec defines — and one thing more: **the foreign
reserved words of `spec/reserved-words.md` are scoped `invalid.illegal`**,
so `struct`, `let`, `while` and the rest show up wrong *while you type*,
before the compiler is ever run. Same for an unknown escape after a
backslash (panel 008) and a malformed character literal. The thesis, in the
editor.

## Install (VS Code)

Copy the whole directory — a grammar file on its own does nothing, because
VS Code loads extensions by their `package.json` manifest:

```sh
cp -r editors/vscode ~/.vscode/extensions/heroes-lang
```

Then **restart VS Code** (⇧⌘P → *Developer: Reload Window* is enough).
Open a `.hero` file; the status bar should say **Heroes**. If it says Plain
Text, click it and pick Heroes — and check that
`~/.vscode/extensions/heroes-lang/package.json` exists, which is the file
that was missing if nothing happened.

Cursor, Windsurf and other forks read a different directory —
`~/.cursor/extensions`, `~/.windsurf/extensions` — same copy, same reload.

## One setting worth adding

A tab is a compile error in Heroes, and the indentation must be exactly
four spaces (design.md §4.15), so tell the editor:

```json
"[hero]": {
  "editor.insertSpaces": true,
  "editor.tabSize": 4,
  "editor.detectIndentation": false
}
```

## Keeping it honest

The grammar is a *second* description of the lexical surface, so it can
drift from `crates/heroes/src/lexer/`. When a token kind, a keyword or an
escape rule changes, change it here in the same commit. It is highlighting,
not truth: the compiler is the only authority (CLAUDE.md § Precedence).
