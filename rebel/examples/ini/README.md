# ini

The twin of `examples/ini/`: a configuration file read into sections, and
the four ways a configuration file lies, written with Rebel. It prints byte
for byte what `examples/ini/` prints, so that program's `main.expected` is
this one's test too.

    heroes run rebel/examples/ini/main.hero

What Rebel does here that `examples/ini/` writes for itself:

- `strings.lines`, `trim`, `has_prefix`, `between` and `split_once` read a
  line, cut its white space, tell a header and a comment and cut a key from
  its value, in place of that program's `strings.hero` (`split_lines`,
  `trim_space`, `space_ending`, `space_at`, `has_prefix`, `has_suffix`,
  `first_at`, `holds_at`, `split_once` and its `Halves`).

What stays this program's own is the reader and its four answers: a key
before any section goes into `""`, the empty section, so it is reachable
rather than lost; a repeated key is a FAILURE, because the writer meant one
of them and nobody can tell which; a repeated section MERGES; and a line that
parses as nothing is a failure with its line number.

WHY NOT REBEL'S `ini`. Rebel reads INI as Python's `configparser` does, where
a section given twice and a key given twice are merged or refused together,
by one option (`ini.ReadOptions.duplicates`, Python's `strict`). This program
merges the one and refuses the other, which no option of `ini` asks for:
read merging, `host = a` then `host = b` is read, the last winning; read
refusing, the sample's repeated `[server]` is refused. Both readers are right
by their own rule, so the rule stays this program's, written over `strings`.

## Its original

The twin of [`examples/ini/`](../../examples/ini/), the compiler's own program, which takes no library. Run from the repository's root with no word, as that program is, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run rebel/examples/ini/main.hero

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 1 face on its `packages` line, each linked here to its reference: [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

1 folder, 2 files and 228 lines, this page aside; of them 1 module, `.hero` files, of 225 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 2 | 228 | 1 | 225 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
