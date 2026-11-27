# build

build: a build tool in make's and ninja's family. It reads `build.toml`
in the folder it is run in: rules (a command template naming `$in`,
`$out` and variables), targets each made by a rule from its inputs, phony
targets naming others, and variables. It checks the dependency graph
before running anything, a cycle told by its path and a missing input by
every target that needs it; it decides each step by the content digest of
its inputs and of its command, kept in `.build_log` beside the outputs,
never by file times; it reads the depfiles a step writes in make's format
to learn the headers no build file names; and it runs up to `-j N` steps
at once on worker threads, each step's output caught apart and printed
whole when the step is collected, the first failing step stopping the
build.

Run for real, in a folder holding a `build.toml`:

    build                      # the default targets
    build -j 8 out/app         # one target, eight steps at once
    build --explain            # why each step runs
    build -n                   # what would run, running nothing
    build --clean              # remove what the steps make
    build -t commands          # targets, commands, deps or graph
    build CFLAGS='-O0 -g'      # a variable over the file's own

`build --demo` writes a small project under `build/build-demo/`, whose
steps are this program's own helpers (`build --helper ...`), builds it,
changes one input and rebuilds only what depends on it, shows a cycle and
a missing input refused and a failing step stopping a build, and prints
an account that is the same on every run.

The packages of Rebel it uses, and what for:

- `graphs`: the dependency graph's order, its cycles told by their path,
  the targets a run reaches, the stages a parallel build hands out, and
  the order a scope's variables are evaluated in;
- `hash`: XXH64, the digest of every input, output and command;
- `process`: each step's program run with no shell, its output caught,
  its time limit kept; a command shown quoted;
- `threads`: how many threads the runtime runs at once, which `-j` is
  held to; the steps run on threads of the language's own (`spawn`);
- `fs`: outputs' folders made, files read, hashed, removed, the log
  appended and written whole, the folder's lock;
- `path`: every path made one node, written with `/` and cleaned, and put
  back on the disk;
- `regex`: the variables a template names, the names a build file gives,
  and the directives of the demo's toy language;
- `toml`: the build file;
- `flags`: the command line;
- `time`: the steps' time limits and durations, and the build's;
- `json`: the log's records, and what crosses to a worker thread and back;
- `strings`, `bytes`, `io`, `os`, `errors`: commands read into words, a
  step's output read as text, the error stream, the processors counted
  and the program's own path, failures told in one line.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/build/main.hero -- --demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 17 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`graphs`](https://heroes-lang.org/rebel/graphs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`process`](https://heroes-lang.org/rebel/process/), [`regex`](https://heroes-lang.org/rebel/regex/), [`strings`](https://heroes-lang.org/rebel/strings/), [`threads`](https://heroes-lang.org/rebel/threads/), [`time`](https://heroes-lang.org/rebel/time/), [`toml`](https://heroes-lang.org/rebel/toml/).

## Its folders

11 folders, 30 files and 5,591 lines, this page aside; of them 27 modules, `.hero` files, of 5,301 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 419 | 1 | 129 |
| `cli/` | 1 | 184 | 1 | 184 |
| `demo/` | 3 | 453 | 3 | 453 |
| `engine/` | 9 | 1,626 | 9 | 1,626 |
| `graph/` | 1 | 328 | 1 | 328 |
| `helpers/` | 1 | 306 | 1 | 306 |
| `load/` | 3 | 698 | 3 | 698 |
| `model/` | 1 | 145 | 1 | 145 |
| `state/` | 4 | 773 | 4 | 773 |
| `tools/` | 1 | 169 | 1 | 169 |
| `vars/` | 2 | 490 | 2 | 490 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
