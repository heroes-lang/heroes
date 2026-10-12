# editors/ — syntax highlighting for `.hero`

`vscode/` is a complete, minimal VS Code extension: a language declaration,
an editor configuration, a TextMate grammar (the format VS Code reads), and a
file icon theme. `vsix/` holds the two manifests needed to package it. Its
version is the language's, `VERSION` in `selfhost/main.hero`, and the `spec`
suite holds the two to each other. Where it goes next is M-vscode-extension
(`docs/ROADMAP.md`, `docs/roadmap/milestones/M-vscode-extension.md`).

It highlights what the spec defines — and one thing more: **the foreign
reserved words of `spec/reserved-words.md` are scoped `invalid.illegal`**,
so `struct`, `let`, `int` and the rest show up wrong *while you type*,
before the compiler is ever run. Same for an unknown escape after a
backslash (panel 008), a malformed character literal, a number the lexer
refuses (`07`, `1e5`, `0X1F`, `1__000`), a tab outside a comment, a raw
control character, `//` and a string in backticks. The thesis, in the
editor.

The contextual words (`as`, `link`, `package`, and the FFI's marks `tag`,
`owned`, `acquires` and the rest) are painted only where the parser reads
them: `as` on a `use` line, `link` and `package` after an `extern` line's
header, the others on an `extern` group's indented lines, never before a `:`.
Anywhere else each is an ordinary name, as it is to the compiler.

## Install

**Copying the folder into `~/.vscode/extensions/` does not work** on
current VS Code, and it fails silently. Since roughly 1.7x, the editor
treats `~/.vscode/extensions/extensions.json` as the registry of what is
installed: a folder that is not listed there is ignored, however correct
its `package.json` is. (Verified on 1.131: the folder was present, complete
and invisible.) Package it and let the CLI register it. From the
repository's root, staging in its own `build/`, which git ignores:

```sh
rm -rf build/heroes-vsix build/heroes-lang.vsix
mkdir -p build/heroes-vsix/extension
cp -R editors/vscode/. build/heroes-vsix/extension/
cp LICENSE build/heroes-vsix/extension/LICENSE.txt
cp NOTICE build/heroes-vsix/extension/NOTICE.txt
cp editors/README.md build/heroes-vsix/extension/readme.md
cp editors/vsix/extension.vsixmanifest 'editors/vsix/[Content_Types].xml' build/heroes-vsix/
( cd build/heroes-vsix && zip -qrX ../heroes-lang.vsix . )
code --install-extension build/heroes-lang.vsix
```

A `.vsix` is just a zip with those two manifests beside an `extension/`
directory, so this needs no network and no toolchain — `zip` is enough.
The licence, the notice and this file are copied in rather than kept twice
in the tree, under the names `vsce` gives them: a `LICENSE` with no
extension becomes `LICENSE.txt`, and the README `readme.md`, the two files
the manifest names as the package's licence and its details page.
(`npx @vscode/vsce package` does the same thing if you would rather not
keep the manifests, but it downloads a package to do it.)

Then **restart VS Code**. Open a `.hero` file: the status bar should read
**Heroes**. To confirm the editor sees it, ⇧⌘P → *Extensions: Show
Installed Extensions* → search "Heroes".

If `code` is not on your PATH: ⇧⌘P → *Shell Command: Install 'code' command
in PATH*. Cursor and Windsurf ship the same CLI under `cursor` /
`windsurf`, and read their own extension directories.

## The bolt on `.hero` files

**The language brings its own icon, from VS Code 1.64.** The extension
declares the bolt as the `hero` language's icon, which VS Code shows *"if the
current file icon theme only has a generic file icon for the language"* (the
1.64 release notes, *Language default icons*). A theme that has an icon of its
own for `.hero` shows its own, and a theme such as *Minimal* or *None* shows
none unless it opts in with `showLanguageModeIcons`. The extension's engine is
`^1.64.0` for this reason. That the bolt appears under each particular theme
has not been checked here.

**Or use the bundled theme** — ⇧⌘P → *Preferences: File Icon Theme* →
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
bundled set. The icon names below are those themes' own, and have not been
verified here:

```json
"material-icon-theme.files.associations": { "*.hero": "zeus" },
"vsicons.associations.files": [
  { "icon": "bolt", "extensions": ["hero"], "format": "svg" }
]
```

## Four spaces, by default

A tab is a compile error in Heroes, and the indentation must be exactly
four spaces (design.md §4.15), so the extension sets these for `.hero` files
itself, as its defaults:

```json
"[hero]": {
  "editor.insertSpaces": true,
  "editor.tabSize": 4,
  "editor.detectIndentation": false
}
```

## Keeping it honest

The grammar is a *second* description of the lexical surface, so it can
drift from the lexer's own tables: the keywords, the contextual words and
the foreign words in `selfhost/keywords.hero`, the tokens in
`selfhost/token.hero` and `selfhost/punctuation.hero`, the escapes in
`selfhost/escape.hero` and `selfhost/code_escape.hero`, the numbers in
`selfhost/number.hero`, the character literals in `selfhost/literals.hero`,
and the `f` literal's holes in `selfhost/lex_interp.hero`. The `spec`
suite holds the grammar to them, its words read from those files and its
rules for each literal run against the lexer:

```sh
./heroes run tests/harness/main.hero -- ./heroes spec
```

When a token kind, a keyword or an escape rule changes, change the grammar
in the same commit, run that, then repackage. It is highlighting, not
truth: the compiler is the only authority (CLAUDE.md § 12).
