# editors/ — the VS Code extension for `.hero`

`vscode/` is a complete, small VS Code extension: a language declaration,
an editor configuration, a TextMate grammar (the format VS Code reads), a
file icon theme, the compiler's diagnostics in the Problems view, and
debugging, where F5 builds the program and hands it to `lldb-dap`
(§ Debugging, below). `lldb/` holds the formatter lldb loads to show a
stopped program's values in Heroes' own syntax, which the extension hands to
lldb-dap and a terminal lldb imports with one line. `vsix/` holds the two
manifests needed to package it.
Its version is the language's, `VERSION` in `selfhost/main.hero`, and the
`spec` suite holds the two to each other. Where it goes next is
M-vscode-extension (`docs/ROADMAP.md`,
`docs/roadmap/milestones/M-vscode-extension.md`).

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
mkdir -p build/heroes-vsix/extension/lldb
cp editors/lldb/*.py build/heroes-vsix/extension/lldb/
cp LICENSE build/heroes-vsix/extension/LICENSE.txt
cp NOTICE build/heroes-vsix/extension/NOTICE.txt
cp editors/README.md build/heroes-vsix/extension/readme.md
cp editors/vsix/extension.vsixmanifest 'editors/vsix/[Content_Types].xml' build/heroes-vsix/
( cd build/heroes-vsix && zip -qrX ../heroes-lang.vsix . )
code --install-extension build/heroes-lang.vsix
```

A `.vsix` is just a zip with those two manifests beside an `extension/`
directory, so this needs no network and no toolchain — `zip` is enough.
The one script, `vscode/extension.js`, is plain JavaScript with no
dependency, so nothing is compiled or bundled either: it is copied with the
rest, and `[Content_Types].xml` gives `.js` its type.
The licence, the notice and this file are copied in rather than kept twice
in the tree, under the names `vsce` gives them: a `LICENSE` with no
extension becomes `LICENSE.txt`, and the README `readme.md`, the two files
the manifest names as the package's licence and its details page.
The formatter, `editors/lldb/heroes_lldb.py`, is copied beside the script
the same way, its `.py` given a type in `[Content_Types].xml`; only the
Python files are copied, so the `__pycache__` that lldb leaves beside the
formatter when it imports it from the tree stays out of the package.
(`npx @vscode/vsce package` does the same thing if you would rather not
keep the manifests, but it downloads a package to do it.)

Then **restart VS Code**. Open a `.hero` file: the status bar should read
**Heroes**. To confirm the editor sees it, ⇧⌘P → *Extensions: Show
Installed Extensions* → search "Heroes".

If `code` is not on your PATH: ⇧⌘P → *Shell Command: Install 'code' command
in PATH*. Cursor and Windsurf ship the same CLI under `cursor` /
`windsurf`, and read their own extension directories.

## Update

The extension is not on a marketplace, so nothing updates it for you: an
update is the same packaging run again over a newer checkout.

```sh
git pull                                 # the newest editors/ and its manifests
# then § Install's packaging commands, unchanged, up to the zip, and instead
# of its last line:
code --install-extension build/heroes-lang.vsix --force
code --list-extensions --show-versions | grep heroes-lang   # the version now installed
```

**`--force` is what makes it work between releases.** The extension's
version is the language's (`VERSION` in `selfhost/main.hero`, held equal to
both manifests by the `spec` suite), so it moves only at a release, and
between two releases every package you build carries the same number. The
CLI's own help says it: *Use '--force' argument to update to latest
version*; without it, a package whose version is already installed may be
refused as already present. A package with a higher version installs over an
older one with or without it.

Then run ⇧⌘P → *Developer: Reload Window*, or restart VS Code, so the new
grammar, configuration and `extension.js` are the ones loaded. The two
things worth checking after an update: a `.hero` file still reads
**Heroes** in the status bar, and `Run and Debug` offers **Heroes** as a
debugger (§ Debugging).

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

## Debugging

**F5 on a `.hero` file debugs it.** No `launch.json` is needed: with the
file that holds `function main()` in front, VS Code picks the one debugger
that names the `hero` language (read in its source, `debugAdapterManager.ts`),
and F5 builds the file with

```sh
heroes build <file> -O0 -o <folder>/build/debug/<its path>/<its name>
```

and hands that binary to `lldb-dap`, the LLVM project's debug adapter, which
VS Code drives through the **LLDB DAP** extension
(`llvm-vs-code-extensions.lldb-dap`). This extension writes no debugger of
its own: the C the compiler emits carries `#line` back to the `.hero` file,
so the binary's debug information already points at Heroes source, and
`-O0` is the build whose debug information describes the variables
(`heroes run` builds at `-O2`, which carries line tables only). What it
hands lldb besides the binary is one fixed formatter, `heroes_lldb.py`,
packaged beside its script: the first of the launch's `initCommands` is
`command script import "<the extension's folder>/lldb/heroes_lldb.py"`,
and any `initCommands` the configuration names run after it, unchanged
(panel 213, provisional, author ratification pending).

If the program does not build, nothing is launched: the compiler's own words
go to the **Heroes** output channel, and each diagnostic, or each `???` hole,
lands in the Problems view on its file and line, a module the program uses
included.

**What it needs**:

- the **LLDB DAP** extension, looked for before the build, so its absence
  costs no compile: F5 says so and offers to show it. Its manifest on LLVM's
  main branch asks for VS Code 1.90 or later; this extension still installs
  from 1.64, for the highlighting;
- the `lldb-dap` program, which that extension finds after the build: its
  setting `lldb-dap.executable-path` where set, else on macOS `xcrun -find
  lldb-dap`, so Xcode or the Command Line Tools are enough, else `PATH`
  (read in its source, `debug-adapter-factory.ts`). On this Mac `xcrun`
  finds Xcode's, `lldb-2103.0.34.103`;
- the `heroes` compiler: the setting `heroes.path` if it is set, else the
  workspace folder's own `heroes` (what a checkout of this repository
  builds at its root), else `heroes` on `PATH`.

**In `launch.json`**, *Add Configuration…* → **Heroes: Launch** writes one:

```json
{
  "type": "heroes",
  "request": "launch",
  "name": "Debug a Heroes program",
  "program": "${workspaceFolder}/main.hero",
  "args": []
}
```

`program` is the `.hero` file (default `${file}`); `args`, `cwd`, `env` and
`stopOnEntry` mean what they mean to lldb-dap; `buildArgs` adds words to the
build after the file, `--include` and `--library` each with a directory for
a program with an `extern` group, or `--sanitize`, and refuses `-O0`, `-O2`,
`-o`, `--emit-c` and `--dump-ir`, which are the launch's own. Every other
attribute is handed to lldb-dap unchanged (`initCommands`, after the
formatter's import, `console` and the rest of its list), and the configuration goes through the LLDB DAP
extension's own resolution, its settings included, where the installed
version offers it to other extensions.

**What a stopped program shows**, measured on this Mac over the Debug Adapter
Protocol, driving Xcode's `lldb-dap` the way VS Code does, on programs built
this way (2026-10-12; the values 2026-10-13, with the `initCommands` this
extension writes, read from its own script loaded in Node with a stand-in for
the editor's API):

- a breakpoint on a `.hero` line is verified and hit, and each Heroes frame
  of the stack is its `.hero` file and line: `h_probe_total` at
  `probe.hero:9:11`, called from `h_probe_main` at `probe.hero:17:11`;
- a local carries the name you wrote behind an index, `h1_base = 7`; the
  compiler's temporaries come with them as `t1`, `t2` and on, and every local
  is declared at the top of its function, so one not yet written shows
  whatever its memory held;
- **the values are written in Heroes' own syntax**, by the formatter: a
  record as its construction, `h2_p = Point(x: 3, y: 4)`, with children `x`
  and `y`; a `str` as a literal read by its length, `"ada"`, a NUL inside it
  as `\u{0}`; a variant as `.num(v: 42)` or `.plus`; a `T?` as its live arm
  alone, `ok(4)` or `fail(code: "odd", msg: "not even")`, where lldb shows
  both arms and the dead one's bytes; an array as
  `0x00000001032f58a0 ["ada", "grace"]` with children `[0]` and `[1]`, a map
  as `0x00000001032f5920 {"ada": 36, "grace": 85}` with children `["ada"]`
  and `["grace"]`. A summary writes sixteen elements, then `... N more`; the
  children are all there. **The address before an array or a map is
  lldb-dap's**, which writes a pointer's value before its summary: the
  summary's `--no-value`, a `void` format and a synthetic `get_value` each
  leave what `GetValue()` answers unchanged (each tried that day), and
  `frame variable` in a terminal leaves the address out;
- a slot holding the runtime's one non-value, a NULL `str`, `[T]` or
  `{K: V}`, shows `<unassigned>`; a slot of whatever the stack held, which
  the formatter cannot read as a value (a `str` with no block behind it, an
  array whose descriptor is none), shows lldb's own display of its C
  members, and no traceback reaches the debug console: none over the 307
  slots of a frame stopped before its first binding was written, nor at the
  stop where all of them were;
- **this said until 2026-10-13 that the view would improve when
  M-typed-inspection landed, with no change here.** Panel 213 measured that
  true only of a formatter the compiler writes into each binary
  (`__lldbsummaries`), a route not taken: such a summary cannot pick a `T?`'s
  live arm or walk an array's elements, and the lldb 18 the CI's Linux legs
  install has no loader for it. The route taken is a formatter lldb loads,
  and a debugger loads one only when told, so the launch tells it. The names
  keep their index, `h2_p`, which no formatter changes;
- lldb-dap's `enableAutoVariableSummaries`, which a Heroes launch turns on
  unless the configuration or the lldb-dap setting of that name says
  otherwise, still writes the fields of what the formatter leaves to lldb, a
  C struct a program binds through an `extern` group: a record's field of
  C's `struct statvfs` reads `{f_bsize:7, f_frsize:0, f_blocks:0, ...}` with
  it and `statvfs @ 0x16d942060` without (2026-10-13). Its cost, measured
  2026-10-12: the
  Variables view of a frame of the compiler itself, 877 locals, took 74 ms
  with it against 55 ms without;
- the frame below the program's `main` is the generated C's `main`, which
  lldb places at a `.c` file beside the folder the build ran in, where no
  such file is: that one frame has no source to show.

**In a terminal**, lldb loads the same formatter with one line, typed at its
prompt or written in your own `~/.lldbinit`, as you prefer:

```
command script import <path to the checkout>/editors/lldb/heroes_lldb.py
```

Then `frame variable` shows `(h_probe_Point) h2_p = Point(x: 3, y: 4)` and
`(HeroArrayHeader *) h7_names = ["ada", "grace"]`; `frame variable -P 1
h7_names` lists the elements, `-A` all of them past lldb's own limit, and a
field is reached by the name you wrote, `frame variable h12_room.corner.x`.
An element is not reached by `h7_names[1]`, which lldb reads as C's pointer
arithmetic before it asks the formatter. lldb itself prints `NULL` for a
NULL array or map before it asks any formatter. A `.lldbinit` in the
program's folder is not read unless lldb's `target.load-cwd-lldbinit` says
so (panel 213, measured).

**Restart** relaunches the binary already built, unless `launch.json`
changed in between, which is how VS Code restarts any session (read in its
source, `debugService.ts`); after an edit, stop and press F5 to rebuild.
**Run Without Debugging** (⌃F5) builds the same way and starts the binary
under lldb-dap with no breakpoints sent, also read in VS Code's source and
not run here; the `lldb-dap` of this Mac, handed `noDebug` with breakpoints,
stops at them all the same.

**What has not been run**: VS Code itself (none was available where this was
written; everything above was measured by driving `lldb-dap` and by loading
the extension's script in Node with a stand-in for the editor's API), and
Windows, where `lldb.exe` has been seen to exit `0xC0000135` before running
a command (`docs/roadmap/milestones/M-typed-inspection.md`). On Linux the
formatter was run in a terminal lldb, 18.1.3 (Ubuntu 24.04's, the CI's) and
22.1.8, and through `lldb-dap-22`, and each printed the rows this Mac's
printed (2026-10-13); Ubuntu's `lldb-dap-18` was not run.

Building and debugging run a program from the workspace, so VS Code allows
them only in a trusted workspace; the extension declares that, and a
workspace's own `heroes.path` is ignored until the workspace is trusted.
Highlighting works either way.

## Build errors in the Problems view

The extension adds a `heroes` task type and three problem matchers, which
read the compiler's text: `$heroes` the full form every verb prints,
`$heroes-brief` the one line `heroes check --brief` prints, and
`$heroes-hole` the report of a `???`. A task of the `heroes` type runs one
verb, `check`, `build`, `run` or `test`, on one file, with all three
attached and the compiler found as F5 finds it:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "type": "heroes",
      "command": "check",
      "file": "${file}",
      "label": "heroes: check the current file"
    }
  ]
}
```

*Run Task* also offers the four verbs on the `.hero` file in front. A task of
any other type can name the matchers itself, `"problemMatcher": ["$heroes",
"$heroes-brief", "$heroes-hole"]`.

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
in the same commit, run that, then repackage.

The formatter is held by the `lldb` suite's `formatter-` cases, which import
it from the tree and pin what a stopped program shows, each soundness rule
made to fire by a write through `expression` (`tests/golden/lldb/`):

```sh
./heroes run tests/harness/main.hero -- ./heroes lldb
``` It is highlighting, not
truth: the compiler is the only authority (CLAUDE.md § 12).

The same suite holds the package to itself: its two manifests say one
version, one engine, one description and one list of categories; every file
`package.json` names is there; and the language it declares is named by
`contributes.breakpoints` and by its debugger, the two places where a slip
leaves VS Code silent rather than wrong (a breakpoint that cannot be set,
an F5 that finds no debugger).
