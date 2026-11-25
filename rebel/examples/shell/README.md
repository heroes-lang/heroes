# shell

shell: a POSIX-flavoured command shell. It reads the shell's language
(words quoted with `'`, `"` and `\`; `$VAR`, `${VAR:-default}` and the
other parameter forms; `$(...)` and backquotes; `$((...))`; globs with
`*`, `?` and `[...]`) and runs pipelines of any length, each program a
child process joined to the next by a pipe; redirections `<`, `>`, `>>`,
`2>&1` and here-documents; `&&`, `||` and `;`; `if`, `while`, `until`,
`for` and `case`; functions; the builtins `cd`, `export`, `unset`, `exit`,
`echo`, `jobs`, `fg`, `wait`, `history` and the others `help` lists; and
background jobs started with `&`, kept in a table. On a terminal it reads
each line with a line editor that keeps a history and completes with Tab.
A mistake is told with the line and the column of the script it is in.

Run for real:

    shell                      an interactive shell, on a terminal
    shell script.sh a b        a script, with `$1` and `$2`
    shell -c 'echo $1' x       one command line
    shell -ex script.sh        options of `set` before the script
    shell --demo < main.stdin  the demo: the session on the input played
                               in `build/shell-demo/`

The binary is also each of the small programs of `tools/kit` when it runs
under that program's name, as BusyBox is; the demo links it so.

The Rebel packages it uses, and for what:
- `process`: every child: a program started with its arguments, its
  environment and its three places, waited for, or kept as a job; the
  quoting of `set -x` and of a background shell's script.
- `io`: the streams: the shell's own, the pipes between stages, the files
  of redirections, the input `read` reads a byte at a time.
- `term`: the interactive line editor with history and completion, and the
  columns of `help`.
- `os`: the working folder of `cd`, the environment the shell starts from,
  the home folder, this binary's own path, the stop on Ctrl-C.
- `fs` and `path`: the files of redirections and globs, the search of
  `PATH`, the tests of `test`, the demo's folder and links.
- `regex`: the shell's patterns, of globs, `case` and `${VAR%pattern}`,
  translated into regular expressions that match in linear time.
- `maps`: the variables, one taken out by `unset`, and a child's
  environment, the command's assignments merged over the exported ones.
- `lists`: the lines `sorted` keeps once or turns around and `first` cuts.
- `strings` and `numbers`: the text of words, lines and messages, and the
  numbers of `printf`, `shift`, `exit` and the small programs.
- `unicode/segment`: the column of a mistake counted in what a reader sees
  as characters, and the caret under it placed by terminal cells; the
  columns of `jobs`.
- `time`: the pause of `snooze`.

## Its demo

Run from the repository's root with the words of `main.args` and `main.stdin` on its input, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/shell/main.hero -- --demo < rebel/examples/shell/main.stdin

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 13 faces on its `packages` line, each linked here to its reference: [`fs`](https://heroes-lang.org/rebel/fs/), [`io`](https://heroes-lang.org/rebel/io/), [`lists`](https://heroes-lang.org/rebel/lists/), [`maps`](https://heroes-lang.org/rebel/maps/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`process`](https://heroes-lang.org/rebel/process/), [`regex`](https://heroes-lang.org/rebel/regex/), [`strings`](https://heroes-lang.org/rebel/strings/), [`term`](https://heroes-lang.org/rebel/term/), [`time`](https://heroes-lang.org/rebel/time/), [`unicode/segment`](https://heroes-lang.org/rebel/unicode/segment/).

## Its folders

9 folders, 45 files and 9,348 lines, this page aside; of them 41 modules, `.hero` files, of 8,963 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 620 | 1 | 235 |
| `builtins/` | 8 | 1,373 | 8 | 1,373 |
| `demo/` | 2 | 169 | 2 | 169 |
| `diag/` | 1 | 134 | 1 | 134 |
| `exec/` | 10 | 2,643 | 10 | 2,643 |
| `expand/` | 7 | 1,616 | 7 | 1,616 |
| `interactive/` | 2 | 235 | 2 | 235 |
| `syntax/` | 7 | 2,281 | 7 | 2,281 |
| `tools/` | 3 | 277 | 3 | 277 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
