# examples/

**One directory per program.** A directory here holding a `main.hero` *is* a
program, and that is the whole rule the harness knows: `tests/corpus.rs` finds
every one of them by looking, so a program joins the suite by existing and cannot
be forgotten. Beside the source, a program directory may carry

- `main.expected` — what it prints, in the `tests/golden/run/` convention: the
  stdout, optionally closed by `!exit: <code>` or `!panic: <message>`;
- `main.args` — one argument per line, handed to `args()`.

Every program is checked in **three configurations** — `-O0`, `-O2`,
`--sanitize` — its `test` blocks green in all three and its output identical in
all three. The leak balance rides along: every generated `main` ends in
`hero_runtime_check_leaks()`, which on Darwin arm64 is the only leak instrument
there is. And each of them is in `heroes mutate`'s corpus, which is where
CLAUDE.md §9 says an invariant belongs — asserted over the corpus rather than
over cases somebody thought of.

## The programs

| directory | what it is |
|---|---|
| `adventure/` | a text adventure played from a script — the world is data written in code, a turn is a **value** (a state in, a state and some lines out), and it is the program that never calls `exit`, so the corpus keeps a path across the leak gate |
| `todo/` | a todo list kept in a file — the corpus's first program that **writes** one. Every decision about *what* to write is pure next door, so the list's own tests never touch the disk |
| `logs/` | a log summariser: a **level mask** (`\|` to add a flag, `&` to test, `~` to take away, `^` to toggle, `>>` to count), `{str: i64}` tallies read back in an order the map does not have, a ranking `sort` gets wrong on purpose, and a reader that **skips and counts** a line it cannot parse rather than refusing the file |
| `assembler/` | a register machine, assembled and run: two passes because a jump may name a label further down, an encoding and its decoder written from one set of `constant`s, and a run bounded by fuel |
| `maze/` | the shortest way from `S` to `E`, drawn: breadth-first search, a queue that only grows because removing from the front copies the array, and one `{i64: i64}` doing both jobs — the visited set and the way back |
| `spreadsheet/` | a sheet of formulas, evaluated: cells that read cells, `SUM` over a rectangle, and a **cycle reported rather than a stack overflow**. The one program that ends non-zero on purpose |
| `markdown/` | Markdown to plain text, in four modules: one line's markup, a line-oriented block grammar, a page, and the program. `continue` as a `match` arm body, and two `str` parameters that must be named at the call site |
| `json/` | a JSON reader: recursive descent over one byte of lookahead, a value recursive through **both** `[T]` and `{K: V}`, and `f64`s built digit by digit because a `str` has no conversion to a number |
| `calculator/` | the acceptance program: a lexer, a recursive-descent parser and an evaluator for arithmetic with variables, across four modules — and `whole.hero` beside them, the same program in one file, so "two spellings of one program" is a claim something checks |
| `sqlite/` | §4.19's own acceptance: open, query, close, against the SDK's `sqlite3.h` with **no shim** |
| `curl/` | a variadic and an enum return, over libcurl. The one program with no `main.expected`, and the reason is read off the program rather than off a list: it prints this machine's libcurl version |

## `gallery/`

Twelve programs written to be **read**: what the language looks like when it is
used rather than tested. One theme each, in reading order, opening with the
smallest program worth compiling and ending with the shapes the middle end
needed. It is not a program directory — each file is its own program, which is
why there is no `main.hero` in it.

| file | what it shows |
|------|---------------|
| `00-first.hero`     | the compiler's first program — M-scalars-run's target, and spike 01 is its hand-written C shape |
| `01-points.hero`    | records, named construction, UFCS |
| `02-tokens.hero`    | a variant with payloads, and exhaustive `match` |
| `03-fallible.hero`  | `T?`, `fail`/`ok`, `?`, `.must()`, `.default()` |
| `04-loops.hero`     | `while cond`, `for x in xs`, cells, `break`/`continue` |
| `05-mutation.hero`  | `@` parameters: copy in, copy out, no aliasing |
| `06-generics.hero`  | generics on functions, and functions as values |
| `07-strings.hero`   | bytes versus characters, `slice`, `join` |
| `08-ffi.hero`       | `extern`: everything else comes from C |
| `09-holes.hero`     | `???`, and working skeleton-first |
| `10-maps.hero`      | `{K: V}`, and `m[k]` returning a `V?` — absence as a type |
| `11-trees.hero`     | a recursive variant: the array is the only indirection |

Every one of them is held to six properties by the test suite, because an example
nobody runs is an example that rots: it **parses** clean (`printer::gallery`), it
**resolves** clean (`resolve::tests::gallery`), it **type-checks** clean
(`types::tests::acceptance`), it **lowers and verifies** (`ir::tests::acceptance`),
it is in **canonical form** byte for byte — so no example teaches a layout
`heroes fmt` would undo — and formatting **preserves its tree**.
