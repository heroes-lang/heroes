# explorer

explorer: a file manager in a window of the system's own widgets. Its
window shows the tree of folders and the bookmarks, the folder's entries
with their size, type and date in the person's locale, ordered by the
locale's collation, and a preview: a text's first lines in whatever
encoding it was saved in, an image decoded with its size, an archive's
members. It searches by name and by content, keeps bookmarks in a TOML
file, copies, moves, renames (one entry, or several by a pattern),
deletes to a trash it keeps and puts back, compresses to ZIP or tar.gz and
extracts, shows a file's checksums, undoes and redoes, shows a progress
window with Cancel for every long operation, and follows the folder when
another program changes it.

Run for real, on macOS (the native side of `ui` on Windows and Linux is
still being written, and the explorer says so there):

    heroes build examples-rebel/explorer/main.hero -o explorer
    ./explorer ~/Documents
    ./explorer --locale de-DE --settings my-explorer.toml .

The settings file is `explorer.toml` under the system's configuration
folder, in `heroes-explorer/`; `store/settings.hero` documents its keys.
`--demo` plays a scripted session on the headless side, on a tree it
writes under `build/explorer/`, and prints the window after each step:
`main.args` and `main.expected`.

The Rebel packages it uses, and for what:

- `ui`: the window, its widgets, menus, timer and dialogs, natively or
  headless; `flags`: this command line.
- `fs`, `fs/watch`, `path`: entries listed, copied, moved, renamed and
  stamped, the folder shown watched for other programs' changes, paths
  joined, named and checked to stay inside a folder.
- `containers/ordered`: the listing kept in order as entries come and go,
  the row of an entry its rank and the entry after a deleted one its
  nearest key.
- `i18n`, `i18n/collate`, `i18n/dates`, `i18n/numbers`: the locale, names
  ordered as it orders them with numbers read as numbers, dates and ages
  in its words, sizes in its digits.
- `unicode/encodings`: texts previewed and searched in the encoding they
  were saved in.
- `image`, `image/png`, `image/jpeg`: pictures decoded, turned upright and
  scaled for the preview.
- `archive`, `compress`: ZIP and tar archives listed, written and
  extracted in steps, gzip files read.
- `hash`: CRC-32, CRC-64 and xxHash64 of a file, in steps.
- `regex`: search patterns, wildcards and the rename by pattern.
- `toml`: the settings, the bookmarks edited in place with their comments
  kept, and the trash's notes.
- `time`, `os`, `io`, `bytes`, `strings`, `numbers`, `maps`: the clock the
  explorer keeps, the system's folders and applications, streams read in
  pieces, and text throughout.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/explorer/main.hero -- --demo

`main.skips` names the platforms where the demo is built and not run, and why.

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 26 faces on its `packages` line, each linked here to its reference: [`archive`](https://heroes-lang.org/rebel/archive/), [`bytes`](https://heroes-lang.org/rebel/bytes/), [`compress`](https://heroes-lang.org/rebel/compress/), [`containers/ordered`](https://heroes-lang.org/rebel/containers/ordered/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`fs/watch`](https://heroes-lang.org/rebel/fs/watch/), [`hash`](https://heroes-lang.org/rebel/hash/), [`i18n`](https://heroes-lang.org/rebel/i18n/), [`i18n/collate`](https://heroes-lang.org/rebel/i18n/collate/), [`i18n/dates`](https://heroes-lang.org/rebel/i18n/dates/), [`i18n/numbers`](https://heroes-lang.org/rebel/i18n/numbers/), [`image`](https://heroes-lang.org/rebel/image/), [`image/jpeg`](https://heroes-lang.org/rebel/image/jpeg/), [`image/png`](https://heroes-lang.org/rebel/image/png/), [`io`](https://heroes-lang.org/rebel/io/), [`maps`](https://heroes-lang.org/rebel/maps/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`regex`](https://heroes-lang.org/rebel/regex/), [`strings`](https://heroes-lang.org/rebel/strings/), [`time`](https://heroes-lang.org/rebel/time/), [`toml`](https://heroes-lang.org/rebel/toml/), [`ui`](https://heroes-lang.org/rebel/ui/), [`unicode/encodings`](https://heroes-lang.org/rebel/unicode/encodings/).

## Its folders

10 folders, 43 files and 9,924 lines, this page aside; of them 39 modules, `.hero` files, of 8,912 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 1,144 | 1 | 132 |
| `app/` | 6 | 1,454 | 6 | 1,454 |
| `demo/` | 4 | 1,002 | 4 | 1,002 |
| `locale/` | 1 | 192 | 1 | 192 |
| `model/` | 7 | 1,497 | 7 | 1,497 |
| `ops/` | 8 | 2,400 | 8 | 2,400 |
| `preview/` | 4 | 751 | 4 | 751 |
| `search/` | 2 | 320 | 2 | 320 |
| `store/` | 2 | 344 | 2 | 344 |
| `views/` | 4 | 820 | 4 | 820 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
