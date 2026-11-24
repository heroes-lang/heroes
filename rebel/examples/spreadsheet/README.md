# spreadsheet

The twin of `examples/spreadsheet/`: a file of cells, evaluated, written
with Rebel. It prints byte for byte what `examples/spreadsheet/` prints from
the same arguments, and ends with the same status, so that program's
`main.expected` is this one's test too.

    heroes run examples-rebel/spreadsheet/main.hero -- examples/spreadsheet/sample.cells

    main.hero      the program: arguments, the file, the report
    formula.hero   the expression language, and the cycle check
    sheet.hero     cell names, ranges, and the file's shape

It ends **1** when any cell failed, which is what makes the exit code carry
information: a shell can ask whether the sheet is sound without reading it.

What Rebel does here that `examples/spreadsheet/` writes for itself:

- `numbers.parse_int` reads a cell's row and `numbers.parse_float` a
  formula's literal, rounded once, in place of that program's `numbers.hero`
  (`atoi`, `parse_float`, `decimal`, `digits_from`, `quoted`, the cut-off
  constants, and the `strtod` it binds from C);
- `strings.lines`, `trim`, `split_once` and `has_prefix` read the file, in
  place of its `strings.hero` (`split_lines`, `trim_space`, `space_ending`,
  `space_at`, `split_once`, `first_at`, `holds_at`);
- `strings.pad_end` pads a name to its column, in place of its loop in
  `padded`;
- `unicode.from_code_point` writes a column's letter, in place of its table
  of letters sliced by hand;
- `math.max_i64` keeps the highest row, in place of its `larger`.

What stays this program's own is the formula language, its parser and the
chain of cells under evaluation that refuses a cycle. Rebel's
`numbers.parse_float` reads a literal past the largest `f64` as an infinity,
as Python's `float` does; this program refuses one, `does_not_fit`, so the
refusal is asked of the value in `formula.hero`.

## Its original

The twin of [`examples/spreadsheet/`](../../examples/spreadsheet/), the compiler's own program, which takes no library. Run from the repository's root with that program's words, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run examples-rebel/spreadsheet/main.hero -- examples/spreadsheet/sample.cells

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 4 faces on its `packages` line, each linked here to its reference: [`math`](https://heroes-lang.org/rebel/math/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`strings`](https://heroes-lang.org/rebel/strings/), [`unicode`](https://heroes-lang.org/rebel/unicode/).

## Its folders

1 folder, 4 files and 705 lines, this page aside; of them 3 modules, `.hero` files, of 702 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 705 | 3 | 702 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
