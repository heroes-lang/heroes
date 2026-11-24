# todo

The twin of `examples/todo/`: a todo list kept in a file, written with
Rebel. It prints byte for byte what `examples/todo/` prints from the same
arguments, so that program's `main.expected` is this one's test too.

    heroes run rebel/examples/todo/main.hero -- examples/todo/sample.todo
    heroes run rebel/examples/todo/main.hero -- mine.todo add buy milk
    heroes run rebel/examples/todo/main.hero -- mine.todo done 2

    main.hero   the program: arguments, the file, the one write
    list.hero   the list, the format, and every change to it

What Rebel does here that `examples/todo/` writes for itself:

- `numbers.parse_int` reads the number after `done`, in place of that
  program's `numbers.hero` (`atoi`, its two cut-off constants and
  `quoted`);
- `strings.lines` and `strings.trim` read the file's lines and the text of a
  task, in place of its `strings.hero` (`split_lines`, `trim_space`,
  `space_ending`, `space_at`);
- `lists.skip` takes the words after the command, in place of its loop in
  `rest_of`.

What stays this program's own is the list and its file: the one write is in
`save`, and everything that decides WHAT to write is pure, in
`list.hero`, so its tests never touch the disk; `complete` answers a new
list because value semantics leaves nothing else it could answer.

## Its original

The twin of [`examples/todo/`](../../examples/todo/), the compiler's own program, which takes no library. Run from the repository's root with that program's words, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run rebel/examples/todo/main.hero -- examples/todo/sample.todo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 3 faces on its `packages` line, each linked here to its reference: [`lists`](https://heroes-lang.org/rebel/lists/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

1 folder, 3 files and 391 lines, this page aside; of them 2 modules, `.hero` files, of 388 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 3 | 391 | 2 | 388 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
