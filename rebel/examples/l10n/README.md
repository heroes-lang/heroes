# l10n

l10n: a translation management tool in gettext's family, for a program
written in Heroes with HTML templates beside it. It extracts the messages
the sources ask `i18n` to translate and the templates ask Jinja's gettext
forms for; keeps a gettext catalogue for each language, merged with the
sources as they change, its plural forms computed from CLDR's rules; checks
what a reader would see wrong; shows each message as a reader of a
language sees it, dates, money, sorted lists and link hosts included;
reviews the changes between versions; reports coverage; imports and
exports catalogues as CSV, XLSX, JSON and YAML, in whatever encoding a
translator's tool wrote them; and compiles them to the `.mo` files gettext
reads.

Run it for real from the repository's root:

    heroes build rebel/examples/l10n/main.hero -o build/l10n/l10n
    cd my-project && ../build/l10n/l10n init --domain shop --locales de,fr,ja
    l10n extract && l10n merge && l10n export --locale de -o de.xlsx
    l10n import de.xlsx --locale de && l10n check && l10n compile

`l10n --help` lists the commands, `l10n <command> --help` their flags.
`l10n demo` runs the scripted session `main.expected` holds: it writes a
sample shop under `build/l10n/demo`, runs some thirty commands over it at
one fixed moment, and removes the folder.

Rebel's packages, and what for:

- `i18n`: the gettext catalogues read and written as `.po` and `.mo`, the
  CLDR plural rules and their samples, the `Plural-Forms` checked by
  gettext's own evaluator, tags and `Accept-Language` read
  (`catalog/*`, `plural/*`, `project/locale_tags`).
- `i18n/locales`: canonical tags (`iw` is `he`), the likely script and
  region behind a currency and a page's direction (`project/locale_tags`).
- `i18n/numbers`, `i18n/dates`, `i18n/collate`: the previews' numbers,
  money, dates and relative times, and lists in each language's order
  (`preview/samples`); the reports' own numbers (`project/session`).
- `url/idna`: a link's host checked as DNS needs it, and written in ASCII
  in a rendered page (`check/text_checks`, `preview/render`).
- `unicode`, `unicode/normalize`, `unicode/encodings`: texts put in NFC
  on import and checked for it, a pseudo-translation's accents composed,
  final punctuation in any script, and a file's encoding found and read
  (`formats/rows`, `check/text_checks`, `preview/pseudo`,
  `formats/text_decode`).
- `unicode/segment`: a report's columns aligned by a terminal's widths
  (`review/table`).
- `regex`: the templates' gettext forms, placeholders, tags and links
  (`extract/templates`, `check/*`, `preview/*`).
- `diff`: a merge's guesses by similarity, and a review's word and unified
  diffs (`catalog/merge`, `review/changes`).
- `csv`, `xlsx`, `json`, `yaml`: the formats translators work in
  (`formats/*`); `yaml` reads the settings too (`project/config`).
- `sets`, `maps`, `lists`: coverage by set arithmetic, multisets of
  placeholders and tags, orders kept (`review/coverage`, `check/*`).
- `flags`, `io`, `fs`, `path`, `os`, `time`, `bytes`: the command line,
  the streams, the files, the demo's folder and its fixed clock.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/l10n/main.hero -- demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 26 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`csv`](https://heroes-lang.org/rebel/csv/), [`diff`](https://heroes-lang.org/rebel/diff/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`i18n`](https://heroes-lang.org/rebel/i18n/), [`i18n/collate`](https://heroes-lang.org/rebel/i18n/collate/), [`i18n/dates`](https://heroes-lang.org/rebel/i18n/dates/), [`i18n/locales`](https://heroes-lang.org/rebel/i18n/locales/), [`i18n/numbers`](https://heroes-lang.org/rebel/i18n/numbers/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`lists`](https://heroes-lang.org/rebel/lists/), [`maps`](https://heroes-lang.org/rebel/maps/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`regex`](https://heroes-lang.org/rebel/regex/), [`sets`](https://heroes-lang.org/rebel/sets/), [`time`](https://heroes-lang.org/rebel/time/), [`unicode`](https://heroes-lang.org/rebel/unicode/), [`unicode/encodings`](https://heroes-lang.org/rebel/unicode/encodings/), [`unicode/normalize`](https://heroes-lang.org/rebel/unicode/normalize/), [`unicode/segment`](https://heroes-lang.org/rebel/unicode/segment/), [`url/idna`](https://heroes-lang.org/rebel/url/idna/), [`xlsx`](https://heroes-lang.org/rebel/xlsx/), [`yaml`](https://heroes-lang.org/rebel/yaml/).

## Its folders

11 folders, 43 files and 9,774 lines, this page aside; of them 40 modules, `.hero` files, of 9,430 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 438 | 1 | 94 |
| `catalog/` | 4 | 1,318 | 4 | 1,318 |
| `check/` | 4 | 883 | 4 | 883 |
| `commands/` | 5 | 1,102 | 5 | 1,102 |
| `demo/` | 2 | 476 | 2 | 476 |
| `extract/` | 5 | 1,402 | 5 | 1,402 |
| `formats/` | 6 | 1,251 | 6 | 1,251 |
| `plural/` | 2 | 735 | 2 | 735 |
| `preview/` | 5 | 955 | 5 | 955 |
| `project/` | 3 | 682 | 3 | 682 |
| `review/` | 3 | 532 | 3 | 532 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
