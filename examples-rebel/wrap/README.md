# wrap

The twin of `examples/wrap/`: text folded to a width, written with Rebel. It
prints byte for byte what `examples/wrap/` prints, so that program's
`main.expected` is this one's test too.

    heroes run examples-rebel/wrap/main.hero

What Rebel does here that `examples/wrap/` writes for itself:

- `unicode/segment.wrap` folds the text, as many words on a line as fit, a
  word longer than the line left whole on a line of its own
  (`break_long_words` off), the spaces at a break taken away, in place of
  that program's loop in `wrapped`;
- `strings.words` cuts the text into its words between runs of Unicode's
  white space, in place of its `strings.hero` (`fields`, `space_at`);
- `unicode/segment.width_of` and `math.max_i64` measure the widest line, in
  place of its loop over `len()`.

What stays this program's own is its four answers, which `wrapped` asks of
Rebel: a word that fits exactly stays on the line; a word longer than the
width overflows rather than being broken, because breaking a URL or an
identifier is worse than a long line; every run of white space, a line break
included, is one space, so the text's words are joined by one space before
the fold; and no line ends in a space.

Where the two fold otherwise, none of it in that program's expected output:
`unicode/segment.wrap` breaks where UAX #14 allows, after a hyphen among
them, and counts a line's width in the columns a terminal draws, a wide
character two; `examples/wrap/` breaks at spaces alone and counts bytes.

## Its original

The twin of [`examples/wrap/`](../../examples/wrap/), the compiler's own program, which takes no library. Run from the repository's root with no word, as that program is, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run examples-rebel/wrap/main.hero

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 3 faces on its `packages` line, each linked here to its reference: [`math`](https://heroes-lang.org/rebel/math/), [`strings`](https://heroes-lang.org/rebel/strings/), [`unicode/segment`](https://heroes-lang.org/rebel/unicode/segment/).

## Its folders

1 folder, 2 files and 161 lines, this page aside; of them 1 module, `.hero` files, of 158 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 2 | 161 | 1 | 158 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
