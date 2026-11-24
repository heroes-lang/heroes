# interpreter

The twin of `examples/interpreter/`: a whole little language, scanned,
parsed and run, written with Rebel. It prints byte for byte what
`examples/interpreter/` prints from the same arguments, and ends with the
same status, so that program's `main.expected` is this one's test too.

    heroes run examples-rebel/interpreter/main.hero -- examples/interpreter/scripts/tour.script

    main.hero         the program: the script, its output, its exit
    lex/token.hero    what a token is
    lex/scan.hero     source text into tokens
    syn/tree.hero     the shapes a parsed program has
    syn/cursor.hero   a place in a token list
    syn/expr.hero     tokens into an expression tree
    syn/parse.hero    statements, blocks and a whole program
    run/value.hero    a value, and the state one run carries
    run/expr.hero     what an expression means
    run/eval.hero     what a statement does

A language's scanner, parser and evaluator are the program, and Rebel takes
the parts no language of its own owns. What Rebel does here that
`examples/interpreter/` writes for itself:

- `numbers.parse_int` reads a number a script writes and refuses one past an
  `i64`, in place of that program's `numbers.hero` (`atoi`, its two cut-off
  constants and `quoted`);
- `math.add_i64`, `subtract_i64`, `multiply_i64`, `divide_i64`,
  `remainder_i64` and `negate_i64` do the script's arithmetic, failing where
  Heroes' own operators would abort, in place of its guards before each
  operator (`plus`, `minus`, `times`, `size_of`, its `MIN` and `MAX`);
- `sets.from_list` tells a parameter named twice, in place of its `repeats`;
- the built-ins `map`, `join`, `all` and `filter` walk a list, in place of
  its `util/each.hero` (`counted`, `first_of`, `joined`, `every`), which
  Rebel leaves to the language as it leaves every built-in.

Rebel's `scanner` reads Go's tokens, its escapes and its numbers; this
language has four escapes and a newline that is a token, so its scanner
stays its own.

## Its original

The twin of [`examples/interpreter/`](../../examples/interpreter/), the compiler's own program, which takes no library. Run from the repository's root with that program's words, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run examples-rebel/interpreter/main.hero -- examples/interpreter/scripts/tour.script

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 4 faces on its `packages` line, each linked here to its reference: [`math`](https://heroes-lang.org/rebel/math/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`sets`](https://heroes-lang.org/rebel/sets/), [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

4 folders, 11 files and 2,397 lines, this page aside; of them 10 modules, `.hero` files, of 2,394 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 2 | 310 | 1 | 307 |
| `lex/` | 2 | 637 | 2 | 637 |
| `run/` | 3 | 600 | 3 | 600 |
| `syn/` | 4 | 850 | 4 | 850 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
