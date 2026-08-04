# editors/ — syntax highlighting for `.hero`

`vscode/` is a complete, minimal VS Code extension: a language declaration,
an editor configuration, and a TextMate grammar (the format VS Code,
Sublime and Zed all read). `vsix/` holds the two manifests needed to
package it. It is the M1 bonus deliverable from `docs/ROADMAP.md`.

It highlights what the spec defines — and one thing more: **the foreign
reserved words of `spec/reserved-words.md` are scoped `invalid.illegal`**,
so `struct`, `let`, `while` and the rest show up wrong *while you type*,
before the compiler is ever run. Same for an unknown escape after a
backslash (panel 008) and a malformed character literal. The thesis, in the
editor.

## Install

**Copying the folder into `~/.vscode/extensions/` does not work** on
current VS Code, and it fails silently. Since roughly 1.7x, the editor
treats `~/.vscode/extensions/extensions.json` as the registry of what is
installed: a folder that is not listed there is ignored, however correct
its `package.json` is. (Verified on 1.131: the folder was present, complete
and invisible.) Package it and let the CLI register it:

```sh
rm -rf /tmp/heroes-vsix && mkdir -p /tmp/heroes-vsix/extension
cp -r editors/vscode/* /tmp/heroes-vsix/extension/
cp editors/vsix/'extension.vsixmanifest' editors/vsix/'[Content_Types].xml' /tmp/heroes-vsix/
( cd /tmp/heroes-vsix && zip -qr /tmp/heroes-lang.vsix . )
code --install-extension /tmp/heroes-lang.vsix
```

A `.vsix` is just a zip with those two manifests beside an `extension/`
directory, so this needs no network and no toolchain — `zip` is enough.
(`npx @vscode/vsce package` does the same thing if you would rather not
keep the manifests, but it downloads a package to do it.)

Then **restart VS Code**. Open a `.hero` file: the status bar should read
**Heroes**. To confirm the editor sees it, ⇧⌘P → *Extensions: Show
Installed Extensions* → search "Heroes".

If `code` is not on your PATH: ⇧⌘P → *Shell Command: Install 'code' command
in PATH*. Cursor and Windsurf ship the same CLI under `cursor` /
`windsurf`, and read their own extension directories.

## The bolt on `.hero` files

**An extension cannot add one file icon.** VS Code takes file icons only
from a complete icon theme, which replaces the whole set — there is no
per-language hook. So the bolt requires *choosing* a theme; installing the
extension is not enough, and nothing appears until you do.

**Use the bundled theme** — ⇧⌘P → *Preferences: File Icon Theme* →
**Heroes (bolt on .hero)**, or set it directly:

```json
"workbench.iconTheme": "heroes-bolt"
```

`.hero` gets the real Aladdin Sane bolt (`vscode/icons/hero-file.svg` — the
same path the site header draws, red over blue). It is a *small* set, not a
full theme: the file types this repo actually contains get a
distinguishable shape (code, prose, data) and everything else gets a plain
sheet. That is enough that switching is not a downgrade while working here,
and it is honest about being narrow.

To go back: delete the line, or pick another theme from the same menu.

**If you already use a full icon theme** (Material Icon Theme, VSCode
Icons), keep it and add an association instead — no theme switch, no
bundled set:

```json
"material-icon-theme.files.associations": { "*.hero": "zeus" },
"vsicons.associations.files": [
  { "icon": "bolt", "extensions": ["hero"], "format": "svg" }
]
```

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
escape rule changes, change it here in the same commit, then repackage. It
is highlighting, not truth: the compiler is the only authority
(CLAUDE.md § Precedence).
