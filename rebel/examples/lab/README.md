# lab

lab: a scientific calculator and notebook, in the family of a computer
algebra system's REPL and of Pluto's reactive notebooks in small. Its
language computes with exact whole numbers and fractions of any size
(`2^128 + 1`, `1/3 + 1/6` is `1/2`, `30!`), floats and complex numbers
(`sqrt(-16)` is `4i`), vectors and matrices (an exact `solve` and `inv`,
`det`, `rank`, `rref`, the LU, QR and Cholesky decompositions,
eigenvalues, least squares, rotations), statistics over data series,
random samples drawn from a seed, the integral and the root of a function,
and plots written as PNG images (a function's graph, a histogram, a
scatter with its fitted line). A notebook is a file of named cells; a cell
runs after the cells whose names it reads, and after an edit only the
cells the edit reaches run again.

Run it for real:

    lab                                 the REPL; :help lists what it knows
    lab run notes.lab                   a notebook run, each cell reported
    lab run notes.lab --plots out --times
    echo "factor(2^64 + 1)" | lab       one line, from a pipe

`lab demo` runs the scripted demo on the notebook read from the input
(`main.stdin`): its report, its plots read back, two edits and what each
re-ran, and a REPL session, all reading the same on every run; the plots
go to `build/lab/`.

The packages of Rebel it uses, and what for:

- `scanner`, `regex`: the language's tokens, its imaginary numbers as a
  class of the program's own; a notebook's cell headers and seed;
- `big`: exact whole numbers and fractions, their powers, roots,
  factorials, primes and decimal expansions;
- `math`, `math/complex`: the functions of a float and of a complex number;
- `math/linear`: the determinant and inverse of the small float matrices,
  rotations of the plane and of space, the cross product;
- `numbers`: numbers written as text and read from it;
- `graphs`: the cells' dependencies, their order, their circles, and the
  cells an edit reaches;
- `random`: every cell's own seeded generator, the uniform, normal and
  whole-number samples, shuffling and sampling;
- `stats`: the means, variances, quantiles, correlations and fitted lines;
- `bench`: a cell or a line timed;
- `image`, `image/png`, `hash`: the plots drawn, written as PNG files, read
  back, and told by their CRC-32;
- `term`: the REPL's line editor, its completion and colours, the tables
  and columns of its answers;
- `lists`, `maps`, `strings`, `errors`, `flags`, `fs`, `io`, `path`, `time`:
  sorting, the names of a session, text, failures, the command line, the
  files, the input, the plots' paths, the timings' spans.

## Its demo

Run from the repository's root with the words of `main.args` and `main.stdin` on its input, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/lab/main.hero -- demo < rebel/examples/lab/main.stdin

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 24 faces on its `packages` line, each linked here to its reference: [`bench`](https://heroes-lang.org/rebel/bench/), [`big`](https://heroes-lang.org/rebel/big/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`graphs`](https://heroes-lang.org/rebel/graphs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`image`](https://heroes-lang.org/rebel/image/), [`image/png`](https://heroes-lang.org/rebel/image/png/), [`io`](https://heroes-lang.org/rebel/io/), [`lists`](https://heroes-lang.org/rebel/lists/), [`maps`](https://heroes-lang.org/rebel/maps/), [`math`](https://heroes-lang.org/rebel/math/), [`math/complex`](https://heroes-lang.org/rebel/math/complex/), [`math/linear`](https://heroes-lang.org/rebel/math/linear/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`path`](https://heroes-lang.org/rebel/path/), [`random`](https://heroes-lang.org/rebel/random/), [`regex`](https://heroes-lang.org/rebel/regex/), [`scanner`](https://heroes-lang.org/rebel/scanner/), [`stats`](https://heroes-lang.org/rebel/stats/), [`strings`](https://heroes-lang.org/rebel/strings/), [`term`](https://heroes-lang.org/rebel/term/), [`time`](https://heroes-lang.org/rebel/time/).

## Its folders

11 folders, 45 files and 10,239 lines, this page aside; of them 41 modules, `.hero` files, of 9,853 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 525 | 1 | 139 |
| `bench/` | 1 | 116 | 1 | 116 |
| `cli/` | 1 | 139 | 1 | 139 |
| `demo/` | 2 | 144 | 2 | 144 |
| `eval/` | 13 | 3,435 | 13 | 3,435 |
| `lang/` | 4 | 1,044 | 4 | 1,044 |
| `linalg/` | 4 | 1,414 | 4 | 1,414 |
| `notebook/` | 4 | 961 | 4 | 961 |
| `numbers/` | 5 | 1,381 | 5 | 1,381 |
| `plot/` | 4 | 775 | 4 | 775 |
| `repl/` | 2 | 305 | 2 | 305 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
