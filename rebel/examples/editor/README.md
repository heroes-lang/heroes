# editor

editor: a terminal text editor in the manner of nano and micro. It opens,
edits and saves files in any encoding of the WHATWG standard (UTF-8,
UTF-16 with or without its mark, windows-1252 and the ISO-8859 family,
Shift_JIS, EUC-JP, GB18030, Big5, EUC-KR and the rest), the encoding found
by a byte order mark, guessed from the bytes or named, and saved back in
it with the file's own line breaks. The cursor moves by grapheme cluster,
word and line, and wide characters and combining marks are drawn as a
terminal draws them. Undo and redo take back a word, a run of deletions,
a paste or a whole "replace all" at once; search and replace by regular
expression with `$1` and `${name}`; a diff view shows the buffer against
its file on disk; a file another program changes is offered for reload,
the reload itself one undo away; several buffers stand as tabs in the
order of their paths; keys and colours come from a settings file.

Run for real, on a terminal:

    heroes build examples-rebel/editor/main.hero -o editor
    ./editor notes.txt todo.md
    ./editor --encoding latin1 old-invoice.txt
    ./editor --config my-editor.toml --tab-width 8 src/*.txt

The settings file is `editor.toml` (or `.yaml`, `.json`, `.ini`) in
`<config dir>/heroes-editor/`, else the one `--config` names;
`settings/settings.hero` documents its tables. F1 lists the keys. `--demo`
plays a scripted session headless, on files it writes under
`build/editor-demo/`, and prints it: `main.args` and `main.expected`.

The Rebel packages it uses, and for what:

- `tui` and `term`: the full screen, its frames written cell by cell, the
  tabs, the prompt's line, the help's table; keys and styles.
- `unicode/segment`: graphemes, word boundaries and the columns a text
  takes; `unicode`: code points, controls and letters.
- `unicode/encodings`: every file read and written in its encoding,
  detected or named; `bytes`: byte order marks, a key's bytes, the bytes a
  character is written with.
- `diff`: the diff view and its count of lines; `regex`: search and
  replace with captures.
- `fs` and `fs/watch`: files read whole, saved atomically, and watched;
  `path`: a tab's name; `time`: the waits.
- `containers/deque`: the bounded history of undo and redo;
  `containers/ordered`: the buffers in the order of their paths.
- `config`, `json` and `flags`: the settings in layers and this command
  line; `os`: the settings' folder and the environment.
- `strings` and `numbers`: text and numbers throughout.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/editor/main.hero -- --demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 20 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`config`](https://heroes-lang.org/rebel/config/), [`containers/deque`](https://heroes-lang.org/rebel/containers/deque/), [`containers/ordered`](https://heroes-lang.org/rebel/containers/ordered/), [`diff`](https://heroes-lang.org/rebel/diff/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`fs/watch`](https://heroes-lang.org/rebel/fs/watch/), [`json`](https://heroes-lang.org/rebel/json/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`regex`](https://heroes-lang.org/rebel/regex/), [`strings`](https://heroes-lang.org/rebel/strings/), [`term`](https://heroes-lang.org/rebel/term/), [`time`](https://heroes-lang.org/rebel/time/), [`tui`](https://heroes-lang.org/rebel/tui/), [`unicode`](https://heroes-lang.org/rebel/unicode/), [`unicode/encodings`](https://heroes-lang.org/rebel/unicode/encodings/), [`unicode/segment`](https://heroes-lang.org/rebel/unicode/segment/).

## Its folders

12 folders, 34 files and 6,572 lines, this page aside; of them 31 modules, `.hero` files, of 6,329 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 391 | 1 | 148 |
| `buffers/` | 3 | 624 | 3 | 624 |
| `demo/` | 3 | 600 | 3 | 600 |
| `editor/` | 6 | 1,424 | 6 | 1,424 |
| `files/` | 3 | 548 | 3 | 548 |
| `history/` | 1 | 337 | 1 | 337 |
| `input/` | 2 | 428 | 2 | 428 |
| `search/` | 2 | 362 | 2 | 362 |
| `settings/` | 2 | 548 | 2 | 548 |
| `terminal/` | 1 | 89 | 1 | 89 |
| `text/` | 3 | 611 | 3 | 611 |
| `view/` | 4 | 610 | 4 | 610 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
