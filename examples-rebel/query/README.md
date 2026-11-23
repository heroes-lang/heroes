# query

The twin of `examples/query/`: a relational engine over a table this
program writes, five thousand rows out to a file, read back, filtered,
joined, ordered, grouped and counted, written with Rebel. It prints byte for
byte what `examples/query/` prints from the same arguments, so that
program's `main.expected` is this one's test too.

    heroes run examples-rebel/query/main.hero -- build/query-table.csv

    main.hero         the program: the round trip, then the report
    data/row.hero     a row, the generator, and the table written as CSV
    data/table.hero   the table read back, strictly
    query/plan.hero   a query as a value: filters, orders, aggregates
    run/engine.hero   filter, join, order, group, aggregate
    fmt/report.hero   the answer as columns that line up

HOW IT IS CHECKED. Every number this program prints is computed **twice,
by two routes that share no line**: the engine's aggregates against a plain
loop over the same rows, the grouped totals against the ungrouped ones, the
joined count against the unjoined one. There is no published answer for a
table this program invented, so the second opinion is built rather than
fetched.

What Rebel does here that `examples/query/` writes for itself:

- `csv.format_with` and `csv.parse_with` write the table and read it back
  by RFC 4180, in place of that program's `split` and `lines_of` and the
  lines it joins by hand;
- `numbers.parse_int` and `numbers.parse_float` read a field, rounded once,
  in place of its `numbers.hero` (`atoi`, `parse_float`, `decimal`,
  `digits_from`, `quoted`, the cut-off constants and the `strtod` it binds
  from C);
- `lists.sort_by` orders the rows, stably, in place of its `sorting.hero`
  (`sort_by`, `end_of_run`); `lists.take` keeps the first rows;
  `strings.compare` orders two texts by bytes, in place of its
  `text_before`; and a group's keys are ordered by `lists.sort_by` too, in
  place of the labels it writes for each group and reads back;
- `numbers.format_float` writes a mean to two places, rounded from the
  exact value, in place of its `rounded`;
- `strings.pad_end` pads a cell; `strings.lines`, `contains`, `has_prefix`
  and `has_suffix`, in its tests, in place of its `strings.hero`.

What stays this program's own is the generator, whose rows must be these,
the strict shape each field is held to, the query as a value, and the
engine. Rebel's `numbers.parse_float` reads a number past the largest `f64`
as an infinity, as Python's `float` does; this table refuses one, so the
refusal is asked of the value in `data/table.hero`.

## Its original

The twin of [`examples/query/`](../../examples/query/), the compiler's own program, which takes no library. Run from the repository's root with that program's words, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run examples-rebel/query/main.hero -- build/query-table.csv

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 4 faces on its `packages` line, each linked here to its reference: [`csv`](https://heroes-lang.org/rebel/csv/), [`lists`](https://heroes-lang.org/rebel/lists/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

5 folders, 7 files and 1,439 lines, this page aside; of them 6 modules, `.hero` files, of 1,436 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 2 | 242 | 1 | 239 |
| `data/` | 2 | 533 | 2 | 533 |
| `fmt/` | 1 | 150 | 1 | 150 |
| `query/` | 1 | 252 | 1 | 252 |
| `run/` | 1 | 262 | 1 | 262 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
