# datakit

datakit: a data tool for the terminal, in the family of xsv, jq, Miller
and a small pandas. It reads tables and documents in CSV, TSV, XLSX, JSON,
JSON Lines, YAML, TOML, INI, Java properties, XML, MessagePack, CBOR and
BSON into one model, whose numbers are exact decimals and whose dates stay
dates; converts between them; selects with JSONPath; filters, groups,
aggregates and computes with an expression language of its own; runs SQL
over files loaded into SQLite; shows a table in a scrollable full-screen
view; and writes a PDF report.

Run it for real from the repository's root:

    heroes build examples-rebel/datakit/main.hero -o build/datakit/datakit
    build/datakit/datakit convert orders.csv -o orders.xlsx
    build/datakit/datakit query orders.csv --where "total > 100" --group region --agg "count(), sum(total)"
    build/datakit/datakit sql orders.csv customers.csv --query "SELECT ..."
    build/datakit/datakit view orders.csv
    build/datakit/datakit report orders.csv -o orders.pdf --group region --agg "sum(total)"

`datakit --help` lists the commands, `datakit <command> --help` their
flags. `datakit demo` runs the scripted session `main.expected` holds: it
writes sample files under `build/datakit-demo`, runs some thirty commands
over them, reads back what they wrote, and removes the folder.

Rebel's packages, and what for:

- `csv`, `xlsx`, `json`, `yaml`, `toml`, `ini`, `xml`, `msgpack`, `cbor`,
  `bson`: the formats, each read into the model and written from it
  (`formats/*_file`); Java properties are read and written here
  (`formats/properties_file`), since the program flattens nested lists in
  Spring's manner, which Rebel's `java_properties` leaves to its caller.
- `json/path`: `--path`, the RFC 9535 queries (`query/selection`).
- `big` and `numbers`: the exact decimals of the model, sums and means to
  the digit (`model/decimal`, `query/evaluate`, `query/aggregate`).
- `stats`: the deviation, the variance and the percentiles
  (`query/aggregate`).
- `scanner`: the tokens of the expression language of `--where`,
  `--select` and `--agg` (`query/expr`).
- `sql/sqlite` and `sql/build`: `datakit sql`, the rows inserted as
  parameters (`query/database`).
- `sql`: the values SQLite is handed and answers (`query/database`).
- `tui` and `term`: `datakit view` (`output/viewer`).
- `pdf`: `datakit report`, and the report read back (`output/report`,
  `output/inspect`).
- `flags`: the command line (`commands/cli`).
- `io` and `fs`: the standard streams and the files (`commands/session`,
  `formats/codec`).
- `property`: the property that every format gives back the table it was
  given (`formats/codec`'s tests).
- `hash`: the digest of a file's content (`model/digest`).
- `time`, `unicode/segment`, `strings`, `lists`, `bytes`, `path`, `os`:
  moments, widths on a screen, texts, sorting, bytes, paths, the working
  folder of the demo.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/datakit/main.hero -- demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 33 faces on its `packages` line, each linked here to its reference: [`big`](https://heroes-lang.org/rebel/big/), [`bson`](https://heroes-lang.org/rebel/bson/), [`bytes`](https://heroes-lang.org/rebel/bytes/), [`cbor`](https://heroes-lang.org/rebel/cbor/), [`csv`](https://heroes-lang.org/rebel/csv/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`ini`](https://heroes-lang.org/rebel/ini/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`json/path`](https://heroes-lang.org/rebel/json/path/), [`lists`](https://heroes-lang.org/rebel/lists/), [`msgpack`](https://heroes-lang.org/rebel/msgpack/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`pdf`](https://heroes-lang.org/rebel/pdf/), [`property`](https://heroes-lang.org/rebel/property/), [`scanner`](https://heroes-lang.org/rebel/scanner/), [`sql`](https://heroes-lang.org/rebel/sql/), [`sql/build`](https://heroes-lang.org/rebel/sql/build/), [`sql/sqlite`](https://heroes-lang.org/rebel/sql/sqlite/), [`stats`](https://heroes-lang.org/rebel/stats/), [`strings`](https://heroes-lang.org/rebel/strings/), [`term`](https://heroes-lang.org/rebel/term/), [`time`](https://heroes-lang.org/rebel/time/), [`toml`](https://heroes-lang.org/rebel/toml/), [`tui`](https://heroes-lang.org/rebel/tui/), [`unicode/segment`](https://heroes-lang.org/rebel/unicode/segment/), [`xlsx`](https://heroes-lang.org/rebel/xlsx/), [`xml`](https://heroes-lang.org/rebel/xml/), [`yaml`](https://heroes-lang.org/rebel/yaml/).

## Its folders

7 folders, 40 files and 9,279 lines, this page aside; of them 37 modules, `.hero` files, of 8,875 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 486 | 1 | 82 |
| `commands/` | 4 | 963 | 4 | 963 |
| `demo/` | 2 | 348 | 2 | 348 |
| `formats/` | 11 | 2,657 | 11 | 2,657 |
| `model/` | 7 | 1,558 | 7 | 1,558 |
| `output/` | 4 | 992 | 4 | 992 |
| `query/` | 8 | 2,275 | 8 | 2,275 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
