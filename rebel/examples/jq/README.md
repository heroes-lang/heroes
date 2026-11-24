# jq

A small jq written with Rebel's `json/jq`: a filter run over the values of
files, or of the standard input where no file is named, each value's
outputs written one a line. `--yaml` reads the input as YAML documents and
`--toml` as one TOML document, both read into a `json.Value` by their own
packages, so the same filter reads all three, as yq does.

    heroes run examples-rebel/jq/main.hero -- '.albums[] | .title' albums.json
    heroes run examples-rebel/jq/main.hero -- --yaml -r '.name' config.yaml

Options, before the filter: `-c` one line a value, `-r` a text written
raw, `-n` the filter run once on `null`, `-s` every value read into one
list, `--arg name text` and `--argjson name json` a variable, `--yaml` and
`--toml` the input's format. `$ENV` reads the process's environment and
`now` the clock. The exit code is jq's: 5 where an error ends a run, 2 where
a file or an option is refused, a halt's own code where it halts.

## Its demo

Run from the repository's root with the words of `main.args` and `main.stdin` on its input, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/jq/main.hero -- -r '.albums[] | .title' < examples-rebel/jq/main.stdin

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 7 faces on its `packages` line, each linked here to its reference: [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`json/jq`](https://heroes-lang.org/rebel/json/jq/), [`os`](https://heroes-lang.org/rebel/os/), [`time`](https://heroes-lang.org/rebel/time/), [`toml`](https://heroes-lang.org/rebel/toml/), [`yaml`](https://heroes-lang.org/rebel/yaml/).

## Its folders

1 folder, 5 files and 292 lines, this page aside; of them 1 module, `.hero` files, of 284 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 292 | 1 | 284 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
