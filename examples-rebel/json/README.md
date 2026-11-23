# json

The twin of `examples/json/`: a JSON document in, its shape and its
canonical form out, written with Rebel. It prints byte for byte what
`examples/json/` prints from the same arguments, so that program's
`main.expected` is this one's test too.

    heroes run examples-rebel/json/main.hero -- examples/json/sample.json title

What Rebel does here that `examples/json/` writes for itself:

- `json.parse` reads the document by RFC 8259, its depth bounded and each
  number rounded once, in place of that program's reader in its `json.hero`
  (`parse`, its `Reader`, the object, array, string, escape, surrogate and
  number readers, and the `strtod` it binds from C);
- `json.format_canonical` writes a value as RFC 8785 gives it, one line, the
  members ordered by their keys, in place of its writer (`render`, `quoted`,
  `number_text`);
- `json.member` finds a member of an object, in place of its `field`;
- `maps.values` gives an object's values in the order they were written, in
  place of its loop over `keys`.

What stays this program's own is its question of a document, how many values
it holds and how deep they nest, a walk with an arm for each of the six
kinds; and its three exits, 0 for a document it read, 1 for one it could
not, 2 for being asked wrong.

Where the two answer otherwise, none of it in that program's expected
output: a text that is not JSON fails `invalid` here, its message opening
`line L, column C:`, and `not_json` there, opening `byte N:`; a missing
member fails `not_found` here, or `invalid` on a value that is no object,
and `no_field` there for both. RFC 8785's form, written here, orders keys by
their UTF-16 code units, writes `\b` and `\f` by name and a small number as
`1e-7`; that program's form orders keys by their bytes, writes `\u0008` and
`\u000c`, and writes `1e-07`.

## Its original

The twin of [`examples/json/`](../../examples/json/), the compiler's own program, which takes no library. Run from the repository's root with that program's words, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run examples-rebel/json/main.hero -- examples/json/sample.json title

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 2 faces on its `packages` line, each linked here to its reference: [`json`](https://heroes-lang.org/rebel/json/), [`maps`](https://heroes-lang.org/rebel/maps/).

## Its folders

1 folder, 2 files and 172 lines, this page aside; of them 1 module, `.hero` files, of 169 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 2 | 172 | 1 | 169 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
