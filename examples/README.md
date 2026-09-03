# examples/

**One directory per program.** A directory here holding a `main.hero` *is* a
program, and that is the whole rule the net knows: `tests/harness/suite_corpus.hero`
finds every one of them by looking, so a program joins the suite by existing and
cannot be forgotten. Beside the source, a program directory may carry

- `main.expected` — what it prints, in the `tests/golden/run/` convention: the
  stdout, optionally closed by `!exit: <code>` or `!panic: <message>`;
- `main.args` — one argument per line, handed to `args()`;
- data the program reads, named in `main.args` (`sample.json`, `sample.maze`, …).

Every program is checked in **three configurations** — `-O0`, `-O2`,
`--sanitize` — its `test` blocks green in all three and its output identical in
all three. The leak balance rides along: every generated `main` ends in
`hero_runtime_check_leaks()`, which on Darwin arm64 is the only leak instrument
there is, and at least one program must reach the end of `main` without calling
`exit` so that the gate is crossed at all (`adventure/` is that program). Four
more suites read this directory: `suite_warnings` (the generated C compiles with
zero clang warnings), `suite_emission` (the generated C is compared byte for byte
against `tests/emission/examples-<dir>-main.c`), `suite_descriptors` (every
descriptor in that C is defined and referenced) and `suite_canonical` (`heroes fmt`
returns every file unchanged). And each program is in `heroes mutate`'s corpus,
which is where CLAUDE.md §9 says an invariant belongs — asserted over the corpus
rather than over cases somebody thought of. **Measured 2026-09-03: `heroes mutate`
over this whole directory is refused today**, because it checks each file from its
own directory and `shapes/`'s two nested modules are `unknown_module` from there;
the fix is M-corpus-depth step 1 in `docs/work/SCHEDULED.md`, and until it lands
the corpus score is the sum of one run per directory (measurement 014).

A program must either test itself or bind C. A program that binds a C library is
skipped on a machine that lacks the library — the skip is read off the compiler's
own diagnostic, never off a list of names — and a machine that skips more than
half the corpus is reported as not testing it. A program whose output is the
machine's (the clock, a random source, the environment, this machine's libcurl
version) carries no `main.expected` and is checked only for not failing; that is
why `curl/` has none and why no program here reads a clock.

## The programs

Each row gives the reason the program exists, which is how the corpus is indexed:
by what a program can get wrong, not by its size. Where a program was written to
close a measured gap, its own header comment carries the measurement.

