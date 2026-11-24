# maze

The twin of `examples/maze/`: the shortest way from `S` to `E`, drawn,
written with Rebel. It prints byte for byte what `examples/maze/` prints
from the same arguments, so that program's `main.expected` is this one's
test too.

    heroes run examples-rebel/maze/main.hero -- examples/maze/sample.maze

    main.hero    the program: arguments, the file, the drawing
    solve.hero   breadth-first search, and the path it found
    grid.hero    the rectangle, and a position as one number

What Rebel does here that `examples/maze/` writes for itself:

- `strings.lines` reads the file's rows, in place of that program's
  `strings.hero` (`split_lines`);
- `sets.from_list` and `sets.contains` hold the squares of the way and the
  squares one step away, in place of its map built by hand and its
  `contains`;
- `lists.reverse` turns the way round, in place of its `reversed`;
- `unicode.from_code_point` writes a mark as text, in place of its
  `one_char`, which knows only the three marks it spells out.

What stays this program's own is the maze and its search: a rectangle with
one start and one goal, a position as one number, the four steps from a
square, the breadth-first search (`solve.hero` says why Rebel's
`graphs.shortest_path` is not asked instead), and the drawing, where `S` and
`E` keep their own letters.

## Its original

The twin of [`examples/maze/`](../../examples/maze/), the compiler's own program, which takes no library. Run from the repository's root with that program's words, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run examples-rebel/maze/main.hero -- examples/maze/sample.maze

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 4 faces on its `packages` line, each linked here to its reference: [`lists`](https://heroes-lang.org/rebel/lists/), [`sets`](https://heroes-lang.org/rebel/sets/), [`strings`](https://heroes-lang.org/rebel/strings/), [`unicode`](https://heroes-lang.org/rebel/unicode/).

## Its folders

1 folder, 4 files and 480 lines, this page aside; of them 3 modules, `.hero` files, of 477 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 480 | 3 | 477 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
