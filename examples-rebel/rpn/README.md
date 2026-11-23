# rpn

The twin of `examples/rpn/`: Reverse Polish evaluated on a stack, written
with Rebel. It prints byte for byte what `examples/rpn/` prints, so that
program's `main.expected` is this one's test too.

    heroes run examples-rebel/rpn/main.hero

What Rebel does here that `examples/rpn/` writes for itself:

- `strings.words` cuts an expression into its words between runs of
  Unicode's white space, in place of that program's `strings.hero`
  (`fields`, `space_at`);
- `numbers.parse_int` reads a signed decimal integer and refuses any other
  text and one past an `i64`, in place of its `numbers.hero` (`atoi`, its
  two cut-off constants and `quoted`).

What stays this program's own is the stack: the three arity mistakes, none
of them a syntax error, and the check before a division, since integer
division by zero aborts (spec § 7) and an evaluator reading its input must
refuse a typed zero rather than end the process.

## Its original

The twin of [`examples/rpn/`](../../examples/rpn/), the compiler's own program, which takes no library. Run from the repository's root with no word, as that program is, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run examples-rebel/rpn/main.hero

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 2 faces on its `packages` line, each linked here to its reference: [`numbers`](https://heroes-lang.org/rebel/numbers/), [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

1 folder, 2 files and 187 lines, this page aside; of them 1 module, `.hero` files, of 184 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 2 | 187 | 1 | 184 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
