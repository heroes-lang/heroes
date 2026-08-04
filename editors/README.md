# editors/ — syntax highlighting for `.hero`

`heroes.tmLanguage.json` is a TextMate grammar (the format VS Code, Sublime
and Zed all read). It is the M1 bonus deliverable from `docs/ROADMAP.md`.

It highlights what the spec defines — and one thing more: **the foreign
reserved words of `spec/reserved-words.md` are scoped `invalid.illegal`**,
so `struct`, `let`, `while` and the rest show up wrong *while you type*,
before the compiler is ever run. Same for an unknown escape after a
backslash (panel 008) and a malformed character literal. The thesis, in the
editor.

## Using it in VS Code

Without packaging an extension, point VS Code at it in your user settings
(or drop the file into a minimal extension's `syntaxes/`). Quickest local
route:

```
mkdir -p ~/.vscode/extensions/heroes-lang/syntaxes
cp editors/heroes.tmLanguage.json ~/.vscode/extensions/heroes-lang/syntaxes/
```

with a `package.json` declaring one `grammars` contribution for language id
`hero`, extension `.hero`, scope `source.hero`. Reload the window.

## Keeping it honest

The grammar is a *second* description of the lexical surface, so it can
drift from `crates/heroes/src/lexer/`. When a token kind, a keyword or an
escape rule changes, change it here in the same commit. It is highlighting,
not truth: the compiler is the only authority (CLAUDE.md § Precedence).
