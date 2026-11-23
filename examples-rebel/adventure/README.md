# adventure

The twin of `examples/adventure/`: a text adventure played from a script,
written with Rebel. It prints byte for byte what `examples/adventure/`
prints from the same arguments, so that program's `main.expected` is this
one's test too.

    heroes run examples-rebel/adventure/main.hero -- examples/adventure/walkthrough.txt

    main.hero    the program: the script, the transcript, no `exit`
    game.hero    one turn: a state in, a state and some lines out
    world.hero   the map: rooms, the ways between them, what lies about

What Rebel does here that `examples/adventure/` writes for itself:

- `strings.lines`, `strings.trim` and `strings.split_once` read the script's
  commands and cut a verb from the rest of its line, in place of that
  program's `strings.hero` (`split_lines`, `trim_space`, `space_ending`,
  `space_at`) and its `first_word` and `rest_after`;
- `graphs.reachable_from` walks the map from the start for the test that
  every room can be reached, in place of its breadth-first `reachable`;
- `maps.values` gives a room's exits as the graph's edges.

What stays this program's own is the game: a turn is a value, a `Game` in and
a `Turn` out, and the program ends by running off the end of `main` rather
than by `exit`, so the runtime's count of what was never freed is asked at
its end.

## Its original

The twin of [`examples/adventure/`](../../examples/adventure/), the compiler's own program, which takes no library. Run from the repository's root with that program's words, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run examples-rebel/adventure/main.hero -- examples/adventure/walkthrough.txt

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 3 faces on its `packages` line, each linked here to its reference: [`graphs`](https://heroes-lang.org/rebel/graphs/), [`maps`](https://heroes-lang.org/rebel/maps/), [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

1 folder, 4 files and 499 lines, this page aside; of them 3 modules, `.hero` files, of 496 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 499 | 3 | 496 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