| directory | what it is |
|---|---|
| `adventure/` | a text adventure played from a script — the world is data written in code, a turn is a **value** (a state in, a state and some lines out), and it is the program that never calls `exit`, so the corpus keeps a path across the leak gate |
| `argv/` | its own command line read two ways, `args()` and `args_checked()`: the only caller of the second, which the spec-budget ledger's row 3541 owed a use |
| `assembler/` | a register machine, assembled and run: two passes because a jump may name a label further down, an encoding and its decoder written from one set of `constant`s, and a run bounded by fuel |
| `binarytrees/` | 135,854 nodes of a recursive `variant` built, walked and dropped, against the benchmarks game's published output at N = 10 — the first program here written to allocate at volume, and its `main.expected` **is** the downloaded file, byte for byte. The two halves live in a `[Tree]` because a record holding itself is `error[no_size]`, so the program is also where the language's one rule about recursive types is visible in a working program rather than in a diagnostic |
| `board/` | a grid of cells that grow and die, and a scoreboard keyed twice: `[[i64]]`, nested maps, `if` as an expression and `_` as a catch-all arm, all measured at zero uses before it |
| `calculator/` | the acceptance program: a lexer, a recursive-descent parser and an evaluator for arithmetic with variables, across four modules — and `whole.hero` beside them, the same program in one file, so "two spellings of one program" is a claim something checks |
| `csv/` | CSV read properly: quotes, commas inside them, and a doubled quote that means one |
| `ctime/` | a calendar date taken apart through `struct tm` — the only program writing `tag` and `partial`. The struct is 36 bytes on Windows and 56 on macOS, both measured, with identical field widths: the twenty bytes between them are what `partial` protects. It replaced `filestat/`, whose `st_size` was 4 bytes on Windows (journal 029) |
| `curl/` | a variadic and an enum return, over libcurl. No `main.expected`, and the reason is read off the program: it prints this machine's libcurl version |
| `dates/` | days between two dates, and the four leap-year rules |
| `deck/` | a generic deck — deal, cut, reverse, riffle: generics declared by the **program** rather than by the library, instantiated at `str` and at `i64` with the answers asserted to agree |
| `diff/` | the longest common subsequence, filled into a `[[i64]]` table |
| `fannkuch/` | every permutation of seven cards, flipped until the 1 is on top, against **two** published oracles that agree: the benchmarks game's output at N = 7 and OEIS A000375, which has never heard of the benchmark. The checksum depends on the order the permutations are generated in — dictionary order gives -502 where the published answer is 228, measured before the program was written — so it is the corpus's one program whose correctness rests on a generator rather than on a formula |
| `floats/` | `inf` and `nan`, and the one comparison that survives them (`x != x`) |
| `ini/` | a configuration file into `{str: {str: str}}`, built from text, with four named refusals |
| `json/` | a JSON reader: recursive descent over one byte of lookahead, a value recursive through **both** `[T]` and `{K: V}`, and `f64`s built digit by digit because a `str` has no conversion to a number |
| `logs/` | a log summariser: a **level mask** (`\|` to add a flag, `&` to test, `~` to take away, `^` to toggle, `>>` to count), `{str: i64}` tallies read back in an order the map does not have, a ranking `sort` gets wrong on purpose, and a reader that **skips and counts** a line it cannot parse rather than refusing the file |
| `markdown/` | Markdown to plain text, in four modules: one line's markup, a line-oriented block grammar, a page, and the program. `continue` as a `match` arm body, and two `str` parameters that must be named at the call site |
| `maze/` | the shortest way from `S` to `E`, drawn: breadth-first search, a queue that only grows because removing from the front copies the array, and one `{i64: i64}` doing both jobs — the visited set and the way back |
| `palette/` | colours packed into one integer: the corpus's hex, octal and binary literals, `~` and `^`. The lines stand as `heroes fmt` produced them — it deleted the C parentheses around `mode & MASK == 0`, because here `&` binds tighter than a comparison, the other way round from C |
| `pipeline/` | lines in, report out: the program that **cannot be written without `use … as`**, because two modules in two directories are both named `source` |
| `raylib/` | §4.19's ladder rung 5: a window opens. The acceptance test for `package "raylib"` — the machine is asked where the library is, and answers differently on every platform from one spelling |
| `readings/` | a week of sensor readings through `map`, `filter`, `fold`, `find`, `any` and `all`, called as `xs.map(f)`: the six library functions the spec calls *"written in Heroes"*, called by one program in the repository before this one, and the UFCS spelling by none. It found a compiler defect on its first run — two generic calls chained through UFCS shared one key (journal 029) |
| `roman/` | Roman numerals both ways, two parallel arrays walked in step |
| `routes/` | the shortest way between stations: breadth-first search over `{str: [str]}`, the visited set a `{str: bool}` |
| `rpn/` | reverse Polish arithmetic on a stack, guarding the division-by-zero abort before dividing |
| `sdl/` | The acceptance test for panel 055's allow-list, and for the two things it did not measure. SDL was bindable through **neither** clause until 2026-08-14; `-D`, `-U`, `-Wl,-framework,<name>` and `-Wl,-rpath,<dir>` opened it. It binds **SDL3**, because SDL2 on this machine is a shim that opens a modal dialog and a `SDL2main` that turns any program linking it into a windowed application |
| `shapes/` | six modules in three directories, `use geom/point` written from three depths: the acceptance program for `use` paths (M-package-layout) |
| `sieve/` | primes by crossing out, checked against published counts — 25 below 100, 168 below 1000, 1229 below 10,000 — with a second algorithm sharing no line with the first. The one program whose answers **somebody else wrote down**; everything else here asserts what its own functions produce, which catches a change and not a mistake |
| `spreadsheet/` | a sheet of formulas, evaluated: cells that read cells, `SUM` over a rectangle, and a **cycle reported rather than a stack overflow**. The one program that ends non-zero on purpose |
| `sqlite/` | §4.19's own acceptance: open, query, close, against the SDK's `sqlite3.h` with **no shim** |
| `template/` | text with holes filled from a map; a missing key refused, `render_or` for a default, and `{{`/`}}` escaped both ways — a test found the rule asymmetric before the compiler or a reading did |
| `todo/` | a todo list kept in a file — the corpus's first program that **writes** one. Every decision about *what* to write is pure next door, so the list's own tests never touch the disk |
| `tree/` | an expression tree in an **arena**: nodes linked by index, a vocabulary wider than any one input builds, and three walks over it (evaluate, depth, render). It is here because the corpus had **no tree with a vocabulary**, and that absence hid a compiler defect until the compiler's own syntax tree was ported to Heroes to find it — a case carrying an optional record that nothing constructs still gets its `==` emitted, and the emitted C named a descriptor nobody defined (exit 2, 2026-08-16). Declaring more shapes than you build is what every real syntax tree does; this program makes the corpus check it on every commit |
| `widths/` | numbers changing width: the seven conversions the corpus had never called (`to_i8`, `to_i16`, `to_u8`, `to_u16`, `to_u32`, `to_u64`, `to_f32`), and the types they reach |
| `words/` | word frequency, and the `# ORDER:` discipline on `keys(m)` — an order the map does not have is imposed, and the comment says by what |
| `wrap/` | text folded to a width, every off-by-one edge named and asserted |

## `gallery/`

Twelve programs written to be **read**: what the language looks like when it is
used rather than tested. One theme each, in reading order, opening with the
smallest program worth compiling and ending with the shapes the middle end
needed. It is not a program directory — each file is its own program, which is
why there is no `main.hero` in it and why the corpus floor is one under the
directory count.

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

What holds the gallery, today, and by which instrument — because an example
nobody runs is an example that rots. Every file **checks clean**: `heroes mutate
examples/gallery` is a row of `tests/harness/suite_surface.hero` at exit 0, and
`mutate` refuses a corpus that does not compile. Every file is in **canonical
form** byte for byte, so no example teaches a layout `heroes fmt` would undo:
`tests/harness/suite_canonical.hero` walks `examples/` whole. And formatting
**preserves the tree**, a guard inside `heroes fmt` itself, tested where it lives
(`selfhost/cli/syntax_cmds.hero`). The site's code blocks are slices of these
files by `data-src` and `data-lines`, which is why they are not renumbered.

What no suite asserts today: that a gallery file **builds** in the three
configurations. `00-first.hero` is built by several surface rows and
`09-holes.hero` is built to exit 1 on purpose; the other ten are checked and
formatted and never lowered. That is filed with the milestone that widens this
directory next (`docs/work/SCHEDULED.md`, M-corpus-depth).
